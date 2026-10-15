SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_657/entry.o"(.text)
        "*/games/tla/src/field/overlay_657/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_657/s004c.o"(.text)
        "*/games/tla/src/field/overlay_657/s0078.o"(.text)
        "*/games/tla/src/field/overlay_657/s013c.o"(.text)
        "*/games/tla/src/field/overlay_657/s0168.o"(.text)
        "*/games/tla/src/field/overlay_657/s0188.o"(.text)
        "*/games/tla/src/field/overlay_657/s01cc.o"(.text)
        "*/games/tla/src/field/overlay_657/s0210.o"(.text)
        "*/games/tla/src/field/overlay_657/s0258.o"(.text)
        "*/resource_657_overlay.o"(.text.x020082cc)
        "*/games/tla/src/field/overlay_657/s0b84.o"(.text)
        "*/games/tla/src/field/overlay_657/s0be0.o"(.text)
        "*/games/tla/src/field/overlay_657/s0c80.o"(.text)
        "*/resource_657_overlay.o"(.text.x02008ce0)
        "*/games/tla/src/field/overlay_657/s1064.o"(.text)
        "*/games/tla/src/field/overlay_657/s1074.o"(.text)
        "*/games/tla/src/field/overlay_657/s1178.o"(.text)
        "*/games/tla/src/field/overlay_657/s1188.o"(.text)
        "*/games/tla/src/field/overlay_657/s1210.o"(.text)
        "*/resource_657_overlay.o"(.text.x020094c0)
        "*/games/tla/src/field/overlay_657/s1d60.o"(.text)
        "*/games/tla/src/field/overlay_657/s1da0.o"(.text)
        "*/games/tla/src/field/overlay_657/import.o"(.text)
        "*/games/tla/src/field/overlay_657/data.o"(.data)
    }
    .bss : { "*/resource_657_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
