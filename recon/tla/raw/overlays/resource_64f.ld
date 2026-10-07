SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/SUHARA_GATE/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/SUHARA_GATE/EVENTS_A.o"(.text)
        "*/resource_64f_overlay.o"(.text.x02008044)
        "*/games/THE LOST AGE/SRC/FIELD/SUHARA_GATE/SCENE_TABLES.o"(.text)
        "*/resource_64f_overlay.o"(.text.x02008090)
        "*/resource_64f_overlay.o"(.text.x020080d0)
        "*/games/THE LOST AGE/SRC/FIELD/SUHARA_GATE/EVENTS_B.o"(.text)
        "*/resource_64f_overlay.o"(.text.x0200828c)
        "*/games/THE LOST AGE/SRC/FIELD/SUHARA_GATE/PREPARE.o"(.text)
        "*/resource_64f_overlay.o"(.text.x020083ac)
        "*/resource_64f_overlay.o"(.text.x02008494)
        "*/resource_64f_overlay.o"(.text.x020085e8)
        "*/games/THE LOST AGE/SRC/FIELD/SUHARA_GATE/EVENTS_C.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/SUHARA_GATE/IMPORT.o"(.text)
        "*/resource_64f_overlay.o"(.rodata.x02008aa8)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
