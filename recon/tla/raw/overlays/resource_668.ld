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
        "*/resource_668_overlay.o"(.text.x02008b18)
        "*/games/tla/src/scene/eastern_alhafra/prepare.o"(.text)
        "*/resource_668_overlay.o"(.text.x02008fa4)
        "*/games/tla/src/scene/eastern_alhafra/events.o"(.text)
        "*/resource_668_overlay.o"(.text.x02009bf4)
        "*/games/tla/src/scene/eastern_alhafra/s1cb0.o"(.text)
        "*/resource_668_overlay.o"(.text.x0200bec4)
        "*/resource_668_overlay.o"(.text.x0200bf48)
        "*/resource_668_overlay.o"(.text.x0200c004)
        "*/games/tla/src/scene/eastern_alhafra/s40c0.o"(.text)
        "*/resource_668_overlay.o"(.text.x0200c0ec)
        "*/games/tla/src/field/common/map/cellfill.o"(.text)
        "*/resource_668_overlay.o"(.text.x0200c240)
        "*/resource_668_overlay.o"(.text.x0200c3a8)
        "*/resource_668_overlay.o"(.text.x0200c4c4)
        "*/resource_668_overlay.o"(.text.x0200c774)
        "*/resource_668_overlay.o"(.text.x0200c7fc)
        "*/resource_668_overlay.o"(.text.x0200c868)
        "*/games/tla/src/scene/eastern_alhafra/s48ec.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/import.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/data.o"(.data)
    }
    .bss : { "*/resource_668_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
