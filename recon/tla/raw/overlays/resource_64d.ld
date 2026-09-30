/* resource_64d: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_IRIGUCHI/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_IRIGUCHI/SCENE_TABLES.o"(.text)
        "*/resource_64d_overlay.o"(.text.x02008054)
        "*/resource_64d_overlay.o"(.text.x0200809c)
        "*/resource_64d_overlay.o"(.text.x020080c8)
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_IRIGUCHI/SCENE_EVENTS.o"(.text)
        "*/resource_64d_overlay.o"(.text.x02008128)
        "*/resource_64d_overlay.o"(.text.x02008308)
        "*/resource_64d_overlay.o"(.text.x0200830c)
        "*/resource_64d_overlay.o"(.text.x02008440)
        "*/resource_64d_overlay.o"(.text.x02008510)
        "*/resource_64d_overlay.o"(.text.x02009a2c)
        "*/resource_64d_overlay.o"(.text.x02009bb0)
        "*/resource_64d_overlay.o"(.text.x02009c30)
        "*/resource_64d_overlay.o"(.text.x02009fd0)
        "*/resource_64d_overlay.o"(.text.x0200a3a4)
        "*/resource_64d_overlay.o"(.text.x0200a58c)
        "*/resource_64d_overlay.o"(.text.x0200a60c)
        "*/resource_64d_overlay.o"(.text.x0200a6f4)
        "*/resource_64d_overlay.o"(.text.x0200a7b0)
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_IRIGUCHI/IMPORT.o"(.text)
        "*/resource_64d_overlay.o"(.rodata.x0200aa6c)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
