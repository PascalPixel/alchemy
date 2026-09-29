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
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/GET_ENTRANCES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/SCENE_TABLES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/GET_PLACEMENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/HELPER_CALLS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/GET_EVENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/LEADER_MOTION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/PUSHED_BLOCK_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/SEQUENCES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/FLOOR_SWITCH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/SEQUENCE_A.o"(.text)
        "*/resource_39c_overlay.o"(.text.x02009db4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/PARTICLE_ROWS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/PUSH_PILLAR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/ACTOR_NINE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/COLUMN_PROBE_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/FOUR_STEPS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/BLOCK_FLAGS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/THREE_BRANCH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/FADE_TO_WHITE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/COLUMN_CHOREOGRAPHY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/ROTATE_PALETTE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/FADE_TO_BLACK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/ACTOR_SINK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/RANDOM_EFFECT_SEQUENCE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/SCRIPTED_STEPS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/SEQUENCE_B.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/ENTRY_DROP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/OPEN_STAIR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/SPAWN_BURSTS.o"(.text)
        "*/resource_39c_overlay.o"(.text.x0200bd20)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/RIDE_LIFT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/CAST_PSYNERGY.o"(.text)
        "*/resource_39c_overlay.o"(.text.x0200cfcc)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/RISE_EFFECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/MAKYURI/FOLLOW_LEADER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/MAKYURI/SPARK_PAIR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_IRIGUCHI/RAISE_PILLAR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/RELEASE_TARGET.o"(.text)
        "*/resource_39c_overlay.o"(.text.x0200d5c0)
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
