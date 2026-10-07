SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_687/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/COMMON/OBJECT/SET_SPRITE_PRIORITY.o"(.text)
        "*/resource_687_overlay.o"(.text.x0200807e)
        "*/games/THE LOST AGE/SRC/FIELD/COMMON/OBJECT/PREPARE_OBJECT.o"(.text)
        "*/resource_687_overlay.o"(.text.x020080ce)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_687/OBJECT.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/resource_687_overlay.o"(.text.x0200815a)
        "*/resource_687_overlay.o"(.text.x0200815c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_687/MAP.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_687/SCENE_TABLES.o"(.text)
        "*/resource_687_overlay.o"(.text.x02008410)
        "*/resource_687_overlay.o"(.text.x0200843c)
        "*/resource_687_overlay.o"(.text.x0200854c)
        "*/resource_687_overlay.o"(.text.x02008818)
        "*/resource_687_overlay.o"(.text.x02008a80)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_687/CLOCK.o"(.text)
        "*/resource_687_overlay.o"(.text.x02008c94)
        "*/resource_687_overlay.o"(.text.x02008da0)
        "*/resource_687_overlay.o"(.text.x02008ef0)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_687/ENTER.o"(.text)
        "*/resource_687_overlay.o"(.text.x020090a8)
        "*/resource_687_overlay.o"(.text.x02009108)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_687/BLOCKS.o"(.text)
        "*/resource_687_overlay.o"(.text.x0200960c)
        "*/resource_687_overlay.o"(.text.x02009650)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_687/SINK.o"(.text)
        "*/resource_687_overlay.o"(.text.x02009a14)
        "*/resource_687_overlay.o"(.text.x02009cdc)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_687/FADE.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_687/IMPORT.o"(.text)
        "*/resource_687_overlay.o"(.rodata.x0200a05c)
    }
    .bss : { "*/resource_687_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
