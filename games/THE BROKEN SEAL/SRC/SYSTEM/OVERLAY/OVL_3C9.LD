EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/OBJECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text.effect_spawn)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/CHOJO.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/DATA.o"(.data)
    }
    .bss (NOLOAD) :
    {
        "*/resource_3c9_overlay.o"(.bss)
    }
    /DISCARD/ : { "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
