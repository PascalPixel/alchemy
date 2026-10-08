SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_SUIRO/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_SUIRO/OBJECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text.effect_spawn)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_SUIRO/SUIRO.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_SUIRO/BURST.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_SUIRO/IMPORT.o"(.text)
        "*/resource_38a_overlay.o"(.rodata.x02008764)
    }
    /DISCARD/ : { "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
