NASM=nasm
NASMFLAGS=-f elf
LDFLAGS=-Tlinker.ld -melf_i386 -nostartfiles -nostdlib
OBJECT_FILES=bios.o

.PHONY: all
all: bios.bin

%.o : %.asm
		$(NASM) $(NASMFLAGS) $< -o $@

bios.bin : bios.o
		$(LD) $(LDFLAGS) $^ -o $@
		cp bios.bin ../

.PHONY: clean
clean:
		rm -rf bios.bin $(OBJECT_FILES)
		rm -rf ../bios.bin