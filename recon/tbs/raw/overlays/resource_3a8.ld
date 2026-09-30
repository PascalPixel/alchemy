/* resource_3a8: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_MACHI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_MACHI/MACHI.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_MACHI/IMPORT.o"(.text)
        "*/resource_3a8_overlay.o"(.rodata.part1)
    }
    .bss : { "*/resource_3a8_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
