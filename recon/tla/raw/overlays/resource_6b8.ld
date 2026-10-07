SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B8/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B8/SCENE_TABLES.o"(.text)
        "*/resource_6b8_overlay.o"(.text.x02008624)
        "*/resource_6b8_overlay.o"(.text.x02008650)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B8/SCENE_EVENTS.o"(.text)
        "*/resource_6b8_overlay.o"(.text.x0200879c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B8/PREPARE.o"(.text)
        "*/resource_6b8_overlay.o"(.text.x0200892c)
        "*/resource_6b8_overlay.o"(.text.x02008950)
        "*/resource_6b8_overlay.o"(.text.x020089c0)
        "*/resource_6b8_overlay.o"(.text.x02008a10)
        "*/resource_6b8_overlay.o"(.text.x02008ad0)
        "*/resource_6b8_overlay.o"(.text.x02008c4c)
        "*/resource_6b8_overlay.o"(.text.x02008d4c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B8/IMPORT.o"(.text)
        "*/resource_6b8_overlay.o"(.rodata.x02008f44)
    }
    .bss : { "*/resource_6b8_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
