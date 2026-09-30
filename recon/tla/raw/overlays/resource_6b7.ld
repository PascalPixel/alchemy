/* resource_6b7: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B7/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B7/SCENE_TABLES.o"(.text)
        "*/resource_6b7_overlay.o"(.text.x0200804c)
        "*/resource_6b7_overlay.o"(.text.x02008098)
        "*/resource_6b7_overlay.o"(.text.x020080fc)
        "*/resource_6b7_overlay.o"(.text.x02008138)
        "*/resource_6b7_overlay.o"(.text.x02008160)
        "*/resource_6b7_overlay.o"(.text.x0200817c)
        "*/resource_6b7_overlay.o"(.text.x020081a8)
        "*/resource_6b7_overlay.o"(.text.x020082cc)
        "*/resource_6b7_overlay.o"(.text.x02008570)
        "*/resource_6b7_overlay.o"(.text.x02008590)
        "*/resource_6b7_overlay.o"(.text.x020085d8)
        "*/resource_6b7_overlay.o"(.text.x02008610)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B7/PREPARE.o"(.text)
        "*/resource_6b7_overlay.o"(.text.x02008624)
        "*/resource_6b7_overlay.o"(.text.x0200862c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B7/IMPORT.o"(.text)
        "*/resource_6b7_overlay.o"(.rodata.x02008744)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
