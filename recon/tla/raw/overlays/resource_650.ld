SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/idejima/entry.o"(.text)
        "*/games/tla/src/scene/idejima/scene_tables.o"(.text)
        "*/games/tla/src/scene/idejima/s0098.o"(.text)
        "*/games/tla/src/scene/idejima/s049c.o"(.text)
        "*/games/tla/src/scene/idejima/depart.o"(.text)
        "*/games/tla/src/scene/idejima/wave.o"(.text)
        "*/resource_650_overlay.o"(.text.x020095a0)
        "*/games/tla/src/scene/idejima/drift.o"(.text)
        "*/games/tla/src/field/common/drift_spawn.o"(.text)
        "*/games/tla/src/scene/idejima/s16d8.o"(.text)
        "*/games/tla/src/scene/idejima/s1ee0.o"(.text)
        "*/games/tla/src/scene/idejima/s2304.o"(.text)
        "*/games/tla/src/scene/idejima/wake.o"(.text)
        "*/games/tla/src/scene/idejima/prepare.o"(.text)
        "*/games/tla/src/scene/idejima/s2f88.o"(.text)
        "*/games/tla/src/scene/idejima/s2fd4.o"(.text)
        "*/games/tla/src/scene/idejima/import.o"(.text)
        "*/games/tla/src/scene/idejima/data.o"(.data)
    }
    .bss : { "*/resource_650_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
