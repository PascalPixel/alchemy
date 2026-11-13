#ifndef ALCHEMY_SCENE_IDS_H
#define ALCHEMY_SCENE_IDS_H

/// @file
/// Scene numbers, named after their places, in the order of gScenes
/// (field/scenes.s).
///
/// Each SceneId_ is an absolute symbol whose value is the scene's row in the
/// table; C names the number by its macro, SCENE_VALE for SceneId_Vale, and
/// the exit tables name a scene by its place (SCENE_EXIT).

extern char SceneId_Title;
#define SCENE_TITLE ((s32)&SceneId_Title)
/// The File Screen, where the player chooses a file; the debug scene
/// selector calls it "(Clear data)".
extern char SceneId_FileScreen;
#define SCENE_FILE_SCREEN ((s32)&SceneId_FileScreen)
extern char SceneId_WorldMap;
#define SCENE_WORLD_MAP ((s32)&SceneId_WorldMap)
/// The World Map's map, row 1 of gMaps (field/scenes.s names it with
/// the World Map's scene row), which the field compares
/// gMainRoutineWork.mapId with.
extern char MapIndex_World;
#define MAP_INDEX_WORLD ((s32)&MapIndex_World)
/// Vale on the stormy night of the prologue, which Camelot labels PLOGV00.
extern char SceneId_ValePrologue;
#define SCENE_VALE_PROLOGUE ((s32)&SceneId_ValePrologue)
extern char SceneId_Vale;
#define SCENE_VALE ((s32)&SceneId_Vale)
extern char SceneId_ValePlaza;
#define SCENE_VALE_PLAZA ((s32)&SceneId_ValePlaza)
extern char SceneId_ValeKradensCottage;
#define SCENE_VALE_KRADENS_COTTAGE ((s32)&SceneId_ValeKradensCottage)
/// A sheet of rooms drawn as cottages, all of them Vale's.
extern char SceneId_CottageRoomsA;
#define SCENE_COTTAGE_ROOMS_A ((s32)&SceneId_CottageRoomsA)
/// A sheet of rooms drawn as cottages, of more than one place.
extern char SceneId_CottageRoomsB;
#define SCENE_COTTAGE_ROOMS_B ((s32)&SceneId_CottageRoomsB)
extern char SceneId_Sanctum;
#define SCENE_SANCTUM ((s32)&SceneId_Sanctum)
extern char SceneId_ValeEruption;
#define SCENE_VALE_ERUPTION ((s32)&SceneId_ValeEruption)
extern char SceneId_SolSanctumLunaSolRooms;
#define SCENE_SOL_SANCTUM_LUNA_SOL_ROOMS ((s32)&SceneId_SolSanctumLunaSolRooms)
extern char SceneId_SolSanctumStatue;
#define SCENE_SOL_SANCTUM_STATUE ((s32)&SceneId_SolSanctumStatue)
extern char SceneId_SolSanctumPsynergyStone;
#define SCENE_SOL_SANCTUM_PSYNERGY_STONE ((s32)&SceneId_SolSanctumPsynergyStone)
extern char SceneId_SolSanctumMaze;
#define SCENE_SOL_SANCTUM_MAZE ((s32)&SceneId_SolSanctumMaze)
extern char SceneId_SolSanctumCorridor;
#define SCENE_SOL_SANCTUM_CORRIDOR ((s32)&SceneId_SolSanctumCorridor)
extern char SceneId_SolSanctumReliefs;
#define SCENE_SOL_SANCTUM_RELIEFS ((s32)&SceneId_SolSanctumReliefs)
extern char SceneId_SolSanctumElementalStarRoom;
#define SCENE_SOL_SANCTUM_ELEMENTAL_STAR_ROOM                                  \
  ((s32)&SceneId_SolSanctumElementalStarRoom)
extern char SceneId_SolSanctumEruption;
#define SCENE_SOL_SANCTUM_ERUPTION ((s32)&SceneId_SolSanctumEruption)
extern char SceneId_SolEntrance;
#define SCENE_SOL_ENTRANCE ((s32)&SceneId_SolEntrance)
extern char SceneId_Vault;
#define SCENE_VAULT ((s32)&SceneId_Vault)
/// A sheet of rooms drawn in logs, all of them Vault's.
extern char SceneId_LogRoomsA;
#define SCENE_LOG_ROOMS_A ((s32)&SceneId_LogRoomsA)
/// A sheet of rooms of more than one place, drawn in stone.
extern char SceneId_StoneRooms;
#define SCENE_STONE_ROOMS ((s32)&SceneId_StoneRooms)
extern char SceneId_VaultRevisit;
#define SCENE_VAULT_REVISIT ((s32)&SceneId_VaultRevisit)
/// A sheet of rooms drawn in logs, all of them Vault's.
extern char SceneId_LogRoomsB;
#define SCENE_LOG_ROOMS_B ((s32)&SceneId_LogRoomsB)
extern char SceneId_GomaCaveEntrance;
#define SCENE_GOMA_CAVE_ENTRANCE ((s32)&SceneId_GomaCaveEntrance)
extern char SceneId_GomaCavePond;
#define SCENE_GOMA_CAVE_POND ((s32)&SceneId_GomaCavePond)
extern char SceneId_GomaCavePillar;
#define SCENE_GOMA_CAVE_PILLAR ((s32)&SceneId_GomaCavePillar)
extern char SceneId_GomaCaveStream;
#define SCENE_GOMA_CAVE_STREAM ((s32)&SceneId_GomaCaveStream)
extern char SceneId_GomaCaveBridge;
#define SCENE_GOMA_CAVE_BRIDGE ((s32)&SceneId_GomaCaveBridge)
extern char SceneId_Bilibin;
#define SCENE_BILIBIN ((s32)&SceneId_Bilibin)
/// A sheet of rooms drawn as cottages, all of them Bilibin's.
extern char SceneId_CottageRoomsC;
#define SCENE_COTTAGE_ROOMS_C ((s32)&SceneId_CottageRoomsC)
extern char SceneId_BilibinTreasury;
#define SCENE_BILIBIN_TREASURY ((s32)&SceneId_BilibinTreasury)
extern char SceneId_McCoyPalace;
#define SCENE_MC_COY_PALACE ((s32)&SceneId_McCoyPalace)
extern char SceneId_McCoyPalaceGrounds;
#define SCENE_MC_COY_PALACE_GROUNDS ((s32)&SceneId_McCoyPalaceGrounds)
extern char SceneId_BilibinBarricade;
#define SCENE_BILIBIN_BARRICADE ((s32)&SceneId_BilibinBarricade)
extern char SceneId_Kolima;
#define SCENE_KOLIMA ((s32)&SceneId_Kolima)
/// A sheet of rooms drawn in logs, all of them Kolima's.
extern char SceneId_LogRoomsC;
#define SCENE_LOG_ROOMS_C ((s32)&SceneId_LogRoomsC)
extern char SceneId_KolimaTunnel;
#define SCENE_KOLIMA_TUNNEL ((s32)&SceneId_KolimaTunnel)
extern char SceneId_KolimaBridge;
#define SCENE_KOLIMA_BRIDGE ((s32)&SceneId_KolimaBridge)
extern char SceneId_KolimaForestEntrance;
#define SCENE_KOLIMA_FOREST_ENTRANCE ((s32)&SceneId_KolimaForestEntrance)
extern char SceneId_KolimaForestGlade;
#define SCENE_KOLIMA_FOREST_GLADE ((s32)&SceneId_KolimaForestGlade)
extern char SceneId_KolimaForestCliff;
#define SCENE_KOLIMA_FOREST_CLIFF ((s32)&SceneId_KolimaForestCliff)
extern char SceneId_KolimaForestFloodgate;
#define SCENE_KOLIMA_FOREST_FLOODGATE ((s32)&SceneId_KolimaForestFloodgate)
extern char SceneId_KolimaForestTree;
#define SCENE_KOLIMA_FOREST_TREE ((s32)&SceneId_KolimaForestTree)
/// A sheet of rooms drawn inside a tree.
extern char SceneId_TreeRooms;
#define SCENE_TREE_ROOMS ((s32)&SceneId_TreeRooms)
extern char SceneId_TretTreeBranch;
#define SCENE_TRET_TREE_BRANCH ((s32)&SceneId_TretTreeBranch)
extern char SceneId_BilibinCaveIce;
#define SCENE_BILIBIN_CAVE_ICE ((s32)&SceneId_BilibinCaveIce)
extern char SceneId_BilibinCaveRapier;
#define SCENE_BILIBIN_CAVE_RAPIER ((s32)&SceneId_BilibinCaveRapier)
extern char SceneId_BilibinCaveEntrance;
#define SCENE_BILIBIN_CAVE_ENTRANCE ((s32)&SceneId_BilibinCaveEntrance)
extern char SceneId_Imil;
#define SCENE_IMIL ((s32)&SceneId_Imil)
/// A sheet of rooms drawn in ice, all of them Imil's.
extern char SceneId_IceRooms;
#define SCENE_ICE_ROOMS ((s32)&SceneId_IceRooms)
extern char SceneId_ImilFallsCave;
#define SCENE_IMIL_FALLS_CAVE ((s32)&SceneId_ImilFallsCave)
extern char SceneId_MercuryLighthouseEntrance;
#define SCENE_MERCURY_LIGHTHOUSE_ENTRANCE                                      \
  ((s32)&SceneId_MercuryLighthouseEntrance)
extern char SceneId_MercuryLighthouseLower;
#define SCENE_MERCURY_LIGHTHOUSE_LOWER ((s32)&SceneId_MercuryLighthouseLower)
extern char SceneId_MercuryLighthouseMiddle;
#define SCENE_MERCURY_LIGHTHOUSE_MIDDLE ((s32)&SceneId_MercuryLighthouseMiddle)
extern char SceneId_MercuryLighthouseUpper;
#define SCENE_MERCURY_LIGHTHOUSE_UPPER ((s32)&SceneId_MercuryLighthouseUpper)
extern char SceneId_MercuryLighthouseExterior;
#define SCENE_MERCURY_LIGHTHOUSE_EXTERIOR                                      \
  ((s32)&SceneId_MercuryLighthouseExterior)
extern char SceneId_MercuryLighthouseAerie;
#define SCENE_MERCURY_LIGHTHOUSE_AERIE ((s32)&SceneId_MercuryLighthouseAerie)
extern char SceneId_UnusedMercuryLighthouseAerieCopy;
#define SCENE_UNUSED_MERCURY_LIGHTHOUSE_AERIE_COPY                             \
  ((s32)&SceneId_UnusedMercuryLighthouseAerieCopy)
extern char SceneId_FuchinTempleGrounds;
#define SCENE_FUCHIN_TEMPLE_GROUNDS ((s32)&SceneId_FuchinTempleGrounds)
/// A sheet of rooms drawn in red lacquer, of more than one place.
extern char SceneId_LacquerRoomsA;
#define SCENE_LACQUER_ROOMS_A ((s32)&SceneId_LacquerRoomsA)
extern char SceneId_FuchinFallsCaveEntrance;
#define SCENE_FUCHIN_FALLS_CAVE_ENTRANCE ((s32)&SceneId_FuchinFallsCaveEntrance)
extern char SceneId_FuchinFallsCaveZephyr;
#define SCENE_FUCHIN_FALLS_CAVE_ZEPHYR ((s32)&SceneId_FuchinFallsCaveZephyr)
extern char SceneId_FuchinFallsCaveDragonsEye;
#define SCENE_FUCHIN_FALLS_CAVE_DRAGONS_EYE                                    \
  ((s32)&SceneId_FuchinFallsCaveDragonsEye)
extern char SceneId_FuchinFallsCaveArcticBlade;
#define SCENE_FUCHIN_FALLS_CAVE_ARCTIC_BLADE                                   \
  ((s32)&SceneId_FuchinFallsCaveArcticBlade)
extern char SceneId_FuchinFallsCaveOrbOfForce;
#define SCENE_FUCHIN_FALLS_CAVE_ORB_OF_FORCE                                   \
  ((s32)&SceneId_FuchinFallsCaveOrbOfForce)
extern char SceneId_FuchinFallsCaveDragonStatue;
#define SCENE_FUCHIN_FALLS_CAVE_DRAGON_STATUE                                  \
  ((s32)&SceneId_FuchinFallsCaveDragonStatue)
extern char SceneId_MogallForestSign;
#define SCENE_MOGALL_FOREST_SIGN ((s32)&SceneId_MogallForestSign)
extern char SceneId_MogallForestQuartz;
#define SCENE_MOGALL_FOREST_QUARTZ ((s32)&SceneId_MogallForestQuartz)
extern char SceneId_MogallForestKillerApe;
#define SCENE_MOGALL_FOREST_KILLER_APE ((s32)&SceneId_MogallForestKillerApe)
extern char SceneId_MogallForestSidePaths;
#define SCENE_MOGALL_FOREST_SIDE_PATHS ((s32)&SceneId_MogallForestSidePaths)
extern char SceneId_Xian;
#define SCENE_XIAN ((s32)&SceneId_Xian)
/// A sheet of rooms drawn in red lacquer, all of them Xian's.
extern char SceneId_LacquerRoomsB;
#define SCENE_LACQUER_ROOMS_B ((s32)&SceneId_LacquerRoomsB)
extern char SceneId_AlpineCrossing;
#define SCENE_ALPINE_CROSSING ((s32)&SceneId_AlpineCrossing)
extern char SceneId_Altin;
#define SCENE_ALTIN ((s32)&SceneId_Altin)
/// A sheet of rooms drawn as cottages, all of them Altin's.
extern char SceneId_CottageRoomsD;
#define SCENE_COTTAGE_ROOMS_D ((s32)&SceneId_CottageRoomsD)
extern char SceneId_AltinPeakFirstWaterBeast;
#define SCENE_ALTIN_PEAK_FIRST_WATER_BEAST                                     \
  ((s32)&SceneId_AltinPeakFirstWaterBeast)
extern char SceneId_AltinPeakRailTunnel;
#define SCENE_ALTIN_PEAK_RAIL_TUNNEL ((s32)&SceneId_AltinPeakRailTunnel)
extern char SceneId_AltinPeakSecondWaterBeast;
#define SCENE_ALTIN_PEAK_SECOND_WATER_BEAST                                    \
  ((s32)&SceneId_AltinPeakSecondWaterBeast)
extern char SceneId_AltinPeakSouthEntrance;
#define SCENE_ALTIN_PEAK_SOUTH_ENTRANCE ((s32)&SceneId_AltinPeakSouthEntrance)
extern char SceneId_AltinPeakThirdWaterBeast;
#define SCENE_ALTIN_PEAK_THIRD_WATER_BEAST                                     \
  ((s32)&SceneId_AltinPeakThirdWaterBeast)
extern char SceneId_AltinPeakPool;
#define SCENE_ALTIN_PEAK_POOL ((s32)&SceneId_AltinPeakPool)
extern char SceneId_AltinPeakSpritz;
#define SCENE_ALTIN_PEAK_SPRITZ ((s32)&SceneId_AltinPeakSpritz)
extern char SceneId_AltinPeakJunction;
#define SCENE_ALTIN_PEAK_JUNCTION ((s32)&SceneId_AltinPeakJunction)
extern char SceneId_AltinPeakFallingRocks;
#define SCENE_ALTIN_PEAK_FALLING_ROCKS ((s32)&SceneId_AltinPeakFallingRocks)
extern char SceneId_AltinPeakHydrosStatue;
#define SCENE_ALTIN_PEAK_HYDROS_STATUE ((s32)&SceneId_AltinPeakHydrosStatue)
extern char SceneId_AltinPeakWestExit;
#define SCENE_ALTIN_PEAK_WEST_EXIT ((s32)&SceneId_AltinPeakWestExit)
extern char SceneId_LamaTemple;
#define SCENE_LAMA_TEMPLE ((s32)&SceneId_LamaTemple)
extern char SceneId_LamakanDesertEast;
#define SCENE_LAMAKAN_DESERT_EAST ((s32)&SceneId_LamakanDesertEast)
extern char SceneId_LamakanDesertSmog;
#define SCENE_LAMAKAN_DESERT_SMOG ((s32)&SceneId_LamakanDesertSmog)
extern char SceneId_LamakanDesertManticore;
#define SCENE_LAMAKAN_DESERT_MANTICORE ((s32)&SceneId_LamakanDesertManticore)
extern char SceneId_LamakanDesertCave;
#define SCENE_LAMAKAN_DESERT_CAVE ((s32)&SceneId_LamakanDesertCave)
extern char SceneId_ValeCaveMouth;
#define SCENE_VALE_CAVE_MOUTH ((s32)&SceneId_ValeCaveMouth)
extern char SceneId_ValeCaveKite;
#define SCENE_VALE_CAVE_KITE ((s32)&SceneId_ValeCaveKite)
extern char SceneId_ValeCaveLogs;
#define SCENE_VALE_CAVE_LOGS ((s32)&SceneId_ValeCaveLogs)
extern char SceneId_VaultCavePassage;
#define SCENE_VAULT_CAVE_PASSAGE ((s32)&SceneId_VaultCavePassage)
extern char SceneId_VaultCaveVambrace;
#define SCENE_VAULT_CAVE_VAMBRACE ((s32)&SceneId_VaultCaveVambrace)
extern char SceneId_VaultCaveMouth;
#define SCENE_VAULT_CAVE_MOUTH ((s32)&SceneId_VaultCaveMouth)
extern char SceneId_Kalay;
#define SCENE_KALAY ((s32)&SceneId_Kalay)
/// A sheet of rooms drawn in stucco, all of them Kalay's.
extern char SceneId_StuccoRoomsA;
#define SCENE_STUCCO_ROOMS_A ((s32)&SceneId_StuccoRoomsA)
extern char SceneId_KalaySanctum;
#define SCENE_KALAY_SANCTUM ((s32)&SceneId_KalaySanctum)
extern char SceneId_HammetPalaceGrounds;
#define SCENE_HAMMET_PALACE_GROUNDS ((s32)&SceneId_HammetPalaceGrounds)
extern char SceneId_HammetPalace;
#define SCENE_HAMMET_PALACE ((s32)&SceneId_HammetPalace)
extern char SceneId_Lunpa;
#define SCENE_LUNPA ((s32)&SceneId_Lunpa)
/// A sheet of rooms cut into rock, of more than one place.
extern char SceneId_RockRooms;
#define SCENE_ROCK_ROOMS ((s32)&SceneId_RockRooms)
extern char SceneId_WestLunpaCave;
#define SCENE_WEST_LUNPA_CAVE ((s32)&SceneId_WestLunpaCave)
extern char SceneId_KalayDocks;
#define SCENE_KALAY_DOCKS ((s32)&SceneId_KalayDocks)
/// A sheet of rooms drawn as cottages, of more than one place.
extern char SceneId_CottageRoomsE;
#define SCENE_COTTAGE_ROOMS_E ((s32)&SceneId_CottageRoomsE)
extern char SceneId_TolbiShipDeck;
#define SCENE_TOLBI_SHIP_DECK ((s32)&SceneId_TolbiShipDeck)
extern char SceneId_TolbiShipMast;
#define SCENE_TOLBI_SHIP_MAST ((s32)&SceneId_TolbiShipMast)
/// A sheet of rooms drawn as a ship's cabins.
extern char SceneId_ShipRooms;
#define SCENE_SHIP_ROOMS ((s32)&SceneId_ShipRooms)
extern char SceneId_TolbiDocks;
#define SCENE_TOLBI_DOCKS ((s32)&SceneId_TolbiDocks)
extern char SceneId_CrossboneIsle;
#define SCENE_CROSSBONE_ISLE ((s32)&SceneId_CrossboneIsle)
extern char SceneId_CrossboneIsleCaveEntrance;
#define SCENE_CROSSBONE_ISLE_CAVE_ENTRANCE                                     \
  ((s32)&SceneId_CrossboneIsleCaveEntrance)
extern char SceneId_CrossboneIsleSteppingStones;
#define SCENE_CROSSBONE_ISLE_STEPPING_STONES                                   \
  ((s32)&SceneId_CrossboneIsleSteppingStones)
extern char SceneId_CrossboneIsleIcePillars;
#define SCENE_CROSSBONE_ISLE_ICE_PILLARS ((s32)&SceneId_CrossboneIsleIcePillars)
extern char SceneId_CrossboneIsleKeys;
#define SCENE_CROSSBONE_ISLE_KEYS ((s32)&SceneId_CrossboneIsleKeys)
extern char SceneId_CrossboneIsleStatues;
#define SCENE_CROSSBONE_ISLE_STATUES ((s32)&SceneId_CrossboneIsleStatues)
extern char SceneId_CrossboneIsleFloodgate;
#define SCENE_CROSSBONE_ISLE_FLOODGATE ((s32)&SceneId_CrossboneIsleFloodgate)
extern char SceneId_CrossboneIsleBane;
#define SCENE_CROSSBONE_ISLE_BANE ((s32)&SceneId_CrossboneIsleBane)
extern char SceneId_CrossboneIsleStackedPillars;
#define SCENE_CROSSBONE_ISLE_STACKED_PILLARS                                   \
  ((s32)&SceneId_CrossboneIsleStackedPillars)
extern char SceneId_CrossboneIsleDragonStatue;
#define SCENE_CROSSBONE_ISLE_DRAGON_STATUE                                     \
  ((s32)&SceneId_CrossboneIsleDragonStatue)
extern char SceneId_CrossboneIsleFloatingLogs;
#define SCENE_CROSSBONE_ISLE_FLOATING_LOGS                                     \
  ((s32)&SceneId_CrossboneIsleFloatingLogs)
extern char SceneId_CrossboneIslePier;
#define SCENE_CROSSBONE_ISLE_PIER ((s32)&SceneId_CrossboneIslePier)
extern char SceneId_PirateShip;
#define SCENE_PIRATE_SHIP ((s32)&SceneId_PirateShip)
extern char SceneId_CrossboneIsleFirstFloor;
#define SCENE_CROSSBONE_ISLE_FIRST_FLOOR ((s32)&SceneId_CrossboneIsleFirstFloor)
extern char SceneId_CrossboneIsleSecondFloor;
#define SCENE_CROSSBONE_ISLE_SECOND_FLOOR                                      \
  ((s32)&SceneId_CrossboneIsleSecondFloor)
extern char SceneId_CrossboneIsleThirdFloor;
#define SCENE_CROSSBONE_ISLE_THIRD_FLOOR ((s32)&SceneId_CrossboneIsleThirdFloor)
extern char SceneId_CrossboneIsleFourthFloor;
#define SCENE_CROSSBONE_ISLE_FOURTH_FLOOR                                      \
  ((s32)&SceneId_CrossboneIsleFourthFloor)
extern char SceneId_CrossboneIsleFifthFloor;
#define SCENE_CROSSBONE_ISLE_FIFTH_FLOOR ((s32)&SceneId_CrossboneIsleFifthFloor)
extern char SceneId_CrossboneIsleSixthFloor;
#define SCENE_CROSSBONE_ISLE_SIXTH_FLOOR ((s32)&SceneId_CrossboneIsleSixthFloor)
extern char SceneId_CrossboneIsleSeventhFloor;
#define SCENE_CROSSBONE_ISLE_SEVENTH_FLOOR                                     \
  ((s32)&SceneId_CrossboneIsleSeventhFloor)
extern char SceneId_CrossboneIsleEighthFloor;
#define SCENE_CROSSBONE_ISLE_EIGHTH_FLOOR                                      \
  ((s32)&SceneId_CrossboneIsleEighthFloor)
extern char SceneId_CrossboneIsleNinthFloor;
#define SCENE_CROSSBONE_ISLE_NINTH_FLOOR ((s32)&SceneId_CrossboneIsleNinthFloor)
extern char SceneId_Tolbi;
#define SCENE_TOLBI ((s32)&SceneId_Tolbi)
/// A sheet of rooms drawn in stucco, all of them Tolbi's.
extern char SceneId_StuccoRoomsB;
#define SCENE_STUCCO_ROOMS_B ((s32)&SceneId_StuccoRoomsB)
/// A sheet of rooms drawn in stucco, both of them Tolbi's.
extern char SceneId_StuccoRoomsC;
#define SCENE_STUCCO_ROOMS_C ((s32)&SceneId_StuccoRoomsC)
extern char SceneId_BabiPalace;
#define SCENE_BABI_PALACE ((s32)&SceneId_BabiPalace)
extern char SceneId_BabiPalaceEntrance;
#define SCENE_BABI_PALACE_ENTRANCE ((s32)&SceneId_BabiPalaceEntrance)
extern char SceneId_Colosseum;
#define SCENE_COLOSSEUM ((s32)&SceneId_Colosseum)
extern char SceneId_ColosseumTunnel;
#define SCENE_COLOSSEUM_TUNNEL ((s32)&SceneId_ColosseumTunnel)
extern char SceneId_ColosseumEntrance;
#define SCENE_COLOSSEUM_ENTRANCE ((s32)&SceneId_ColosseumEntrance)
extern char SceneId_ColosseumAzartMatch;
#define SCENE_COLOSSEUM_AZART_MATCH ((s32)&SceneId_ColosseumAzartMatch)
extern char SceneId_ColosseumSatrageMatch;
#define SCENE_COLOSSEUM_SATRAGE_MATCH ((s32)&SceneId_ColosseumSatrageMatch)
extern char SceneId_ColosseumNavampaMatch;
#define SCENE_COLOSSEUM_NAVAMPA_MATCH ((s32)&SceneId_ColosseumNavampaMatch)
extern char SceneId_AltmillerCaveEntrance;
#define SCENE_ALTMILLER_CAVE_ENTRANCE ((s32)&SceneId_AltmillerCaveEntrance)
extern char SceneId_AltmillerCaveBabi;
#define SCENE_ALTMILLER_CAVE_BABI ((s32)&SceneId_AltmillerCaveBabi)
extern char SceneId_AltmillerCaveVialChest;
#define SCENE_ALTMILLER_CAVE_VIAL_CHEST ((s32)&SceneId_AltmillerCaveVialChest)
extern char SceneId_AltmillerCaveSquall;
#define SCENE_ALTMILLER_CAVE_SQUALL ((s32)&SceneId_AltmillerCaveSquall)
extern char SceneId_AltmillerCaveCookieChest;
#define SCENE_ALTMILLER_CAVE_COOKIE_CHEST                                      \
  ((s32)&SceneId_AltmillerCaveCookieChest)
extern char SceneId_AltmillerCaveDraught;
#define SCENE_ALTMILLER_CAVE_DRAUGHT ((s32)&SceneId_AltmillerCaveDraught)
extern char SceneId_GondowanPassage;
#define SCENE_GONDOWAN_PASSAGE ((s32)&SceneId_GondowanPassage)
extern char SceneId_KalayTunnelEntrance;
#define SCENE_KALAY_TUNNEL_ENTRANCE ((s32)&SceneId_KalayTunnelEntrance)
extern char SceneId_KalayTunnelCave;
#define SCENE_KALAY_TUNNEL_CAVE ((s32)&SceneId_KalayTunnelCave)
extern char SceneId_KalayTunnelPassage;
#define SCENE_KALAY_TUNNEL_PASSAGE ((s32)&SceneId_KalayTunnelPassage)
extern char SceneId_KalayTunnelTreasureRoom;
#define SCENE_KALAY_TUNNEL_TREASURE_ROOM ((s32)&SceneId_KalayTunnelTreasureRoom)
extern char SceneId_GondowanCaveRiver;
#define SCENE_GONDOWAN_CAVE_RIVER ((s32)&SceneId_GondowanCaveRiver)
extern char SceneId_GondowanCave;
#define SCENE_GONDOWAN_CAVE ((s32)&SceneId_GondowanCave)
extern char SceneId_LunpaFortressGates;
#define SCENE_LUNPA_FORTRESS_GATES ((s32)&SceneId_LunpaFortressGates)
extern char SceneId_LunpaFortressGroundFloor;
#define SCENE_LUNPA_FORTRESS_GROUND_FLOOR                                      \
  ((s32)&SceneId_LunpaFortressGroundFloor)
extern char SceneId_LunpaFortressUpperFloor;
#define SCENE_LUNPA_FORTRESS_UPPER_FLOOR ((s32)&SceneId_LunpaFortressUpperFloor)
extern char SceneId_LunpaFortressCave;
#define SCENE_LUNPA_FORTRESS_CAVE ((s32)&SceneId_LunpaFortressCave)
extern char SceneId_LunpaFortressToadonpa;
#define SCENE_LUNPA_FORTRESS_TOADONPA ((s32)&SceneId_LunpaFortressToadonpa)
extern char SceneId_SuhallaDesertNorth;
#define SCENE_SUHALLA_DESERT_NORTH ((s32)&SceneId_SuhallaDesertNorth)
extern char SceneId_SuhallaDesertSouth;
#define SCENE_SUHALLA_DESERT_SOUTH ((s32)&SceneId_SuhallaDesertSouth)
extern char SceneId_SuhallaDesertCaves;
#define SCENE_SUHALLA_DESERT_CAVES ((s32)&SceneId_SuhallaDesertCaves)
extern char SceneId_Suhalla;
#define SCENE_SUHALLA ((s32)&SceneId_Suhalla)
/// A sheet of rooms drawn in timber, all of them Suhalla's.
extern char SceneId_TimberRoomsA;
#define SCENE_TIMBER_ROOMS_A ((s32)&SceneId_TimberRoomsA)
extern char SceneId_SuhallaGateCheckpoint;
#define SCENE_SUHALLA_GATE_CHECKPOINT ((s32)&SceneId_SuhallaGateCheckpoint)
extern char SceneId_SuhallaGateDew;
#define SCENE_SUHALLA_GATE_DEW ((s32)&SceneId_SuhallaGateDew)
extern char SceneId_GatewayCave;
#define SCENE_GATEWAY_CAVE ((s32)&SceneId_GatewayCave)
extern char SceneId_TunnelRuinsGreatStatue;
#define SCENE_TUNNEL_RUINS_GREAT_STATUE ((s32)&SceneId_TunnelRuinsGreatStatue)
extern char SceneId_TunnelRuinsLadyStatues;
#define SCENE_TUNNEL_RUINS_LADY_STATUES ((s32)&SceneId_TunnelRuinsLadyStatues)
extern char SceneId_BabiLighthouseStoneDoor;
#define SCENE_BABI_LIGHTHOUSE_STONE_DOOR ((s32)&SceneId_BabiLighthouseStoneDoor)
extern char SceneId_BabiLighthouseLuff;
#define SCENE_BABI_LIGHTHOUSE_LUFF ((s32)&SceneId_BabiLighthouseLuff)
extern char SceneId_BabiLighthouseEntry;
#define SCENE_BABI_LIGHTHOUSE_ENTRY ((s32)&SceneId_BabiLighthouseEntry)
extern char SceneId_RoadToBabiLighthouse;
#define SCENE_ROAD_TO_BABI_LIGHTHOUSE ((s32)&SceneId_RoadToBabiLighthouse)
extern char SceneId_Lalivero;
#define SCENE_LALIVERO ((s32)&SceneId_Lalivero)
/// The first of two sheets of Lalivero's rooms, drawn in timber.
extern char SceneId_TimberRoomsB;
#define SCENE_TIMBER_ROOMS_B ((s32)&SceneId_TimberRoomsB)
/// The second of two sheets of Lalivero's rooms, drawn in timber.
extern char SceneId_TimberRoomsC;
#define SCENE_TIMBER_ROOMS_C ((s32)&SceneId_TimberRoomsC)
extern char SceneId_VenusLighthouseEntry;
#define SCENE_VENUS_LIGHTHOUSE_ENTRY ((s32)&SceneId_VenusLighthouseEntry)
extern char SceneId_VenusLighthouseGroundFloor;
#define SCENE_VENUS_LIGHTHOUSE_GROUND_FLOOR                                    \
  ((s32)&SceneId_VenusLighthouseGroundFloor)
extern char SceneId_VenusLighthouseFirstFloor;
#define SCENE_VENUS_LIGHTHOUSE_FIRST_FLOOR                                     \
  ((s32)&SceneId_VenusLighthouseFirstFloor)
extern char SceneId_VenusLighthouseSecondFloor;
#define SCENE_VENUS_LIGHTHOUSE_SECOND_FLOOR                                    \
  ((s32)&SceneId_VenusLighthouseSecondFloor)
extern char SceneId_VenusLighthouseThirdFloor;
#define SCENE_VENUS_LIGHTHOUSE_THIRD_FLOOR                                     \
  ((s32)&SceneId_VenusLighthouseThirdFloor)
extern char SceneId_VenusLighthouseFourthFloor;
#define SCENE_VENUS_LIGHTHOUSE_FOURTH_FLOOR                                    \
  ((s32)&SceneId_VenusLighthouseFourthFloor)
extern char SceneId_VenusLighthouseAerie;
#define SCENE_VENUS_LIGHTHOUSE_AERIE ((s32)&SceneId_VenusLighthouseAerie)
extern char SceneId_LemurianShip;
#define SCENE_LEMURIAN_SHIP ((s32)&SceneId_LemurianShip)
extern char SceneId_TolbiSpring;
#define SCENE_TOLBI_SPRING ((s32)&SceneId_TolbiSpring)
extern char SceneId_BattleArena;
#define SCENE_BATTLE_ARENA ((s32)&SceneId_BattleArena)
extern char SceneId_Test0;
#define SCENE_TEST_0 ((s32)&SceneId_Test0)
extern char SceneId_Test1;
#define SCENE_TEST_1 ((s32)&SceneId_Test1)
extern char SceneId_Test2;
#define SCENE_TEST_2 ((s32)&SceneId_Test2)
extern char SceneId_Test3;
#define SCENE_TEST_3 ((s32)&SceneId_Test3)
extern char SceneId_Test4;
#define SCENE_TEST_4 ((s32)&SceneId_Test4)
extern char SceneId_Test5;
#define SCENE_TEST_5 ((s32)&SceneId_Test5)
extern char SceneId_Test6;
#define SCENE_TEST_6 ((s32)&SceneId_Test6)
extern char SceneId_Test7;
#define SCENE_TEST_7 ((s32)&SceneId_Test7)
extern char SceneId_IncreaseItemAndLevel;
#define SCENE_INCREASE_ITEM_AND_LEVEL ((s32)&SceneId_IncreaseItemAndLevel)
extern char SceneId_MenuTest;
#define SCENE_MENU_TEST ((s32)&SceneId_MenuTest)

#endif
