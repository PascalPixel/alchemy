OUTPUT_ARCH(arm)
MEMORY { ROM (rx) : ORIGIN = 0x8000000, LENGTH = 32M }
SECTIONS
{
    .rom : { "*/recon/tbs/de/rom.o"(.rom) } > ROM
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
