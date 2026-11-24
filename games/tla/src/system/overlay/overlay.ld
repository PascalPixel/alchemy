/* One overlay of The Lost Age, loaded at 0x2008000: ld_script.ld includes
   this file when the build defines OVERLAY and OVERLAY_NAME for the overlay
   of that name, and the block that tests the name is the overlay. Its number
   is its row in the resource directory (gamepak/resource_directory.s), which
   names it the same. The build finds the overlays from the lines that test a
   name, and each overlay's objects from its statements.

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

#if defined(OVERLAY_Title)
SCENE_WITH_BSS(title)
#elif defined(OVERLAY_Clear)
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
#elif defined(OVERLAY_WorldMap)
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
#elif defined(OVERLAY_StaffCredit)
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
#elif defined(OVERLAY_VenusLighthouseEntrance)
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
#elif defined(OVERLAY_VenusLighthouse)
SCENE(venus_lighthouse)
#elif defined(OVERLAY_SuhallaGate)
SCENE(suhalla_gate)
#elif defined(OVERLAY_Idejima)
SCENE(idejima)
#elif defined(OVERLAY_Daila)
SCENE(daila)
#elif defined(OVERLAY_DailaRooms)
SCENE(daila_rooms)
#elif defined(OVERLAY_KandoreanTemple)
SCENE(kandorean_temple)
#elif defined(OVERLAY_ShrineOfTheSeaGod)
SCENE(shrine_of_the_sea_god)
#elif defined(OVERLAY_DehkanPlateau)
SCENE(dehkan_plateau)
#elif defined(OVERLAY_LemurianShip)
SCENE(lemurian_ship)
#elif defined(OVERLAY_LemurianShipDeck)
SCENE(lemurian_ship_deck)
#elif defined(OVERLAY_Madra)
SCENE(madra)
#elif defined(OVERLAY_MadraRooms)
SCENE(madra_rooms)
#elif defined(OVERLAY_MadraCatacombs)
SCENE(madra_catacombs)
#elif defined(OVERLAY_MadraDrawbridge)
SCENE(madra_drawbridge)
#elif defined(OVERLAY_OseniaCliffs)
SCENE(osenia_cliffs)
#elif defined(OVERLAY_Mikasalla)
SCENE(mikasalla)
#elif defined(OVERLAY_MikasallaRooms)
SCENE(mikasalla_rooms)
#elif defined(OVERLAY_Garoh)
SCENE(garoh)
#elif defined(OVERLAY_GarohRooms)
SCENE(garoh_rooms)
#elif defined(OVERLAY_YampiDesert)
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
#elif defined(OVERLAY_YampiDesertCave)
SCENE(yampi_desert_cave)
#elif defined(OVERLAY_AirsRock)
SCENE(airs_rock)
#elif defined(OVERLAY_AirsRockInside)
SCENE(airs_rock_inside)
#elif defined(OVERLAY_Alhafra)
SCENE(alhafra)
#elif defined(OVERLAY_AlhafraRooms)
SCENE(alhafra_rooms)
#elif defined(OVERLAY_AlhafraManor)
SCENE(alhafra_manor)
#elif defined(OVERLAY_EasternAlhafra)
SCENE(eastern_alhafra)
#elif defined(OVERLAY_EasternAlhafraShip)
SCENE(eastern_alhafra_ship)
#elif defined(OVERLAY_AlhafranCave)
SCENE(alhafran_cave)
#elif defined(OVERLAY_GondowanCliffs)
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
#elif defined(OVERLAY_GondowanCliffsHideout)
SCENE(gondowan_cliffs_hideout)
#elif defined(OVERLAY_Naribwe)
SCENE(naribwe)
#elif defined(OVERLAY_NaribweRooms)
SCENE(naribwe_rooms)
#elif defined(OVERLAY_KibomboMountains)
SCENE(kibombo_mountains)
#elif defined(OVERLAY_Kibombo)
SCENE(kibombo)
#elif defined(OVERLAY_GabombaStatue)
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
#elif defined(OVERLAY_GabombaStatueChamber)
SCENE(gabomba_statue_chamber)
#elif defined(OVERLAY_GabombaCatacombs)
SCENE(gabomba_catacombs)
#elif defined(OVERLAY_Islets)
SCENE(islets)
#elif defined(OVERLAY_IsletCave)
SCENE(islet_cave)
#elif defined(OVERLAY_Yallam)
SCENE(yallam)
#elif defined(OVERLAY_YallamRooms)
SCENE(yallam_rooms)
#elif defined(OVERLAY_TaopoSwamp)
SCENE(taopo_swamp)
#elif defined(OVERLAY_ApojiiIslands)
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
#elif defined(OVERLAY_ApojiiIslandsCave)
SCENE(apojii_islands_cave)
#elif defined(OVERLAY_AquaRock)
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
#elif defined(OVERLAY_AquaRockInside)
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
#elif defined(OVERLAY_IzumoRooms)
SCENE(izumo_rooms)
#elif defined(OVERLAY_Izumo)
SCENE(izumo)
#elif defined(OVERLAY_IzumoRuins)
SCENE(izumo_ruins)
#elif defined(OVERLAY_GaiaRock)
SCENE(gaia_rock)
#elif defined(OVERLAY_GaiaRockInside)
SCENE(gaia_rock_inside)
#elif defined(OVERLAY_Champa)
SCENE(champa)
#elif defined(OVERLAY_ChampaRooms)
SCENE(champa_rooms)
#elif defined(OVERLAY_ChampaSanctum)
SCENE(champa_sanctum)
#elif defined(OVERLAY_ChampaCliffCaves)
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
#elif defined(OVERLAY_AnkohlRuins)
SCENE(ankohl_ruins)
#elif defined(OVERLAY_SeaOfTime)
SCENE(sea_of_time)
#elif defined(OVERLAY_LemuriaEntrance)
SCENE(lemuria_entrance)
#elif defined(OVERLAY_LemuriaDock)
SCENE(lemuria_dock)
#elif defined(OVERLAY_AncientLemuria)
SCENE(ancient_lemuria)
#elif defined(OVERLAY_Lemuria)
SCENE(lemuria)
#elif defined(OVERLAY_LemuriaRooms)
SCENE(lemuria_rooms)
#elif defined(OVERLAY_LemuriaPalace)
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
#elif defined(OVERLAY_LemuriaSpring)
SCENE(lemuria_spring)
#elif defined(OVERLAY_TundariaTower)
SCENE(tundaria_tower)
#elif defined(OVERLAY_Settlements)
SCENE(settlements)
#elif defined(OVERLAY_MagmaRock)
SCENE(magma_rock)
#elif defined(OVERLAY_MagmaRockInside)
SCENE(magma_rock_inside)
#elif defined(OVERLAY_AttekaInlet)
SCENE(atteka_inlet)
#elif defined(OVERLAY_Contigo)
SCENE(contigo)
#elif defined(OVERLAY_ShamanVillageCave)
SCENE(shaman_village_cave)
#elif defined(OVERLAY_ShamanVillage)
SCENE(shaman_village)
#elif defined(OVERLAY_TrialRoad)
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
#elif defined(OVERLAY_JupiterLighthouseEntrance)
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
#elif defined(OVERLAY_JupiterLighthouseBasement)
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
#elif defined(OVERLAY_JupiterLighthouseRoomGateButton)
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
#elif defined(OVERLAY_JupiterLighthouseRoomBeam)
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
#elif defined(OVERLAY_JupiterLighthouseRoomErinyesTunicChest)
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
#elif defined(OVERLAY_JupiterLighthouseRoomLockedDoors)
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
#elif defined(OVERLAY_JupiterLighthouseRoomRedKey)
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
#elif defined(OVERLAY_JupiterLighthouseRoomCrackedFloor)
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
#elif defined(OVERLAY_JupiterLighthouseWhorl)
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
#elif defined(OVERLAY_JupiterLighthouseRoomBlueKey)
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
#elif defined(OVERLAY_JupiterLighthouseRoomPhaetonsBladeChest)
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
#elif defined(OVERLAY_JupiterLighthouseTerraces)
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
#elif defined(OVERLAY_JupiterLighthouseAerie)
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
#elif defined(OVERLAY_JupiterLighthouseAerieBeacon)
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
#elif defined(OVERLAY_Loho)
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
#elif defined(OVERLAY_NorthernReaches)
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
#elif defined(OVERLAY_Prox)
SCENE(prox)
#elif defined(OVERLAY_Caverns)
SCENE(caverns)
#elif defined(OVERLAY_MarsLighthouseEntrance)
SCENE(mars_lighthouse_entrance)
#elif defined(OVERLAY_MarsLighthouseStarRoom)
SCENE(mars_lighthouse_star_room)
#elif defined(OVERLAY_MarsLighthouseHall)
SCENE(mars_lighthouse_hall)
#elif defined(OVERLAY_MarsLighthouseRoomF)
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
#elif defined(OVERLAY_MarsLighthouseAerie)
SCENE(mars_lighthouse_aerie)
#elif defined(OVERLAY_TreasureIsle)
SCENE(treasure_isle)
#elif defined(OVERLAY_AnemosInnerSanctum)
SCENE(anemos_inner_sanctum)
#elif defined(OVERLAY_AnemosInnerSanctumEntrance)
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
#elif defined(OVERLAY_MercuryLighthouse)
SCENE(mercury_lighthouse)
#elif defined(OVERLAY_Epilogue)
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
#elif defined(OVERLAY_MtAlephSummit)
SCENE(mt_aleph_summit)
#elif defined(OVERLAY_Lobby)
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
#elif defined(OVERLAY_TestScenes)
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
#elif defined(OVERLAY_IconTest)
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
#elif defined(OVERLAY_PsynergyTest)
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
#elif defined(OVERLAY_PsynergyTest2)
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
#elif defined(OVERLAY_ShopTest)
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
