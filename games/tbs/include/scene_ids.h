#ifndef ALCHEMY_SCENE_IDS_H
#define ALCHEMY_SCENE_IDS_H

/// @file
/// Scene numbers, named after their places, in the order of gScenes
/// (field/scenes.s).
///
/// Each SceneId_ is an absolute symbol whose value is the scene's row in the
/// table; C takes the number as (s32)&SceneId_Vale, and the exit tables name a
/// scene by its place (SCENE_EXIT).

extern char SceneId_Title;
/// The File Screen, where the player chooses a file; the debug scene
/// selector calls it "(Clear data)".
extern char SceneId_FileScreen;
extern char SceneId_WorldMap;
/// The World Map's map, row 1 of gMaps (field/scenes.s names it with
/// the World Map's scene row), which the field compares
/// gMainRoutineWork.mapId with.
extern char MapIndex_World;
#define MAP_INDEX_WORLD ((s32)&MapIndex_World)
/// Vale on the stormy night of the prologue, which Camelot labels PLOGV00.
extern char SceneId_ValePrologue;
extern char SceneId_Vale;
extern char SceneId_ValePlaza;
extern char SceneId_ValeKradensCottage;
/// A sheet of rooms drawn as cottages, all of them Vale's.
extern char SceneId_CottageRoomsA;
/// A sheet of rooms drawn as cottages, of more than one place.
extern char SceneId_CottageRoomsB;
extern char SceneId_Sanctum;
extern char SceneId_ValeEruption;
extern char SceneId_SolSanctumLunaSolRooms;
extern char SceneId_SolSanctumStatue;
extern char SceneId_SolSanctumPsynergyStone;
extern char SceneId_SolSanctumMaze;
extern char SceneId_SolSanctumCorridor;
extern char SceneId_SolSanctumReliefs;
extern char SceneId_SolSanctumElementalStarRoom;
extern char SceneId_SolSanctumEruption;
extern char SceneId_SolEntrance;
extern char SceneId_Vault;
/// A sheet of rooms drawn in logs, all of them Vault's.
extern char SceneId_LogRoomsA;
/// A sheet of rooms of more than one place, drawn in stone.
extern char SceneId_StoneRooms;
extern char SceneId_VaultRevisit;
/// A sheet of rooms drawn in logs, all of them Vault's.
extern char SceneId_LogRoomsB;
extern char SceneId_GomaCaveEntrance;
extern char SceneId_GomaCavePond;
extern char SceneId_GomaCavePillar;
extern char SceneId_GomaCaveStream;
extern char SceneId_GomaCaveBridge;
extern char SceneId_Bilibin;
/// A sheet of rooms drawn as cottages, all of them Bilibin's.
extern char SceneId_CottageRoomsC;
extern char SceneId_BilibinTreasury;
extern char SceneId_McCoyPalace;
extern char SceneId_McCoyPalaceGrounds;
extern char SceneId_BilibinBarricade;
extern char SceneId_Kolima;
/// A sheet of rooms drawn in logs, all of them Kolima's.
extern char SceneId_LogRoomsC;
extern char SceneId_KolimaTunnel;
extern char SceneId_KolimaBridge;
extern char SceneId_KolimaForestEntrance;
extern char SceneId_KolimaForestGlade;
extern char SceneId_KolimaForestCliff;
extern char SceneId_KolimaForestFloodgate;
extern char SceneId_KolimaForestTree;
/// A sheet of rooms drawn inside a tree.
extern char SceneId_TreeRooms;
extern char SceneId_TretTreeBranch;
extern char SceneId_BilibinCaveIce;
extern char SceneId_BilibinCaveRapier;
extern char SceneId_BilibinCaveEntrance;
extern char SceneId_Imil;
/// A sheet of rooms drawn in ice, all of them Imil's.
extern char SceneId_IceRooms;
extern char SceneId_ImilFallsCave;
extern char SceneId_MercuryLighthouseEntrance;
extern char SceneId_MercuryLighthouseLower;
extern char SceneId_MercuryLighthouseMiddle;
extern char SceneId_MercuryLighthouseUpper;
extern char SceneId_MercuryLighthouseExterior;
extern char SceneId_MercuryLighthouseAerie;
extern char SceneId_UnusedMercuryLighthouseAerieCopy;
extern char SceneId_FuchinTempleGrounds;
/// A sheet of rooms drawn in red lacquer, of more than one place.
extern char SceneId_LacquerRoomsA;
extern char SceneId_FuchinFallsCaveEntrance;
extern char SceneId_FuchinFallsCaveZephyr;
extern char SceneId_FuchinFallsCaveDragonsEye;
extern char SceneId_FuchinFallsCaveArcticBlade;
extern char SceneId_FuchinFallsCaveOrbOfForce;
extern char SceneId_FuchinFallsCaveDragonStatue;
extern char SceneId_MogallForestSign;
extern char SceneId_MogallForestQuartz;
extern char SceneId_MogallForestKillerApe;
extern char SceneId_MogallForestSidePaths;
extern char SceneId_Xian;
/// A sheet of rooms drawn in red lacquer, all of them Xian's.
extern char SceneId_LacquerRoomsB;
extern char SceneId_AlpineCrossing;
extern char SceneId_Altin;
/// A sheet of rooms drawn as cottages, all of them Altin's.
extern char SceneId_CottageRoomsD;
extern char SceneId_AltinPeakFirstWaterBeast;
extern char SceneId_AltinPeakRailTunnel;
extern char SceneId_AltinPeakSecondWaterBeast;
extern char SceneId_AltinPeakSouthEntrance;
extern char SceneId_AltinPeakThirdWaterBeast;
extern char SceneId_AltinPeakPool;
extern char SceneId_AltinPeakSpritz;
extern char SceneId_AltinPeakJunction;
extern char SceneId_AltinPeakFallingRocks;
extern char SceneId_AltinPeakHydrosStatue;
extern char SceneId_AltinPeakWestExit;
extern char SceneId_LamaTemple;
extern char SceneId_LamakanDesertEast;
extern char SceneId_LamakanDesertSmog;
extern char SceneId_LamakanDesertManticore;
extern char SceneId_LamakanDesertCave;
extern char SceneId_ValeCaveMouth;
extern char SceneId_ValeCaveKite;
extern char SceneId_ValeCaveLogs;
extern char SceneId_VaultCavePassage;
extern char SceneId_VaultCaveVambrace;
extern char SceneId_VaultCaveMouth;
extern char SceneId_Kalay;
/// A sheet of rooms drawn in stucco, all of them Kalay's.
extern char SceneId_StuccoRoomsA;
extern char SceneId_KalaySanctum;
extern char SceneId_HammetPalaceGrounds;
extern char SceneId_HammetPalace;
extern char SceneId_Lunpa;
/// A sheet of rooms cut into rock, of more than one place.
extern char SceneId_RockRooms;
extern char SceneId_WestLunpaCave;
extern char SceneId_KalayDocks;
/// A sheet of rooms drawn as cottages, of more than one place.
extern char SceneId_CottageRoomsE;
extern char SceneId_TolbiShipDeck;
extern char SceneId_TolbiShipMast;
/// A sheet of rooms drawn as a ship's cabins.
extern char SceneId_ShipRooms;
extern char SceneId_TolbiDocks;
extern char SceneId_CrossboneIsle;
extern char SceneId_CrossboneIsleCaveEntrance;
extern char SceneId_CrossboneIsleSteppingStones;
extern char SceneId_CrossboneIsleIcePillars;
extern char SceneId_CrossboneIsleKeys;
extern char SceneId_CrossboneIsleStatues;
extern char SceneId_CrossboneIsleFloodgate;
extern char SceneId_CrossboneIsleBane;
extern char SceneId_CrossboneIsleStackedPillars;
extern char SceneId_CrossboneIsleDragonStatue;
extern char SceneId_CrossboneIsleFloatingLogs;
extern char SceneId_CrossboneIslePier;
extern char SceneId_PirateShip;
extern char SceneId_CrossboneIsleFirstFloor;
extern char SceneId_CrossboneIsleSecondFloor;
extern char SceneId_CrossboneIsleThirdFloor;
extern char SceneId_CrossboneIsleFourthFloor;
extern char SceneId_CrossboneIsleFifthFloor;
extern char SceneId_CrossboneIsleSixthFloor;
extern char SceneId_CrossboneIsleSeventhFloor;
extern char SceneId_CrossboneIsleEighthFloor;
extern char SceneId_CrossboneIsleNinthFloor;
extern char SceneId_Tolbi;
/// A sheet of rooms drawn in stucco, all of them Tolbi's.
extern char SceneId_StuccoRoomsB;
/// A sheet of rooms drawn in stucco, both of them Tolbi's.
extern char SceneId_StuccoRoomsC;
extern char SceneId_BabiPalace;
extern char SceneId_BabiPalaceEntrance;
extern char SceneId_Colosseum;
extern char SceneId_ColosseumTunnel;
extern char SceneId_ColosseumEntrance;
extern char SceneId_ColosseumAzartMatch;
extern char SceneId_ColosseumSatrageMatch;
extern char SceneId_ColosseumNavampaMatch;
extern char SceneId_AltmillerCaveEntrance;
extern char SceneId_AltmillerCaveBabi;
extern char SceneId_AltmillerCaveVialChest;
extern char SceneId_AltmillerCaveSquall;
extern char SceneId_AltmillerCaveCookieChest;
extern char SceneId_AltmillerCaveDraught;
extern char SceneId_GondowanPassage;
extern char SceneId_KalayTunnelEntrance;
extern char SceneId_KalayTunnelCave;
extern char SceneId_KalayTunnelPassage;
extern char SceneId_KalayTunnelTreasureRoom;
extern char SceneId_GondowanCaveRiver;
extern char SceneId_GondowanCave;
extern char SceneId_LunpaFortressGates;
extern char SceneId_LunpaFortressGroundFloor;
extern char SceneId_LunpaFortressUpperFloor;
extern char SceneId_LunpaFortressCave;
extern char SceneId_LunpaFortressToadonpa;
extern char SceneId_SuhallaDesertNorth;
extern char SceneId_SuhallaDesertSouth;
extern char SceneId_SuhallaDesertCaves;
extern char SceneId_Suhalla;
/// A sheet of rooms drawn in timber, all of them Suhalla's.
extern char SceneId_TimberRoomsA;
extern char SceneId_SuhallaGateCheckpoint;
extern char SceneId_SuhallaGateDew;
extern char SceneId_GatewayCave;
extern char SceneId_TunnelRuinsGreatStatue;
extern char SceneId_TunnelRuinsLadyStatues;
extern char SceneId_BabiLighthouseStoneDoor;
extern char SceneId_BabiLighthouseLuff;
extern char SceneId_BabiLighthouseEntry;
extern char SceneId_RoadToBabiLighthouse;
extern char SceneId_Lalivero;
/// The first of two sheets of Lalivero's rooms, drawn in timber.
extern char SceneId_TimberRoomsB;
/// The second of two sheets of Lalivero's rooms, drawn in timber.
extern char SceneId_TimberRoomsC;
extern char SceneId_VenusLighthouseEntry;
extern char SceneId_VenusLighthouseGroundFloor;
extern char SceneId_VenusLighthouseFirstFloor;
extern char SceneId_VenusLighthouseSecondFloor;
extern char SceneId_VenusLighthouseThirdFloor;
extern char SceneId_VenusLighthouseFourthFloor;
extern char SceneId_VenusLighthouseAerie;
extern char SceneId_LemurianShip;
extern char SceneId_TolbiSpring;
extern char SceneId_BattleArena;
extern char SceneId_Test0;
extern char SceneId_Test1;
extern char SceneId_Test2;
extern char SceneId_Test3;
extern char SceneId_Test4;
extern char SceneId_Test5;
extern char SceneId_Test6;
extern char SceneId_Test7;
extern char SceneId_IncreaseItemAndLevel;
extern char SceneId_MenuTest;

#endif
