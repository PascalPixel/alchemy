SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/goma_iriguchi/entry.o"(.text)
        "*/games/tbs/src/field/goma_iriguchi/effobj.o"(.text)
        "*/games/tbs/src/field/common/effect/move.o"(.text)
        "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn)
        "*/games/tbs/src/field/goma_iriguchi/iriguchi.o"(.text)
        "*/games/tbs/src/field/goma_iriguchi/import.o"(.text)
        "*/games/tbs/src/field/goma_iriguchi/data.o"(.data)
    }
    /DISCARD/ : { "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
