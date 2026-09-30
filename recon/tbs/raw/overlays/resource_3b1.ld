/* resource_3b1: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/STAGED.o"(.text)
        "*/resource_3b1_overlay.o"(.text.x0200812c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/HEYA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/HEYA2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/IMPORT.o"(.text)
        "*/resource_3b1_overlay.o"(.rodata.part1)
    }
    .bss : { "*/resource_3b1_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
