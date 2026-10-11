SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_687/entry.o"(.text)
        "*/games/tla/src/field/common/object/set_sprite_priority.o"(.text)
        "*/resource_687_overlay.o"(.text.x0200807e)
        "*/games/tla/src/field/common/object/prepare_object.o"(.text)
        "*/resource_687_overlay.o"(.text.x020080ce)
        "*/games/tla/src/field/overlay_687/object.o"(.text)
        "*/games/tla/src/field/common/effect/move.o"(.text)
        "*/resource_687_overlay.o"(.text.x0200815a)
        "*/resource_687_overlay.o"(.text.x0200815c)
        "*/games/tla/src/field/overlay_687/map.o"(.text)
        "*/games/tla/src/field/overlay_687/scene_tables.o"(.text)
        "*/resource_687_overlay.o"(.text.x0200843c)
        "*/resource_687_overlay.o"(.text.x0200854c)
        "*/resource_687_overlay.o"(.text.x02008818)
        "*/resource_687_overlay.o"(.text.x02008a80)
        "*/games/tla/src/field/overlay_687/clock.o"(.text)
        "*/resource_687_overlay.o"(.text.x02008c94)
        "*/resource_687_overlay.o"(.text.x02008da0)
        "*/resource_687_overlay.o"(.text.x02008ef0)
        "*/games/tla/src/field/overlay_687/enter.o"(.text)
        "*/resource_687_overlay.o"(.text.x020090a8)
        "*/resource_687_overlay.o"(.text.x02009108)
        "*/games/tla/src/field/overlay_687/blocks.o"(.text)
        "*/resource_687_overlay.o"(.text.x02009650)
        "*/games/tla/src/field/overlay_687/sink.o"(.text)
        "*/resource_687_overlay.o"(.text.x02009a14)
        "*/resource_687_overlay.o"(.text.x02009cdc)
        "*/games/tla/src/field/overlay_687/fade.o"(.text)
        "*/games/tla/src/field/overlay_687/import.o"(.text)
        "*/games/tla/src/field/overlay_687/data.o"(.data)
    }
    .bss : { "*/resource_687_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
