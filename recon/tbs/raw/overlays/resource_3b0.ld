/* resource_3b0: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. The overlay's own work follows the image. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/HOBASHIRA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/IMPORT.o"(.text)
        "*/resource_3b0_overlay.o"(.rodata)
    }
    .bss ALIGN(16) (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/HOBASHIRA.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
