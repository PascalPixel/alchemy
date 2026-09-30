/* resource_6a7: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6A7/ENTRY.o"(.text)
        "*/resource_6a7_overlay.o"(.text.x02008038)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6A7/SCENE_TABLES.o"(.text)
        "*/resource_6a7_overlay.o"(.text.x02008064)
        "*/resource_6a7_overlay.o"(.text.x020080a8)
        "*/resource_6a7_overlay.o"(.text.x02008108)
        "*/resource_6a7_overlay.o"(.text.x020081d8)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6A7/SCENE_EVENTS.o"(.text)
        "*/resource_6a7_overlay.o"(.text.x02008618)
        "*/resource_6a7_overlay.o"(.text.x02008680)
        "*/resource_6a7_overlay.o"(.text.x020086b0)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6A7/IMPORT.o"(.text)
        "*/resource_6a7_overlay.o"(.rodata.x02008b60)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
