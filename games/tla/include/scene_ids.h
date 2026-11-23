#ifndef ALCHEMY_SCENE_IDS_H
#define ALCHEMY_SCENE_IDS_H

/// @file
/// Scene numbers, named after their places, in the order of gScenes
/// (field/scenes.s).
///
/// Each SceneId_ is an absolute symbol whose value is the scene's row in the
/// table; C names the number by its macro, SCENE_DAILA_FLOODED for
/// SceneId_DailaFlooded, and the exit tables name a scene by its place
/// (SCENE_EXIT).

#include "types.h"

extern char SceneId_Title;
#define SCENE_TITLE ((s32)&SceneId_Title)
extern char SceneId_WorldMap;
#define SCENE_WORLD_MAP ((s32)&SceneId_WorldMap)
extern char SceneId_StaffCredit;
#define SCENE_STAFF_CREDIT ((s32)&SceneId_StaffCredit)
extern char SceneId_Clear;
#define SCENE_CLEAR ((s32)&SceneId_Clear)
extern char SceneId_VenusLighthouseEntrance;
#define SCENE_VENUS_LIGHTHOUSE_ENTRANCE ((s32)&SceneId_VenusLighthouseEntrance)
extern char SceneId_VenusLighthouse;
#define SCENE_VENUS_LIGHTHOUSE ((s32)&SceneId_VenusLighthouse)
extern char SceneId_SuhallaGateRoad;
#define SCENE_SUHALLA_GATE_ROAD ((s32)&SceneId_SuhallaGateRoad)
extern char SceneId_SuhallaGateAmbush;
#define SCENE_SUHALLA_GATE_AMBUSH ((s32)&SceneId_SuhallaGateAmbush)
extern char SceneId_SuhallaGatePsynergyStone;
#define SCENE_SUHALLA_GATE_PSYNERGY_STONE                                      \
  ((s32)&SceneId_SuhallaGatePsynergyStone)
extern char SceneId_Idejima;
#define SCENE_IDEJIMA ((s32)&SceneId_Idejima)
extern char SceneId_DailaRoomsHouses;
#define SCENE_DAILA_ROOMS_HOUSES ((s32)&SceneId_DailaRoomsHouses)
extern char SceneId_DailaRoomsSanctum;
#define SCENE_DAILA_ROOMS_SANCTUM ((s32)&SceneId_DailaRoomsSanctum)
extern char SceneId_DailaFlooded;
#define SCENE_DAILA_FLOODED ((s32)&SceneId_DailaFlooded)
extern char SceneId_Daila;
#define SCENE_DAILA ((s32)&SceneId_Daila)
/// The Kandorean Temple's room of the east and west lifts.
extern char SceneId_KandoreanTempleLifts;
#define SCENE_KANDOREAN_TEMPLE_LIFTS ((s32)&SceneId_KandoreanTempleLifts)
extern char SceneId_KandoreanTempleMimic;
#define SCENE_KANDOREAN_TEMPLE_MIMIC ((s32)&SceneId_KandoreanTempleMimic)
/// The Kandorean Temple room where the Chestbeaters are fought.
extern char SceneId_KandoreanTempleChestbeaters;
#define SCENE_KANDOREAN_TEMPLE_CHESTBEATERS                                    \
  ((s32)&SceneId_KandoreanTempleChestbeaters)
extern char SceneId_KandoreanTemplePillar;
#define SCENE_KANDOREAN_TEMPLE_PILLAR ((s32)&SceneId_KandoreanTemplePillar)
extern char SceneId_KandoreanTempleStream;
#define SCENE_KANDOREAN_TEMPLE_STREAM ((s32)&SceneId_KandoreanTempleStream)
extern char SceneId_KandoreanTempleCurrent;
#define SCENE_KANDOREAN_TEMPLE_CURRENT ((s32)&SceneId_KandoreanTempleCurrent)
/// The Kandorean Temple room where Fog is fought.
extern char SceneId_KandoreanTempleFog;
#define SCENE_KANDOREAN_TEMPLE_FOG ((s32)&SceneId_KandoreanTempleFog)
/// The Kandorean Temple's gate, where its students and guard stand.
extern char SceneId_KandoreanTempleGate;
#define SCENE_KANDOREAN_TEMPLE_GATE ((s32)&SceneId_KandoreanTempleGate)
/// Poi's hall, where Lash is received.
extern char SceneId_KandoreanTemplePoi;
#define SCENE_KANDOREAN_TEMPLE_POI ((s32)&SceneId_KandoreanTemplePoi)
extern char SceneId_YampiDesertGuardianRingChest;
#define SCENE_YAMPI_DESERT_GUARDIAN_RING_CHEST                                 \
  ((s32)&SceneId_YampiDesertGuardianRingChest)
extern char SceneId_YampiDesertSpring;
#define SCENE_YAMPI_DESERT_SPRING ((s32)&SceneId_YampiDesertSpring)
/// The Yampi Desert scene where Blitz is fought.
extern char SceneId_YampiDesertBlitz;
#define SCENE_YAMPI_DESERT_BLITZ ((s32)&SceneId_YampiDesertBlitz)
extern char SceneId_YampiDesertPsynergyStone;
#define SCENE_YAMPI_DESERT_PSYNERGY_STONE                                      \
  ((s32)&SceneId_YampiDesertPsynergyStone)
extern char SceneId_YampiDesertKingScorpion;
#define SCENE_YAMPI_DESERT_KING_SCORPION ((s32)&SceneId_YampiDesertKingScorpion)
extern char SceneId_YampiDesertCoinsPuddle;
#define SCENE_YAMPI_DESERT_COINS_PUDDLE ((s32)&SceneId_YampiDesertCoinsPuddle)
extern char SceneId_YampiDesertSandPass;
#define SCENE_YAMPI_DESERT_SAND_PASS ((s32)&SceneId_YampiDesertSandPass)
extern char SceneId_YampiDesertTrainersWhipChest;
#define SCENE_YAMPI_DESERT_TRAINERS_WHIP_CHEST                                 \
  ((s32)&SceneId_YampiDesertTrainersWhipChest)
extern char SceneId_YampiDesertHiddenPsynergyStone;
#define SCENE_YAMPI_DESERT_HIDDEN_PSYNERGY_STONE                               \
  ((s32)&SceneId_YampiDesertHiddenPsynergyStone)
extern char SceneId_YampiDesertScorpionPit;
#define SCENE_YAMPI_DESERT_SCORPION_PIT ((s32)&SceneId_YampiDesertScorpionPit)
extern char SceneId_YampiDesertSpringCave;
#define SCENE_YAMPI_DESERT_SPRING_CAVE ((s32)&SceneId_YampiDesertSpringCave)
extern char SceneId_YampiDesertBlowMaceChest;
#define SCENE_YAMPI_DESERT_BLOW_MACE_CHEST                                     \
  ((s32)&SceneId_YampiDesertBlowMaceChest)
extern char SceneId_YampiDesertCaveWaterOfLifeChest;
#define SCENE_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST                            \
  ((s32)&SceneId_YampiDesertCaveWaterOfLifeChest)
extern char SceneId_YampiDesertCaveGuardian;
#define SCENE_YAMPI_DESERT_CAVE_GUARDIAN ((s32)&SceneId_YampiDesertCaveGuardian)
/// The Yampi Desert cave room where Crystal is fought.
extern char SceneId_YampiDesertCaveCrystal;
#define SCENE_YAMPI_DESERT_CAVE_CRYSTAL ((s32)&SceneId_YampiDesertCaveCrystal)
extern char SceneId_AirsRockStormBrandChest;
#define SCENE_AIRS_ROCK_STORM_BRAND_CHEST                                      \
  ((s32)&SceneId_AirsRockStormBrandChest)
extern char SceneId_AirsRockEntrance;
#define SCENE_AIRS_ROCK_ENTRANCE ((s32)&SceneId_AirsRockEntrance)
extern char SceneId_AirsRockFujinShieldChest;
#define SCENE_AIRS_ROCK_FUJIN_SHIELD_CHEST                                     \
  ((s32)&SceneId_AirsRockFujinShieldChest)
extern char SceneId_AirsRockInsideHall;
#define SCENE_AIRS_ROCK_INSIDE_HALL ((s32)&SceneId_AirsRockInsideHall)
extern char SceneId_AirsRockInsideVialChest;
#define SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST                                      \
  ((s32)&SceneId_AirsRockInsideVialChest)
extern char SceneId_AirsRockInsideClarityCircletChest;
#define SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST                           \
  ((s32)&SceneId_AirsRockInsideClarityCircletChest)
extern char SceneId_AirsRockInsideTightropes;
#define SCENE_AIRS_ROCK_INSIDE_TIGHTROPES                                      \
  ((s32)&SceneId_AirsRockInsideTightropes)
/// The room inside Air's Rock with the tablet that teaches Reveal.
extern char SceneId_AirsRockInsideRevealTablet;
#define SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET                                   \
  ((s32)&SceneId_AirsRockInsideRevealTablet)
extern char SceneId_AirsRockInsideElixirChest;
#define SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST                                    \
  ((s32)&SceneId_AirsRockInsideElixirChest)
extern char SceneId_AirsRockInsidePairedStakes;
#define SCENE_AIRS_ROCK_INSIDE_PAIRED_STAKES                                   \
  ((s32)&SceneId_AirsRockInsidePairedStakes)
extern char SceneId_AirsRockInsideFaceVents;
#define SCENE_AIRS_ROCK_INSIDE_FACE_VENTS                                      \
  ((s32)&SceneId_AirsRockInsideFaceVents)
extern char SceneId_AirsRockInsideCoinsChest;
#define SCENE_AIRS_ROCK_INSIDE_COINS_CHEST                                     \
  ((s32)&SceneId_AirsRockInsideCoinsChest)
/// The room inside Air's Rock with a Psynergy Stone.
extern char SceneId_AirsRockInsidePsynergyStone;
#define SCENE_AIRS_ROCK_INSIDE_PSYNERGY_STONE                                  \
  ((s32)&SceneId_AirsRockInsidePsynergyStone)
/// Alhafra itself, the town's one outdoor scene.
extern char SceneId_Alhafra;
#define SCENE_ALHAFRA ((s32)&SceneId_Alhafra)
extern char SceneId_AlhafraRooms;
#define SCENE_ALHAFRA_ROOMS ((s32)&SceneId_AlhafraRooms)
extern char SceneId_SeaOfTimeSwirls;
#define SCENE_SEA_OF_TIME_SWIRLS ((s32)&SceneId_SeaOfTimeSwirls)
extern char SceneId_SeaOfTimeToLemuria;
#define SCENE_SEA_OF_TIME_TO_LEMURIA ((s32)&SceneId_SeaOfTimeToLemuria)
extern char SceneId_LemuriaEntrance;
#define SCENE_LEMURIA_ENTRANCE ((s32)&SceneId_LemuriaEntrance)
extern char SceneId_LemuriaDock;
#define SCENE_LEMURIA_DOCK ((s32)&SceneId_LemuriaDock)
extern char SceneId_AncientLemuria;
#define SCENE_ANCIENT_LEMURIA ((s32)&SceneId_AncientLemuria)
extern char SceneId_MadraRoomsHouses;
#define SCENE_MADRA_ROOMS_HOUSES ((s32)&SceneId_MadraRoomsHouses)
extern char SceneId_MadraRoomsCavernHomes;
#define SCENE_MADRA_ROOMS_CAVERN_HOMES ((s32)&SceneId_MadraRoomsCavernHomes)
extern char SceneId_MadraCatacombsEntrance;
#define SCENE_MADRA_CATACOMBS_ENTRANCE ((s32)&SceneId_MadraCatacombsEntrance)
extern char SceneId_MadraCatacombsLadders;
#define SCENE_MADRA_CATACOMBS_LADDERS ((s32)&SceneId_MadraCatacombsLadders)
extern char SceneId_MadraCatacombsChampaTunnel;
#define SCENE_MADRA_CATACOMBS_CHAMPA_TUNNEL                                    \
  ((s32)&SceneId_MadraCatacombsChampaTunnel)
extern char SceneId_MadraCatacombsStoneDoor;
#define SCENE_MADRA_CATACOMBS_STONE_DOOR ((s32)&SceneId_MadraCatacombsStoneDoor)
extern char SceneId_MadraCatacombsBookshelf;
#define SCENE_MADRA_CATACOMBS_BOOKSHELF ((s32)&SceneId_MadraCatacombsBookshelf)
extern char SceneId_MadraCatacombsTablet;
#define SCENE_MADRA_CATACOMBS_TABLET ((s32)&SceneId_MadraCatacombsTablet)
extern char SceneId_MadraDrawbridge;
#define SCENE_MADRA_DRAWBRIDGE ((s32)&SceneId_MadraDrawbridge)
extern char SceneId_OseniaCliffs;
#define SCENE_OSENIA_CLIFFS ((s32)&SceneId_OseniaCliffs)
extern char SceneId_GondowanCliffs;
#define SCENE_GONDOWAN_CLIFFS ((s32)&SceneId_GondowanCliffs)
extern char SceneId_GondowanCliffsPass;
#define SCENE_GONDOWAN_CLIFFS_PASS ((s32)&SceneId_GondowanCliffsPass)
extern char SceneId_GondowanCliffsHideout;
#define SCENE_GONDOWAN_CLIFFS_HIDEOUT ((s32)&SceneId_GondowanCliffsHideout)
extern char SceneId_Naribwe;
#define SCENE_NARIBWE ((s32)&SceneId_Naribwe)
extern char SceneId_NaribweRooms;
#define SCENE_NARIBWE_ROOMS ((s32)&SceneId_NaribweRooms)
extern char SceneId_MikasallaRoomsHouses;
#define SCENE_MIKASALLA_ROOMS_HOUSES ((s32)&SceneId_MikasallaRoomsHouses)
extern char SceneId_MikasallaRoomsSanctum;
#define SCENE_MIKASALLA_ROOMS_SANCTUM ((s32)&SceneId_MikasallaRoomsSanctum)
extern char SceneId_Garoh;
#define SCENE_GAROH ((s32)&SceneId_Garoh)
extern char SceneId_GarohPath;
#define SCENE_GAROH_PATH ((s32)&SceneId_GarohPath)
extern char SceneId_GarohWayUp;
#define SCENE_GAROH_WAY_UP ((s32)&SceneId_GarohWayUp)
extern char SceneId_GarohTunnels;
#define SCENE_GAROH_TUNNELS ((s32)&SceneId_GarohTunnels)
extern char SceneId_GarohWerewolfCave;
#define SCENE_GAROH_WEREWOLF_CAVE ((s32)&SceneId_GarohWerewolfCave)
extern char SceneId_AlhafraManor;
#define SCENE_ALHAFRA_MANOR ((s32)&SceneId_AlhafraManor)
/// Eastern Alhafra, where Briggs's ship lies.
extern char SceneId_EasternAlhafra;
#define SCENE_EASTERN_ALHAFRA ((s32)&SceneId_EasternAlhafra)
extern char SceneId_EasternAlhafraShip;
#define SCENE_EASTERN_ALHAFRA_SHIP ((s32)&SceneId_EasternAlhafraShip)
extern char SceneId_AlhafranCave;
#define SCENE_ALHAFRAN_CAVE ((s32)&SceneId_AlhafranCave)
extern char SceneId_AlhafraJail;
#define SCENE_ALHAFRA_JAIL ((s32)&SceneId_AlhafraJail)
extern char SceneId_ChampaRooms;
#define SCENE_CHAMPA_ROOMS ((s32)&SceneId_ChampaRooms)
extern char SceneId_ChampaSanctum;
#define SCENE_CHAMPA_SANCTUM ((s32)&SceneId_ChampaSanctum)
extern char SceneId_Contigo;
#define SCENE_CONTIGO ((s32)&SceneId_Contigo)
extern char SceneId_ContigoShops;
#define SCENE_CONTIGO_SHOPS ((s32)&SceneId_ContigoShops)
extern char SceneId_ContigoLuckyDice;
#define SCENE_CONTIGO_LUCKY_DICE ((s32)&SceneId_ContigoLuckyDice)
extern char SceneId_ContigoGeoglyph;
#define SCENE_CONTIGO_GEOGLYPH ((s32)&SceneId_ContigoGeoglyph)
extern char SceneId_GarohRoomsHouses;
#define SCENE_GAROH_ROOMS_HOUSES ((s32)&SceneId_GarohRoomsHouses)
extern char SceneId_GarohRoomsShop;
#define SCENE_GAROH_ROOMS_SHOP ((s32)&SceneId_GarohRoomsShop)
extern char SceneId_GarohRoomsSanctum;
#define SCENE_GAROH_ROOMS_SANCTUM ((s32)&SceneId_GarohRoomsSanctum)
extern char SceneId_GabombaStatueBoneArmletChest;
#define SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST                                 \
  ((s32)&SceneId_GabombaStatueBoneArmletChest)
extern char SceneId_GabombaStatueElixirChest;
#define SCENE_GABOMBA_STATUE_ELIXIR_CHEST                                      \
  ((s32)&SceneId_GabombaStatueElixirChest)
/// The room inside the Gabomba Statue where Steel is fought.
extern char SceneId_GabombaStatueSteel;
#define SCENE_GABOMBA_STATUE_STEEL ((s32)&SceneId_GabombaStatueSteel)
extern char SceneId_GabombaStatueMouse;
#define SCENE_GABOMBA_STATUE_MOUSE ((s32)&SceneId_GabombaStatueMouse)
extern char SceneId_GabombaStatueCogs;
#define SCENE_GABOMBA_STATUE_COGS ((s32)&SceneId_GabombaStatueCogs)
extern char SceneId_GabombaStatueArrows;
#define SCENE_GABOMBA_STATUE_ARROWS ((s32)&SceneId_GabombaStatueArrows)
extern char SceneId_GabombaStatuePinion;
#define SCENE_GABOMBA_STATUE_PINION ((s32)&SceneId_GabombaStatuePinion)
extern char SceneId_AnemosInnerSanctumDragonSkinChest;
#define SCENE_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST                           \
  ((s32)&SceneId_AnemosInnerSanctumDragonSkinChest)
extern char SceneId_AnemosInnerSanctumElevator;
#define SCENE_ANEMOS_INNER_SANCTUM_ELEVATOR                                    \
  ((s32)&SceneId_AnemosInnerSanctumElevator)
extern char SceneId_AnemosInnerSanctumSwitches;
#define SCENE_ANEMOS_INNER_SANCTUM_SWITCHES                                    \
  ((s32)&SceneId_AnemosInnerSanctumSwitches)
extern char SceneId_AnemosInnerSanctumOrihalconChest;
#define SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST                             \
  ((s32)&SceneId_AnemosInnerSanctumOrihalconChest)
extern char SceneId_AnemosInnerSanctumGuardian;
#define SCENE_ANEMOS_INNER_SANCTUM_GUARDIAN                                    \
  ((s32)&SceneId_AnemosInnerSanctumGuardian)
extern char SceneId_GaiaRockSusa;
#define SCENE_GAIA_ROCK_SUSA ((s32)&SceneId_GaiaRockSusa)
extern char SceneId_GaiaRockStatueSpots;
#define SCENE_GAIA_ROCK_STATUE_SPOTS ((s32)&SceneId_GaiaRockStatueSpots)
extern char SceneId_GaiaRockDancingIdol;
#define SCENE_GAIA_ROCK_DANCING_IDOL ((s32)&SceneId_GaiaRockDancingIdol)
extern char SceneId_GaiaRockInsideMaze;
#define SCENE_GAIA_ROCK_INSIDE_MAZE ((s32)&SceneId_GaiaRockInsideMaze)
extern char SceneId_GaiaRockInsideFirstAltar;
#define SCENE_GAIA_ROCK_INSIDE_FIRST_ALTAR                                     \
  ((s32)&SceneId_GaiaRockInsideFirstAltar)
extern char SceneId_GaiaRockInsideTwinAltars;
#define SCENE_GAIA_ROCK_INSIDE_TWIN_ALTARS                                     \
  ((s32)&SceneId_GaiaRockInsideTwinAltars)
extern char SceneId_GaiaRockInsidePlatforms;
#define SCENE_GAIA_ROCK_INSIDE_PLATFORMS ((s32)&SceneId_GaiaRockInsidePlatforms)
extern char SceneId_GaiaRockInsideAltar;
#define SCENE_GAIA_ROCK_INSIDE_ALTAR ((s32)&SceneId_GaiaRockInsideAltar)
/// Champa itself, the town's one outdoor scene.
extern char SceneId_Champa;
#define SCENE_CHAMPA ((s32)&SceneId_Champa)
extern char SceneId_MagmaRockSlide;
#define SCENE_MAGMA_ROCK_SLIDE ((s32)&SceneId_MagmaRockSlide)
extern char SceneId_MagmaRockSalamanderTailChest;
#define SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST                                 \
  ((s32)&SceneId_MagmaRockSalamanderTailChest)
extern char SceneId_MagmaRockStatue;
#define SCENE_MAGMA_ROCK_STATUE ((s32)&SceneId_MagmaRockStatue)
extern char SceneId_MagmaRockInsideLuckyMedalChest;
#define SCENE_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST                              \
  ((s32)&SceneId_MagmaRockInsideLuckyMedalChest)
extern char SceneId_MagmaRockInsideEntrance;
#define SCENE_MAGMA_ROCK_INSIDE_ENTRANCE ((s32)&SceneId_MagmaRockInsideEntrance)
/// The room inside Magma Rock where Fury is fought.
extern char SceneId_MagmaRockInsideFury;
#define SCENE_MAGMA_ROCK_INSIDE_FURY ((s32)&SceneId_MagmaRockInsideFury)
extern char SceneId_MagmaRockInsideCrackedPillars;
#define SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS                                \
  ((s32)&SceneId_MagmaRockInsideCrackedPillars)
extern char SceneId_MagmaRockInsideGolemCoreChest;
#define SCENE_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST                               \
  ((s32)&SceneId_MagmaRockInsideGolemCoreChest)
extern char SceneId_MagmaRockInsideSalamanderTailChest;
#define SCENE_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST                          \
  ((s32)&SceneId_MagmaRockInsideSalamanderTailChest)
extern char SceneId_MagmaRockInsideEmberStone;
#define SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE                                    \
  ((s32)&SceneId_MagmaRockInsideEmberStone)
/// The room inside Magma Rock with the tablet that teaches Blaze.
extern char SceneId_MagmaRockInsideBlazeTablet;
#define SCENE_MAGMA_ROCK_INSIDE_BLAZE_TABLET                                   \
  ((s32)&SceneId_MagmaRockInsideBlazeTablet)
extern char SceneId_TundariaTowerEntrance;
#define SCENE_TUNDARIA_TOWER_ENTRANCE ((s32)&SceneId_TundariaTowerEntrance)
extern char SceneId_TundariaTowerCenterProng;
#define SCENE_TUNDARIA_TOWER_CENTER_PRONG                                      \
  ((s32)&SceneId_TundariaTowerCenterProng)
extern char SceneId_TundariaTowerIceSlide;
#define SCENE_TUNDARIA_TOWER_ICE_SLIDE ((s32)&SceneId_TundariaTowerIceSlide)
/// The Tundaria Tower room where Reflux is fought.
extern char SceneId_TundariaTowerReflux;
#define SCENE_TUNDARIA_TOWER_REFLUX ((s32)&SceneId_TundariaTowerReflux)
extern char SceneId_TundariaTowerIceColumns;
#define SCENE_TUNDARIA_TOWER_ICE_COLUMNS ((s32)&SceneId_TundariaTowerIceColumns)
extern char SceneId_TundariaTowerPillarPuzzle;
#define SCENE_TUNDARIA_TOWER_PILLAR_PUZZLE                                     \
  ((s32)&SceneId_TundariaTowerPillarPuzzle)
extern char SceneId_TundariaTowerBurstBrooch;
#define SCENE_TUNDARIA_TOWER_BURST_BROOCH                                      \
  ((s32)&SceneId_TundariaTowerBurstBrooch)
extern char SceneId_GondowanSettlement;
#define SCENE_GONDOWAN_SETTLEMENT ((s32)&SceneId_GondowanSettlement)
extern char SceneId_GondowanSettlementHouses;
#define SCENE_GONDOWAN_SETTLEMENT_HOUSES                                       \
  ((s32)&SceneId_GondowanSettlementHouses)
extern char SceneId_ShrineOfTheSeaGodRope;
#define SCENE_SHRINE_OF_THE_SEA_GOD_ROPE ((s32)&SceneId_ShrineOfTheSeaGodRope)
extern char SceneId_ShrineOfTheSeaGodPlankBridge;
#define SCENE_SHRINE_OF_THE_SEA_GOD_PLANK_BRIDGE                               \
  ((s32)&SceneId_ShrineOfTheSeaGodPlankBridge)
extern char SceneId_ShrineOfTheSeaGodCrossroads;
#define SCENE_SHRINE_OF_THE_SEA_GOD_CROSSROADS                                 \
  ((s32)&SceneId_ShrineOfTheSeaGodCrossroads)
extern char SceneId_ShrineOfTheSeaGodFootprints;
#define SCENE_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS                                 \
  ((s32)&SceneId_ShrineOfTheSeaGodFootprints)
/// The Shrine of the Sea God room where Breath is chased and fought.
extern char SceneId_ShrineOfTheSeaGodBreath;
#define SCENE_SHRINE_OF_THE_SEA_GOD_BREATH                                     \
  ((s32)&SceneId_ShrineOfTheSeaGodBreath)
extern char SceneId_ShrineOfTheSeaGodPillarHole;
#define SCENE_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE                                \
  ((s32)&SceneId_ShrineOfTheSeaGodPillarHole)
extern char SceneId_ShrineOfTheSeaGodPillars;
#define SCENE_SHRINE_OF_THE_SEA_GOD_PILLARS                                    \
  ((s32)&SceneId_ShrineOfTheSeaGodPillars)
extern char SceneId_ShrineOfTheSeaGodSeaGodsTear;
#define SCENE_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR                              \
  ((s32)&SceneId_ShrineOfTheSeaGodSeaGodsTear)
extern char SceneId_ShrineOfTheSeaGodStatue;
#define SCENE_SHRINE_OF_THE_SEA_GOD_STATUE                                     \
  ((s32)&SceneId_ShrineOfTheSeaGodStatue)
extern char SceneId_ShrineOfTheSeaGodLadders;
#define SCENE_SHRINE_OF_THE_SEA_GOD_LADDERS                                    \
  ((s32)&SceneId_ShrineOfTheSeaGodLadders)
extern char SceneId_ShrineOfTheSeaGodRightProng;
#define SCENE_SHRINE_OF_THE_SEA_GOD_RIGHT_PRONG                                \
  ((s32)&SceneId_ShrineOfTheSeaGodRightProng)
extern char SceneId_AquaRockMoai;
#define SCENE_AQUA_ROCK_MOAI ((s32)&SceneId_AquaRockMoai)
extern char SceneId_AquaRockMistSabreChest;
#define SCENE_AQUA_ROCK_MIST_SABRE_CHEST ((s32)&SceneId_AquaRockMistSabreChest)
extern char SceneId_DehkanPlateauFullMetalVestChest;
#define SCENE_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST                             \
  ((s32)&SceneId_DehkanPlateauFullMetalVestChest)
extern char SceneId_DehkanPlateauThemisAxeChest;
#define SCENE_DEHKAN_PLATEAU_THEMIS_AXE_CHEST                                  \
  ((s32)&SceneId_DehkanPlateauThemisAxeChest)
extern char SceneId_DehkanPlateauTightropes;
#define SCENE_DEHKAN_PLATEAU_TIGHTROPES ((s32)&SceneId_DehkanPlateauTightropes)
extern char SceneId_DehkanPlateauMintChest;
#define SCENE_DEHKAN_PLATEAU_MINT_CHEST ((s32)&SceneId_DehkanPlateauMintChest)
extern char SceneId_DehkanPlateauPoundCube;
#define SCENE_DEHKAN_PLATEAU_POUND_CUBE ((s32)&SceneId_DehkanPlateauPoundCube)
/// The Dehkan Plateau scene where Cannon is met and fought.
extern char SceneId_DehkanPlateauCannon;
#define SCENE_DEHKAN_PLATEAU_CANNON ((s32)&SceneId_DehkanPlateauCannon)
extern char SceneId_LemurianShipTiller;
#define SCENE_LEMURIAN_SHIP_TILLER ((s32)&SceneId_LemurianShipTiller)
extern char SceneId_LemurianShipMainDeck;
#define SCENE_LEMURIAN_SHIP_MAIN_DECK ((s32)&SceneId_LemurianShipMainDeck)
extern char SceneId_Madra;
#define SCENE_MADRA ((s32)&SceneId_Madra)
/// The Kibombo Mountains scene where Waft is fought.
extern char SceneId_KibomboMountainsWaft;
#define SCENE_KIBOMBO_MOUNTAINS_WAFT ((s32)&SceneId_KibomboMountainsWaft)
extern char SceneId_KibomboMountainsDiskAxeChest;
#define SCENE_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST                                 \
  ((s32)&SceneId_KibomboMountainsDiskAxeChest)
extern char SceneId_KibomboMountainsCavern;
#define SCENE_KIBOMBO_MOUNTAINS_CAVERN ((s32)&SceneId_KibomboMountainsCavern)
/// The Islet Cave room where Meld is fought.
extern char SceneId_IsletCaveHalls;
#define SCENE_ISLET_CAVE_HALLS ((s32)&SceneId_IsletCaveHalls)
/// The Islet Cave room where Serac is fought.
extern char SceneId_IsletCaveCorridors;
#define SCENE_ISLET_CAVE_CORRIDORS ((s32)&SceneId_IsletCaveCorridors)
/// The Islet Cave room where the Sentinel waits.
extern char SceneId_IsletCaveSentinel;
#define SCENE_ISLET_CAVE_SENTINEL ((s32)&SceneId_IsletCaveSentinel)
extern char SceneId_Yallam;
#define SCENE_YALLAM ((s32)&SceneId_Yallam)
extern char SceneId_YallamCave;
#define SCENE_YALLAM_CAVE ((s32)&SceneId_YallamCave)
extern char SceneId_ApojiiIslands;
#define SCENE_APOJII_ISLANDS ((s32)&SceneId_ApojiiIslands)
/// The Apojii Islands scene where Haze joins the party.
extern char SceneId_ApojiiIslandsHaze;
#define SCENE_APOJII_ISLANDS_HAZE ((s32)&SceneId_ApojiiIslandsHaze)
extern char SceneId_ApojiiIslandsShops;
#define SCENE_APOJII_ISLANDS_SHOPS ((s32)&SceneId_ApojiiIslandsShops)
extern char SceneId_ApojiiIslandsCave;
#define SCENE_APOJII_ISLANDS_CAVE ((s32)&SceneId_ApojiiIslandsCave)
extern char SceneId_AquaRockInsideWaterOfLifeChest;
#define SCENE_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST                             \
  ((s32)&SceneId_AquaRockInsideWaterOfLifeChest)
/// The room inside Aqua Rock with the tablet that teaches Parch.
extern char SceneId_AquaRockInsideParchTablet;
#define SCENE_AQUA_ROCK_INSIDE_PARCH_TABLET                                    \
  ((s32)&SceneId_AquaRockInsideParchTablet)
extern char SceneId_IzumoRooms;
#define SCENE_IZUMO_ROOMS ((s32)&SceneId_IzumoRooms)
extern char SceneId_Izumo;
#define SCENE_IZUMO ((s32)&SceneId_Izumo)
extern char SceneId_IzumoRuinsFirstRoom;
#define SCENE_IZUMO_RUINS_FIRST_ROOM ((s32)&SceneId_IzumoRuinsFirstRoom)
extern char SceneId_IzumoRuinsSecondRoom;
#define SCENE_IZUMO_RUINS_SECOND_ROOM ((s32)&SceneId_IzumoRuinsSecondRoom)
/// The room inside Aqua Rock where Steam is fought.
extern char SceneId_AquaRockInsideSteam;
#define SCENE_AQUA_ROCK_INSIDE_STEAM ((s32)&SceneId_AquaRockInsideSteam)
extern char SceneId_AquaRockInsideAquariusStone;
#define SCENE_AQUA_ROCK_INSIDE_AQUARIUS_STONE                                  \
  ((s32)&SceneId_AquaRockInsideAquariusStone)
extern char SceneId_AquaRockInsideRustySwordChest;
#define SCENE_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST                               \
  ((s32)&SceneId_AquaRockInsideRustySwordChest)
extern char SceneId_AquaRockInsideCrystalPowderChest;
#define SCENE_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST                            \
  ((s32)&SceneId_AquaRockInsideCrystalPowderChest)
extern char SceneId_AquaRockInsideVialChest;
#define SCENE_AQUA_ROCK_INSIDE_VIAL_CHEST                                      \
  ((s32)&SceneId_AquaRockInsideVialChest)
/// The room inside Aqua Rock with a Psynergy Stone.
extern char SceneId_AquaRockInsidePsynergyStone;
#define SCENE_AQUA_ROCK_INSIDE_PSYNERGY_STONE                                  \
  ((s32)&SceneId_AquaRockInsidePsynergyStone)
extern char SceneId_AquaRockInsideTightrope;
#define SCENE_AQUA_ROCK_INSIDE_TIGHTROPE ((s32)&SceneId_AquaRockInsideTightrope)
extern char SceneId_AquaRockInsideTearStone;
#define SCENE_AQUA_ROCK_INSIDE_TEAR_STONE                                      \
  ((s32)&SceneId_AquaRockInsideTearStone)
extern char SceneId_AttekaInlet;
#define SCENE_ATTEKA_INLET ((s32)&SceneId_AttekaInlet)
/// The Atteka Inlet scene where Cyclone uncovers Geode.
extern char SceneId_AttekaInletGeode;
#define SCENE_ATTEKA_INLET_GEODE ((s32)&SceneId_AttekaInletGeode)
extern char SceneId_ProxSouthGate;
#define SCENE_PROX_SOUTH_GATE ((s32)&SceneId_ProxSouthGate)
extern char SceneId_Prox;
#define SCENE_PROX ((s32)&SceneId_Prox)
extern char SceneId_ProxMold;
#define SCENE_PROX_MOLD ((s32)&SceneId_ProxMold)
extern char SceneId_ProxHouses;
#define SCENE_PROX_HOUSES ((s32)&SceneId_ProxHouses)
extern char SceneId_ProxCaveHomes;
#define SCENE_PROX_CAVE_HOMES ((s32)&SceneId_ProxCaveHomes)
extern char SceneId_ProxCaves;
#define SCENE_PROX_CAVES ((s32)&SceneId_ProxCaves)
extern char SceneId_CavernsIndra;
#define SCENE_CAVERNS_INDRA ((s32)&SceneId_CavernsIndra)
extern char SceneId_CavernsOsenia;
#define SCENE_CAVERNS_OSENIA ((s32)&SceneId_CavernsOsenia)
extern char SceneId_CavernsAtteka;
#define SCENE_CAVERNS_ATTEKA ((s32)&SceneId_CavernsAtteka)
extern char SceneId_CavernsAngara;
#define SCENE_CAVERNS_ANGARA ((s32)&SceneId_CavernsAngara)
extern char SceneId_MarsLighthouseOutside;
#define SCENE_MARS_LIGHTHOUSE_OUTSIDE ((s32)&SceneId_MarsLighthouseOutside)
/// The Mars Lighthouse room where Fugue is fought.
extern char SceneId_MarsLighthouseEntranceFugue;
#define SCENE_MARS_LIGHTHOUSE_ENTRANCE_FUGUE                                   \
  ((s32)&SceneId_MarsLighthouseEntranceFugue)
extern char SceneId_MarsLighthouseEntranceTeleportLapis;
#define SCENE_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS                          \
  ((s32)&SceneId_MarsLighthouseEntranceTeleportLapis)
/// The Mars Lighthouse room where Balm is fought.
extern char SceneId_MarsLighthouseEntranceBalm;
#define SCENE_MARS_LIGHTHOUSE_ENTRANCE_BALM                                    \
  ((s32)&SceneId_MarsLighthouseEntranceBalm)
extern char SceneId_MarsLighthouseEntranceHiddenCircle;
#define SCENE_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE                           \
  ((s32)&SceneId_MarsLighthouseEntranceHiddenCircle)
extern char SceneId_MarsLighthouseEntranceValkyrieMailChest;
#define SCENE_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST                     \
  ((s32)&SceneId_MarsLighthouseEntranceValkyrieMailChest)
extern char SceneId_MarsLighthouseEntranceSolBladeChest;
#define SCENE_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST                         \
  ((s32)&SceneId_MarsLighthouseEntranceSolBladeChest)
/// Mars Lighthouse's room of the Mars Star.
extern char SceneId_MarsLighthouseStarRoom;
#define SCENE_MARS_LIGHTHOUSE_STAR_ROOM ((s32)&SceneId_MarsLighthouseStarRoom)
extern char SceneId_MarsLighthouseRoomMInscriptions;
#define SCENE_MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS                              \
  ((s32)&SceneId_MarsLighthouseRoomMInscriptions)
extern char SceneId_MarsLighthouseMercuryLedge;
#define SCENE_MARS_LIGHTHOUSE_MERCURY_LEDGE                                    \
  ((s32)&SceneId_MarsLighthouseMercuryLedge)
extern char SceneId_MarsLighthouseJupiterLedge;
#define SCENE_MARS_LIGHTHOUSE_JUPITER_LEDGE                                    \
  ((s32)&SceneId_MarsLighthouseJupiterLedge)
extern char SceneId_MarsLighthouseRoomMAlastorsHoodChest;
#define SCENE_MARS_LIGHTHOUSE_ROOM_M_ALASTORS_HOOD_CHEST                       \
  ((s32)&SceneId_MarsLighthouseRoomMAlastorsHoodChest)
extern char SceneId_MarsLighthouseVenusLedge;
#define SCENE_MARS_LIGHTHOUSE_VENUS_LEDGE                                      \
  ((s32)&SceneId_MarsLighthouseVenusLedge)
extern char SceneId_MarsLighthouseMarsTower;
#define SCENE_MARS_LIGHTHOUSE_MARS_TOWER ((s32)&SceneId_MarsLighthouseMarsTower)
extern char SceneId_MarsLighthouseMercuryTower;
#define SCENE_MARS_LIGHTHOUSE_MERCURY_TOWER                                    \
  ((s32)&SceneId_MarsLighthouseMercuryTower)
extern char SceneId_MarsLighthouseJupiterTower;
#define SCENE_MARS_LIGHTHOUSE_JUPITER_TOWER                                    \
  ((s32)&SceneId_MarsLighthouseJupiterTower)
extern char SceneId_MarsLighthouseVenusTower;
#define SCENE_MARS_LIGHTHOUSE_VENUS_TOWER                                      \
  ((s32)&SceneId_MarsLighthouseVenusTower)
extern char SceneId_MarsLighthouseRoomMFlame;
#define SCENE_MARS_LIGHTHOUSE_ROOM_M_FLAME                                     \
  ((s32)&SceneId_MarsLighthouseRoomMFlame)
/// Mars Lighthouse's aerie, where the Doom Dragon is faced.
extern char SceneId_MarsLighthouseAerie;
#define SCENE_MARS_LIGHTHOUSE_AERIE ((s32)&SceneId_MarsLighthouseAerie)
extern char SceneId_TreasureIsleEntrance;
#define SCENE_TREASURE_ISLE_ENTRANCE ((s32)&SceneId_TreasureIsleEntrance)
extern char SceneId_TreasureIsleChests;
#define SCENE_TREASURE_ISLE_CHESTS ((s32)&SceneId_TreasureIsleChests)
extern char SceneId_TreasureIsleJestersArmletChest;
#define SCENE_TREASURE_ISLE_JESTERS_ARMLET_CHEST                               \
  ((s32)&SceneId_TreasureIsleJestersArmletChest)
/// The Treasure Isle room where Gale is fought.
extern char SceneId_TreasureIsleGale;
#define SCENE_TREASURE_ISLE_GALE ((s32)&SceneId_TreasureIsleGale)
extern char SceneId_TreasureIsleGuardian;
#define SCENE_TREASURE_ISLE_GUARDIAN ((s32)&SceneId_TreasureIsleGuardian)
extern char SceneId_Kibombo;
#define SCENE_KIBOMBO ((s32)&SceneId_Kibombo)
extern char SceneId_KibomboStatueGrounds;
#define SCENE_KIBOMBO_STATUE_GROUNDS ((s32)&SceneId_KibomboStatueGrounds)
extern char SceneId_KibomboHouses;
#define SCENE_KIBOMBO_HOUSES ((s32)&SceneId_KibomboHouses)
extern char SceneId_KibomboCeremony;
#define SCENE_KIBOMBO_CEREMONY ((s32)&SceneId_KibomboCeremony)
extern char SceneId_KibomboStatueGroundsCeremony;
#define SCENE_KIBOMBO_STATUE_GROUNDS_CEREMONY                                  \
  ((s32)&SceneId_KibomboStatueGroundsCeremony)
extern char SceneId_GabombaStatueFace;
#define SCENE_GABOMBA_STATUE_FACE ((s32)&SceneId_GabombaStatueFace)
extern char SceneId_GabombaCatacombsTeleporters;
#define SCENE_GABOMBA_CATACOMBS_TELEPORTERS                                    \
  ((s32)&SceneId_GabombaCatacombsTeleporters)
/// The Gabomba Catacombs room where Mud is fought.
extern char SceneId_GabombaCatacombsMud;
#define SCENE_GABOMBA_CATACOMBS_MUD ((s32)&SceneId_GabombaCatacombsMud)
extern char SceneId_GabombaCatacombsHiddenWall;
#define SCENE_GABOMBA_CATACOMBS_HIDDEN_WALL                                    \
  ((s32)&SceneId_GabombaCatacombsHiddenWall)
extern char SceneId_GabombaCatacombsTomegathericon;
#define SCENE_GABOMBA_CATACOMBS_TOMEGATHERICON                                 \
  ((s32)&SceneId_GabombaCatacombsTomegathericon)
extern char SceneId_ShamanVillageCave;
#define SCENE_SHAMAN_VILLAGE_CAVE ((s32)&SceneId_ShamanVillageCave)
extern char SceneId_ShamanVillageTrialGrounds;
#define SCENE_SHAMAN_VILLAGE_TRIAL_GROUNDS                                     \
  ((s32)&SceneId_ShamanVillageTrialGrounds)
extern char SceneId_ShamanVillage;
#define SCENE_SHAMAN_VILLAGE ((s32)&SceneId_ShamanVillage)
extern char SceneId_ShamanVillageNorthEnd;
#define SCENE_SHAMAN_VILLAGE_NORTH_END ((s32)&SceneId_ShamanVillageNorthEnd)
extern char SceneId_ShamanVillageHouses;
#define SCENE_SHAMAN_VILLAGE_HOUSES ((s32)&SceneId_ShamanVillageHouses)
extern char SceneId_ShamanVillageTents;
#define SCENE_SHAMAN_VILLAGE_TENTS ((s32)&SceneId_ShamanVillageTents)
extern char SceneId_ShamanVillageSanctum;
#define SCENE_SHAMAN_VILLAGE_SANCTUM ((s32)&SceneId_ShamanVillageSanctum)
extern char SceneId_LemurianShipCabins;
#define SCENE_LEMURIAN_SHIP_CABINS ((s32)&SceneId_LemurianShipCabins)
/// The Lemurian Ship's hold, where the Aqua Hydra is fought.
extern char SceneId_LemurianShipAquaHydra;
#define SCENE_LEMURIAN_SHIP_AQUA_HYDRA ((s32)&SceneId_LemurianShipAquaHydra)
extern char SceneId_YallamRoomsInn;
#define SCENE_YALLAM_ROOMS_INN ((s32)&SceneId_YallamRoomsInn)
extern char SceneId_YallamRoomsShops;
#define SCENE_YALLAM_ROOMS_SHOPS ((s32)&SceneId_YallamRoomsShops)
extern char SceneId_TaopoSwampEntrance;
#define SCENE_TAOPO_SWAMP_ENTRANCE ((s32)&SceneId_TaopoSwampEntrance)
extern char SceneId_TaopoSwampMud;
#define SCENE_TAOPO_SWAMP_MUD ((s32)&SceneId_TaopoSwampMud)
extern char SceneId_TaopoSwampTearStones;
#define SCENE_TAOPO_SWAMP_TEAR_STONES ((s32)&SceneId_TaopoSwampTearStones)
/// The Taopo Swamp scene where Flower is fought.
extern char SceneId_TaopoSwampFlower;
#define SCENE_TAOPO_SWAMP_FLOWER ((s32)&SceneId_TaopoSwampFlower)
extern char SceneId_TaopoSwampVents;
#define SCENE_TAOPO_SWAMP_VENTS ((s32)&SceneId_TaopoSwampVents)
extern char SceneId_TaopoSwampPool;
#define SCENE_TAOPO_SWAMP_POOL ((s32)&SceneId_TaopoSwampPool)
extern char SceneId_TaopoSwampPillarGap;
#define SCENE_TAOPO_SWAMP_PILLAR_GAP ((s32)&SceneId_TaopoSwampPillarGap)
extern char SceneId_TaopoSwampDrips;
#define SCENE_TAOPO_SWAMP_DRIPS ((s32)&SceneId_TaopoSwampDrips)
extern char SceneId_TaopoSwampLavaRocks;
#define SCENE_TAOPO_SWAMP_LAVA_ROCKS ((s32)&SceneId_TaopoSwampLavaRocks)
extern char SceneId_TaopoSwampTremor;
#define SCENE_TAOPO_SWAMP_TREMOR ((s32)&SceneId_TaopoSwampTremor)
extern char SceneId_TaopoSwampStarDust;
#define SCENE_TAOPO_SWAMP_STAR_DUST ((s32)&SceneId_TaopoSwampStarDust)
extern char SceneId_GabombaStatueChamberIdol;
#define SCENE_GABOMBA_STATUE_CHAMBER_IDOL                                      \
  ((s32)&SceneId_GabombaStatueChamberIdol)
extern char SceneId_GabombaStatueChamberLift;
#define SCENE_GABOMBA_STATUE_CHAMBER_LIFT                                      \
  ((s32)&SceneId_GabombaStatueChamberLift)
extern char SceneId_GabombaStatueChamberWitchDoctor;
#define SCENE_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR                              \
  ((s32)&SceneId_GabombaStatueChamberWitchDoctor)
extern char SceneId_ChampaCliffCavesBriggs;
#define SCENE_CHAMPA_CLIFF_CAVES_BRIGGS ((s32)&SceneId_ChampaCliffCavesBriggs)
extern char SceneId_ChampaCliffCavesHouses;
#define SCENE_CHAMPA_CLIFF_CAVES_HOUSES ((s32)&SceneId_ChampaCliffCavesHouses)
extern char SceneId_ChampaCliffCavesObaba;
#define SCENE_CHAMPA_CLIFF_CAVES_OBABA ((s32)&SceneId_ChampaCliffCavesObaba)
/// The Champa cliff cave where the Avimander is fought and the Trident taken.
extern char SceneId_ChampaCliffCavesAvimander;
#define SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER                                     \
  ((s32)&SceneId_ChampaCliffCavesAvimander)
extern char SceneId_AnkohlRuinsStoneFaces;
#define SCENE_ANKOHL_RUINS_STONE_FACES ((s32)&SceneId_AnkohlRuinsStoneFaces)
extern char SceneId_AnkohlRuinsThanatosMaceChest;
#define SCENE_ANKOHL_RUINS_THANATOS_MACE_CHEST                                 \
  ((s32)&SceneId_AnkohlRuinsThanatosMaceChest)
extern char SceneId_AnkohlRuinsGreatFace;
#define SCENE_ANKOHL_RUINS_GREAT_FACE ((s32)&SceneId_AnkohlRuinsGreatFace)
extern char SceneId_AnkohlRuinsFaceHalves;
#define SCENE_ANKOHL_RUINS_FACE_HALVES ((s32)&SceneId_AnkohlRuinsFaceHalves)
extern char SceneId_AnkohlRuinsCrystalPowderChest;
#define SCENE_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST                                \
  ((s32)&SceneId_AnkohlRuinsCrystalPowderChest)
extern char SceneId_AnkohlRuinsSandDoors;
#define SCENE_ANKOHL_RUINS_SAND_DOORS ((s32)&SceneId_AnkohlRuinsSandDoors)
extern char SceneId_AnkohlRuinsEntrance;
#define SCENE_ANKOHL_RUINS_ENTRANCE ((s32)&SceneId_AnkohlRuinsEntrance)
extern char SceneId_AnkohlRuinsLeftProng;
#define SCENE_ANKOHL_RUINS_LEFT_PRONG ((s32)&SceneId_AnkohlRuinsLeftProng)
extern char SceneId_AnemosInnerSanctumEntrance;
#define SCENE_ANEMOS_INNER_SANCTUM_ENTRANCE                                    \
  ((s32)&SceneId_AnemosInnerSanctumEntrance)
extern char SceneId_MercuryLighthouse;
#define SCENE_MERCURY_LIGHTHOUSE ((s32)&SceneId_MercuryLighthouse)
extern char SceneId_Epilogue;
#define SCENE_EPILOGUE ((s32)&SceneId_Epilogue)
extern char SceneId_MtAlephSummit;
#define SCENE_MT_ALEPH_SUMMIT ((s32)&SceneId_MtAlephSummit)
extern char SceneId_Lobby;
#define SCENE_LOBBY ((s32)&SceneId_Lobby)
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
extern char SceneId_IconTest;
#define SCENE_ICON_TEST ((s32)&SceneId_IconTest)
extern char SceneId_PsynergyTest;
#define SCENE_PSYNERGY_TEST ((s32)&SceneId_PsynergyTest)
extern char SceneId_PsynergyTest2;
#define SCENE_PSYNERGY_TEST_2 ((s32)&SceneId_PsynergyTest2)
extern char SceneId_ShopTest;
#define SCENE_SHOP_TEST ((s32)&SceneId_ShopTest)
extern char SceneId_TrialRoadStages;
#define SCENE_TRIAL_ROAD_STAGES ((s32)&SceneId_TrialRoadStages)
extern char SceneId_TrialRoadSummit;
#define SCENE_TRIAL_ROAD_SUMMIT ((s32)&SceneId_TrialRoadSummit)
extern char SceneId_TrialRoadCaveRock;
#define SCENE_TRIAL_ROAD_CAVE_ROCK ((s32)&SceneId_TrialRoadCaveRock)
/// The Trial Road scene where Gasp is fought.
extern char SceneId_TrialRoadGasp;
#define SCENE_TRIAL_ROAD_GASP ((s32)&SceneId_TrialRoadGasp)
extern char SceneId_JupiterLighthouseEntrance;
#define SCENE_JUPITER_LIGHTHOUSE_ENTRANCE                                      \
  ((s32)&SceneId_JupiterLighthouseEntrance)
extern char SceneId_JupiterLighthouseBasementTeleporters;
#define SCENE_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS                          \
  ((s32)&SceneId_JupiterLighthouseBasementTeleporters)
extern char SceneId_JupiterLighthouseBasementPsynergyStone;
#define SCENE_JUPITER_LIGHTHOUSE_BASEMENT_PSYNERGY_STONE                       \
  ((s32)&SceneId_JupiterLighthouseBasementPsynergyStone)
extern char SceneId_JupiterLighthouseRoomGateButton;
#define SCENE_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON                              \
  ((s32)&SceneId_JupiterLighthouseRoomGateButton)
extern char SceneId_JupiterLighthouseRoomBeam;
#define SCENE_JUPITER_LIGHTHOUSE_ROOM_BEAM                                     \
  ((s32)&SceneId_JupiterLighthouseRoomBeam)
extern char SceneId_JupiterLighthouseRoomErinyesTunicChest;
#define SCENE_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST                      \
  ((s32)&SceneId_JupiterLighthouseRoomErinyesTunicChest)
extern char SceneId_JupiterLighthouseRoomLockedDoors;
#define SCENE_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS                             \
  ((s32)&SceneId_JupiterLighthouseRoomLockedDoors)
extern char SceneId_JupiterLighthouseRoomRedKey;
#define SCENE_JUPITER_LIGHTHOUSE_ROOM_RED_KEY                                  \
  ((s32)&SceneId_JupiterLighthouseRoomRedKey)
extern char SceneId_JupiterLighthouseRoomCrackedFloor;
#define SCENE_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR                            \
  ((s32)&SceneId_JupiterLighthouseRoomCrackedFloor)
/// The Jupiter Lighthouse room where Whorl is fought.
extern char SceneId_JupiterLighthouseWhorl;
#define SCENE_JUPITER_LIGHTHOUSE_WHORL ((s32)&SceneId_JupiterLighthouseWhorl)
extern char SceneId_JupiterLighthouseRoomBlueKey;
#define SCENE_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY                                 \
  ((s32)&SceneId_JupiterLighthouseRoomBlueKey)
extern char SceneId_JupiterLighthouseRoomPhaetonsBladeChest;
#define SCENE_JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST                     \
  ((s32)&SceneId_JupiterLighthouseRoomPhaetonsBladeChest)

extern char SceneId_GondowanSettlementCave;
#define SCENE_GONDOWAN_SETTLEMENT_CAVE ((s32)&SceneId_GondowanSettlementCave)
extern char SceneId_HesperiaSettlement;
#define SCENE_HESPERIA_SETTLEMENT ((s32)&SceneId_HesperiaSettlement)
extern char SceneId_HesperiaSettlementHouses;
#define SCENE_HESPERIA_SETTLEMENT_HOUSES                                       \
  ((s32)&SceneId_HesperiaSettlementHouses)
extern char SceneId_HesperiaSettlementCave;
#define SCENE_HESPERIA_SETTLEMENT_CAVE ((s32)&SceneId_HesperiaSettlementCave)
extern char SceneId_KaltIsland;
#define SCENE_KALT_ISLAND ((s32)&SceneId_KaltIsland)
extern char SceneId_KaltIslandHouses;
#define SCENE_KALT_ISLAND_HOUSES ((s32)&SceneId_KaltIslandHouses)
extern char SceneId_SwAttekaIslet;
#define SCENE_SW_ATTEKA_ISLET ((s32)&SceneId_SwAttekaIslet)
extern char SceneId_SwAttekaIsletHouses;
#define SCENE_SW_ATTEKA_ISLET_HOUSES ((s32)&SceneId_SwAttekaIsletHouses)
extern char SceneId_Mikasalla;
#define SCENE_MIKASALLA ((s32)&SceneId_Mikasalla)
extern char SceneId_MikasallaNorth;
#define SCENE_MIKASALLA_NORTH ((s32)&SceneId_MikasallaNorth)
extern char SceneId_Lemuria;
#define SCENE_LEMURIA ((s32)&SceneId_Lemuria)
extern char SceneId_LemuriaRooms;
#define SCENE_LEMURIA_ROOMS ((s32)&SceneId_LemuriaRooms)
extern char SceneId_LemuriaPalace;
#define SCENE_LEMURIA_PALACE ((s32)&SceneId_LemuriaPalace)
extern char SceneId_LemuriaSpring;
#define SCENE_LEMURIA_SPRING ((s32)&SceneId_LemuriaSpring)
extern char SceneId_JupiterLighthouseRoomM;
#define SCENE_JUPITER_LIGHTHOUSE_ROOM_M ((s32)&SceneId_JupiterLighthouseRoomM)
extern char SceneId_GabombaCatacombsStream;
#define SCENE_GABOMBA_CATACOMBS_STREAM ((s32)&SceneId_GabombaCatacombsStream)
extern char SceneId_WestIndraIslet;
#define SCENE_WEST_INDRA_ISLET ((s32)&SceneId_WestIndraIslet)
extern char SceneId_EastTundariaIslet;
#define SCENE_EAST_TUNDARIA_ISLET ((s32)&SceneId_EastTundariaIslet)
extern char SceneId_NorthOseniaIslet;
#define SCENE_NORTH_OSENIA_ISLET ((s32)&SceneId_NorthOseniaIslet)
extern char SceneId_SoutheastAngaraIslet;
#define SCENE_SOUTHEAST_ANGARA_ISLET ((s32)&SceneId_SoutheastAngaraIslet)
extern char SceneId_SeaOfTimeIslet;
#define SCENE_SEA_OF_TIME_ISLET ((s32)&SceneId_SeaOfTimeIslet)
extern char SceneId_JupiterLighthouseAerie;
#define SCENE_JUPITER_LIGHTHOUSE_AERIE ((s32)&SceneId_JupiterLighthouseAerie)
extern char SceneId_JupiterLighthouseAerieBeacon;
#define SCENE_JUPITER_LIGHTHOUSE_AERIE_BEACON                                  \
  ((s32)&SceneId_JupiterLighthouseAerieBeacon)
extern char SceneId_Loho;
#define SCENE_LOHO ((s32)&SceneId_Loho)
extern char SceneId_LohoRooms;
#define SCENE_LOHO_ROOMS ((s32)&SceneId_LohoRooms)
extern char SceneId_NorthernReaches;
#define SCENE_NORTHERN_REACHES ((s32)&SceneId_NorthernReaches)

#endif
