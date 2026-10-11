SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_65e/entry.o"(.text)
        "*/games/tla/src/field/overlay_65e/scene_tables.o"(.text)
        "*/resource_65e_overlay.o"(.text.x0200804c)
        "*/games/tla/src/field/overlay_65e/events.o"(.text)
        "*/resource_65e_overlay.o"(.text.x020080c8)
        "*/games/tla/src/field/overlay_65e/events3.o"(.text)
        "*/resource_65e_overlay.o"(.text.x02008340)
        "*/games/tla/src/field/overlay_65e/prepare.o"(.text)
        "*/resource_65e_overlay.o"(.text.x02008470)
        "*/games/tla/src/field/overlay_65e/events2.o"(.text)
        "*/games/tla/src/field/overlay_65e/import.o"(.text)
        "*/games/tla/src/field/overlay_65e/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
