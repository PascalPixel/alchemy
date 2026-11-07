SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/title/entry.o"(.text)
        "*/games/tla/src/scene/title/scene_tables.o"(.text)
        "*/recon/tla/raw/src/scene/title/prepare.o"(.text)
        "*/recon/tla/raw/src/scene/title/s031c.o"(.text)
        "*/recon/tla/raw/src/scene/title/s0490.o"(.text)
        "*/games/tla/src/scene/title/import.o"(.text)
        "*/games/tla/src/scene/title/data.o"(.data)
    }
    .bss : { "*/resource_649_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
