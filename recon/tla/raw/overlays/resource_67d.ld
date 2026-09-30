/* resource_67d: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_67D/ENTRY.o"(.text)
        "*/resource_67d_overlay.o"(.text.x02008038)
        "*/resource_67d_overlay.o"(.text.x0200806c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_67D/SCENE_TABLES.o"(.text)
        "*/resource_67d_overlay.o"(.text.x020080b4)
        "*/resource_67d_overlay.o"(.text.x020080f0)
        "*/resource_67d_overlay.o"(.text.x0200812c)
        "*/resource_67d_overlay.o"(.text.x020081a8)
        "*/resource_67d_overlay.o"(.text.x02008220)
        "*/resource_67d_overlay.o"(.text.x02008298)
        "*/resource_67d_overlay.o"(.text.x02008358)
        "*/resource_67d_overlay.o"(.text.x02008378)
        "*/resource_67d_overlay.o"(.text.x02008400)
        "*/resource_67d_overlay.o"(.text.x02008458)
        "*/resource_67d_overlay.o"(.text.x0200851c)
        "*/resource_67d_overlay.o"(.text.x02008604)
        "*/resource_67d_overlay.o"(.text.x02008610)
        "*/resource_67d_overlay.o"(.text.x0200861c)
        "*/resource_67d_overlay.o"(.text.x02009d84)
        "*/resource_67d_overlay.o"(.text.x02009d8c)
        "*/resource_67d_overlay.o"(.text.x02009dd0)
        "*/resource_67d_overlay.o"(.text.x0200a00c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_67D/PREPARE.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_67D/IMPORT.o"(.text)
        "*/resource_67d_overlay.o"(.rodata.x0200a324)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
