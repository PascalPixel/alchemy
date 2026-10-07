SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B7/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B7/SCENE_TABLES.o"(.text)
        "*/resource_6b7_overlay.o"(.text.x0200804c)
        "*/resource_6b7_overlay.o"(.text.x02008098)
        "*/resource_6b7_overlay.o"(.text.x020080fc)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B7/LEVEL.o"(.text)
        "*/resource_6b7_overlay.o"(.text.x0200817c)
        "*/resource_6b7_overlay.o"(.text.x020081a8)
        "*/resource_6b7_overlay.o"(.text.x020082cc)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B7/FINISH.o"(.text)
        "*/resource_6b7_overlay.o"(.text.x02008590)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B7/POSE.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B7/PREPARE.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B7/CALLS.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B7/IMPORT.o"(.text)
        "*/resource_6b7_overlay.o"(.rodata.x02008744)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
