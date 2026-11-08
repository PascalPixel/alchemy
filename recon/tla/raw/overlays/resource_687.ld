SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/sea_of_time/entry.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/resource_687_overlay.o"(.text.x0200807e)
        "*/games/tla/src/field/object/prepare_object.o"(.text)
        "*/resource_687_overlay.o"(.text.x020080ce)
        "*/games/tla/src/scene/sea_of_time/object.o"(.text)
        "*/recon/tla/raw/src/field/field_effect/move.o"(.text)
        "*/resource_687_overlay.o"(.text.x0200815a)
        "*/recon/tla/raw/src/scene/sea_of_time/s015c.o"(.text)
        "*/recon/tla/raw/src/scene/sea_of_time/map.o"(.text)
        "*/games/tla/src/scene/sea_of_time/scene_tables.o"(.text)
        "*/recon/tla/raw/src/scene/sea_of_time/s043c.o"(.text)
        "*/recon/tla/raw/src/scene/sea_of_time/s054c.o"(.text)
        "*/recon/tla/raw/src/scene/sea_of_time/s0818.o"(.text)
        "*/recon/tla/raw/src/scene/sea_of_time/s0a80.o"(.text)
        "*/recon/tla/raw/src/scene/sea_of_time/clock.o"(.text)
        "*/recon/tla/raw/src/scene/sea_of_time/s0c94.o"(.text)
        "*/resource_687_overlay.o"(.text.x02008da0)
        "*/recon/tla/raw/src/scene/sea_of_time/s0ef0.o"(.text)
        "*/recon/tla/raw/src/scene/sea_of_time/enter.o"(.text)
        "*/recon/tla/raw/src/scene/sea_of_time/s10a8.o"(.text)
        "*/resource_687_overlay.o"(.text.x02009108)
        "*/recon/tla/raw/src/scene/sea_of_time/blocks.o"(.text)
        "*/recon/tla/raw/src/scene/sea_of_time/s1650.o"(.text)
        "*/recon/tla/raw/src/scene/sea_of_time/sink.o"(.text)
        "*/recon/tla/raw/src/scene/sea_of_time/s1a14.o"(.text)
        "*/recon/tla/raw/src/scene/sea_of_time/s1cdc.o"(.text)
        "*/recon/tla/raw/src/scene/sea_of_time/fade.o"(.text)
        "*/games/tla/src/scene/sea_of_time/import.o"(.text)
        "*/games/tla/src/scene/sea_of_time/data.o"(.data)
    }
    .bss : { "*/resource_687_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
