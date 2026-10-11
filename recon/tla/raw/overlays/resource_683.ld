SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_683/entry.o"(.text)
        "*/games/tla/src/field/overlay_683/scene_tables.o"(.text)
        "*/resource_683_overlay.o"(.text.x0200804c)
        "*/resource_683_overlay.o"(.text.x0200808c)
        "*/resource_683_overlay.o"(.text.x02008098)
        "*/resource_683_overlay.o"(.text.x020080a4)
        "*/resource_683_overlay.o"(.text.x020080fc)
        "*/games/tla/src/field/overlay_683/prepare.o"(.text)
        "*/games/tla/src/field/overlay_683/import.o"(.text)
        "*/resource_683_overlay.o"(.rodata.x02008188)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
