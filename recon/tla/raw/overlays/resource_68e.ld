SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_68e/entry.o"(.text)
        "*/games/tla/src/field/overlay_68e/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_68e/s0054.o"(.text)
        "*/resource_68e_overlay.o"(.text.x02008070)
        "*/games/tla/src/field/overlay_68e/s00d0.o"(.text)
        "*/games/tla/src/field/overlay_68e/scene_events.o"(.text)
        "*/resource_68e_overlay.o"(.text.x0200812c)
        "*/games/tla/src/field/overlay_68e/prepare.o"(.text)
        "*/resource_68e_overlay.o"(.text.x020081e4)
        "*/games/tla/src/field/overlay_68e/s049c.o"(.text)
        "*/resource_68e_overlay.o"(.text.x02008884)
        "*/games/tla/src/field/overlay_68e/s08f0.o"(.text)
        "*/resource_68e_overlay.o"(.text.x02008904)
        "*/resource_68e_overlay.o"(.text.x020090f4)
        "*/resource_68e_overlay.o"(.text.x02009184)
        "*/games/tla/src/field/overlay_68e/import.o"(.text)
        "*/resource_68e_overlay.o"(.rodata.x02009410)
    }
    .bss : { "*/resource_68e_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
