/* resource_3b3: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/OBJ_SETUP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/OBJECT_CONFIG.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/resource_3b3_overlay.o"(.text.x0200813c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/resource_3b3_overlay.o"(.text.x02008cc0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/SETTLE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/PROBE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/PLACE_QUERY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/READ_MAP_CELL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/CELL_ATTRIBUTES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/LOWER_ACTOR_TO_LEDGE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/SEQUENCES.o"(.text)
        "*/resource_3b3_overlay.o"(.text.x02009440)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/SCENE_TABLES.o"(.text)
        "*/resource_3b3_overlay.o"(.text.x020094c4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/PLACEMENT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/SCROLL_JITTER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/DISPLAY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/FRONT_BEHIND.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/FRONT_AHEAD.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/PRIORITY_UPDATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/SORT_PILLARS.o"(.text)
        "*/resource_3b3_overlay.o"(.text.x02009d84)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/SINGLE_STEP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/STAGED_TRIGGER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/ACTOR_STEPS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/RAISE_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/PARTICLES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/MAP_SHIFT_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/PLACE_TEN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/DROP_ACTOR_TEN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/PUSH_SCENE.o"(.text)
        "*/resource_3b3_overlay.o"(.text.x0200a580)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/PREP_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/CALLBACKS.o"(.text)
        "*/resource_3b3_overlay.o"(.text.x0200a74c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3b3_overlay.o"(.rodata.part1)
    }
    .bss : { "*/resource_3b3_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
