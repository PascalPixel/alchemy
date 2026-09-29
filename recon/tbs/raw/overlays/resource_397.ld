/* resource_397: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. The scene's work follows the image. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_EDA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_EDA/BACKGROUND_SCROLL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_EDA/BG3_SPLIT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_EDA/IMPORT.o"(.text)
        "*/resource_397_overlay.o"(.rodata)
    }
    .bss ALIGN(16) (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_EDA/BACKGROUND_SCROLL.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
