SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/madra_interiors/entry.o"(.text)
        "*/games/tla/src/scene/madra_interiors/scene_tables.o"(.text)
        "*/games/tla/src/scene/madra_interiors/s004c.o"(.text)
        "*/games/tla/src/scene/madra_interiors/s01b8.o"(.text)
        "*/resource_659_overlay.o"(.text.x02008570)
        "*/games/tla/src/scene/madra_interiors/prepare.o"(.text)
        "*/resource_659_overlay.o"(.text.x02009008)
        "*/games/tla/src/scene/madra_interiors/events.o"(.text)
        "*/games/tla/src/scene/madra_interiors/import.o"(.text)
        "*/games/tla/src/scene/madra_interiors/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
