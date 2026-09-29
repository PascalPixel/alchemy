/* resource_3b5: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_MACHI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_MACHI/EFFECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_MACHI/DISTANCE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_MACHI/PROXIMITY.o"(.text)
        "*/resource_3b5_overlay.o"(.text.x020082f0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_MACHI/EVENTS.o"(.text)
        "*/resource_3b5_overlay.o"(.text.x02008528)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_MACHI/SETUP.o"(.text)
        "*/resource_3b5_overlay.o"(.text.x02008980)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_MACHI/TILES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_MACHI/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3b5_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
