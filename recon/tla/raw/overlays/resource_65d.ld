/* resource_65d: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65D/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65D/SCENE_TABLES.o"(.text)
        "*/resource_65d_overlay.o"(.text.x02008040)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65D/SCENE_TABLES_2.o"(.text)
        "*/resource_65d_overlay.o"(.text.x02008070)
        "*/resource_65d_overlay.o"(.text.x0200809c)
        "*/resource_65d_overlay.o"(.text.x0200814c)
        "*/resource_65d_overlay.o"(.text.x0200815c)
        "*/resource_65d_overlay.o"(.text.x02008188)
        "*/resource_65d_overlay.o"(.text.x020081e0)
        "*/resource_65d_overlay.o"(.text.x02008218)
        "*/resource_65d_overlay.o"(.text.x020082f8)
        "*/resource_65d_overlay.o"(.text.x0200830c)
        "*/resource_65d_overlay.o"(.text.x02008438)
        "*/resource_65d_overlay.o"(.text.x020084ec)
        "*/resource_65d_overlay.o"(.text.x02008514)
        "*/resource_65d_overlay.o"(.text.x02008544)
        "*/resource_65d_overlay.o"(.text.x02008564)
        "*/resource_65d_overlay.o"(.text.x020085d4)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_65D/IMPORT.o"(.text)
        "*/resource_65d_overlay.o"(.rodata.x020086f8)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
