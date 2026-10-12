SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_68d/entry.o"(.text)
        "*/games/tla/src/field/overlay_68d/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_68d/s00c8.o"(.text)
        "*/games/tla/src/field/overlay_68d/scene_events.o"(.text)
        "*/resource_68d_overlay.o"(.text.x020080f4)
        "*/resource_68d_overlay.o"(.text.x02008158)
        "*/games/tla/src/field/overlay_68d/prepare.o"(.text)
        "*/resource_68d_overlay.o"(.text.x020081d8)
        "*/resource_68d_overlay.o"(.text.x020082b4)
        "*/resource_68d_overlay.o"(.text.x020083a0)
        "*/games/tla/src/field/overlay_68d/s04e8.o"(.text)
        "*/resource_68d_overlay.o"(.text.x02008510)
        "*/resource_68d_overlay.o"(.text.x02008550)
        "*/resource_68d_overlay.o"(.text.x0200858c)
        "*/games/tla/src/field/overlay_68d/import.o"(.text)
        "*/games/tla/src/field/overlay_68d/data.o"(.data)
        "*/resource_68d_overlay.o"(.rodata.x0200a620)
        "*/games/tla/src/field/overlay_68d/data2.o"(.data)
    }
    .bss : { "*/resource_68d_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
