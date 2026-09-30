/* resource_683: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_683/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_683/SCENE_TABLES.o"(.text)
        "*/resource_683_overlay.o"(.text.x0200804c)
        "*/resource_683_overlay.o"(.text.x0200808c)
        "*/resource_683_overlay.o"(.text.x02008098)
        "*/resource_683_overlay.o"(.text.x020080a4)
        "*/resource_683_overlay.o"(.text.x020080fc)
        "*/resource_683_overlay.o"(.text.x02008148)
        "*/resource_683_overlay.o"(.text.x0200814c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_683/IMPORT.o"(.text)
        "*/resource_683_overlay.o"(.rodata.x02008188)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
