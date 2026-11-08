SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/lemuria_spring/entry.o"(.text)
        "*/games/tla/src/scene/lemuria_spring/scene_tables.o"(.text)
        "*/games/tla/src/scene/lemuria_spring/s0054.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria_spring/s0070.o"(.text)
        "*/games/tla/src/scene/lemuria_spring/s00d0.o"(.text)
        "*/games/tla/src/scene/lemuria_spring/scene_events.o"(.text)
        "*/resource_68e_overlay.o"(.text.x0200812c)
        "*/games/tla/src/scene/lemuria_spring/prepare.o"(.text)
        "*/resource_68e_overlay.o"(.text.x020081e4)
        "*/games/tla/src/scene/lemuria_spring/s049c.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria_spring/s0884.o"(.text)
        "*/games/tla/src/scene/lemuria_spring/s08f0.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria_spring/s0904.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria_spring/ride.o"(.text)
        "*/games/tla/src/scene/lemuria_spring/import.o"(.text)
        "*/games/tla/src/scene/lemuria_spring/data.o"(.data)
    }
    .bss : { "*/resource_68e_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
