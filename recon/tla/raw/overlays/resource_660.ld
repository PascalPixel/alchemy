SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_660/entry.o"(.text)
        "*/games/tla/src/field/overlay_660/scene_tables.o"(.text)
        "*/resource_660_overlay.o"(.text.x0200804c)
        "*/resource_660_overlay.o"(.text.x0200809c)
        "*/resource_660_overlay.o"(.text.x02008114)
        "*/resource_660_overlay.o"(.text.x0200818c)
        "*/resource_660_overlay.o"(.text.x02008230)
        "*/resource_660_overlay.o"(.text.x02008298)
        "*/resource_660_overlay.o"(.text.x02008318)
        "*/resource_660_overlay.o"(.text.x0200839c)
        "*/resource_660_overlay.o"(.text.x020083dc)
        "*/resource_660_overlay.o"(.text.x0200841c)
        "*/resource_660_overlay.o"(.text.x0200845c)
        "*/resource_660_overlay.o"(.text.x0200849c)
        "*/resource_660_overlay.o"(.text.x020084f0)
        "*/resource_660_overlay.o"(.text.x02008580)
        "*/games/tla/src/field/overlay_660/prepare.o"(.text)
        "*/resource_660_overlay.o"(.text.x02008710)
        "*/games/tla/src/field/overlay_660/import.o"(.text)
        "*/games/tla/src/field/overlay_660/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
