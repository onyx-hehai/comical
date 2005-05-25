
#ifdef __GNUG__
#pragma implementation "rarstrm.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
  #pragma hdrstop
#endif

#if wxUSE_STREAMS

#include "rarstrm.h"

/* This implementation is far from ideal.  This carpenter blames his tool.  The
 * unrar dll interface is awful, and the source code of the library should be
 * used as an example of how NOT to write C++ code.
 * So anyway, ideally this class would only extract data from the RAR when
 * OnSysRead is called, but instead I have to extract the whole file first
 * and then pretend that it's a stream.
 */
wxRarInputStream::wxRarInputStream(const wxString& archive, const wxString& file) : wxInputStream()
{
	HANDLE RarFile;
	int RHCode,PFCode;
	char CmtBuf[16384];
	struct RARHeaderData HeaderData;
	struct RAROpenArchiveDataEx OpenArchiveData;
	char * CallBackBuffer;
	
	m_Pos = 0;
	m_Size = 0;
	m_ArcName = archive;
	
	memset(&OpenArchiveData,0,sizeof(OpenArchiveData));
	OpenArchiveData.ArcName=(char *) archive.c_str();
	OpenArchiveData.CmtBuf=CmtBuf;
	OpenArchiveData.CmtBufSize=sizeof(CmtBuf);
	OpenArchiveData.OpenMode=RAR_OM_EXTRACT;
	RarFile=RAROpenArchiveEx(&OpenArchiveData);

	if ((RHCode=OpenArchiveData.OpenResult)!=0)
	{
		m_lasterror = wxSTREAM_READ_ERROR;
		wxLogError("Could not open " + archive + " for reading.");
		OpenArchiveError(RHCode);
		return;
	}
    
	HeaderData.CmtBuf=NULL;

	while ((RHCode=RARReadHeader(RarFile,&HeaderData))==0)
	{
		if (file.IsSameAs(HeaderData.FileName))
		{
			m_Size = HeaderData.UnpSize;
			break;	
		}
		else
			RARProcessFile(RarFile,RAR_SKIP,NULL,NULL);
	}
	
	if (m_Size == 0) // archived file not found
	{
		m_lasterror = wxSTREAM_READ_ERROR;
		wxLogError(file + " not found in archive " + archive + ".");
	}

	m_Buffer = new char[m_Size];
	CallBackBuffer = m_Buffer;

	RARSetCallback(RarFile, CallbackProc, (long) &CallBackBuffer);

	PFCode = RARProcessFile(RarFile, RAR_TEST, NULL, NULL);

	if (PFCode!=0)
	{
		m_lasterror = wxSTREAM_READ_ERROR;
		ProcessFileError(PFCode);	
	}
	
	if (RarFile)  
		RARCloseArchive(RarFile);

}

wxRarInputStream::~wxRarInputStream()
{
	delete[] m_Buffer;
}

bool wxRarInputStream::Eof() const
{
	wxASSERT_MSG( m_Pos <= (off_t)m_Size, _T("wxRarInputStream: invalid current position") );
	return m_Pos >= (off_t)m_Size;
}

size_t wxRarInputStream::OnSysRead(void *buffer, size_t bufsize)
{
	wxASSERT_MSG( m_Pos <= (off_t)m_Size, _T("wxRarInputStream: invalid current position") );
	if (m_Pos >= (off_t)m_Size)
	{
		m_lasterror = wxSTREAM_EOF;
		return 0;
	}
    
	if (m_Pos + bufsize > m_Size)
		bufsize = m_Size - m_Pos;

	memcpy(buffer, m_Buffer + m_Pos, bufsize);
	
	m_Pos += bufsize;

	return bufsize;
}

off_t wxRarInputStream::OnSysSeek(off_t seek, wxSeekMode mode)
{
    off_t nextpos;

    switch ( mode )
	{
        case wxFromCurrent : nextpos = seek + m_Pos; break;
        case wxFromStart : nextpos = seek; break;
        case wxFromEnd : nextpos = m_Size - 1 + seek; break;
        default : nextpos = m_Pos; break; /* just to fool compiler, never happens */
    }

    m_Pos = nextpos;
    return m_Pos;
}

void wxRarInputStream::OpenArchiveError(int Error)
{
	switch(Error)
	{
		case ERAR_NO_MEMORY:
			wxLogVerbose("ERAR_NO_MEMORY: Not enough memory to open m_ArcName.");
			break;
		case ERAR_EOPEN:
			wxLogVerbose("ERAR_EOPEN: Cannot open " + m_ArcName + ".");
			break;
		case ERAR_BAD_ARCHIVE:
			wxLogVerbose("ERAR_BAD_ARCHIVE: " + m_ArcName + " is not a RAR archive.");
			break;
		case ERAR_BAD_DATA:
			wxLogVerbose("ERAR_BAD_DATA: " + m_ArcName + " archive header broken.");
			break;
		case ERAR_UNKNOWN:
			wxLogVerbose("ERAR_UNKNOWN: Unknown error.");
			break;
	}
}

void wxRarInputStream::ProcessFileError(int Error)
{
	switch(Error)
	{
		case ERAR_UNKNOWN_FORMAT:
			wxLogVerbose("ERAR_UNKNOWN_FORMAT: Unknown archive format.");
			break;
		case ERAR_BAD_ARCHIVE:
			wxLogVerbose("ERAR_BAD_ARCHIVE: Bad volume.");
			break;
		case ERAR_ECREATE:
			wxLogVerbose("ERAR_ECREATE: File create error.");
			break;
		case ERAR_EOPEN:
			wxLogVerbose("ERAR_EOPEN: Volume open error.");
			break;
		case ERAR_ECLOSE:
			wxLogVerbose("ERAR_ECLOSE: File close error.");
			break;
		case ERAR_EREAD:
			wxLogVerbose("ERAR_EREAD: Read error.");
			break;
		case ERAR_EWRITE:
			wxLogVerbose("ERAR_EWRITE: Write error.");
			break;
		case ERAR_BAD_DATA:
			wxLogVerbose("ERAR_BAD_DATA: CRC error.");
			break;
		case ERAR_UNKNOWN:
			wxLogVerbose("ERAR_UNKNOWN: Unknown error.");
			break;
	}
}

int CallbackProc(uint msg, long UserData, long P1, long P2)
{
	char **buffer;
	switch(msg)
	{

	case UCM_CHANGEVOLUME:
		break;
	case UCM_PROCESSDATA:
		buffer = (char **) UserData;
		memcpy(*buffer, (char *)P1, P2);
		// advance the buffer ptr, original m_buffer ptr is untouched
		*buffer += P2;
		break;
	case UCM_NEEDPASSWORD:
		break;
	}
	return(0);
}

#endif