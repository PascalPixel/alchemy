/* resource_39d: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/OBJECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/TABLES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/ENTRY_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/PRESENT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/DISTANCE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/NEAREST.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/INITIALIZE_MOTION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/FORMATION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/SCENE58.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/SCENE59.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/SINK_PAIR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/RISE_PAIR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/COLLECT_SLOTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/LOWER_BAND.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/RAISE_BAND.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/ARC.o"(.text)
        "*/resource_39d_overlay.o"(.text.x0200b4bc)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_39d_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = IwramSignedDivideEntry;
}
