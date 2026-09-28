/* resource_3b3: its listing with maintained code and the compiler library linked in place,
   as pret links objects and libgcc members in ROM order. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_3b3_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/resource_3b3_overlay.o"(.text.x0200813c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/resource_3b3_overlay.o"(.text.x02008cc0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/PROBE.o"(.text)
        "*/resource_3b3_overlay.o"(.text.x02008ec8)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/READ_MAP_CELL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/CELL_ATTRIBUTES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/LOWER_ACTOR_TO_LEDGE.o"(.text)
        "*/resource_3b3_overlay.o"(.text.x02009174)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/SCROLL_JITTER.o"(.text)
        "*/resource_3b3_overlay.o"(.text.x020095cc)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/FRONT_BEHIND.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/FRONT_AHEAD.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/PRIORITY_UPDATE.o"(.text)
        "*/resource_3b3_overlay.o"(.text.x02009be8)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/STAGED_TRIGGER.o"(.text)
        "*/resource_3b3_overlay.o"(.text.x0200a080)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/RAISE_ACTOR.o"(.text)
        "*/resource_3b3_overlay.o"(.text.x0200a230)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/MAP_SHIFT_SCENE.o"(.text)
        "*/resource_3b3_overlay.o"(.text.x0200a498)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/DROP_ACTOR_TEN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/PUSH_SCENE.o"(.text)
        "*/resource_3b3_overlay.o"(.text.x0200a580)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/PREP_ACTOR.o"(.text)
        "*/resource_3b3_overlay.o"(.text.x0200a63c)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3b3_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
