/* resource_3c8: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/OBJECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/HEYA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/HEYA5.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/HEYA6.o"(.text)
        "*/resource_3c8_overlay.o"(.text.x0200a47c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/HEYA2.o"(.text)
        "*/resource_3c8_overlay.o"(.text.x0200ab14)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/HEYA3.o"(.text)
        "*/resource_3c8_overlay.o"(.text.x0200c2bc)
        "*/resource_3c8_overlay.o"(.text.x0200c520)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/HEYA4.o"(.text)
        "*/resource_3c8_overlay.o"(.text.x0200cbd8)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3c8_overlay.o"(.rodata.x0200d040)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = IwramSignedDivideEntry;
}
