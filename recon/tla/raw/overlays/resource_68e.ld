/* resource_68e: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68E/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68E/SCENE_TABLES.o"(.text)
        "*/resource_68e_overlay.o"(.text.x02008054)
        "*/resource_68e_overlay.o"(.text.x02008070)
        "*/resource_68e_overlay.o"(.text.x020080d0)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68E/SCENE_EVENTS.o"(.text)
        "*/resource_68e_overlay.o"(.text.x0200812c)
        "*/resource_68e_overlay.o"(.text.x020081e0)
        "*/resource_68e_overlay.o"(.text.x020081e4)
        "*/resource_68e_overlay.o"(.text.x0200849c)
        "*/resource_68e_overlay.o"(.text.x02008884)
        "*/resource_68e_overlay.o"(.text.x020088f0)
        "*/resource_68e_overlay.o"(.text.x02008904)
        "*/resource_68e_overlay.o"(.text.x020090f4)
        "*/resource_68e_overlay.o"(.text.x02009184)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68E/IMPORT.o"(.text)
        "*/resource_68e_overlay.o"(.rodata.x02009410)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
