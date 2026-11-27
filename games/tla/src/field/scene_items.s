	.include "enums/flags.inc"
	.include "enums/item_ids.inc"
	.include "enums/game/game.inc"
	.include "enums/src/field/field_sequence_internal.inc"

@ What a treasure spot is comes from CONTAINER_KIND
@ (field_sequence_internal.h). The rest the build cannot hand to assembly,
@ since their headers compute values: four of EXAMINE
@ (field/scene_tables.h), the mark on a scene's header and on coins
@ (field_sequence_internal.h), and the nine Mimics counted from the Kandorean
@ Temple's (enemy_ids.h).
	.equiv EXAMINE_JAR, 2
	.equiv EXAMINE_BARREL, 3
	.equiv EXAMINE_GRAVE, 8
	.equiv EXAMINE_WOODEN_BOX, 13
	.equiv SCENE_ITEMS_HEADER, 1 << 14
	.equiv CONTAINER_COINS, 1 << 15
	.equiv MIMIC_KANDOREAN_TEMPLE, 0
	.equiv MIMIC_AIRS_ROCK, 1
	.equiv MIMIC_GABOMBA_STATUE, 2
	.equiv MIMIC_AQUA_ROCK, 3
	.equiv MIMIC_GAIA_ROCK, 4
	.equiv MIMIC_TREASURE_ISLE, 5
	.equiv MIMIC_JUPITER_LIGHTHOUSE, 6
	.equiv MIMIC_MAGMA_ROCK, 7
	.equiv MIMIC_MARS_LIGHTHOUSE, 8

@ The treasure of each scene (SceneItems): its chests and Mimics, Psynergy
@ Stones, summon tablets, the items lying about or in the shallows, and what
@ is hidden in jars, barrels, graves and wooden boxes. fdPlaceTreasureSpots
@ makes a scene's rows its actors from FIELD_TREASURE_ACTOR on, sixteen at
@ most, and scTalkToActor opens them.
@
@ A scene's header: its scene marked SCENE_ITEMS_HEADER, and a flag (-1 for
@ none) whose being set passes these rows over for a later header of the same
@ scene (fdSceneItemGetRows).
	.macro scene_items scene:req, flag:req
	.2byte SCENE_ITEMS_HEADER + SceneId_\scene, \flag
	.endm

@ A treasure spot: what it is (for something hidden in a jar, barrel, grave or
@ wooden box, an unseen spot Reveal makes sparkle), the scene point it stands
@ on, the flag set once it is taken (-1 for none; a summon tablet's is its
@ summon) and what it holds: an item, CONTAINER_COINS and their number, which
@ Mimic jumps out, a summon tablet's summon, or 0 for a Psynergy Stone, whose
@ area flag brings it back once the party has left the area.
	.macro item kind:req, point:req, flag:req, contents:req
	.2byte \kind, \point, \flag, \contents
	.endm

@ Each scene's header and its rows, the scenes in no order; a header of 0 ends
@ the table.
	.data
	.balign 2
	.global SceneItems
SceneItems:
	scene_items VenusLighthouse, -1
	@ "The chest was already open." before it gives the Herb.
	item CONTAINER_CHEST, 0, 0xf00, ITEM_HERB
	scene_items SuhallaGatePsynergyStone, -1
	item CONTAINER_PSYNERGY_STONE, 1, FLAG_AREA_TEMP_F0, 0
	scene_items DailaFlooded, -1
	item EXAMINE_JAR, 1, 0xf01, ITEM_HERB
	item EXAMINE_BARREL, 2, 0xf02, ITEM_SMOKE_BOMB
	scene_items Daila, -1
	item EXAMINE_JAR, 1, 0xf01, ITEM_HERB
	item EXAMINE_BARREL, 2, 0xf02, ITEM_SMOKE_BOMB
	item CONTAINER_GROUND, 100, 0xf5e, ITEM_PSY_CRYSTAL
	scene_items DailaRoomsHouses, -1
	item EXAMINE_BARREL, 4, 0xf12, ITEM_LUCKY_MEDAL
	item EXAMINE_BARREL, 1, 0xf03, ITEM_SLEEP_BOMB
	item EXAMINE_JAR, 2, 0xf04, CONTAINER_COINS + 3
	item EXAMINE_JAR, 3, 0xf05, CONTAINER_COINS + 12
	scene_items KandoreanTempleLifts, -1
	item CONTAINER_CHEST, 10, 0xf06, ITEM_MYSTERIOUS_CARD
	scene_items KandoreanTempleMimic, -1
	item CONTAINER_MIMIC, 100, 0xe70, MIMIC_KANDOREAN_TEMPLE
	scene_items DehkanPlateauFullMetalVestChest, -1
	item CONTAINER_CHEST, 1, 0xf07, ITEM_FULL_METAL_VEST
	item CONTAINER_CHEST, 2, 0xf08, ITEM_ELIXIR
	scene_items DehkanPlateauMintChest, -1
	item CONTAINER_CHEST, 1, 0xf09, ITEM_MINT
	scene_items DehkanPlateauThemisAxeChest, -1
	item CONTAINER_CHEST, 20, 0xf0a, ITEM_THEMIS_AXE
	scene_items DehkanPlateauTightropes, -1
	item CONTAINER_CHEST, 20, 0xf0b, ITEM_NUT
	scene_items Madra, -1
	item CONTAINER_CHEST, 1, 0xf0c, ITEM_NURSES_CAP
	item EXAMINE_WOODEN_BOX, 2, 0xf0d, ITEM_ANTIDOTE
	item EXAMINE_JAR, 100, 0x918, ITEM_CYCLONE_CHIP
	scene_items MadraRoomsHouses, -1
	item EXAMINE_BARREL, 1, 0xf0e, ITEM_SMOKE_BOMB
	item EXAMINE_WOODEN_BOX, 2, 0xf0f, CONTAINER_COINS + 15
	item EXAMINE_JAR, 3, 0xf10, ITEM_SLEEP_BOMB
	scene_items MadraRoomsCavernHomes, -1
	item EXAMINE_JAR, 1, 0xf11, ITEM_ELIXIR
	scene_items MadraCatacombsEntrance, -1
	item CONTAINER_CHEST, 2, 0xf13, ITEM_APPLE
	item CONTAINER_CHEST, 1, 0xf42, ITEM_MIST_POTION
	scene_items MadraCatacombsStoneDoor, -1
	item CONTAINER_CHEST, 1, 0xf14, ITEM_LUCKY_MEDAL
	item CONTAINER_CHEST, 2, 0xf15, ITEM_RUIN_KEY
	scene_items MadraCatacombsBookshelf, -1
	item CONTAINER_CHEST, 1, 0xf15, ITEM_RUIN_KEY
	scene_items MadraCatacombsTablet, -1
	item CONTAINER_CHEST, 1, 0xf16, ITEM_TREMOR_BIT
	item CONTAINER_STONE_TABLET, 2, SUMMON_MOLOCH, SUMMON_MOLOCH
	scene_items OseniaCliffs, -1
	item CONTAINER_CHEST, 1, 0xf17, ITEM_PIRATES_SWORD
	scene_items YampiDesertGuardianRingChest, -1
	item CONTAINER_CHEST, 1, 0xf18, ITEM_GUARDIAN_RING
	scene_items YampiDesertBlitz, -1
	item CONTAINER_CHEST, 1, 0xf19, ITEM_ANTIDOTE
	scene_items YampiDesertPsynergyStone, -1
	item CONTAINER_PSYNERGY_STONE, 1, FLAG_AREA_TEMP_F2, 0
	scene_items YampiDesertCoinsPuddle, -1
	item CONTAINER_GROUND, 1, 0xf89, CONTAINER_COINS + 315
	scene_items YampiDesertTrainersWhipChest, -1
	item CONTAINER_CHEST, 1, 0x88e, ITEM_LUCKY_MEDAL
	item CONTAINER_CHEST, 2, 0xf1a, ITEM_TRAINERS_WHIP
	scene_items YampiDesertHiddenPsynergyStone, -1
	item CONTAINER_PSYNERGY_STONE, 1, FLAG_AREA_TEMP_F4, 0
	scene_items YampiDesertBlowMaceChest, -1
	item CONTAINER_CHEST, 1, 0xf1b, ITEM_HARD_NUT
	item CONTAINER_CHEST, 2, 0xf1c, ITEM_BLOW_MACE
	scene_items YampiDesertCaveWaterOfLifeChest, -1
	item CONTAINER_CHEST, 1, 0xf8a, ITEM_WATER_OF_LIFE
	scene_items YampiDesertCaveGuardian, -1
	item CONTAINER_GROUND, 1, 0xf8b, ITEM_MYTHRIL_SILVER
	item CONTAINER_STONE_TABLET, 2, SUMMON_DAEDALUS, SUMMON_DAEDALUS
	scene_items YampiDesertCaveCrystal, -1
	item CONTAINER_CHEST, 1, 0xf8c, ITEM_DARK_MATTER
	item CONTAINER_CHEST, 2, 0xf8d, ITEM_ORIHALCON
	scene_items Alhafra, -1
	item CONTAINER_CHEST, 100, 0xf1d, ITEM_PSY_CRYSTAL
	item EXAMINE_JAR, 2, 0xf1e, ITEM_SLEEP_BOMB
	item EXAMINE_JAR, 3, 0xf1f, ITEM_LUCKY_MEDAL
	item EXAMINE_WOODEN_BOX, 4, 0xf20, CONTAINER_COINS + 32
	scene_items EasternAlhafra, -1
	item EXAMINE_JAR, 1, 0xf21, ITEM_SMOKE_BOMB
	scene_items AlhafraManor, -1
	item EXAMINE_BARREL, 1, 0xf22, CONTAINER_COINS + 7
	scene_items AlhafraRooms, -1
	item EXAMINE_BARREL, 1, 0xf23, ITEM_ELIXIR
	item EXAMINE_JAR, 2, 0xf24, ITEM_APPLE
	scene_items AlhafraJail, -1
	item CONTAINER_CHEST, 1, 0xf25, CONTAINER_COINS + 123
	item CONTAINER_CHEST, 2, 0xf26, ITEM_IXION_MAIL
	item CONTAINER_CHEST, 3, 0xf27, ITEM_LUCKY_MEDAL
	item EXAMINE_JAR, 4, 0xf8e, ITEM_POWER_BREAD
	scene_items AlhafranCave, -1
	item CONTAINER_CHEST, 1, 0xf8f, CONTAINER_COINS + 777
	item CONTAINER_CHEST, 2, 0xf90, ITEM_POTION
	item CONTAINER_CHEST, 3, 0xf91, ITEM_PSY_CRYSTAL
	scene_items Mikasalla, -1
	item CONTAINER_CHEST, 1, 0xf28, CONTAINER_COINS + 82
	item EXAMINE_WOODEN_BOX, 2, 0xf29, ITEM_NUT
	item EXAMINE_BARREL, 3, 0xf2a, ITEM_ELIXIR
	scene_items MikasallaRoomsHouses, -1
	item EXAMINE_BARREL, 1, 0xf2b, ITEM_LUCKY_PEPPER
	item EXAMINE_JAR, 2, 0xf2c, ITEM_HERB
	item EXAMINE_JAR, 3, 0xf92, ITEM_LUCKY_MEDAL
	scene_items GarohPath, -1
	item CONTAINER_CHEST, 1, 0xf2d, ITEM_HYPNOS_SWORD
	scene_items Garoh, -1
	item EXAMINE_BARREL, 2, 0xf2e, ITEM_NUT
	item EXAMINE_BARREL, 3, 0xf2f, ITEM_SMOKE_BOMB
	scene_items GarohRoomsHouses, -1
	item EXAMINE_JAR, 1, 0xf30, ITEM_SLEEP_BOMB
	item EXAMINE_JAR, 2, 0xf31, ITEM_ELIXIR
	scene_items AirsRockStormBrandChest, -1
	item CONTAINER_CHEST, 1, 0xf32, ITEM_SMOKE_BOMB
	item CONTAINER_CHEST, 2, 0xf33, ITEM_COOKIE
	item CONTAINER_CHEST, 3, 0xf34, ITEM_STORM_BRAND
	item CONTAINER_MIMIC, 100, 0xe71, MIMIC_AIRS_ROCK
	scene_items AirsRockEntrance, -1
	item CONTAINER_CHEST, 12, 0xf35, ITEM_VIAL
	item CONTAINER_CHEST, 13, 0xf36, ITEM_SLEEP_BOMB
	scene_items AirsRockFujinShieldChest, -1
	item CONTAINER_CHEST, 1, 0xf37, ITEM_FUJIN_SHIELD
	scene_items AirsRockInsideVialChest, -1
	item CONTAINER_CHEST, 5, 0xf38, ITEM_VIAL
	item CONTAINER_PSYNERGY_STONE, 6, FLAG_AREA_TEMP_FE, 0
	scene_items AirsRockInsideClarityCircletChest, -1
	item CONTAINER_CHEST, 7, 0xf39, ITEM_CLARITY_CIRCLET
	scene_items AirsRockInsideHall, -1
	item CONTAINER_CHEST, 6, 0xf3a, ITEM_VIAL
	scene_items AirsRockInsideElixirChest, -1
	item CONTAINER_CHEST, 10, 0xf3b, ITEM_ELIXIR
	scene_items AirsRockInsideRevealTablet, -1
	item CONTAINER_CHEST, 3, 0xf3c, ITEM_PSY_CRYSTAL
	scene_items AirsRockInsideFaceVents, -1
	item CONTAINER_STONE_TABLET, 8, SUMMON_FLORA, SUMMON_FLORA
	scene_items AirsRockInsideCoinsChest, -1
	item CONTAINER_CHEST, 100, 0xf3d, CONTAINER_COINS + 666
	scene_items GondowanCliffs, -1
	item CONTAINER_GROUND, 1, 0xf40, ITEM_HEALING_FUNGUS
	item CONTAINER_GROUND, 2, 0xf41, ITEM_LAUGHING_FUNGUS
	item CONTAINER_CHEST, 3, 0xf43, ITEM_SLEEP_BOMB
	scene_items Naribwe, -1
	item CONTAINER_CHEST, 1, 0xf44, ITEM_THORN_CROWN
	item CONTAINER_CHEST, 2, 0xf45, ITEM_UNICORN_RING
	item EXAMINE_JAR, 3, 0xf46, ITEM_ELIXIR
	scene_items NaribweRooms, -1
	item EXAMINE_JAR, 1, 0xf47, CONTAINER_COINS + 18
	item EXAMINE_JAR, 2, 0xf48, ITEM_SLEEP_BOMB
	scene_items KibomboMountainsWaft, -1
	item CONTAINER_CHEST, 1, 0xf49, ITEM_POWER_BREAD
	item CONTAINER_CHEST, 2, 0xf4a, ITEM_TEAR_STONE
	scene_items KibomboMountainsDiskAxeChest, -1
	item CONTAINER_CHEST, 1, 0xf4b, ITEM_DISK_AXE
	scene_items KibomboMountainsCavern, -1
	item EXAMINE_WOODEN_BOX, 1, 0xf4c, ITEM_SMOKE_BOMB
	scene_items KibomboStatueGrounds, -1
	item CONTAINER_PSYNERGY_STONE, 1, FLAG_AREA_TEMP_60, 0
	scene_items KibomboStatueGroundsCeremony, -1
	item CONTAINER_PSYNERGY_STONE, 1, FLAG_AREA_TEMP_60, 0
	scene_items Kibombo, -1
	item EXAMINE_JAR, 10, 0xf4e, ITEM_LUCKY_PEPPER
	scene_items KibomboCeremony, -1
	item EXAMINE_JAR, 10, 0xf4e, ITEM_LUCKY_PEPPER
	scene_items KibomboHouses, -1
	item EXAMINE_JAR, 1, 0xf4f, ITEM_LUCKY_MEDAL
	item EXAMINE_BARREL, 2, 0xf50, ITEM_NUT
	scene_items GabombaStatueBoneArmletChest, -1
	item CONTAINER_CHEST, 1, 0xf51, ITEM_BONE_ARMLET
	item CONTAINER_MIMIC, 100, 0xe72, MIMIC_GABOMBA_STATUE
	scene_items GabombaStatueElixirChest, -1
	item CONTAINER_CHEST, 1, 0xf52, ITEM_ELIXIR
	scene_items GabombaCatacombsHiddenWall, -1
	item CONTAINER_GROUND, 1, 0xf53, ITEM_MINT
	scene_items GabombaCatacombsTomegathericon, -1
	item CONTAINER_GROUND, 1, 0xf93, ITEM_TOMEGATHERICON
	scene_items LemurianShipCabins, -1
	item CONTAINER_CHEST, 1, 0xf54, ITEM_POTION
	item EXAMINE_BARREL, 2, 0xf55, ITEM_ELIXIR
	item EXAMINE_WOODEN_BOX, 3, 0xf56, ITEM_ANTIDOTE
	scene_items LemurianShipAquaHydra, -1
	item CONTAINER_CHEST, 1, 0xf58, ITEM_MIST_POTION
	@ Two barrels with one flag: whichever is searched first gives the
	@ Oil Drop.
	item EXAMINE_BARREL, 100, 0xf57, ITEM_OIL_DROP
	item EXAMINE_BARREL, 101, 0xf57, ITEM_OIL_DROP
	scene_items LemurianShipMainDeck, -1
	item EXAMINE_WOODEN_BOX, 1, 0xf94, ITEM_COOKIE
	scene_items ShrineOfTheSeaGodPillars, -1
	item CONTAINER_CHEST, 1, 0xf59, ITEM_RUSTY_STAFF_GLOWER_STAFF
	scene_items ShrineOfTheSeaGodRightProng, -1
	item CONTAINER_GROUND, 40, 0x8c7, ITEM_RIGHT_PRONG
	scene_items WestIndraIslet, -1
	item EXAMINE_BARREL, 1, 0xf5a, ITEM_LUCKY_MEDAL
	scene_items EastTundariaIslet, -1
	item CONTAINER_PSYNERGY_STONE, 1, FLAG_AREA_TEMP_40, 0
	scene_items SoutheastAngaraIslet, -1
	item EXAMINE_WOODEN_BOX, 2, 0xf5b, ITEM_LUCKY_MEDAL
	scene_items SeaOfTimeIslet, -1
	item EXAMINE_BARREL, 1, 0xf5c, ITEM_LUCKY_MEDAL
	scene_items Yallam, -1
	item CONTAINER_GROUND, 1, FLAG_YALLAM_NUT, ITEM_NUT
	item EXAMINE_JAR, 2, 0xf61, CONTAINER_COINS + 16
	item CONTAINER_GROUND, 3, FLAG_YALLAM_ANTIDOTE, ITEM_ANTIDOTE
	item CONTAINER_CHEST, 4, 0xf95, ITEM_MASAMUNE
	scene_items YallamRoomsInn, -1
	item EXAMINE_WOODEN_BOX, 1, 0xf96, ITEM_ELIXIR
	item EXAMINE_BARREL, 2, 0xf97, ITEM_OIL_DROP
	scene_items TaopoSwampEntrance, -1
	item CONTAINER_CHEST, 100, 0xf98, ITEM_COOKIE
	scene_items TaopoSwampTearStones, -1
	item CONTAINER_GROUND, 100, 0xf63, ITEM_TEAR_STONE
	item CONTAINER_GROUND, 101, 0xf64, ITEM_TEAR_STONE
	scene_items TaopoSwampLavaRocks, -1
	item CONTAINER_CHEST, 100, 0xf65, ITEM_VIAL
	scene_items TaopoSwampStarDust, -1
	item CONTAINER_GROUND, 100, 0xf66, ITEM_STAR_DUST
	scene_items TaopoSwampFlower, -1
	item CONTAINER_GROUND, 1, 0xf99, ITEM_BRAMBLE_SEED
	scene_items ApojiiIslands, -1
	item CONTAINER_GROUND, 1, 0xf9a, ITEM_MINT
	item CONTAINER_GROUND, 2, 0xf9b, ITEM_HERB
	item EXAMINE_JAR, 3, 0xf9c, CONTAINER_COINS + 182
	scene_items ApojiiIslandsShops, -1
	item EXAMINE_BARREL, 1, 0xf9d, CONTAINER_COINS + 32
	scene_items ApojiiIslandsHaze, -1
	item CONTAINER_GROUND, 1, 0xf9e, ITEM_BRAMBLE_SEED
	scene_items AquaRockMoai, -1
	item CONTAINER_CHEST, 100, 0xf68, ITEM_NUT
	item CONTAINER_CHEST, 101, 0xf69, ITEM_ELIXIR
	scene_items AquaRockMistSabreChest, -1
	item CONTAINER_CHEST, 100, 0xf6a, ITEM_MIST_SABRE
	item CONTAINER_CHEST, 101, 0xf6b, ITEM_OIL_DROP
	scene_items AquaRockInsideWaterOfLifeChest, -1
	item CONTAINER_CHEST, 100, 0xf6c, ITEM_WATER_OF_LIFE
	scene_items AquaRockInsideSteam, -1
	item CONTAINER_MIMIC, 100, 0xe73, MIMIC_AQUA_ROCK
	scene_items AquaRockInsideAquariusStone, -1
	item CONTAINER_CHEST, 100, 0xf6d, ITEM_AQUARIUS_STONE
	item CONTAINER_CHEST, 101, 0xf6e, ITEM_LUCKY_PEPPER
	scene_items AquaRockInsideRustySwordChest, -1
	item CONTAINER_CHEST, 100, 0xf6f, ITEM_RUSTY_SWORD_ROBBERS_BLADE
	scene_items AquaRockInsideCrystalPowderChest, -1
	item CONTAINER_CHEST, 100, 0xf70, ITEM_CRYSTAL_POWDER
	item CONTAINER_PSYNERGY_STONE, 101, FLAG_AREA_TEMP_FE, 0
	scene_items AquaRockInsideVialChest, -1
	item CONTAINER_CHEST, 100, 0xf71, ITEM_VIAL
	scene_items AquaRockInsideTearStone, -1
	item CONTAINER_CHEST, 100, 0xf72, ITEM_TEAR_STONE
	scene_items AquaRockInsideParchTablet, -1
	scene_items Izumo, -1
	item CONTAINER_GROUND, 1, FLAG_IZUMO_FIRST_ANTIDOTE, ITEM_ANTIDOTE
	item CONTAINER_GROUND, 2, FLAG_IZUMO_SECOND_ANTIDOTE, ITEM_ANTIDOTE
	item CONTAINER_GROUND, 3, FLAG_IZUMO_LUCKY_MEDAL, ITEM_LUCKY_MEDAL
	item EXAMINE_JAR, 4, 0xfa2, ITEM_ELIXIR
	item EXAMINE_JAR, 5, 0xfa3, ITEM_WATER_OF_LIFE
	scene_items IzumoRooms, -1
	item EXAMINE_JAR, 1, 0xfa4, ITEM_SMOKE_BOMB
	item EXAMINE_WOODEN_BOX, 2, 0xfa5, ITEM_FESTIVAL_COAT
	scene_items IzumoRuinsSecondRoom, -1
	item CONTAINER_STONE_TABLET, 1, SUMMON_ULYSSES, SUMMON_ULYSSES
	item CONTAINER_CHEST, 3, 0xfa6, ITEM_PHANTASMAL_MAIL
	scene_items GaiaRockSusa, -1
	item CONTAINER_CHEST, 1, 0xf73, ITEM_NUT
	scene_items GaiaRockDancingIdol, -1
	item CONTAINER_GROUND, 100, 0xf74, ITEM_DANCING_IDOL
	item CONTAINER_CHEST, 101, 0xf75, ITEM_APPLE
	scene_items GaiaRockInsideMaze, -1
	item CONTAINER_MIMIC, 100, 0xe74, MIMIC_GAIA_ROCK
	item CONTAINER_CHEST, 101, 0xf76, ITEM_RUSTY_MACE
	item CONTAINER_GROUND, 102, 0xe41, ITEM_CLOUD_BRAND
	scene_items IsletCaveHalls, -1
	item CONTAINER_CHEST, 2, 0xfa8, ITEM_TURTLE_BOOTS
	item CONTAINER_CHEST, 1, 0xfa9, ITEM_RUSTY_STAFF
	scene_items Champa, -1
	item CONTAINER_CHEST, 1, 0xfaa, ITEM_VIKING_HELM
	item EXAMINE_WOODEN_BOX, 2, 0xfab, ITEM_SMOKE_BOMB
	item EXAMINE_WOODEN_BOX, 3, 0xfac, CONTAINER_COINS + 12
	item EXAMINE_JAR, 4, 0xfad, ITEM_LUCKY_MEDAL
	scene_items ChampaRooms, -1
	item EXAMINE_WOODEN_BOX, 1, 0xfae, ITEM_ELIXIR
	scene_items ChampaCliffCavesBriggs, -1
	item EXAMINE_BARREL, 1, 0xfaf, ITEM_SLEEP_BOMB
	scene_items AnkohlRuinsStoneFaces, -1
	item CONTAINER_CHEST, 1, 0xfb0, ITEM_NONE
	item CONTAINER_CHEST, 2, 0xfb1, ITEM_NONE
	item CONTAINER_CHEST, 3, 0xfb2, ITEM_NONE
	item CONTAINER_CHEST, 4, 0xfb3, ITEM_NONE
	item CONTAINER_CHEST, 5, 0xfb4, ITEM_NONE
	item CONTAINER_CHEST, 6, 0xfb5, ITEM_NONE
	item CONTAINER_CHEST, 7, 0xf77, CONTAINER_COINS + 210
	item CONTAINER_CHEST, 8, 0xf78, ITEM_NUT
	scene_items AnkohlRuinsCrystalPowderChest, -1
	item CONTAINER_CHEST, 1, 0xf79, ITEM_CRYSTAL_POWDER
	scene_items AnkohlRuinsThanatosMaceChest, -1
	item CONTAINER_CHEST, 1, 0xf7a, ITEM_THANATOS_MACE
	item CONTAINER_CHEST, 2, 0xf7b, ITEM_POWER_BREAD
	item CONTAINER_CHEST, 3, 0xf7c, ITEM_MUNI_ROBE
	scene_items AnkohlRuinsGreatFace, -1
	item CONTAINER_CHEST, 1, 0xf7d, CONTAINER_COINS + 365
	item CONTAINER_CHEST, 2, 0xf7e, ITEM_SYLPH_FEATHER
	item CONTAINER_CHEST, 3, 0xf7f, ITEM_VIAL
	scene_items AnkohlRuinsFaceHalves, -1
	item CONTAINER_CHEST, 100, 0xf3f, ITEM_POTION
	scene_items AnkohlRuinsLeftProng, -1
	item CONTAINER_GROUND, 1, 0xf80, ITEM_LEFT_PRONG
	scene_items TundariaTowerCenterProng, -1
	item CONTAINER_CHEST, 2, 0xf81, ITEM_SYLPH_FEATHER
	item CONTAINER_CHEST, 3, 0xf82, ITEM_LUCKY_MEDAL
	item CONTAINER_CHEST, 4, 0xf83, ITEM_VIAL
	item CONTAINER_CHEST, 5, 0xf84, ITEM_LIGHTNING_SWORD
	item CONTAINER_GROUND, 6, 0x945, ITEM_CENTER_PRONG
	scene_items TundariaTowerIceSlide, -1
	item CONTAINER_CHEST, 1, 0xf85, CONTAINER_COINS + 365
	item CONTAINER_CHEST, 2, 0xf86, ITEM_MINT
	scene_items TundariaTowerIceColumns, -1
	item CONTAINER_CHEST, 5, 0xf87, ITEM_HARD_NUT
	item CONTAINER_CHEST, 6, 0xf88, ITEM_CRYSTAL_POWDER
	scene_items TundariaTowerBurstBrooch, -1
	item CONTAINER_GROUND, 1, FLAG_TUNDARIA_TOWER_BURST_BROOCH, ITEM_BURST_BROOCH
	scene_items Lemuria, -1
	item CONTAINER_GROUND, 101, 0xfb6, ITEM_WATER_OF_LIFE
	item CONTAINER_GROUND, 102, 0xfb7, ITEM_NUT
	item CONTAINER_GROUND, 103, 0xfb8, ITEM_BRAMBLE_SEED
	item CONTAINER_GROUND, 104, 0xfb9, ITEM_LUCKY_MEDAL
	item CONTAINER_GROUND, 105, 0xfba, ITEM_RUSTY_SWORD
	item CONTAINER_GROUND, 106, 0xfbb, ITEM_HARD_NUT
	item CONTAINER_GROUND, 107, 0xfbc, ITEM_BONE
	item CONTAINER_GROUND, 108, 0xfbd, ITEM_STAR_DUST
	item CONTAINER_GROUND, 109, 0xfbe, ITEM_VIAL
	scene_items LemuriaPalace, -1
	item CONTAINER_CHEST, 100, 0xf67, ITEM_GRINDSTONE
	item EXAMINE_BARREL, 101, 0xfbf, ITEM_LUCKY_MEDAL
	scene_items GondowanSettlementCave, -1
	item CONTAINER_CHEST, 1, 0xfc0, ITEM_STAR_DUST
	scene_items GondowanSettlement, -1
	item EXAMINE_GRAVE, 1, 0xfc1, ITEM_LUCKY_MEDAL
	scene_items HesperiaSettlementCave, -1
	item CONTAINER_CHEST, 2, 0xfc2, CONTAINER_COINS + 166
	scene_items KaltIsland, -1
	item CONTAINER_GROUND, 0, 0xf5d, ITEM_APPLE
	scene_items SouthwestAttekaIslet, -1
	item CONTAINER_CHEST, 1, 0xfc3, ITEM_DRAGON_SKIN
	scene_items AttekaInlet, -1
	item CONTAINER_CHEST, 1, 0xfc4, ITEM_VIAL
	item CONTAINER_CHEST, 2, 0xfa7, ITEM_ORIHALCON
	scene_items Contigo, -1
	item EXAMINE_BARREL, 1, 0xfc5, ITEM_POWER_BREAD
	item CONTAINER_GROUND, 2, 0xfc6, ITEM_CORN
	item CONTAINER_GROUND, 3, 0xfc7, ITEM_BRAMBLE_SEED
	scene_items ShamanVillage, -1
	item CONTAINER_CHEST, 1, 0xfc8, ITEM_SPIRIT_GLOVES_UNUSED
	item EXAMINE_JAR, 2, 0xfc9, ITEM_LUCKY_MEDAL
	scene_items ShamanVillageHouses, -1
	item EXAMINE_BARREL, 1, 0xfca, ITEM_WEASELS_CLAW
	item EXAMINE_JAR, 2, 0xfcb, ITEM_ELIXIR
	scene_items ShamanVillageTents, -1
	item EXAMINE_JAR, 3, 0xfcc, ITEM_LUCKY_PEPPER
	scene_items ShamanVillageTrialGrounds, -1
	item CONTAINER_CHEST, 100, 0xf5f, ITEM_HARD_NUT
	scene_items TreasureIsleEntrance, -1
	item CONTAINER_CHEST, 1, 0xfcd, CONTAINER_COINS + 161
	item CONTAINER_CHEST, 2, 0xfce, ITEM_LUCKY_MEDAL
	item CONTAINER_CHEST, 3, 0xfcf, ITEM_NONE
	item CONTAINER_CHEST, 4, 0xfd0, ITEM_NONE
	item CONTAINER_CHEST, 5, 0xfd1, ITEM_NONE
	item CONTAINER_CHEST, 6, 0xfd2, ITEM_NONE
	scene_items TreasureIsleChests, -1
	item CONTAINER_CHEST, 1, 0xfd3, ITEM_NONE
	item CONTAINER_CHEST, 2, 0xfd4, ITEM_NONE
	item CONTAINER_CHEST, 3, 0xfd5, ITEM_NONE
	item CONTAINER_CHEST, 4, 0xfd6, ITEM_NONE
	item CONTAINER_CHEST, 5, 0xfd7, ITEM_NONE
	item CONTAINER_CHEST, 6, 0xfd8, ITEM_NONE
	item CONTAINER_CHEST, 7, 0xfd9, CONTAINER_COINS + 911
	item CONTAINER_CHEST, 8, 0xfda, ITEM_PSY_CRYSTAL
	item CONTAINER_CHEST, 9, 0xfdb, ITEM_COOKIE
	item CONTAINER_CHEST, 10, 0xfdc, ITEM_SYLPH_FEATHER
	item CONTAINER_CHEST, 11, 0xfdd, ITEM_RUSTY_AXE_VIKING_AXE
	item CONTAINER_CHEST, 12, 0xfde, ITEM_STAR_DUST
	scene_items TreasureIsleJestersArmletChest, -1
	item CONTAINER_CHEST, 1, 0xfdf, ITEM_JESTERS_ARMLET
	item CONTAINER_MIMIC, 100, 0xe75, MIMIC_TREASURE_ISLE
	scene_items TreasureIsleGale, -1
	item CONTAINER_CHEST, 1, 0xfe0, ITEM_FIRE_BRAND
	item CONTAINER_CHEST, 3, 0xfe1, ITEM_IRIS_ROBE
	scene_items TreasureIsleGuardian, -1
	item CONTAINER_STONE_TABLET, 1, SUMMON_AZUL, SUMMON_AZUL
	scene_items JupiterLighthouseEntrance, -1
	item CONTAINER_GROUND, 1, FLAG_JUPITER_LIGHTHOUSE_MINT, ITEM_MINT
	scene_items JupiterLighthouseBasementPsynergyStone, -1
	item CONTAINER_PSYNERGY_STONE, 1, FLAG_AREA_TEMP_00, 0
	scene_items JupiterLighthouseRoomErinyesTunicChest, -1
	item CONTAINER_CHEST, 100, 0xfe3, ITEM_ERINYES_TUNIC
	scene_items JupiterLighthouseRoomLockedDoors, -1
	item CONTAINER_CHEST, 1, 0xfe4, ITEM_POTION
	item CONTAINER_CHEST, 2, 0xfe5, ITEM_PSY_CRYSTAL
	scene_items JupiterLighthouseRoomRedKey, -1
	item CONTAINER_CHEST, 1, 0xfe6, ITEM_MEDITATION_ROD
	item CONTAINER_GROUND, 2, 0xfe7, ITEM_RED_KEY
	scene_items JupiterLighthouseRoomBlueKey, -1
	item CONTAINER_MIMIC, 1, 0xe76, MIMIC_JUPITER_LIGHTHOUSE
	item CONTAINER_GROUND, 2, 0xfe8, ITEM_BLUE_KEY
	item CONTAINER_CHEST, 3, 0xfe9, ITEM_MIST_POTION
	item CONTAINER_CHEST, 4, 0xfea, CONTAINER_COINS + 306
	scene_items JupiterLighthouseWhorl, -1
	item CONTAINER_CHEST, 1, 0xfeb, ITEM_WATER_OF_LIFE
	scene_items JupiterLighthouseRoomPhaetonsBladeChest, -1
	item CONTAINER_CHEST, 1, 0xfec, ITEM_PHAETONS_BLADE
	scene_items MagmaRockSlide, -1
	item CONTAINER_CHEST, 100, 0xfed, ITEM_OIL_DROP
	scene_items MagmaRockSalamanderTailChest, -1
	item CONTAINER_CHEST, 100, 0xfee, CONTAINER_COINS + 383
	item CONTAINER_CHEST, 101, 0xfef, ITEM_SALAMANDER_TAIL
	scene_items MagmaRockInsideLuckyMedalChest, -1
	item CONTAINER_CHEST, 100, 0xff0, ITEM_LUCKY_MEDAL
	scene_items MagmaRockInsideFury, -1
	item CONTAINER_CHEST, 100, 0xff1, ITEM_MIST_POTION
	scene_items MagmaRockInsideSalamanderTailChest, -1
	item CONTAINER_CHEST, 100, 0xff2, ITEM_SALAMANDER_TAIL
	item CONTAINER_CHEST, 101, 0xff3, ITEM_OIL_DROP
	scene_items MagmaRockInsideGolemCoreChest, -1
	item CONTAINER_CHEST, 100, 0xff4, ITEM_GOLEM_CORE
	scene_items MagmaRockInsideEmberStone, -1
	item CONTAINER_MIMIC, 100, 0xe77, MIMIC_MAGMA_ROCK
	scene_items Loho, -1
	item CONTAINER_GROUND, 1, 0xff5, ITEM_MYTHRIL_SILVER
	item CONTAINER_GROUND, 2, 0xff6, ITEM_GOLEM_CORE
	item CONTAINER_GROUND, 3, 0xff7, ITEM_GOLEM_CORE
	item EXAMINE_BARREL, 4, 0xff8, ITEM_CRYSTAL_POWDER
	scene_items Prox, -1
	item CONTAINER_GROUND, 100, 0xff9, ITEM_DARK_MATTER
	item EXAMINE_JAR, 1, 0xffa, ITEM_COOKIE
	scene_items ProxCaveHomes, -1
	item EXAMINE_JAR, 3, 0xffb, ITEM_POTION
	item EXAMINE_WOODEN_BOX, 2, 0xffc, ITEM_SACRED_FEATHER
	scene_items MarsLighthouseOutside, -1
	item CONTAINER_CHEST, 1, 0xffd, ITEM_APPLE
	scene_items MarsLighthouseEntranceTeleportLapis, -1
	item CONTAINER_CHEST, 100, 0xffe, ITEM_TELEPORT_LAPIS
	item CONTAINER_MIMIC, 101, 0xe78, MIMIC_MARS_LIGHTHOUSE
	scene_items MarsLighthouseMarsLedge, -1
	item CONTAINER_CHEST, 100, 0xfff, ITEM_ALASTORS_HOOD
	scene_items MarsLighthouseEntranceBalm, -1
	item CONTAINER_CHEST, 100, 0xe00, ITEM_ORIHALCON
	scene_items MarsLighthouseEntranceValkyrieMailChest, -1
	item CONTAINER_CHEST, 100, 0xe01, ITEM_VALKYRIE_MAIL
	scene_items MarsLighthouseEntranceSolBladeChest, -1
	item CONTAINER_CHEST, 100, 0xe02, ITEM_SOL_BLADE
	scene_items MarsLighthouseJupiterTower, -1
	item CONTAINER_CHEST, 101, 0xe03, ITEM_PSY_CRYSTAL
	scene_items AnemosInnerSanctumDragonSkinChest, -1
	item CONTAINER_CHEST, 100, 0xe04, ITEM_DRAGON_SKIN
	scene_items AnemosInnerSanctumOrihalconChest, -1
	item CONTAINER_CHEST, 1, 0xe05, ITEM_DARK_MATTER
	item CONTAINER_CHEST, 2, 0xe06, ITEM_ORIHALCON
	scene_items TrialRoadStages, -1
	item CONTAINER_CHEST, 100, FLAG_TRIAL_ROAD_STAGE_ITEM_FIRST, ITEM_VIAL
	item CONTAINER_CHEST, 101, FLAG_TRIAL_ROAD_STAGE_ITEM_FIRST + 1, ITEM_VIAL
	item CONTAINER_CHEST, 102, FLAG_TRIAL_ROAD_STAGE_ITEM_FIRST + 2, ITEM_VIAL
	item CONTAINER_CHEST, 103, FLAG_TRIAL_ROAD_STAGE_ITEM_FIRST + 3, ITEM_VIAL
	item CONTAINER_CHEST, 104, FLAG_TRIAL_ROAD_STAGE_ITEM_FIRST + 4, ITEM_POTION
	item CONTAINER_CHEST, 105, FLAG_TRIAL_ROAD_STAGE_ITEM_FIRST + 5, ITEM_VIAL
	item CONTAINER_CHEST, 106, FLAG_TRIAL_ROAD_STAGE_ITEM_FIRST + 6, ITEM_NUT
	item CONTAINER_CHEST, 107, FLAG_TRIAL_ROAD_STAGE_ITEM_FIRST + 7, ITEM_VIAL
	item CONTAINER_CHEST, 108, FLAG_TRIAL_ROAD_STAGE_ITEM_FIRST + 8, ITEM_POTION
	item CONTAINER_CHEST, 109, FLAG_TRIAL_ROAD_STAGE_ITEM_LAST, ITEM_POTION
	scene_items TrialRoadGasp, -1
	item CONTAINER_CHEST, 100, 0xe5a, ITEM_ELIXIR
	scene_items CavernsIndra, -1
	item CONTAINER_STONE_TABLET, 1, SUMMON_ZAGAN, SUMMON_ZAGAN
	scene_items CavernsOsenia, -1
	item CONTAINER_STONE_TABLET, 1, SUMMON_MEGAERA, SUMMON_MEGAERA
	scene_items CavernsAngara, -1
	item CONTAINER_STONE_TABLET, 1, SUMMON_HAURES, SUMMON_HAURES
	scene_items CavernsAtteka, -1
	item CONTAINER_STONE_TABLET, 2, SUMMON_COATLICUE, SUMMON_COATLICUE
	scene_items IsletCaveSentinel, -1
	item CONTAINER_STONE_TABLET, 1, SUMMON_CATASTROPHE, SUMMON_CATASTROPHE
	scene_items AnemosInnerSanctumGuardian, -1
	item CONTAINER_STONE_TABLET, 100, SUMMON_CHARON, SUMMON_CHARON
	scene_items AnemosInnerSanctumEntrance, -1
	item CONTAINER_STONE_TABLET, 100, SUMMON_IRIS, SUMMON_IRIS
	scene_items WorldMap, -1
	item CONTAINER_POND, 100, 0xe5b, ITEM_RUSTY_AXE
	item CONTAINER_POND, 101, 0xe5c, ITEM_RUSTY_MACE_HAGBONE_MACE
	item CONTAINER_POND, 102, 0xe5d, ITEM_RUSTY_SWORD_PIRATES_SABRE
	item CONTAINER_POND, 103, 0xe5e, ITEM_RUSTY_STAFF_GOBLINS_ROD
	item CONTAINER_POND, 104, 0xe5f, ITEM_RUSTY_SWORD_SOUL_BRAND
	.2byte 0, 0 @ the end
