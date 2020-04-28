# --------------------------------------------------------------------------
# Einstellungen in Abhaengigkeit vom Generierungssystem:
#
#    ASMOBJFORMAT: Ausgabeformat fuer den Assembler. Das Format muss dem
#                  jeweiligen Format des verwendeten C++ Compilers angepasst
#                  werde, damit gemischte Objektdateien gelinkt werden koennen.
#    OBJDIR:       Verzeichnispräfix, in dem die Objektdateien abgelegt werden
#                  sollen.
#    DEPDIR:       Verzeichnispräfix, in dem die Abhaengigkeitsdateien abgelegt
#                  werden sollen
#    ISODIR:       Verzeichnispräfix, in dem die Daten für das bootfähige
#                  Medium angelegt werden sollen
#    NOOPTTAG:     Endung, welche an die Verzeichnisse für Objekt- und
#                  Abhängigkeitsdateien bei Debugversionen angehängt wird
#    VERBOSETAG:   Endung ähnlich NOOPTTAG, nur für Builds mit Verboseausgaben
#    ASM:          Zu benutzender Assembler
#    CC/CXX:       Zu benutzender C/C++-Compiler
#    AR:           Zu benutzender Archivierer
#    CFLAGS:       Flags fuer den C-Compileraufruf
#    CXXFLAGS:     Flags fuer den C++-Compileraufruf
#    LD:           Zu benutzender Linker
#    LDFLAGS:      Flags fuer den Linkeraufruf
#    LDLIBS:       Dazuzulinkende Bibliotheken
#    LDHEAD:       Am Anfang zu linkende Dateien
#    LDTAIL:       Am Ende zu linkende Dateien

VERBOSE = @
ASMOBJFORMAT = elf
OBJDIR = ./build
DEPDIR = ./dep
NOOPTTAG = -noopt
DBGTAG = -dbg
VERBOSETAG = -verbose
ASM = nasm
CXX = g++
LD = ld
MKISO = grub-mkrescue
QEMU = qemu-system-i386

PROJECT="MPStuBS"

WARNFLAGS = -Wall -Wextra -Werror -Wno-error=unused-parameter
OPTFLAGS = -O3 -fomit-frame-pointer
FLOATINGPOINT = -mno-mmx -mno-sse
STANDALONEFLAGS = -ffreestanding -fno-builtin -nodefaultlibs -nostdlib -nostdinc

ifeq ($(CXX),g++)
	STANDALONEFLAGS += -fno-tree-loop-distribute-patterns -nostartfiles
	WARNFLAGS += -Wstack-usage=1024 -Wno-error=stack-usage=
else ifeq ($(CXX),clang++)
	WARNFLAGS += -Wno-error=unused-private-field -Wno-implicit-exception-spec-mismatch  -Wno-error=unused-const-variable
endif
CXXFLAGS = -std=c++11 -m32 -I. $(OPTFLAGS) $(WARNFLAGS) $(STANDALONEFLAGS) $(FLOATINGPOINT) -Wno-non-virtual-dtor -fno-rtti -fno-exceptions -Wno-write-strings -fno-stack-protector -mno-red-zone -g -gdwarf-2
ASMFLAGS = -f $(ASMOBJFORMAT)
LDFLAGS = -melf_i386
LDHEAD = $(shell $(CXX) -m32 --print-file-name=crti.o && $(CXX) -m32 --print-file-name=crtbegin.o)
LDTAIL = $(shell $(CXX) -m32 --print-file-name=crtend.o && $(CXX) -m32 --print-file-name=crtn.o)
AR = ar
QEMUCPUS = 4
QEMUWAIT =
QEMUFLAGS = -k en-us -d guest_errors $(QEMUWAIT)
QEMUREMOTEGDB = -gdb tcp::1234
QEMUDBGFLAGS = -no-shutdown -no-reboot
KVMFLAGS = -enable-kvm -cpu host
SERIALPTY = -serial pty
SERIALSTDIO = -serial mon:stdio
INITRD = /dev/null

ISODIR = ./build-iso
ISOBOOTDIR = /boot
ISOGRUBCFG = $(ISOBOOTDIR)/grub/grub.cfg
ISOKERNEL = $(ISOBOOTDIR)/kernel
GRUBTIMEOUT = 2
GRUBBIN = /usr/lib/grub/i386-pc
DD = dd
XORRISO = xorriso

# Namen der Unterverzeichnisse mit den Quelltexten
VPATH = $(sort $(dir $(STARTUP_SOURCE) $(CC_SOURCES) $(ASM_SOURCES)))

# -------------------------------------------------------------------------
# Listen mit den Objektdateien, die beim Kompilieren entstehen:
# (Die Eingabevariablen werden vom eigentlichen Makefile zwar erst spaeter
# definiert, sind aber zum Auswertezeitpunkt *dieser* Variablen schon
# verfuegbar.)
STARTUP_OBJECT = $(addprefix $(OBJDIR)/,$(patsubst %.asm,_%.o, $(notdir $(STARTUP_SOURCE))))
CC_OBJECTS = $(notdir $(CC_SOURCES:.cc=.o))
DEP_FILES = $(patsubst %.o,$(DEPDIR)/%.d,$(CC_OBJECTS))
ASM_OBJECTS = $(patsubst %.asm,_%.o, $(notdir $(ASM_SOURCES)))
OBJPRE = $(addprefix $(OBJDIR)/,$(ASM_OBJECTS) $(CC_OBJECTS))

# --------------------------------------------------------------------------
# Standardrezept zur Erzeugung der Abhaengigkeitsdateien
$(DEPDIR)/%.d : %.cc $(MAKEFILE_LIST)
	@echo "DEP		$@"
	@if test \( ! \( -d $(@D) \) \) ;then mkdir -p $(@D);fi
	$(VERBOSE) $(CXX) $(CXXFLAGS) -MM -MT $(OBJDIR)/$*.o -MF $@ $<

# --------------------------------------------------------------------------
# Standardrezept zur Erzeugung von Objektdateien
$(OBJDIR)/%.o : %.cc $(MAKEFILE_LIST)
	@echo "CXX		$@"
	@if test \( ! \( -d $(@D) \) \) ;then mkdir -p $(@D);fi
	$(VERBOSE) $(CXX) -c $(CXXFLAGS) -o $@ $<

$(OBJDIR)/_%.o : %.asm $(MAKEFILE_LIST)
	@echo "ASM		$@"
	@if test \( ! \( -d $(@D) \) \) ;then mkdir -p $(@D);fi
	$(VERBOSE) $(ASM) $(ASMFLAGS) -o $@ $<

# --------------------------------------------------------------------------
# Standardrezept 'clean' loescht das generierte System, die Objektdateien und
# die Abhaengigkeitsdateien

clean:
	@echo "RM		$(OBJDIR)"
	$(VERBOSE) rm -rf "$(OBJDIR)"
	$(VERBOSE) rm -rf "$(OBJDIR)$(NOOPTTAG)"
	$(VERBOSE) rm -rf "$(OBJDIR)$(VERBOSETAG)"
	$(VERBOSE) rm -rf "$(OBJDIR)$(DBGTAG)"
	@echo "RM		$(DEPDIR)"
	$(VERBOSE) rm -rf "$(DEPDIR)"
	$(VERBOSE) rm -rf "$(DEPDIR)$(NOOPTTAG)"
	$(VERBOSE) rm -rf "$(DEPDIR)$(VERBOSETAG)"
	$(VERBOSE) rm -rf "$(DEPDIR)$(DBGTAG)"
	@echo "RM		$(ISODIR)"
	$(VERBOSE) rm -rf "$(ISODIR)"
	$(VERBOSE) rm -rf "$(ISODIR)$(NOOPTTAG)"
	$(VERBOSE) rm -rf "$(ISODIR)$(VERBOSETAG)"
	$(VERBOSE) rm -rf "$(ISODIR)$(DBGTAG)"

# --------------------------------------------------------------------------
# Rezept fuer rekursiven Aufruf mit deaktivierten Optimierungen
%-noopt:
	$(VERBOSE) make OBJDIR="$(OBJDIR)$(NOOPTTAG)" DEPDIR="$(DEPDIR)$(NOOPTTAG)" ISODIR="$(ISODIR)$(NOOPTTAG)" OPTFLAGS="-O0" $*

# --------------------------------------------------------------------------
# Rezept fuer rekursiven Aufruf mit erweiterten Ausgaben
%-verbose:
	$(VERBOSE) make OBJDIR="$(OBJDIR)$(VERBOSETAG)" DEPDIR="$(DEPDIR)$(VERBOSETAG)" ISODIR="$(ISODIR)$(VERBOSETAG)" OPTFLAGS="-DVERBOSE" $*

# --------------------------------------------------------------------------
# Rezept fuer rekursiven Aufruf mit besserem Debugging

%-dbg:
	$(VERBOSE) make OBJDIR="$(OBJDIR)$(DBGTAG)" DEPDIR="$(DEPDIR)$(DBGTAG)" ISODIR="$(ISODIR)$(DBGTAG)" OPTFLAGS="-Og -fno-omit-frame-pointer" $*

# --------------------------------------------------------------------------
# Standardrezepte zum Ausfuehren und Debuggen
# --------------------------------------------------------------------------

# --------------------------------------------------------------------------
# Rezept fuer rekursiven Aufruf und warten auf GDB
# Sinnvoll anwendbar mit den Targets: qemu, kvm, *-curses, *-serial
%-gdb:
	$(VERBOSE) make QEMUWAIT="-S" $*

# --------------------------------------------------------------------------
# fuer die lokale Benutzung (mit X), von SSH (ohne -X/Y) nicht benutzbar
# qemu: X-Window mit Serial auf Stdio, Debugger optional auf tcp:1234, Virtualisierung ohne KVM
# kvm:  X-Window mit Serial auf Stdio, Debugger optional auf tcp:1234, Virtualisierung mit KVM
# gdb:  X-Window mit Serial auf /dev/pty, der GDB-Debugger wird gestartet und verbindet sich mit dem System

qemu: all
	$(QEMU) -kernel $(KERNEL) -initrd $(INITRD) -smp $(QEMUCPUS) $(QEMUFLAGS) $(SERIALPTY) $(QEMUREMOTEGDB)

kvm: all
	$(QEMU) $(KVMFLAGS) -kernel $(KERNEL) -initrd $(INITRD) -smp $(QEMUCPUS) $(QEMUFLAGS) $(SERIALPTY) $(QEMUREMOTEGDB)

gdb: all
	gdb $(KERNEL) \
		-ex "set arch i386:intel" \
		-ex "target remote | exec $(QEMU) -gdb stdio -kernel $(KERNEL) -initrd $(INITRD) -smp $(QEMUCPUS) -S $(QEMUFLAGS) $(QEMUDBGFLAGS) $(SERIALPTY)"

# um sich mit GDB auf dem Standardport zu verbinden
connect-gdb:
	gdb -ex "target remote localhost:1234" $(KERNEL)

# --------------------------------------------------------------------------
# fuer die remote Benutzung (ohne X), von SSH aus
# curses: startet qemu/kvm mit einem curses Interface, Serial auf /dev/pty (Auf den Monitor mit ALT-2 wechseln)
# serial: startet qemu/kvm vollständig ohne CGA-Screen, Serial auf stdio (Auf den Monitor mit STRG-A C wechseln)
#
# curses und serial jeweils mit qemu und kvm geprefixt werden

QEMUCURSES = $(QEMU) -curses -kernel $(KERNEL) -initrd $(INITRD) -smp $(QEMUCPUS) $(QEMUFLAGS) $(SERIALPTY) $(QEMUREMOTEGDB)
QEMUSERIAL = $(QEMU) -nographic -kernel $(KERNEL) -initrd $(INITRD) -smp $(QEMUCPUS) $(QEMUFLAGS) $(SERIALSTDIO) $(QEMUREMOTEGDB)

qemu-curses: all
	$(QEMUCURSES)

kvm-curses: all
	$(QEMUCURSES) $(KVMFLAGS)

qemu-serial: all
	$(QEMUSERIAL)

kvm-serial: all
	$(QEMUSERIAL) $(KVMFLAGS)

# --------------------------------------------------------------------------
# 'iso' erstellt ein bootfähige Abbild, welches auf ein externes Medium (wie
# CD oder [etwas moderner] USB übertragen werden kann. Dazu wird neben eurem
# Kernel auch der Bootloader Grub ( https://www.gnu.org/software/grub/ )
# installiert.
# Mit dem Target 'qemu-iso' kann dieses Medium getestet werden.
# Sofern ein USB-Massenspeicher eingesteckt ist, z.B. als /dev/sdc , so wird
# durch das Target 'usb-sdc' ein bootfähiges Live-System erstellt - ihr braucht
# dafür jedoch Rootzugriff.
# Alternativ könnt ihr die ISO-Datei einfach auf CD brennen.

iso: $(ISOFILE)

$(ISODIR)$(ISOGRUBCFG):
	@echo "GEN		$@"
	@mkdir -p $(dir $@)
	@/bin/echo -e "set timeout=$(GRUBTIMEOUT)\nset default=0\n\nmenuentry \"$(shell id -un)s $(PROJECT)\" {\n\tmultiboot $(ISOKERNEL)\n\tboot\n}" > $@

$(ISODIR)$(ISOKERNEL): all
	@echo "CPY		$@"
	@mkdir -p $(dir $@)
	@cp -a $(KERNEL) $@

$(ISOFILE): $(ISODIR)$(ISOKERNEL) $(ISODIR)$(ISOGRUBCFG)
	@echo "ISO		$@"
	@which $(XORRISO) >/dev/null || echo "Xorriso wurde nicht gefunden - sofern das erstellen der ISO fehl schlägt, kann dies eine der Ursachen sein!" >&2
	$(VERBOSE) $(MKISO) -d $(GRUBBIN) -o $@ $(ISODIR)

qemu-iso: $(ISOFILE)
	$(QEMU) -cdrom $< -smp $(QEMUCPUS) $(QEMUFLAGS)

usb: $(ISOFILE)
ifeq (,$(USBDEV))
	@echo "Benötigt Umgebungsvariable USBDEV mit Pfad zum USB-Speichergerät:" >&2
	@lsblk -o TYPE,KNAME,SIZE,MODEL -a -p | grep "^disk" | cut -b 6-
	@exit 1
else
	$(VERBOSE) $(DD) if=$< of=$(USBDEV) bs=4M status=progress && sync
endif

usb-%:
	@$(MAKE) USBDEV=/dev/$* usb

cd: $(ISOFILE)
ifeq ($(CDRWDEV),)
	@echo "Benötigt Umgebungsvariable CDRWDEV mit Pfad zum CD/DVD Brenner" >&2
	@exit 1
else
	$(VERBOSE) $(XORRISO) -as cdrecord -v dev=$(CDRWDEV) -dao $<
endif

cd-%:
	@$(MAKE) CDRWDEV=/dev/$* cd

# --------------------------------------------------------------------------
# Das Image wirds ins Verzeichnis des tftp-servers kopiert, um es ueber
# pxelinux auf den Testrechnern starten zu können.

# --------------------------------------------------------------------------
# 'help' gibt - wenig überraschend - eine kurze Hilfe über die Targets aus
help:
	@/bin/echo -e "                              \e[1mM A K E F I L E\e[0m\n\n" \
		"\e[1mzum Uebungsbetriebssystem $(PROJECT)" \
		"der Lehrveranstaltung Betriebssysteme.\e[0m\n\n" \
		"Durch Eingabe von '\e[4mmake\e[0m' werden die Quelltexte des Systems kompiliert.\n" \
		"Es gibt Targets in jeweils zwei Varianten, \e[2;3m<name>\e[0m und \e[2;3m<name>\e[0;3m-noopt\e[0m.\n" \
		"Targets mit dem Suffix \e[3m-noopt\e[0m werden ohne Optimierungen kompiliert. Dies\n" \
		"kann sich z.B. für das Debugging mit \e[4mgdb\e[0m eignen.\n\n" \
		"Die folgenden Targets sind verfügbar (jeweils auch mit \e[3m-noopt\e[0m).\n" \
		"Targets zum Bauen:\n\n" \
		"	\e[3mall\e[0m      Das System wird gebaut und liegt anschließend als ELF-Binary vor.\n\n" \
		"	\e[3miso\e[0m      Erstellt ein bootfähiges Abbild für den Start auf einem\n" \
		"	         Testrechner mittels externem Datenträger (Datei: $(ISOFILE))\n\n" \
		"Targets zum lokalen Ausführen:\n\n" \
		"	\e[3mqemu\e[0m     QEMU wird gestartet und führt euer System aus. Da QEMU langsamer\n" \
		"	         ist als KVM, treten hier u.U. andere Fehler hervor als in KVM oder\n" \
		"	         auf der Hardware.\n\n" \
		"	\e[3mkvm\e[0m      KVM wird gestartet und führt euer System in einer\n" \
		"	         Hardware-Beschleunigten virtuellen Maschine aus.\n\n" \
		"	\e[3mgdb\e[0m      QEMU wird mit aktiviertem GDB-Stub gestartet und eine GDB-Instanz\n" \
		"	         angehängt. Damit lässt sich euer System Schritt für Schritt\n" \
		"	         debuggen\n\n" \
		"	\e[3mqemu-iso\e[0m Startet das System in QEMU über ein virtuelles CD Laufwerk\n\n" \
		"Targets zum entfernten Ausführen:\n\n" \
		"	\e[3mqemu-curses\e[0m QEMU wird gestartet und führt euer System aus. Die CGA-Ausgabe\n" \
		"	            erfolgt dabei nicht in einem X-Window, sondern direkt im Terminal\n" \
		"	            über ncurses. Um das System zu beenden, könnt ihr mit ALT-2 in\n" \
		"	            den QEMU-Monitor wechseln und dann mit 'quit' beenden.\n\n" \
		"	\e[3mkvm-curses\e[0m  Genauso wie 'qemu-curses' nur mit KVM statt QEMU.\n\n" \
		"	\e[3mqemu-serial\e[0m QEMU wird gestartet und führt euer System aus. Die CGA-Ausgabe\n" \
		"	            ist abgestellt und nur die Ein- und Ausgabe über den seriallen\n" \
		"	            Port ist auf stdin/stdout umgeleitet. Dieses Target ist nur mit\n" \
		"	            den optionalen Zusatzaufgaben sinnvoll. Um das System zu beenden,\n" \
		"	            könnt ihr mit STRG-A C in den QEMU-Monitor wechseln und dann mit\n" \
		"	            'quit' beenden.\n\n" \
		"	\e[3mkvm-serial\e[0m  Genauso wie 'qemu-serial' nur mit KVM statt QEMU.\n\n" \
		"Sonstige Targets:\n\n" \
		"	\e[3mconnect-gdb\e[0m Verbindet einen GDB mit einem der mit den obigen Targets\n" \
		"	            gestarteten Systeme. Konkret müsst ihr dazu auf ein anderes\n" \
		"	            Terminal gehen und mit z.B. 'make qemu' ein System startet. Auf\n" \
		"	            dem aktuellen Terminal könnt ihr dann mit 'make connect-gdb' den\n" \
		"	            GDB anhängen. Kompatible Targets sind: 'qemu', 'kvm', '*-curses',\n" \
		"	            '*-serial'. Von alleine warten die Targets nicht auf GDB, dazu\n" \
		"	            müsst ihr '-gdb' an das Target anhängen, also z.B.\n" \
		"	            'make qemu-gdb' zusammen mit 'make connect-gdb'.\n\n" \
		"	\e[3musb\e[0m         Erstellt ein bootfähiges System auf den mit der Umgebungsvariable\n" \
		"	            \e[4mUSBDEV\e[0m bestimmten USB Datenträger\n\n" \
		"	\e[3mcd\e[0m          Erstellt eine bootfähige CD auf den mit der Umgebungsvariable\n" \
		"	            \e[4mCDRWDEV\e[0m bestimmten CD-Brenner\n\n" \
		""

# --------------------------------------------------------------------------
# Einbindung der Abhaengigkeitsdateien

ifneq ($(MAKECMDGOALS),clean)
-include $(DEP_FILES)
endif

.PHONY: all clean qemu kvm help qemu-curses kvm-curses qemu-serial kvm-serial
