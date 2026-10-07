SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6A6/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6A6/START.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6A6/SCENE_TABLES.o"(.text)
        "*/resource_6a6_overlay.o"(.text.x02008050)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6A6/EXITS.o"(.text)
        "*/resource_6a6_overlay.o"(.text.x02008080)
        "*/resource_6a6_overlay.o"(.text.x020080ac)
        "*/resource_6a6_overlay.o"(.text.x0200812c)
        "*/resource_6a6_overlay.o"(.text.x02008178)
        "*/resource_6a6_overlay.o"(.text.x02008198)
        "*/resource_6a6_overlay.o"(.text.x02008bf4)
        "*/resource_6a6_overlay.o"(.text.x02008c24)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6A6/LIGHTS.o"(.text)
        "*/resource_6a6_overlay.o"(.text.x02008c84)
        "*/resource_6a6_overlay.o"(.text.x02008cc0)
        "*/resource_6a6_overlay.o"(.text.x02008d00)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6A6/CALL_D40.o"(.text)
        "*/resource_6a6_overlay.o"(.text.x02008d4c)
        "*/resource_6a6_overlay.o"(.text.x02008da4)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6A6/SWITCH.o"(.text)
        "*/resource_6a6_overlay.o"(.text.x02008e54)
        "*/resource_6a6_overlay.o"(.text.x02008eec)
        "*/resource_6a6_overlay.o"(.text.x02008f38)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6A6/PUSH.o"(.text)
        "*/resource_6a6_overlay.o"(.text.x020090c0)
        "*/resource_6a6_overlay.o"(.text.x020091ec)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6A6/IMPORT.o"(.text)
        "*/resource_6a6_overlay.o"(.rodata.x02009450)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
