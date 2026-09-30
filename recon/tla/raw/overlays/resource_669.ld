/* resource_669: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_669/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_669/SCENE_TABLES.o"(.text)
        "*/resource_669_overlay.o"(.text.x0200804c)
        "*/resource_669_overlay.o"(.text.x02008058)
        "*/resource_669_overlay.o"(.text.x0200808c)
        "*/resource_669_overlay.o"(.text.x02008128)
        "*/resource_669_overlay.o"(.text.x0200813c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_669/SCENE_EVENTS.o"(.text)
        "*/resource_669_overlay.o"(.text.x02008250)
        "*/resource_669_overlay.o"(.text.x020083e4)
        "*/resource_669_overlay.o"(.text.x020083e8)
        "*/resource_669_overlay.o"(.text.x02008438)
        "*/resource_669_overlay.o"(.text.x020084f4)
        "*/resource_669_overlay.o"(.text.x02008c5c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_669/IMPORT.o"(.text)
        "*/resource_669_overlay.o"(.rodata.x0200b2b0)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
