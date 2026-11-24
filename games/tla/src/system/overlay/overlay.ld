/* One overlay of The Lost Age, loaded at 0x2008000: ld_script.ld includes
   this file when the build defines OVERLAY as the overlay's number, and the
   block for that number is the overlay. The build finds the overlays from the
   lines that test OVERLAY, and each overlay's objects from its statements.

   A scene's .text opens with its entry table, src/scene/entry.s, which every
   scene links (each resolves its names against its own code), or
   DIR/entry/NAME.s for the few whose routines are named otherwise, where DIR
   is src/scene (or src/debug). Its import table, imports.o, follows its
   code: the build writes it for each overlay from imports.inc, a stub for
   each main-image routine the overlay's code calls. Each statement names the
   section it places, so the objects keep their order. Its globals follow in
   .bss, in the order its C first declares them (BSS_DATA keeps a global out
   of the commons). */

/* Most scenes are one overlay of one shape: the entry table, the scene's
   code, its import table, then the scene's tables, and its globals;
   SCENE_WITH_BSS adds those written in assembly, in src/scene/NAME_bss.s. A
   scene of another shape writes its block out. */
#define SCENE(name) \
SECTIONS \
{ \
    .text 0x2008000 : \
    { \
        */games/tla/src/scene/entry.o(.text) \
        */games/tla/src/scene/name.o(.text) \
        */imports.o(.text) \
        */games/tla/src/scene/name.o(.data) \
    } \
    .bss : \
    { \
        */games/tla/src/scene/name.o(.bss COMMON) \
    } \
}
#define SCENE_WITH_BSS(name) \
SECTIONS \
{ \
    .text 0x2008000 : \
    { \
        */games/tla/src/scene/entry.o(.text) \
        */games/tla/src/scene/name.o(.text) \
        */imports.o(.text) \
        */games/tla/src/scene/name.o(.data) \
    } \
    .bss : \
    { \
        */games/tla/src/scene/name.o(.bss COMMON) \
        */games/tla/src/scene/name##_bss.o(.bss) \
    } \
}

#if OVERLAY == 0x649
SCENE_WITH_BSS(title)
#elif OVERLAY == 0x64a
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/clear.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/clear.o"(.data)
        "*/games/tla/src/graphics/clear/tilemap.o"(.rodata)
        "*/games/tla/src/scene/clear.o"(.data.late)
    }
    .bss :
    {
        "*/games/tla/src/scene/clear.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x64b
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/world_map.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/world_map.o"(.data)
        "*/games/tla/src/graphics/world_map/story.list.o"(.rodata)
        "*/games/tla/src/graphics/world_map/storyb.list.o"(.rodata)
        "*/games/tla/src/graphics/world_map/storyc.list.o"(.rodata)
        "*/games/tla/src/scene/world_map.o"(.data.late)
    }
    .bss :
    {
        "*/games/tla/src/scene/world_map.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x64c
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/staff_credit.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/graphics/staff_credit/graphics.list.o"(.rodata)
        "*/games/tla/src/scene/staff_credit.o"(.data)
        "*/games/tla/src/graphics/world_map/story.list.o"(.rodata)
        "*/games/tla/src/graphics/world_map/storyc.list.o"(.rodata)
        "*/games/tla/src/scene/staff_credit.o"(.data.late)
    }
    .bss :
    {
        "*/games/tla/src/scene/staff_credit.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x64d
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/graphics/field/geyser.list.o"(.rodata)
        "*/games/tla/src/scene/venus_lighthouse_entrance.o"(.data)
    }
}
#elif OVERLAY == 0x64e
SCENE(venus_lighthouse)
#elif OVERLAY == 0x64f
SCENE(suhalla_gate)
#elif OVERLAY == 0x650
SCENE(idejima)
#elif OVERLAY == 0x651
SCENE(daila)
#elif OVERLAY == 0x652
SCENE(daila_rooms)
#elif OVERLAY == 0x653
SCENE(kandorean_temple)
#elif OVERLAY == 0x654
SCENE(shrine_of_the_sea_god)
#elif OVERLAY == 0x655
SCENE(dehkan_plateau)
#elif OVERLAY == 0x656
SCENE(lemurian_ship)
#elif OVERLAY == 0x657
SCENE(lemurian_ship_deck)
#elif OVERLAY == 0x658
SCENE(madra)
#elif OVERLAY == 0x659
SCENE(madra_rooms)
#elif OVERLAY == 0x65a
SCENE(madra_catacombs)
#elif OVERLAY == 0x65b
SCENE(madra_drawbridge)
#elif OVERLAY == 0x65c
SCENE(osenia_cliffs)
#elif OVERLAY == 0x65d
SCENE(mikasalla)
#elif OVERLAY == 0x65e
SCENE(mikasalla_rooms)
#elif OVERLAY == 0x65f
SCENE(garoh)
#elif OVERLAY == 0x660
SCENE(garoh_rooms)
#elif OVERLAY == 0x661
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/yampi_desert.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/yampi_desert.o"(.data)
        "*/games/tla/src/graphics/field/geyser.list.o"(.rodata)
        "*/games/tla/src/scene/yampi_desert.o"(.data.late)
    }
}
#elif OVERLAY == 0x662
SCENE(yampi_desert_cave)
#elif OVERLAY == 0x663
SCENE(airs_rock)
#elif OVERLAY == 0x664
SCENE(airs_rock_inside)
#elif OVERLAY == 0x665
SCENE(alhafra)
#elif OVERLAY == 0x666
SCENE(alhafra_rooms)
#elif OVERLAY == 0x667
SCENE(alhafra_manor)
#elif OVERLAY == 0x668
SCENE(eastern_alhafra)
#elif OVERLAY == 0x669
SCENE(eastern_alhafra_ship)
#elif OVERLAY == 0x66a
SCENE(alhafran_cave)
#elif OVERLAY == 0x66b
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs.o"(.data)
        "*/games/tla/src/graphics/field/geyser.list.o"(.rodata)
        "*/games/tla/src/scene/gondowan_cliffs.o"(.data.late)
    }
    .bss :
    {
        "*/games/tla/src/scene/gondowan_cliffs.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x66c
SCENE(gondowan_cliffs_hideout)
#elif OVERLAY == 0x66d
SCENE(naribwe)
#elif OVERLAY == 0x66e
SCENE(naribwe_rooms)
#elif OVERLAY == 0x66f
SCENE(kibombo_mountains)
#elif OVERLAY == 0x670
SCENE(kibombo)
#elif OVERLAY == 0x671
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/gabomba_statue.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/gabomba_statue.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.list.o"(.rodata)
        "*/games/tla/src/scene/gabomba_statue.o"(.data.late)
    }
    .bss :
    {
        "*/games/tla/src/scene/gabomba_statue.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x672
SCENE(gabomba_statue_chamber)
#elif OVERLAY == 0x673
SCENE(gabomba_catacombs)
#elif OVERLAY == 0x674
SCENE(islets)
#elif OVERLAY == 0x675
SCENE(islet_cave)
#elif OVERLAY == 0x676
SCENE(yallam)
#elif OVERLAY == 0x677
SCENE(yallam_rooms)
#elif OVERLAY == 0x678
SCENE(taopo_swamp)
#elif OVERLAY == 0x679
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/apojii_islands.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/apojii_islands.o"(.data)
        "*/games/tla/src/graphics/field/ground_tiles.list.o"(.rodata)
    }
    .bss :
    {
        "*/games/tla/src/scene/apojii_islands.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x67a
SCENE(apojii_islands_cave)
#elif OVERLAY == 0x67b
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/aqua_rock.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/aqua_rock.o"(.data)
        "*/games/tla/src/graphics/aqua_rock/tiles.list.o"(.rodata)
        "*/games/tla/src/scene/aqua_rock.o"(.data.late)
        "*/games/tla/src/graphics/field/ground_tiles.list.o"(.rodata)
    }
    .bss :
    {
        "*/games/tla/src/scene/aqua_rock.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x67c
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside.o"(.data)
        "*/games/tla/src/graphics/aqua_rock_inside/tiles.list.o"(.rodata)
        "*/games/tla/src/scene/aqua_rock_inside.o"(.data.late)
    }
    .bss :
    {
        "*/games/tla/src/scene/aqua_rock_inside.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x67d
SCENE(izumo_rooms)
#elif OVERLAY == 0x67e
SCENE(izumo)
#elif OVERLAY == 0x67f
SCENE(izumo_ruins)
#elif OVERLAY == 0x680
SCENE(gaia_rock)
#elif OVERLAY == 0x681
SCENE(gaia_rock_inside)
#elif OVERLAY == 0x682
SCENE(champa)
#elif OVERLAY == 0x683
SCENE(champa_rooms)
#elif OVERLAY == 0x684
SCENE(champa_sanctum)
#elif OVERLAY == 0x685
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/champa_cliff_caves.o"(.data)
        "*/games/tla/src/graphics/field/explosion_palette.list.o"(.rodata)
        "*/games/tla/src/scene/champa_cliff_caves.o"(.data.late)
    }
}
#elif OVERLAY == 0x686
SCENE(ankohl_ruins)
#elif OVERLAY == 0x687
SCENE(sea_of_time)
#elif OVERLAY == 0x688
SCENE(lemuria_entrance)
#elif OVERLAY == 0x689
SCENE(lemuria_dock)
#elif OVERLAY == 0x68a
SCENE(ancient_lemuria)
#elif OVERLAY == 0x68b
SCENE(lemuria)
#elif OVERLAY == 0x68c
SCENE(lemuria_rooms)
#elif OVERLAY == 0x68d
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/lemuria_palace.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/lemuria_palace.o"(.data)
        "*/games/tla/src/graphics/lemuria_palace/tiles.o"(.rodata)
        "*/games/tla/src/scene/lemuria_palace.o"(.data.late)
    }
    .bss :
    {
        "*/games/tla/src/scene/lemuria_palace.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x68e
SCENE(lemuria_spring)
#elif OVERLAY == 0x68f
SCENE(tundaria_tower)
#elif OVERLAY == 0x690
SCENE(settlements)
#elif OVERLAY == 0x691
SCENE(magma_rock)
#elif OVERLAY == 0x692
SCENE(magma_rock_inside)
#elif OVERLAY == 0x693
SCENE(atteka_inlet)
#elif OVERLAY == 0x694
SCENE(contigo)
#elif OVERLAY == 0x695
SCENE(shaman_village_cave)
#elif OVERLAY == 0x696
SCENE(shaman_village)
#elif OVERLAY == 0x697
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/trial_road.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/trial_road.o"(.data)
        "*/games/tla/src/graphics/field/geyser.list.o"(.rodata)
        "*/games/tla/src/scene/trial_road.o"(.data.late)
        "*/games/tla/src/graphics/trial_road/picture.list.o"(.rodata)
        "*/games/tla/src/scene/trial_road.o"(.data.last)
    }
    .bss :
    {
        "*/games/tla/src/scene/trial_road.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x698
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.list.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_entrance.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x699
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.list.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_basement.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_basement.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x69a
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_gate_button.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.list.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_gate_button.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_gate_button.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x69b
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_beam.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.list.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_beam.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_beam.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x69c
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_erinyes_tunic_chest.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_erinyes_tunic_chest.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.list.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_erinyes_tunic_chest.o"(.data.late)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_erinyes_tunic_chest.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x69d
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_locked_doors.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.list.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_locked_doors.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_locked_doors.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x69e
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_red_key.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.list.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_red_key.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_red_key.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x69f
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_cracked_floor.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.list.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_cracked_floor.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_cracked_floor.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x6a0
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_whorl.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_whorl.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.list.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_whorl.o"(.data.late)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_whorl.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x6a1
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_blue_key.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_blue_key.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.list.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_blue_key.o"(.data.late)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_blue_key.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x6a2
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_phaetons_blade_chest.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.list.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_phaetons_blade_chest.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_room_phaetons_blade_chest.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x6a3
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_terraces.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_terraces.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.list.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_terraces.o"(.data.late)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_terraces.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x6a4
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.list.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_aerie.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x6a5
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.list.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon.o"(.data.late)
    }
    .bss :
    {
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x6a6
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/loho.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/graphics/field/explosion_palette.list.o"(.rodata)
        "*/games/tla/src/scene/loho.o"(.data)
    }
}
#elif OVERLAY == 0x6a7
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/northern_reaches.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/graphics/field/explosion_palette.list.o"(.rodata)
        "*/games/tla/src/scene/northern_reaches.o"(.data)
    }
}
#elif OVERLAY == 0x6a8
SCENE(prox)
#elif OVERLAY == 0x6a9
SCENE(caverns)
#elif OVERLAY == 0x6aa
SCENE(mars_lighthouse_entrance)
#elif OVERLAY == 0x6ab
SCENE(mars_lighthouse_star_room)
#elif OVERLAY == 0x6ac
SCENE(mars_lighthouse_hall)
#elif OVERLAY == 0x6ad
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.list.o"(.rodata)
        "*/games/tla/src/scene/mars_lighthouse_room_f.o"(.data.late)
    }
    .bss :
    {
        "*/games/tla/src/scene/mars_lighthouse_room_f.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x6ae
SCENE(mars_lighthouse_aerie)
#elif OVERLAY == 0x6af
SCENE(treasure_isle)
#elif OVERLAY == 0x6b0
SCENE(anemos_inner_sanctum)
#elif OVERLAY == 0x6b1
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.list.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_d.list.o"(.rodata)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance.o"(.data.late)
    }
    .bss :
    {
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x6b2
SCENE(mercury_lighthouse)
#elif OVERLAY == 0x6b3
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/epilogue.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/graphics/epilogue/pal.list.o"(.rodata)
        "*/games/tla/src/scene/epilogue.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/epilogue.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x6b4
SCENE(mt_aleph_summit)
#elif OVERLAY == 0x6b5
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/lobby.o"(.text)
        "*/games/tla/src/scene/lobby.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/lobby.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/lobby.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x6b6
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/debug/test_scenes.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/debug/test_scenes.o"(.data)
    }
}
#elif OVERLAY == 0x6b7
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/debug/level.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/debug/level.o"(.data)
    }
}
#elif OVERLAY == 0x6b8
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/debug/psynergy_test.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/debug/psynergy_test.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/debug/psynergy_test.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x6b9
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2.o"(.data)
    }
}
#elif OVERLAY == 0x6ba
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/debug/shop_test.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/debug/shop_test.o"(.data)
    }
}
#endif

SECTIONS
{
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
