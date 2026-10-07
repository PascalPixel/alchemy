SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66D/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66D/SCENE_TABLES.o"(.text)
        "*/resource_66d_overlay.o"(.text.x02008088)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66D/EVENTS_A.o"(.text)
        "*/resource_66d_overlay.o"(.text.x02008368)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66D/PREPARE.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66D/IMPORT.o"(.text)
        "*/resource_66d_overlay.o"(.rodata.x020085fc)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
