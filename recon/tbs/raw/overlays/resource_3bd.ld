/* resource_3bd: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/SCENE_TABLES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/GET_PLACEMENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/FADE_BLEND.o"(.text)
        "*/resource_3bd_overlay.o"(.text.x02008abc)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/BLEND_STEPS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/ROOM_VISUALS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/FLAG200_SETUP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/RESPAWN_ACTOR_OBJECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/SETTLE_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/ACTOR_ELEVEN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/ACTOR_HOP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/GUARDED_STEP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/SCALE_PULSE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/STATE_MACHINE.o"(.text)
        "*/resource_3bd_overlay.o"(.text.x02008f94)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/ACTOR_PRESENTATION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/CUTSCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/ROCK_ORDER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/ACTOR_EIGHT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/GET_EVENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/MATCH_PRIORITY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/ENTRY_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/THREE_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/OPEN_WALL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3bd_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
