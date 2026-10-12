SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_6b5/entry.o"(.text)
        "*/games/tla/src/field/overlay_6b5/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_6b5/ime_call.o"(.text)
        "*/games/tla/src/field/overlay_6b5/layout.o"(.text)
        "*/games/tla/src/field/overlay_6b5/peer.o"(.text)
        "*/games/tla/src/field/overlay_6b5/lobby.o"(.text)
        "*/resource_6b5_overlay.o"(.text.x02008134)
        "*/games/tla/src/field/overlay_6b5/circle.o"(.text)
        "*/resource_6b5_overlay.o"(.text.x0200839c)
        "*/resource_6b5_overlay.o"(.text.x02008538)
        "*/resource_6b5_overlay.o"(.text.x02008578)
        "*/games/tla/src/field/overlay_6b5/exchange.o"(.text)
        "*/resource_6b5_overlay.o"(.text.x02008864)
        "*/resource_6b5_overlay.o"(.text.x02008bb0)
        "*/resource_6b5_overlay.o"(.text.x02008da0)
        "*/games/tla/src/field/overlay_6b5/prompt.o"(.text)
        "*/games/tla/src/field/overlay_6b5/sound_call.o"(.text)
        "*/resource_6b5_overlay.o"(.text.x02008f6c)
        "*/resource_6b5_overlay.o"(.text.x02008fbc)
        "*/games/tla/src/field/overlay_6b5/talk.o"(.text)
        "*/games/tla/src/field/overlay_6b5/scene_events.o"(.text)
        "*/games/tla/src/field/overlay_6b5/results.o"(.text)
        "*/resource_6b5_overlay.o"(.text.x02009414)
        "*/resource_6b5_overlay.o"(.text.x0200945c)
        "*/games/tla/src/field/overlay_6b5/prepare.o"(.text)
        "*/games/tla/src/field/overlay_6b5/import.o"(.text)
        "*/games/tla/src/field/overlay_6b5/data.o"(.data)
    }
    .bss : { "*/resource_6b5_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
