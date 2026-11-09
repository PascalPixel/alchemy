SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/sea_of_time/entry.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/object/prepare_object.o"(.text)
        "*/games/tla/src/scene/sea_of_time/object.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/sea_of_time/s015c.o"(.text)
        "*/games/tla/src/scene/sea_of_time/map.o"(.text)
        "*/games/tla/src/scene/sea_of_time/scene_tables.o"(.text)
        "*/games/tla/src/scene/sea_of_time/s043c.o"(.text)
        "*/games/tla/src/scene/sea_of_time/s054c.o"(.text)
        "*/games/tla/src/scene/sea_of_time/s0818.o"(.text)
        "*/games/tla/src/scene/sea_of_time/s0a80.o"(.text)
        "*/games/tla/src/scene/sea_of_time/clock.o"(.text)
        "*/games/tla/src/scene/sea_of_time/s0c94.o"(.text)
        "*/games/tla/src/scene/sea_of_time/s0da0.o"(.text)
        "*/games/tla/src/scene/sea_of_time/s0ef0.o"(.text)
        "*/games/tla/src/scene/sea_of_time/enter.o"(.text)
        "*/games/tla/src/scene/sea_of_time/s10a8.o"(.text)
        "*/games/tla/src/scene/sea_of_time/s1108.o"(.text)
        "*/games/tla/src/scene/sea_of_time/blocks.o"(.text)
        "*/games/tla/src/scene/sea_of_time/s1650.o"(.text)
        "*/games/tla/src/scene/sea_of_time/sink.o"(.text)
        "*/games/tla/src/scene/sea_of_time/s1a14.o"(.text)
        "*/games/tla/src/scene/sea_of_time/s1cdc.o"(.text)
        "*/games/tla/src/scene/sea_of_time/fade.o"(.text)
        "*/games/tla/src/scene/sea_of_time/import.o"(.text)
        "*/games/tla/src/scene/sea_of_time/data.o"(.data)
    }
    .bss : { "*/resource_687_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
