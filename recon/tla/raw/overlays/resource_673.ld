SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_673/entry.o"(.text)
        "*/games/tla/src/field/overlay_673/events.o"(.text)
        "*/resource_673_overlay.o"(.text.x020088a0)
        "*/games/tla/src/field/overlay_673/events_b.o"(.text)
        "*/resource_673_overlay.o"(.text.x020089d4)
        "*/games/tla/src/field/overlay_673/s0d90.o"(.text)
        "*/games/tla/src/field/overlay_673/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_673/regions.o"(.text)
        "*/games/tla/src/field/overlay_673/exits.o"(.text)
        "*/games/tla/src/field/overlay_673/placements.o"(.text)
        "*/resource_673_overlay.o"(.text.x02008ecc)
        "*/games/tla/src/field/overlay_673/prepare.o"(.text)
        "*/games/tla/src/field/overlay_673/import.o"(.text)
        "*/resource_673_overlay.o"(.rodata.x0200928c)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
