SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_657/entry.o"(.text)
        "*/games/tla/src/field/overlay_657/scene_tables.o"(.text)
        "*/resource_657_overlay.o"(.text.x0200804c)
        "*/resource_657_overlay.o"(.text.x02008078)
        "*/resource_657_overlay.o"(.text.x0200813c)
        "*/resource_657_overlay.o"(.text.x02008168)
        "*/resource_657_overlay.o"(.text.x02008188)
        "*/resource_657_overlay.o"(.text.x020081cc)
        "*/resource_657_overlay.o"(.text.x02008210)
        "*/resource_657_overlay.o"(.text.x02008258)
        "*/resource_657_overlay.o"(.text.x02008288)
        "*/resource_657_overlay.o"(.text.x020082cc)
        "*/resource_657_overlay.o"(.text.x020086e4)
        "*/resource_657_overlay.o"(.text.x02008b84)
        "*/resource_657_overlay.o"(.text.x02008be0)
        "*/resource_657_overlay.o"(.text.x02008c80)
        "*/resource_657_overlay.o"(.text.x02008ce0)
        "*/resource_657_overlay.o"(.text.x02009064)
        "*/resource_657_overlay.o"(.text.x02009074)
        "*/resource_657_overlay.o"(.text.x02009178)
        "*/resource_657_overlay.o"(.text.x02009188)
        "*/resource_657_overlay.o"(.text.x02009210)
        "*/resource_657_overlay.o"(.text.x0200928c)
        "*/resource_657_overlay.o"(.text.x020094c0)
        "*/resource_657_overlay.o"(.text.x02009d60)
        "*/resource_657_overlay.o"(.text.x02009da0)
        "*/games/tla/src/field/overlay_657/import.o"(.text)
        "*/resource_657_overlay.o"(.rodata.x0200a0dc)
    }
    .bss : { "*/resource_657_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
