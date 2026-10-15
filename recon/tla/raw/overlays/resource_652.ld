SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/daila_interiors/entry.o"(.text)
        "*/games/tla/src/scene/daila_interiors/scene_tables.o"(.text)
        "*/games/tla/src/scene/daila_interiors/lines.o"(.text)
        "*/games/tla/src/scene/daila_interiors/villager.o"(.text)
        "*/games/tla/src/scene/daila_interiors/s0704.o"(.text)
        "*/games/tla/src/scene/daila_interiors/item.o"(.text)
        "*/resource_652_overlay.o"(.text.x02008878)
        "*/games/tla/src/scene/daila_interiors/arrival.o"(.text)
        "*/games/tla/src/scene/daila_interiors/import.o"(.text)
        "*/games/tla/src/scene/daila_interiors/data.o"(.data)
        "*/games/tla/src/scene/daila_interiors/data2.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
