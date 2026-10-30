SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/shaman_village_cave/entry.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave/scene_tables.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave/s0098.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave/s00f0.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave/s0148.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave/s0184.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave/s030c.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave/placements.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave/scene_events.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave/s0430.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave/s083c.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave/s087c.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave/s0a24.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave/s0d84.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave/s0e04.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave/events.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave/import.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
