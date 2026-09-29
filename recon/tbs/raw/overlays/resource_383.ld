/* resource_383: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/GET_TABLE.o"(.text)
        "*/resource_383_overlay.o"(.text.x02008048)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/SET_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/COUNTER_TALK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/RUN_ACTOR.o"(.text)
        "*/resource_383_overlay.o"(.text.x02008310)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/RUN_ACTOR_MODE.o"(.text)
        "*/resource_383_overlay.o"(.text.x02008690)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/RUN_ACTOR10.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/THANKS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/RUN_SCENE383.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/ROD_HELP.o"(.text)
        "*/resource_383_overlay.o"(.text.x02009348)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/SET_VALUE123.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/OPENING_EXIT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/RUN_OPENING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/SUSPICIOUS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/RUN_STEPS107.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/PAIR_RETURNS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/SELECT_ACTOR_PAIR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/SCENE_021C8.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/DIALOGUE_POSE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/UPDATE_ANIMATION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/ENTRY_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/FACE_ACTORS24.o"(.text)
        "*/resource_383_overlay.o"(.text.x0200adb4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/PROMPT.o"(.text)
        "*/resource_383_overlay.o"(.text.x0200af98)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/RUN_EVENT.o"(.text)
        "*/resource_383_overlay.o"(.text.x0200b6f8)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/SET_WORD1C0.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/NEAREST_STOP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/RECORD_NEAR_POINT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/ACTOR_NEAR_POINT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/CHECK_TILE.o"(.text)
        "*/resource_383_overlay.o"(.text.x0200c8c8)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/IMPORT.o"(.text)
        "*/resource_383_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
