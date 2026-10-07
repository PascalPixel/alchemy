SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66F/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66F/START.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66F/SCENE_TABLES.o"(.text)
        "*/resource_66f_overlay.o"(.text.x02008058)
        "*/resource_66f_overlay.o"(.text.x020080ac)
        "*/resource_66f_overlay.o"(.text.x020080ec)
        "*/resource_66f_overlay.o"(.text.x02008124)
        "*/resource_66f_overlay.o"(.text.x0200815c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66F/ACTION.o"(.text)
        "*/resource_66f_overlay.o"(.text.x020081a0)
        "*/resource_66f_overlay.o"(.text.x0200836c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66F/FLAGS.o"(.text)
        "*/resource_66f_overlay.o"(.text.x02008440)
        "*/resource_66f_overlay.o"(.text.x02008478)
        "*/resource_66f_overlay.o"(.text.x020084b0)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66F/CELL_CALL.o"(.text)
        "*/resource_66f_overlay.o"(.text.x0200851c)
        "*/resource_66f_overlay.o"(.text.x020085ec)
        "*/resource_66f_overlay.o"(.text.x0200863c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66F/PREPARE.o"(.text)
        "*/resource_66f_overlay.o"(.text.x02008960)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66F/SPEED.o"(.text)
        "*/resource_66f_overlay.o"(.text.x02008af0)
        "*/resource_66f_overlay.o"(.text.x02008d80)
        "*/resource_66f_overlay.o"(.text.x02008da8)
        "*/resource_66f_overlay.o"(.text.x02008e28)
        "*/resource_66f_overlay.o"(.text.x02008ea8)
        "*/resource_66f_overlay.o"(.text.x02008ef4)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66F/TRIGGERS.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_66F/IMPORT.o"(.text)
        "*/resource_66f_overlay.o"(.rodata.x020091a4)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
