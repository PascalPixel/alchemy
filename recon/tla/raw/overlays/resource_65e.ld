SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65E/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65E/SCENE_TABLES.o"(.text)
        "*/resource_65e_overlay.o"(.text.x0200804c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65E/EVENTS.o"(.text)
        "*/resource_65e_overlay.o"(.text.x020080c8)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65E/EVENTS3.o"(.text)
        "*/resource_65e_overlay.o"(.text.x02008340)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65E/PREPARE.o"(.text)
        "*/resource_65e_overlay.o"(.text.x02008454)
        "*/resource_65e_overlay.o"(.text.x02008470)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65E/EVENTS2.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65E/IMPORT.o"(.text)
        "*/resource_65e_overlay.o"(.rodata.x02008654)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
