SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/menu/lobby/entry.o"(.text)
        "*/games/tla/src/menu/lobby/scene_tables.o"(.text)
        "*/games/tla/src/menu/lobby/ime_call.o"(.text)
        "*/games/tla/src/menu/lobby/layout.o"(.text)
        "*/games/tla/src/menu/lobby/peer.o"(.text)
        "*/games/tla/src/menu/lobby/lobby.o"(.text)
        "*/games/tla/src/menu/lobby/circle.o"(.text)
        "*/resource_6b5_overlay.o"(.text.x0200839c)
        "*/games/tla/src/menu/lobby/owners.o"(.text)
        "*/resource_6b5_overlay.o"(.text.x02008578)
        "*/games/tla/src/menu/lobby/exchange.o"(.text)
        "*/resource_6b5_overlay.o"(.text.x02008864)
        "*/games/tla/src/menu/lobby/counter.o"(.text)
        "*/games/tla/src/menu/lobby/s0da0.o"(.text)
        "*/games/tla/src/menu/lobby/prompt.o"(.text)
        "*/games/tla/src/menu/lobby/sound_call.o"(.text)
        "*/resource_6b5_overlay.o"(.text.x02008fbc)
        "*/games/tla/src/menu/lobby/talk.o"(.text)
        "*/games/tla/src/menu/lobby/scene_events.o"(.text)
        "*/games/tla/src/menu/lobby/results.o"(.text)
        "*/resource_6b5_overlay.o"(.text.x0200945c)
        "*/games/tla/src/menu/lobby/prepare.o"(.text)
        "*/games/tla/src/menu/lobby/import.o"(.text)
        "*/games/tla/src/menu/lobby/data.o"(.data)
    }
    .bss : { "*/resource_6b5_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
