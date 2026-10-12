SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_6b3/entry.o"(.text)
        "*/games/tla/src/field/overlay_6b3/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_6b3/s005c.o"(.text)
        "*/resource_6b3_overlay.o"(.text.x02008098)
        "*/resource_6b3_overlay.o"(.text.x02008158)
        "*/resource_6b3_overlay.o"(.text.x02008244)
        "*/resource_6b3_overlay.o"(.text.x02008318)
        "*/games/tla/src/field/overlay_6b3/prepare.o"(.text)
        "*/games/tla/src/field/overlay_6b3/import.o"(.text)
        "*/games/tla/src/field/overlay_6b3/pal.o"(.rodata)
        "*/games/tla/src/field/overlay_6b3/data.o"(.data)
    }
    .bss : { "*/resource_6b3_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
