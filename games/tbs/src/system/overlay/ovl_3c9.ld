EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/vinasu_chojo/entry.o"(.text)
        "*/games/tbs/src/field/vinasu_chojo/objects.o"(.text)
        "*/games/tbs/src/field/common/effect/move.o"(.text)
        "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn)
        "*/games/tbs/src/field/vinasu_chojo/chojo.o"(.text)
        "*/games/tbs/src/field/vinasu_chojo/import.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/tbs/src/field/vinasu_chojo/data.o"(.data)
    }
    .bss (NOLOAD) :
    {
        "*/resource_3c9_overlay.o"(.bss)
    }
    /DISCARD/ : { "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
