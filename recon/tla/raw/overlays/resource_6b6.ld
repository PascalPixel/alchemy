SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B6/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B6/SCENE_TABLES.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B6/EVENTS_A.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B6/SCENE_EVENTS.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B6/EVENTS_B.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B6/PREPARE.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B6/IMPORT.o"(.text)
        "*/resource_6b6_overlay.o"(.rodata.x020080c4)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
