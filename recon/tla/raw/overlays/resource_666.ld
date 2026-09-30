/* resource_666: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_666/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_666/SCENE_TABLES.o"(.text)
        "*/resource_666_overlay.o"(.text.x0200804c)
        "*/resource_666_overlay.o"(.text.x02008084)
        "*/resource_666_overlay.o"(.text.x020081ac)
        "*/resource_666_overlay.o"(.text.x020081fc)
        "*/resource_666_overlay.o"(.text.x0200824c)
        "*/resource_666_overlay.o"(.text.x02008298)
        "*/resource_666_overlay.o"(.text.x020082bc)
        "*/resource_666_overlay.o"(.text.x02008444)
        "*/resource_666_overlay.o"(.text.x0200847c)
        "*/resource_666_overlay.o"(.text.x020084b8)
        "*/resource_666_overlay.o"(.text.x020084f4)
        "*/resource_666_overlay.o"(.text.x02008540)
        "*/resource_666_overlay.o"(.text.x02008578)
        "*/resource_666_overlay.o"(.text.x02008598)
        "*/resource_666_overlay.o"(.text.x020085b8)
        "*/resource_666_overlay.o"(.text.x02008734)
        "*/resource_666_overlay.o"(.text.x02008738)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_666/IMPORT.o"(.text)
        "*/resource_666_overlay.o"(.rodata.x02008be4)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
