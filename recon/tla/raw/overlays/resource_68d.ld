SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_68d/entry.o"(.text)
        "*/games/tla/src/field/overlay_68d/scene_tables.o"(.text)
        "*/resource_68d_overlay.o"(.text.x020080c8)
        "*/games/tla/src/field/overlay_68d/scene_events.o"(.text)
        "*/resource_68d_overlay.o"(.text.x020080f4)
        "*/resource_68d_overlay.o"(.text.x02008158)
        "*/games/tla/src/field/overlay_68d/prepare.o"(.text)
        "*/resource_68d_overlay.o"(.text.x020081d8)
        "*/resource_68d_overlay.o"(.text.x020082b4)
        "*/resource_68d_overlay.o"(.text.x020083a0)
        "*/resource_68d_overlay.o"(.text.x020084e8)
        "*/resource_68d_overlay.o"(.text.x02008510)
        "*/resource_68d_overlay.o"(.text.x02008550)
        "*/resource_68d_overlay.o"(.text.x0200858c)
        "*/games/tla/src/field/overlay_68d/import.o"(.text)
        "*/resource_68d_overlay.o"(.rodata.x0200a12c)
    }
    .bss : { "*/resource_68d_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
