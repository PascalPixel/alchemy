SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/lemuria/entry.o"(.text)
        "*/games/tla/src/scene/lemuria/distance.o"(.text)
        "*/resource_68b_overlay.o"(.text.x02008070)
        "*/resource_68b_overlay.o"(.text.x020080e8)
        "*/games/tla/src/scene/lemuria/scene_tables.o"(.text)
        "*/games/tla/src/scene/lemuria/s0308.o"(.text)
        "*/games/tla/src/scene/lemuria/events_a.o"(.text)
        "*/games/tla/src/scene/lemuria/s0538.o"(.text)
        "*/games/tla/src/scene/lemuria/events_b.o"(.text)
        "*/games/tla/src/scene/lemuria/s062c.o"(.text)
        "*/games/tla/src/scene/lemuria/s08e8.o"(.text)
        "*/games/tla/src/scene/lemuria/scene_events.o"(.text)
        "*/games/tla/src/scene/lemuria/s0a6c.o"(.text)
        "*/resource_68b_overlay.o"(.text.x02008b28)
        "*/games/tla/src/scene/lemuria/s0bbc.o"(.text)
        "*/games/tla/src/scene/lemuria/s0bdc.o"(.text)
        "*/games/tla/src/scene/lemuria/prepare.o"(.text)
        "*/games/tla/src/scene/lemuria/s0e7c.o"(.text)
        "*/games/tla/src/scene/lemuria/s1160.o"(.text)
        "*/games/tla/src/scene/lemuria/s1b3c.o"(.text)
        "*/games/tla/src/scene/lemuria/import.o"(.text)
        "*/games/tla/src/scene/lemuria/data.o"(.data)
    }
    .bss : { "*/resource_68b_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
