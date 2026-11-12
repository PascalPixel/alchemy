/* One overlay of The Lost Age, loaded at 0x2008000: ld_script.ld includes
   this file when the build defines OVERLAY as the overlay's number, and the
   block for that number is the overlay. The build finds the overlays from the
   lines that test OVERLAY, and each overlay's objects from its statements.

   A scene's objects sit in its folder DIR. Its .text opens with its entry
   table DIR/entry.s, and each statement names the section it places, so
   the objects keep their order. Where its globals are written in assembly,
   its .bss is DIR/bss.s. */
#if OVERLAY == 0x649
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/title/entry.o"(.text)
        "*/games/tla/src/scene/title.o"(.text)
        "*/games/tla/src/scene/title/import.o"(.text)
        "*/games/tla/src/scene/title.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/title/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x64a
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/clear/entry.o"(.text)
        "*/games/tla/src/scene/clear.o"(.text)
        "*/games/tla/src/scene/clear/import.o"(.text)
        "*/games/tla/src/scene/clear/data.o"(.data)
        "*/games/tla/src/scene/clear/tilemap.o"(.rodata)
        "*/games/tla/src/scene/clear/data2.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/clear/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x64b
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/world_map/entry.o"(.text)
        "*/games/tla/src/scene/world_map.o"(.text)
        "*/games/tla/src/scene/world_map/s3600.o"(.text)
        "*/games/tla/src/scene/world_map/s36a8.o"(.text)
        "*/games/tla/src/scene/world_map/s3960.o"(.text)
        "*/games/tla/src/scene/world_map/cell_index.o"(.text)
        "*/games/tla/src/scene/world_map/s39f4.o"(.text)
        "*/games/tla/src/scene/world_map/s3b38.o"(.text)
        "*/games/tla/src/scene/world_map/import.o"(.text)
        "*/games/tla/src/scene/world_map/s3b38.o"(.data)
        "*/games/tla/src/graphics/world_map/story.o"(.rodata)
        "*/games/tla/src/graphics/world_map/storyb.o"(.rodata)
        "*/games/tla/src/graphics/world_map/storyc.o"(.rodata)
        "*/games/tla/src/scene/world_map/data2.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/world_map/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x64c
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/staff_credit/entry.o"(.text)
        "*/games/tla/src/scene/staff_credit.o"(.text)
        "*/games/tla/src/scene/staff_credit/s1ad0.o"(.text)
        "*/games/tla/src/scene/staff_credit/import.o"(.text)
        "*/games/tla/src/scene/staff_credit/graphics.o"(.rodata)
        "*/games/tla/src/scene/staff_credit/s1ad0.o"(.data)
        "*/games/tla/src/graphics/world_map/story.o"(.rodata)
        "*/games/tla/src/graphics/world_map/storyc.o"(.rodata)
        "*/games/tla/src/scene/staff_credit/data2.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/staff_credit/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x64d
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/venus_lighthouse_entrance/entry.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/s23a4.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/s26f4.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/sparks.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/import.o"(.text)
        "*/games/tla/src/graphics/field/tiles64d.o"(.rodata)
        "*/games/tla/src/scene/venus_lighthouse_entrance/sparks.o"(.data)
    }
}
#elif OVERLAY == 0x64e
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/venus_lighthouse/entry.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse/objects.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse/s0150.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse/floor.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse/push.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse/import.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse/push.o"(.data)
    }
}
#elif OVERLAY == 0x64f
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/suhalla_gate/entry.o"(.text)
        "*/games/tla/src/scene/suhalla_gate.o"(.text)
        "*/games/tla/src/scene/suhalla_gate/import.o"(.text)
        "*/games/tla/src/scene/suhalla_gate.o"(.data)
    }
}
#elif OVERLAY == 0x650
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/idejima/entry.o"(.text)
        "*/games/tla/src/scene/idejima.o"(.text)
        "*/games/tla/src/scene/idejima/s16d8.o"(.text)
        "*/games/tla/src/scene/idejima/import.o"(.text)
        "*/games/tla/src/scene/idejima/data.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/idejima/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x651
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/daila/entry.o"(.text)
        "*/games/tla/src/scene/daila.o"(.text)
        "*/games/tla/src/scene/daila/import.o"(.text)
        "*/games/tla/src/scene/daila/data.o"(.data)
    }
}
#elif OVERLAY == 0x652
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/daila_interiors/entry.o"(.text)
        "*/games/tla/src/scene/daila_interiors.o"(.text)
        "*/games/tla/src/scene/daila_interiors/item.o"(.text)
        "*/games/tla/src/scene/daila_interiors/arrival.o"(.text)
        "*/games/tla/src/scene/daila_interiors/import.o"(.text)
        "*/games/tla/src/scene/daila_interiors/arrival.o"(.data)
    }
}
#elif OVERLAY == 0x653
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/kandorean_temple/entry.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/kandorean_temple/s00b8.o"(.text)
        "*/games/tla/src/scene/kandorean_temple.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/kandorean_temple/s2ec4.o"(.text)
        "*/games/tla/src/scene/kandorean_temple/import.o"(.text)
        "*/games/tla/src/scene/kandorean_temple/s2ec4.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/kandorean_temple/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x654
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/shrine_of_the_sea_god/entry.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/shrine_of_the_sea_god/s00b8.o"(.text)
        "*/games/tla/src/scene/shrine_of_the_sea_god/event0.o"(.text)
        "*/games/tla/src/scene/shrine_of_the_sea_god.o"(.text)
        "*/games/tla/src/scene/shrine_of_the_sea_god/event7.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/shrine_of_the_sea_god/s1340.o"(.text)
        "*/games/tla/src/scene/shrine_of_the_sea_god/import.o"(.text)
        "*/games/tla/src/scene/shrine_of_the_sea_god/data.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/shrine_of_the_sea_god/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x655
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/dehkan_plateau/entry.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s0038.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s0974.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s0bc0.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s0fc4.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/events_g.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s2698.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/events_i.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s2c14.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s3164.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s3284.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s390c.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s3a30.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/import.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/data.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/dehkan_plateau/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x656
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/lemurian_ship/entry.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/scene_tables.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s00b8.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/import.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/data.o"(.data)
    }
}
#elif OVERLAY == 0x657
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/lemurian_ship_deck/entry.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/s0b84.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/import.o"(.text)
        "*/games/tla/src/scene/lemurian_ship_deck/s0b84.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/lemurian_ship_deck/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x658
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/madra/entry.o"(.text)
        "*/games/tla/src/scene/madra.o"(.text)
        "*/games/tla/src/scene/madra/events_b.o"(.text)
        "*/games/tla/src/scene/madra/s051c.o"(.text)
        "*/games/tla/src/scene/madra/s0ac4.o"(.text)
        "*/games/tla/src/scene/madra/events_d.o"(.text)
        "*/games/tla/src/scene/madra/import.o"(.text)
        "*/games/tla/src/scene/madra/events_d.o"(.data)
    }
}
#elif OVERLAY == 0x659
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/madra_interiors/entry.o"(.text)
        "*/games/tla/src/scene/madra_interiors.o"(.text)
        "*/games/tla/src/scene/madra_interiors/import.o"(.text)
        "*/games/tla/src/scene/madra_interiors/data.o"(.data)
    }
}
#elif OVERLAY == 0x65a
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/madra_catacombs/entry.o"(.text)
        "*/games/tla/src/scene/madra_catacombs.o"(.text)
        "*/games/tla/src/scene/madra_catacombs/import.o"(.text)
        "*/games/tla/src/scene/madra_catacombs/data.o"(.data)
    }
}
#elif OVERLAY == 0x65b
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/madra_drawbridge/entry.o"(.text)
        "*/games/tla/src/scene/madra_drawbridge.o"(.text)
        "*/games/tla/src/scene/madra_drawbridge/import.o"(.text)
        "*/games/tla/src/scene/madra_drawbridge.o"(.data)
    }
}
#elif OVERLAY == 0x65c
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/osenia_cliffs/entry.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs/import.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs.o"(.data)
    }
}
#elif OVERLAY == 0x65d
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/mikasalla/entry.o"(.text)
        "*/games/tla/src/scene/mikasalla.o"(.text)
        "*/games/tla/src/scene/mikasalla/import.o"(.text)
        "*/games/tla/src/scene/mikasalla/data.o"(.data)
    }
}
#elif OVERLAY == 0x65e
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/mikasalla_interiors/entry.o"(.text)
        "*/games/tla/src/scene/mikasalla_interiors.o"(.text)
        "*/games/tla/src/scene/mikasalla_interiors/import.o"(.text)
        "*/games/tla/src/scene/mikasalla_interiors.o"(.data)
    }
}
#elif OVERLAY == 0x65f
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/garoh/entry.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/garoh/s00b8.o"(.text)
        "*/games/tla/src/scene/garoh.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/garoh/s12a8.o"(.text)
        "*/games/tla/src/scene/garoh/push.o"(.text)
        "*/games/tla/src/scene/garoh/import.o"(.text)
        "*/games/tla/src/scene/garoh/push.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/garoh/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x660
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/garoh_interiors/entry.o"(.text)
        "*/games/tla/src/scene/garoh_interiors.o"(.text)
        "*/games/tla/src/scene/garoh_interiors/import.o"(.text)
        "*/games/tla/src/scene/garoh_interiors.o"(.data)
    }
}
#elif OVERLAY == 0x661
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/yampi_desert/entry.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s00b8.o"(.text)
        "*/games/tla/src/scene/yampi_desert/placements.o"(.text)
        "*/games/tla/src/scene/yampi_desert/scene_tables.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s02f8.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion_priority.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s0b84.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s1b70.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s1bec.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s1cc4.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s1e58.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s388c.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s3910.o"(.text)
        "*/games/tla/src/scene/yampi_desert/import.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s3910.o"(.data)
        "*/games/tla/src/graphics/field/tiles64d.o"(.rodata)
        "*/games/tla/src/scene/yampi_desert/data2.o"(.data)
    }
}
#elif OVERLAY == 0x662
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/yampi_desert_cave/entry.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/s0038.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/s01d4.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/import.o"(.text)
        "*/games/tla/src/scene/yampi_desert_cave/data.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/yampi_desert_cave/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x663
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/airs_rock/entry.o"(.text)
        "*/games/tla/src/scene/airs_rock/s00d0.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/airs_rock/s015c.o"(.text)
        "*/games/tla/src/scene/airs_rock.o"(.text)
        "*/games/tla/src/scene/airs_rock/import.o"(.text)
        "*/games/tla/src/scene/airs_rock/data.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/airs_rock/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x664
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/airs_rock_inside/entry.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s0038.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/object.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s02f4.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s0550.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/cell_get.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/scene_tables.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s2604.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s26c0.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s2770.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s2cf0.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s3170.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s3654.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s3ac4.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s4ab8.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s4bb4.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s505c.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s5cb8.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/import.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/data.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/airs_rock_inside/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x665
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/alhafra/entry.o"(.text)
        "*/games/tla/src/scene/alhafra.o"(.text)
        "*/games/tla/src/scene/alhafra/import.o"(.text)
        "*/games/tla/src/scene/alhafra/data.o"(.data)
    }
}
#elif OVERLAY == 0x666
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/alhafra_interiors/entry.o"(.text)
        "*/games/tla/src/scene/alhafra_interiors.o"(.text)
        "*/games/tla/src/scene/alhafra_interiors/s01ac.o"(.text)
        "*/games/tla/src/scene/alhafra_interiors/s0298.o"(.text)
        "*/games/tla/src/scene/alhafra_interiors/import.o"(.text)
        "*/games/tla/src/scene/alhafra_interiors/data.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/alhafra_interiors/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x667
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/alhafra_manor/entry.o"(.text)
        "*/games/tla/src/scene/alhafra_manor.o"(.text)
        "*/games/tla/src/scene/alhafra_manor/import.o"(.text)
        "*/games/tla/src/scene/alhafra_manor/data.o"(.data)
    }
}
#elif OVERLAY == 0x668
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/eastern_alhafra/entry.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s0514.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s1bf4.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s4004.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s44c4.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/import.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/data.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/eastern_alhafra/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x669
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/eastern_alhafra_ship/entry.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra_ship.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra_ship/s0438.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra_ship/import.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra_ship/s0438.o"(.data)
    }
}
#elif OVERLAY == 0x66a
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/alhafran_cave/entry.o"(.text)
        "*/games/tla/src/scene/alhafran_cave.o"(.text)
        "*/games/tla/src/scene/alhafran_cave/import.o"(.text)
        "*/games/tla/src/scene/alhafran_cave/data.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/alhafran_cave/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x66b
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/gondowan_cliffs/entry.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs/s0160.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs/s01ec.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs/s03cc.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs/import.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs.o"(.data)
        "*/games/tla/src/graphics/field/tiles64d.o"(.rodata)
        "*/games/tla/src/scene/gondowan_cliffs/data2.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/gondowan_cliffs/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x66c
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/gondowan_cliffs_hideout/entry.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs_hideout.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs_hideout/import.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs_hideout.o"(.data)
    }
}
#elif OVERLAY == 0x66d
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/naribwe/entry.o"(.text)
        "*/games/tla/src/scene/naribwe.o"(.text)
        "*/games/tla/src/scene/naribwe/import.o"(.text)
        "*/games/tla/src/scene/naribwe/data.o"(.data)
    }
}
#elif OVERLAY == 0x66e
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/naribwe_interiors/entry.o"(.text)
        "*/games/tla/src/scene/naribwe_interiors.o"(.text)
        "*/games/tla/src/scene/naribwe_interiors/import.o"(.text)
        "*/games/tla/src/scene/naribwe_interiors.o"(.data)
    }
}
#elif OVERLAY == 0x66f
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/kibombo_mountains/entry.o"(.text)
        "*/games/tla/src/scene/kibombo_mountains.o"(.text)
        "*/games/tla/src/scene/kibombo_mountains/import.o"(.text)
        "*/games/tla/src/scene/kibombo_mountains/data.o"(.data)
    }
}
#elif OVERLAY == 0x670
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/kibombo/entry.o"(.text)
        "*/games/tla/src/scene/kibombo/s0038.o"(.text)
        "*/games/tla/src/scene/kibombo.o"(.text)
        "*/games/tla/src/scene/kibombo/events_a.o"(.text)
        "*/games/tla/src/scene/kibombo/s1318.o"(.text)
        "*/games/tla/src/scene/kibombo/s2ad8.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/kibombo/s2f34.o"(.text)
        "*/games/tla/src/scene/kibombo/s3308.o"(.text)
        "*/games/tla/src/scene/kibombo/s3504.o"(.text)
        "*/games/tla/src/scene/kibombo/s516c.o"(.text)
        "*/games/tla/src/scene/kibombo/s538c.o"(.text)
        "*/games/tla/src/scene/kibombo/import.o"(.text)
        "*/games/tla/src/scene/kibombo/s538c.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/kibombo/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x671
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/gabomba_statue/entry.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/gabomba_statue.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/find_near.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/setup.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/s1630.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/s2394.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/s2a64.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/s3974.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/s3a64.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/puzzle_cells.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/s4288.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/puzzle_turn.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/s43ec.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/puzzle_mode.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/s44d0.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/s45d8.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/s46c8.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/s4710.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/s48a4.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/s4938.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/s4d48.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/s5090.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/clock_count_down.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/clock.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/import.o"(.text)
        "*/games/tla/src/scene/gabomba_statue/clock.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/scene/gabomba_statue/data2.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/gabomba_statue/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x672
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/gabomba_statue_chamber/entry.o"(.text)
        "*/games/tla/src/scene/gabomba_statue_chamber.o"(.text)
        "*/games/tla/src/scene/gabomba_statue_chamber/s0f00.o"(.text)
        "*/games/tla/src/scene/gabomba_statue_chamber/s1710.o"(.text)
        "*/games/tla/src/scene/gabomba_statue_chamber/prepare.o"(.text)
        "*/games/tla/src/scene/gabomba_statue_chamber/import.o"(.text)
        "*/games/tla/src/scene/gabomba_statue_chamber/data.o"(.data)
    }
}
#elif OVERLAY == 0x673
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/gabomba_catacombs/entry.o"(.text)
        "*/games/tla/src/scene/gabomba_catacombs.o"(.text)
        "*/games/tla/src/scene/gabomba_catacombs/import.o"(.text)
        "*/games/tla/src/scene/gabomba_catacombs.o"(.data)
    }
}
#elif OVERLAY == 0x674
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/islets/entry.o"(.text)
        "*/games/tla/src/scene/islets/scene_tables.o"(.text)
        "*/games/tla/src/scene/islets/s004c.o"(.text)
        "*/games/tla/src/scene/islets/s14e0.o"(.text)
        "*/games/tla/src/scene/islets/s3410.o"(.text)
        "*/games/tla/src/scene/islets/s37d8.o"(.text)
        "*/games/tla/src/scene/islets/s3a5c.o"(.text)
        "*/games/tla/src/scene/islets/s3d0c.o"(.text)
        "*/games/tla/src/scene/islets/import.o"(.text)
        "*/games/tla/src/scene/islets/data.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/islets/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x675
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/islet_cave/entry.o"(.text)
        "*/games/tla/src/scene/islet_cave.o"(.text)
        "*/games/tla/src/scene/islet_cave/import.o"(.text)
        "*/games/tla/src/scene/islet_cave/data.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/islet_cave/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x676
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/yallam/entry.o"(.text)
        "*/games/tla/src/scene/yallam.o"(.text)
        "*/games/tla/src/scene/yallam/import.o"(.text)
        "*/games/tla/src/scene/yallam/data.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/yallam/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x677
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/yallam_interiors/entry.o"(.text)
        "*/games/tla/src/scene/yallam_interiors.o"(.text)
        "*/games/tla/src/scene/yallam_interiors/import.o"(.text)
        "*/games/tla/src/scene/yallam_interiors.o"(.data)
    }
}
#elif OVERLAY == 0x678
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/taopo_swamp/entry.o"(.text)
        "*/games/tla/src/scene/taopo_swamp.o"(.text)
        "*/games/tla/src/scene/taopo_swamp/import.o"(.text)
        "*/games/tla/src/scene/taopo_swamp/data.o"(.data)
    }
}
#elif OVERLAY == 0x679
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/apojii_islands/entry.o"(.text)
        "*/games/tla/src/scene/apojii_islands/object.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/apojii_islands/s015c.o"(.text)
        "*/games/tla/src/scene/apojii_islands.o"(.text)
        "*/games/tla/src/scene/apojii_islands/s07a4.o"(.text)
        "*/games/tla/src/scene/apojii_islands/events_d.o"(.text)
        "*/games/tla/src/scene/apojii_islands/s10d8.o"(.text)
        "*/games/tla/src/scene/apojii_islands/events_h.o"(.text)
        "*/games/tla/src/scene/apojii_islands/s15f8.o"(.text)
        "*/games/tla/src/scene/apojii_islands/s17bc.o"(.text)
        "*/games/tla/src/scene/apojii_islands/s17f8.o"(.text)
        "*/games/tla/src/scene/apojii_islands/events_j.o"(.text)
        "*/games/tla/src/scene/apojii_islands/import.o"(.text)
        "*/games/tla/src/scene/apojii_islands/events_j.o"(.data)
        "*/games/tla/src/graphics/field/tiles679.o"(.rodata)
    }
    .bss :
    {
        "*/games/tla/src/scene/apojii_islands/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x67a
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/apojii_islands_cave/entry.o"(.text)
        "*/games/tla/src/scene/apojii_islands_cave.o"(.text)
        "*/games/tla/src/scene/apojii_islands_cave/import.o"(.text)
        "*/games/tla/src/scene/apojii_islands_cave.o"(.data)
    }
}
#elif OVERLAY == 0x67b
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/aqua_rock/entry.o"(.text)
        "*/games/tla/src/scene/aqua_rock/s00d0.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/aqua_rock/s015c.o"(.text)
        "*/games/tla/src/scene/aqua_rock/scene_tables.o"(.text)
        "*/games/tla/src/scene/aqua_rock/s0344.o"(.text)
        "*/games/tla/src/scene/aqua_rock/exits.o"(.text)
        "*/games/tla/src/scene/aqua_rock/s0468.o"(.text)
        "*/games/tla/src/scene/aqua_rock/s0580.o"(.text)
        "*/games/tla/src/scene/aqua_rock/s0844.o"(.text)
        "*/games/tla/src/scene/aqua_rock/events_b.o"(.text)
        "*/games/tla/src/scene/aqua_rock/s0d80.o"(.text)
        "*/games/tla/src/scene/aqua_rock/s0f8c.o"(.text)
        "*/games/tla/src/scene/aqua_rock/s17f4.o"(.text)
        "*/games/tla/src/scene/aqua_rock/events_g.o"(.text)
        "*/games/tla/src/scene/aqua_rock/s1e00.o"(.text)
        "*/games/tla/src/scene/aqua_rock/events_h.o"(.text)
        "*/games/tla/src/scene/aqua_rock/s2250.o"(.text)
        "*/games/tla/src/scene/aqua_rock/s23c0.o"(.text)
        "*/games/tla/src/scene/aqua_rock/events_i.o"(.text)
        "*/games/tla/src/scene/aqua_rock/s25d4.o"(.text)
        "*/games/tla/src/scene/aqua_rock/events_j.o"(.text)
        "*/games/tla/src/scene/aqua_rock/import.o"(.text)
        "*/games/tla/src/scene/aqua_rock/events_j.o"(.data)
        "*/games/tla/src/scene/aqua_rock/tiles.o"(.rodata)
        "*/games/tla/src/scene/aqua_rock/data2.o"(.data)
        "*/games/tla/src/graphics/field/tiles679.o"(.rodata)
    }
    .bss :
    {
        "*/games/tla/src/scene/aqua_rock/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x67c
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/aqua_rock_inside/entry.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s00d0.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s015c.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s033c.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s042c.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s0cc8.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s13bc.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s1620.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s1c68.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s2340.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s2bd4.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s30f4.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s3278.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s32d4.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s3420.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s34fc.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s3628.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s36b8.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s3844.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/import.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s3844.o"(.data)
        "*/games/tla/src/scene/aqua_rock_inside/tiles.o"(.rodata)
        "*/games/tla/src/scene/aqua_rock_inside/data2.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/aqua_rock_inside/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x67d
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/izumo_interiors/entry.o"(.text)
        "*/games/tla/src/scene/izumo_interiors.o"(.text)
        "*/games/tla/src/scene/izumo_interiors/import.o"(.text)
        "*/games/tla/src/scene/izumo_interiors.o"(.data)
    }
}
#elif OVERLAY == 0x67e
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/izumo/entry.o"(.text)
        "*/games/tla/src/scene/izumo.o"(.text)
        "*/games/tla/src/scene/izumo/import.o"(.text)
        "*/games/tla/src/scene/izumo/data.o"(.data)
    }
}
#elif OVERLAY == 0x67f
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/izumo_ruins/entry.o"(.text)
        "*/games/tla/src/scene/izumo_ruins/s0038.o"(.text)
        "*/games/tla/src/scene/izumo_ruins/s00a4.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion_priority.o"(.text)
        "*/games/tla/src/scene/izumo_ruins/s0974.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/izumo_ruins/s0ce4.o"(.text)
        "*/games/tla/src/scene/izumo_ruins.o"(.text)
        "*/games/tla/src/scene/izumo_ruins/import.o"(.text)
        "*/games/tla/src/scene/izumo_ruins/data.o"(.data)
    }
}
#elif OVERLAY == 0x680
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/gaia_rock/entry.o"(.text)
        "*/games/tla/src/scene/gaia_rock.o"(.text)
        "*/games/tla/src/scene/gaia_rock/import.o"(.text)
        "*/games/tla/src/scene/gaia_rock/data.o"(.data)
    }
}
#elif OVERLAY == 0x681
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/gaia_rock_inside/entry.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s00d0.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s015c.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s07f4.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s0900.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s0cd8.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s0cf8.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s1048.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s12ac.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s136c.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s15d0.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s1f68.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s2020.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s2ac8.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s2b04.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s2b58.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s2b8c.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s2cb4.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s4d00.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/import.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s4d00.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/gaia_rock_inside/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x682
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/champa/entry.o"(.text)
        "*/games/tla/src/scene/champa.o"(.text)
        "*/games/tla/src/scene/champa/s0188.o"(.text)
        "*/games/tla/src/scene/champa/s05dc.o"(.text)
        "*/games/tla/src/scene/champa/s0b18.o"(.text)
        "*/games/tla/src/scene/champa/s0c40.o"(.text)
        "*/games/tla/src/scene/champa/s0e04.o"(.text)
        "*/games/tla/src/scene/champa/import.o"(.text)
        "*/games/tla/src/scene/champa/data.o"(.data)
    }
}
#elif OVERLAY == 0x683
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/champa_interiors/entry.o"(.text)
        "*/games/tla/src/scene/champa_interiors.o"(.text)
        "*/games/tla/src/scene/champa_interiors/import.o"(.text)
        "*/games/tla/src/scene/champa_interiors.o"(.data)
    }
}
#elif OVERLAY == 0x684
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/champa_sanctum/entry.o"(.text)
        "*/games/tla/src/scene/champa_sanctum.o"(.text)
        "*/games/tla/src/scene/champa_sanctum/import.o"(.text)
        "*/games/tla/src/scene/champa_sanctum.o"(.data)
    }
}
#elif OVERLAY == 0x685
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/champa_cliff_caves/entry.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s0038.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s04d4.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s0734.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s0804.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s0a34.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s0f48.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s115c.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s29f8.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s2c88.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s2d34.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s2de4.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s3164.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s320c.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s32a4.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/import.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s32a4.o"(.data)
        "*/games/tla/src/graphics/field/pal685.o"(.rodata)
        "*/games/tla/src/scene/champa_cliff_caves/data2.o"(.data)
    }
}
#elif OVERLAY == 0x686
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/ankohl_ruins/entry.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/object/prepare_object.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/spawn.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/s015c.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/scene_tables.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/events_b.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/s10f4.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/events_d.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/s1420.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/s160c.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/s2438.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/import.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/s2438.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/ankohl_ruins/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x687
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
        "*/games/tla/src/scene/sea_of_time/s0818.o"(.text)
        "*/games/tla/src/scene/sea_of_time/s0a80.o"(.text)
        "*/games/tla/src/scene/sea_of_time/clock.o"(.text)
        "*/games/tla/src/scene/sea_of_time/s0c94.o"(.text)
        "*/games/tla/src/scene/sea_of_time/s0ef0.o"(.text)
        "*/games/tla/src/scene/sea_of_time/enter.o"(.text)
        "*/games/tla/src/scene/sea_of_time/s1650.o"(.text)
        "*/games/tla/src/scene/sea_of_time/s1a14.o"(.text)
        "*/games/tla/src/scene/sea_of_time/s1cdc.o"(.text)
        "*/games/tla/src/scene/sea_of_time/import.o"(.text)
        "*/games/tla/src/scene/sea_of_time/data.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/sea_of_time/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x688
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/lemuria_entrance/entry.o"(.text)
        "*/games/tla/src/scene/lemuria_entrance.o"(.text)
        "*/games/tla/src/scene/lemuria_entrance/import.o"(.text)
        "*/games/tla/src/scene/lemuria_entrance.o"(.data)
    }
}
#elif OVERLAY == 0x689
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/lemuria_dock/entry.o"(.text)
        "*/games/tla/src/scene/lemuria_dock.o"(.text)
        "*/games/tla/src/scene/lemuria_dock/import.o"(.text)
        "*/games/tla/src/scene/lemuria_dock/data.o"(.data)
    }
}
#elif OVERLAY == 0x68a
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/ancient_lemuria/entry.o"(.text)
        "*/games/tla/src/scene/ancient_lemuria.o"(.text)
        "*/games/tla/src/scene/ancient_lemuria/import.o"(.text)
        "*/games/tla/src/scene/ancient_lemuria/data.o"(.data)
    }
}
#elif OVERLAY == 0x68b
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/lemuria/entry.o"(.text)
        "*/games/tla/src/scene/lemuria.o"(.text)
        "*/games/tla/src/scene/lemuria/import.o"(.text)
        "*/games/tla/src/scene/lemuria/data.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/lemuria/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x68c
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/lemuria_interiors/entry.o"(.text)
        "*/games/tla/src/scene/lemuria_interiors.o"(.text)
        "*/games/tla/src/scene/lemuria_interiors/s0a5c.o"(.text)
        "*/games/tla/src/scene/lemuria_interiors/s12dc.o"(.text)
        "*/games/tla/src/scene/lemuria_interiors/import.o"(.text)
        "*/games/tla/src/scene/lemuria_interiors/s12dc.o"(.data)
    }
}
#elif OVERLAY == 0x68d
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/lemuria_palace/entry.o"(.text)
        "*/games/tla/src/scene/lemuria_palace.o"(.text)
        "*/games/tla/src/scene/lemuria_palace/s03a0.o"(.text)
        "*/games/tla/src/scene/lemuria_palace/s058c.o"(.text)
        "*/games/tla/src/scene/lemuria_palace/import.o"(.text)
        "*/games/tla/src/scene/lemuria_palace/s058c.o"(.data)
        "*/games/tla/src/scene/lemuria_palace/tiles.o"(.rodata)
        "*/games/tla/src/scene/lemuria_palace/data2.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/lemuria_palace/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x68e
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/lemuria_spring/entry.o"(.text)
        "*/games/tla/src/scene/lemuria_spring.o"(.text)
        "*/games/tla/src/scene/lemuria_spring/s012c.o"(.text)
        "*/games/tla/src/scene/lemuria_spring/prepare.o"(.text)
        "*/games/tla/src/scene/lemuria_spring/s08f0.o"(.text)
        "*/games/tla/src/scene/lemuria_spring/s0904.o"(.text)
        "*/games/tla/src/scene/lemuria_spring/ride.o"(.text)
        "*/games/tla/src/scene/lemuria_spring/import.o"(.text)
        "*/games/tla/src/scene/lemuria_spring/data.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/lemuria_spring/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x68f
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/tundaria_tower/entry.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s00b8.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/script0.o"(.text)
        "*/games/tla/src/field/map/cell_set.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s0338.o"(.text)
        "*/games/tla/src/scene/tundaria_tower.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s05d0.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/fall.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/script3.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s0c64.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s0d2c.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s11e0.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/events_d.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/events_e.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s1b48.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/events_f.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s29d0.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s2b04.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s2e00.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/import.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/data.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/tundaria_tower/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x690
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/settlements/entry.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/settlements/s00b8.o"(.text)
        "*/games/tla/src/scene/settlements/s0280.o"(.text)
        "*/games/tla/src/scene/settlements/scene_tables.o"(.text)
        "*/games/tla/src/scene/settlements/placements.o"(.text)
        "*/games/tla/src/scene/settlements/exits.o"(.text)
        "*/games/tla/src/scene/settlements/s0358.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/settlements/s1620.o"(.text)
        "*/games/tla/src/scene/settlements/s173c.o"(.text)
        "*/games/tla/src/scene/settlements/s1a1c.o"(.text)
        "*/games/tla/src/scene/settlements/s1d30.o"(.text)
        "*/games/tla/src/scene/settlements/import.o"(.text)
        "*/games/tla/src/scene/settlements/data.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/settlements/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x691
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/magma_rock/entry.o"(.text)
        "*/games/tla/src/scene/magma_rock/create.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/magma_rock/s015c.o"(.text)
        "*/games/tla/src/scene/magma_rock/s03b4.o"(.text)
        "*/games/tla/src/scene/magma_rock.o"(.text)
        "*/games/tla/src/scene/magma_rock/s0c08.o"(.text)
        "*/games/tla/src/scene/magma_rock/s0ff4.o"(.text)
        "*/games/tla/src/scene/magma_rock/target.o"(.text)
        "*/games/tla/src/scene/magma_rock/s1738.o"(.text)
        "*/games/tla/src/scene/magma_rock/s1c00.o"(.text)
        "*/games/tla/src/scene/magma_rock/s1e08.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/magma_rock/s22ac.o"(.text)
        "*/games/tla/src/scene/magma_rock/s34ac.o"(.text)
        "*/games/tla/src/scene/magma_rock/import.o"(.text)
        "*/games/tla/src/scene/magma_rock/s34ac.o"(.data)
    }
}
#elif OVERLAY == 0x692
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/magma_rock_inside/entry.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s00d0.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s015c.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s033c.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s03b4.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s0594.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s0800.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s0880.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s08f8.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s0a30.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s0e20.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s0ed0.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s1148.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s1ca0.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s1d64.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s26a4.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s28ac.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s3678.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s447c.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s46e8.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s4934.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s4b18.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/import.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/data.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/magma_rock_inside/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x693
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/atteka_inlet/entry.o"(.text)
        "*/games/tla/src/scene/atteka_inlet.o"(.text)
        "*/games/tla/src/scene/atteka_inlet/import.o"(.text)
        "*/games/tla/src/scene/atteka_inlet/data.o"(.data)
    }
}
#elif OVERLAY == 0x694
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/contigo/entry.o"(.text)
        "*/games/tla/src/scene/contigo/s0038.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/contigo.o"(.text)
        "*/games/tla/src/scene/contigo/s3e54.o"(.text)
        "*/games/tla/src/scene/contigo/import.o"(.text)
        "*/games/tla/src/scene/contigo/s3e54.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/contigo/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x695
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/shaman_village_cave/entry.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave/import.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave.o"(.data)
    }
}
#elif OVERLAY == 0x696
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/shaman_village/entry.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/shaman_village/s00b8.o"(.text)
        "*/games/tla/src/scene/shaman_village/s0280.o"(.text)
        "*/games/tla/src/scene/shaman_village/scene_tables.o"(.text)
        "*/games/tla/src/scene/shaman_village/s02dc.o"(.text)
        "*/games/tla/src/field/field_effect/circle_blink.o"(.text)
        "*/games/tla/src/scene/shaman_village/s0508.o"(.text)
        "*/games/tla/src/scene/shaman_village/s057c.o"(.text)
        "*/games/tla/src/scene/shaman_village/s0c80.o"(.text)
        "*/games/tla/src/scene/shared/drift_spawn.o"(.text)
        "*/games/tla/src/scene/shaman_village/s1fec.o"(.text)
        "*/games/tla/src/scene/shaman_village/s2f20.o"(.text)
        "*/games/tla/src/scene/shaman_village/import.o"(.text)
        "*/games/tla/src/scene/shaman_village/data.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/shaman_village/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x697
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/trial_road/entry.o"(.text)
        "*/games/tla/src/scene/trial_road.o"(.text)
        "*/games/tla/src/scene/trial_road/s0218.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/trial_road/s129c.o"(.text)
        "*/games/tla/src/scene/trial_road/s15a8.o"(.text)
        "*/games/tla/src/scene/trial_road/s1fc4.o"(.text)
        "*/games/tla/src/scene/trial_road/s3f68.o"(.text)
        "*/games/tla/src/scene/trial_road/s4828.o"(.text)
        "*/games/tla/src/scene/trial_road/s4a94.o"(.text)
        "*/games/tla/src/scene/trial_road/s4d6c.o"(.text)
        "*/games/tla/src/scene/trial_road/balance.o"(.text)
        "*/games/tla/src/scene/trial_road/s4ddc.o"(.text)
        "*/games/tla/src/scene/trial_road/s51a0.o"(.text)
        "*/games/tla/src/scene/trial_road/import.o"(.text)
        "*/games/tla/src/scene/trial_road/s51a0.o"(.data)
        "*/games/tla/src/graphics/field/tiles64d.o"(.rodata)
        "*/games/tla/src/scene/trial_road/data2.o"(.data)
        "*/games/tla/src/scene/trial_road/picture.o"(.rodata)
        "*/games/tla/src/scene/trial_road/data3.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/trial_road/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x698
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/dist.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/s0eac.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/s14dc.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/s1674.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/import.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/s1674.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x699
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_basement/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement/s098c.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement/s1094.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement/s16c4.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement/s185c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement/import.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_basement/s185c.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_basement/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x69a
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_1/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_1.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_1/s0674.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_1/s0f3c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_1/s156c.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_1/s1704.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_1/import.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_1/s1704.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_1/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x69b
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/s0d94.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/s165c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/s1c8c.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/s1e24.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/import.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/s1e24.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x69c
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/s07b0.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/s0dcc.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/s1300.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/s1788.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/s1a7c.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/s1f50.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/import.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/s1f50.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/data2.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x69d
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_4/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_4.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_4/s0b50.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_4/s1258.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_4/s1888.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_4/s1a20.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_4/import.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_4/s1a20.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_4/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x69e
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/s0cb8.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/s1230.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/s1650.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/s185c.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/s1c88.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/s22b8.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/s2450.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/import.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/s2450.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x69f
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_6/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6/s00b4.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6/scene_events.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6/s01e8.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6/s047c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6/s0854.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6/s0f5c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6/s158c.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6/s1724.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6/import.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6/s1724.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_6/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x6a0
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/s16d4.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/s1f74.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/s25a4.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/s273c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/import.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/s273c.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/data2.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x6a1
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/s07e8.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/s0c44.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/s1178.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/s1600.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/s1c30.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/s1dc8.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/import.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/s1dc8.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/data2.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x6a2
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_9/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_9.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_9/s0910.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_9/s1018.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_9/s1648.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_9/s17e0.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_9/import.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_9/s17e0.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_9/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x6a3
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/s1c7c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/s3474.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/s3b7c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/s41ac.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/s4344.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/import.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/s4344.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/data2.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x6a4
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/s0d08.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/s17a8.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/s1e78.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/s24a8.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/s2640.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/import.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/s2640.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x6a5
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon.o"(.text)
        "*/games/tla/src/field/object/step_angle.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s0198.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s3f54.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s44cc.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s4bd4.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s4ec8.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s539c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/import.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s539c.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/data2.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x6a6
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/loho/entry.o"(.text)
        "*/games/tla/src/scene/loho.o"(.text)
        "*/games/tla/src/scene/loho/import.o"(.text)
        "*/games/tla/src/graphics/field/pal685.o"(.rodata)
        "*/games/tla/src/scene/loho/data.o"(.data)
    }
}
#elif OVERLAY == 0x6a7
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/northern_reaches/entry.o"(.text)
        "*/games/tla/src/scene/northern_reaches.o"(.text)
        "*/games/tla/src/scene/northern_reaches/import.o"(.text)
        "*/games/tla/src/graphics/field/pal685.o"(.rodata)
        "*/games/tla/src/scene/northern_reaches.o"(.data)
    }
}
#elif OVERLAY == 0x6a8
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/prox/entry.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/prox/s00b8.o"(.text)
        "*/games/tla/src/scene/prox.o"(.text)
        "*/games/tla/src/scene/prox/s1cf0.o"(.text)
        "*/games/tla/src/scene/prox/s391c.o"(.text)
        "*/games/tla/src/scene/prox/s4254.o"(.text)
        "*/games/tla/src/scene/prox/import.o"(.text)
        "*/games/tla/src/scene/prox/s4254.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/prox/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x6a9
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/caverns/entry.o"(.text)
        "*/games/tla/src/scene/caverns/object.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/caverns/s015c.o"(.text)
        "*/games/tla/src/scene/caverns.o"(.text)
        "*/games/tla/src/scene/caverns/s0454.o"(.text)
        "*/games/tla/src/scene/caverns/import.o"(.text)
        "*/games/tla/src/scene/caverns/data.o"(.data)
    }
}
#elif OVERLAY == 0x6aa
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/mars_lighthouse_entrance/entry.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/calls.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s01f8.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s0338.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s1254.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s1344.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s138c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s1930.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s1c44.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s1e60.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s20ec.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s2270.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s263c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s2860.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/ride_active.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s2f4c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s32b8.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s3564.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s3884.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s3b30.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s3cf8.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s42f8.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s4a60.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/import.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s4a60.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/mars_lighthouse_entrance/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x6ab
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/mars_lighthouse_star_room/entry.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s0ae4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s1424.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/spawn.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s19b4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s1b78.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s1bf8.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s1d7c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/ride_handle.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s2148.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s236c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/ride_active.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s2a58.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s2dc4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s3070.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/set_data.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s363c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s3804.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s3e04.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/import.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s3e04.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/mars_lighthouse_star_room/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x6ac
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/mars_lighthouse_room_m/entry.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s0b64.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/stop.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s1988.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s1b64.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s1d08.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s1f94.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s2118.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s2708.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/import.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s2708.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/mars_lighthouse_room_m/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x6ad
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/mars_lighthouse_room_f/entry.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s167c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s1a54.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s1e20.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s1f84.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s1ffc.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s21a0.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s242c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s25b0.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/ride.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/flash.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s2f80.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s328c.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s35b8.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s3d80.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s4700.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s5048.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/import.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s5048.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/mars_lighthouse_room_f/data2.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/mars_lighthouse_room_f/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x6ae
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/mars_lighthouse_aerie/entry.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/s3370.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/s53e4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/import.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/s53e4.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/mars_lighthouse_aerie/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x6af
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/treasure_isle/entry.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s00d0.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s015c.o"(.text)
        "*/games/tla/src/scene/treasure_isle/scene_tables.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s043c.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s05ac.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s0880.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s0cf4.o"(.text)
        "*/games/tla/src/scene/treasure_isle/import.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s0cf4.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/treasure_isle/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x6b0
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/anemos_inner_sanctum/entry.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s0038.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s0480.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/scene_tables.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s09b8.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s0a78.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s0ac8.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s1398.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s17b4.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s1938.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/move.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s2db0.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s2f48.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/import.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s2f48.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/anemos_inner_sanctum/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x6b1
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/entry.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/object/prepare_object.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/spawn.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/s015c.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/hide.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/s0588.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/list.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/import.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/list.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_d.o"(.rodata)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/data2.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x6b2
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/mercury_lighthouse/entry.o"(.text)
        "*/games/tla/src/scene/mercury_lighthouse.o"(.text)
        "*/games/tla/src/scene/mercury_lighthouse/import.o"(.text)
        "*/games/tla/src/scene/mercury_lighthouse.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/mercury_lighthouse/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x6b3
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/epilogue/entry.o"(.text)
        "*/games/tla/src/scene/epilogue.o"(.text)
        "*/games/tla/src/scene/epilogue/s0098.o"(.text)
        "*/games/tla/src/scene/epilogue/import.o"(.text)
        "*/games/tla/src/scene/epilogue/pal.o"(.rodata)
        "*/games/tla/src/scene/epilogue.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/epilogue/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x6b4
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/mt_aleph_summit/entry.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/mt_aleph_summit.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/mt_aleph_summit/s0478.o"(.text)
        "*/games/tla/src/scene/mt_aleph_summit/import.o"(.text)
        "*/games/tla/src/scene/mt_aleph_summit.o"(.data)
    }
}
#elif OVERLAY == 0x6b5
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/lobby/entry.o"(.text)
        "*/games/tla/src/scene/lobby/scene_tables.o"(.text)
        "*/games/tla/src/scene/lobby/lobby.o"(.text)
        "*/games/tla/src/scene/lobby/s0fbc.o"(.text)
        "*/games/tla/src/scene/lobby/talk.o"(.text)
        "*/games/tla/src/scene/lobby/scene_events.o"(.text)
        "*/games/tla/src/scene/lobby/import.o"(.text)
        "*/games/tla/src/scene/lobby/data.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/lobby/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x6b6
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/debug/rooms/entry.o"(.text)
        "*/games/tla/src/debug/test_scenes.o"(.text)
        "*/games/tla/src/debug/rooms/import.o"(.text)
        "*/games/tla/src/debug/test_scenes.o"(.data)
    }
}
#elif OVERLAY == 0x6b7
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/debug/level/entry.o"(.text)
        "*/games/tla/src/debug/level.o"(.text)
        "*/games/tla/src/debug/level/import.o"(.text)
        "*/games/tla/src/debug/level.o"(.data)
    }
}
#elif OVERLAY == 0x6b8
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/debug/psynergy_test/entry.o"(.text)
        "*/games/tla/src/debug/psynergy_test.o"(.text)
        "*/games/tla/src/debug/psynergy_test/s0c4c.o"(.text)
        "*/games/tla/src/debug/psynergy_test/import.o"(.text)
        "*/games/tla/src/debug/psynergy_test.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/debug/psynergy_test/bss.o"(.bss)
    }
}
#elif OVERLAY == 0x6b9
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/debug/psynergy_test_2/entry.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2/import.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2.o"(.data)
    }
}
#elif OVERLAY == 0x6ba
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/debug/shop_test/entry.o"(.text)
        "*/games/tla/src/debug/shop_test.o"(.text)
        "*/games/tla/src/debug/shop_test/s0520.o"(.text)
        "*/games/tla/src/debug/shop_test/import.o"(.text)
        "*/games/tla/src/debug/shop_test.o"(.data)
    }
}
#endif

SECTIONS
{
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
