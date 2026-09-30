/* resource_39f: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/SPAWNED.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/SLOT_STEP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/SLOT_MOVE_STEP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/APPLY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/SLIDE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/PUFF_RING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/MORI.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/PROBED_ACTOR_NINE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/WAYPOINT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/ACTOR_ELEVEN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/WAYPOINT2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/PROBED_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/MORI2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/PROBED_LANDING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/SEQUENCE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/SEQUENCE_A.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/SEQUENCE2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/CLOSE_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/MORI3.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_39f_overlay.o"(.rodata.x0200aef8)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = IwramSignedDivideEntry;
}
