/* resource_6b3: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B3/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B3/SCENE_TABLES.o"(.text)
        "*/resource_6b3_overlay.o"(.text.x0200805c)
        "*/resource_6b3_overlay.o"(.text.x02008098)
        "*/resource_6b3_overlay.o"(.text.x02008158)
        "*/resource_6b3_overlay.o"(.text.x02008244)
        "*/resource_6b3_overlay.o"(.text.x02008318)
        "*/resource_6b3_overlay.o"(.text.x02008788)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B3/IMPORT.o"(.text)
        "*/resource_6b3_overlay.o"(.rodata.x02008874)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
