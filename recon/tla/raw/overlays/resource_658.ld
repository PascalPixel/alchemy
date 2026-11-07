SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/madra/entry.o"(.text)
        "*/recon/tla/raw/src/scene/madra/scene_tables.o"(.text)
        "*/recon/tla/raw/src/scene/madra/s006c.o"(.text)
        "*/recon/tla/raw/src/scene/madra/events_a.o"(.text)
        "*/recon/tla/raw/src/scene/madra/s02bc.o"(.text)
        "*/recon/tla/raw/src/scene/madra/events_b.o"(.text)
        "*/games/tla/src/scene/madra/s051c.o"(.text)
        "*/recon/tla/raw/src/scene/madra/events_c.o"(.text)
        "*/games/tla/src/scene/madra/s068c.o"(.text)
        "*/recon/tla/raw/src/scene/madra/s06ac.o"(.text)
        "*/recon/tla/raw/src/scene/madra/prepare.o"(.text)
        "*/recon/tla/raw/src/scene/madra/s0ac4.o"(.text)
        "*/recon/tla/raw/src/scene/madra/events_d.o"(.text)
        "*/games/tla/src/scene/madra/import.o"(.text)
        "*/games/tla/src/scene/madra/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
