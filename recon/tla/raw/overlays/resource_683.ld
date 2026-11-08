SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/champa_interiors/entry.o"(.text)
        "*/games/tla/src/scene/champa_interiors/scene_tables.o"(.text)
        "*/games/tla/src/scene/champa_interiors/prepare.o"(.text)
        "*/games/tla/src/scene/champa_interiors/import.o"(.text)
        "*/games/tla/src/scene/champa_interiors/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
