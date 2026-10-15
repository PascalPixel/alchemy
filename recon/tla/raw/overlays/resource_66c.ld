SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/gondowan_cliffs_hideout/entry.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs_hideout/scene_tables.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs_hideout/placements.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs_hideout/scene_events.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs_hideout/warriors.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs_hideout/import.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs_hideout/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
