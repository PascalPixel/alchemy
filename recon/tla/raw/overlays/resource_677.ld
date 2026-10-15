SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_677/entry.o"(.text)
        "*/games/tla/src/field/overlay_677/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_677/s004c.o"(.text)
        "*/games/tla/src/field/overlay_677/s008c.o"(.text)
        "*/games/tla/src/field/overlay_677/s00cc.o"(.text)
        "*/games/tla/src/field/overlay_677/s010c.o"(.text)
        "*/games/tla/src/field/overlay_677/s014c.o"(.text)
        "*/games/tla/src/field/overlay_677/s018c.o"(.text)
        "*/games/tla/src/field/overlay_677/s01b4.o"(.text)
        "*/games/tla/src/field/overlay_677/s01dc.o"(.text)
        "*/games/tla/src/field/overlay_677/s0204.o"(.text)
        "*/games/tla/src/field/overlay_677/s04e0.o"(.text)
        "*/games/tla/src/field/overlay_677/s077c.o"(.text)
        "*/resource_677_overlay.o"(.text.x0200886c)
        "*/games/tla/src/field/overlay_677/prepare.o"(.text)
        "*/games/tla/src/field/overlay_677/import.o"(.text)
        "*/games/tla/src/field/overlay_677/data.o"(.data)
        "*/games/tla/src/field/overlay_677/data2.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
