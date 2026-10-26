SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/mercury_lighthouse/entry.o"(.text)
        "*/games/tla/src/scene/mercury_lighthouse/s0038.o"(.text)
        "*/games/tla/src/scene/mercury_lighthouse/s0060.o"(.text)
        "*/games/tla/src/scene/mercury_lighthouse/scene_tables.o"(.text)
        "*/resource_6b2_overlay.o"(.text.x020080f4)
        "*/games/tla/src/scene/mercury_lighthouse/s01b0.o"(.text)
        "*/games/tla/src/scene/mercury_lighthouse/s04c4.o"(.text)
        "*/games/tla/src/scene/mercury_lighthouse/prepare.o"(.text)
        "*/games/tla/src/scene/mercury_lighthouse/import.o"(.text)
        "*/games/tla/src/scene/mercury_lighthouse/data.o"(.data)
    }
    .bss : { "*/resource_6b2_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
