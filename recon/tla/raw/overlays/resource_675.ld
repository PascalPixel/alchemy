SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_675/entry.o"(.text)
        "*/resource_675_overlay.o"(.text.x02008038)
        "*/resource_675_overlay.o"(.text.x020080f4)
        "*/resource_675_overlay.o"(.text.x02008158)
        "*/resource_675_overlay.o"(.text.x02008178)
        "*/resource_675_overlay.o"(.text.x020081d4)
        "*/resource_675_overlay.o"(.text.x020081f8)
        "*/resource_675_overlay.o"(.text.x02008328)
        "*/games/tla/src/field/overlay_675/events2.o"(.text)
        "*/resource_675_overlay.o"(.text.x020083d4)
        "*/games/tla/src/field/overlay_675/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_675/s042c.o"(.text)
        "*/resource_675_overlay.o"(.text.x020084ac)
        "*/resource_675_overlay.o"(.text.x02008508)
        "*/resource_675_overlay.o"(.text.x02008544)
        "*/resource_675_overlay.o"(.text.x02008580)
        "*/resource_675_overlay.o"(.text.x020085fc)
        "*/resource_675_overlay.o"(.text.x0200869c)
        "*/games/tla/src/field/overlay_675/prepare.o"(.text)
        "*/resource_675_overlay.o"(.text.x020087dc)
        "*/resource_675_overlay.o"(.text.x02008848)
        "*/games/tla/src/field/overlay_675/events.o"(.text)
        "*/resource_675_overlay.o"(.text.x020089f8)
        "*/games/tla/src/field/overlay_675/events3.o"(.text)
        "*/resource_675_overlay.o"(.text.x020090ac)
        "*/resource_675_overlay.o"(.text.x0200915c)
        "*/games/tla/src/field/overlay_675/import.o"(.text)
        "*/resource_675_overlay.o"(.rodata.x0200934c)
    }
    .bss : { "*/resource_675_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
