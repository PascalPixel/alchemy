/* One overlay of The Lost Age, loaded at 0x2008000: ld_script.ld includes
   this file when the build defines OVERLAY as the overlay's number, and the
   block for that number is the overlay. The build finds the overlays from the
   lines that test OVERLAY, and each overlay's objects from its statements.

   A scene's .text opens with its entry table, DIR/entry/NAME.s, and its
   import table, DIR/import/NAME.s, follows its code, where DIR is src/scene
   (or src/debug); each statement names the section it places, so the objects
   keep their order. Where its globals are written in assembly, its .bss is
   DIR/bss/NAME.s. */
#if OVERLAY == 0x649
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/title.o"(.text)
        "*/games/tla/src/scene/title.o"(.text)
        "*/games/tla/src/scene/import/title.o"(.text)
        "*/games/tla/src/scene/title.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/title.o"(.bss)
    }
}
#elif OVERLAY == 0x64a
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/clear.o"(.text)
        "*/games/tla/src/scene/clear.o"(.text)
        "*/games/tla/src/scene/import/clear.o"(.text)
        "*/games/tla/src/scene/clear.o"(.data)
        "*/games/tla/src/graphics/clear/tilemap.o"(.rodata)
        "*/games/tla/src/scene/data/clear.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/clear.o"(.bss)
    }
}
#elif OVERLAY == 0x64b
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/world_map.o"(.text)
        "*/games/tla/src/scene/world_map.o"(.text)
        "*/games/tla/src/scene/world_map/s3600.o"(.text)
        "*/games/tla/src/scene/world_map/s36a8.o"(.text)
        "*/games/tla/src/scene/world_map/s3960.o"(.text)
        "*/games/tla/src/scene/world_map/cell_index.o"(.text)
        "*/games/tla/src/scene/world_map/s39f4.o"(.text)
        "*/games/tla/src/scene/world_map/s3b38.o"(.text)
        "*/games/tla/src/scene/import/world_map.o"(.text)
        "*/games/tla/src/scene/world_map/s3b38.o"(.data)
        "*/games/tla/src/graphics/world_map/story.o"(.rodata)
        "*/games/tla/src/graphics/world_map/storyb.o"(.rodata)
        "*/games/tla/src/graphics/world_map/storyc.o"(.rodata)
        "*/games/tla/src/scene/data/world_map.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/world_map.o"(.bss)
    }
}
#elif OVERLAY == 0x64c
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/staff_credit.o"(.text)
        "*/games/tla/src/scene/staff_credit.o"(.text)
        "*/games/tla/src/scene/import/staff_credit.o"(.text)
        "*/games/tla/src/graphics/staff_credit/graphics.o"(.rodata)
        "*/games/tla/src/scene/staff_credit.o"(.data)
        "*/games/tla/src/graphics/world_map/story.o"(.rodata)
        "*/games/tla/src/graphics/world_map/storyc.o"(.rodata)
        "*/games/tla/src/scene/data/staff_credit.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/staff_credit.o"(.bss)
    }
}
#elif OVERLAY == 0x64d
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/venus_lighthouse_entrance.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance.o"(.text)
        "*/games/tla/src/scene/import/venus_lighthouse_entrance.o"(.text)
        "*/games/tla/src/graphics/field/tiles64d.o"(.rodata)
        "*/games/tla/src/scene/venus_lighthouse_entrance.o"(.data)
    }
}
#elif OVERLAY == 0x64e
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/venus_lighthouse.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse/objects.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse/s0150.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse/floor.o"(.text)
        "*/games/tla/src/scene/import/venus_lighthouse.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse/floor.o"(.data)
    }
}
#elif OVERLAY == 0x64f
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/suhalla_gate.o"(.text)
        "*/games/tla/src/scene/suhalla_gate.o"(.text)
        "*/games/tla/src/scene/import/suhalla_gate.o"(.text)
        "*/games/tla/src/scene/suhalla_gate.o"(.data)
    }
}
#elif OVERLAY == 0x650
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/idejima.o"(.text)
        "*/games/tla/src/scene/idejima.o"(.text)
        "*/games/tla/src/scene/import/idejima.o"(.text)
        "*/games/tla/src/scene/idejima.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/idejima.o"(.bss)
    }
}
#elif OVERLAY == 0x651
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/daila.o"(.text)
        "*/games/tla/src/scene/daila.o"(.text)
        "*/games/tla/src/scene/import/daila.o"(.text)
        "*/games/tla/src/scene/daila.o"(.data)
    }
}
#elif OVERLAY == 0x652
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/daila_interiors.o"(.text)
        "*/games/tla/src/scene/daila_interiors.o"(.text)
        "*/games/tla/src/scene/import/daila_interiors.o"(.text)
        "*/games/tla/src/scene/daila_interiors.o"(.data)
    }
}
#elif OVERLAY == 0x653
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/kandorean_temple.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/kandorean_temple.o"(.text)
        "*/games/tla/src/scene/kandorean_temple/kandorean_temple.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/kandorean_temple/events.o"(.text)
        "*/games/tla/src/scene/import/kandorean_temple.o"(.text)
        "*/games/tla/src/scene/kandorean_temple/events.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/kandorean_temple.o"(.bss)
    }
}
#elif OVERLAY == 0x654
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/shrine_of_the_sea_god.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/shrine_of_the_sea_god.o"(.text)
        "*/games/tla/src/scene/shrine_of_the_sea_god/shrine_of_the_sea_god.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/shrine_of_the_sea_god/s1340.o"(.text)
        "*/games/tla/src/scene/import/shrine_of_the_sea_god.o"(.text)
        "*/games/tla/src/scene/data/shrine_of_the_sea_god.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/shrine_of_the_sea_god.o"(.bss)
    }
}
#elif OVERLAY == 0x655
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/dehkan_plateau.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s0974.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s0bc0.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s0fc4.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/dehkan_plateau.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s2698.o"(.text)
        "*/games/tla/src/scene/import/dehkan_plateau.o"(.text)
        "*/games/tla/src/scene/data/dehkan_plateau.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/dehkan_plateau.o"(.bss)
    }
}
#elif OVERLAY == 0x656
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/lemurian_ship.o"(.text)
        "*/games/tla/src/scene/lemurian_ship.o"(.text)
        "*/games/tla/src/scene/import/lemurian_ship.o"(.text)
        "*/games/tla/src/scene/lemurian_ship.o"(.data)
    }
}
#elif OVERLAY == 0x657
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/lemurian_ship_deck.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/s0b84.o"(.text)
        "*/games/tla/src/scene/import/lemurian_ship_deck.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/s0b84.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/lemurian_ship_deck.o"(.bss)
    }
}
#elif OVERLAY == 0x658
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/madra.o"(.text)
        "*/games/tla/src/scene/madra.o"(.text)
        "*/games/tla/src/scene/import/madra.o"(.text)
        "*/games/tla/src/scene/madra.o"(.data)
    }
}
#elif OVERLAY == 0x659
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/madra_interiors.o"(.text)
        "*/games/tla/src/scene/madra_interiors.o"(.text)
        "*/games/tla/src/scene/import/madra_interiors.o"(.text)
        "*/games/tla/src/scene/madra_interiors.o"(.data)
    }
}
#elif OVERLAY == 0x65a
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/madra_catacombs.o"(.text)
        "*/games/tla/src/scene/madra_catacombs.o"(.text)
        "*/games/tla/src/scene/import/madra_catacombs.o"(.text)
        "*/games/tla/src/scene/madra_catacombs.o"(.data)
    }
}
#elif OVERLAY == 0x65b
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/madra_drawbridge.o"(.text)
        "*/games/tla/src/scene/madra_drawbridge.o"(.text)
        "*/games/tla/src/scene/import/madra_drawbridge.o"(.text)
        "*/games/tla/src/scene/madra_drawbridge.o"(.data)
    }
}
#elif OVERLAY == 0x65c
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/osenia_cliffs.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs.o"(.text)
        "*/games/tla/src/scene/import/osenia_cliffs.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs.o"(.data)
    }
}
#elif OVERLAY == 0x65d
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/mikasalla.o"(.text)
        "*/games/tla/src/scene/mikasalla.o"(.text)
        "*/games/tla/src/scene/import/mikasalla.o"(.text)
        "*/games/tla/src/scene/mikasalla.o"(.data)
    }
}
#elif OVERLAY == 0x65e
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/mikasalla_interiors.o"(.text)
        "*/games/tla/src/scene/mikasalla_interiors.o"(.text)
        "*/games/tla/src/scene/import/mikasalla_interiors.o"(.text)
        "*/games/tla/src/scene/mikasalla_interiors.o"(.data)
    }
}
#elif OVERLAY == 0x65f
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/garoh.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/garoh.o"(.text)
        "*/games/tla/src/scene/garoh/garoh.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/garoh/s12a8.o"(.text)
        "*/games/tla/src/scene/import/garoh.o"(.text)
        "*/games/tla/src/scene/garoh/s12a8.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/garoh.o"(.bss)
    }
}
#elif OVERLAY == 0x660
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/garoh_interiors.o"(.text)
        "*/games/tla/src/scene/garoh_interiors.o"(.text)
        "*/games/tla/src/scene/import/garoh_interiors.o"(.text)
        "*/games/tla/src/scene/garoh_interiors.o"(.data)
    }
}
#elif OVERLAY == 0x661
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/yampi_desert.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s00b8.o"(.text)
        "*/games/tla/src/scene/yampi_desert/placements.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion_priority.o"(.text)
        "*/games/tla/src/scene/yampi_desert.o"(.text)
        "*/games/tla/src/scene/import/yampi_desert.o"(.text)
        "*/games/tla/src/scene/yampi_desert.o"(.data)
        "*/games/tla/src/graphics/field/tiles64d.o"(.rodata)
        "*/games/tla/src/scene/yampi_desert/data2.o"(.data)
    }
}
#elif OVERLAY == 0x662
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/yampi_desert_cave.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave.o"(.text)
        "*/games/tla/src/scene/import/yampi_desert_cave.o"(.text)
        "*/games/tla/src/scene/data/yampi_desert_cave.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/yampi_desert_cave.o"(.bss)
    }
}
#elif OVERLAY == 0x663
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/airs_rock.o"(.text)
        "*/games/tla/src/scene/airs_rock.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/airs_rock/s015c.o"(.text)
        "*/games/tla/src/scene/airs_rock/airs_rock.o"(.text)
        "*/games/tla/src/scene/import/airs_rock.o"(.text)
        "*/games/tla/src/scene/data/airs_rock.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/airs_rock.o"(.bss)
    }
}
#elif OVERLAY == 0x664
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/airs_rock_inside.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s02f4.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/airs_rock_inside.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s2770.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s505c.o"(.text)
        "*/games/tla/src/scene/import/airs_rock_inside.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s505c.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/airs_rock_inside.o"(.bss)
    }
}
#elif OVERLAY == 0x665
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/alhafra.o"(.text)
        "*/games/tla/src/scene/alhafra.o"(.text)
        "*/games/tla/src/scene/import/alhafra.o"(.text)
        "*/games/tla/src/scene/alhafra.o"(.data)
    }
}
#elif OVERLAY == 0x666
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/alhafra_interiors.o"(.text)
        "*/games/tla/src/scene/alhafra_interiors.o"(.text)
        "*/games/tla/src/scene/import/alhafra_interiors.o"(.text)
        "*/games/tla/src/scene/alhafra_interiors.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/alhafra_interiors.o"(.bss)
    }
}
#elif OVERLAY == 0x667
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/alhafra_manor.o"(.text)
        "*/games/tla/src/scene/alhafra_manor.o"(.text)
        "*/games/tla/src/scene/import/alhafra_manor.o"(.text)
        "*/games/tla/src/scene/alhafra_manor.o"(.data)
    }
}
#elif OVERLAY == 0x668
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/eastern_alhafra.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s4004.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s44c4.o"(.text)
        "*/games/tla/src/scene/import/eastern_alhafra.o"(.text)
        "*/games/tla/src/scene/data/eastern_alhafra.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/eastern_alhafra.o"(.bss)
    }
}
#elif OVERLAY == 0x669
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/eastern_alhafra_ship.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra_ship.o"(.text)
        "*/games/tla/src/scene/import/eastern_alhafra_ship.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra_ship.o"(.data)
    }
}
#elif OVERLAY == 0x66a
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/alhafran_cave.o"(.text)
        "*/games/tla/src/scene/alhafran_cave.o"(.text)
        "*/games/tla/src/scene/import/alhafran_cave.o"(.text)
        "*/games/tla/src/scene/alhafran_cave.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/alhafran_cave.o"(.bss)
    }
}
#elif OVERLAY == 0x66b
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/gondowan_cliffs.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs/effect_spawn.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs/cliffs.o"(.text)
        "*/games/tla/src/scene/import/gondowan_cliffs.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs.o"(.data)
        "*/games/tla/src/graphics/field/tiles64d.o"(.rodata)
        "*/games/tla/src/scene/data/gondowan_cliffs.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/gondowan_cliffs.o"(.bss)
    }
}
#elif OVERLAY == 0x66c
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/gondowan_cliffs_hideout.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs_hideout.o"(.text)
        "*/games/tla/src/scene/import/gondowan_cliffs_hideout.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs_hideout.o"(.data)
    }
}
#elif OVERLAY == 0x66d
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/naribwe.o"(.text)
        "*/games/tla/src/scene/naribwe.o"(.text)
        "*/games/tla/src/scene/import/naribwe.o"(.text)
        "*/games/tla/src/scene/data/naribwe.o"(.data)
    }
}
#elif OVERLAY == 0x66e
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/naribwe_interiors.o"(.text)
        "*/games/tla/src/scene/naribwe_interiors.o"(.text)
        "*/games/tla/src/scene/import/naribwe_interiors.o"(.text)
        "*/games/tla/src/scene/naribwe_interiors.o"(.data)
    }
}
#elif OVERLAY == 0x66f
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/kibombo_mountains.o"(.text)
        "*/games/tla/src/scene/kibombo_mountains.o"(.text)
        "*/games/tla/src/scene/import/kibombo_mountains.o"(.text)
        "*/games/tla/src/scene/data/kibombo_mountains.o"(.data)
    }
}
#elif OVERLAY == 0x670
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/kibombo.o"(.text)
        "*/games/tla/src/scene/kibombo.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/kibombo/s2f34.o"(.text)
        "*/games/tla/src/scene/import/kibombo.o"(.text)
        "*/games/tla/src/scene/kibombo/s2f34.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/kibombo.o"(.bss)
    }
}
#elif OVERLAY == 0x671
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/gabomba_statue.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/gabomba_statue.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/s2394.o"(.text)
        "*/games/tla/src/scene/import/gabomba_statue.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/s2394.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/scene/data/gabomba_statue.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/gabomba_statue.o"(.bss)
    }
}
#elif OVERLAY == 0x672
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/gabomba_statue_chamber.o"(.text)
        "*/games/tla/src/scene/gabomba_statue_chamber.o"(.text)
        "*/games/tla/src/scene/import/gabomba_statue_chamber.o"(.text)
        "*/games/tla/src/scene/data/gabomba_statue_chamber.o"(.data)
    }
}
#elif OVERLAY == 0x673
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/gabomba_catacombs.o"(.text)
        "*/games/tla/src/scene/gabomba_catacombs.o"(.text)
        "*/games/tla/src/scene/import/gabomba_catacombs.o"(.text)
        "*/games/tla/src/scene/gabomba_catacombs.o"(.data)
    }
}
#elif OVERLAY == 0x674
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/islets.o"(.text)
        "*/games/tla/src/scene/islets.o"(.text)
        "*/games/tla/src/scene/islets/s3410.o"(.text)
        "*/games/tla/src/scene/islets/s3d0c.o"(.text)
        "*/games/tla/src/scene/import/islets.o"(.text)
        "*/games/tla/src/scene/data/islets.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/islets.o"(.bss)
    }
}
#elif OVERLAY == 0x675
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/islet_cave.o"(.text)
        "*/games/tla/src/scene/islet_cave.o"(.text)
        "*/games/tla/src/scene/import/islet_cave.o"(.text)
        "*/games/tla/src/scene/data/islet_cave.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/islet_cave.o"(.bss)
    }
}
#elif OVERLAY == 0x676
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/yallam.o"(.text)
        "*/games/tla/src/scene/yallam.o"(.text)
        "*/games/tla/src/scene/import/yallam.o"(.text)
        "*/games/tla/src/scene/data/yallam.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/yallam.o"(.bss)
    }
}
#elif OVERLAY == 0x677
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/yallam_interiors.o"(.text)
        "*/games/tla/src/scene/yallam_interiors.o"(.text)
        "*/games/tla/src/scene/import/yallam_interiors.o"(.text)
        "*/games/tla/src/scene/yallam_interiors.o"(.data)
    }
}
#elif OVERLAY == 0x678
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/taopo_swamp.o"(.text)
        "*/games/tla/src/scene/taopo_swamp.o"(.text)
        "*/games/tla/src/scene/import/taopo_swamp.o"(.text)
        "*/games/tla/src/scene/data/taopo_swamp.o"(.data)
    }
}
#elif OVERLAY == 0x679
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/apojii_islands.o"(.text)
        "*/games/tla/src/scene/apojii_islands.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/apojii_islands/s015c.o"(.text)
        "*/games/tla/src/scene/apojii_islands/apojii_islands.o"(.text)
        "*/games/tla/src/scene/import/apojii_islands.o"(.text)
        "*/games/tla/src/scene/apojii_islands/apojii_islands.o"(.data)
        "*/games/tla/src/graphics/field/tiles679.o"(.rodata)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/apojii_islands.o"(.bss)
    }
}
#elif OVERLAY == 0x67a
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/apojii_islands_cave.o"(.text)
        "*/games/tla/src/scene/apojii_islands_cave.o"(.text)
        "*/games/tla/src/scene/import/apojii_islands_cave.o"(.text)
        "*/games/tla/src/scene/apojii_islands_cave.o"(.data)
    }
}
#elif OVERLAY == 0x67b
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/aqua_rock.o"(.text)
        "*/games/tla/src/scene/aqua_rock.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/aqua_rock/s015c.o"(.text)
        "*/games/tla/src/scene/aqua_rock/aqua_rock.o"(.text)
        "*/games/tla/src/scene/import/aqua_rock.o"(.text)
        "*/games/tla/src/scene/aqua_rock/aqua_rock.o"(.data)
        "*/games/tla/src/graphics/aqua_rock/tiles.o"(.rodata)
        "*/games/tla/src/scene/data/aqua_rock.o"(.data)
        "*/games/tla/src/graphics/field/tiles679.o"(.rodata)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/aqua_rock.o"(.bss)
    }
}
#elif OVERLAY == 0x67c
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/aqua_rock_inside.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s015c.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/aqua_rock_inside.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s3844.o"(.text)
        "*/games/tla/src/scene/import/aqua_rock_inside.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s3844.o"(.data)
        "*/games/tla/src/graphics/aqua_rock_inside/tiles.o"(.rodata)
        "*/games/tla/src/scene/data/aqua_rock_inside.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/aqua_rock_inside.o"(.bss)
    }
}
#elif OVERLAY == 0x67d
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/izumo_interiors.o"(.text)
        "*/games/tla/src/scene/izumo_interiors.o"(.text)
        "*/games/tla/src/scene/import/izumo_interiors.o"(.text)
        "*/games/tla/src/scene/izumo_interiors.o"(.data)
    }
}
#elif OVERLAY == 0x67e
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/izumo.o"(.text)
        "*/games/tla/src/scene/izumo.o"(.text)
        "*/games/tla/src/scene/import/izumo.o"(.text)
        "*/games/tla/src/scene/izumo.o"(.data)
    }
}
#elif OVERLAY == 0x67f
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/izumo_ruins.o"(.text)
        "*/games/tla/src/scene/izumo_ruins/pillars.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion_priority.o"(.text)
        "*/games/tla/src/scene/izumo_ruins/rubble.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/izumo_ruins/effect_spawn.o"(.text)
        "*/games/tla/src/scene/izumo_ruins.o"(.text)
        "*/games/tla/src/scene/import/izumo_ruins.o"(.text)
        "*/games/tla/src/scene/izumo_ruins.o"(.data)
    }
}
#elif OVERLAY == 0x680
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/gaia_rock.o"(.text)
        "*/games/tla/src/scene/gaia_rock.o"(.text)
        "*/games/tla/src/scene/import/gaia_rock.o"(.text)
        "*/games/tla/src/scene/gaia_rock.o"(.data)
    }
}
#elif OVERLAY == 0x681
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/gaia_rock_inside.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s015c.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/gaia_rock_inside.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s07f4.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s15d0.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s1f68.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s2cb4.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s4d00.o"(.text)
        "*/games/tla/src/scene/import/gaia_rock_inside.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s4d00.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/gaia_rock_inside.o"(.bss)
    }
}
#elif OVERLAY == 0x682
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/champa.o"(.text)
        "*/games/tla/src/scene/champa.o"(.text)
        "*/games/tla/src/scene/import/champa.o"(.text)
        "*/games/tla/src/scene/champa.o"(.data)
    }
}
#elif OVERLAY == 0x683
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/champa_interiors.o"(.text)
        "*/games/tla/src/scene/champa_interiors.o"(.text)
        "*/games/tla/src/scene/import/champa_interiors.o"(.text)
        "*/games/tla/src/scene/champa_interiors.o"(.data)
    }
}
#elif OVERLAY == 0x684
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/champa_sanctum.o"(.text)
        "*/games/tla/src/scene/champa_sanctum.o"(.text)
        "*/games/tla/src/scene/import/champa_sanctum.o"(.text)
        "*/games/tla/src/scene/champa_sanctum.o"(.data)
    }
}
#elif OVERLAY == 0x685
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/champa_cliff_caves.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves.o"(.text)
        "*/games/tla/src/scene/import/champa_cliff_caves.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves.o"(.data)
        "*/games/tla/src/graphics/field/pal685.o"(.rodata)
        "*/games/tla/src/scene/data/champa_cliff_caves.o"(.data)
    }
}
#elif OVERLAY == 0x686
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/ankohl_ruins.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/object/prepare_object.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/spawn.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/s015c.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/s2438.o"(.text)
        "*/games/tla/src/scene/import/ankohl_ruins.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/s2438.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/ankohl_ruins.o"(.bss)
    }
}
#elif OVERLAY == 0x687
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/sea_of_time.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/object/prepare_object.o"(.text)
        "*/games/tla/src/scene/sea_of_time/object.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/sea_of_time/s015c.o"(.text)
        "*/games/tla/src/scene/sea_of_time.o"(.text)
        "*/games/tla/src/scene/import/sea_of_time.o"(.text)
        "*/games/tla/src/scene/sea_of_time.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/sea_of_time.o"(.bss)
    }
}
#elif OVERLAY == 0x688
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/lemuria_entrance.o"(.text)
        "*/games/tla/src/scene/lemuria_entrance.o"(.text)
        "*/games/tla/src/scene/import/lemuria_entrance.o"(.text)
        "*/games/tla/src/scene/lemuria_entrance.o"(.data)
    }
}
#elif OVERLAY == 0x689
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/lemuria_dock.o"(.text)
        "*/games/tla/src/scene/lemuria_dock.o"(.text)
        "*/games/tla/src/scene/import/lemuria_dock.o"(.text)
        "*/games/tla/src/scene/lemuria_dock.o"(.data)
    }
}
#elif OVERLAY == 0x68a
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/ancient_lemuria.o"(.text)
        "*/games/tla/src/scene/ancient_lemuria.o"(.text)
        "*/games/tla/src/scene/import/ancient_lemuria.o"(.text)
        "*/games/tla/src/scene/ancient_lemuria.o"(.data)
    }
}
#elif OVERLAY == 0x68b
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/lemuria.o"(.text)
        "*/games/tla/src/scene/lemuria.o"(.text)
        "*/games/tla/src/scene/import/lemuria.o"(.text)
        "*/games/tla/src/scene/lemuria.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/lemuria.o"(.bss)
    }
}
#elif OVERLAY == 0x68c
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/lemuria_interiors.o"(.text)
        "*/games/tla/src/scene/lemuria_interiors.o"(.text)
        "*/games/tla/src/scene/import/lemuria_interiors.o"(.text)
        "*/games/tla/src/scene/lemuria_interiors.o"(.data)
    }
}
#elif OVERLAY == 0x68d
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/lemuria_palace.o"(.text)
        "*/games/tla/src/scene/lemuria_palace.o"(.text)
        "*/games/tla/src/scene/import/lemuria_palace.o"(.text)
        "*/games/tla/src/scene/lemuria_palace.o"(.data)
        "*/games/tla/src/graphics/lemuria_palace/tiles.o"(.rodata)
        "*/games/tla/src/scene/data/lemuria_palace.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/lemuria_palace.o"(.bss)
    }
}
#elif OVERLAY == 0x68e
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/lemuria_spring.o"(.text)
        "*/games/tla/src/scene/lemuria_spring.o"(.text)
        "*/games/tla/src/scene/import/lemuria_spring.o"(.text)
        "*/games/tla/src/scene/lemuria_spring.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/lemuria_spring.o"(.bss)
    }
}
#elif OVERLAY == 0x68f
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/tundaria_tower.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s00b8.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/script0.o"(.text)
        "*/games/tla/src/field/map/cell_set.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s11e0.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s1b48.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/events_f.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s29d0.o"(.text)
        "*/games/tla/src/scene/import/tundaria_tower.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s29d0.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/tundaria_tower.o"(.bss)
    }
}
#elif OVERLAY == 0x690
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/settlements.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/settlements.o"(.text)
        "*/games/tla/src/scene/settlements/s0280.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/settlements/settlements.o"(.text)
        "*/games/tla/src/scene/settlements/s1a1c.o"(.text)
        "*/games/tla/src/scene/settlements/s1d30.o"(.text)
        "*/games/tla/src/scene/import/settlements.o"(.text)
        "*/games/tla/src/scene/settlements/s1d30.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/settlements.o"(.bss)
    }
}
#elif OVERLAY == 0x691
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/magma_rock.o"(.text)
        "*/games/tla/src/scene/magma_rock/create.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/magma_rock/s015c.o"(.text)
        "*/games/tla/src/scene/magma_rock/s03b4.o"(.text)
        "*/games/tla/src/scene/magma_rock.o"(.text)
        "*/games/tla/src/scene/magma_rock/s0ff4.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/magma_rock/s22ac.o"(.text)
        "*/games/tla/src/scene/magma_rock/s34ac.o"(.text)
        "*/games/tla/src/scene/import/magma_rock.o"(.text)
        "*/games/tla/src/scene/magma_rock/s34ac.o"(.data)
    }
}
#elif OVERLAY == 0x692
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/magma_rock_inside.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s015c.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s033c.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/magma_rock_inside.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s3678.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s46e8.o"(.text)
        "*/games/tla/src/scene/import/magma_rock_inside.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s46e8.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/magma_rock_inside.o"(.bss)
    }
}
#elif OVERLAY == 0x693
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/atteka_inlet.o"(.text)
        "*/games/tla/src/scene/atteka_inlet.o"(.text)
        "*/games/tla/src/scene/import/atteka_inlet.o"(.text)
        "*/games/tla/src/scene/atteka_inlet.o"(.data)
    }
}
#elif OVERLAY == 0x694
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/contigo.o"(.text)
        "*/games/tla/src/scene/contigo.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/contigo/contigo.o"(.text)
        "*/games/tla/src/scene/import/contigo.o"(.text)
        "*/games/tla/src/scene/contigo/contigo.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/contigo.o"(.bss)
    }
}
#elif OVERLAY == 0x695
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/shaman_village_cave.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave.o"(.text)
        "*/games/tla/src/scene/import/shaman_village_cave.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave.o"(.data)
    }
}
#elif OVERLAY == 0x696
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/shaman_village.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/shaman_village/s00b8.o"(.text)
        "*/games/tla/src/scene/shared/drift_spawn.o"(.text)
        "*/games/tla/src/scene/shaman_village/s1fec.o"(.text)
        "*/games/tla/src/scene/import/shaman_village.o"(.text)
        "*/games/tla/src/scene/shaman_village/s1fec.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/shaman_village.o"(.bss)
    }
}
#elif OVERLAY == 0x697
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/trial_road.o"(.text)
        "*/games/tla/src/scene/trial_road.o"(.text)
        "*/games/tla/src/scene/trial_road/s0218.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/trial_road/s129c.o"(.text)
        "*/games/tla/src/scene/trial_road/s15a8.o"(.text)
        "*/games/tla/src/scene/import/trial_road.o"(.text)
        "*/games/tla/src/scene/trial_road/s15a8.o"(.data)
        "*/games/tla/src/graphics/field/tiles64d.o"(.rodata)
        "*/games/tla/src/scene/data/trial_road.o"(.data)
        "*/games/tla/src/graphics/trial_road/picture.o"(.rodata)
        "*/games/tla/src/scene/data/trial_road_2.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/trial_road.o"(.bss)
    }
}
#elif OVERLAY == 0x698
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/jupiter_lighthouse_entrance.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/cracked_tiles.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/cracked_tile_steps.o"(.text)
        "*/games/tla/src/scene/import/jupiter_lighthouse_entrance.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/jupiter_lighthouse_entrance.o"(.bss)
    }
}
#elif OVERLAY == 0x699
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/jupiter_lighthouse_basement.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement/s098c.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement/s1094.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement/s16c4.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement/s185c.o"(.text)
        "*/games/tla/src/scene/import/jupiter_lighthouse_basement.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_basement/s185c.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/jupiter_lighthouse_basement.o"(.bss)
    }
}
#elif OVERLAY == 0x69a
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/jupiter_lighthouse_room_1.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_1.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_1/cracked_tiles.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_1/cracked_tile_steps.o"(.text)
        "*/games/tla/src/scene/import/jupiter_lighthouse_room_1.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_1.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/jupiter_lighthouse_room_1.o"(.bss)
    }
}
#elif OVERLAY == 0x69b
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/jupiter_lighthouse_room_2.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/s0d94.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/s165c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/s1c8c.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/s1e24.o"(.text)
        "*/games/tla/src/scene/import/jupiter_lighthouse_room_2.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/s1e24.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/jupiter_lighthouse_room_2.o"(.bss)
    }
}
#elif OVERLAY == 0x69c
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/jupiter_lighthouse_room_3.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/cracked_tiles.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/cracked_tile_steps.o"(.text)
        "*/games/tla/src/scene/import/jupiter_lighthouse_room_3.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/data/jupiter_lighthouse_room_3.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/jupiter_lighthouse_room_3.o"(.bss)
    }
}
#elif OVERLAY == 0x69d
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/jupiter_lighthouse_room_4.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_4.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_4/cracked_tiles.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_4/cracked_tile_steps.o"(.text)
        "*/games/tla/src/scene/import/jupiter_lighthouse_room_4.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_4.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/jupiter_lighthouse_room_4.o"(.bss)
    }
}
#elif OVERLAY == 0x69e
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/jupiter_lighthouse_room_5.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/s1650.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/s1c88.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/s22b8.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/s2450.o"(.text)
        "*/games/tla/src/scene/import/jupiter_lighthouse_room_5.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/s2450.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/jupiter_lighthouse_room_5.o"(.bss)
    }
}
#elif OVERLAY == 0x69f
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/jupiter_lighthouse_room_6.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6/cracked_tiles.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6/cracked_tile_steps.o"(.text)
        "*/games/tla/src/scene/import/jupiter_lighthouse_room_6.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/jupiter_lighthouse_room_6.o"(.bss)
    }
}
#elif OVERLAY == 0x6a0
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/jupiter_lighthouse_room_7.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/s16d4.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/s1f74.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/s25a4.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/s273c.o"(.text)
        "*/games/tla/src/scene/import/jupiter_lighthouse_room_7.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/s273c.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/data/jupiter_lighthouse_room_7.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/jupiter_lighthouse_room_7.o"(.bss)
    }
}
#elif OVERLAY == 0x6a1
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/jupiter_lighthouse_room_8.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/cracked_tiles.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/cracked_tile_steps.o"(.text)
        "*/games/tla/src/scene/import/jupiter_lighthouse_room_8.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/data/jupiter_lighthouse_room_8.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/jupiter_lighthouse_room_8.o"(.bss)
    }
}
#elif OVERLAY == 0x6a2
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/jupiter_lighthouse_room_9.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_9.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_9/cracked_tiles.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_9/cracked_tile_steps.o"(.text)
        "*/games/tla/src/scene/import/jupiter_lighthouse_room_9.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_9.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/jupiter_lighthouse_room_9.o"(.bss)
    }
}
#elif OVERLAY == 0x6a3
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/jupiter_lighthouse_room_m.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/s3474.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/s3b7c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/s41ac.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/s4344.o"(.text)
        "*/games/tla/src/scene/import/jupiter_lighthouse_room_m.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/s4344.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/data/jupiter_lighthouse_room_m.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/jupiter_lighthouse_room_m.o"(.bss)
    }
}
#elif OVERLAY == 0x6a4
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/jupiter_lighthouse_aerie.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/s17a8.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/s1e78.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/s24a8.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/s2640.o"(.text)
        "*/games/tla/src/scene/import/jupiter_lighthouse_aerie.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/s2640.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/jupiter_lighthouse_aerie.o"(.bss)
    }
}
#elif OVERLAY == 0x6a5
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/jupiter_lighthouse_aerie_beacon.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon.o"(.text)
        "*/games/tla/src/field/object/step_angle.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s0198.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s44cc.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s4bd4.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s4ec8.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s539c.o"(.text)
        "*/games/tla/src/scene/import/jupiter_lighthouse_aerie_beacon.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s539c.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/data/jupiter_lighthouse_aerie_beacon.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/jupiter_lighthouse_aerie_beacon.o"(.bss)
    }
}
#elif OVERLAY == 0x6a6
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/loho.o"(.text)
        "*/games/tla/src/scene/loho.o"(.text)
        "*/games/tla/src/scene/import/loho.o"(.text)
        "*/games/tla/src/graphics/field/pal685.o"(.rodata)
        "*/games/tla/src/scene/loho.o"(.data)
    }
}
#elif OVERLAY == 0x6a7
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/northern_reaches.o"(.text)
        "*/games/tla/src/scene/northern_reaches.o"(.text)
        "*/games/tla/src/scene/import/northern_reaches.o"(.text)
        "*/games/tla/src/graphics/field/pal685.o"(.rodata)
        "*/games/tla/src/scene/northern_reaches.o"(.data)
    }
}
#elif OVERLAY == 0x6a8
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/prox.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/prox.o"(.text)
        "*/games/tla/src/scene/prox/prox.o"(.text)
        "*/games/tla/src/scene/import/prox.o"(.text)
        "*/games/tla/src/scene/prox/prox.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/prox.o"(.bss)
    }
}
#elif OVERLAY == 0x6a9
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/caverns.o"(.text)
        "*/games/tla/src/scene/caverns.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/caverns/s015c.o"(.text)
        "*/games/tla/src/scene/caverns/caverns.o"(.text)
        "*/games/tla/src/scene/import/caverns.o"(.text)
        "*/games/tla/src/scene/caverns/caverns.o"(.data)
    }
}
#elif OVERLAY == 0x6aa
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/mars_lighthouse_entrance.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/mars_lighthouse_entrance.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s2270.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s263c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s3884.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s3b30.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s3cf8.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s42f8.o"(.text)
        "*/games/tla/src/scene/import/mars_lighthouse_entrance.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s42f8.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/mars_lighthouse_entrance.o"(.bss)
    }
}
#elif OVERLAY == 0x6ab
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/mars_lighthouse_star_room.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/ride_handle.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s2148.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/set_data.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s363c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s3804.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s3e04.o"(.text)
        "*/games/tla/src/scene/import/mars_lighthouse_star_room.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s3e04.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/mars_lighthouse_star_room.o"(.bss)
    }
}
#elif OVERLAY == 0x6ac
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/mars_lighthouse_room_m.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s2118.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s2708.o"(.text)
        "*/games/tla/src/scene/import/mars_lighthouse_room_m.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s2708.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/mars_lighthouse_room_m.o"(.bss)
    }
}
#elif OVERLAY == 0x6ad
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/mars_lighthouse_room_f.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/ride.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/flash.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s328c.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s35b8.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s3d80.o"(.text)
        "*/games/tla/src/scene/import/mars_lighthouse_room_f.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s3d80.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/data/mars_lighthouse_room_f.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/mars_lighthouse_room_f.o"(.bss)
    }
}
#elif OVERLAY == 0x6ae
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/mars_lighthouse_aerie.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/s3370.o"(.text)
        "*/games/tla/src/scene/import/mars_lighthouse_aerie.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/s3370.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/mars_lighthouse_aerie.o"(.bss)
    }
}
#elif OVERLAY == 0x6af
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/treasure_isle.o"(.text)
        "*/games/tla/src/scene/treasure_isle.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s015c.o"(.text)
        "*/games/tla/src/scene/treasure_isle/treasure_isle.o"(.text)
        "*/games/tla/src/scene/import/treasure_isle.o"(.text)
        "*/games/tla/src/scene/treasure_isle/treasure_isle.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/treasure_isle.o"(.bss)
    }
}
#elif OVERLAY == 0x6b0
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/anemos_inner_sanctum.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s0480.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/scene_tables.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s0ac8.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s1938.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/move.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s2db0.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s2f48.o"(.text)
        "*/games/tla/src/scene/import/anemos_inner_sanctum.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s2f48.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/anemos_inner_sanctum.o"(.bss)
    }
}
#elif OVERLAY == 0x6b1
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/anemos_inner_sanctum_entrance.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/object/prepare_object.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/spawn.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/s015c.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance.o"(.text)
        "*/games/tla/src/scene/import/anemos_inner_sanctum_entrance.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_d.o"(.rodata)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/data2.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/anemos_inner_sanctum_entrance.o"(.bss)
    }
}
#elif OVERLAY == 0x6b2
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/mercury_lighthouse.o"(.text)
        "*/games/tla/src/scene/mercury_lighthouse.o"(.text)
        "*/games/tla/src/scene/import/mercury_lighthouse.o"(.text)
        "*/games/tla/src/scene/mercury_lighthouse.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/mercury_lighthouse.o"(.bss)
    }
}
#elif OVERLAY == 0x6b3
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/epilogue.o"(.text)
        "*/games/tla/src/scene/epilogue.o"(.text)
        "*/games/tla/src/scene/import/epilogue.o"(.text)
        "*/games/tla/src/graphics/epilogue/pal.o"(.rodata)
        "*/games/tla/src/scene/epilogue.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/epilogue.o"(.bss)
    }
}
#elif OVERLAY == 0x6b4
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/mt_aleph_summit.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/mt_aleph_summit.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/mt_aleph_summit/s0478.o"(.text)
        "*/games/tla/src/scene/import/mt_aleph_summit.o"(.text)
        "*/games/tla/src/scene/mt_aleph_summit.o"(.data)
    }
}
#elif OVERLAY == 0x6b5
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/lobby.o"(.text)
        "*/games/tla/src/scene/lobby.o"(.text)
        "*/games/tla/src/scene/import/lobby.o"(.text)
        "*/games/tla/src/scene/lobby.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/lobby.o"(.bss)
    }
}
#elif OVERLAY == 0x6b6
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/debug/entry/rooms.o"(.text)
        "*/games/tla/src/debug/test_scenes.o"(.text)
        "*/games/tla/src/debug/import/rooms.o"(.text)
        "*/games/tla/src/debug/test_scenes.o"(.data)
    }
}
#elif OVERLAY == 0x6b7
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/debug/entry/level.o"(.text)
        "*/games/tla/src/debug/level.o"(.text)
        "*/games/tla/src/debug/import/level.o"(.text)
        "*/games/tla/src/debug/level.o"(.data)
    }
}
#elif OVERLAY == 0x6b8
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/debug/entry/psynergy_test.o"(.text)
        "*/games/tla/src/debug/psynergy_test.o"(.text)
        "*/games/tla/src/debug/import/psynergy_test.o"(.text)
        "*/games/tla/src/debug/psynergy_test.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/debug/bss/psynergy_test.o"(.bss)
    }
}
#elif OVERLAY == 0x6b9
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/debug/entry/psynergy_test_2.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2.o"(.text)
        "*/games/tla/src/debug/import/psynergy_test_2.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2.o"(.data)
    }
}
#elif OVERLAY == 0x6ba
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/debug/entry/shop_test.o"(.text)
        "*/games/tla/src/debug/shop_test.o"(.text)
        "*/games/tla/src/debug/import/shop_test.o"(.text)
        "*/games/tla/src/debug/shop_test.o"(.data)
    }
}
#endif

SECTIONS
{
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
