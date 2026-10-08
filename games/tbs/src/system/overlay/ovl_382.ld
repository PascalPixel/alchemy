EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/kuupuappu_mura/entry.o"(.text)
        "*/games/tbs/src/field/kuupuappu_mura/object.o"(.text)
        "*/games/tbs/src/field/common/effect/move.o"(.text)
        "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn)
        "*/games/tbs/src/field/kuupuappu_mura/mura.o"(.text)
        "*/games/tbs/src/field/kuupuappu_mura/scene_cells.o"(.text)
        "*/games/tbs/src/field/kuupuappu_mura/arrival.o"(.text)
        "*/games/tbs/src/field/kuupuappu_mura/import.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/tbs/src/field/kuupuappu_mura/data.o"(.data)
    }
    /DISCARD/ : { "*/games/tbs/src/field/common/effect/spawn.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
