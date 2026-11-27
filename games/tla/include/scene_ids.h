#ifndef ALCHEMY_SCENE_IDS_H
#define ALCHEMY_SCENE_IDS_H

// Scene numbers, named after their places, in the order of ScenesWork (field/scenes.s).
//
// Each SceneId is an absolute symbol whose value is the scene's row in the table; C names the
// number by its macro, SCENE_DAILA_FLOODED for SceneIdDailaFlooded, and the exit tables name a
// scene by its place (SCENE_EXIT). The scenes are grouped under the area each belongs to, by the
// area name the game shows; a scene's own file says what its rooms are.

#include "types.h"

// Title screen
extern char SceneIdTitle;
#define SCENE_TITLE ((Sint32)&SceneIdTitle)

// World Map
extern char SceneIdWorldMap;
#define SCENE_WORLD_MAP ((Sint32)&SceneIdWorldMap)

// Staff Credit
extern char SceneIdStaffCredit;
#define SCENE_STAFF_CREDIT ((Sint32)&SceneIdStaffCredit)

// (Clear data)
extern char SceneIdClear;
#define SCENE_CLEAR ((Sint32)&SceneIdClear)

// Venus Lighthouse
extern char SceneIdVenusLighthouseEntrance;
#define SCENE_VENUS_LIGHTHOUSE_ENTRANCE ((Sint32)&SceneIdVenusLighthouseEntrance)
extern char SceneIdVenusLighthouse;
#define SCENE_VENUS_LIGHTHOUSE ((Sint32)&SceneIdVenusLighthouse)

// Suhalla Gate
extern char SceneIdSuhallaGateRoad;
#define SCENE_SUHALLA_GATE_ROAD ((Sint32)&SceneIdSuhallaGateRoad)
extern char SceneIdSuhallaGateAmbush;
#define SCENE_SUHALLA_GATE_AMBUSH ((Sint32)&SceneIdSuhallaGateAmbush)
extern char SceneIdSuhallaGatePsynergyStone;
#define SCENE_SUHALLA_GATE_PSYNERGY_STONE ((Sint32)&SceneIdSuhallaGatePsynergyStone)

// Idejima
extern char SceneIdIdejima;
#define SCENE_IDEJIMA ((Sint32)&SceneIdIdejima)

// Daila
extern char SceneIdDailaRoomsHouses;
#define SCENE_DAILA_ROOMS_HOUSES ((Sint32)&SceneIdDailaRoomsHouses)
extern char SceneIdDailaRoomsSanctum;
#define SCENE_DAILA_ROOMS_SANCTUM ((Sint32)&SceneIdDailaRoomsSanctum)
extern char SceneIdDailaFlooded;
#define SCENE_DAILA_FLOODED ((Sint32)&SceneIdDailaFlooded)
extern char SceneIdDaila;
#define SCENE_DAILA ((Sint32)&SceneIdDaila)

// Kandorean Temple The Kandorean Temple's room of the east and west lifts.
extern char SceneIdKandoreanTempleLifts;
#define SCENE_KANDOREAN_TEMPLE_LIFTS ((Sint32)&SceneIdKandoreanTempleLifts)
extern char SceneIdKandoreanTempleMimic;
#define SCENE_KANDOREAN_TEMPLE_MIMIC ((Sint32)&SceneIdKandoreanTempleMimic)
// The Kandorean Temple room where the Chestbeaters are fought.
extern char SceneIdKandoreanTempleChestbeaters;
#define SCENE_KANDOREAN_TEMPLE_CHESTBEATERS ((Sint32)&SceneIdKandoreanTempleChestbeaters)
extern char SceneIdKandoreanTemplePillar;
#define SCENE_KANDOREAN_TEMPLE_PILLAR ((Sint32)&SceneIdKandoreanTemplePillar)
// The cave of shallow water behind the bush by the Kandorean Temple's gate, the hidden way in.
extern char SceneIdKandoreanTempleStream;
#define SCENE_KANDOREAN_TEMPLE_STREAM ((Sint32)&SceneIdKandoreanTempleStream)
// The Kandorean Temple room whose current of water pushes Felix south.
extern char SceneIdKandoreanTempleCurrent;
#define SCENE_KANDOREAN_TEMPLE_CURRENT ((Sint32)&SceneIdKandoreanTempleCurrent)
// The Kandorean Temple room where Fog is fought.
extern char SceneIdKandoreanTempleFog;
#define SCENE_KANDOREAN_TEMPLE_FOG ((Sint32)&SceneIdKandoreanTempleFog)
// The Kandorean Temple's gate, where its students and guard stand.
extern char SceneIdKandoreanTempleGate;
#define SCENE_KANDOREAN_TEMPLE_GATE ((Sint32)&SceneIdKandoreanTempleGate)
// Poi's hall, where Lash is received.
extern char SceneIdKandoreanTemplePoi;
#define SCENE_KANDOREAN_TEMPLE_POI ((Sint32)&SceneIdKandoreanTemplePoi)

// Yampi Desert
extern char SceneIdYampiDesertGuardianRingChest;
#define SCENE_YAMPI_DESERT_GUARDIAN_RING_CHEST ((Sint32)&SceneIdYampiDesertGuardianRingChest)
extern char SceneIdYampiDesertSpring;
#define SCENE_YAMPI_DESERT_SPRING ((Sint32)&SceneIdYampiDesertSpring)
// The Yampi Desert scene where Blitz is fought.
extern char SceneIdYampiDesertBlitz;
#define SCENE_YAMPI_DESERT_BLITZ ((Sint32)&SceneIdYampiDesertBlitz)
extern char SceneIdYampiDesertPsynergyStone;
#define SCENE_YAMPI_DESERT_PSYNERGY_STONE ((Sint32)&SceneIdYampiDesertPsynergyStone)
extern char SceneIdYampiDesertKingScorpion;
#define SCENE_YAMPI_DESERT_KING_SCORPION ((Sint32)&SceneIdYampiDesertKingScorpion)
extern char SceneIdYampiDesertCoinsPuddle;
#define SCENE_YAMPI_DESERT_COINS_PUDDLE ((Sint32)&SceneIdYampiDesertCoinsPuddle)
extern char SceneIdYampiDesertSandPass;
#define SCENE_YAMPI_DESERT_SAND_PASS ((Sint32)&SceneIdYampiDesertSandPass)
extern char SceneIdYampiDesertTrainersWhipChest;
#define SCENE_YAMPI_DESERT_TRAINERS_WHIP_CHEST ((Sint32)&SceneIdYampiDesertTrainersWhipChest)
extern char SceneIdYampiDesertHiddenPsynergyStone;
#define SCENE_YAMPI_DESERT_HIDDEN_PSYNERGY_STONE ((Sint32)&SceneIdYampiDesertHiddenPsynergyStone)
extern char SceneIdYampiDesertScorpionPit;
#define SCENE_YAMPI_DESERT_SCORPION_PIT ((Sint32)&SceneIdYampiDesertScorpionPit)
extern char SceneIdYampiDesertSpringCave;
#define SCENE_YAMPI_DESERT_SPRING_CAVE ((Sint32)&SceneIdYampiDesertSpringCave)
extern char SceneIdYampiDesertBlowMaceChest;
#define SCENE_YAMPI_DESERT_BLOW_MACE_CHEST ((Sint32)&SceneIdYampiDesertBlowMaceChest)

// Yampi Desert Cave
extern char SceneIdYampiDesertCaveWaterOfLifeChest;
#define SCENE_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST                                                \
    ((Sint32)&SceneIdYampiDesertCaveWaterOfLifeChest)
extern char SceneIdYampiDesertCaveGuardian;
#define SCENE_YAMPI_DESERT_CAVE_GUARDIAN ((Sint32)&SceneIdYampiDesertCaveGuardian)
// The Yampi Desert cave room where Crystal is fought.
extern char SceneIdYampiDesertCaveCrystal;
#define SCENE_YAMPI_DESERT_CAVE_CRYSTAL ((Sint32)&SceneIdYampiDesertCaveCrystal)

// Air's Rock
extern char SceneIdAirsRockStormBrandChest;
#define SCENE_AIRS_ROCK_STORM_BRAND_CHEST ((Sint32)&SceneIdAirsRockStormBrandChest)
extern char SceneIdAirsRockEntrance;
#define SCENE_AIRS_ROCK_ENTRANCE ((Sint32)&SceneIdAirsRockEntrance)
extern char SceneIdAirsRockFujinShieldChest;
#define SCENE_AIRS_ROCK_FUJIN_SHIELD_CHEST ((Sint32)&SceneIdAirsRockFujinShieldChest)
extern char SceneIdAirsRockInsideHall;
#define SCENE_AIRS_ROCK_INSIDE_HALL ((Sint32)&SceneIdAirsRockInsideHall)
extern char SceneIdAirsRockInsideVialChest;
#define SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST ((Sint32)&SceneIdAirsRockInsideVialChest)
extern char SceneIdAirsRockInsideClarityCircletChest;
#define SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST                                               \
    ((Sint32)&SceneIdAirsRockInsideClarityCircletChest)
extern char SceneIdAirsRockInsideTightropes;
#define SCENE_AIRS_ROCK_INSIDE_TIGHTROPES ((Sint32)&SceneIdAirsRockInsideTightropes)
// The room inside Air's Rock with the tablet that teaches Reveal.
extern char SceneIdAirsRockInsideRevealTablet;
#define SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET ((Sint32)&SceneIdAirsRockInsideRevealTablet)
extern char SceneIdAirsRockInsideElixirChest;
#define SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST ((Sint32)&SceneIdAirsRockInsideElixirChest)
extern char SceneIdAirsRockInsidePairedStakes;
#define SCENE_AIRS_ROCK_INSIDE_PAIRED_STAKES ((Sint32)&SceneIdAirsRockInsidePairedStakes)
extern char SceneIdAirsRockInsideFaceVents;
#define SCENE_AIRS_ROCK_INSIDE_FACE_VENTS ((Sint32)&SceneIdAirsRockInsideFaceVents)
extern char SceneIdAirsRockInsideCoinsChest;
#define SCENE_AIRS_ROCK_INSIDE_COINS_CHEST ((Sint32)&SceneIdAirsRockInsideCoinsChest)
// The room inside Air's Rock with a Psynergy Stone.
extern char SceneIdAirsRockInsidePsynergyStone;
#define SCENE_AIRS_ROCK_INSIDE_PSYNERGY_STONE ((Sint32)&SceneIdAirsRockInsidePsynergyStone)

// Alhafra Alhafra itself, the town's one outdoor scene.
extern char SceneIdAlhafra;
#define SCENE_ALHAFRA ((Sint32)&SceneIdAlhafra)
extern char SceneIdAlhafraRooms;
#define SCENE_ALHAFRA_ROOMS ((Sint32)&SceneIdAlhafraRooms)

// Sea of Time
extern char SceneIdSeaOfTimeSwirls;
#define SCENE_SEA_OF_TIME_SWIRLS ((Sint32)&SceneIdSeaOfTimeSwirls)
extern char SceneIdSeaOfTimeToLemuria;
#define SCENE_SEA_OF_TIME_TO_LEMURIA ((Sint32)&SceneIdSeaOfTimeToLemuria)

// Lemuria
extern char SceneIdLemuriaEntrance;
#define SCENE_LEMURIA_ENTRANCE ((Sint32)&SceneIdLemuriaEntrance)
extern char SceneIdLemuriaDock;
#define SCENE_LEMURIA_DOCK ((Sint32)&SceneIdLemuriaDock)
extern char SceneIdAncientLemuria;
#define SCENE_ANCIENT_LEMURIA ((Sint32)&SceneIdAncientLemuria)

// Madra
extern char SceneIdMadraRoomsHouses;
#define SCENE_MADRA_ROOMS_HOUSES ((Sint32)&SceneIdMadraRoomsHouses)
extern char SceneIdMadraRoomsCavernHomes;
#define SCENE_MADRA_ROOMS_CAVERN_HOMES ((Sint32)&SceneIdMadraRoomsCavernHomes)

// Madra Catacombs
extern char SceneIdMadraCatacombsEntrance;
#define SCENE_MADRA_CATACOMBS_ENTRANCE ((Sint32)&SceneIdMadraCatacombsEntrance)
extern char SceneIdMadraCatacombsLadders;
#define SCENE_MADRA_CATACOMBS_LADDERS ((Sint32)&SceneIdMadraCatacombsLadders)
extern char SceneIdMadraCatacombsChampaTunnel;
#define SCENE_MADRA_CATACOMBS_CHAMPA_TUNNEL ((Sint32)&SceneIdMadraCatacombsChampaTunnel)
extern char SceneIdMadraCatacombsStoneDoor;
#define SCENE_MADRA_CATACOMBS_STONE_DOOR ((Sint32)&SceneIdMadraCatacombsStoneDoor)
extern char SceneIdMadraCatacombsBookshelf;
#define SCENE_MADRA_CATACOMBS_BOOKSHELF ((Sint32)&SceneIdMadraCatacombsBookshelf)
extern char SceneIdMadraCatacombsTablet;
#define SCENE_MADRA_CATACOMBS_TABLET ((Sint32)&SceneIdMadraCatacombsTablet)

// Madra Drawbridge
extern char SceneIdMadraDrawbridge;
#define SCENE_MADRA_DRAWBRIDGE ((Sint32)&SceneIdMadraDrawbridge)

// Osenia Cliffs
extern char SceneIdOseniaCliffs;
#define SCENE_OSENIA_CLIFFS ((Sint32)&SceneIdOseniaCliffs)

// Gondowan Cliffs
extern char SceneIdGondowanCliffs;
#define SCENE_GONDOWAN_CLIFFS ((Sint32)&SceneIdGondowanCliffs)
extern char SceneIdGondowanCliffsPass;
#define SCENE_GONDOWAN_CLIFFS_PASS ((Sint32)&SceneIdGondowanCliffsPass)
extern char SceneIdGondowanCliffsHideout;
#define SCENE_GONDOWAN_CLIFFS_HIDEOUT ((Sint32)&SceneIdGondowanCliffsHideout)

// Naribwe
extern char SceneIdNaribwe;
#define SCENE_NARIBWE ((Sint32)&SceneIdNaribwe)
extern char SceneIdNaribweRooms;
#define SCENE_NARIBWE_ROOMS ((Sint32)&SceneIdNaribweRooms)

// Mikasalla
extern char SceneIdMikasallaRoomsHouses;
#define SCENE_MIKASALLA_ROOMS_HOUSES ((Sint32)&SceneIdMikasallaRoomsHouses)
extern char SceneIdMikasallaRoomsSanctum;
#define SCENE_MIKASALLA_ROOMS_SANCTUM ((Sint32)&SceneIdMikasallaRoomsSanctum)

// Garoh
extern char SceneIdGaroh;
#define SCENE_GAROH ((Sint32)&SceneIdGaroh)
// The path up to Garoh from its way up, whose chest holds the Hypnos Sword.
extern char SceneIdGarohPath;
#define SCENE_GAROH_PATH ((Sint32)&SceneIdGarohPath)
// The way up to Garoh from the World Map.
extern char SceneIdGarohWayUp;
#define SCENE_GAROH_WAY_UP ((Sint32)&SceneIdGarohWayUp)
extern char SceneIdGarohTunnels;
#define SCENE_GAROH_TUNNELS ((Sint32)&SceneIdGarohTunnels)
extern char SceneIdGarohWerewolfCave;
#define SCENE_GAROH_WEREWOLF_CAVE ((Sint32)&SceneIdGarohWerewolfCave)

// Alhafra
extern char SceneIdAlhafraManor;
#define SCENE_ALHAFRA_MANOR ((Sint32)&SceneIdAlhafraManor)

// Eastern Alhafra Eastern Alhafra, where Briggs's ship lies.
extern char SceneIdEasternAlhafra;
#define SCENE_EASTERN_ALHAFRA ((Sint32)&SceneIdEasternAlhafra)
extern char SceneIdEasternAlhafraShip;
#define SCENE_EASTERN_ALHAFRA_SHIP ((Sint32)&SceneIdEasternAlhafraShip)

// Alhafran Cave
extern char SceneIdAlhafranCave;
#define SCENE_ALHAFRAN_CAVE ((Sint32)&SceneIdAlhafranCave)
extern char SceneIdAlhafraJail;
#define SCENE_ALHAFRA_JAIL ((Sint32)&SceneIdAlhafraJail)

// Champa
extern char SceneIdChampaRooms;
#define SCENE_CHAMPA_ROOMS ((Sint32)&SceneIdChampaRooms)
extern char SceneIdChampaSanctum;
#define SCENE_CHAMPA_SANCTUM ((Sint32)&SceneIdChampaSanctum)

// Contigo
extern char SceneIdContigo;
#define SCENE_CONTIGO ((Sint32)&SceneIdContigo)
extern char SceneIdContigoShops;
#define SCENE_CONTIGO_SHOPS ((Sint32)&SceneIdContigoShops)
extern char SceneIdContigoLuckyDice;
#define SCENE_CONTIGO_LUCKY_DICE ((Sint32)&SceneIdContigoLuckyDice)

// Anemos Sanctum
extern char SceneIdContigoGeoglyph;
#define SCENE_CONTIGO_GEOGLYPH ((Sint32)&SceneIdContigoGeoglyph)

// Garoh
extern char SceneIdGarohRoomsHouses;
#define SCENE_GAROH_ROOMS_HOUSES ((Sint32)&SceneIdGarohRoomsHouses)
extern char SceneIdGarohRoomsShop;
#define SCENE_GAROH_ROOMS_SHOP ((Sint32)&SceneIdGarohRoomsShop)
extern char SceneIdGarohRoomsSanctum;
#define SCENE_GAROH_ROOMS_SANCTUM ((Sint32)&SceneIdGarohRoomsSanctum)

// Gabomba Statue
extern char SceneIdGabombaStatueBoneArmletChest;
#define SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST ((Sint32)&SceneIdGabombaStatueBoneArmletChest)
extern char SceneIdGabombaStatueElixirChest;
#define SCENE_GABOMBA_STATUE_ELIXIR_CHEST ((Sint32)&SceneIdGabombaStatueElixirChest)
// The room inside the Gabomba Statue where Steel is fought.
extern char SceneIdGabombaStatueSteel;
#define SCENE_GABOMBA_STATUE_STEEL ((Sint32)&SceneIdGabombaStatueSteel)
extern char SceneIdGabombaStatueMouse;
#define SCENE_GABOMBA_STATUE_MOUSE ((Sint32)&SceneIdGabombaStatueMouse)
extern char SceneIdGabombaStatueCogs;
#define SCENE_GABOMBA_STATUE_COGS ((Sint32)&SceneIdGabombaStatueCogs)
extern char SceneIdGabombaStatueArrows;
#define SCENE_GABOMBA_STATUE_ARROWS ((Sint32)&SceneIdGabombaStatueArrows)
extern char SceneIdGabombaStatuePinion;
#define SCENE_GABOMBA_STATUE_PINION ((Sint32)&SceneIdGabombaStatuePinion)

// Anemos Inner Sanctum
extern char SceneIdAnemosInnerSanctumDragonSkinChest;
#define SCENE_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST                                               \
    ((Sint32)&SceneIdAnemosInnerSanctumDragonSkinChest)
extern char SceneIdAnemosInnerSanctumElevator;
#define SCENE_ANEMOS_INNER_SANCTUM_ELEVATOR ((Sint32)&SceneIdAnemosInnerSanctumElevator)
extern char SceneIdAnemosInnerSanctumSwitches;
#define SCENE_ANEMOS_INNER_SANCTUM_SWITCHES ((Sint32)&SceneIdAnemosInnerSanctumSwitches)
extern char SceneIdAnemosInnerSanctumOrihalconChest;
#define SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST                                                 \
    ((Sint32)&SceneIdAnemosInnerSanctumOrihalconChest)
extern char SceneIdAnemosInnerSanctumGuardian;
#define SCENE_ANEMOS_INNER_SANCTUM_GUARDIAN ((Sint32)&SceneIdAnemosInnerSanctumGuardian)

// Gaia Rock
extern char SceneIdGaiaRockSusa;
#define SCENE_GAIA_ROCK_SUSA ((Sint32)&SceneIdGaiaRockSusa)
extern char SceneIdGaiaRockStatueSpots;
#define SCENE_GAIA_ROCK_STATUE_SPOTS ((Sint32)&SceneIdGaiaRockStatueSpots)
extern char SceneIdGaiaRockDancingIdol;
#define SCENE_GAIA_ROCK_DANCING_IDOL ((Sint32)&SceneIdGaiaRockDancingIdol)
extern char SceneIdGaiaRockInsideMaze;
#define SCENE_GAIA_ROCK_INSIDE_MAZE ((Sint32)&SceneIdGaiaRockInsideMaze)
extern char SceneIdGaiaRockInsideFirstAltar;
#define SCENE_GAIA_ROCK_INSIDE_FIRST_ALTAR ((Sint32)&SceneIdGaiaRockInsideFirstAltar)
extern char SceneIdGaiaRockInsideTwinAltars;
#define SCENE_GAIA_ROCK_INSIDE_TWIN_ALTARS ((Sint32)&SceneIdGaiaRockInsideTwinAltars)
extern char SceneIdGaiaRockInsidePlatforms;
#define SCENE_GAIA_ROCK_INSIDE_PLATFORMS ((Sint32)&SceneIdGaiaRockInsidePlatforms)
extern char SceneIdGaiaRockInsideAltar;
#define SCENE_GAIA_ROCK_INSIDE_ALTAR ((Sint32)&SceneIdGaiaRockInsideAltar)

// Champa Champa itself, the town's one outdoor scene.
extern char SceneIdChampa;
#define SCENE_CHAMPA ((Sint32)&SceneIdChampa)

// Magma Rock
extern char SceneIdMagmaRockSlide;
#define SCENE_MAGMA_ROCK_SLIDE ((Sint32)&SceneIdMagmaRockSlide)
extern char SceneIdMagmaRockSalamanderTailChest;
#define SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST ((Sint32)&SceneIdMagmaRockSalamanderTailChest)
extern char SceneIdMagmaRockStatue;
#define SCENE_MAGMA_ROCK_STATUE ((Sint32)&SceneIdMagmaRockStatue)
extern char SceneIdMagmaRockInsideLuckyMedalChest;
#define SCENE_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST ((Sint32)&SceneIdMagmaRockInsideLuckyMedalChest)
extern char SceneIdMagmaRockInsideEntrance;
#define SCENE_MAGMA_ROCK_INSIDE_ENTRANCE ((Sint32)&SceneIdMagmaRockInsideEntrance)
// The room inside Magma Rock where Fury is fought.
extern char SceneIdMagmaRockInsideFury;
#define SCENE_MAGMA_ROCK_INSIDE_FURY ((Sint32)&SceneIdMagmaRockInsideFury)
extern char SceneIdMagmaRockInsideCrackedPillars;
#define SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS ((Sint32)&SceneIdMagmaRockInsideCrackedPillars)
extern char SceneIdMagmaRockInsideGolemCoreChest;
#define SCENE_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST ((Sint32)&SceneIdMagmaRockInsideGolemCoreChest)
extern char SceneIdMagmaRockInsideSalamanderTailChest;
#define SCENE_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST                                              \
    ((Sint32)&SceneIdMagmaRockInsideSalamanderTailChest)
extern char SceneIdMagmaRockInsideEmberStone;
#define SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE ((Sint32)&SceneIdMagmaRockInsideEmberStone)
// The room inside Magma Rock with the tablet that teaches Blaze.
extern char SceneIdMagmaRockInsideBlazeTablet;
#define SCENE_MAGMA_ROCK_INSIDE_BLAZE_TABLET ((Sint32)&SceneIdMagmaRockInsideBlazeTablet)

// Tundaria Tower
extern char SceneIdTundariaTowerEntrance;
#define SCENE_TUNDARIA_TOWER_ENTRANCE ((Sint32)&SceneIdTundariaTowerEntrance)
extern char SceneIdTundariaTowerCenterProng;
#define SCENE_TUNDARIA_TOWER_CENTER_PRONG ((Sint32)&SceneIdTundariaTowerCenterProng)
extern char SceneIdTundariaTowerIceSlide;
#define SCENE_TUNDARIA_TOWER_ICE_SLIDE ((Sint32)&SceneIdTundariaTowerIceSlide)
// The Tundaria Tower room where Reflux is fought.
extern char SceneIdTundariaTowerReflux;
#define SCENE_TUNDARIA_TOWER_REFLUX ((Sint32)&SceneIdTundariaTowerReflux)
extern char SceneIdTundariaTowerIceColumns;
#define SCENE_TUNDARIA_TOWER_ICE_COLUMNS ((Sint32)&SceneIdTundariaTowerIceColumns)
extern char SceneIdTundariaTowerPillarPuzzle;
#define SCENE_TUNDARIA_TOWER_PILLAR_PUZZLE ((Sint32)&SceneIdTundariaTowerPillarPuzzle)
extern char SceneIdTundariaTowerBurstBrooch;
#define SCENE_TUNDARIA_TOWER_BURST_BROOCH ((Sint32)&SceneIdTundariaTowerBurstBrooch)

// Gondowan Settlement
extern char SceneIdGondowanSettlement;
#define SCENE_GONDOWAN_SETTLEMENT ((Sint32)&SceneIdGondowanSettlement)
extern char SceneIdGondowanSettlementHouses;
#define SCENE_GONDOWAN_SETTLEMENT_HOUSES ((Sint32)&SceneIdGondowanSettlementHouses)

// Shrine of the Sea God
extern char SceneIdShrineOfTheSeaGodRope;
#define SCENE_SHRINE_OF_THE_SEA_GOD_ROPE ((Sint32)&SceneIdShrineOfTheSeaGodRope)
extern char SceneIdShrineOfTheSeaGodPlankBridge;
#define SCENE_SHRINE_OF_THE_SEA_GOD_PLANK_BRIDGE ((Sint32)&SceneIdShrineOfTheSeaGodPlankBridge)
extern char SceneIdShrineOfTheSeaGodCrossroads;
#define SCENE_SHRINE_OF_THE_SEA_GOD_CROSSROADS ((Sint32)&SceneIdShrineOfTheSeaGodCrossroads)
extern char SceneIdShrineOfTheSeaGodFootprints;
#define SCENE_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS ((Sint32)&SceneIdShrineOfTheSeaGodFootprints)
// The Shrine of the Sea God room where Breath is chased and fought.
extern char SceneIdShrineOfTheSeaGodBreath;
#define SCENE_SHRINE_OF_THE_SEA_GOD_BREATH ((Sint32)&SceneIdShrineOfTheSeaGodBreath)
extern char SceneIdShrineOfTheSeaGodPillarHole;
#define SCENE_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE ((Sint32)&SceneIdShrineOfTheSeaGodPillarHole)
extern char SceneIdShrineOfTheSeaGodPillars;
#define SCENE_SHRINE_OF_THE_SEA_GOD_PILLARS ((Sint32)&SceneIdShrineOfTheSeaGodPillars)
extern char SceneIdShrineOfTheSeaGodSeaGodsTear;
#define SCENE_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR ((Sint32)&SceneIdShrineOfTheSeaGodSeaGodsTear)
extern char SceneIdShrineOfTheSeaGodStatue;
#define SCENE_SHRINE_OF_THE_SEA_GOD_STATUE ((Sint32)&SceneIdShrineOfTheSeaGodStatue)
extern char SceneIdShrineOfTheSeaGodLadders;
#define SCENE_SHRINE_OF_THE_SEA_GOD_LADDERS ((Sint32)&SceneIdShrineOfTheSeaGodLadders)
extern char SceneIdShrineOfTheSeaGodRightProng;
#define SCENE_SHRINE_OF_THE_SEA_GOD_RIGHT_PRONG ((Sint32)&SceneIdShrineOfTheSeaGodRightProng)

// Aqua Rock
extern char SceneIdAquaRockMoai;
#define SCENE_AQUA_ROCK_MOAI ((Sint32)&SceneIdAquaRockMoai)
extern char SceneIdAquaRockMistSabreChest;
#define SCENE_AQUA_ROCK_MIST_SABRE_CHEST ((Sint32)&SceneIdAquaRockMistSabreChest)

// Dehkan Plateau
extern char SceneIdDehkanPlateauFullMetalVestChest;
#define SCENE_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST ((Sint32)&SceneIdDehkanPlateauFullMetalVestChest)
extern char SceneIdDehkanPlateauThemisAxeChest;
#define SCENE_DEHKAN_PLATEAU_THEMIS_AXE_CHEST ((Sint32)&SceneIdDehkanPlateauThemisAxeChest)
extern char SceneIdDehkanPlateauTightropes;
#define SCENE_DEHKAN_PLATEAU_TIGHTROPES ((Sint32)&SceneIdDehkanPlateauTightropes)
extern char SceneIdDehkanPlateauMintChest;
#define SCENE_DEHKAN_PLATEAU_MINT_CHEST ((Sint32)&SceneIdDehkanPlateauMintChest)
extern char SceneIdDehkanPlateauPoundCube;
#define SCENE_DEHKAN_PLATEAU_POUND_CUBE ((Sint32)&SceneIdDehkanPlateauPoundCube)
// The Dehkan Plateau scene where Cannon is met and fought.
extern char SceneIdDehkanPlateauCannon;
#define SCENE_DEHKAN_PLATEAU_CANNON ((Sint32)&SceneIdDehkanPlateauCannon)

// Lemurian Ship
extern char SceneIdLemurianShipTiller;
#define SCENE_LEMURIAN_SHIP_TILLER ((Sint32)&SceneIdLemurianShipTiller)

// East Indra Shore
extern char SceneIdLemurianShipMainDeck;
#define SCENE_LEMURIAN_SHIP_MAIN_DECK ((Sint32)&SceneIdLemurianShipMainDeck)

// Madra
extern char SceneIdMadra;
#define SCENE_MADRA ((Sint32)&SceneIdMadra)

// Kibombo Mountains The Kibombo Mountains scene where Waft is fought.
extern char SceneIdKibomboMountainsWaft;
#define SCENE_KIBOMBO_MOUNTAINS_WAFT ((Sint32)&SceneIdKibomboMountainsWaft)
extern char SceneIdKibomboMountainsDiskAxeChest;
#define SCENE_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST ((Sint32)&SceneIdKibomboMountainsDiskAxeChest)
extern char SceneIdKibomboMountainsCavern;
#define SCENE_KIBOMBO_MOUNTAINS_CAVERN ((Sint32)&SceneIdKibomboMountainsCavern)

// Islet Cave The Islet Cave room where Meld is fought.
extern char SceneIdIsletCaveHalls;
#define SCENE_ISLET_CAVE_HALLS ((Sint32)&SceneIdIsletCaveHalls)
// The Islet Cave room where Serac is fought.
extern char SceneIdIsletCaveCorridors;
#define SCENE_ISLET_CAVE_CORRIDORS ((Sint32)&SceneIdIsletCaveCorridors)
// The Islet Cave room where the Sentinel waits.
extern char SceneIdIsletCaveSentinel;
#define SCENE_ISLET_CAVE_SENTINEL ((Sint32)&SceneIdIsletCaveSentinel)

// Yallam
extern char SceneIdYallam;
#define SCENE_YALLAM ((Sint32)&SceneIdYallam)
extern char SceneIdYallamCave;
#define SCENE_YALLAM_CAVE ((Sint32)&SceneIdYallamCave)

// Apojii Islands
extern char SceneIdApojiiIslands;
#define SCENE_APOJII_ISLANDS ((Sint32)&SceneIdApojiiIslands)
// The Apojii Islands scene where Haze joins the party.
extern char SceneIdApojiiIslandsHaze;
#define SCENE_APOJII_ISLANDS_HAZE ((Sint32)&SceneIdApojiiIslandsHaze)
extern char SceneIdApojiiIslandsShops;
#define SCENE_APOJII_ISLANDS_SHOPS ((Sint32)&SceneIdApojiiIslandsShops)

// Apojii Islands Cave
extern char SceneIdApojiiIslandsCave;
#define SCENE_APOJII_ISLANDS_CAVE ((Sint32)&SceneIdApojiiIslandsCave)

// Aqua Rock
extern char SceneIdAquaRockInsideWaterOfLifeChest;
#define SCENE_AQUA_ROCK_INSIDE_WATER_OF_LIFE_CHEST ((Sint32)&SceneIdAquaRockInsideWaterOfLifeChest)
// The room inside Aqua Rock with the tablet that teaches Parch.
extern char SceneIdAquaRockInsideParchTablet;
#define SCENE_AQUA_ROCK_INSIDE_PARCH_TABLET ((Sint32)&SceneIdAquaRockInsideParchTablet)

// Izumo
extern char SceneIdIzumoRooms;
#define SCENE_IZUMO_ROOMS ((Sint32)&SceneIdIzumoRooms)
extern char SceneIdIzumo;
#define SCENE_IZUMO ((Sint32)&SceneIdIzumo)

// Izumo Ruins
extern char SceneIdIzumoRuinsFirstRoom;
#define SCENE_IZUMO_RUINS_FIRST_ROOM ((Sint32)&SceneIdIzumoRuinsFirstRoom)
extern char SceneIdIzumoRuinsSecondRoom;
#define SCENE_IZUMO_RUINS_SECOND_ROOM ((Sint32)&SceneIdIzumoRuinsSecondRoom)

// Aqua Rock The room inside Aqua Rock where Steam is fought.
extern char SceneIdAquaRockInsideSteam;
#define SCENE_AQUA_ROCK_INSIDE_STEAM ((Sint32)&SceneIdAquaRockInsideSteam)
extern char SceneIdAquaRockInsideAquariusStone;
#define SCENE_AQUA_ROCK_INSIDE_AQUARIUS_STONE ((Sint32)&SceneIdAquaRockInsideAquariusStone)
extern char SceneIdAquaRockInsideRustySwordChest;
#define SCENE_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST ((Sint32)&SceneIdAquaRockInsideRustySwordChest)
extern char SceneIdAquaRockInsideCrystalPowderChest;
#define SCENE_AQUA_ROCK_INSIDE_CRYSTAL_POWDER_CHEST                                                \
    ((Sint32)&SceneIdAquaRockInsideCrystalPowderChest)
extern char SceneIdAquaRockInsideVialChest;
#define SCENE_AQUA_ROCK_INSIDE_VIAL_CHEST ((Sint32)&SceneIdAquaRockInsideVialChest)
// The room inside Aqua Rock with a Psynergy Stone.
extern char SceneIdAquaRockInsidePsynergyStone;
#define SCENE_AQUA_ROCK_INSIDE_PSYNERGY_STONE ((Sint32)&SceneIdAquaRockInsidePsynergyStone)
extern char SceneIdAquaRockInsideTightrope;
#define SCENE_AQUA_ROCK_INSIDE_TIGHTROPE ((Sint32)&SceneIdAquaRockInsideTightrope)
extern char SceneIdAquaRockInsideTearStone;
#define SCENE_AQUA_ROCK_INSIDE_TEAR_STONE ((Sint32)&SceneIdAquaRockInsideTearStone)

// Atteka Inlet
extern char SceneIdAttekaInlet;
#define SCENE_ATTEKA_INLET ((Sint32)&SceneIdAttekaInlet)
// The Atteka Inlet scene where Cyclone uncovers Geode.
extern char SceneIdAttekaInletGeode;
#define SCENE_ATTEKA_INLET_GEODE ((Sint32)&SceneIdAttekaInletGeode)

// Prox
extern char SceneIdProxSouthGate;
#define SCENE_PROX_SOUTH_GATE ((Sint32)&SceneIdProxSouthGate)
extern char SceneIdProx;
#define SCENE_PROX ((Sint32)&SceneIdProx)
extern char SceneIdProxMold;
#define SCENE_PROX_MOLD ((Sint32)&SceneIdProxMold)
extern char SceneIdProxHouses;
#define SCENE_PROX_HOUSES ((Sint32)&SceneIdProxHouses)
extern char SceneIdProxCaveHomes;
#define SCENE_PROX_CAVE_HOMES ((Sint32)&SceneIdProxCaveHomes)
extern char SceneIdProxCaves;
#define SCENE_PROX_CAVES ((Sint32)&SceneIdProxCaves)

// Indra Cavern
extern char SceneIdCavernsIndra;
#define SCENE_CAVERNS_INDRA ((Sint32)&SceneIdCavernsIndra)

// Osenia Cavern
extern char SceneIdCavernsOsenia;
#define SCENE_CAVERNS_OSENIA ((Sint32)&SceneIdCavernsOsenia)

// Atteka Cavern
extern char SceneIdCavernsAtteka;
#define SCENE_CAVERNS_ATTEKA ((Sint32)&SceneIdCavernsAtteka)

// Angara Cavern
extern char SceneIdCavernsAngara;
#define SCENE_CAVERNS_ANGARA ((Sint32)&SceneIdCavernsAngara)

// Mars Lighthouse
extern char SceneIdMarsLighthouseOutside;
#define SCENE_MARS_LIGHTHOUSE_OUTSIDE ((Sint32)&SceneIdMarsLighthouseOutside)
// The Mars Lighthouse room where Fugue is fought.
extern char SceneIdMarsLighthouseEntranceFugue;
#define SCENE_MARS_LIGHTHOUSE_ENTRANCE_FUGUE ((Sint32)&SceneIdMarsLighthouseEntranceFugue)
extern char SceneIdMarsLighthouseEntranceTeleportLapis;
#define SCENE_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS                                              \
    ((Sint32)&SceneIdMarsLighthouseEntranceTeleportLapis)
// The Mars Lighthouse room where Balm is fought.
extern char SceneIdMarsLighthouseEntranceBalm;
#define SCENE_MARS_LIGHTHOUSE_ENTRANCE_BALM ((Sint32)&SceneIdMarsLighthouseEntranceBalm)
extern char SceneIdMarsLighthouseEntranceHiddenCircle;
#define SCENE_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE                                               \
    ((Sint32)&SceneIdMarsLighthouseEntranceHiddenCircle)
extern char SceneIdMarsLighthouseEntranceValkyrieMailChest;
#define SCENE_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST                                         \
    ((Sint32)&SceneIdMarsLighthouseEntranceValkyrieMailChest)
extern char SceneIdMarsLighthouseEntranceSolBladeChest;
#define SCENE_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST                                             \
    ((Sint32)&SceneIdMarsLighthouseEntranceSolBladeChest)
// Mars Lighthouse's room of the Mars Star.
extern char SceneIdMarsLighthouseStarRoom;
#define SCENE_MARS_LIGHTHOUSE_STAR_ROOM ((Sint32)&SceneIdMarsLighthouseStarRoom)
extern char SceneIdMarsLighthouseHall;
#define SCENE_MARS_LIGHTHOUSE_HALL ((Sint32)&SceneIdMarsLighthouseHall)
extern char SceneIdMarsLighthouseMercuryLedge;
#define SCENE_MARS_LIGHTHOUSE_MERCURY_LEDGE ((Sint32)&SceneIdMarsLighthouseMercuryLedge)
extern char SceneIdMarsLighthouseJupiterLedge;
#define SCENE_MARS_LIGHTHOUSE_JUPITER_LEDGE ((Sint32)&SceneIdMarsLighthouseJupiterLedge)
extern char SceneIdMarsLighthouseMarsLedge;
#define SCENE_MARS_LIGHTHOUSE_MARS_LEDGE ((Sint32)&SceneIdMarsLighthouseMarsLedge)
extern char SceneIdMarsLighthouseVenusLedge;
#define SCENE_MARS_LIGHTHOUSE_VENUS_LEDGE ((Sint32)&SceneIdMarsLighthouseVenusLedge)
extern char SceneIdMarsLighthouseMarsTower;
#define SCENE_MARS_LIGHTHOUSE_MARS_TOWER ((Sint32)&SceneIdMarsLighthouseMarsTower)
extern char SceneIdMarsLighthouseMercuryTower;
#define SCENE_MARS_LIGHTHOUSE_MERCURY_TOWER ((Sint32)&SceneIdMarsLighthouseMercuryTower)
extern char SceneIdMarsLighthouseJupiterTower;
#define SCENE_MARS_LIGHTHOUSE_JUPITER_TOWER ((Sint32)&SceneIdMarsLighthouseJupiterTower)
extern char SceneIdMarsLighthouseVenusTower;
#define SCENE_MARS_LIGHTHOUSE_VENUS_TOWER ((Sint32)&SceneIdMarsLighthouseVenusTower)
extern char SceneIdMarsLighthouseBeaconLedge;
#define SCENE_MARS_LIGHTHOUSE_BEACON_LEDGE ((Sint32)&SceneIdMarsLighthouseBeaconLedge)
// Mars Lighthouse's aerie, where the Doom Dragon is faced.
extern char SceneIdMarsLighthouseAerie;
#define SCENE_MARS_LIGHTHOUSE_AERIE ((Sint32)&SceneIdMarsLighthouseAerie)

// Treasure Isle
extern char SceneIdTreasureIsleEntrance;
#define SCENE_TREASURE_ISLE_ENTRANCE ((Sint32)&SceneIdTreasureIsleEntrance)
extern char SceneIdTreasureIsleChests;
#define SCENE_TREASURE_ISLE_CHESTS ((Sint32)&SceneIdTreasureIsleChests)
extern char SceneIdTreasureIsleJestersArmletChest;
#define SCENE_TREASURE_ISLE_JESTERS_ARMLET_CHEST ((Sint32)&SceneIdTreasureIsleJestersArmletChest)
// The Treasure Isle room where Gale is fought.
extern char SceneIdTreasureIsleGale;
#define SCENE_TREASURE_ISLE_GALE ((Sint32)&SceneIdTreasureIsleGale)
extern char SceneIdTreasureIsleGuardian;
#define SCENE_TREASURE_ISLE_GUARDIAN ((Sint32)&SceneIdTreasureIsleGuardian)

// Kibombo
extern char SceneIdKibombo;
#define SCENE_KIBOMBO ((Sint32)&SceneIdKibombo)
extern char SceneIdKibomboStatueGrounds;
#define SCENE_KIBOMBO_STATUE_GROUNDS ((Sint32)&SceneIdKibomboStatueGrounds)
extern char SceneIdKibomboHouses;
#define SCENE_KIBOMBO_HOUSES ((Sint32)&SceneIdKibomboHouses)
extern char SceneIdKibomboCeremony;
#define SCENE_KIBOMBO_CEREMONY ((Sint32)&SceneIdKibomboCeremony)
extern char SceneIdKibomboStatueGroundsCeremony;
#define SCENE_KIBOMBO_STATUE_GROUNDS_CEREMONY ((Sint32)&SceneIdKibomboStatueGroundsCeremony)

// Gabomba Statue
extern char SceneIdGabombaStatueFace;
#define SCENE_GABOMBA_STATUE_FACE ((Sint32)&SceneIdGabombaStatueFace)

// Gabomba Catacombs
extern char SceneIdGabombaCatacombsTeleporters;
#define SCENE_GABOMBA_CATACOMBS_TELEPORTERS ((Sint32)&SceneIdGabombaCatacombsTeleporters)
// The Gabomba Catacombs room where Mud is fought.
extern char SceneIdGabombaCatacombsMud;
#define SCENE_GABOMBA_CATACOMBS_MUD ((Sint32)&SceneIdGabombaCatacombsMud)
extern char SceneIdGabombaCatacombsHiddenWall;
#define SCENE_GABOMBA_CATACOMBS_HIDDEN_WALL ((Sint32)&SceneIdGabombaCatacombsHiddenWall)
extern char SceneIdGabombaCatacombsTomegathericon;
#define SCENE_GABOMBA_CATACOMBS_TOMEGATHERICON ((Sint32)&SceneIdGabombaCatacombsTomegathericon)

// Shaman Village Cave
extern char SceneIdShamanVillageCave;
#define SCENE_SHAMAN_VILLAGE_CAVE ((Sint32)&SceneIdShamanVillageCave)

// Shaman Village
extern char SceneIdShamanVillageTrialGrounds;
#define SCENE_SHAMAN_VILLAGE_TRIAL_GROUNDS ((Sint32)&SceneIdShamanVillageTrialGrounds)
extern char SceneIdShamanVillage;
#define SCENE_SHAMAN_VILLAGE ((Sint32)&SceneIdShamanVillage)
extern char SceneIdShamanVillageNorthEnd;
#define SCENE_SHAMAN_VILLAGE_NORTH_END ((Sint32)&SceneIdShamanVillageNorthEnd)
extern char SceneIdShamanVillageHouses;
#define SCENE_SHAMAN_VILLAGE_HOUSES ((Sint32)&SceneIdShamanVillageHouses)
extern char SceneIdShamanVillageTents;
#define SCENE_SHAMAN_VILLAGE_TENTS ((Sint32)&SceneIdShamanVillageTents)
extern char SceneIdShamanVillageSanctum;
#define SCENE_SHAMAN_VILLAGE_SANCTUM ((Sint32)&SceneIdShamanVillageSanctum)

// Lemurian Ship
extern char SceneIdLemurianShipCabins;
#define SCENE_LEMURIAN_SHIP_CABINS ((Sint32)&SceneIdLemurianShipCabins)
// The Lemurian Ship's hold, where the Aqua Hydra is fought.
extern char SceneIdLemurianShipAquaHydra;
#define SCENE_LEMURIAN_SHIP_AQUA_HYDRA ((Sint32)&SceneIdLemurianShipAquaHydra)

// Yallam
extern char SceneIdYallamRoomsInn;
#define SCENE_YALLAM_ROOMS_INN ((Sint32)&SceneIdYallamRoomsInn)
extern char SceneIdYallamRoomsShops;
#define SCENE_YALLAM_ROOMS_SHOPS ((Sint32)&SceneIdYallamRoomsShops)

// Taopo Swamp
extern char SceneIdTaopoSwampEntrance;
#define SCENE_TAOPO_SWAMP_ENTRANCE ((Sint32)&SceneIdTaopoSwampEntrance)
extern char SceneIdTaopoSwampMud;
#define SCENE_TAOPO_SWAMP_MUD ((Sint32)&SceneIdTaopoSwampMud)
extern char SceneIdTaopoSwampTearStones;
#define SCENE_TAOPO_SWAMP_TEAR_STONES ((Sint32)&SceneIdTaopoSwampTearStones)
// The Taopo Swamp scene where Flower is fought.
extern char SceneIdTaopoSwampFlower;
#define SCENE_TAOPO_SWAMP_FLOWER ((Sint32)&SceneIdTaopoSwampFlower)
extern char SceneIdTaopoSwampVents;
#define SCENE_TAOPO_SWAMP_VENTS ((Sint32)&SceneIdTaopoSwampVents)
extern char SceneIdTaopoSwampPool;
#define SCENE_TAOPO_SWAMP_POOL ((Sint32)&SceneIdTaopoSwampPool)
extern char SceneIdTaopoSwampPillarGap;
#define SCENE_TAOPO_SWAMP_PILLAR_GAP ((Sint32)&SceneIdTaopoSwampPillarGap)
extern char SceneIdTaopoSwampDrips;
#define SCENE_TAOPO_SWAMP_DRIPS ((Sint32)&SceneIdTaopoSwampDrips)
extern char SceneIdTaopoSwampLavaRocks;
#define SCENE_TAOPO_SWAMP_LAVA_ROCKS ((Sint32)&SceneIdTaopoSwampLavaRocks)
extern char SceneIdTaopoSwampTremor;
#define SCENE_TAOPO_SWAMP_TREMOR ((Sint32)&SceneIdTaopoSwampTremor)
extern char SceneIdTaopoSwampStarDust;
#define SCENE_TAOPO_SWAMP_STAR_DUST ((Sint32)&SceneIdTaopoSwampStarDust)

// Gabomba Statue
extern char SceneIdGabombaStatueChamberIdol;
#define SCENE_GABOMBA_STATUE_CHAMBER_IDOL ((Sint32)&SceneIdGabombaStatueChamberIdol)
extern char SceneIdGabombaStatueChamberLift;
#define SCENE_GABOMBA_STATUE_CHAMBER_LIFT ((Sint32)&SceneIdGabombaStatueChamberLift)
extern char SceneIdGabombaStatueChamberWitchDoctor;
#define SCENE_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR ((Sint32)&SceneIdGabombaStatueChamberWitchDoctor)

// Champa
extern char SceneIdChampaCliffCavesBriggs;
#define SCENE_CHAMPA_CLIFF_CAVES_BRIGGS ((Sint32)&SceneIdChampaCliffCavesBriggs)
extern char SceneIdChampaCliffCavesHouses;
#define SCENE_CHAMPA_CLIFF_CAVES_HOUSES ((Sint32)&SceneIdChampaCliffCavesHouses)
extern char SceneIdChampaCliffCavesObaba;
#define SCENE_CHAMPA_CLIFF_CAVES_OBABA ((Sint32)&SceneIdChampaCliffCavesObaba)
// The Champa cliff cave where the Avimander is fought and the Trident taken.
extern char SceneIdChampaCliffCavesAvimander;
#define SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER ((Sint32)&SceneIdChampaCliffCavesAvimander)

// Ankohl Ruins
extern char SceneIdAnkohlRuinsStoneFaces;
#define SCENE_ANKOHL_RUINS_STONE_FACES ((Sint32)&SceneIdAnkohlRuinsStoneFaces)
extern char SceneIdAnkohlRuinsThanatosMaceChest;
#define SCENE_ANKOHL_RUINS_THANATOS_MACE_CHEST ((Sint32)&SceneIdAnkohlRuinsThanatosMaceChest)
extern char SceneIdAnkohlRuinsGreatFace;
#define SCENE_ANKOHL_RUINS_GREAT_FACE ((Sint32)&SceneIdAnkohlRuinsGreatFace)
extern char SceneIdAnkohlRuinsFaceHalves;
#define SCENE_ANKOHL_RUINS_FACE_HALVES ((Sint32)&SceneIdAnkohlRuinsFaceHalves)
extern char SceneIdAnkohlRuinsCrystalPowderChest;
#define SCENE_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST ((Sint32)&SceneIdAnkohlRuinsCrystalPowderChest)
extern char SceneIdAnkohlRuinsSandDoors;
#define SCENE_ANKOHL_RUINS_SAND_DOORS ((Sint32)&SceneIdAnkohlRuinsSandDoors)
extern char SceneIdAnkohlRuinsEntrance;
#define SCENE_ANKOHL_RUINS_ENTRANCE ((Sint32)&SceneIdAnkohlRuinsEntrance)
extern char SceneIdAnkohlRuinsLeftProng;
#define SCENE_ANKOHL_RUINS_LEFT_PRONG ((Sint32)&SceneIdAnkohlRuinsLeftProng)

// Anemos Inner Sanctum
extern char SceneIdAnemosInnerSanctumEntrance;
#define SCENE_ANEMOS_INNER_SANCTUM_ENTRANCE ((Sint32)&SceneIdAnemosInnerSanctumEntrance)

// Mercury Lighthouse
extern char SceneIdMercuryLighthouse;
#define SCENE_MERCURY_LIGHTHOUSE ((Sint32)&SceneIdMercuryLighthouse)

// Epilogue
extern char SceneIdEpilogue;
#define SCENE_EPILOGUE ((Sint32)&SceneIdEpilogue)

// Mt. Aleph Summit
extern char SceneIdMtAlephSummit;
#define SCENE_MT_ALEPH_SUMMIT ((Sint32)&SceneIdMtAlephSummit)

// Link Lobby
extern char SceneIdLobby;
#define SCENE_LOBBY ((Sint32)&SceneIdLobby)

// Test 0
extern char SceneIdTest0;
#define SCENE_TEST_0 ((Sint32)&SceneIdTest0)

// Test 1
extern char SceneIdTest1;
#define SCENE_TEST_1 ((Sint32)&SceneIdTest1)

// Test 2
extern char SceneIdTest2;
#define SCENE_TEST_2 ((Sint32)&SceneIdTest2)

// Test 3
extern char SceneIdTest3;
#define SCENE_TEST_3 ((Sint32)&SceneIdTest3)

// Test 4
extern char SceneIdTest4;
#define SCENE_TEST_4 ((Sint32)&SceneIdTest4)

// Test 5
extern char SceneIdTest5;
#define SCENE_TEST_5 ((Sint32)&SceneIdTest5)

// Test 6
extern char SceneIdTest6;
#define SCENE_TEST_6 ((Sint32)&SceneIdTest6)

// Test 7
extern char SceneIdTest7;
#define SCENE_TEST_7 ((Sint32)&SceneIdTest7)

// Icon Test
extern char SceneIdIconTest;
#define SCENE_ICON_TEST ((Sint32)&SceneIdIconTest)

// Psynergy Test
extern char SceneIdPsynergyTest;
#define SCENE_PSYNERGY_TEST ((Sint32)&SceneIdPsynergyTest)

// Psynergy Test 2
extern char SceneIdPsynergyTest2;
#define SCENE_PSYNERGY_TEST_2 ((Sint32)&SceneIdPsynergyTest2)

// Shop Test
extern char SceneIdShopTest;
#define SCENE_SHOP_TEST ((Sint32)&SceneIdShopTest)

// Trial Road
extern char SceneIdTrialRoadStages;
#define SCENE_TRIAL_ROAD_STAGES ((Sint32)&SceneIdTrialRoadStages)
extern char SceneIdTrialRoadSummit;
#define SCENE_TRIAL_ROAD_SUMMIT ((Sint32)&SceneIdTrialRoadSummit)
extern char SceneIdTrialRoadCaveRock;
#define SCENE_TRIAL_ROAD_CAVE_ROCK ((Sint32)&SceneIdTrialRoadCaveRock)
// The Trial Road scene where Gasp is fought.
extern char SceneIdTrialRoadGasp;
#define SCENE_TRIAL_ROAD_GASP ((Sint32)&SceneIdTrialRoadGasp)

// Jupiter Lighthouse
extern char SceneIdJupiterLighthouseEntrance;
#define SCENE_JUPITER_LIGHTHOUSE_ENTRANCE ((Sint32)&SceneIdJupiterLighthouseEntrance)
extern char SceneIdJupiterLighthouseBasementTeleporters;
#define SCENE_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS                                              \
    ((Sint32)&SceneIdJupiterLighthouseBasementTeleporters)
extern char SceneIdJupiterLighthouseBasementPsynergyStone;
#define SCENE_JUPITER_LIGHTHOUSE_BASEMENT_PSYNERGY_STONE                                           \
    ((Sint32)&SceneIdJupiterLighthouseBasementPsynergyStone)
extern char SceneIdJupiterLighthouseRoomGateButton;
#define SCENE_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON ((Sint32)&SceneIdJupiterLighthouseRoomGateButton)
extern char SceneIdJupiterLighthouseRoomBeam;
#define SCENE_JUPITER_LIGHTHOUSE_ROOM_BEAM ((Sint32)&SceneIdJupiterLighthouseRoomBeam)
extern char SceneIdJupiterLighthouseRoomErinyesTunicChest;
#define SCENE_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST                                          \
    ((Sint32)&SceneIdJupiterLighthouseRoomErinyesTunicChest)
extern char SceneIdJupiterLighthouseRoomLockedDoors;
#define SCENE_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS                                                 \
    ((Sint32)&SceneIdJupiterLighthouseRoomLockedDoors)
extern char SceneIdJupiterLighthouseRoomRedKey;
#define SCENE_JUPITER_LIGHTHOUSE_ROOM_RED_KEY ((Sint32)&SceneIdJupiterLighthouseRoomRedKey)
extern char SceneIdJupiterLighthouseRoomCrackedFloor;
#define SCENE_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR                                                \
    ((Sint32)&SceneIdJupiterLighthouseRoomCrackedFloor)
// The Jupiter Lighthouse room where Whorl is fought.
extern char SceneIdJupiterLighthouseWhorl;
#define SCENE_JUPITER_LIGHTHOUSE_WHORL ((Sint32)&SceneIdJupiterLighthouseWhorl)
extern char SceneIdJupiterLighthouseRoomBlueKey;
#define SCENE_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY ((Sint32)&SceneIdJupiterLighthouseRoomBlueKey)
extern char SceneIdJupiterLighthouseRoomPhaetonsBladeChest;
#define SCENE_JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST                                         \
    ((Sint32)&SceneIdJupiterLighthouseRoomPhaetonsBladeChest)

// Gondowan Settlement
extern char SceneIdGondowanSettlementCave;
#define SCENE_GONDOWAN_SETTLEMENT_CAVE ((Sint32)&SceneIdGondowanSettlementCave)

// Hesperia Settlement
extern char SceneIdHesperiaSettlement;
#define SCENE_HESPERIA_SETTLEMENT ((Sint32)&SceneIdHesperiaSettlement)
extern char SceneIdHesperiaSettlementHouses;
#define SCENE_HESPERIA_SETTLEMENT_HOUSES ((Sint32)&SceneIdHesperiaSettlementHouses)
extern char SceneIdHesperiaSettlementCave;
#define SCENE_HESPERIA_SETTLEMENT_CAVE ((Sint32)&SceneIdHesperiaSettlementCave)

// Kalt Island
extern char SceneIdKaltIsland;
#define SCENE_KALT_ISLAND ((Sint32)&SceneIdKaltIsland)
extern char SceneIdKaltIslandHouses;
#define SCENE_KALT_ISLAND_HOUSES ((Sint32)&SceneIdKaltIslandHouses)

// Southwest Atteka Islet
extern char SceneIdSouthwestAttekaIslet;
#define SCENE_SOUTHWEST_ATTEKA_ISLET ((Sint32)&SceneIdSouthwestAttekaIslet)
extern char SceneIdSouthwestAttekaIsletHouses;
#define SCENE_SOUTHWEST_ATTEKA_ISLET_HOUSES ((Sint32)&SceneIdSouthwestAttekaIsletHouses)

// Mikasalla
extern char SceneIdMikasalla;
#define SCENE_MIKASALLA ((Sint32)&SceneIdMikasalla)
extern char SceneIdMikasallaNorth;
#define SCENE_MIKASALLA_NORTH ((Sint32)&SceneIdMikasallaNorth)

// Lemuria
extern char SceneIdLemuria;
#define SCENE_LEMURIA ((Sint32)&SceneIdLemuria)
extern char SceneIdLemuriaRooms;
#define SCENE_LEMURIA_ROOMS ((Sint32)&SceneIdLemuriaRooms)
extern char SceneIdLemuriaPalace;
#define SCENE_LEMURIA_PALACE ((Sint32)&SceneIdLemuriaPalace)
extern char SceneIdLemuriaSpring;
#define SCENE_LEMURIA_SPRING ((Sint32)&SceneIdLemuriaSpring)

// Jupiter Lighthouse
extern char SceneIdJupiterLighthouseTerraces;
#define SCENE_JUPITER_LIGHTHOUSE_TERRACES ((Sint32)&SceneIdJupiterLighthouseTerraces)

// Gabomba Catacombs
extern char SceneIdGabombaCatacombsStream;
#define SCENE_GABOMBA_CATACOMBS_STREAM ((Sint32)&SceneIdGabombaCatacombsStream)

// West Indra Islet
extern char SceneIdWestIndraIslet;
#define SCENE_WEST_INDRA_ISLET ((Sint32)&SceneIdWestIndraIslet)

// East Tundaria Islet
extern char SceneIdEastTundariaIslet;
#define SCENE_EAST_TUNDARIA_ISLET ((Sint32)&SceneIdEastTundariaIslet)

// North Osenia Islet
extern char SceneIdNorthOseniaIslet;
#define SCENE_NORTH_OSENIA_ISLET ((Sint32)&SceneIdNorthOseniaIslet)

// Southeast Angara Islet
extern char SceneIdSoutheastAngaraIslet;
#define SCENE_SOUTHEAST_ANGARA_ISLET ((Sint32)&SceneIdSoutheastAngaraIslet)

// Sea of Time Islet
extern char SceneIdSeaOfTimeIslet;
#define SCENE_SEA_OF_TIME_ISLET ((Sint32)&SceneIdSeaOfTimeIslet)

// Jupiter Lighthouse
extern char SceneIdJupiterLighthouseAerie;
#define SCENE_JUPITER_LIGHTHOUSE_AERIE ((Sint32)&SceneIdJupiterLighthouseAerie)
extern char SceneIdJupiterLighthouseAerieBeacon;
#define SCENE_JUPITER_LIGHTHOUSE_AERIE_BEACON ((Sint32)&SceneIdJupiterLighthouseAerieBeacon)

// Loho
extern char SceneIdLoho;
#define SCENE_LOHO ((Sint32)&SceneIdLoho)
extern char SceneIdLohoRooms;
#define SCENE_LOHO_ROOMS ((Sint32)&SceneIdLohoRooms)

// Northern Reaches
extern char SceneIdNorthernReaches;
#define SCENE_NORTHERN_REACHES ((Sint32)&SceneIdNorthernReaches)

#endif
