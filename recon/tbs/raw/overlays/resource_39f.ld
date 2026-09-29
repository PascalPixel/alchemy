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
        "*/resource_39f_overlay.o"(.text.x02008cd0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/APPLY.o"(.text)
        "*/resource_39f_overlay.o"(.text.x02008d90)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/PUFF_RING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/SIX_CALL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/GET_ENTRANCES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/SCENE_DATA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/GET_PLACEMENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/PROBED_ACTOR_NINE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/WAYPOINT.o"(.text)
        "*/resource_39f_overlay.o"(.text.x02009244)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/WAYPOINT2.o"(.text)
        "*/resource_39f_overlay.o"(.text.x02009454)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/CROSS_RECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/BRANCH_HOP_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/BEATS.o"(.text)
        "*/resource_39f_overlay.o"(.text.x020099b8)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/SEQUENCE.o"(.text)
        "*/resource_39f_overlay.o"(.text.x02009d04)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/SEQUENCE2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/CLOSE_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/GET_EVENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/SCENE_ENTRY_HOOK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/ORBIT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_39f_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = IwramSignedDivideEntry;
}
