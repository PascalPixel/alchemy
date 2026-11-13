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
   section it places, so the objects keep their order. Where its globals are
   written in assembly, its .bss is DIR/bss/NAME.s. */

/* Most scenes are one overlay of one shape: the entry table, the scene's
   code, its import table, then the scene's tables; SCENE_WITH_BSS adds the
   globals the scene's bss/NAME.s defines. A scene of another shape writes
   its block out. */
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
        */games/tla/src/scene/bss/name.o(.bss) \
    } \
}

#if OVERLAY == 0x649
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/title.o"(.text)
        "*/games/tla/src/scene/title.o"(.text)
        "*/imports.o"(.text)
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
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/clear.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/clear.o"(.data)
        "*/games/tla/src/graphics/clear/tilemap.o"(.rodata)
        "*/games/tla/src/scene/clear.o"(.data.late)
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
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/world_map.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/world_map.o"(.data)
        "*/games/tla/src/graphics/world_map/story.o"(.rodata)
        "*/games/tla/src/graphics/world_map/storyb.o"(.rodata)
        "*/games/tla/src/graphics/world_map/storyc.o"(.rodata)
        "*/games/tla/src/scene/world_map.o"(.data.late)
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
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/staff_credit.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/graphics/staff_credit/graphics.o"(.rodata)
        "*/games/tla/src/scene/staff_credit.o"(.data)
        "*/games/tla/src/graphics/world_map/story.o"(.rodata)
        "*/games/tla/src/graphics/world_map/storyc.o"(.rodata)
        "*/games/tla/src/scene/staff_credit.o"(.data.late)
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
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance.o"(.text)
        "*/imports.o"(.text)
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
        "*/games/tla/src/scene/venus_lighthouse.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse.o"(.data)
    }
}
#elif OVERLAY == 0x64f
SCENE(suhalla_gate)
#elif OVERLAY == 0x650
SCENE_WITH_BSS(idejima)
#elif OVERLAY == 0x651
SCENE(daila)
#elif OVERLAY == 0x652
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/daila_interiors.o"(.text)
        "*/games/tla/src/scene/daila_interiors.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/daila_interiors.o"(.data)
    }
}
#elif OVERLAY == 0x653
SCENE_WITH_BSS(kandorean_temple)
#elif OVERLAY == 0x654
SCENE_WITH_BSS(shrine_of_the_sea_god)
#elif OVERLAY == 0x655
SCENE_WITH_BSS(dehkan_plateau)
#elif OVERLAY == 0x656
SCENE(lemurian_ship)
#elif OVERLAY == 0x657
SCENE_WITH_BSS(lemurian_ship_deck)
#elif OVERLAY == 0x658
SCENE(madra)
#elif OVERLAY == 0x659
SCENE(madra_interiors)
#elif OVERLAY == 0x65a
SCENE(madra_catacombs)
#elif OVERLAY == 0x65b
SCENE(madra_drawbridge)
#elif OVERLAY == 0x65c
SCENE(osenia_cliffs)
#elif OVERLAY == 0x65d
SCENE(mikasalla)
#elif OVERLAY == 0x65e
SCENE(mikasalla_interiors)
#elif OVERLAY == 0x65f
SCENE_WITH_BSS(garoh)
#elif OVERLAY == 0x660
SCENE(garoh_interiors)
#elif OVERLAY == 0x661
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/yampi_desert.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/yampi_desert.o"(.data)
        "*/games/tla/src/graphics/field/tiles64d.o"(.rodata)
        "*/games/tla/src/scene/yampi_desert.o"(.data.late)
    }
}
#elif OVERLAY == 0x662
SCENE_WITH_BSS(yampi_desert_cave)
#elif OVERLAY == 0x663
SCENE_WITH_BSS(airs_rock)
#elif OVERLAY == 0x664
SCENE_WITH_BSS(airs_rock_inside)
#elif OVERLAY == 0x665
SCENE(alhafra)
#elif OVERLAY == 0x666
SCENE_WITH_BSS(alhafra_interiors)
#elif OVERLAY == 0x667
SCENE(alhafra_manor)
#elif OVERLAY == 0x668
SCENE_WITH_BSS(eastern_alhafra)
#elif OVERLAY == 0x669
SCENE(eastern_alhafra_ship)
#elif OVERLAY == 0x66a
SCENE_WITH_BSS(alhafran_cave)
#elif OVERLAY == 0x66b
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/gondowan_cliffs.o"(.data)
        "*/games/tla/src/graphics/field/tiles64d.o"(.rodata)
        "*/games/tla/src/scene/gondowan_cliffs.o"(.data.late)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/gondowan_cliffs.o"(.bss)
    }
}
#elif OVERLAY == 0x66c
SCENE(gondowan_cliffs_hideout)
#elif OVERLAY == 0x66d
SCENE(naribwe)
#elif OVERLAY == 0x66e
SCENE(naribwe_interiors)
#elif OVERLAY == 0x66f
SCENE(kibombo_mountains)
#elif OVERLAY == 0x670
SCENE_WITH_BSS(kibombo)
#elif OVERLAY == 0x671
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/gabomba_statue.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/gabomba_statue.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/scene/gabomba_statue.o"(.data.late)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/gabomba_statue.o"(.bss)
    }
}
#elif OVERLAY == 0x672
SCENE(gabomba_statue_chamber)
#elif OVERLAY == 0x673
SCENE(gabomba_catacombs)
#elif OVERLAY == 0x674
SCENE_WITH_BSS(islets)
#elif OVERLAY == 0x675
SCENE_WITH_BSS(islet_cave)
#elif OVERLAY == 0x676
SCENE_WITH_BSS(yallam)
#elif OVERLAY == 0x677
SCENE(yallam_interiors)
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
        "*/games/tla/src/graphics/field/tiles679.o"(.rodata)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/apojii_islands.o"(.bss)
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
        "*/games/tla/src/graphics/aqua_rock/tiles.o"(.rodata)
        "*/games/tla/src/scene/aqua_rock.o"(.data.late)
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
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/aqua_rock_inside.o"(.data)
        "*/games/tla/src/graphics/aqua_rock_inside/tiles.o"(.rodata)
        "*/games/tla/src/scene/aqua_rock_inside.o"(.data.late)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/aqua_rock_inside.o"(.bss)
    }
}
#elif OVERLAY == 0x67d
SCENE(izumo_interiors)
#elif OVERLAY == 0x67e
SCENE(izumo)
#elif OVERLAY == 0x67f
SCENE(izumo_ruins)
#elif OVERLAY == 0x680
SCENE(gaia_rock)
#elif OVERLAY == 0x681
SCENE_WITH_BSS(gaia_rock_inside)
#elif OVERLAY == 0x682
SCENE(champa)
#elif OVERLAY == 0x683
SCENE(champa_interiors)
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
        "*/games/tla/src/graphics/field/pal685.o"(.rodata)
        "*/games/tla/src/scene/champa_cliff_caves.o"(.data.late)
    }
}
#elif OVERLAY == 0x686
SCENE_WITH_BSS(ankohl_ruins)
#elif OVERLAY == 0x687
SCENE_WITH_BSS(sea_of_time)
#elif OVERLAY == 0x688
SCENE(lemuria_entrance)
#elif OVERLAY == 0x689
SCENE(lemuria_dock)
#elif OVERLAY == 0x68a
SCENE(ancient_lemuria)
#elif OVERLAY == 0x68b
SCENE_WITH_BSS(lemuria)
#elif OVERLAY == 0x68c
SCENE(lemuria_interiors)
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
        "*/games/tla/src/scene/bss/lemuria_palace.o"(.bss)
    }
}
#elif OVERLAY == 0x68e
SCENE_WITH_BSS(lemuria_spring)
#elif OVERLAY == 0x68f
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry/tundaria_tower.o"(.text)
        "*/games/tla/src/scene/tundaria_tower.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/tundaria_tower.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/tundaria_tower.o"(.bss)
    }
}
#elif OVERLAY == 0x690
SCENE_WITH_BSS(settlements)
#elif OVERLAY == 0x691
SCENE(magma_rock)
#elif OVERLAY == 0x692
SCENE_WITH_BSS(magma_rock_inside)
#elif OVERLAY == 0x693
SCENE(atteka_inlet)
#elif OVERLAY == 0x694
SCENE_WITH_BSS(contigo)
#elif OVERLAY == 0x695
SCENE(shaman_village_cave)
#elif OVERLAY == 0x696
SCENE_WITH_BSS(shaman_village)
#elif OVERLAY == 0x697
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/trial_road.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/trial_road.o"(.data)
        "*/games/tla/src/graphics/field/tiles64d.o"(.rodata)
        "*/games/tla/src/scene/trial_road.o"(.data.late)
        "*/games/tla/src/graphics/trial_road/picture.o"(.rodata)
        "*/games/tla/src/scene/trial_road.o"(.data.last)
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
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_entrance.o"(.text)
        "*/imports.o"(.text)
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
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_basement.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_basement.o"(.data)
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
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_1.o"(.text)
        "*/imports.o"(.text)
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
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_2.o"(.data)
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
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_3.o"(.data.late)
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
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_4.o"(.text)
        "*/imports.o"(.text)
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
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_5.o"(.data)
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
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_6.o"(.text)
        "*/imports.o"(.text)
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
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_7.o"(.data.late)
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
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_8.o"(.data.late)
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
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_9.o"(.text)
        "*/imports.o"(.text)
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
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_room_m.o"(.data.late)
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
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie.o"(.data)
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
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/jupiter_lighthouse_aerie_beacon.o"(.data.late)
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
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/loho.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/graphics/field/pal685.o"(.rodata)
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
        "*/games/tla/src/graphics/field/pal685.o"(.rodata)
        "*/games/tla/src/scene/northern_reaches.o"(.data)
    }
}
#elif OVERLAY == 0x6a8
SCENE_WITH_BSS(prox)
#elif OVERLAY == 0x6a9
SCENE(caverns)
#elif OVERLAY == 0x6aa
SCENE_WITH_BSS(mars_lighthouse_entrance)
#elif OVERLAY == 0x6ab
SCENE_WITH_BSS(mars_lighthouse_star_room)
#elif OVERLAY == 0x6ac
SCENE_WITH_BSS(mars_lighthouse_room_m)
#elif OVERLAY == 0x6ad
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/mars_lighthouse_room_f.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_b.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_c.o"(.rodata)
        "*/games/tla/src/scene/mars_lighthouse_room_f.o"(.data.late)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/mars_lighthouse_room_f.o"(.bss)
    }
}
#elif OVERLAY == 0x6ae
SCENE_WITH_BSS(mars_lighthouse_aerie)
#elif OVERLAY == 0x6af
SCENE_WITH_BSS(treasure_isle)
#elif OVERLAY == 0x6b0
SCENE_WITH_BSS(anemos_inner_sanctum)
#elif OVERLAY == 0x6b1
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance.o"(.data)
        "*/games/tla/src/graphics/field/cached_a.o"(.rodata)
        "*/games/tla/src/graphics/field/cached_d.o"(.rodata)
        "*/games/tla/src/scene/anemos_inner_sanctum_entrance.o"(.data.late)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/anemos_inner_sanctum_entrance.o"(.bss)
    }
}
#elif OVERLAY == 0x6b2
SCENE_WITH_BSS(mercury_lighthouse)
#elif OVERLAY == 0x6b3
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/entry.o"(.text)
        "*/games/tla/src/scene/epilogue.o"(.text)
        "*/imports.o"(.text)
        "*/games/tla/src/graphics/epilogue/pal.o"(.rodata)
        "*/games/tla/src/scene/epilogue.o"(.data)
    }
    .bss :
    {
        "*/games/tla/src/scene/bss/epilogue.o"(.bss)
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
        "*/imports.o"(.text)
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
        "*/imports.o"(.text)
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
        "*/imports.o"(.text)
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
        "*/imports.o"(.text)
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
        "*/imports.o"(.text)
        "*/games/tla/src/debug/shop_test.o"(.data)
    }
}
#endif

SECTIONS
{
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
