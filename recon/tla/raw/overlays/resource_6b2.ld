SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_6b2/entry.o"(.text)
        "*/games/tla/src/field/overlay_6b2/s0038.o"(.text)
        "*/resource_6b2_overlay.o"(.text.x02008060)
        "*/games/tla/src/field/overlay_6b2/scene_tables.o"(.text)
        "*/resource_6b2_overlay.o"(.text.x020080f4)
        "*/resource_6b2_overlay.o"(.text.x020081b0)
        "*/games/tla/src/field/overlay_6b2/s04c4.o"(.text)
        "*/games/tla/src/field/overlay_6b2/prepare.o"(.text)
        "*/games/tla/src/field/overlay_6b2/import.o"(.text)
        "*/games/tla/src/field/overlay_6b2/data.o"(.data)
    }
    .bss : { "*/resource_6b2_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
