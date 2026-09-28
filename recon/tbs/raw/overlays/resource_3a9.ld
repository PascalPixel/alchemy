/* resource_3a9: its listing with maintained code
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_3a9_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_HEYA/RETURN_ZERO.o"(.text)
        "*/resource_3a9_overlay.o"(.text.x0200807c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_HEYA/ARRIVAL.o"(.text)
        "*/resource_3a9_overlay.o"(.text.x02008308)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_HEYA/CLEAR_SLOTS.o"(.text)
        "*/resource_3a9_overlay.o"(.text.x020083e8)
        "*/resource_3a9_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
