#ifndef ALCHEMY_SCENE_IDS_H
#define ALCHEMY_SCENE_IDS_H

/// @file
/// Scene numbers, named after their places, in the order of gScenes
/// (field/scenes.s), as The Broken Seal's scene_ids.h.
///
/// Each SceneId_ is an absolute symbol whose value is the scene's row in the
/// table; C names the number by its macro, SCENE_DAILA_1 for SceneId_Daila1,
/// and the exit tables name a scene by its place (SCENE_EXIT).

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
extern char SceneId_SuhallaGateHouse;
#define SCENE_SUHALLA_GATE_HOUSE ((s32)&SceneId_SuhallaGateHouse)
extern char SceneId_SuhallaGateAmbush;
#define SCENE_SUHALLA_GATE_AMBUSH ((s32)&SceneId_SuhallaGateAmbush)
extern char SceneId_SuhallaGatePsynergyStone;
#define SCENE_SUHALLA_GATE_PSYNERGY_STONE                                      \
  ((s32)&SceneId_SuhallaGatePsynergyStone)
extern char SceneId_Idejima;
#define SCENE_IDEJIMA ((s32)&SceneId_Idejima)
extern char SceneId_DailaInteriorsHouses;
#define SCENE_DAILA_INTERIORS_HOUSES ((s32)&SceneId_DailaInteriorsHouses)
extern char SceneId_DailaInteriorsSanctum;
#define SCENE_DAILA_INTERIORS_SANCTUM ((s32)&SceneId_DailaInteriorsSanctum)
extern char SceneId_Daila1;
#define SCENE_DAILA_1 ((s32)&SceneId_Daila1)
extern char SceneId_Daila2;
#define SCENE_DAILA_2 ((s32)&SceneId_Daila2)
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
extern char SceneId_KandoreanTemple5;
#define SCENE_KANDOREAN_TEMPLE_5 ((s32)&SceneId_KandoreanTemple5)
extern char SceneId_KandoreanTempleSteam;
#define SCENE_KANDOREAN_TEMPLE_STEAM ((s32)&SceneId_KandoreanTempleSteam)
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
extern char SceneId_YampiDesertOasis;
#define SCENE_YAMPI_DESERT_OASIS ((s32)&SceneId_YampiDesertOasis)
/// The Yampi Desert scene where Blitz is fought.
extern char SceneId_YampiDesertBlitz;
#define SCENE_YAMPI_DESERT_BLITZ ((s32)&SceneId_YampiDesertBlitz)
extern char SceneId_YampiDesertPsynergyStone;
#define SCENE_YAMPI_DESERT_PSYNERGY_STONE                                      \
  ((s32)&SceneId_YampiDesertPsynergyStone)
extern char SceneId_YampiDesertKingScorpion;
#define SCENE_YAMPI_DESERT_KING_SCORPION ((s32)&SceneId_YampiDesertKingScorpion)
extern char SceneId_YampiDesertTreasurePit;
#define SCENE_YAMPI_DESERT_TREASURE_PIT ((s32)&SceneId_YampiDesertTreasurePit)
extern char SceneId_YampiDesert7;
#define SCENE_YAMPI_DESERT_7 ((s32)&SceneId_YampiDesert7)
extern char SceneId_YampiDesertTrainersWhipChest;
#define SCENE_YAMPI_DESERT_TRAINERS_WHIP_CHEST                                 \
  ((s32)&SceneId_YampiDesertTrainersWhipChest)
extern char SceneId_YampiDesertMedalSpot;
#define SCENE_YAMPI_DESERT_MEDAL_SPOT ((s32)&SceneId_YampiDesertMedalSpot)
extern char SceneId_YampiDesert10;
#define SCENE_YAMPI_DESERT_10 ((s32)&SceneId_YampiDesert10)
extern char SceneId_YampiDesert11;
#define SCENE_YAMPI_DESERT_11 ((s32)&SceneId_YampiDesert11)
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
extern char SceneId_AirsRockInside1;
#define SCENE_AIRS_ROCK_INSIDE_1 ((s32)&SceneId_AirsRockInside1)
extern char SceneId_AirsRockInside2;
#define SCENE_AIRS_ROCK_INSIDE_2 ((s32)&SceneId_AirsRockInside2)
extern char SceneId_AirsRockInsideClarityCircletChest;
#define SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST                           \
  ((s32)&SceneId_AirsRockInsideClarityCircletChest)
extern char SceneId_AirsRockInside4;
#define SCENE_AIRS_ROCK_INSIDE_4 ((s32)&SceneId_AirsRockInside4)
/// The room inside Air's Rock with the tablet that teaches Reveal.
extern char SceneId_AirsRockInsideRevealTablet;
#define SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET                                   \
  ((s32)&SceneId_AirsRockInsideRevealTablet)
extern char SceneId_AirsRockInsideElixirChest;
#define SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST                                    \
  ((s32)&SceneId_AirsRockInsideElixirChest)
extern char SceneId_AirsRockInsidePairedStatues;
#define SCENE_AIRS_ROCK_INSIDE_PAIRED_STATUES                                  \
  ((s32)&SceneId_AirsRockInsidePairedStatues)
extern char SceneId_AirsRockInsideFaceWarp;
#define SCENE_AIRS_ROCK_INSIDE_FACE_WARP ((s32)&SceneId_AirsRockInsideFaceWarp)
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
extern char SceneId_AlhafraInteriors;
#define SCENE_ALHAFRA_INTERIORS ((s32)&SceneId_AlhafraInteriors)
extern char SceneId_SeaOfTimeWhirlpool;
#define SCENE_SEA_OF_TIME_WHIRLPOOL ((s32)&SceneId_SeaOfTimeWhirlpool)
extern char SceneId_SeaOfTimeToLemuria;
#define SCENE_SEA_OF_TIME_TO_LEMURIA ((s32)&SceneId_SeaOfTimeToLemuria)
extern char SceneId_LemuriaEntrance;
#define SCENE_LEMURIA_ENTRANCE ((s32)&SceneId_LemuriaEntrance)
extern char SceneId_LemuriaDock;
#define SCENE_LEMURIA_DOCK ((s32)&SceneId_LemuriaDock)
extern char SceneId_AncientLemuria;
#define SCENE_ANCIENT_LEMURIA ((s32)&SceneId_AncientLemuria)
extern char SceneId_MadraInteriorsHouses;
#define SCENE_MADRA_INTERIORS_HOUSES ((s32)&SceneId_MadraInteriorsHouses)
extern char SceneId_MadraInteriors2;
#define SCENE_MADRA_INTERIORS_2 ((s32)&SceneId_MadraInteriors2)
extern char SceneId_MadraCatacombsEntrance;
#define SCENE_MADRA_CATACOMBS_ENTRANCE ((s32)&SceneId_MadraCatacombsEntrance)
extern char SceneId_MadraCatacombs2;
#define SCENE_MADRA_CATACOMBS_2 ((s32)&SceneId_MadraCatacombs2)
extern char SceneId_MadraCatacombs3;
#define SCENE_MADRA_CATACOMBS_3 ((s32)&SceneId_MadraCatacombs3)
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
extern char SceneId_NaribweInteriors;
#define SCENE_NARIBWE_INTERIORS ((s32)&SceneId_NaribweInteriors)
extern char SceneId_MikasallaInteriorsHouses;
#define SCENE_MIKASALLA_INTERIORS_HOUSES                                       \
  ((s32)&SceneId_MikasallaInteriorsHouses)
extern char SceneId_MikasallaInteriorsSanctum;
#define SCENE_MIKASALLA_INTERIORS_SANCTUM                                      \
  ((s32)&SceneId_MikasallaInteriorsSanctum)
extern char SceneId_Garoh;
#define SCENE_GAROH ((s32)&SceneId_Garoh)
extern char SceneId_GarohHypnosSwordChest;
#define SCENE_GAROH_HYPNOS_SWORD_CHEST ((s32)&SceneId_GarohHypnosSwordChest)
extern char SceneId_GarohBlocks;
#define SCENE_GAROH_BLOCKS ((s32)&SceneId_GarohBlocks)
extern char SceneId_Garoh4;
#define SCENE_GAROH_4 ((s32)&SceneId_Garoh4)
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
extern char SceneId_ChampaInteriors;
#define SCENE_CHAMPA_INTERIORS ((s32)&SceneId_ChampaInteriors)
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
extern char SceneId_GarohInteriorsHouses;
#define SCENE_GAROH_INTERIORS_HOUSES ((s32)&SceneId_GarohInteriorsHouses)
extern char SceneId_GarohInteriorsShop;
#define SCENE_GAROH_INTERIORS_SHOP ((s32)&SceneId_GarohInteriorsShop)
extern char SceneId_GarohInteriorsSanctum;
#define SCENE_GAROH_INTERIORS_SANCTUM ((s32)&SceneId_GarohInteriorsSanctum)
extern char SceneId_GabombaStatueBoneArmletChest;
#define SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST                                 \
  ((s32)&SceneId_GabombaStatueBoneArmletChest)
extern char SceneId_GabombaStatueElixirChest;
#define SCENE_GABOMBA_STATUE_ELIXIR_CHEST                                      \
  ((s32)&SceneId_GabombaStatueElixirChest)
/// The room inside the Gabomba Statue where Steel is fought.
extern char SceneId_GabombaStatueSteel;
#define SCENE_GABOMBA_STATUE_STEEL ((s32)&SceneId_GabombaStatueSteel)
extern char SceneId_GabombaStatueTurntable;
#define SCENE_GABOMBA_STATUE_TURNTABLE ((s32)&SceneId_GabombaStatueTurntable)
extern char SceneId_GabombaStatueCogs;
#define SCENE_GABOMBA_STATUE_COGS ((s32)&SceneId_GabombaStatueCogs)
extern char SceneId_GabombaStatueArrows;
#define SCENE_GABOMBA_STATUE_ARROWS ((s32)&SceneId_GabombaStatueArrows)
extern char SceneId_GabombaStatuePinion;
#define SCENE_GABOMBA_STATUE_PINION ((s32)&SceneId_GabombaStatuePinion)
extern char SceneId_AnemosInnerSanctumDragonSkinChest;
#define SCENE_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST                           \
  ((s32)&SceneId_AnemosInnerSanctumDragonSkinChest)
extern char SceneId_AnemosInnerSanctum2;
#define SCENE_ANEMOS_INNER_SANCTUM_2 ((s32)&SceneId_AnemosInnerSanctum2)
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
extern char SceneId_GaiaRockInside4;
#define SCENE_GAIA_ROCK_INSIDE_4 ((s32)&SceneId_GaiaRockInside4)
extern char SceneId_GaiaRockInside5;
#define SCENE_GAIA_ROCK_INSIDE_5 ((s32)&SceneId_GaiaRockInside5)
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
extern char SceneId_MagmaRockInside2;
#define SCENE_MAGMA_ROCK_INSIDE_2 ((s32)&SceneId_MagmaRockInside2)
/// The room inside Magma Rock where Fury is fought.
extern char SceneId_MagmaRockInsideFury;
#define SCENE_MAGMA_ROCK_INSIDE_FURY ((s32)&SceneId_MagmaRockInsideFury)
extern char SceneId_MagmaRockInside4;
#define SCENE_MAGMA_ROCK_INSIDE_4 ((s32)&SceneId_MagmaRockInside4)
extern char SceneId_MagmaRockInsideGolemCoreChest;
#define SCENE_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST                               \
  ((s32)&SceneId_MagmaRockInsideGolemCoreChest)
extern char SceneId_MagmaRockInside6;
#define SCENE_MAGMA_ROCK_INSIDE_6 ((s32)&SceneId_MagmaRockInside6)
extern char SceneId_MagmaRockInsideEmberStone;
#define SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE                                    \
  ((s32)&SceneId_MagmaRockInsideEmberStone)
/// The room inside Magma Rock with the tablet that teaches Blaze.
extern char SceneId_MagmaRockInsideBlazeTablet;
#define SCENE_MAGMA_ROCK_INSIDE_BLAZE_TABLET                                   \
  ((s32)&SceneId_MagmaRockInsideBlazeTablet)
extern char SceneId_TundariaTower1;
#define SCENE_TUNDARIA_TOWER_1 ((s32)&SceneId_TundariaTower1)
extern char SceneId_TundariaTowerCenterProng;
#define SCENE_TUNDARIA_TOWER_CENTER_PRONG                                      \
  ((s32)&SceneId_TundariaTowerCenterProng)
extern char SceneId_TundariaTower3;
#define SCENE_TUNDARIA_TOWER_3 ((s32)&SceneId_TundariaTower3)
/// The Tundaria Tower room where Reflux is fought.
extern char SceneId_TundariaTowerReflux;
#define SCENE_TUNDARIA_TOWER_REFLUX ((s32)&SceneId_TundariaTowerReflux)
extern char SceneId_TundariaTower5;
#define SCENE_TUNDARIA_TOWER_5 ((s32)&SceneId_TundariaTower5)
extern char SceneId_TundariaTower6;
#define SCENE_TUNDARIA_TOWER_6 ((s32)&SceneId_TundariaTower6)
extern char SceneId_TundariaTowerBurstBrooch;
#define SCENE_TUNDARIA_TOWER_BURST_BROOCH                                      \
  ((s32)&SceneId_TundariaTowerBurstBrooch)
extern char SceneId_Settlements1;
#define SCENE_SETTLEMENTS_1 ((s32)&SceneId_Settlements1)
extern char SceneId_Settlements2;
#define SCENE_SETTLEMENTS_2 ((s32)&SceneId_Settlements2)
extern char SceneId_ShrineOfTheSeaGodRope;
#define SCENE_SHRINE_OF_THE_SEA_GOD_ROPE ((s32)&SceneId_ShrineOfTheSeaGodRope)
extern char SceneId_ShrineOfTheSeaGod2;
#define SCENE_SHRINE_OF_THE_SEA_GOD_2 ((s32)&SceneId_ShrineOfTheSeaGod2)
extern char SceneId_ShrineOfTheSeaGod3;
#define SCENE_SHRINE_OF_THE_SEA_GOD_3 ((s32)&SceneId_ShrineOfTheSeaGod3)
extern char SceneId_ShrineOfTheSeaGod4;
#define SCENE_SHRINE_OF_THE_SEA_GOD_4 ((s32)&SceneId_ShrineOfTheSeaGod4)
/// The Shrine of the Sea God room where Breath is chased and fought.
extern char SceneId_ShrineOfTheSeaGodBreath;
#define SCENE_SHRINE_OF_THE_SEA_GOD_BREATH                                     \
  ((s32)&SceneId_ShrineOfTheSeaGodBreath)
extern char SceneId_ShrineOfTheSeaGod6;
#define SCENE_SHRINE_OF_THE_SEA_GOD_6 ((s32)&SceneId_ShrineOfTheSeaGod6)
extern char SceneId_ShrineOfTheSeaGodPillars;
#define SCENE_SHRINE_OF_THE_SEA_GOD_PILLARS                                    \
  ((s32)&SceneId_ShrineOfTheSeaGodPillars)
extern char SceneId_ShrineOfTheSeaGodSeaGodsTear;
#define SCENE_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR                              \
  ((s32)&SceneId_ShrineOfTheSeaGodSeaGodsTear)
extern char SceneId_ShrineOfTheSeaGod9;
#define SCENE_SHRINE_OF_THE_SEA_GOD_9 ((s32)&SceneId_ShrineOfTheSeaGod9)
extern char SceneId_ShrineOfTheSeaGod10;
#define SCENE_SHRINE_OF_THE_SEA_GOD_10 ((s32)&SceneId_ShrineOfTheSeaGod10)
extern char SceneId_ShrineOfTheSeaGodRightProng;
#define SCENE_SHRINE_OF_THE_SEA_GOD_RIGHT_PRONG                                \
  ((s32)&SceneId_ShrineOfTheSeaGodRightProng)
extern char SceneId_AquaRock1;
#define SCENE_AQUA_ROCK_1 ((s32)&SceneId_AquaRock1)
extern char SceneId_AquaRockMistSabreChest;
#define SCENE_AQUA_ROCK_MIST_SABRE_CHEST ((s32)&SceneId_AquaRockMistSabreChest)
extern char SceneId_DehkanPlateauFullMetalVestChest;
#define SCENE_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST                             \
  ((s32)&SceneId_DehkanPlateauFullMetalVestChest)
extern char SceneId_DehkanPlateauThemisAxeChest;
#define SCENE_DEHKAN_PLATEAU_THEMIS_AXE_CHEST                                  \
  ((s32)&SceneId_DehkanPlateauThemisAxeChest)
extern char SceneId_DehkanPlateau3;
#define SCENE_DEHKAN_PLATEAU_3 ((s32)&SceneId_DehkanPlateau3)
extern char SceneId_DehkanPlateau4;
#define SCENE_DEHKAN_PLATEAU_4 ((s32)&SceneId_DehkanPlateau4)
extern char SceneId_DehkanPlateauPoundCube;
#define SCENE_DEHKAN_PLATEAU_POUND_CUBE ((s32)&SceneId_DehkanPlateauPoundCube)
/// The Dehkan Plateau scene where Cannon is met and fought.
extern char SceneId_DehkanPlateauCannon;
#define SCENE_DEHKAN_PLATEAU_CANNON ((s32)&SceneId_DehkanPlateauCannon)
extern char SceneId_LemurianShipDeck1;
#define SCENE_LEMURIAN_SHIP_DECK_1 ((s32)&SceneId_LemurianShipDeck1)
extern char SceneId_LemurianShipDeck2;
#define SCENE_LEMURIAN_SHIP_DECK_2 ((s32)&SceneId_LemurianShipDeck2)
extern char SceneId_Madra;
#define SCENE_MADRA ((s32)&SceneId_Madra)
/// The Kibombo Mountains scene where Waft is fought.
extern char SceneId_KibomboMountainsWaft;
#define SCENE_KIBOMBO_MOUNTAINS_WAFT ((s32)&SceneId_KibomboMountainsWaft)
extern char SceneId_KibomboMountainsDiskAxeChest;
#define SCENE_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST                                 \
  ((s32)&SceneId_KibomboMountainsDiskAxeChest)
extern char SceneId_KibomboMountains3;
#define SCENE_KIBOMBO_MOUNTAINS_3 ((s32)&SceneId_KibomboMountains3)
/// The Islet Cave room where Meld is fought.
extern char SceneId_IsletCaveMeld;
#define SCENE_ISLET_CAVE_MELD ((s32)&SceneId_IsletCaveMeld)
/// The Islet Cave room where Serac is fought.
extern char SceneId_IsletCaveSerac;
#define SCENE_ISLET_CAVE_SERAC ((s32)&SceneId_IsletCaveSerac)
/// The Islet Cave room where the Sentinel waits.
extern char SceneId_IsletCaveSentinel;
#define SCENE_ISLET_CAVE_SENTINEL ((s32)&SceneId_IsletCaveSentinel)
extern char SceneId_Yallam;
#define SCENE_YALLAM ((s32)&SceneId_Yallam)
extern char SceneId_Yallam2;
#define SCENE_YALLAM_2 ((s32)&SceneId_Yallam2)
extern char SceneId_ApojiiIslands1;
#define SCENE_APOJII_ISLANDS_1 ((s32)&SceneId_ApojiiIslands1)
/// The Apojii Islands scene where Haze joins the party.
extern char SceneId_ApojiiIslandsHaze;
#define SCENE_APOJII_ISLANDS_HAZE ((s32)&SceneId_ApojiiIslandsHaze)
extern char SceneId_ApojiiIslands3;
#define SCENE_APOJII_ISLANDS_3 ((s32)&SceneId_ApojiiIslands3)
extern char SceneId_ApojiiIslandsCave;
#define SCENE_APOJII_ISLANDS_CAVE ((s32)&SceneId_ApojiiIslandsCave)
extern char SceneId_AquaRockInsideWaterOfLifeChest;
#define SCENE_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST                             \
  ((s32)&SceneId_AquaRockInsideWaterOfLifeChest)
/// The room inside Aqua Rock with the tablet that teaches Parch.
extern char SceneId_AquaRockInsideParchTablet;
#define SCENE_AQUA_ROCK_INSIDE_PARCH_TABLET                                    \
  ((s32)&SceneId_AquaRockInsideParchTablet)
extern char SceneId_IzumoInteriors;
#define SCENE_IZUMO_INTERIORS ((s32)&SceneId_IzumoInteriors)
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
extern char SceneId_AquaRockInside4;
#define SCENE_AQUA_ROCK_INSIDE_4 ((s32)&SceneId_AquaRockInside4)
extern char SceneId_AquaRockInside5;
#define SCENE_AQUA_ROCK_INSIDE_5 ((s32)&SceneId_AquaRockInside5)
extern char SceneId_AquaRockInside6;
#define SCENE_AQUA_ROCK_INSIDE_6 ((s32)&SceneId_AquaRockInside6)
/// The room inside Aqua Rock with a Psynergy Stone.
extern char SceneId_AquaRockInsidePsynergyStone;
#define SCENE_AQUA_ROCK_INSIDE_PSYNERGY_STONE                                  \
  ((s32)&SceneId_AquaRockInsidePsynergyStone)
extern char SceneId_AquaRockInside8;
#define SCENE_AQUA_ROCK_INSIDE_8 ((s32)&SceneId_AquaRockInside8)
extern char SceneId_AquaRockInsideTearStone;
#define SCENE_AQUA_ROCK_INSIDE_TEAR_STONE                                      \
  ((s32)&SceneId_AquaRockInsideTearStone)
extern char SceneId_AttekaInlet;
#define SCENE_ATTEKA_INLET ((s32)&SceneId_AttekaInlet)
/// The Atteka Inlet scene where Cyclone uncovers Geode.
extern char SceneId_AttekaInletGeode;
#define SCENE_ATTEKA_INLET_GEODE ((s32)&SceneId_AttekaInletGeode)
extern char SceneId_Prox1;
#define SCENE_PROX_1 ((s32)&SceneId_Prox1)
extern char SceneId_Prox;
#define SCENE_PROX ((s32)&SceneId_Prox)
extern char SceneId_ProxMold;
#define SCENE_PROX_MOLD ((s32)&SceneId_ProxMold)
extern char SceneId_Prox4;
#define SCENE_PROX_4 ((s32)&SceneId_Prox4)
extern char SceneId_Prox5;
#define SCENE_PROX_5 ((s32)&SceneId_Prox5)
extern char SceneId_Prox6;
#define SCENE_PROX_6 ((s32)&SceneId_Prox6)
extern char SceneId_CavernsIndra;
#define SCENE_CAVERNS_INDRA ((s32)&SceneId_CavernsIndra)
extern char SceneId_CavernsOsenia;
#define SCENE_CAVERNS_OSENIA ((s32)&SceneId_CavernsOsenia)
extern char SceneId_CavernsAtteka;
#define SCENE_CAVERNS_ATTEKA ((s32)&SceneId_CavernsAtteka)
extern char SceneId_CavernsAngara;
#define SCENE_CAVERNS_ANGARA ((s32)&SceneId_CavernsAngara)
extern char SceneId_MarsLighthouseEntrance1;
#define SCENE_MARS_LIGHTHOUSE_ENTRANCE_1 ((s32)&SceneId_MarsLighthouseEntrance1)
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
extern char SceneId_MarsLighthouseEntrance5;
#define SCENE_MARS_LIGHTHOUSE_ENTRANCE_5 ((s32)&SceneId_MarsLighthouseEntrance5)
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
extern char SceneId_MarsLighthouseRoomM2;
#define SCENE_MARS_LIGHTHOUSE_ROOM_M_2 ((s32)&SceneId_MarsLighthouseRoomM2)
extern char SceneId_MarsLighthouseRoomM3;
#define SCENE_MARS_LIGHTHOUSE_ROOM_M_3 ((s32)&SceneId_MarsLighthouseRoomM3)
extern char SceneId_MarsLighthouseRoomMAlastorsHoodChest;
#define SCENE_MARS_LIGHTHOUSE_ROOM_M_ALASTORS_HOOD_CHEST                       \
  ((s32)&SceneId_MarsLighthouseRoomMAlastorsHoodChest)
extern char SceneId_MarsLighthouseRoomM5;
#define SCENE_MARS_LIGHTHOUSE_ROOM_M_5 ((s32)&SceneId_MarsLighthouseRoomM5)
extern char SceneId_MarsLighthouseRoomF1;
#define SCENE_MARS_LIGHTHOUSE_ROOM_F_1 ((s32)&SceneId_MarsLighthouseRoomF1)
extern char SceneId_MarsLighthouseRoomF2;
#define SCENE_MARS_LIGHTHOUSE_ROOM_F_2 ((s32)&SceneId_MarsLighthouseRoomF2)
extern char SceneId_MarsLighthouseRoomF3;
#define SCENE_MARS_LIGHTHOUSE_ROOM_F_3 ((s32)&SceneId_MarsLighthouseRoomF3)
extern char SceneId_MarsLighthouseRoomF4;
#define SCENE_MARS_LIGHTHOUSE_ROOM_F_4 ((s32)&SceneId_MarsLighthouseRoomF4)
extern char SceneId_MarsLighthouseRoomMFlame;
#define SCENE_MARS_LIGHTHOUSE_ROOM_M_FLAME                                     \
  ((s32)&SceneId_MarsLighthouseRoomMFlame)
/// Mars Lighthouse's aerie, where the Doom Dragon is faced.
extern char SceneId_MarsLighthouseAerie;
#define SCENE_MARS_LIGHTHOUSE_AERIE ((s32)&SceneId_MarsLighthouseAerie)
extern char SceneId_TreasureIsle1;
#define SCENE_TREASURE_ISLE_1 ((s32)&SceneId_TreasureIsle1)
extern char SceneId_TreasureIsle2;
#define SCENE_TREASURE_ISLE_2 ((s32)&SceneId_TreasureIsle2)
extern char SceneId_TreasureIsleJestersArmletChest;
#define SCENE_TREASURE_ISLE_JESTERS_ARMLET_CHEST                               \
  ((s32)&SceneId_TreasureIsleJestersArmletChest)
/// The Treasure Isle room where Gale is fought.
extern char SceneId_TreasureIsleGale;
#define SCENE_TREASURE_ISLE_GALE ((s32)&SceneId_TreasureIsleGale)
extern char SceneId_TreasureIsleGuardian;
#define SCENE_TREASURE_ISLE_GUARDIAN ((s32)&SceneId_TreasureIsleGuardian)
extern char SceneId_Kibombo1;
#define SCENE_KIBOMBO_1 ((s32)&SceneId_Kibombo1)
extern char SceneId_Kibombo2;
#define SCENE_KIBOMBO_2 ((s32)&SceneId_Kibombo2)
extern char SceneId_Kibombo3;
#define SCENE_KIBOMBO_3 ((s32)&SceneId_Kibombo3)
extern char SceneId_Kibombo4;
#define SCENE_KIBOMBO_4 ((s32)&SceneId_Kibombo4)
extern char SceneId_Kibombo5;
#define SCENE_KIBOMBO_5 ((s32)&SceneId_Kibombo5)
extern char SceneId_Kibombo6;
#define SCENE_KIBOMBO_6 ((s32)&SceneId_Kibombo6)
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
extern char SceneId_ShamanVillage1;
#define SCENE_SHAMAN_VILLAGE_1 ((s32)&SceneId_ShamanVillage1)
extern char SceneId_ShamanVillage;
#define SCENE_SHAMAN_VILLAGE ((s32)&SceneId_ShamanVillage)
extern char SceneId_ShamanVillageAroma;
#define SCENE_SHAMAN_VILLAGE_AROMA ((s32)&SceneId_ShamanVillageAroma)
extern char SceneId_ShamanVillageInn;
#define SCENE_SHAMAN_VILLAGE_INN ((s32)&SceneId_ShamanVillageInn)
extern char SceneId_ShamanVillageShop;
#define SCENE_SHAMAN_VILLAGE_SHOP ((s32)&SceneId_ShamanVillageShop)
extern char SceneId_ShamanVillageSanctum;
#define SCENE_SHAMAN_VILLAGE_SANCTUM ((s32)&SceneId_ShamanVillageSanctum)
extern char SceneId_LemurianShip1;
#define SCENE_LEMURIAN_SHIP_1 ((s32)&SceneId_LemurianShip1)
/// The Lemurian Ship's hold, where the Aqua Hydra is fought.
extern char SceneId_LemurianShipAquaHydra;
#define SCENE_LEMURIAN_SHIP_AQUA_HYDRA ((s32)&SceneId_LemurianShipAquaHydra)
extern char SceneId_YallamInteriorsInn;
#define SCENE_YALLAM_INTERIORS_INN ((s32)&SceneId_YallamInteriorsInn)
extern char SceneId_YallamInteriorsShops;
#define SCENE_YALLAM_INTERIORS_SHOPS ((s32)&SceneId_YallamInteriorsShops)
extern char SceneId_TaopoSwamp1;
#define SCENE_TAOPO_SWAMP_1 ((s32)&SceneId_TaopoSwamp1)
extern char SceneId_TaopoSwampMud;
#define SCENE_TAOPO_SWAMP_MUD ((s32)&SceneId_TaopoSwampMud)
extern char SceneId_TaopoSwampTearStones;
#define SCENE_TAOPO_SWAMP_TEAR_STONES ((s32)&SceneId_TaopoSwampTearStones)
/// The Taopo Swamp scene where Flower is fought.
extern char SceneId_TaopoSwampFlower;
#define SCENE_TAOPO_SWAMP_FLOWER ((s32)&SceneId_TaopoSwampFlower)
extern char SceneId_TaopoSwamp5;
#define SCENE_TAOPO_SWAMP_5 ((s32)&SceneId_TaopoSwamp5)
extern char SceneId_TaopoSwamp6;
#define SCENE_TAOPO_SWAMP_6 ((s32)&SceneId_TaopoSwamp6)
extern char SceneId_TaopoSwamp7;
#define SCENE_TAOPO_SWAMP_7 ((s32)&SceneId_TaopoSwamp7)
extern char SceneId_TaopoSwamp8;
#define SCENE_TAOPO_SWAMP_8 ((s32)&SceneId_TaopoSwamp8)
extern char SceneId_TaopoSwamp9;
#define SCENE_TAOPO_SWAMP_9 ((s32)&SceneId_TaopoSwamp9)
extern char SceneId_TaopoSwamp10;
#define SCENE_TAOPO_SWAMP_10 ((s32)&SceneId_TaopoSwamp10)
extern char SceneId_TaopoSwampStarDust;
#define SCENE_TAOPO_SWAMP_STAR_DUST ((s32)&SceneId_TaopoSwampStarDust)
extern char SceneId_GabombaStatueChamber1;
#define SCENE_GABOMBA_STATUE_CHAMBER_1 ((s32)&SceneId_GabombaStatueChamber1)
extern char SceneId_GabombaStatueChamberLift;
#define SCENE_GABOMBA_STATUE_CHAMBER_LIFT                                      \
  ((s32)&SceneId_GabombaStatueChamberLift)
extern char SceneId_GabombaStatueChamberWitchDoctor;
#define SCENE_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR                              \
  ((s32)&SceneId_GabombaStatueChamberWitchDoctor)
extern char SceneId_ChampaCliffCavesBriggs;
#define SCENE_CHAMPA_CLIFF_CAVES_BRIGGS ((s32)&SceneId_ChampaCliffCavesBriggs)
extern char SceneId_ChampaCliffCaves2;
#define SCENE_CHAMPA_CLIFF_CAVES_2 ((s32)&SceneId_ChampaCliffCaves2)
extern char SceneId_ChampaCliffCavesObaba;
#define SCENE_CHAMPA_CLIFF_CAVES_OBABA ((s32)&SceneId_ChampaCliffCavesObaba)
/// The Champa cliff cave where the Avimander is fought and the Trident taken.
extern char SceneId_ChampaCliffCavesAvimander;
#define SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER                                     \
  ((s32)&SceneId_ChampaCliffCavesAvimander)
extern char SceneId_AnkohlRuins1;
#define SCENE_ANKOHL_RUINS_1 ((s32)&SceneId_AnkohlRuins1)
extern char SceneId_AnkohlRuinsThanatosMaceChest;
#define SCENE_ANKOHL_RUINS_THANATOS_MACE_CHEST                                 \
  ((s32)&SceneId_AnkohlRuinsThanatosMaceChest)
extern char SceneId_AnkohlRuins3;
#define SCENE_ANKOHL_RUINS_3 ((s32)&SceneId_AnkohlRuins3)
extern char SceneId_AnkohlRuins4;
#define SCENE_ANKOHL_RUINS_4 ((s32)&SceneId_AnkohlRuins4)
extern char SceneId_AnkohlRuins5;
#define SCENE_ANKOHL_RUINS_5 ((s32)&SceneId_AnkohlRuins5)
extern char SceneId_AnkohlRuinsSandDoors;
#define SCENE_ANKOHL_RUINS_SAND_DOORS ((s32)&SceneId_AnkohlRuinsSandDoors)
extern char SceneId_AnkohlRuins7;
#define SCENE_ANKOHL_RUINS_7 ((s32)&SceneId_AnkohlRuins7)
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
extern char SceneId_Level;
#define SCENE_LEVEL ((s32)&SceneId_Level)
extern char SceneId_PsynergyTest;
#define SCENE_PSYNERGY_TEST ((s32)&SceneId_PsynergyTest)
extern char SceneId_PsynergyTest2;
#define SCENE_PSYNERGY_TEST_2 ((s32)&SceneId_PsynergyTest2)
extern char SceneId_ShopTest;
#define SCENE_SHOP_TEST ((s32)&SceneId_ShopTest)
extern char SceneId_TrialRoad1;
#define SCENE_TRIAL_ROAD_1 ((s32)&SceneId_TrialRoad1)
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
extern char SceneId_JupiterLighthouseBasement1;
#define SCENE_JUPITER_LIGHTHOUSE_BASEMENT_1                                    \
  ((s32)&SceneId_JupiterLighthouseBasement1)
extern char SceneId_JupiterLighthouseBasement2;
#define SCENE_JUPITER_LIGHTHOUSE_BASEMENT_2                                    \
  ((s32)&SceneId_JupiterLighthouseBasement2)
extern char SceneId_JupiterLighthouseRoom1;
#define SCENE_JUPITER_LIGHTHOUSE_ROOM_1 ((s32)&SceneId_JupiterLighthouseRoom1)
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
extern char SceneId_JupiterLighthouseRoom6;
#define SCENE_JUPITER_LIGHTHOUSE_ROOM_6 ((s32)&SceneId_JupiterLighthouseRoom6)
/// The Jupiter Lighthouse room where Whorl is fought.
extern char SceneId_JupiterLighthouseWhorl;
#define SCENE_JUPITER_LIGHTHOUSE_WHORL ((s32)&SceneId_JupiterLighthouseWhorl)
extern char SceneId_JupiterLighthouseRoomBlueKey;
#define SCENE_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY                                 \
  ((s32)&SceneId_JupiterLighthouseRoomBlueKey)
extern char SceneId_JupiterLighthouseRoomPhaetonsBladeChest;
#define SCENE_JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST                     \
  ((s32)&SceneId_JupiterLighthouseRoomPhaetonsBladeChest)

extern char SceneId_Settlements3;
#define SCENE_SETTLEMENTS_3 ((s32)&SceneId_Settlements3)
extern char SceneId_Settlements4;
#define SCENE_SETTLEMENTS_4 ((s32)&SceneId_Settlements4)
extern char SceneId_Settlements5;
#define SCENE_SETTLEMENTS_5 ((s32)&SceneId_Settlements5)
extern char SceneId_Settlements6;
#define SCENE_SETTLEMENTS_6 ((s32)&SceneId_Settlements6)
extern char SceneId_Settlements7;
#define SCENE_SETTLEMENTS_7 ((s32)&SceneId_Settlements7)
extern char SceneId_Settlements8;
#define SCENE_SETTLEMENTS_8 ((s32)&SceneId_Settlements8)
extern char SceneId_Settlements9;
#define SCENE_SETTLEMENTS_9 ((s32)&SceneId_Settlements9)
extern char SceneId_Settlements10;
#define SCENE_SETTLEMENTS_10 ((s32)&SceneId_Settlements10)
extern char SceneId_Mikasalla;
#define SCENE_MIKASALLA ((s32)&SceneId_Mikasalla)
extern char SceneId_Mikasalla2;
#define SCENE_MIKASALLA_2 ((s32)&SceneId_Mikasalla2)
extern char SceneId_Lemuria;
#define SCENE_LEMURIA ((s32)&SceneId_Lemuria)
extern char SceneId_LemuriaInteriors;
#define SCENE_LEMURIA_INTERIORS ((s32)&SceneId_LemuriaInteriors)
extern char SceneId_LemuriaPalace;
#define SCENE_LEMURIA_PALACE ((s32)&SceneId_LemuriaPalace)
extern char SceneId_LemuriaSpring;
#define SCENE_LEMURIA_SPRING ((s32)&SceneId_LemuriaSpring)
extern char SceneId_JupiterLighthouseRoomM;
#define SCENE_JUPITER_LIGHTHOUSE_ROOM_M ((s32)&SceneId_JupiterLighthouseRoomM)
extern char SceneId_GabombaCatacombs5;
#define SCENE_GABOMBA_CATACOMBS_5 ((s32)&SceneId_GabombaCatacombs5)
extern char SceneId_IsletsDog;
#define SCENE_ISLETS_DOG ((s32)&SceneId_IsletsDog)
extern char SceneId_IsletsPrettyStone;
#define SCENE_ISLETS_PRETTY_STONE ((s32)&SceneId_IsletsPrettyStone)
extern char SceneId_IsletsCow;
#define SCENE_ISLETS_COW ((s32)&SceneId_IsletsCow)
extern char SceneId_Islets4;
#define SCENE_ISLETS_4 ((s32)&SceneId_Islets4)
extern char SceneId_IsletsTurtle;
#define SCENE_ISLETS_TURTLE ((s32)&SceneId_IsletsTurtle)
extern char SceneId_JupiterLighthouseAerie;
#define SCENE_JUPITER_LIGHTHOUSE_AERIE ((s32)&SceneId_JupiterLighthouseAerie)
extern char SceneId_JupiterLighthouseAerieBeacon;
#define SCENE_JUPITER_LIGHTHOUSE_AERIE_BEACON                                  \
  ((s32)&SceneId_JupiterLighthouseAerieBeacon)
extern char SceneId_Loho1;
#define SCENE_LOHO_1 ((s32)&SceneId_Loho1)
extern char SceneId_Loho2;
#define SCENE_LOHO_2 ((s32)&SceneId_Loho2)
extern char SceneId_NorthernReaches;
#define SCENE_NORTHERN_REACHES ((s32)&SceneId_NorthernReaches)

#endif
