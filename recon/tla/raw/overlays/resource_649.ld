SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_649/entry.o"(.text)
        "*/games/tla/src/field/overlay_649/scene_tables.o"(.text)
        "*/resource_649_overlay.o"(.text.x0200805c)
        "*/games/tla/src/field/overlay_649/prepare.o"(.text)
        "*/resource_649_overlay.o"(.text.x02008278)
        "*/games/tla/src/field/overlay_649/s031c.o"(.text)
        "*/games/tla/src/field/overlay_649/s0490.o"(.text)
        "*/games/tla/src/field/overlay_649/import.o"(.text)
        "*/games/tla/src/field/overlay_649/data.o"(.data)
    }
    .bss : { "*/resource_649_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
