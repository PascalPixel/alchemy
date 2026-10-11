SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_680/entry.o"(.text)
        "*/games/tla/src/field/overlay_680/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_680/exits.o"(.text)
        "*/games/tla/src/field/overlay_680/events_a.o"(.text)
        "*/resource_680_overlay.o"(.text.x02008100)
        "*/games/tla/src/field/overlay_680/events_b.o"(.text)
        "*/resource_680_overlay.o"(.text.x02008164)
        "*/games/tla/src/field/overlay_680/events_c.o"(.text)
        "*/resource_680_overlay.o"(.text.x02008474)
        "*/resource_680_overlay.o"(.text.x02008528)
        "*/resource_680_overlay.o"(.text.x020085f0)
        "*/resource_680_overlay.o"(.text.x02008630)
        "*/games/tla/src/field/overlay_680/events_d.o"(.text)
        "*/resource_680_overlay.o"(.text.x020086ec)
        "*/games/tla/src/field/overlay_680/events_e.o"(.text)
        "*/resource_680_overlay.o"(.text.x02008ab4)
        "*/resource_680_overlay.o"(.text.x02008b2c)
        "*/resource_680_overlay.o"(.text.x02008bc0)
        "*/games/tla/src/field/overlay_680/events_f.o"(.text)
        "*/games/tla/src/field/overlay_680/import.o"(.text)
        "*/resource_680_overlay.o"(.rodata.x02008f2c)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
