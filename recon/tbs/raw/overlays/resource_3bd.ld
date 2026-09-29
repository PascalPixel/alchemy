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
        "*/resource_3bd_overlay.o"(.text.x02008a00)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/FADE_BLEND.o"(.text)
        "*/resource_3bd_overlay.o"(.text.x02008abc)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/BLEND_STEPS.o"(.text)
        "*/resource_3bd_overlay.o"(.text.x02008bc8)
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
        "*/resource_3bd_overlay.o"(.text.x0200b598)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/MATCH_PRIORITY.o"(.text)
        "*/resource_3bd_overlay.o"(.text.x0200b644)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/THREE_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/OPEN_WALL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3bd_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
