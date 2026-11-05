SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/yampi_desert_cave/entry.o"(.text)
        "*/resource_662_overlay.o"(.text.x02008038)
        "*/games/tla/src/field/map/cellfill.o"(.text)
        "*/resource_662_overlay.o"(.text.x020081d4)
        "*/games/tla/src/scene/yampi_desert_cave/s033c.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/s0458.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/s0754.o"(.text)
        "*/resource_662_overlay.o"(.text.x020087dc)
        "*/games/tla/src/scene/yampi_desert_cave/stop.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/s0978.o"(.text)
        "*/resource_662_overlay.o"(.text.x0200899c)
        "*/games/tla/src/scene/yampi_desert_cave/s0acc.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/s0b54.o"(.text)
        "*/resource_662_overlay.o"(.text.x02008bc0)
        "*/games/tla/src/scene/yampi_desert_cave/s0c44.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/scene_tables.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/events.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/s0e68.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/s1120.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/s11b4.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/s11e0.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/s1238.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/guardian.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/s13c8.o"(.text)
        "*/resource_662_overlay.o"(.text.x02009418)
        "*/games/tla/src/scene/yampi_desert_cave/prepare.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/import.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/data.o"(.data)
    }
    .bss : { "*/resource_662_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
