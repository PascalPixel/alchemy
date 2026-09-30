/* resource_660: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_660/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_660/SCENE_TABLES.o"(.text)
        "*/resource_660_overlay.o"(.text.x0200804c)
        "*/resource_660_overlay.o"(.text.x0200809c)
        "*/resource_660_overlay.o"(.text.x02008114)
        "*/resource_660_overlay.o"(.text.x0200818c)
        "*/resource_660_overlay.o"(.text.x02008230)
        "*/resource_660_overlay.o"(.text.x02008298)
        "*/resource_660_overlay.o"(.text.x02008318)
        "*/resource_660_overlay.o"(.text.x0200839c)
        "*/resource_660_overlay.o"(.text.x020083dc)
        "*/resource_660_overlay.o"(.text.x0200841c)
        "*/resource_660_overlay.o"(.text.x0200845c)
        "*/resource_660_overlay.o"(.text.x0200849c)
        "*/resource_660_overlay.o"(.text.x020084f0)
        "*/resource_660_overlay.o"(.text.x02008580)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_660/PREPARE.o"(.text)
        "*/resource_660_overlay.o"(.text.x02008710)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_660/IMPORT.o"(.text)
        "*/resource_660_overlay.o"(.rodata.x020088bc)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
