/* resource_397: its listing with maintained code linked in place, as pret
   links objects in ROM order. The scene's work follows the image. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_397_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_EDA/BACKGROUND_SCROLL.o"(.text)
        "*/resource_397_overlay.o"(.text.x020082e0)
        "*/resource_397_overlay.o"(.rodata)
    }
    .bss ALIGN(16) (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_EDA/BACKGROUND_SCROLL.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
