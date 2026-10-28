SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/lobby/entry.o"(.text)
        "*/games/tla/src/scene/lobby/scene_tables.o"(.text)
        "*/games/tla/src/scene/lobby/ime_call.o"(.text)
        "*/games/tla/src/scene/lobby/layout.o"(.text)
        "*/games/tla/src/scene/lobby/peer.o"(.text)
        "*/games/tla/src/scene/lobby/lobby.o"(.text)
        "*/games/tla/src/scene/lobby/circle.o"(.text)
        "*/resource_6b5_overlay.o"(.text.x0200839c)
        "*/games/tla/src/scene/lobby/owners.o"(.text)
        "*/resource_6b5_overlay.o"(.text.x02008578)
        "*/games/tla/src/scene/lobby/exchange.o"(.text)
        "*/resource_6b5_overlay.o"(.text.x02008864)
        "*/games/tla/src/scene/lobby/counter.o"(.text)
        "*/games/tla/src/scene/lobby/s0da0.o"(.text)
        "*/games/tla/src/scene/lobby/prompt.o"(.text)
        "*/games/tla/src/scene/lobby/sound_call.o"(.text)
        "*/resource_6b5_overlay.o"(.text.x02008fbc)
        "*/games/tla/src/scene/lobby/talk.o"(.text)
        "*/games/tla/src/scene/lobby/scene_events.o"(.text)
        "*/games/tla/src/scene/lobby/results.o"(.text)
        "*/resource_6b5_overlay.o"(.text.x0200945c)
        "*/games/tla/src/scene/lobby/prepare.o"(.text)
        "*/games/tla/src/scene/lobby/import.o"(.text)
        "*/games/tla/src/scene/lobby/data.o"(.data)
    }
    .bss : { "*/resource_6b5_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
