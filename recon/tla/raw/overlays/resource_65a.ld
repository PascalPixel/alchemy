SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65A/ENTRY.o"(.text)
        "*/resource_65a_overlay.o"(.text.x02008038)
        "*/resource_65a_overlay.o"(.text.x0200814c)
        "*/resource_65a_overlay.o"(.text.x02008270)
        "*/resource_65a_overlay.o"(.text.x02008318)
        "*/resource_65a_overlay.o"(.text.x02008358)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65A/SCENE_TABLES.o"(.text)
        "*/resource_65a_overlay.o"(.text.x02008388)
        "*/resource_65a_overlay.o"(.text.x020083ec)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65A/EVENTS_A.o"(.text)
        "*/resource_65a_overlay.o"(.text.x02008568)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65A/EVENTS_D.o"(.text)
        "*/resource_65a_overlay.o"(.text.x02008874)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65A/EVENTS_B.o"(.text)
        "*/resource_65a_overlay.o"(.text.x0200893c)
        "*/resource_65a_overlay.o"(.text.x02008958)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65A/EVENTS_F.o"(.text)
        "*/resource_65a_overlay.o"(.text.x02008a88)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65A/EVENTS_C.o"(.text)
        "*/resource_65a_overlay.o"(.text.x02008d54)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65A/PREPARE.o"(.text)
        "*/resource_65a_overlay.o"(.text.x02008dcc)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65A/EVENTS_E.o"(.text)
        "*/resource_65a_overlay.o"(.text.x0200902c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65A/IMPORT.o"(.text)
        "*/resource_65a_overlay.o"(.rodata.x0200921c)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
