SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/lemurian_ship_deck/entry.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/scene_tables.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/s004c.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/s0078.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/s013c.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/s0168.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/s0188.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/s01cc.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/s0210.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/s0258.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/s0b84.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/s0be0.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/s0c80.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/s1064.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/s1074.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/s1178.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/s1188.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/s1210.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/s1d60.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/s1da0.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/import.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/data.o"(.data)
    }
    .bss : { "*/resource_657_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
