SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/DERI_HEYA/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/DERI_HEYA/SCENE_TABLES.o"(.text)
        "*/resource_652_overlay.o"(.text.x02008064)
        "*/resource_652_overlay.o"(.text.x020080ac)
        "*/games/THE LOST AGE/SRC/FIELD/DERI_HEYA/LINES.o"(.text)
        "*/resource_652_overlay.o"(.text.x02008218)
        "*/games/THE LOST AGE/SRC/FIELD/DERI_HEYA/MURABITO.o"(.text)
        "*/resource_652_overlay.o"(.text.x02008704)
        "*/resource_652_overlay.o"(.text.x02008730)
        "*/resource_652_overlay.o"(.text.x02008878)
        "*/resource_652_overlay.o"(.text.x02008a50)
        "*/resource_652_overlay.o"(.text.x02008c0c)
        "*/games/THE LOST AGE/SRC/FIELD/DERI_HEYA/TOUCHAKU.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/DERI_HEYA/IMPORT.o"(.text)
        "*/resource_652_overlay.o"(.rodata.x02008e04)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
