SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_67A/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_67A/SCENE_TABLES.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_67A/S0054.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_67A/SCENE_EVENTS.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_67A/S0070.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_67A/PREPARE.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_67A/IMPORT.o"(.text)
        "*/resource_67a_overlay.o"(.rodata.x02008090)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
