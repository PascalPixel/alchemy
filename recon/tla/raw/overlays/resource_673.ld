SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/gabomba_catacombs/entry.o"(.text)
        "*/games/tla/src/scene/gabomba_catacombs/events.o"(.text)
        "*/resource_673_overlay.o"(.text.x020088a0)
        "*/games/tla/src/scene/gabomba_catacombs/events_b.o"(.text)
        "*/resource_673_overlay.o"(.text.x020089d4)
        "*/games/tla/src/scene/gabomba_catacombs/s0d90.o"(.text)
        "*/games/tla/src/scene/gabomba_catacombs/scene_tables.o"(.text)
        "*/games/tla/src/scene/gabomba_catacombs/regions.o"(.text)
        "*/games/tla/src/scene/gabomba_catacombs/exits.o"(.text)
        "*/games/tla/src/scene/gabomba_catacombs/placements.o"(.text)
        "*/resource_673_overlay.o"(.text.x02008ecc)
        "*/games/tla/src/scene/gabomba_catacombs/prepare.o"(.text)
        "*/games/tla/src/scene/gabomba_catacombs/import.o"(.text)
        "*/games/tla/src/scene/gabomba_catacombs/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
