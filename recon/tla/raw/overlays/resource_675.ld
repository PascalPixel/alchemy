SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_675/entry.o"(.text)
        "*/resource_675_overlay.o"(.text.x02008038)
        "*/games/tla/src/field/overlay_675/s0158.o"(.text)
        "*/resource_675_overlay.o"(.text.x020081d4)
        "*/resource_675_overlay.o"(.text.x020081f8)
        "*/games/tla/src/field/overlay_675/s0328.o"(.text)
        "*/games/tla/src/field/overlay_675/events2.o"(.text)
        "*/games/tla/src/field/overlay_675/s03d4.o"(.text)
        "*/games/tla/src/field/overlay_675/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_675/s042c.o"(.text)
        "*/games/tla/src/field/overlay_675/s04ac.o"(.text)
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
        "*/games/tla/src/field/overlay_675/s115c.o"(.text)
        "*/games/tla/src/field/overlay_675/import.o"(.text)
        "*/games/tla/src/field/overlay_675/data.o"(.data)
    }
    .bss : { "*/resource_675_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
