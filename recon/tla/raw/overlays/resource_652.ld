SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/DERI_HEYA/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/DERI_HEYA/SCENE_TABLES.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/DERI_HEYA/LINES.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/DERI_HEYA/MURABITO.o"(.text)
        "*/resource_652_overlay.o"(.text.x02008704)
        "*/games/THE LOST AGE/SRC/FIELD/DERI_HEYA/ITEM.o"(.text)
        "*/resource_652_overlay.o"(.text.x02008878)
        "*/resource_652_overlay.o"(.text.x02008a50)
        "*/games/THE LOST AGE/SRC/FIELD/DERI_HEYA/TOUCHAKU.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/DERI_HEYA/IMPORT.o"(.text)
        "*/resource_652_overlay.o"(.rodata.x02008e04)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
