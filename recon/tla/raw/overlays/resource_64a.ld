/* resource_64a: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_64A/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_64A/SCENE_TABLES.o"(.text)
        "*/resource_64a_overlay.o"(.text.x0200805c)
        "*/resource_64a_overlay.o"(.text.x0200815c)
        "*/resource_64a_overlay.o"(.text.x020081f4)
        "*/resource_64a_overlay.o"(.text.x020082e0)
        "*/resource_64a_overlay.o"(.text.x02008328)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_64A/PREPARE.o"(.text)
        "*/resource_64a_overlay.o"(.text.x02008a70)
        "*/resource_64a_overlay.o"(.text.x02008b24)
        "*/resource_64a_overlay.o"(.text.x02008db4)
        "*/resource_64a_overlay.o"(.text.x02008e44)
        "*/resource_64a_overlay.o"(.text.x02009804)
        "*/resource_64a_overlay.o"(.text.x0200987c)
        "*/resource_64a_overlay.o"(.text.x020098e0)
        "*/resource_64a_overlay.o"(.text.x02009c7c)
        "*/resource_64a_overlay.o"(.text.x0200a068)
        "*/resource_64a_overlay.o"(.text.x0200a17c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_64A/IMPORT.o"(.text)
        "*/resource_64a_overlay.o"(.rodata.x0200a410)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
