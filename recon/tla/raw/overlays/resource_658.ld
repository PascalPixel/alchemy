SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_658/entry.o"(.text)
        "*/games/tla/src/field/overlay_658/scene_tables.o"(.text)
        "*/resource_658_overlay.o"(.text.x0200806c)
        "*/games/tla/src/field/overlay_658/events_a.o"(.text)
        "*/resource_658_overlay.o"(.text.x020082bc)
        "*/games/tla/src/field/overlay_658/events_b.o"(.text)
        "*/resource_658_overlay.o"(.text.x0200851c)
        "*/resource_658_overlay.o"(.text.x020085b8)
        "*/games/tla/src/field/overlay_658/events_c.o"(.text)
        "*/resource_658_overlay.o"(.text.x0200868c)
        "*/resource_658_overlay.o"(.text.x020086ac)
        "*/games/tla/src/field/overlay_658/prepare.o"(.text)
        "*/resource_658_overlay.o"(.text.x02008ac4)
        "*/resource_658_overlay.o"(.text.x02008f80)
        "*/resource_658_overlay.o"(.text.x020094fc)
        "*/resource_658_overlay.o"(.text.x020099d8)
        "*/games/tla/src/field/overlay_658/events_d.o"(.text)
        "*/games/tla/src/field/overlay_658/import.o"(.text)
        "*/resource_658_overlay.o"(.rodata.x0200ac98)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
