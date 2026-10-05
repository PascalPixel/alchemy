SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B3/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B3/SCENE_TABLES.o"(.text)
        "*/resource_6b3_overlay.o"(.text.x0200805c)
        "*/resource_6b3_overlay.o"(.text.x02008098)
        "*/resource_6b3_overlay.o"(.text.x02008158)
        "*/resource_6b3_overlay.o"(.text.x02008244)
        "*/resource_6b3_overlay.o"(.text.x02008318)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B3/PREPARE.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B3/IMPORT.o"(.text)
        "*/resource_6b3_overlay.o"(.rodata.x02008874)
    }
    .bss : { "*/resource_6b3_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
