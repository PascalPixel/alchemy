SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/lemuria/entry.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria/distance.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria/s0070.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria/s00e8.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria/scene_tables.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria/s0308.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria/events_a.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria/s0538.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria/events_b.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria/s062c.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria/s08e8.o"(.text)
        "*/games/tla/src/scene/lemuria/scene_events.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria/s0a6c.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria/s0b28.o"(.text)
        "*/games/tla/src/scene/lemuria/s0bbc.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria/s0bdc.o"(.text)
        "*/games/tla/src/scene/lemuria/prepare.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria/s0e7c.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria/s1160.o"(.text)
        "*/games/tla/src/scene/lemuria/s1b3c.o"(.text)
        "*/games/tla/src/scene/lemuria/import.o"(.text)
        "*/games/tla/src/scene/lemuria/data.o"(.data)
    }
    .bss : { "*/resource_68b_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
