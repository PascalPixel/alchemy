SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_STAR/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_STAR/OBJECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text.effect_spawn)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_STAR/STAR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_STAR/DIALOGUE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_STAR/RING_SETUP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_STAR/IMPORT.o"(.text)
        "*/resource_380_overlay.o"(.rodata.x0200cb1c)
    }
    /DISCARD/ : { "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
