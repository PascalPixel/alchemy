SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/northern_reaches/entry.o"(.text)
        "*/recon/tla/raw/src/scene/northern_reaches/scene_tables.o"(.text)
        "*/recon/tla/raw/src/scene/northern_reaches/s01d8.o"(.text)
        "*/games/tla/src/scene/northern_reaches/scene_events.o"(.text)
        "*/recon/tla/raw/src/scene/northern_reaches/s0618.o"(.text)
        "*/recon/tla/raw/src/scene/northern_reaches/s0680.o"(.text)
        "*/recon/tla/raw/src/scene/northern_reaches/s06b0.o"(.text)
        "*/games/tla/src/scene/northern_reaches/import.o"(.text)
        "*/games/tla/src/graphics/field/pal685.o"(.rodata)
        "*/games/tla/src/scene/northern_reaches/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
