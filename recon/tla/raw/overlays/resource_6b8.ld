SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_6b8/entry.o"(.text)
        "*/games/tla/src/field/overlay_6b8/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_6b8/exits.o"(.text)
        "*/games/tla/src/field/overlay_6b8/scene_events.o"(.text)
        "*/resource_6b8_overlay.o"(.text.x0200879c)
        "*/games/tla/src/field/overlay_6b8/prepare.o"(.text)
        "*/resource_6b8_overlay.o"(.text.x02008a10)
        "*/resource_6b8_overlay.o"(.text.x02008ad0)
        "*/resource_6b8_overlay.o"(.text.x02008c4c)
        "*/games/tla/src/field/overlay_6b8/glow_end.o"(.text)
        "*/games/tla/src/field/overlay_6b8/import.o"(.text)
        "*/resource_6b8_overlay.o"(.rodata.x02008f44)
    }
    .bss : { "*/resource_6b8_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
