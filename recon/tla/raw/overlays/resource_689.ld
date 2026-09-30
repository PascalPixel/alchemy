/* resource_689: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_689/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_689/SCENE_TABLES.o"(.text)
        "*/resource_689_overlay.o"(.text.x02008044)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_689/SCENE_TABLES_2.o"(.text)
        "*/resource_689_overlay.o"(.text.x0200806c)
        "*/resource_689_overlay.o"(.text.x02008158)
        "*/resource_689_overlay.o"(.text.x02008254)
        "*/resource_689_overlay.o"(.text.x020082dc)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_689/SCENE_EVENTS.o"(.text)
        "*/resource_689_overlay.o"(.text.x020082f8)
        "*/resource_689_overlay.o"(.text.x02008360)
        "*/resource_689_overlay.o"(.text.x020083cc)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_689/IMPORT.o"(.text)
        "*/resource_689_overlay.o"(.rodata.x0200854c)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
