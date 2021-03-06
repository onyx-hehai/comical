LDFLAGS = `wx-config --libs` -lz -ljpeg
ifeq ($(DEBUG),1)
LDFLAGS += -g -lm -lstdc++
endif
INSTALL = install
INSTALL_PROGRAM = $(INSTALL)
prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin

.SUFFIXES: .cpp .o .a

OBJS = $(patsubst %.cpp,%.o,$(wildcard src/*.cpp))

all: comical

comical: sources unrar/libunrar.a unzip/libminiunzip.a
	$(CC) -o $@ $(OBJS) unrar/libunrar.a unzip/libminiunzip.a $(LDFLAGS)

sources:
	$(MAKE) -C src

unrar/libunrar.a:
	$(MAKE) lib -C unrar -f makefile.linux

unzip/libminiunzip.a:
	$(MAKE) -C unzip

install: comical
	$(INSTALL_PROGRAM) comical $(DESTDIR)$(bindir)/comical

install-strip:
	$(MAKE) INSTALL_PROGRAM='$(INSTALL_PROGRAM) -s' install

uninstall:
	rm -f $(DESTDIR)$(bindir)/comical

clean:
	$(MAKE) clean -C src
	rm -f comical

distclean:
	$(MAKE) clean
	$(MAKE) clean -C unrar -f makefile.linux
	$(MAKE) clean -C unzip
	rm -f $(DEPS) bin2h src/bin2h.o

.PHONY : source install install-strip uninstall clean distclean
