/* resource_68d: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68D/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68D/SCENE_TABLES.o"(.text)
        "*/resource_68d_overlay.o"(.text.x02008054)
        "*/resource_68d_overlay.o"(.text.x020080c8)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68D/SCENE_EVENTS.o"(.text)
        "*/resource_68d_overlay.o"(.text.x020080f4)
        "*/resource_68d_overlay.o"(.text.x02008158)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68D/PREPARE.o"(.text)
        "*/resource_68d_overlay.o"(.text.x020081d8)
        "*/resource_68d_overlay.o"(.text.x020082b4)
        "*/resource_68d_overlay.o"(.text.x020083a0)
        "*/resource_68d_overlay.o"(.text.x020084e8)
        "*/resource_68d_overlay.o"(.text.x02008510)
        "*/resource_68d_overlay.o"(.text.x02008550)
        "*/resource_68d_overlay.o"(.text.x0200858c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68D/IMPORT.o"(.text)
        "*/resource_68d_overlay.o"(.rodata.x0200a12c)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
