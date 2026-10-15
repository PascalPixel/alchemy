SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/lemuria_entrance/entry.o"(.text)
        "*/games/tla/src/scene/lemuria_entrance/events_a.o"(.text)
        "*/games/tla/src/scene/lemuria_entrance/scene_tables.o"(.text)
        "*/games/tla/src/scene/lemuria_entrance/events_b.o"(.text)
        "*/games/tla/src/scene/lemuria_entrance/scene_events.o"(.text)
        "*/games/tla/src/scene/lemuria_entrance/s0120.o"(.text)
        "*/resource_688_overlay.o"(.text.x02008190)
        "*/games/tla/src/scene/lemuria_entrance/prepare.o"(.text)
        "*/resource_688_overlay.o"(.text.x02008310)
        "*/games/tla/src/scene/lemuria_entrance/s03cc.o"(.text)
        "*/games/tla/src/scene/lemuria_entrance/import.o"(.text)
        "*/games/tla/src/scene/lemuria_entrance/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
