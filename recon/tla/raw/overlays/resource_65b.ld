/* resource_65b: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65B/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65B/SCENE_TABLES.o"(.text)
        "*/resource_65b_overlay.o"(.text.x02008054)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65B/SCENE_EVENTS.o"(.text)
        "*/resource_65b_overlay.o"(.text.x020080c8)
        "*/resource_65b_overlay.o"(.text.x02008224)
        "*/resource_65b_overlay.o"(.text.x02008280)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65B/PREPARE.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65B/IMPORT.o"(.text)
        "*/resource_65b_overlay.o"(.rodata.x02008348)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
