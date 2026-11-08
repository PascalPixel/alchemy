SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/apojii_islands_cave/entry.o"(.text)
        "*/games/tla/src/scene/apojii_islands_cave/scene_tables.o"(.text)
        "*/games/tla/src/scene/apojii_islands_cave/s0054.o"(.text)
        "*/games/tla/src/scene/apojii_islands_cave/scene_events.o"(.text)
        "*/games/tla/src/scene/apojii_islands_cave/s0070.o"(.text)
        "*/games/tla/src/scene/apojii_islands_cave/prepare.o"(.text)
        "*/games/tla/src/scene/apojii_islands_cave/import.o"(.text)
        "*/games/tla/src/scene/apojii_islands_cave/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
