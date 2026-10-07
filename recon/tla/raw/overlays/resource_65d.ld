SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65D/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65D/SCENE_TABLES.o"(.text)
        "*/resource_65d_overlay.o"(.text.x02008040)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65D/EXITS.o"(.text)
        "*/resource_65d_overlay.o"(.text.x02008070)
        "*/resource_65d_overlay.o"(.text.x0200809c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65D/EVENTS_A.o"(.text)
        "*/resource_65d_overlay.o"(.text.x0200815c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65D/EVENTS_B.o"(.text)
        "*/resource_65d_overlay.o"(.text.x0200830c)
        "*/resource_65d_overlay.o"(.text.x02008438)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65D/EVENTS_C.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65D/IMPORT.o"(.text)
        "*/resource_65d_overlay.o"(.rodata.x020086f8)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
