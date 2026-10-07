SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_695/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_695/SCENE_TABLES.o"(.text)
        "*/resource_695_overlay.o"(.text.x02008098)
        "*/resource_695_overlay.o"(.text.x020080d4)
        "*/resource_695_overlay.o"(.text.x020080f0)
        "*/resource_695_overlay.o"(.text.x02008148)
        "*/resource_695_overlay.o"(.text.x02008184)
        "*/resource_695_overlay.o"(.text.x0200830c)
        "*/resource_695_overlay.o"(.text.x0200832c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_695/PLACES.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_695/SCENE_EVENTS.o"(.text)
        "*/resource_695_overlay.o"(.text.x02008430)
        "*/resource_695_overlay.o"(.text.x0200883c)
        "*/resource_695_overlay.o"(.text.x0200887c)
        "*/resource_695_overlay.o"(.text.x02008a24)
        "*/resource_695_overlay.o"(.text.x02008d84)
        "*/resource_695_overlay.o"(.text.x02008e04)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_695/EVENTS.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_695/IMPORT.o"(.text)
        "*/resource_695_overlay.o"(.rodata.x02009218)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
