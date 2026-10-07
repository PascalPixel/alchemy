SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68E/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68E/SCENE_TABLES.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68E/S0054.o"(.text)
        "*/resource_68e_overlay.o"(.text.x02008070)
        "*/resource_68e_overlay.o"(.text.x020080d0)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68E/SCENE_EVENTS.o"(.text)
        "*/resource_68e_overlay.o"(.text.x0200812c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68E/PREPARE.o"(.text)
        "*/resource_68e_overlay.o"(.text.x020081e4)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68E/S049C.o"(.text)
        "*/resource_68e_overlay.o"(.text.x02008884)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68E/S08F0.o"(.text)
        "*/resource_68e_overlay.o"(.text.x02008904)
        "*/resource_68e_overlay.o"(.text.x020090f4)
        "*/resource_68e_overlay.o"(.text.x02009184)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68E/IMPORT.o"(.text)
        "*/resource_68e_overlay.o"(.rodata.x02009410)
    }
    .bss : { "*/resource_68e_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
