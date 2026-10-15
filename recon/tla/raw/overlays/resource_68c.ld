SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/lemuria_interiors/entry.o"(.text)
        "*/games/tla/src/scene/lemuria_interiors/scene_tables.o"(.text)
        "*/games/tla/src/scene/lemuria_interiors/s0104.o"(.text)
        "*/games/tla/src/scene/lemuria_interiors/events.o"(.text)
        "*/games/tla/src/scene/lemuria_interiors/scene_events.o"(.text)
        "*/games/tla/src/scene/lemuria_interiors/prepare.o"(.text)
        "*/resource_68c_overlay.o"(.text.x02008a5c)
        "*/games/tla/src/scene/lemuria_interiors/s128c.o"(.text)
        "*/resource_68c_overlay.o"(.text.x020092dc)
        "*/games/tla/src/scene/lemuria_interiors/s1398.o"(.text)
        "*/games/tla/src/scene/lemuria_interiors/s13f0.o"(.text)
        "*/games/tla/src/scene/lemuria_interiors/import.o"(.text)
        "*/games/tla/src/scene/lemuria_interiors/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
