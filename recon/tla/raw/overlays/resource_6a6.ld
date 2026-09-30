/* resource_6a6: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6A6/ENTRY.o"(.text)
        "*/resource_6a6_overlay.o"(.text.x02008038)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6A6/SCENE_TABLES.o"(.text)
        "*/resource_6a6_overlay.o"(.text.x02008050)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6A6/SCENE_TABLES_2.o"(.text)
        "*/resource_6a6_overlay.o"(.text.x02008080)
        "*/resource_6a6_overlay.o"(.text.x020080ac)
        "*/resource_6a6_overlay.o"(.text.x0200812c)
        "*/resource_6a6_overlay.o"(.text.x02008178)
        "*/resource_6a6_overlay.o"(.text.x02008198)
        "*/resource_6a6_overlay.o"(.text.x02008bf4)
        "*/resource_6a6_overlay.o"(.text.x02008c24)
        "*/resource_6a6_overlay.o"(.text.x02008c54)
        "*/resource_6a6_overlay.o"(.text.x02008c64)
        "*/resource_6a6_overlay.o"(.text.x02008c74)
        "*/resource_6a6_overlay.o"(.text.x02008c84)
        "*/resource_6a6_overlay.o"(.text.x02008cc0)
        "*/resource_6a6_overlay.o"(.text.x02008d00)
        "*/resource_6a6_overlay.o"(.text.x02008d40)
        "*/resource_6a6_overlay.o"(.text.x02008d4c)
        "*/resource_6a6_overlay.o"(.text.x02008da4)
        "*/resource_6a6_overlay.o"(.text.x02008e0c)
        "*/resource_6a6_overlay.o"(.text.x02008e24)
        "*/resource_6a6_overlay.o"(.text.x02008e3c)
        "*/resource_6a6_overlay.o"(.text.x02008e54)
        "*/resource_6a6_overlay.o"(.text.x02008eec)
        "*/resource_6a6_overlay.o"(.text.x02008f38)
        "*/resource_6a6_overlay.o"(.text.x02008ffc)
        "*/resource_6a6_overlay.o"(.text.x0200902c)
        "*/resource_6a6_overlay.o"(.text.x0200905c)
        "*/resource_6a6_overlay.o"(.text.x02009080)
        "*/resource_6a6_overlay.o"(.text.x020090a0)
        "*/resource_6a6_overlay.o"(.text.x020090c0)
        "*/resource_6a6_overlay.o"(.text.x020091ec)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6A6/IMPORT.o"(.text)
        "*/resource_6a6_overlay.o"(.rodata.x02009450)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
