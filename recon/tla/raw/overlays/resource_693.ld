SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_693/entry.o"(.text)
        "*/games/tla/src/field/overlay_693/scene_tables.o"(.text)
        "*/resource_693_overlay.o"(.text.x02008064)
        "*/games/tla/src/field/overlay_693/s00e0.o"(.text)
        "*/resource_693_overlay.o"(.text.x02008120)
        "*/resource_693_overlay.o"(.text.x02008208)
        "*/resource_693_overlay.o"(.text.x020083e4)
        "*/resource_693_overlay.o"(.text.x02008640)
        "*/games/tla/src/field/overlay_693/gate.o"(.text)
        "*/resource_693_overlay.o"(.text.x0200897c)
        "*/games/tla/src/field/overlay_693/variant.o"(.text)
        "*/resource_693_overlay.o"(.text.x02008c14)
        "*/resource_693_overlay.o"(.text.x02008c8c)
        "*/resource_693_overlay.o"(.text.x02008d08)
        "*/resource_693_overlay.o"(.text.x02008d3c)
        "*/resource_693_overlay.o"(.text.x02008dc4)
        "*/resource_693_overlay.o"(.text.x02008ed0)
        "*/games/tla/src/field/overlay_693/import.o"(.text)
        "*/games/tla/src/field/overlay_693/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
