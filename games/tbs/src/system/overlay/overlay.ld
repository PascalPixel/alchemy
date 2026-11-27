/* One overlay of The Broken Seal, loaded at 0x2008000: ld_script.ld includes
   this file when the build defines OVERLAY and OVERLAY_NAME for the overlay
   of that name, and the block that tests the name is the overlay. Its number
   is its row in the resource directory (system/resource_directory.s), which
   names it the same. The build finds the overlays from the lines that test a
   name, and each overlay's objects from its statements.

   An overlay's .text opens with the entry table every scene and debug room
   links, src/scene/entry.s, which each resolves against its own code. Its
   import table, imports.o, follows its code: the build writes it for each
   overlay from imports.inc, a stub for each main-image function the overlay's
   code calls. The libgcc functions its code uses come next (register calls,
   and double-precision arithmetic in two scenes), then a part kept in its
   own object, where it has one. Each statement names the sections it places,
   so the objects keep their order in each. Its globals follow in .bss, in the
   order its C defines them (BSS_DATA keeps a global out of the commons). */

#define OVERLAY_TEXT(object) \
    .text 0x2008000 : \
    { \
        */games/tbs/src/scene/entry.o(.text .padding) \
        */games/tbs/src/object.o(.text .padding) \
        */imports.o(.text .padding) \
        *libgcc.a:_call_via_rX.o(.text .padding) \
        *libgcc.a:_addsub_df.o(.text .padding) \
        *libgcc.a:_si_to_df.o(.text .padding) \
        *libgcc.a:_df_to_si.o(.text .padding) \
        *libgcc.a:_lshrdi3.o(.text .padding) \
        *libgcc.a:_pack_df.o(.text .padding) \
        *libgcc.a:_unpack_df.o(.text .padding) \
        *libgcc.a:_thenan_df.o(.text .padding) \
        */games/tbs/src/scene/entry.o(.rodata) \
        */games/tbs/src/object.o(.rodata) \
        */imports.o(.rodata) \
        *libgcc.a:_call_via_rX.o(.rodata) \
        *libgcc.a:_addsub_df.o(.rodata) \
        *libgcc.a:_si_to_df.o(.rodata) \
        *libgcc.a:_df_to_si.o(.rodata) \
        *libgcc.a:_lshrdi3.o(.rodata) \
        *libgcc.a:_pack_df.o(.rodata) \
        *libgcc.a:_unpack_df.o(.rodata) \
        *libgcc.a:_thenan_df.o(.rodata) \
        */games/tbs/src/scene/entry.o(.data .overlays) \
        */games/tbs/src/object.o(.data .overlays) \
        */imports.o(.data .overlays) \
        *libgcc.a:_call_via_rX.o(.data .overlays) \
        *libgcc.a:_addsub_df.o(.data .overlays) \
        *libgcc.a:_si_to_df.o(.data .overlays) \
        *libgcc.a:_df_to_si.o(.data .overlays) \
        *libgcc.a:_lshrdi3.o(.data .overlays) \
        *libgcc.a:_pack_df.o(.data .overlays) \
        *libgcc.a:_unpack_df.o(.data .overlays) \
        *libgcc.a:_thenan_df.o(.data .overlays) \
    }
#define OVERLAY_TEXT_AND(object, part) \
    .text 0x2008000 : \
    { \
        */games/tbs/src/scene/entry.o(.text .padding) \
        */games/tbs/src/object.o(.text .padding) \
        */imports.o(.text .padding) \
        *libgcc.a:_call_via_rX.o(.text .padding) \
        *libgcc.a:_addsub_df.o(.text .padding) \
        *libgcc.a:_si_to_df.o(.text .padding) \
        *libgcc.a:_df_to_si.o(.text .padding) \
        *libgcc.a:_lshrdi3.o(.text .padding) \
        *libgcc.a:_pack_df.o(.text .padding) \
        *libgcc.a:_unpack_df.o(.text .padding) \
        *libgcc.a:_thenan_df.o(.text .padding) \
        */games/tbs/src/part.o(.text .padding) \
        */games/tbs/src/scene/entry.o(.rodata) \
        */games/tbs/src/object.o(.rodata) \
        */imports.o(.rodata) \
        *libgcc.a:_call_via_rX.o(.rodata) \
        *libgcc.a:_addsub_df.o(.rodata) \
        *libgcc.a:_si_to_df.o(.rodata) \
        *libgcc.a:_df_to_si.o(.rodata) \
        *libgcc.a:_lshrdi3.o(.rodata) \
        *libgcc.a:_pack_df.o(.rodata) \
        *libgcc.a:_unpack_df.o(.rodata) \
        *libgcc.a:_thenan_df.o(.rodata) \
        */games/tbs/src/part.o(.rodata) \
        */games/tbs/src/scene/entry.o(.data .overlays) \
        */games/tbs/src/object.o(.data .overlays) \
        */imports.o(.data .overlays) \
        *libgcc.a:_call_via_rX.o(.data .overlays) \
        *libgcc.a:_addsub_df.o(.data .overlays) \
        *libgcc.a:_si_to_df.o(.data .overlays) \
        *libgcc.a:_df_to_si.o(.data .overlays) \
        *libgcc.a:_lshrdi3.o(.data .overlays) \
        *libgcc.a:_pack_df.o(.data .overlays) \
        *libgcc.a:_unpack_df.o(.data .overlays) \
        *libgcc.a:_thenan_df.o(.data .overlays) \
        */games/tbs/src/part.o(.data .overlays) \
    }
/* A scene: src/scene/NAME.c, its globals in its own .bss. */
#define SCENE(name) \
SECTIONS \
{ \
    OVERLAY_TEXT(scene/name) \
    .bss : \
    { \
        */games/tbs/src/scene/name.o(.bss COMMON) \
    } \
}
/* A scene whose globals are written in assembly, in src/scene/NAME_bss.s. */
#define SCENE_WITH_BSS(name) \
SECTIONS \
{ \
    OVERLAY_TEXT(scene/name) \
    .bss : \
    { \
        */games/tbs/src/scene/name.o(.bss COMMON) \
        */games/tbs/src/scene/name##_bss.o(.bss) \
    } \
}
/* A scene with a part in an object of its own, linked after its code. */
#define SCENE_AND(name, part) \
SECTIONS \
{ \
    OVERLAY_TEXT_AND(scene/name, part) \
    .bss : \
    { \
        */games/tbs/src/scene/name.o(.bss COMMON) \
    } \
}
/* A debug room: src/debug/NAME.c. */
#define DEBUG_ROOM(name) \
SECTIONS \
{ \
    OVERLAY_TEXT(debug/name) \
    .bss : \
    { \
        */games/tbs/src/debug/name.o(.bss COMMON) \
    } \
}

#if defined(OVERLAY_Title)
SCENE_WITH_BSS(title)
#elif defined(OVERLAY_FileScreen)
SCENE_AND(file_screen, scene/file_screen_keyboard.list)
#elif defined(OVERLAY_WorldMap)
SCENE_AND(world_map, graphics/world_map/lighthouse_transition.list)
#elif defined(OVERLAY_ValePrologue)
SCENE(vale_prologue)
#elif defined(OVERLAY_Vale)
SCENE(vale)
#elif defined(OVERLAY_ValePlaza)
SCENE(vale_plaza)
#elif defined(OVERLAY_ValeKradensCottage)
SCENE(vale_kradens_cottage)
#elif defined(OVERLAY_CottageRoomsA)
SCENE(cottage_rooms_a)
#elif defined(OVERLAY_CottageRoomsB)
SCENE(cottage_rooms_b)
#elif defined(OVERLAY_Sanctum)
SCENE(sanctum)
#elif defined(OVERLAY_ValeEruption)
SCENE(vale_eruption)
#elif defined(OVERLAY_SolSanctumLunaSolRooms)
SCENE(sol_sanctum_luna_sol_rooms)
#elif defined(OVERLAY_SolSanctumStatue)
SCENE_AND(sol_sanctum_statue, graphics/sol_sanctum_statue/rows.list)
#elif defined(OVERLAY_SolSanctumPsynergyStone)
SCENE(sol_sanctum_psynergy_stone)
#elif defined(OVERLAY_SolSanctumMaze)
SCENE(sol_sanctum_maze)
#elif defined(OVERLAY_SolSanctumCorridor)
SCENE(sol_sanctum_corridor)
#elif defined(OVERLAY_SolSanctumEntrance)
SCENE(sol_sanctum_entrance)
#elif defined(OVERLAY_SolSanctumElementalStarRoom)
SCENE(sol_sanctum_elemental_star_room)
#elif defined(OVERLAY_SolSanctumEruption)
SCENE(sol_sanctum_eruption)
#elif defined(OVERLAY_Vault)
SCENE(vault)
#elif defined(OVERLAY_LogRoomsA)
SCENE(log_rooms_a)
#elif defined(OVERLAY_StoneRooms)
SCENE(stone_rooms)
#elif defined(OVERLAY_VaultRevisit)
SCENE(vault_revisit)
#elif defined(OVERLAY_LogRoomsB)
SCENE(log_rooms_b)
#elif defined(OVERLAY_GomaCaveEntrance)
SCENE(goma_cave_entrance)
#elif defined(OVERLAY_GomaCavePond)
SCENE(goma_cave_pond)
#elif defined(OVERLAY_GomaCavePillar)
SCENE(goma_cave_pillar)
#elif defined(OVERLAY_GomaCaveStream)
SCENE(goma_cave_stream)
#elif defined(OVERLAY_Bilibin)
SCENE(bilibin)
#elif defined(OVERLAY_CottageRoomsC)
SCENE(cottage_rooms_c)
#elif defined(OVERLAY_McCoyPalace)
SCENE(mccoy_palace)
#elif defined(OVERLAY_McCoyPalaceGrounds)
SCENE(mccoy_palace_grounds)
#elif defined(OVERLAY_Kolima)
SCENE(kolima)
#elif defined(OVERLAY_LogRoomsC)
SCENE(log_rooms_c)
#elif defined(OVERLAY_KolimaForestEntrance)
SCENE(kolima_forest_entrance)
#elif defined(OVERLAY_KolimaForestGlade)
SCENE(kolima_forest_glade)
#elif defined(OVERLAY_KolimaForestCliff)
SCENE(kolima_forest_cliff)
#elif defined(OVERLAY_KolimaForestFloodgate)
SCENE(kolima_forest_floodgate)
#elif defined(OVERLAY_KolimaForestTree)
SCENE(kolima_forest_tree)
#elif defined(OVERLAY_TreeRooms)
SCENE(tree_rooms)
#elif defined(OVERLAY_TretTreeBranch)
SCENE(tret_tree_branch)
#elif defined(OVERLAY_BilibinCave)
SCENE(bilibin_cave)
#elif defined(OVERLAY_Imil)
SCENE(imil)
#elif defined(OVERLAY_ImilFuchinFalls)
SCENE(imil_fuchin_falls)
#elif defined(OVERLAY_MercuryLighthouseEntrance)
SCENE(mercury_lighthouse_entrance)
#elif defined(OVERLAY_MercuryLighthouse)
SCENE(mercury_lighthouse)
#elif defined(OVERLAY_MercuryLighthouseAerie)
SCENE(mercury_lighthouse_aerie)
#elif defined(OVERLAY_FuchinTemple)
SCENE(fuchin_temple)
#elif defined(OVERLAY_MogallForest)
SCENE(mogall_forest)
#elif defined(OVERLAY_Xian)
SCENE(xian)
#elif defined(OVERLAY_LacquerRoomsB)
SCENE(lacquer_rooms_b)
#elif defined(OVERLAY_AlpineCrossingLamaTemple)
SCENE(alpine_crossing_lama_temple)
#elif defined(OVERLAY_Altin)
SCENE(altin)
#elif defined(OVERLAY_AltinPeak)
SCENE(altin_peak)
#elif defined(OVERLAY_LamakanDesert)
SCENE_AND(lamakan_desert, graphics/lamakan_desert/heat_gauge.list)
#elif defined(OVERLAY_ValeCave)
SCENE(vale_cave)
#elif defined(OVERLAY_VaultCave)
SCENE(vault_cave)
#elif defined(OVERLAY_Kalay)
SCENE(kalay)
#elif defined(OVERLAY_StuccoRoomsA)
SCENE(stucco_rooms_a)
#elif defined(OVERLAY_HammetPalace)
SCENE(hammet_palace)
#elif defined(OVERLAY_Lunpa)
SCENE(lunpa)
#elif defined(OVERLAY_RockRooms)
SCENE(rock_rooms)
#elif defined(OVERLAY_WestLunpaCave)
SCENE(west_lunpa_cave)
#elif defined(OVERLAY_KalayTolbiDocks)
SCENE(kalay_tolbi_docks)
#elif defined(OVERLAY_TolbiShipDeck)
SCENE(tolbi_ship_deck)
#elif defined(OVERLAY_TolbiShipMast)
SCENE(tolbi_ship_mast)
#elif defined(OVERLAY_ShipRooms)
SCENE(ship_rooms)
#elif defined(OVERLAY_CrossboneIsle)
SCENE(crossbone_isle)
#elif defined(OVERLAY_CrossboneIsleSteppingStones)
SCENE(crossbone_isle_stepping_stones)
#elif defined(OVERLAY_CrossboneIsleKeys)
SCENE(crossbone_isle_keys)
#elif defined(OVERLAY_Tolbi)
SCENE(tolbi)
#elif defined(OVERLAY_StuccoRoomsB)
SCENE(stucco_rooms_b)
#elif defined(OVERLAY_TolbiGames)
SCENE(tolbi_games)
#elif defined(OVERLAY_BabiPalace)
SCENE(babi_palace)
#elif defined(OVERLAY_ColosseumEntrance)
SCENE(colosseum_entrance)
#elif defined(OVERLAY_ColosseumAzartMatch)
SCENE_AND(colosseum_azart_match, graphics/colosso/gauge.list)
#elif defined(OVERLAY_ColosseumSatrageMatch)
SCENE_AND(colosseum_satrage_match, graphics/colosso/gauge.list)
#elif defined(OVERLAY_ColosseumNavampaMatch)
SCENE_AND(colosseum_navampa_match, graphics/colosso/gauge.list)
#elif defined(OVERLAY_AltmillerCave)
SCENE(altmiller_cave)
#elif defined(OVERLAY_GondowanCave)
SCENE(gondowan_cave)
#elif defined(OVERLAY_LunpaFortress)
SCENE(lunpa_fortress)
#elif defined(OVERLAY_SuhallaDesert)
SCENE(suhalla_desert)
#elif defined(OVERLAY_Suhalla)
SCENE(suhalla)
#elif defined(OVERLAY_TimberRoomsA)
SCENE(timber_rooms_a)
#elif defined(OVERLAY_SuhallaGate)
SCENE(suhalla_gate)
#elif defined(OVERLAY_TunnelRuins)
SCENE(tunnel_ruins)
#elif defined(OVERLAY_BabiLighthouse)
SCENE(babi_lighthouse)
#elif defined(OVERLAY_Lalivero)
SCENE(lalivero)
#elif defined(OVERLAY_TimberRoomsBc)
SCENE(timber_rooms_bc)
#elif defined(OVERLAY_VenusLighthouse)
SCENE(venus_lighthouse)
#elif defined(OVERLAY_VenusLighthouseAerie)
SCENE(venus_lighthouse_aerie)
#elif defined(OVERLAY_LemurianShip)
SCENE(lemurian_ship)
#elif defined(OVERLAY_BattleArena)
SCENE(battle_arena)
#elif defined(OVERLAY_TestScenes)
DEBUG_ROOM(test_scenes)
#elif defined(OVERLAY_IncreaseItemAndLevel)
DEBUG_ROOM(level)
#elif defined(OVERLAY_MenuTest)
DEBUG_ROOM(test)
#endif

SECTIONS
{
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
