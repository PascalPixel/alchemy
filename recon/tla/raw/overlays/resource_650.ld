SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/idejima/entry.o"(.text)
        "*/games/tla/src/scene/idejima/scene_tables.o"(.text)
        "*/recon/tla/raw/src/scene/idejima/s0098.o"(.text)
        "*/recon/tla/raw/src/scene/idejima/s049c.o"(.text)
        "*/recon/tla/raw/src/scene/idejima/depart.o"(.text)
        "*/recon/tla/raw/src/scene/idejima/wave.o"(.text)
        "*/recon/tla/raw/src/scene/idejima/s15a0.o"(.text)
        "*/recon/tla/raw/src/scene/idejima/drift.o"(.text)
        "*/games/tla/src/scene/shared/drift_spawn.o"(.text)
        "*/recon/tla/raw/src/scene/idejima/s16d8.o"(.text)
        "*/recon/tla/raw/src/scene/idejima/s1ee0.o"(.text)
        "*/recon/tla/raw/src/scene/idejima/s2304.o"(.text)
        "*/recon/tla/raw/src/scene/idejima/wake.o"(.text)
        "*/recon/tla/raw/src/scene/idejima/prepare.o"(.text)
        "*/recon/tla/raw/src/scene/idejima/s2f88.o"(.text)
        "*/recon/tla/raw/src/scene/idejima/s2fd4.o"(.text)
        "*/games/tla/src/scene/idejima/import.o"(.text)
        "*/games/tla/src/scene/idejima/data.o"(.data)
    }
    .bss : { "*/resource_650_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
