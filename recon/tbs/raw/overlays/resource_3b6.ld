/* resource_3b6: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/FACING_EVENTS.o"(.text)
        "*/resource_3b6_overlay.o"(.text.x02008500)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/ACTION.o"(.text)
        "*/resource_3b6_overlay.o"(.text.x020086ec)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/CHEF.o"(.text)
        "*/resource_3b6_overlay.o"(.text.x02008760)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/FLAGGED.o"(.text)
        "*/resource_3b6_overlay.o"(.text.x020088cc)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/MESSAGE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/IMPORT.o"(.text)
        "*/resource_3b6_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
