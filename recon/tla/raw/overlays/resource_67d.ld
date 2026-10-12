SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_67d/entry.o"(.text)
        "*/games/tla/src/field/overlay_67d/s0038.o"(.text)
        "*/games/tla/src/field/overlay_67d/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_67d/s00b4.o"(.text)
        "*/resource_67d_overlay.o"(.text.x02008458)
        "*/games/tla/src/field/overlay_67d/s051c.o"(.text)
        "*/resource_67d_overlay.o"(.text.x0200861c)
        "*/games/tla/src/field/overlay_67d/s1d84.o"(.text)
        "*/resource_67d_overlay.o"(.text.x02009dd0)
        "*/resource_67d_overlay.o"(.text.x0200a00c)
        "*/games/tla/src/field/overlay_67d/prepare.o"(.text)
        "*/games/tla/src/field/overlay_67d/import.o"(.text)
        "*/games/tla/src/field/overlay_67d/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
