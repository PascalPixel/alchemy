SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/garoh_interiors/entry.o"(.text)
        "*/games/tla/src/scene/garoh_interiors/scene_tables.o"(.text)
        "*/games/tla/src/scene/garoh_interiors/s004c.o"(.text)
        "*/games/tla/src/scene/garoh_interiors/s039c.o"(.text)
        "*/resource_660_overlay.o"(.text.x02008580)
        "*/games/tla/src/scene/garoh_interiors/prepare.o"(.text)
        "*/games/tla/src/scene/garoh_interiors/s0710.o"(.text)
        "*/games/tla/src/scene/garoh_interiors/import.o"(.text)
        "*/games/tla/src/scene/garoh_interiors/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
