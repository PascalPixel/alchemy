SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/madra/entry.o"(.text)
        "*/games/tla/src/scene/madra/scene_tables.o"(.text)
        "*/resource_658_overlay.o"(.text.x0200806c)
        "*/games/tla/src/scene/madra/events_a.o"(.text)
        "*/games/tla/src/scene/madra/s02bc.o"(.text)
        "*/games/tla/src/scene/madra/events_b.o"(.text)
        "*/games/tla/src/scene/madra/s051c.o"(.text)
        "*/games/tla/src/scene/madra/events_c.o"(.text)
        "*/games/tla/src/scene/madra/s068c.o"(.text)
        "*/games/tla/src/scene/madra/s06ac.o"(.text)
        "*/games/tla/src/scene/madra/prepare.o"(.text)
        "*/games/tla/src/scene/madra/s0ac4.o"(.text)
        "*/games/tla/src/scene/madra/events_d.o"(.text)
        "*/games/tla/src/scene/madra/import.o"(.text)
        "*/games/tla/src/scene/madra/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
