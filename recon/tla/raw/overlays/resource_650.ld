SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/idejima/entry.o"(.text)
        "*/games/tla/src/field/idejima/scene_tables.o"(.text)
        "*/games/tla/src/field/idejima/s0098.o"(.text)
        "*/games/tla/src/field/idejima/s049c.o"(.text)
        "*/games/tla/src/field/idejima/depart.o"(.text)
        "*/games/tla/src/field/idejima/wave.o"(.text)
        "*/resource_650_overlay.o"(.text.x020095a0)
        "*/games/tla/src/field/idejima/drift.o"(.text)
        "*/games/tla/src/field/common/drift_spawn.o"(.text)
        "*/resource_650_overlay.o"(.text.x020096d8)
        "*/resource_650_overlay.o"(.text.x02009ee0)
        "*/resource_650_overlay.o"(.text.x0200a304)
        "*/games/tla/src/field/idejima/wake.o"(.text)
        "*/resource_650_overlay.o"(.text.x0200aec8)
        "*/games/tla/src/field/idejima/prepare.o"(.text)
        "*/resource_650_overlay.o"(.text.x0200af88)
        "*/resource_650_overlay.o"(.text.x0200afd4)
        "*/games/tla/src/field/idejima/import.o"(.text)
        "*/games/tla/src/field/idejima/data.o"(.data)
    }
    .bss : { "*/resource_650_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
