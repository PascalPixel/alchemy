/* resource_685: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_685/ENTRY.o"(.text)
        "*/resource_685_overlay.o"(.text.x02008038)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_685/SCENE_TABLES.o"(.text)
        "*/resource_685_overlay.o"(.text.x02008070)
        "*/resource_685_overlay.o"(.text.x0200816c)
        "*/resource_685_overlay.o"(.text.x020081f4)
        "*/resource_685_overlay.o"(.text.x02008208)
        "*/resource_685_overlay.o"(.text.x02008304)
        "*/resource_685_overlay.o"(.text.x02008350)
        "*/resource_685_overlay.o"(.text.x0200838c)
        "*/resource_685_overlay.o"(.text.x020083bc)
        "*/resource_685_overlay.o"(.text.x020083ec)
        "*/resource_685_overlay.o"(.text.x02008464)
        "*/resource_685_overlay.o"(.text.x02008478)
        "*/resource_685_overlay.o"(.text.x020084d4)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_685/PREPARE.o"(.text)
        "*/resource_685_overlay.o"(.text.x020085e8)
        "*/resource_685_overlay.o"(.text.x0200867c)
        "*/resource_685_overlay.o"(.text.x02008734)
        "*/resource_685_overlay.o"(.text.x02008804)
        "*/resource_685_overlay.o"(.text.x02008978)
        "*/resource_685_overlay.o"(.text.x02008a34)
        "*/resource_685_overlay.o"(.text.x02008f48)
        "*/resource_685_overlay.o"(.text.x02009138)
        "*/resource_685_overlay.o"(.text.x0200915c)
        "*/resource_685_overlay.o"(.text.x02009774)
        "*/resource_685_overlay.o"(.text.x02009868)
        "*/resource_685_overlay.o"(.text.x0200a9f8)
        "*/resource_685_overlay.o"(.text.x0200ac88)
        "*/resource_685_overlay.o"(.text.x0200ad34)
        "*/resource_685_overlay.o"(.text.x0200ade4)
        "*/resource_685_overlay.o"(.text.x0200af58)
        "*/resource_685_overlay.o"(.text.x0200b078)
        "*/resource_685_overlay.o"(.text.x0200b164)
        "*/resource_685_overlay.o"(.text.x0200b20c)
        "*/resource_685_overlay.o"(.text.x0200b2a4)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_685/IMPORT.o"(.text)
        "*/resource_685_overlay.o"(.rodata.x0200b634)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
