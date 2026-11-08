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
        "*/games/tla/src/scene/lemuria_entrance/s0190.o"(.text)
        "*/games/tla/src/scene/lemuria_entrance/prepare.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria_entrance/s0310.o"(.text)
        "*/games/tla/src/scene/lemuria_entrance/s03cc.o"(.text)
        "*/games/tla/src/scene/lemuria_entrance/import.o"(.text)
        "*/games/tla/src/scene/lemuria_entrance/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
