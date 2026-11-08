SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/eastern_alhafra/entry.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/scene_tables.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s0200.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/events2.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s0514.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/scene_events.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/wave.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s0b18.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/prepare.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s0fa4.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/events.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s1bf4.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s1cb0.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s3ec4.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s3f48.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s4004.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s40c0.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s40ec.o"(.text)
        "*/games/tla/src/field/map/cellfill.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s4240.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s43a8.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s44c4.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s4774.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s47fc.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s4868.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s48ec.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/import.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/data.o"(.data)
    }
    .bss : { "*/resource_668_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
