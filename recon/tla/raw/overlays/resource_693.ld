SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_693/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_693/SCENE_TABLES.o"(.text)
        "*/resource_693_overlay.o"(.text.x02008064)
        "*/resource_693_overlay.o"(.text.x020080e0)
        "*/resource_693_overlay.o"(.text.x02008120)
        "*/resource_693_overlay.o"(.text.x02008208)
        "*/resource_693_overlay.o"(.text.x020083e4)
        "*/resource_693_overlay.o"(.text.x02008640)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_693/GATE.o"(.text)
        "*/resource_693_overlay.o"(.text.x0200897c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_693/VARIANT.o"(.text)
        "*/resource_693_overlay.o"(.text.x02008c14)
        "*/resource_693_overlay.o"(.text.x02008c8c)
        "*/resource_693_overlay.o"(.text.x02008d08)
        "*/resource_693_overlay.o"(.text.x02008d3c)
        "*/resource_693_overlay.o"(.text.x02008dc4)
        "*/resource_693_overlay.o"(.text.x02008ed0)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_693/IMPORT.o"(.text)
        "*/resource_693_overlay.o"(.rodata.x02009130)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
