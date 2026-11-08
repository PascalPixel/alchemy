SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/lemuria_palace/entry.o"(.text)
        "*/games/tla/src/scene/lemuria_palace/scene_tables.o"(.text)
        "*/games/tla/src/scene/lemuria_palace/s00c8.o"(.text)
        "*/games/tla/src/scene/lemuria_palace/scene_events.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria_palace/s00f4.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria_palace/prepare.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria_palace/s01d8.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria_palace/s02b4.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria_palace/s03a0.o"(.text)
        "*/games/tla/src/scene/lemuria_palace/s04e8.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria_palace/blend_fade.o"(.text)
        "*/recon/tla/raw/src/scene/lemuria_palace/s058c.o"(.text)
        "*/games/tla/src/scene/lemuria_palace/import.o"(.text)
        "*/games/tla/src/scene/lemuria_palace/data.o"(.data)
        "*/games/tla/src/scene/lemuria_palace/tiles.o"(.rodata)
        "*/games/tla/src/scene/lemuria_palace/data2.o"(.data)
    }
    .bss : { "*/resource_68d_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
