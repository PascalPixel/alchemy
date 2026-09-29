/* resource_3af: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. The overlay's own work follows the image. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/UPDATE_SCALE.o"(.text)
        "*/resource_3af_overlay.o"(.text.x020080c4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/OSCILLATE_HEIGHT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/FLY_BY_21.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/FLY_BY_22.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/SET_WORD28.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/IDLE_TURN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/GET_TABLEC994.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/GET_PLACEMENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/GET_EVENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/RUN_OPENING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/RUN_SCENE3AF.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/DECK_STATE_EVENT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/PRESENTATION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/MULTI_ENCOUNTER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/RESET_DECK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/ENTRY_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/RANDOM_ACTORS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/PLACE_ACTORS20.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/DECK_ACTORS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/RUN_2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/DECK_EVENT_SCRIPT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/DECK_CREW_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/DECK_SEQ.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/JUMP_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/CONFIGURE_LEAD.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/PARTY_ROSTER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/FIND_FIRST.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/ROCK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/INIT_SLOTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/ACTIVATE_SLOT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/SLOT_MODES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/SELECT_TABLE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/SLOT_BANDS.o"(.text)
        "*/resource_3af_overlay.o"(.text.x0200b5ec)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/INIT_ACTOR_SLOTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/APPLY_FIVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/TOLBI_ARRIVAL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/RUN_THREE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/ROBIN_TALK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/RUN_3.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/IMPORT.o"(.text)
        "*/resource_3af_overlay.o"(.rodata)
    }
    .bss ALIGN(16) (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/ROCK.o"(.data .bss COMMON)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/SLOT_MODES.o"(COMMON)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/INIT_SLOTS.o"(COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
