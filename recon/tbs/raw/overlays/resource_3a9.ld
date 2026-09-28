/* resource_3a9: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_HEYA/ENTRY.o"(.text)
        "*/resource_3a9_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_HEYA/RETURN_ZERO.o"(.text)
        "*/resource_3a9_overlay.o"(.text.x0200807c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_HEYA/ARRIVAL.o"(.text)
        "*/resource_3a9_overlay.o"(.text.x02008308)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_HEYA/CLEAR_SLOTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_HEYA/IMPORT.o"(.text)
        "*/resource_3a9_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
