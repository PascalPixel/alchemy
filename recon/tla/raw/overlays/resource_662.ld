SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/yampi_desert_cave/entry.o"(.text)
        "*/resource_662_overlay.o"(.text.x02008038)
        "*/recon/tla/raw/src/field/map/cellfill.o"(.text)
        "*/resource_662_overlay.o"(.text.x020081d4)
        "*/recon/tla/raw/src/scene/yampi_desert_cave/s033c.o"(.text)
        "*/recon/tla/raw/src/scene/yampi_desert_cave/s0458.o"(.text)
        "*/recon/tla/raw/src/scene/yampi_desert_cave/s0754.o"(.text)
        "*/recon/tla/raw/src/scene/yampi_desert_cave/s07dc.o"(.text)
        "*/recon/tla/raw/src/scene/yampi_desert_cave/stop.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/s0978.o"(.text)
        "*/recon/tla/raw/src/scene/yampi_desert_cave/s099c.o"(.text)
        "*/recon/tla/raw/src/scene/yampi_desert_cave/s0acc.o"(.text)
        "*/recon/tla/raw/src/scene/yampi_desert_cave/s0b54.o"(.text)
        "*/recon/tla/raw/src/scene/yampi_desert_cave/s0bc0.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/s0c44.o"(.text)
        "*/recon/tla/raw/src/scene/yampi_desert_cave/scene_tables.o"(.text)
        "*/recon/tla/raw/src/scene/yampi_desert_cave/events.o"(.text)
        "*/recon/tla/raw/src/scene/yampi_desert_cave/s0e68.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/s1120.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/s11b4.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/s11e0.o"(.text)
        "*/recon/tla/raw/src/scene/yampi_desert_cave/s1238.o"(.text)
        "*/recon/tla/raw/src/scene/yampi_desert_cave/guardian.o"(.text)
        "*/recon/tla/raw/src/scene/yampi_desert_cave/s13c8.o"(.text)
        "*/resource_662_overlay.o"(.text.x02009418)
        "*/games/tla/src/scene/yampi_desert_cave/prepare.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/import.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/data.o"(.data)
    }
    .bss : { "*/resource_662_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
