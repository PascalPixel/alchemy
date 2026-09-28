/* resource_383: its listing with maintained code
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_383_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/GET_TABLE.o"(.text)
        "*/resource_383_overlay.o"(.text.x02008048)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/SET_SCENE.o"(.text)
        "*/resource_383_overlay.o"(.text.x0200813c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/RUN_ACTOR.o"(.text)
        "*/resource_383_overlay.o"(.text.x02008310)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/RUN_ACTOR_MODE.o"(.text)
        "*/resource_383_overlay.o"(.text.x02008690)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/RUN_ACTOR10.o"(.text)
        "*/resource_383_overlay.o"(.text.x0200882c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/RUN_SCENE383.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/ROD_HELP.o"(.text)
        "*/resource_383_overlay.o"(.text.x02009348)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/SET_VALUE123.o"(.text)
        "*/resource_383_overlay.o"(.text.x020099e4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/RUN_OPENING.o"(.text)
        "*/resource_383_overlay.o"(.text.x02009ba0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/RUN_STEPS107.o"(.text)
        "*/resource_383_overlay.o"(.text.x02009e80)
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
        "*/resource_383_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
