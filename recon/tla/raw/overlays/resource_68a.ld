SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_68a/entry.o"(.text)
        "*/games/tla/src/field/overlay_68a/start.o"(.text)
        "*/games/tla/src/field/overlay_68a/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_68a/map.o"(.text)
        "*/resource_68a_overlay.o"(.text.x02008110)
        "*/games/tla/src/field/overlay_68a/copies.o"(.text)
        "*/resource_68a_overlay.o"(.text.x02008468)
        "*/resource_68a_overlay.o"(.text.x02008534)
        "*/games/tla/src/field/overlay_68a/scene_events.o"(.text)
        "*/resource_68a_overlay.o"(.text.x02008630)
        "*/resource_68a_overlay.o"(.text.x020086bc)
        "*/resource_68a_overlay.o"(.text.x020087a4)
        "*/games/tla/src/field/overlay_68a/s0830.o"(.text)
        "*/resource_68a_overlay.o"(.text.x02008838)
        "*/games/tla/src/field/overlay_68a/import.o"(.text)
        "*/resource_68a_overlay.o"(.rodata.x02009a34)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
