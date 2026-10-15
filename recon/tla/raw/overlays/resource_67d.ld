SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/izumo_interiors/entry.o"(.text)
        "*/games/tla/src/scene/izumo_interiors/s0038.o"(.text)
        "*/games/tla/src/scene/izumo_interiors/scene_tables.o"(.text)
        "*/games/tla/src/scene/izumo_interiors/s00b4.o"(.text)
        "*/games/tla/src/scene/izumo_interiors/s051c.o"(.text)
        "*/resource_67d_overlay.o"(.text.x0200861c)
        "*/games/tla/src/scene/izumo_interiors/s1d84.o"(.text)
        "*/resource_67d_overlay.o"(.text.x02009dd0)
        "*/resource_67d_overlay.o"(.text.x0200a00c)
        "*/games/tla/src/scene/izumo_interiors/prepare.o"(.text)
        "*/games/tla/src/scene/izumo_interiors/import.o"(.text)
        "*/games/tla/src/scene/izumo_interiors/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
