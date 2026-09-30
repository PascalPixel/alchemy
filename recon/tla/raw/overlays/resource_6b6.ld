/* resource_6b6: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B6/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B6/SCENE_TABLES.o"(.text)
        "*/resource_6b6_overlay.o"(.text.x02008054)
        "*/resource_6b6_overlay.o"(.text.x02008068)
        "*/resource_6b6_overlay.o"(.text.x0200807c)
        "*/resource_6b6_overlay.o"(.text.x02008090)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B6/SCENE_EVENTS.o"(.text)
        "*/resource_6b6_overlay.o"(.text.x020080ac)
        "*/resource_6b6_overlay.o"(.text.x020080b0)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B6/IMPORT.o"(.text)
        "*/resource_6b6_overlay.o"(.rodata.x020080c4)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
