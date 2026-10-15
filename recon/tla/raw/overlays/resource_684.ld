SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/champa_sanctum/entry.o"(.text)
        "*/games/tla/src/scene/champa_sanctum/scene_tables.o"(.text)
        "*/games/tla/src/scene/champa_sanctum/script0.o"(.text)
        "*/games/tla/src/scene/champa_sanctum/scene_events.o"(.text)
        "*/games/tla/src/scene/champa_sanctum/s0064.o"(.text)
        "*/games/tla/src/scene/champa_sanctum/prepare.o"(.text)
        "*/games/tla/src/scene/champa_sanctum/import.o"(.text)
        "*/games/tla/src/scene/champa_sanctum/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
