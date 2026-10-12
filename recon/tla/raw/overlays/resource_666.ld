SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_666/entry.o"(.text)
        "*/games/tla/src/field/overlay_666/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_666/s004c.o"(.text)
        "*/games/tla/src/field/overlay_666/s0084.o"(.text)
        "*/games/tla/src/field/overlay_666/s01ac.o"(.text)
        "*/games/tla/src/field/overlay_666/s0298.o"(.text)
        "*/games/tla/src/field/overlay_666/s02bc.o"(.text)
        "*/games/tla/src/field/overlay_666/s0444.o"(.text)
        "*/games/tla/src/field/overlay_666/s047c.o"(.text)
        "*/games/tla/src/field/overlay_666/s04b8.o"(.text)
        "*/games/tla/src/field/overlay_666/s04f4.o"(.text)
        "*/games/tla/src/field/overlay_666/s0578.o"(.text)
        "*/games/tla/src/field/overlay_666/s0598.o"(.text)
        "*/resource_666_overlay.o"(.text.x020085b8)
        "*/games/tla/src/field/overlay_666/prepare.o"(.text)
        "*/games/tla/src/field/overlay_666/s0738.o"(.text)
        "*/games/tla/src/field/overlay_666/import.o"(.text)
        "*/games/tla/src/field/overlay_666/data.o"(.data)
    }
    .bss : { "*/resource_666_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
