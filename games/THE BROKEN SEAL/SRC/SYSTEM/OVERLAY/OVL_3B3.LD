EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/HASHIRA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text.effect_spawn)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/MAP/PANEL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/PROBE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/LEDGE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/PRIORITY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/PILLAR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3b3_overlay.o"(.rodata.x0200abf8)
    }
    .bss : { "*/resource_3b3_overlay.o"(.bss) }
    /DISCARD/ : { "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
