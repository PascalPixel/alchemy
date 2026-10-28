SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/eastern_alhafra/entry.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/scene_tables.o"(.text)
        "*/resource_668_overlay.o"(.text.x02008200)
        "*/games/tla/src/scene/eastern_alhafra/events2.o"(.text)
        "*/resource_668_overlay.o"(.text.x02008514)
        "*/games/tla/src/scene/eastern_alhafra/scene_events.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/wave.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s0b18.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/prepare.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s0fa4.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/events.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s1bf4.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s1cb0.o"(.text)
        "*/resource_668_overlay.o"(.text.x0200bec4)
        "*/games/tla/src/scene/eastern_alhafra/s3f48.o"(.text)
        "*/resource_668_overlay.o"(.text.x0200c004)
        "*/games/tla/src/scene/eastern_alhafra/s40c0.o"(.text)
        "*/resource_668_overlay.o"(.text.x0200c0ec)
        "*/games/tla/src/field/shared/map/cellfill.o"(.text)
        "*/resource_668_overlay.o"(.text.x0200c240)
        "*/resource_668_overlay.o"(.text.x0200c3a8)
        "*/resource_668_overlay.o"(.text.x0200c4c4)
        "*/games/tla/src/scene/eastern_alhafra/s4774.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s47fc.o"(.text)
        "*/resource_668_overlay.o"(.text.x0200c868)
        "*/games/tla/src/scene/eastern_alhafra/s48ec.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/import.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/data.o"(.data)
    }
    .bss : { "*/resource_668_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
