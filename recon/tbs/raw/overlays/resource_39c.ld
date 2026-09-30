/* resource_39c: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/EFFECT_RECORD.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/PAIR_BURST.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/MAKYURI/PALETTE_CYCLE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/HEYA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/HEYA2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/HEYA3.o"(.text)
        "*/resource_39c_overlay.o"(.text.x02009db4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/HEYA4.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/HEYA5.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/PARTY_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/HEYA6.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/MAKYURI/LIGHTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/RISE_EFFECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/MAKYURI/FOLLOW_LEADER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/MAKYURI/SPARK_PAIR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_IRIGUCHI/RAISE_PILLAR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/RELEASE_TARGET.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/MAKYURI/ACTOR_MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/MAKYURI/SPAWN_TIMER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/RELEASE_ATTACH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_39c_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helpers by their ABI names; here they
       are the overlay's veneers into the resident IWRAM routines, as in
       MAIN.LD. */
    __divsi3 = IwramSignedDivideEntry;
}
