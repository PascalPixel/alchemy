SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/northern_reaches/entry.o"(.text)
        "*/games/tla/src/scene/northern_reaches/scene_tables.o"(.text)
        "*/resource_6a7_overlay.o"(.text.x020081d8)
        "*/games/tla/src/scene/northern_reaches/scene_events.o"(.text)
        "*/resource_6a7_overlay.o"(.text.x02008618)
        "*/games/tla/src/scene/northern_reaches/s0680.o"(.text)
        "*/resource_6a7_overlay.o"(.text.x020086b0)
        "*/games/tla/src/scene/northern_reaches/import.o"(.text)
        "*/games/tla/src/graphics/field/pal685.o"(.rodata)
        "*/games/tla/src/scene/northern_reaches/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
