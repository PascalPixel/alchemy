/* resource_684: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_684/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_684/SCENE_TABLES.o"(.text)
        "*/resource_684_overlay.o"(.text.x02008054)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_684/SCENE_EVENTS.o"(.text)
        "*/resource_684_overlay.o"(.text.x02008064)
        "*/resource_684_overlay.o"(.text.x020080a4)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_684/IMPORT.o"(.text)
        "*/resource_684_overlay.o"(.rodata.x020080c0)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
