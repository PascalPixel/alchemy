/* resource_395: its listing with maintained code
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_395_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_KI/GESTURE.o"(.text)
        "*/resource_395_overlay.o"(.text.x0200952c)
        "*/resource_395_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
