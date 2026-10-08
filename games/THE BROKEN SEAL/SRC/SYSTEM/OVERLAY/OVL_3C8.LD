EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/OBJECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text.effect_spawn)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/HEYA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/PUSH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/BLOCKS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/BRIDGE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/EVENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/PARTICLE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3c8_overlay.o"(.rodata.x0200d040)
    }
    /DISCARD/ : { "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
