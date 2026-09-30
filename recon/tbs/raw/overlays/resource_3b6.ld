/* resource_3b6: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/HEYA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/IMPORT.o"(.text)
        "*/resource_3b6_overlay.o"(.rodata.x02008a90)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
