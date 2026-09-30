/* resource_6b9: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B9/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B9/SCENE_TABLES.o"(.text)
        "*/resource_6b9_overlay.o"(.text.x02008054)
        "*/resource_6b9_overlay.o"(.text.x02008064)
        "*/resource_6b9_overlay.o"(.text.x0200807c)
        "*/resource_6b9_overlay.o"(.text.x02008088)
        "*/resource_6b9_overlay.o"(.text.x020080b8)
        "*/resource_6b9_overlay.o"(.text.x020080d8)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B9/SCENE_EVENTS.o"(.text)
        "*/resource_6b9_overlay.o"(.text.x02008104)
        "*/resource_6b9_overlay.o"(.text.x02008158)
        "*/resource_6b9_overlay.o"(.text.x0200815c)
        "*/resource_6b9_overlay.o"(.text.x020081e0)
        "*/resource_6b9_overlay.o"(.text.x0200829c)
        "*/resource_6b9_overlay.o"(.text.x02008358)
        "*/resource_6b9_overlay.o"(.text.x02008384)
        "*/resource_6b9_overlay.o"(.text.x020083a8)
        "*/resource_6b9_overlay.o"(.text.x02008418)
        "*/resource_6b9_overlay.o"(.text.x02008468)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B9/IMPORT.o"(.text)
        "*/resource_6b9_overlay.o"(.rodata.x02008608)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
