SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/naribwe_interiors/entry.o"(.text)
        "*/games/tla/src/scene/naribwe_interiors/scene_tables.o"(.text)
        "*/games/tla/src/scene/naribwe_interiors/script0.o"(.text)
        "*/games/tla/src/scene/naribwe_interiors/people.o"(.text)
        "*/games/tla/src/scene/naribwe_interiors/fortune.o"(.text)
        "*/games/tla/src/scene/naribwe_interiors/s0390.o"(.text)
        "*/games/tla/src/scene/naribwe_interiors/reading.o"(.text)
        "*/games/tla/src/scene/naribwe_interiors/s0ba0.o"(.text)
        "*/games/tla/src/scene/naribwe_interiors/s0c04.o"(.text)
        "*/games/tla/src/scene/naribwe_interiors/prepare.o"(.text)
        "*/games/tla/src/scene/naribwe_interiors/import.o"(.text)
        "*/games/tla/src/scene/naribwe_interiors/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
