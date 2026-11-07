SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/izumo_interiors/entry.o"(.text)
        "*/games/tla/src/scene/izumo_interiors/s0038.o"(.text)
        "*/games/tla/src/scene/izumo_interiors/scene_tables.o"(.text)
        "*/recon/tla/raw/src/scene/izumo_interiors/s00b4.o"(.text)
        "*/recon/tla/raw/src/scene/izumo_interiors/s051c.o"(.text)
        "*/recon/tla/raw/src/scene/izumo_interiors/s1d84.o"(.text)
        "*/recon/tla/raw/src/scene/izumo_interiors/s1dd0.o"(.text)
        "*/recon/tla/raw/src/scene/izumo_interiors/s200c.o"(.text)
        "*/games/tla/src/scene/izumo_interiors/prepare.o"(.text)
        "*/games/tla/src/scene/izumo_interiors/import.o"(.text)
        "*/games/tla/src/scene/izumo_interiors/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
