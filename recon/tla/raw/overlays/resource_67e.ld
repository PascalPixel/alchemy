SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/izumo/entry.o"(.text)
        "*/games/tla/src/scene/izumo/s0038.o"(.text)
        "*/games/tla/src/scene/izumo/scene_tables.o"(.text)
        "*/games/tla/src/scene/izumo/events.o"(.text)
        "*/games/tla/src/scene/izumo/s05b4.o"(.text)
        "*/games/tla/src/scene/izumo/s06a4.o"(.text)
        "*/games/tla/src/scene/izumo/s0954.o"(.text)
        "*/games/tla/src/scene/izumo/prepare.o"(.text)
        "*/games/tla/src/scene/izumo/blocks.o"(.text)
        "*/games/tla/src/scene/izumo/import.o"(.text)
        "*/games/tla/src/scene/izumo/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
