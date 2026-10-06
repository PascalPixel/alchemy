SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_649/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_649/SCENE_TABLES.o"(.text)
        "*/resource_649_overlay.o"(.text.x0200805c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_649/PREPARE.o"(.text)
        "*/resource_649_overlay.o"(.text.x02008278)
        "*/resource_649_overlay.o"(.text.x0200831c)
        "*/resource_649_overlay.o"(.text.x02008490)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_649/IMPORT.o"(.text)
        "*/resource_649_overlay.o"(.rodata.x0200864c)
    }
    .bss : { "*/resource_649_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
