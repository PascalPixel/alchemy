EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/ramakan_sabaku/entry.o"(.text)
        "*/games/tbs/src/field/ramakan_sabaku/sabaku.o"(.text)
        "*/games/tbs/src/field/common/effect/move.o"(.text)
        "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn)
        "*/games/tbs/src/field/ramakan_sabaku/setup.o"(.text)
        "*/games/tbs/src/field/ramakan_sabaku/import.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/tbs/src/field/ramakan_sabaku/data.o"(.data)
        "*/games/tbs/src/graphics/ramakan/sand.o"(.rodata)
        "*/games/tbs/src/field/ramakan_sabaku/data2.o"(.data)
    }
    .bss ALIGN(16) (NOLOAD) : { "*/resource_3a5_overlay.o"(.bss) }
    /DISCARD/ : { "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
