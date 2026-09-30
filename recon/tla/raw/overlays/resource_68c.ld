/* resource_68c: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68C/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68C/SCENE_TABLES.o"(.text)
        "*/resource_68c_overlay.o"(.text.x02008058)
        "*/resource_68c_overlay.o"(.text.x02008104)
        "*/resource_68c_overlay.o"(.text.x02008154)
        "*/resource_68c_overlay.o"(.text.x020081a0)
        "*/resource_68c_overlay.o"(.text.x020081ec)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68C/SCENE_EVENTS.o"(.text)
        "*/resource_68c_overlay.o"(.text.x02008284)
        "*/resource_68c_overlay.o"(.text.x02008318)
        "*/resource_68c_overlay.o"(.text.x0200831c)
        "*/resource_68c_overlay.o"(.text.x02008a5c)
        "*/resource_68c_overlay.o"(.text.x0200928c)
        "*/resource_68c_overlay.o"(.text.x020092dc)
        "*/resource_68c_overlay.o"(.text.x02009398)
        "*/resource_68c_overlay.o"(.text.x020093f0)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_68C/IMPORT.o"(.text)
        "*/resource_68c_overlay.o"(.rodata.x0200a92c)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
