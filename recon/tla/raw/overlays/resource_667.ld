/* resource_667: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_667/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_667/SCENE_TABLES.o"(.text)
        "*/resource_667_overlay.o"(.text.x0200804c)
        "*/resource_667_overlay.o"(.text.x0200806c)
        "*/resource_667_overlay.o"(.text.x020080b8)
        "*/resource_667_overlay.o"(.text.x02008150)
        "*/resource_667_overlay.o"(.text.x0200819c)
        "*/resource_667_overlay.o"(.text.x02008238)
        "*/resource_667_overlay.o"(.text.x02008370)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_667/SCENE_EVENTS.o"(.text)
        "*/resource_667_overlay.o"(.text.x02008514)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_667/PREPARE.o"(.text)
        "*/resource_667_overlay.o"(.text.x02008658)
        "*/resource_667_overlay.o"(.text.x02009488)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_667/IMPORT.o"(.text)
        "*/resource_667_overlay.o"(.rodata.x02009600)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
