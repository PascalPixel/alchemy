SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/eastern_alhafra_ship/entry.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra_ship/scene_tables.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra_ship/s0058.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra_ship/events.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra_ship/scene_events.o"(.text)
        "*/resource_669_overlay.o"(.text.x02008250)
        "*/games/tla/src/scene/eastern_alhafra_ship/prepare.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra_ship/spawn.o"(.text)
        "*/resource_669_overlay.o"(.text.x02008438)
        "*/games/tla/src/scene/eastern_alhafra_ship/cutscene.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra_ship/import.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra_ship/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
