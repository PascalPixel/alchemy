SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/menu/title/entry.o"(.text)
        "*/games/tla/src/menu/title/scene_tables.o"(.text)
        "*/games/tla/src/menu/title/prepare.o"(.text)
        "*/games/tla/src/menu/title/s031c.o"(.text)
        "*/games/tla/src/menu/title/s0490.o"(.text)
        "*/games/tla/src/menu/title/import.o"(.text)
        "*/games/tla/src/menu/title/data.o"(.data)
    }
    .bss : { "*/resource_649_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
