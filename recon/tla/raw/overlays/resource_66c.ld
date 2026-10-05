SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66C/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66C/SCENE_TABLES.o"(.text)
        "*/resource_66c_overlay.o"(.text.x0200804c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66C/SCENE_EVENTS.o"(.text)
        "*/resource_66c_overlay.o"(.text.x02008074)
        "*/resource_66c_overlay.o"(.text.x02008154)
        "*/resource_66c_overlay.o"(.text.x02008184)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66C/IMPORT.o"(.text)
        "*/resource_66c_overlay.o"(.rodata.x02008224)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
