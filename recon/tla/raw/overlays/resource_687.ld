/* resource_687: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_687/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/COMMON/OBJECT/SET_SPRITE_PRIORITY.o"(.text)
        "*/resource_687_overlay.o"(.text.x0200807e)
        "*/resource_687_overlay.o"(.text.x02008080)
        "*/resource_687_overlay.o"(.text.x020080d0)
        "*/resource_687_overlay.o"(.text.x02008124)
        "*/resource_687_overlay.o"(.text.x0200815c)
        "*/resource_687_overlay.o"(.text.x0200833c)
        "*/resource_687_overlay.o"(.text.x0200834c)
        "*/resource_687_overlay.o"(.text.x02008388)
        "*/resource_687_overlay.o"(.text.x020083d0)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_687/SCENE_TABLES.o"(.text)
        "*/resource_687_overlay.o"(.text.x02008410)
        "*/resource_687_overlay.o"(.text.x0200843c)
        "*/resource_687_overlay.o"(.text.x0200854c)
        "*/resource_687_overlay.o"(.text.x02008818)
        "*/resource_687_overlay.o"(.text.x02008a80)
        "*/resource_687_overlay.o"(.text.x02008c50)
        "*/resource_687_overlay.o"(.text.x02008c94)
        "*/resource_687_overlay.o"(.text.x02008da0)
        "*/resource_687_overlay.o"(.text.x02008ef0)
        "*/resource_687_overlay.o"(.text.x02009068)
        "*/resource_687_overlay.o"(.text.x020090a8)
        "*/resource_687_overlay.o"(.text.x02009108)
        "*/resource_687_overlay.o"(.text.x0200941c)
        "*/resource_687_overlay.o"(.text.x02009480)
        "*/resource_687_overlay.o"(.text.x020094b4)
        "*/resource_687_overlay.o"(.text.x020094ec)
        "*/resource_687_overlay.o"(.text.x02009524)
        "*/resource_687_overlay.o"(.text.x0200955c)
        "*/resource_687_overlay.o"(.text.x02009594)
        "*/resource_687_overlay.o"(.text.x020095cc)
        "*/resource_687_overlay.o"(.text.x020095ec)
        "*/resource_687_overlay.o"(.text.x0200960c)
        "*/resource_687_overlay.o"(.text.x02009650)
        "*/resource_687_overlay.o"(.text.x020098a8)
        "*/resource_687_overlay.o"(.text.x02009928)
        "*/resource_687_overlay.o"(.text.x02009a14)
        "*/resource_687_overlay.o"(.text.x02009cdc)
        "*/resource_687_overlay.o"(.text.x02009ec8)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_687/IMPORT.o"(.text)
        "*/resource_687_overlay.o"(.rodata.x0200a05c)
    }
    .bss : { "*/resource_687_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
