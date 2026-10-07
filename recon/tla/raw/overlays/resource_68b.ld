SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68B/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68B/DISTANCE.o"(.text)
        "*/resource_68b_overlay.o"(.text.x02008070)
        "*/resource_68b_overlay.o"(.text.x020080e8)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68B/SCENE_TABLES.o"(.text)
        "*/resource_68b_overlay.o"(.text.x02008308)
        "*/resource_68b_overlay.o"(.text.x02008398)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68B/EVENTS_A.o"(.text)
        "*/resource_68b_overlay.o"(.text.x020084a8)
        "*/resource_68b_overlay.o"(.text.x02008538)
        "*/resource_68b_overlay.o"(.text.x02008560)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68B/EVENTS_B.o"(.text)
        "*/resource_68b_overlay.o"(.text.x0200862c)
        "*/resource_68b_overlay.o"(.text.x020088e8)
        "*/resource_68b_overlay.o"(.text.x02008934)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68B/SCENE_EVENTS.o"(.text)
        "*/resource_68b_overlay.o"(.text.x02008a6c)
        "*/resource_68b_overlay.o"(.text.x02008ad8)
        "*/resource_68b_overlay.o"(.text.x02008b28)
        "*/resource_68b_overlay.o"(.text.x02008bbc)
        "*/resource_68b_overlay.o"(.text.x02008bdc)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68B/PREPARE.o"(.text)
        "*/resource_68b_overlay.o"(.text.x02008e7c)
        "*/resource_68b_overlay.o"(.text.x0200909c)
        "*/resource_68b_overlay.o"(.text.x02009160)
        "*/resource_68b_overlay.o"(.text.x02009b3c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68B/IMPORT.o"(.text)
        "*/resource_68b_overlay.o"(.rodata.x02009d84)
    }
    .bss : { "*/resource_68b_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
