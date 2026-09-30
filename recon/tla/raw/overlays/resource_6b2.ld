/* resource_6b2: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B2/ENTRY.o"(.text)
        "*/resource_6b2_overlay.o"(.text.x02008038)
        "*/resource_6b2_overlay.o"(.text.x02008060)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B2/SCENE_TABLES.o"(.text)
        "*/resource_6b2_overlay.o"(.text.x020080f4)
        "*/resource_6b2_overlay.o"(.text.x020081b0)
        "*/resource_6b2_overlay.o"(.text.x020084c4)
        "*/resource_6b2_overlay.o"(.text.x0200851c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B2/IMPORT.o"(.text)
        "*/resource_6b2_overlay.o"(.rodata.x02008618)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
