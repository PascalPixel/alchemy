/* resource_6b5: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B5/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B5/SCENE_TABLES.o"(.text)
        "*/resource_6b5_overlay.o"(.text.x02008044)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B5/SCENE_TABLES_2.o"(.text)
        "*/resource_6b5_overlay.o"(.text.x0200806c)
        "*/resource_6b5_overlay.o"(.text.x02008114)
        "*/resource_6b5_overlay.o"(.text.x02008134)
        "*/resource_6b5_overlay.o"(.text.x020082d8)
        "*/resource_6b5_overlay.o"(.text.x02008344)
        "*/resource_6b5_overlay.o"(.text.x0200839c)
        "*/resource_6b5_overlay.o"(.text.x02008538)
        "*/resource_6b5_overlay.o"(.text.x02008578)
        "*/resource_6b5_overlay.o"(.text.x020087b8)
        "*/resource_6b5_overlay.o"(.text.x02008864)
        "*/resource_6b5_overlay.o"(.text.x02008bb0)
        "*/resource_6b5_overlay.o"(.text.x02008da0)
        "*/resource_6b5_overlay.o"(.text.x02008e4c)
        "*/resource_6b5_overlay.o"(.text.x02008f50)
        "*/resource_6b5_overlay.o"(.text.x02008f6c)
        "*/resource_6b5_overlay.o"(.text.x02008fbc)
        "*/resource_6b5_overlay.o"(.text.x02009130)
        "*/resource_6b5_overlay.o"(.text.x02009210)
        "*/resource_6b5_overlay.o"(.text.x0200922c)
        "*/resource_6b5_overlay.o"(.text.x02009304)
        "*/resource_6b5_overlay.o"(.text.x02009388)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B5/SCENE_EVENTS.o"(.text)
        "*/resource_6b5_overlay.o"(.text.x020093ec)
        "*/resource_6b5_overlay.o"(.text.x02009400)
        "*/resource_6b5_overlay.o"(.text.x02009414)
        "*/resource_6b5_overlay.o"(.text.x0200945c)
        "*/resource_6b5_overlay.o"(.text.x020098c8)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B5/IMPORT.o"(.text)
        "*/resource_6b5_overlay.o"(.rodata.x02009ae4)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
