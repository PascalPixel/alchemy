/* resource_394: its listing with maintained code
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_394_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/PLACE_OBJECTS.o"(.text)
        "*/resource_394_overlay.o"(.text.x02008b3c)
        "*/resource_394_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
