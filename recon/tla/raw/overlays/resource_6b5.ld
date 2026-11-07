SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/lobby/entry.o"(.text)
        "*/games/tla/src/scene/lobby/scene_tables.o"(.text)
        "*/games/tla/src/scene/lobby/ime_call.o"(.text)
        "*/games/tla/src/scene/lobby/layout.o"(.text)
        "*/recon/tla/raw/src/scene/lobby/peer.o"(.text)
        "*/recon/tla/raw/src/scene/lobby/lobby.o"(.text)
        "*/recon/tla/raw/src/scene/lobby/circle.o"(.text)
        "*/recon/tla/raw/src/scene/lobby/s039c.o"(.text)
        "*/games/tla/src/scene/lobby/owners.o"(.text)
        "*/recon/tla/raw/src/scene/lobby/s0578.o"(.text)
        "*/recon/tla/raw/src/scene/lobby/exchange.o"(.text)
        "*/recon/tla/raw/src/scene/lobby/s0864.o"(.text)
        "*/recon/tla/raw/src/scene/lobby/counter.o"(.text)
        "*/recon/tla/raw/src/scene/lobby/s0da0.o"(.text)
        "*/games/tla/src/scene/lobby/prompt.o"(.text)
        "*/games/tla/src/scene/lobby/sound_call.o"(.text)
        "*/recon/tla/raw/src/scene/lobby/s0fbc.o"(.text)
        "*/recon/tla/raw/src/scene/lobby/talk.o"(.text)
        "*/games/tla/src/scene/lobby/scene_events.o"(.text)
        "*/recon/tla/raw/src/scene/lobby/results.o"(.text)
        "*/recon/tla/raw/src/scene/lobby/s145c.o"(.text)
        "*/games/tla/src/scene/lobby/prepare.o"(.text)
        "*/games/tla/src/scene/lobby/import.o"(.text)
        "*/games/tla/src/scene/lobby/data.o"(.data)
    }
    .bss : { "*/resource_6b5_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
