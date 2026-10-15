SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/debug/shop_test/entry.o"(.text)
        "*/games/tla/src/debug/shop_test/scene_tables.o"(.text)
        "*/resource_6ba_overlay.o"(.text.x02008054)
        "*/games/tla/src/debug/shop_test/menu_calls.o"(.text)
        "*/games/tla/src/debug/shop_test/s01d4.o"(.text)
        "*/games/tla/src/debug/shop_test/s02a8.o"(.text)
        "*/games/tla/src/debug/shop_test/loop_call.o"(.text)
        "*/games/tla/src/debug/shop_test/treasure.o"(.text)
        "*/resource_6ba_overlay.o"(.text.x02008520)
        "*/games/tla/src/debug/shop_test/djinn.o"(.text)
        "*/games/tla/src/debug/shop_test/give.o"(.text)
        "*/games/tla/src/debug/shop_test/scene_events.o"(.text)
        "*/games/tla/src/debug/shop_test/prepare.o"(.text)
        "*/resource_6ba_overlay.o"(.text.x02008704)
        "*/games/tla/src/debug/shop_test/level.o"(.text)
        "*/resource_6ba_overlay.o"(.text.x02008848)
        "*/games/tla/src/debug/shop_test/forward.o"(.text)
        "*/games/tla/src/debug/shop_test/drop.o"(.text)
        "*/resource_6ba_overlay.o"(.text.x02008ba0)
        "*/games/tla/src/debug/shop_test/import.o"(.text)
        "*/games/tla/src/debug/shop_test/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
