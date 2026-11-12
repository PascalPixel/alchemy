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
        "*/games/tla/src/scene/title/data.o"(.data)
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
        "*/games/tla/src/scene/clear/s0328.o"(.text)
        "*/games/tla/src/scene/clear/s0e44.o"(.text)
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
        "*/games/tla/src/scene/world_map/s29a4.o"(.text)
        "*/games/tla/src/scene/world_map/s2e78.o"(.text)
        "*/games/tla/src/scene/world_map/s3600.o"(.text)
        "*/games/tla/src/scene/world_map/s36a8.o"(.text)
        "*/games/tla/src/scene/world_map/s3960.o"(.text)
        "*/games/tla/src/scene/world_map/cell_index.o"(.text)
        "*/games/tla/src/scene/world_map/s39f4.o"(.text)
        "*/games/tla/src/scene/world_map/s3b38.o"(.text)
        "*/games/tla/src/scene/world_map/s3d44.o"(.text)
        "*/games/tla/src/scene/world_map/s4744.o"(.text)
        "*/games/tla/src/scene/world_map/release.o"(.text)
        "*/games/tla/src/scene/world_map/import.o"(.text)
        "*/games/tla/src/scene/world_map/data.o"(.data)
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
        "*/games/tla/src/scene/staff_credit/s0fa0.o"(.text)
        "*/games/tla/src/scene/staff_credit/s1ad0.o"(.text)
        "*/games/tla/src/scene/staff_credit/import.o"(.text)
        "*/games/tla/src/scene/staff_credit/graphics.o"(.rodata)
        "*/games/tla/src/scene/staff_credit/data.o"(.data)
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
        "*/games/tla/src/scene/venus_lighthouse_entrance/s0128.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/prepare.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/s0510.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/s1c30.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/s23a4.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/s26f4.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/sparks.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/import.o"(.text)
        "*/games/tla/src/graphics/field/tiles64d.o"(.rodata)
        "*/games/tla/src/scene/venus_lighthouse_entrance/data.o"(.data)
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
        "*/games/tla/src/scene/venus_lighthouse/distance.o"(.text)
        "*/games/tla/src/field/object/find_in_cell.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse/s038c.o"(.text)
        "*/games/tla/src/field/map/cell_fill_byte.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse/floor.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse/push.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse/s0a54.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse/import.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse/data.o"(.data)
    }
}
#elif OVERLAY == 0x64f
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/suhalla_gate/entry.o"(.text)
        "*/games/tla/src/scene/suhalla_gate/events_a.o"(.text)
        "*/games/tla/src/scene/suhalla_gate.o"(.text)
        "*/games/tla/src/scene/suhalla_gate/events_b.o"(.text)
        "*/games/tla/src/scene/suhalla_gate/s0494.o"(.text)
        "*/games/tla/src/scene/suhalla_gate/s05e8.o"(.text)
        "*/games/tla/src/scene/suhalla_gate/import.o"(.text)
        "*/games/tla/src/scene/suhalla_gate/data.o"(.data)
    }
}
#elif OVERLAY == 0x650
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/idejima/entry.o"(.text)
        "*/games/tla/src/scene/idejima.o"(.text)
        "*/games/tla/src/scene/shared/drift_spawn.o"(.text)
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
        "*/games/tla/src/scene/daila_interiors/data.o"(.data)
        "*/games/tla/src/scene/daila_interiors/data2.o"(.data)
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
        "*/games/tla/src/scene/kandorean_temple/events_a.o"(.text)
        "*/games/tla/src/scene/kandorean_temple/scene_tables.o"(.text)
        "*/games/tla/src/scene/kandorean_temple/s0390.o"(.text)
        "*/games/tla/src/scene/kandorean_temple/exits.o"(.text)
        "*/games/tla/src/scene/kandorean_temple/s03d0.o"(.text)
        "*/games/tla/src/scene/kandorean_temple/s2c1c.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/kandorean_temple/s2ec4.o"(.text)
        "*/games/tla/src/scene/kandorean_temple/import.o"(.text)
        "*/games/tla/src/scene/kandorean_temple/data.o"(.data)
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
        "*/games/tla/src/scene/dehkan_plateau/s00a4.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s0128.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s01d0.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s02f8.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s048c.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/events_l.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s0584.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/events_m.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s0708.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion_priority.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s0974.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s0aa8.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s0bc0.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/events_n.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s0c64.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s0d78.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s0e9c.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s0fc4.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/events_o.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/scene_tables.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s11b8.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/events_a.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s1368.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/events_b.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s1444.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s1474.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/events_c.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s1514.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/events_d.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s1820.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s18a8.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s1930.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s19b8.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s1a30.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/events_e.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s2124.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/events_f.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/events_g.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/events_h.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s2698.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s27e0.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s2a5c.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s2b68.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/events_i.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s2c14.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s2dd4.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s2f08.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/events_j.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s2ff4.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/events_k.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s30e4.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s3164.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s3284.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s353c.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s35ec.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s369c.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s3764.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s390c.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s3a30.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/s3aa0.o"(.text)
        "*/games/tla/src/scene/dehkan_plateau/prepare.o"(.text)
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
        "*/games/tla/src/scene/lemurian_ship/s021c.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s0324.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s0438.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s04f4.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s065c.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s0770.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s0870.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s0af4.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s0e2c.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s10bc.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s12b4.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s1368.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s155c.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s1650.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s1944.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s1a9c.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s1abc.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s1c38.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s1c88.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s1dc8.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s1e0c.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s23f8.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s2498.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s27e4.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s2820.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s2970.o"(.text)
        "*/games/tla/src/scene/lemurian_ship/s2b98.o"(.text)
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
        "*/games/tla/src/scene/madra/scene_tables.o"(.text)
        "*/games/tla/src/scene/madra/s006c.o"(.text)
        "*/games/tla/src/scene/madra/events_a.o"(.text)
        "*/games/tla/src/scene/madra/s02bc.o"(.text)
        "*/games/tla/src/scene/madra/events_b.o"(.text)
        "*/games/tla/src/scene/madra/s051c.o"(.text)
        "*/games/tla/src/scene/madra/events_c.o"(.text)
        "*/games/tla/src/scene/madra/s068c.o"(.text)
        "*/games/tla/src/scene/madra/s06ac.o"(.text)
        "*/games/tla/src/scene/madra/prepare.o"(.text)
        "*/games/tla/src/scene/madra/s0ac4.o"(.text)
        "*/games/tla/src/scene/madra/events_d.o"(.text)
        "*/games/tla/src/scene/madra/import.o"(.text)
        "*/games/tla/src/scene/madra/data.o"(.data)
    }
}
#elif OVERLAY == 0x659
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/madra_interiors/entry.o"(.text)
        "*/games/tla/src/scene/madra_interiors/scene_tables.o"(.text)
        "*/games/tla/src/scene/madra_interiors/s004c.o"(.text)
        "*/games/tla/src/scene/madra_interiors/s01b8.o"(.text)
        "*/games/tla/src/scene/madra_interiors/s0570.o"(.text)
        "*/games/tla/src/scene/madra_interiors/s0624.o"(.text)
        "*/games/tla/src/scene/madra_interiors/prepare.o"(.text)
        "*/games/tla/src/scene/madra_interiors/events.o"(.text)
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
        "*/games/tla/src/scene/madra_catacombs/s0038.o"(.text)
        "*/games/tla/src/scene/madra_catacombs/s014c.o"(.text)
        "*/games/tla/src/scene/madra_catacombs/s0270.o"(.text)
        "*/games/tla/src/scene/madra_catacombs/s0318.o"(.text)
        "*/games/tla/src/scene/madra_catacombs/scene_tables.o"(.text)
        "*/games/tla/src/scene/madra_catacombs/s0388.o"(.text)
        "*/games/tla/src/scene/madra_catacombs/events_a.o"(.text)
        "*/games/tla/src/scene/madra_catacombs/events_d.o"(.text)
        "*/games/tla/src/scene/madra_catacombs/s0874.o"(.text)
        "*/games/tla/src/scene/madra_catacombs/events_b.o"(.text)
        "*/games/tla/src/scene/madra_catacombs/s093c.o"(.text)
        "*/games/tla/src/scene/madra_catacombs/s0958.o"(.text)
        "*/games/tla/src/scene/madra_catacombs/events_f.o"(.text)
        "*/games/tla/src/scene/madra_catacombs/s0a88.o"(.text)
        "*/games/tla/src/scene/madra_catacombs/events_c.o"(.text)
        "*/games/tla/src/scene/madra_catacombs/s0d54.o"(.text)
        "*/games/tla/src/scene/madra_catacombs/prepare.o"(.text)
        "*/games/tla/src/scene/madra_catacombs/events_e.o"(.text)
        "*/games/tla/src/scene/madra_catacombs/s102c.o"(.text)
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
        "*/games/tla/src/scene/madra_drawbridge/scene_tables.o"(.text)
        "*/games/tla/src/scene/madra_drawbridge/s0054.o"(.text)
        "*/games/tla/src/scene/madra_drawbridge/scene_events.o"(.text)
        "*/games/tla/src/scene/madra_drawbridge/s00c8.o"(.text)
        "*/games/tla/src/scene/madra_drawbridge/s0224.o"(.text)
        "*/games/tla/src/scene/madra_drawbridge/s0280.o"(.text)
        "*/games/tla/src/scene/madra_drawbridge/prepare.o"(.text)
        "*/games/tla/src/scene/madra_drawbridge/import.o"(.text)
        "*/games/tla/src/scene/madra_drawbridge/data.o"(.data)
    }
}
#elif OVERLAY == 0x65c
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/osenia_cliffs/entry.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs/scene_tables.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs/s0054.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs/scene_events.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs/s0074.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs/s00fc.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs/prepare.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs/import.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs/data.o"(.data)
    }
}
#elif OVERLAY == 0x65d
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/mikasalla/entry.o"(.text)
        "*/games/tla/src/scene/mikasalla/scene_tables.o"(.text)
        "*/games/tla/src/scene/mikasalla/exits.o"(.text)
        "*/games/tla/src/scene/mikasalla/events_a.o"(.text)
        "*/games/tla/src/scene/mikasalla/events_b.o"(.text)
        "*/games/tla/src/scene/mikasalla/s0438.o"(.text)
        "*/games/tla/src/scene/mikasalla/events_c.o"(.text)
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
        "*/games/tla/src/scene/mikasalla_interiors/scene_tables.o"(.text)
        "*/games/tla/src/scene/mikasalla_interiors/events.o"(.text)
        "*/games/tla/src/scene/mikasalla_interiors/events3.o"(.text)
        "*/games/tla/src/scene/mikasalla_interiors/s0340.o"(.text)
        "*/games/tla/src/scene/mikasalla_interiors/prepare.o"(.text)
        "*/games/tla/src/scene/mikasalla_interiors/s0470.o"(.text)
        "*/games/tla/src/scene/mikasalla_interiors/events2.o"(.text)
        "*/games/tla/src/scene/mikasalla_interiors/import.o"(.text)
        "*/games/tla/src/scene/mikasalla_interiors/data.o"(.data)
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
        "*/games/tla/src/scene/garoh/layer.o"(.text)
        "*/games/tla/src/scene/garoh/s02a4.o"(.text)
        "*/games/tla/src/scene/garoh/s0390.o"(.text)
        "*/games/tla/src/scene/garoh/hide.o"(.text)
        "*/games/tla/src/scene/garoh/scene_tables.o"(.text)
        "*/games/tla/src/scene/garoh/s0440.o"(.text)
        "*/games/tla/src/scene/garoh/talk.o"(.text)
        "*/games/tla/src/scene/garoh/s06ec.o"(.text)
        "*/games/tla/src/scene/garoh/s0808.o"(.text)
        "*/games/tla/src/scene/garoh/gate.o"(.text)
        "*/games/tla/src/scene/garoh/chime.o"(.text)
        "*/games/tla/src/scene/garoh/s0aa8.o"(.text)
        "*/games/tla/src/scene/garoh/prepare.o"(.text)
        "*/games/tla/src/scene/garoh/s0be4.o"(.text)
        "*/games/tla/src/scene/garoh/s0d30.o"(.text)
        "*/games/tla/src/scene/garoh/lift_a.o"(.text)
        "*/games/tla/src/scene/garoh/lift.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/garoh/s12a8.o"(.text)
        "*/games/tla/src/scene/garoh/spawn.o"(.text)
        "*/games/tla/src/scene/garoh/push.o"(.text)
        "*/games/tla/src/scene/garoh/s1490.o"(.text)
        "*/games/tla/src/scene/garoh/s22f4.o"(.text)
        "*/games/tla/src/scene/garoh/s2694.o"(.text)
        "*/games/tla/src/scene/garoh/s3878.o"(.text)
        "*/games/tla/src/scene/garoh/import.o"(.text)
        "*/games/tla/src/scene/garoh/data.o"(.data)
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
        "*/games/tla/src/scene/garoh_interiors/scene_tables.o"(.text)
        "*/games/tla/src/scene/garoh_interiors/s004c.o"(.text)
        "*/games/tla/src/scene/garoh_interiors/s039c.o"(.text)
        "*/games/tla/src/scene/garoh_interiors/s0580.o"(.text)
        "*/games/tla/src/scene/garoh_interiors/prepare.o"(.text)
        "*/games/tla/src/scene/garoh_interiors/s0710.o"(.text)
        "*/games/tla/src/scene/garoh_interiors/import.o"(.text)
        "*/games/tla/src/scene/garoh_interiors/data.o"(.data)
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
        "*/games/tla/src/scene/yampi_desert/s03b8.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s03c8.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s04bc.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s04e0.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s0550.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s05a8.o"(.text)
        "*/games/tla/src/scene/yampi_desert/signs.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s06ac.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s07e0.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s0918.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s0994.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s0a10.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s0a8c.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion_priority.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s0b84.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s0c34.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s0e78.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s0fc8.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s1288.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s12d4.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s12dc.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s13f8.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s1518.o"(.text)
        "*/games/tla/src/scene/yampi_desert/events_a.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s1690.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s1b70.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s1bec.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s1c5c.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s1cc4.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s1e58.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s2010.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s20a8.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s20e4.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s2140.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s2180.o"(.text)
        "*/games/tla/src/scene/yampi_desert/events_b.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s21f0.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s2208.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s2318.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s23cc.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s26a0.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s2764.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s2b84.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s2c6c.o"(.text)
        "*/games/tla/src/scene/yampi_desert/events_c.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s2d6c.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s2e14.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s2e98.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s2ebc.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s3094.o"(.text)
        "*/games/tla/src/scene/yampi_desert/prepare.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s316c.o"(.text)
        "*/games/tla/src/scene/yampi_desert/events_d.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s3364.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s35b8.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s36d0.o"(.text)
        "*/games/tla/src/scene/yampi_desert/events_e.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s3820.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s388c.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s3910.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s39b8.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s3b80.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s3da0.o"(.text)
        "*/games/tla/src/scene/yampi_desert/s3e3c.o"(.text)
        "*/games/tla/src/scene/yampi_desert/import.o"(.text)
        "*/games/tla/src/scene/yampi_desert/data.o"(.data)
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
        "*/games/tla/src/field/map/cellfill.o"(.text)
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
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/object/prepare_object.o"(.text)
        "*/games/tla/src/scene/airs_rock/s00d0.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/airs_rock/s015c.o"(.text)
        "*/games/tla/src/field/object/wait_until_idle.o"(.text)
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
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/object/prepare_object.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/object.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s02f4.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s0550.o"(.text)
        "*/games/tla/src/field/object/wait_until_idle.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/cell_get.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/scene_tables.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s2604.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s26c0.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s2770.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s2cf0.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s3170.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s3654.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s3ac4.o"(.text)
        "*/games/tla/src/scene/airs_rock_inside/s4740.o"(.text)
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
        "*/games/tla/src/field/map/cellfill.o"(.text)
        "*/games/tla/src/scene/eastern_alhafra/s4240.o"(.text)
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
        "*/games/tla/src/scene/eastern_alhafra_ship/data.o"(.data)
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
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/object/prepare_object.o"(.text)
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
        "*/games/tla/src/scene/gondowan_cliffs_hideout/data.o"(.data)
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
        "*/games/tla/src/scene/naribwe_interiors/data.o"(.data)
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
        "*/games/tla/src/scene/kibombo/data.o"(.data)
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
        "*/games/tla/src/scene/gabomba_statue/s11bc.o"(.text)
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
        "*/games/tla/src/scene/gabomba_statue/data.o"(.data)
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
        "*/games/tla/src/scene/gabomba_catacombs/data.o"(.data)
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
        "*/games/tla/src/scene/yallam/s0038.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse/distance.o"(.text)
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
        "*/games/tla/src/scene/yallam_interiors/data.o"(.data)
        "*/games/tla/src/scene/yallam_interiors/data2.o"(.data)
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
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/object/prepare_object.o"(.text)
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
        "*/games/tla/src/scene/apojii_islands/data.o"(.data)
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
        "*/games/tla/src/scene/apojii_islands_cave/data.o"(.data)
    }
}
#elif OVERLAY == 0x67b
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/aqua_rock/entry.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/object/prepare_object.o"(.text)
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
        "*/games/tla/src/scene/aqua_rock/data.o"(.data)
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
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/object/prepare_object.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s00d0.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s015c.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s033c.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s042c.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s0cc8.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s13bc.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside/s1598.o"(.text)
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
        "*/games/tla/src/scene/aqua_rock_inside/data.o"(.data)
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
        "*/games/tla/src/scene/izumo_interiors/data.o"(.data)
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
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
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
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/object/prepare_object.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s00d0.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s015c.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion_priority.o"(.text)
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
        "*/games/tla/src/scene/gaia_rock_inside/s225c.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s2ac8.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s2b04.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s2b58.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s2b8c.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s2cb4.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s2f30.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s4274.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s495c.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/s4d00.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/import.o"(.text)
        "*/games/tla/src/scene/gaia_rock_inside/data.o"(.data)
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
        "*/games/tla/src/scene/champa/s0e30.o"(.text)
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
        "*/games/tla/src/scene/champa_interiors/data.o"(.data)
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
        "*/games/tla/src/scene/champa_sanctum/data.o"(.data)
    }
}
#elif OVERLAY == 0x685
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/champa_cliff_caves/entry.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s0038.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/scene_tables.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s0070.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s04d4.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s0734.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s0804.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s0978.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s0a34.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s0f48.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s115c.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s1774.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s29f8.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s2c88.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s2d34.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s2de4.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s3164.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s320c.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/s32a4.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/import.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves/data.o"(.data)
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
        "*/games/tla/src/scene/ankohl_ruins/placements.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/exits.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/events_a.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/s0418.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/events_b.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/s10f4.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/events_d.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/s1420.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/s160c.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/s2438.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/prepare.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/import.o"(.text)
        "*/games/tla/src/scene/ankohl_ruins/data.o"(.data)
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
        "*/games/tla/src/scene/sea_of_time/s0da0.o"(.text)
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
        "*/games/tla/src/scene/lemuria_entrance/data.o"(.data)
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
        "*/games/tla/src/scene/lemuria_interiors/s128c.o"(.text)
        "*/games/tla/src/scene/lemuria_interiors/s12dc.o"(.text)
        "*/games/tla/src/scene/lemuria_interiors/import.o"(.text)
        "*/games/tla/src/scene/lemuria_interiors/data.o"(.data)
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
        "*/games/tla/src/scene/lemuria_palace/s04e8.o"(.text)
        "*/games/tla/src/scene/lemuria_palace/s058c.o"(.text)
        "*/games/tla/src/scene/lemuria_palace/import.o"(.text)
        "*/games/tla/src/scene/lemuria_palace/data.o"(.data)
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
        "*/games/tla/src/field/object/wait_until_idle.o"(.text)
        "*/games/tla/src/field/map/cell_get.o"(.text)
        "*/games/tla/src/field/map/cell_set.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s0338.o"(.text)
        "*/games/tla/src/scene/tundaria_tower.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s05d0.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/fall.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/script3.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s0b3c.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s0c64.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s0d2c.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s11e0.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/events_d.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s17dc.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/events_e.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s1b48.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/events_f.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s2764.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion_priority.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s29d0.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s2b04.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s2c1c.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/s2e00.o"(.text)
        "*/games/tla/src/scene/tundaria_tower/sparks.o"(.text)
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
        "*/games/tla/src/scene/settlements/s0b18.o"(.text)
        "*/games/tla/src/scene/settlements/s0c1c.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/settlements/s1620.o"(.text)
        "*/games/tla/src/scene/settlements/s16b0.o"(.text)
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
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/object/prepare_object.o"(.text)
        "*/games/tla/src/scene/magma_rock/create.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/magma_rock/s015c.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse/distance.o"(.text)
        "*/games/tla/src/field/object/find_in_cell.o"(.text)
        "*/games/tla/src/scene/magma_rock/s03b4.o"(.text)
        "*/games/tla/src/field/map/cell_fill_byte.o"(.text)
        "*/games/tla/src/scene/magma_rock/distance.o"(.text)
        "*/games/tla/src/field/map/cell_get.o"(.text)
        "*/games/tla/src/scene/magma_rock/s0668.o"(.text)
        "*/games/tla/src/scene/magma_rock.o"(.text)
        "*/games/tla/src/scene/magma_rock/s0a48.o"(.text)
        "*/games/tla/src/scene/magma_rock/s0c08.o"(.text)
        "*/games/tla/src/scene/magma_rock/s0ff4.o"(.text)
        "*/games/tla/src/scene/magma_rock/s1084.o"(.text)
        "*/games/tla/src/scene/magma_rock/s10cc.o"(.text)
        "*/games/tla/src/scene/magma_rock/s11e4.o"(.text)
        "*/games/tla/src/scene/magma_rock/target.o"(.text)
        "*/games/tla/src/scene/magma_rock/s1738.o"(.text)
        "*/games/tla/src/scene/magma_rock/s19e8.o"(.text)
        "*/games/tla/src/scene/magma_rock/s1c00.o"(.text)
        "*/games/tla/src/scene/magma_rock/s1e08.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/magma_rock/s22ac.o"(.text)
        "*/games/tla/src/scene/magma_rock/set_state.o"(.text)
        "*/games/tla/src/scene/magma_rock/s30bc.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion_priority.o"(.text)
        "*/games/tla/src/scene/magma_rock/s34ac.o"(.text)
        "*/games/tla/src/scene/magma_rock/import.o"(.text)
        "*/games/tla/src/scene/magma_rock/data.o"(.data)
    }
}
#elif OVERLAY == 0x692
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/magma_rock_inside/entry.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/object/prepare_object.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s00d0.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s015c.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s033c.o"(.text)
        "*/games/tla/src/field/object/find_in_cell.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s03b4.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s0538.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s0594.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s05dc.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/map.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s06dc.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s075c.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s07ac.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s0800.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s0880.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/scene_tables.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s08f8.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s0a30.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s0ce4.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s0e20.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s0ea4.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s0ed0.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s1148.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s1350.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s1828.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s1ca0.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s1d64.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s255c.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s26a4.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s27d4.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s28ac.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s29dc.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s34bc.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s34f0.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s3678.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s3ee0.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s3f44.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s406c.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s42f8.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s4460.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s447c.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion_priority.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s46e8.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s481c.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s4934.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s49c0.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s49d8.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s4a5c.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s4b18.o"(.text)
        "*/games/tla/src/scene/magma_rock_inside/s4bd4.o"(.text)
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
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/contigo.o"(.text)
        "*/games/tla/src/scene/contigo/s3e54.o"(.text)
        "*/games/tla/src/scene/contigo/mark.o"(.text)
        "*/games/tla/src/scene/contigo/import.o"(.text)
        "*/games/tla/src/scene/contigo/data.o"(.data)
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
        "*/games/tla/src/scene/shaman_village_cave/scene_tables.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave/s0098.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave/placements.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave/scene_events.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave/s0430.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave/import.o"(.text)
        "*/games/tla/src/scene/shaman_village_cave/data.o"(.data)
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
        "*/games/tla/src/scene/shaman_village/s0378.o"(.text)
        "*/games/tla/src/scene/shaman_village/s0420.o"(.text)
        "*/games/tla/src/field/field_effect/circle_blink.o"(.text)
        "*/games/tla/src/scene/shaman_village/s0508.o"(.text)
        "*/games/tla/src/scene/shaman_village/s057c.o"(.text)
        "*/games/tla/src/scene/shaman_village/s0c80.o"(.text)
        "*/games/tla/src/scene/shaman_village/s0d08.o"(.text)
        "*/games/tla/src/scene/shared/drift_spawn.o"(.text)
        "*/games/tla/src/scene/shaman_village/s1fec.o"(.text)
        "*/games/tla/src/scene/shaman_village/s2f20.o"(.text)
        "*/games/tla/src/scene/shaman_village/lettering.o"(.text)
        "*/games/tla/src/scene/shaman_village/import.o"(.text)
        "*/games/tla/src/scene/shaman_village/data.o"(.data)
        "*/games/tla/src/scene/shaman_village/data2.o"(.data)
        "*/games/tla/src/scene/shaman_village/data3.o"(.data)
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
        "*/games/tla/src/scene/trial_road/s0138.o"(.text)
        "*/games/tla/src/scene/trial_road/s0218.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/trial_road/s129c.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion_priority.o"(.text)
        "*/games/tla/src/scene/trial_road/s14e4.o"(.text)
        "*/games/tla/src/scene/trial_road/s15a8.o"(.text)
        "*/games/tla/src/scene/trial_road/s1890.o"(.text)
        "*/games/tla/src/scene/trial_road/s1fc4.o"(.text)
        "*/games/tla/src/scene/trial_road/s200c.o"(.text)
        "*/games/tla/src/scene/trial_road/s3f68.o"(.text)
        "*/games/tla/src/scene/trial_road/s4188.o"(.text)
        "*/games/tla/src/scene/trial_road/s4418.o"(.text)
        "*/games/tla/src/scene/trial_road/s45ac.o"(.text)
        "*/games/tla/src/scene/trial_road/s468c.o"(.text)
        "*/games/tla/src/scene/trial_road/s46a4.o"(.text)
        "*/games/tla/src/scene/trial_road/s480c.o"(.text)
        "*/games/tla/src/scene/trial_road/s4828.o"(.text)
        "*/games/tla/src/scene/trial_road/s4a94.o"(.text)
        "*/games/tla/src/scene/trial_road/s4bc8.o"(.text)
        "*/games/tla/src/scene/trial_road/s4ce0.o"(.text)
        "*/games/tla/src/scene/trial_road/s4d6c.o"(.text)
        "*/games/tla/src/scene/trial_road/s4d84.o"(.text)
        "*/games/tla/src/scene/trial_road/balance.o"(.text)
        "*/games/tla/src/scene/trial_road/s4ddc.o"(.text)
        "*/games/tla/src/scene/trial_road/s51a0.o"(.text)
        "*/games/tla/src/scene/trial_road/import.o"(.text)
        "*/games/tla/src/scene/trial_road/data.o"(.data)
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
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/s00f4.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/s0184.o"(.text)
        "*/games/tla/src/scene/shared/scene_set_arrival_flags.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/s022c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/s0238.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/lightset.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/s03cc.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/dist.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/s0eac.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/s11a0.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/s13e4.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/s14dc.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/s1674.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/s18bc.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/import.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance/data.o"(.data)
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
        "*/games/tla/src/scene/shared/scene_set_arrival_flags.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement/s0414.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement/s05b4.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement/dist.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement/s098c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement/s0a64.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement/s1094.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement/s1388.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement/s15cc.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement/s16c4.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement/s185c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement/s1aa4.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement/import.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_basement/data.o"(.data)
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
        "*/games/tla/src/scene/shared/scene_set_arrival_flags.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_1/lightmod.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_1/s045c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_1/s0674.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_1/s0f3c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_1/s1230.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_1/s156c.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_1/s1704.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_1/s194c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_1/import.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_1/data.o"(.data)
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
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/scene_tables.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/s0054.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/s00d0.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/s04d0.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/s0500.o"(.text)
        "*/games/tla/src/scene/shared/scene_set_arrival_flags.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/s09dc.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/s09e8.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/s0b7c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/s0d94.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/s15d0.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/s165c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/s1950.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/s1c8c.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/s1e24.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/s206c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/import.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2/data.o"(.data)
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
        "*/games/tla/src/scene/shared/scene_set_arrival_flags.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/events_a.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/s0598.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/s07b0.o"(.text)
        "*/games/tla/src/field/map/cellfill.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/s0dcc.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/s1050.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/s1300.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/s1788.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/s1a7c.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/s1f50.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/s2198.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/import.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3/data.o"(.data)
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
        "*/games/tla/src/scene/shared/scene_set_arrival_flags.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_4/s05d8.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_4/s0778.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_4/s0b50.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_4/s1258.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_4/s154c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_4/s1888.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_4/s1a20.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_4/s1c68.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_4/import.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_4/data.o"(.data)
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
        "*/games/tla/src/scene/shared/scene_set_arrival_flags.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/s0cb8.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/s0e58.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/s1230.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/s1650.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/s1718.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/s185c.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/s1c88.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/s1f7c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/s22b8.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/s2450.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/s2698.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/import.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5/data.o"(.data)
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
        "*/games/tla/src/scene/shared/scene_set_arrival_flags.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6/set_value.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6/s047c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6/s0854.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6/s0f5c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6/s1250.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6/s158c.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6/s1724.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6/s196c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6/import.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6/data.o"(.data)
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
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/s0060.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/s05a0.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/s05e8.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/scene_events.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/s0fc0.o"(.text)
        "*/games/tla/src/scene/shared/scene_set_arrival_flags.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/set_value.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/s12fc.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/s16d4.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/s1f74.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/s2268.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/s25a4.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/s273c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/s2984.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/import.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7/data.o"(.data)
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
        "*/games/tla/src/scene/shared/scene_set_arrival_flags.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/s0270.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/s0410.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/s07e8.o"(.text)
        "*/games/tla/src/field/map/cellfill.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/s0c44.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/s0ec8.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/s1178.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/s1600.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/s18f4.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/s1c30.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/s1dc8.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/s2010.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/import.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8/data.o"(.data)
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
        "*/games/tla/src/scene/shared/scene_set_arrival_flags.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_9/lightflag.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_9/s0538.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_9/s0910.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_9/s1018.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_9/s130c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_9/s1648.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_9/s17e0.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_9/s1a28.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_9/import.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_9/data.o"(.data)
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
        "*/games/tla/src/scene/shared/scene_set_arrival_flags.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/lighton.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/s309c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/s3474.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/s3b7c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/s3e70.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/s41ac.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/s4344.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/s458c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/import.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m/data.o"(.data)
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
        "*/games/tla/src/scene/shared/scene_set_arrival_flags.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/lighton.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/s0930.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/s0d08.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/s0de0.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/s17a8.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/s1e78.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/s216c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/s23b0.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/s24a8.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/s2640.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/import.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie/data.o"(.data)
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
        "*/games/tla/src/field/object/park_record.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s0090.o"(.text)
        "*/games/tla/src/field/object/step_angle.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s0198.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s1388.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s16e8.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s3a28.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/prepare.o"(.text)
        "*/games/tla/src/scene/shared/scene_set_arrival_flags.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s3f54.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s3f60.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s40f4.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s44cc.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s4bd4.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s4ec8.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s539c.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s55e4.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/import.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/data.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/data2.o"(.data)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon/data3.o"(.data)
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
        "*/games/tla/src/scene/northern_reaches/data.o"(.data)
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
        "*/games/tla/src/scene/venus_lighthouse/distance.o"(.text)
        "*/games/tla/src/scene/prox.o"(.text)
        "*/games/tla/src/scene/prox/s19a4.o"(.text)
        "*/games/tla/src/scene/prox/s1c24.o"(.text)
        "*/games/tla/src/scene/prox/s1cf0.o"(.text)
        "*/games/tla/src/scene/prox/s29a4.o"(.text)
        "*/games/tla/src/scene/prox/s2a08.o"(.text)
        "*/games/tla/src/scene/prox/s385c.o"(.text)
        "*/games/tla/src/scene/prox/s391c.o"(.text)
        "*/games/tla/src/scene/prox/s3edc.o"(.text)
        "*/games/tla/src/scene/prox/s4254.o"(.text)
        "*/games/tla/src/scene/prox/import.o"(.text)
        "*/games/tla/src/scene/prox/data.o"(.data)
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
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/object/prepare_object.o"(.text)
        "*/games/tla/src/scene/caverns/object.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/caverns/s015c.o"(.text)
        "*/games/tla/src/scene/caverns/scene_tables.o"(.text)
        "*/games/tla/src/scene/caverns/placements.o"(.text)
        "*/games/tla/src/scene/caverns/exits.o"(.text)
        "*/games/tla/src/scene/caverns/map_cell.o"(.text)
        "*/games/tla/src/field/object/wait_until_idle.o"(.text)
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
        "*/games/tla/src/scene/mars_lighthouse_entrance/scene_tables.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s0070.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/map.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s01f8.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s02bc.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s0338.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s040c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s0414.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s051c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s0564.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s0590.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s0710.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s0718.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s0930.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s093c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s09c0.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s0a74.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s0a7c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s0c68.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s0c84.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s0d18.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s0df0.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s0e3c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s0fa4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s0fd4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s0fe0.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s112c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s1138.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/dismiss.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s1254.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s12bc.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s12e4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s1344.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s1378.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s138c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s1930.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s1ae0.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s1c44.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s1cbc.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s1e60.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s20ec.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s2270.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/ride.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s22e4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/ride_row.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s24c0.o"(.text)
        "*/games/tla/src/field/field_effect/advance_spin.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/flash.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s263c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/ride_step.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s2860.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/ride_active.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s29c8.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s2b4c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/flag_state.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s2c40.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s2f4c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s2fc4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s308c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/reset.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s31f8.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s3284.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s32b8.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s3564.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s3878.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s3884.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority_b.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s3b30.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s3cf8.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s3e24.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s4068.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s4160.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s42f8.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s4490.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s4540.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s45f8.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s46b4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/stop.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s4738.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s4794.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s47b8.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s48e8.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s4970.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s49dc.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/s4a60.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/import.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_entrance/data.o"(.data)
        "*/games/tla/src/scene/mars_lighthouse_entrance/data2.o"(.data)
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
        "*/games/tla/src/scene/mars_lighthouse_star_room/hide.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/scene_tables.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/mark.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s0080.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s0188.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s024c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/scene_events.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/dismiss.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s02f4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s055c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s05b0.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s0714.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s0ab4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s0ae4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/talk.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s0bd8.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/tiles.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s0db8.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/note.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s0e80.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s13e4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s1424.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/paths.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s1528.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s15ec.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/spawn.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s17c8.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/spawn2.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s19b4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s1a9c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/spawn3.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s1b78.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s1bf8.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s1d7c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/ride_handle.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s1df0.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/ride.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s1fcc.o"(.text)
        "*/games/tla/src/field/field_effect/advance_spin.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/flash.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s2148.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/ride_step.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s236c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/ride_active.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s24d4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s2658.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/flag_state.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s274c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s2a58.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s2ad0.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s2b98.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/reset.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s2d04.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s2dc4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s3070.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/set_data.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s3390.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s363c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s3804.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s3930.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s3b74.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s3c6c.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s3e04.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s3f9c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/s404c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/import.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_star_room/data.o"(.data)
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
        "*/games/tla/src/scene/mars_lighthouse_room_m/scene_tables.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/hide.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s0080.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s00c0.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/start.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s030c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s03c4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s055c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s06c8.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s0934.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/finish.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s0af4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s0b64.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s0e84.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s10e4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s154c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s1608.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/stop.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s168c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s16e8.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s170c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s183c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s18c4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s1988.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s1aec.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s1b64.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s1d08.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s1e38.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s1ed4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s1f14.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s1f94.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s2118.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/ride_handle.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s218c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/ride.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s2368.o"(.text)
        "*/games/tla/src/field/field_effect/advance_spin.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/flash.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s24e4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/ride_step.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s2708.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/ride_active.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s2870.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s29f4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/flag_state.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s2ae8.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s2df4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s2e6c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/s2f34.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/reset.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/import.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_m/data.o"(.data)
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
        "*/games/tla/src/scene/mars_lighthouse_room_f/scene_tables.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s004c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s0170.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s01f4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s03b4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s055c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s0670.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s09ec.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s0cf4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s0e7c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s140c.o"(.text)
        "*/games/tla/src/scene/shared/scene_set_arrival_flags.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s14dc.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s14e8.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/lightset.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s1628.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s167c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s1868.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s1894.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s1a54.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s1b2c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s1bcc.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s1d5c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s1e20.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s1f84.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s1ffc.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s21a0.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s242c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s25b0.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/ride_handle.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s2624.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/ride.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s2800.o"(.text)
        "*/games/tla/src/field/field_effect/advance_spin.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/flash.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s297c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/ride_step.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s2ba0.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/ride_active.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s2d08.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s2e8c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/flag_state.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s2f80.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s328c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s3304.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s33cc.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/reset.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s35b8.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s3780.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s38ac.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s3af0.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s3be8.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s3d80.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s3f18.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s3fc8.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s4080.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s410c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s4140.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s43ec.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s4700.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s470c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s4938.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s4a44.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s4a8c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s4bf4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s4d10.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s4fc0.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s5048.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/light.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/s538c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/lightcue.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/import.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f/data.o"(.data)
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
        "*/games/tla/src/scene/mars_lighthouse_aerie/update_a.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/scene_tables.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/events_a.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/scene_events.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/s01d8.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/s0314.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/s0450.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/s04a4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/arc.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/events_b.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/s065c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/blink.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/s1f10.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/events_c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/orbit.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/s326c.o"(.text)
        "*/games/tla/src/field/object/park_record.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/s3370.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/events_d.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/events_e.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/s3658.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/s3ff4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/s4bc4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/events_f.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/s4d50.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/events_g.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/s4e80.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/events_h.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/s4ef0.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/stop.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/s508c.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/s50b0.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/s51e0.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/s5268.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/s53e4.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/mark.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/import.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_aerie/data.o"(.data)
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
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/object/prepare_object.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s00d0.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s015c.o"(.text)
        "*/games/tla/src/scene/treasure_isle/scene_tables.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s0350.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s03b4.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s03fc.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s043c.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s05ac.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s0864.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s0870.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s0880.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s0914.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s0940.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s0970.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s09a0.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s09d0.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s09fc.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s0a2c.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s0a5c.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s0a8c.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s0b3c.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s0ba0.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s0cf4.o"(.text)
        "*/games/tla/src/scene/treasure_isle/prepare.o"(.text)
        "*/games/tla/src/scene/treasure_isle/s0f4c.o"(.text)
        "*/games/tla/src/scene/treasure_isle/import.o"(.text)
        "*/games/tla/src/scene/treasure_isle/data.o"(.data)
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
        "*/games/tla/src/field/object/set_sprite_priority.o"(.text)
        "*/games/tla/src/field/object/prepare_object.o"(.text)
        "*/games/tla/src/field/field_effect/move.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s0480.o"(.text)
        "*/games/tla/src/field/object/wait_until_idle.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/scene_tables.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s0818.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s088c.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s09b8.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s0a78.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s0ac8.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s1090.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s109c.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s1160.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s1398.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s1518.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s1614.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s1644.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s1660.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s16fc.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s178c.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s17a0.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s17b4.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s1938.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s19a8.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s1a94.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s1b88.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s1bec.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s1c60.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s1c94.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s1d68.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s1e64.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/prepare.o"(.text)
        "*/games/tla/src/field/object/set_sprite_priority_b.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/move.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s2780.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s2948.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s2a74.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s2cb8.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s2db0.o"(.text)
        "*/games/tla/src/field/field_effect/advance_motion.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s2f48.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s30e0.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/s3190.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/import.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum/data.o"(.data)
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
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/scene_tables.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/events_a.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/reset.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/events_b.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/s0588.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/events_c.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/prepare.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/s0858.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/light.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/list.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/import.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance/data.o"(.data)
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
        "*/games/tla/src/scene/lobby/ime_call.o"(.text)
        "*/games/tla/src/scene/lobby/layout.o"(.text)
        "*/games/tla/src/scene/lobby/peer.o"(.text)
        "*/games/tla/src/scene/lobby/lobby.o"(.text)
        "*/games/tla/src/scene/lobby/circle.o"(.text)
        "*/games/tla/src/scene/lobby/s039c.o"(.text)
        "*/games/tla/src/scene/lobby/owners.o"(.text)
        "*/games/tla/src/scene/lobby/s0578.o"(.text)
        "*/games/tla/src/scene/lobby/exchange.o"(.text)
        "*/games/tla/src/scene/lobby/s0864.o"(.text)
        "*/games/tla/src/scene/lobby/counter.o"(.text)
        "*/games/tla/src/scene/lobby/s0da0.o"(.text)
        "*/games/tla/src/scene/lobby/prompt.o"(.text)
        "*/games/tla/src/scene/lobby/sound_call.o"(.text)
        "*/games/tla/src/scene/lobby/s0fbc.o"(.text)
        "*/games/tla/src/scene/lobby/talk.o"(.text)
        "*/games/tla/src/scene/lobby/scene_events.o"(.text)
        "*/games/tla/src/scene/lobby/results.o"(.text)
        "*/games/tla/src/scene/lobby/s145c.o"(.text)
        "*/games/tla/src/scene/lobby/prepare.o"(.text)
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
        "*/games/tla/src/debug/level/finish.o"(.text)
        "*/games/tla/src/debug/level/s0590.o"(.text)
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
        "*/games/tla/src/debug/psynergy_test_2/s01e0.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2/s029c.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2/s0358.o"(.text)
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
