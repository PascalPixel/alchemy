SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_68c/entry.o"(.text)
        "*/games/tla/src/field/overlay_68c/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_68c/s0104.o"(.text)
        "*/games/tla/src/field/overlay_68c/events.o"(.text)
        "*/games/tla/src/field/overlay_68c/scene_events.o"(.text)
        "*/resource_68c_overlay.o"(.text.x02008284)
        "*/games/tla/src/field/overlay_68c/prepare.o"(.text)
        "*/resource_68c_overlay.o"(.text.x02008a5c)
        "*/resource_68c_overlay.o"(.text.x0200928c)
        "*/resource_68c_overlay.o"(.text.x020092dc)
        "*/games/tla/src/field/overlay_68c/s1398.o"(.text)
        "*/resource_68c_overlay.o"(.text.x020093f0)
        "*/games/tla/src/field/overlay_68c/import.o"(.text)
        "*/games/tla/src/field/overlay_68c/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
