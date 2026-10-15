SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/naribwe_interiors/entry.o"(.text)
        "*/games/tla/src/scene/naribwe_interiors/scene_tables.o"(.text)
        "*/games/tla/src/scene/naribwe_interiors/script0.o"(.text)
        "*/games/tla/src/scene/naribwe_interiors/people.o"(.text)
        "*/games/tla/src/scene/naribwe_interiors/fortune.o"(.text)
        "*/resource_66e_overlay.o"(.text.x02008390)
        "*/games/tla/src/scene/naribwe_interiors/reading.o"(.text)
        "*/resource_66e_overlay.o"(.text.x02008ba0)
        "*/resource_66e_overlay.o"(.text.x02008c04)
        "*/games/tla/src/scene/naribwe_interiors/prepare.o"(.text)
        "*/games/tla/src/scene/naribwe_interiors/import.o"(.text)
        "*/games/tla/src/scene/naribwe_interiors/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
