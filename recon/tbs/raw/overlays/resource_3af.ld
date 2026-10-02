/* resource_3af: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. The overlay's own work follows the image. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/UPDATE_SCALE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/KANPAN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/KANPAN2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/KANPAN3.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/IMPORT.o"(.text)
        "*/resource_3af_overlay.o"(.rodata.x0200c464)
    }
    .bss ALIGN(16) (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/KANPAN.o"(.data .bss COMMON)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/KANPAN3.o"(COMMON)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/KANPAN2.o"(COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
