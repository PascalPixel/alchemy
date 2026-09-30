/* resource_67a: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_67A/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_67A/SCENE_TABLES.o"(.text)
        "*/resource_67a_overlay.o"(.text.x02008054)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_67A/SCENE_EVENTS.o"(.text)
        "*/resource_67a_overlay.o"(.text.x02008070)
        "*/resource_67a_overlay.o"(.text.x02008084)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_67A/IMPORT.o"(.text)
        "*/resource_67a_overlay.o"(.rodata.x02008090)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
