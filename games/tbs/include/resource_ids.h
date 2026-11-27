#ifndef ALCHEMY_RESOURCE_IDS_H
#define ALCHEMY_RESOURCE_IDS_H

// Resource numbers, named where their contents are known.
//
// Each ResourceId is an absolute symbol whose value is the resource's row in
// syResourceDirectoryTable (system/resource_directory.s); C names the number by its macro,
// RESOURCE_WINDOW_TILES for ResourceIdWindowTiles.

// Twelve ASCII digits, yymmddhhmmss, and a newline, different in each edition. syCopyBuildStamp
// copies it into BuildStampWork, and gmBuildStampGetTime turns its time into the number saves
// compare.
extern char ResourceIdBuildStamp;
#define RESOURCE_BUILD_STAMP ((Sint32)&ResourceIdBuildStamp)
// The row of StubsTitle. The Title hands it to syDummyNoOperationHook, which ignores it.
extern char ResourceIdTitleStubs;
#define RESOURCE_TITLE_STUBS ((Sint32)&ResourceIdTitleStubs)
// The row of btStubsBattleEffect. Lucky Dice and Lucky Wheels hand it to syDummyNoOperationHook as
// they start.
extern char ResourceIdBattleEffectStubs;
#define RESOURCE_BATTLE_EFFECT_STUBS ((Sint32)&ResourceIdBattleEffectStubs)
// The row of StubsResource, which the sprite viewer passes to syDummyReturnZero when Select is
// pressed with B held.
extern char ResourceIdResourceStubs;
#define RESOURCE_RESOURCE_STUBS ((Sint32)&ResourceIdResourceStubs)
// A second row that also holds StubsResource; the sprite viewer passes it when Select is pressed
// without B.
extern char ResourceIdResourceStubsAlternate;
#define RESOURCE_RESOURCE_FAR_CALLS_ALTERNATE ((Sint32)&ResourceIdResourceStubsAlternate)
// The window font and border tiles, 256 tiles of 4 bits a pixel. syWindowLoadGraphics copies them
// into BG character block 0, and the window text functions draw their glyphs from here.
extern char ResourceIdWindowTiles;
#define RESOURCE_WINDOW_TILES ((Sint32)&ResourceIdWindowTiles)
// The narrow glyphs the Japanese edition draws in the battle message window; the other editions
// look it up there and then draw from WindowTiles.
extern char ResourceIdSmallFont;
#define RESOURCE_SMALL_FONT ((Sint32)&ResourceIdSmallFont)
// Shown first by mnTitleShowPublisherLogo.
extern char ResourceIdPublisherLogo;
#define RESOURCE_PUBLISHER_LOGO ((Sint32)&ResourceIdPublisherLogo)
// The Camelot logo's four ticks of tiles, which mnTitleShowCamelotLogo cycles.
extern char ResourceIdCamelotLogo;
#define RESOURCE_CAMELOT_LOGO ((Sint32)&ResourceIdCamelotLogo)
// The title picture on BG2, loaded by scTitleLoadBackground and, for the main menu, by
// scFileScreenLoadBackground.
extern char ResourceIdGoldenSunLogo;
#define RESOURCE_GOLDEN_SUN_LOGO ((Sint32)&ResourceIdGoldenSunLogo)
// The still world map fdMapScreenShow shows for a FIELD_RAISED_CUE_MAP_SCREEN script; it loads this
// one whatever picture it's passed.
extern char ResourceIdWorldMapPicture;
#define RESOURCE_WORLD_MAP_PICTURE ((Sint32)&ResourceIdWorldMapPicture)
// The lettering sprites scTitleLoadSprites loads: 40 tiles of 16 colours in English and Japanese, a
// whole 224-colour screen in the other editions, of which it keeps the bottom 32 rows.
extern char ResourceIdToBeContinuedLettering;
#define RESOURCE_TO_BE_CONTINUED_LETTERING ((Sint32)&ResourceIdToBeContinuedLettering)

// Battle backgrounds. fdBattleBackgroundSelectForMap picks one for the current scene from
// SceneBattleBackgroundRules (field/battle_background_rules.c), and on the world map
// fdBattleBackgroundSelectForEnvironment picks one by battle environment; either leaves it in
// MainRoutineStateWork.battleBg for the battle. The credits (CreditsBackgrounds) shows every one
// but DesertBackdrop and AerialViewBackdrop, and some summons draw one as their sky.

// Suhalla Gate's checkpoint and Dew rooms. The debug battle menu fights on it too.
extern char ResourceIdSuhallaGateBackdrop;
#define RESOURCE_SUHALLA_GATE_BACKDROP ((Sint32)&ResourceIdSuhallaGateBackdrop)
extern char ResourceIdPirateShipBackdrop;        // SceneIdPirateShip.
#define RESOURCE_PIRATE_SHIP_BACKDROP ((Sint32)&ResourceIdPirateShipBackdrop)
extern char ResourceIdCrossboneIsleCaveBackdrop; // Crossbone Isle's floors.
#define RESOURCE_CROSSBONE_ISLE_CAVE_BACKDROP ((Sint32)&ResourceIdCrossboneIsleCaveBackdrop)
// Colosso's three matches (Azart, Satrage and Navampa) and the Battle Arena.
extern char ResourceIdColossoBackdrop;
#define RESOURCE_COLOSSO_BACKDROP ((Sint32)&ResourceIdColossoBackdrop)
// SceneIdVenusLighthouseAerie.
extern char ResourceIdVenusLighthouseAerieBackdrop;
#define RESOURCE_VENUS_LIGHTHOUSE_AERIE_BACKDROP ((Sint32)&ResourceIdVenusLighthouseAerieBackdrop)
// Venus Lighthouse's floors, from the ground floor to the fourth.
extern char ResourceIdVenusLighthouseBackdrop;
#define RESOURCE_VENUS_LIGHTHOUSE_BACKDROP ((Sint32)&ResourceIdVenusLighthouseBackdrop)
// Suhalla Desert, north and south.
extern char ResourceIdSuhallaDesertBackdrop;
#define RESOURCE_SUHALLA_DESERT_BACKDROP ((Sint32)&ResourceIdSuhallaDesertBackdrop)
// Altmiller Cave and Lunpa Fortress.
extern char ResourceIdAltmillerCaveBackdrop;
#define RESOURCE_ALTMILLER_CAVE_BACKDROP ((Sint32)&ResourceIdAltmillerCaveBackdrop)
// Tunnel Ruins' two statue rooms.
extern char ResourceIdTunnelRuinsBackdrop;
#define RESOURCE_TUNNEL_RUINS_BACKDROP ((Sint32)&ResourceIdTunnelRuinsBackdrop)
// Babi Lighthouse, and SceneIdCrossboneIsleBane on Crossbone Isle.
extern char ResourceIdBabiLighthouseBackdrop;
#define RESOURCE_BABI_LIGHTHOUSE_BACKDROP ((Sint32)&ResourceIdBabiLighthouseBackdrop)
// Lamakan Desert's three scenes.
extern char ResourceIdLamakanDesertBackdrop;
#define RESOURCE_LAMAKAN_DESERT_BACKDROP ((Sint32)&ResourceIdLamakanDesertBackdrop)
extern char ResourceIdMogallForestBackdrop; // Mogall Forest's three scenes.
#define RESOURCE_MOGALL_FOREST_BACKDROP ((Sint32)&ResourceIdMogallForestBackdrop)
// SceneIdMercuryLighthouseAerie.
extern char ResourceIdMercuryLighthouseAerieBackdrop;
#define RESOURCE_MERCURY_LIGHTHOUSE_AERIE_BACKDROP                                                 \
	((Sint32)&ResourceIdMercuryLighthouseAerieBackdrop)
// SceneIdShipRooms.
extern char ResourceIdTolbiShipRoomsBackdrop;
#define RESOURCE_TOLBI_SHIP_ROOMS_BACKDROP ((Sint32)&ResourceIdTolbiShipRoomsBackdrop)
// Fuchin Falls Cave, and SceneIdCrossboneIsleFloatingLogs on Crossbone Isle.
extern char ResourceIdFuchinFallsCaveBackdrop;
#define RESOURCE_FUCHIN_FALLS_CAVE_BACKDROP ((Sint32)&ResourceIdFuchinFallsCaveBackdrop)
extern char ResourceIdKolimaForestBackdrop; // Kolima Forest's five scenes.
#define RESOURCE_KOLIMA_FOREST_BACKDROP ((Sint32)&ResourceIdKolimaForestBackdrop)
// SceneIdTreeRooms.
extern char ResourceIdTretTreeRoomsBackdrop;
#define RESOURCE_TRET_TREE_ROOMS_BACKDROP ((Sint32)&ResourceIdTretTreeRoomsBackdrop)
// Goma Cave, Kolima Tunnel, Bilibin Cave and Gateway Cave, and any scene the table doesn't list,
// since it comes last.
extern char ResourceIdGomaCaveBackdrop;
#define RESOURCE_GOMA_CAVE_BACKDROP ((Sint32)&ResourceIdGomaCaveBackdrop)
// The vault rooms and Lalivero's scenes, Faran's house included.
extern char ResourceIdRoomsBackdrop;
#define RESOURCE_ROOMS_BACKDROP ((Sint32)&ResourceIdRoomsBackdrop)
// Altin Peak's scenes but the Hydros statue.
extern char ResourceIdAltinPeakBackdrop;
#define RESOURCE_ALTIN_PEAK_BACKDROP ((Sint32)&ResourceIdAltinPeakBackdrop)
// SceneIdTolbiShipDeck and SceneIdTolbiShipMast.
extern char ResourceIdTolbiShipDeckBackdrop;
#define RESOURCE_TOLBI_SHIP_DECK_BACKDROP ((Sint32)&ResourceIdTolbiShipDeckBackdrop)
// Mercury Lighthouse's floors below the aerie and its exterior.
extern char ResourceIdMercuryLighthouseBackdrop;
#define RESOURCE_MERCURY_LIGHTHOUSE_BACKDROP ((Sint32)&ResourceIdMercuryLighthouseBackdrop)
// Vale's scenes, the stormy night included; the credits open on it.
extern char ResourceIdValeStormBackdrop;
#define RESOURCE_VALE_STORM_BACKDROP ((Sint32)&ResourceIdValeStormBackdrop)
extern char ResourceIdTretTreeBranchBackdrop; // SceneIdTretTreeBranch.
#define RESOURCE_TRET_TREE_BRANCH_BACKDROP ((Sint32)&ResourceIdTretTreeBranchBackdrop)
// Set by the first slot of ENCOUNTER_ZONE_SCRIPTED (fdRequestBattleFromEncounterTable), and by the
// storm opening of btTransitionRun, where the enemies alone take part.
extern char ResourceIdVenusLighthouseAerieStormBackdrop;
#define RESOURCE_VENUS_LIGHTHOUSE_AERIE_STORM_BACKDROP                                             \
	((Sint32)&ResourceIdVenusLighthouseAerieStormBackdrop)
// World map beaches and shallows; Judgment draws it too.
extern char ResourceIdBeachBackdrop;
#define RESOURCE_BEACH_BACKDROP ((Sint32)&ResourceIdBeachBackdrop)
// World map snowfields and snowy forests.
extern char ResourceIdSnowfieldBackdrop;
#define RESOURCE_SNOWFIELD_BACKDROP ((Sint32)&ResourceIdSnowfieldBackdrop)
extern char ResourceIdGrasslandBackdrop; // World map grassland.
#define RESOURCE_GRASSLAND_BACKDROP ((Sint32)&ResourceIdGrasslandBackdrop)
// Sol Sanctum's rooms, SceneIdAltinPeakHydrosStatue and SceneIdCrossboneIsleSteppingStones, and any
// battle environment without a battle background of its own.
extern char ResourceIdSolSanctumBackdrop;
#define RESOURCE_SOL_SANCTUM_BACKDROP ((Sint32)&ResourceIdSolSanctumBackdrop)
// World map forests; the Judgment and Kirin summons draw it too.
extern char ResourceIdForestBackdrop;
#define RESOURCE_FOREST_BACKDROP ((Sint32)&ResourceIdForestBackdrop)
// No battle is fought on it: the Atalanta, Boreas, Judgment and Procne summons draw it as their
// sky, and the credits close on it.
extern char ResourceIdVioletSkyBackdrop;
#define RESOURCE_VIOLET_SKY_BACKDROP ((Sint32)&ResourceIdVioletSkyBackdrop)
// World map desert, and the sky of the Ramses summon.
extern char ResourceIdDesertBackdrop;
#define RESOURCE_DESERT_BACKDROP ((Sint32)&ResourceIdDesertBackdrop)
// The sky of the Neptune and Thor summons.
extern char ResourceIdDuskCloudsBackdrop;
#define RESOURCE_DUSK_CLOUDS_BACKDROP ((Sint32)&ResourceIdDuskCloudsBackdrop)
extern char ResourceIdAerialViewBackdrop; // Drawn only by Judgment.
#define RESOURCE_AERIAL_VIEW_BACKDROP ((Sint32)&ResourceIdAerialViewBackdrop)

// The Lucky Wheels machine around the wheels (fdLuckyWheelsRun): 16 colours for BG palette 10, then
// the picture.
extern char ResourceIdLuckyWheelsMachinePicture;
#define RESOURCE_LUCKY_WHEELS_MACHINE_PICTURE ((Sint32)&ResourceIdLuckyWheelsMachinePicture)
// The wheels' symbol sprites: 480 bytes of sprite colours, then the compressed tiles.
extern char ResourceIdLuckyWheelsIcons;
#define RESOURCE_LUCKY_WHEELS_ICONS ((Sint32)&ResourceIdLuckyWheelsIcons)
// The jackpot sprites, with their colours for sprite palette 15.
extern char ResourceIdLuckyWheelsJackpot;
#define RESOURCE_LUCKY_WHEELS_JACKPOT ((Sint32)&ResourceIdLuckyWheelsJackpot)
// The Lucky Dice board, with its colours, on the background (fdLuckyDiceRun).
extern char ResourceIdLuckyDiceBoard;
#define RESOURCE_LUCKY_DICE_BOARD ((Sint32)&ResourceIdLuckyDiceBoard)
// The Lucky Dice sprites and their colours.
extern char ResourceIdLuckyDiceSprites;
#define RESOURCE_LUCKY_DICE_SPRITES ((Sint32)&ResourceIdLuckyDiceSprites)

// The flashing strikes' pictures (battle_effect/battle_effect_thrash.c). Each holds two pictures;
// the first of a pair fills state->imageStorage, the second the start of RamMapGridMetatileBuffer.

extern char ResourceIdVorpalSlashImageA; // Vorpal Slash, first of its pair.
#define RESOURCE_VORPAL_SLASH_IMAGE_A ((Sint32)&ResourceIdVorpalSlashImageA)
// Vorpal Slash, second of its pair.
extern char ResourceIdVorpalSlashImageB;
#define RESOURCE_VORPAL_SLASH_IMAGE_B ((Sint32)&ResourceIdVorpalSlashImageB)
extern char ResourceIdQuickStrikeImageA; // Quick Strike, first of its pair.
#define RESOURCE_QUICK_STRIKE_IMAGE_A ((Sint32)&ResourceIdQuickStrikeImageA)
// Quick Strike, second of its pair.
extern char ResourceIdQuickStrikeImageB;
#define RESOURCE_QUICK_STRIKE_IMAGE_B ((Sint32)&ResourceIdQuickStrikeImageB)
// First of the pair for FLASHING_STRIKE_UNUSED, which nothing passes.
extern char ResourceIdUnusedFlashingStrikeImageA;
#define RESOURCE_UNUSED_FLASHING_STRIKE_IMAGE_A ((Sint32)&ResourceIdUnusedFlashingStrikeImageA)
// Second of the pair for FLASHING_STRIKE_UNUSED.
extern char ResourceIdUnusedFlashingStrikeImageB;
#define RESOURCE_UNUSED_FLASHING_STRIKE_IMAGE_B ((Sint32)&ResourceIdUnusedFlashingStrikeImageB)

// Battle effect pictures. A sprite sheet holds 64 colours (128 bytes) and then its compressed
// pictures, which the animation parts load with btResourceLoadAndDecompress; a palette holds
// colours alone, which a part copies over a shared sprite sheet's to recolour it, and some parts
// borrow a sprite sheet's colours the same way. Each note names the animations that read it.

extern char ResourceIdLimeColumnSheet; // Undead Sword.
#define RESOURCE_LIME_COLUMN_SHEET ((Sint32)&ResourceIdLimeColumnSheet)
extern char ResourceIdDemonFaceSheet;  // The blessings' burst showers.
#define RESOURCE_DEMON_FACE_SHEET ((Sint32)&ResourceIdDemonFaceSheet)
// The blessings' burst showers, beside DemonFaceSheet.
extern char ResourceIdVioletLightningSheetA;
#define RESOURCE_VIOLET_LIGHTNING_SHEET_A ((Sint32)&ResourceIdVioletLightningSheetA)
// The yellow sparks of Wing Flutter, Wing Beat and Wing Stroke.
extern char ResourceIdYellowSparkSheet;
#define RESOURCE_YELLOW_SPARK_SHEET ((Sint32)&ResourceIdYellowSparkSheet)
// Crazy Voice's bouncing pellets, drawn in SlashCometSheet's colours.
extern char ResourceIdProjectileVolleyImage;
#define RESOURCE_PROJECTILE_VOLLEY_IMAGE ((Sint32)&ResourceIdProjectileVolleyImage)
// The Shuriken volley's spinning shurikens.
extern char ResourceIdShurikenSheet;
#define RESOURCE_SHURIKEN_SHEET ((Sint32)&ResourceIdShurikenSheet)
// The target bursts of battle_effect_orbiting.c.
extern char ResourceIdTargetBurstImage;
#define RESOURCE_TARGET_BURST_IMAGE ((Sint32)&ResourceIdTargetBurstImage)
// Hail, the Neptune summon and the blessings' burst showers; Nereid takes its colours.
extern char ResourceIdWaterSpraySheet;
#define RESOURCE_WATER_SPRAY_SHEET ((Sint32)&ResourceIdWaterSpraySheet)
// The casting impact, the Djinn attacks and the Neptune summon.
extern char ResourceIdBlueFlameSheet;
#define RESOURCE_BLUE_FLAME_SHEET ((Sint32)&ResourceIdBlueFlameSheet)
// The particles nearly every animation loads, from Annihilation to Volcano and most summons.
extern char ResourceIdYellowGlowSheet;
#define RESOURCE_YELLOW_GLOW_SHEET ((Sint32)&ResourceIdYellowGlowSheet)
// A second shared particle sprite sheet: the beams, Bind, Death Plunge, Ice and Douse, the Neptune
// summon, the stat changes, and Lucky Wheels.
extern char ResourceIdBlueGlowSheet;
#define RESOURCE_BLUE_GLOW_SHEET ((Sint32)&ResourceIdBlueGlowSheet)
// Curse's flames (btEffectRunCurse).
extern char ResourceIdFlameColumnSheet;
#define RESOURCE_FLAME_COLUMN_SHEET ((Sint32)&ResourceIdFlameColumnSheet)
// Shared by the blessings' showers, the casting impact, Death Plunge, Dragon Cloud, Planet Diver,
// Plasma, Ragnarok, Tiamat and Titan Blade.
extern char ResourceIdFlashBurstSheet;
#define RESOURCE_FLASH_BURST_SHEET ((Sint32)&ResourceIdFlashBurstSheet)
extern char ResourceIdThornSheet;
#define RESOURCE_THORN_SHEET ((Sint32)&ResourceIdThornSheet)
// Slash's comet, which the impact's comets (PROJECTILE_VOLLEY_IMPACT_COMETS) fire too, and the
// colours of Crazy Voice's pellets.
extern char ResourceIdSlashCometSheet;
#define RESOURCE_SLASH_COMET_SHEET ((Sint32)&ResourceIdSlashCometSheet)
// Wind Slash's comet.
extern char ResourceIdWindSlashCometSheet;
#define RESOURCE_WIND_SLASH_COMET_SHEET ((Sint32)&ResourceIdWindSlashCometSheet)
// Sonic Slash's comet, and Cutting Edge's in ice colours.
extern char ResourceIdSonicSlashCometSheet;
#define RESOURCE_SONIC_SLASH_COMET_SHEET ((Sint32)&ResourceIdSonicSlashCometSheet)
// GROWING_COLUMNS_PILLARS, in btEffectRunGrowingColumns.
extern char ResourceIdGreenPillarSheet;
#define RESOURCE_GREEN_PILLAR_SHEET ((Sint32)&ResourceIdGreenPillarSheet)
// btEffectRunGrowingColumns' other modes.
extern char ResourceIdVineSheet;
#define RESOURCE_VINE_SHEET ((Sint32)&ResourceIdVineSheet)
// The swirling stars of battle_effect_orbiting.c.
extern char ResourceIdStarDotSheet;
#define RESOURCE_STAR_DOT_SHEET ((Sint32)&ResourceIdStarDotSheet)
// Borrowed mostly for its colours: by the showers, the casting impact, Rumble's lights, the healing
// motes, the lobbed shells, the walls, the stat changes, the status motes and the monster strikes.
extern char ResourceIdMarsDjinnSheet;
#define RESOURCE_MARS_DJINN_SHEET ((Sint32)&ResourceIdMarsDjinnSheet)
// The small Mercury Djinni of the casting impact, the Djinn attacks, Djinn healing, Hail and the
// ice volleys' prelude.
extern char ResourceIdMercuryDjinnSmallSheet;
#define RESOURCE_MERCURY_DJINN_SMALL_SHEET ((Sint32)&ResourceIdMercuryDjinnSmallSheet)
// The casting impact, the lobbed shells, the orbiting bursts, the status motes and the monster
// strikes.
extern char ResourceIdSmokeSheet;
#define RESOURCE_SMOKE_SHEET ((Sint32)&ResourceIdSmokeSheet)
// Colours for one style of the blessings' showers and of the monster strikes.
extern char ResourceIdPinkPalette;
#define RESOURCE_PINK_PALETTE ((Sint32)&ResourceIdPinkPalette)
extern char ResourceIdGreenVineSheet;
#define RESOURCE_GREEN_VINE_SHEET ((Sint32)&ResourceIdGreenVineSheet)
extern char ResourceIdReaperSheet; // Condemn and the casting impact.
#define RESOURCE_REAPER_SHEET ((Sint32)&ResourceIdReaperSheet)
// Condemn and the casting impact, beside ReaperSheet.
extern char ResourceIdRedCrescentSheetA;
#define RESOURCE_RED_CRESCENT_SHEET_A ((Sint32)&ResourceIdRedCrescentSheetA)
// The orbiting spirals of battle_effect_planet_diver.c.
extern char ResourceIdSpiralSheet;
#define RESOURCE_SPIRAL_SHEET ((Sint32)&ResourceIdSpiralSheet)
// The fire family: Fire, Flare, Atalanta, Dragon Cloud, Tiamat, the casting impact, the Djinn
// attacks, Death Plunge, the streaks and the strikes; Lucky Wheels steps its colours toward it.
extern char ResourceIdEmberStreakSheet;
#define RESOURCE_EMBER_STREAK_SHEET ((Sint32)&ResourceIdEmberStreakSheet)
// Frost, Hail, the ice volleys and the casting impact.
extern char ResourceIdIceChipSheet;
#define RESOURCE_ICE_CHIP_SHEET ((Sint32)&ResourceIdIceChipSheet)
// The Boreas summon; the showers, the casting impact, Break and Drain, the healing motes, Cutting
// Edge and the rushing strikes take its colours.
extern char ResourceIdIceBlockSheet;
#define RESOURCE_ICE_BLOCK_SHEET ((Sint32)&ResourceIdIceBlockSheet)
// Boreas, Djinn healing, Ice and Douse, Frost and the healing motes.
extern char ResourceIdSparkleDots;
#define RESOURCE_SPARKLE_DOTS ((Sint32)&ResourceIdSparkleDots)
// Judgment's gathering, and the colours of Demon Night, the orbiting bursts, Planet Diver, the
// rushing strikes, the stat changes and the status motes.
extern char ResourceIdPinkBurstSheet;
#define RESOURCE_PINK_BURST_SHEET ((Sint32)&ResourceIdPinkBurstSheet)
// Colours of the element streaks for ELEMENT_NONE.
extern char ResourceIdDustPuffSheet;
#define RESOURCE_DUST_PUFF_SHEET ((Sint32)&ResourceIdDustPuffSheet)
// Smoke Bomb and Blast, Cybele, Meteor and Outer Space, Ragnarok, Ramses, Titan Blade, Undead
// Sword, the casting impact and the status motes; Break and Drain, the healing motes and the
// rushing strikes take its colours.
extern char ResourceIdBlastSheet;
#define RESOURCE_BLAST_SHEET ((Sint32)&ResourceIdBlastSheet)
// The streaks behind the comets of Slash, Cutting Edge and the impact.
extern char ResourceIdWindStreakSheet;
#define RESOURCE_WIND_STREAK_SHEET ((Sint32)&ResourceIdWindStreakSheet)
// The Venus star field's colours (btElementGlowInitStarField), and Death Plunge's.
extern char ResourceIdVenusStarFieldPalette;
#define RESOURCE_VENUS_STAR_FIELD_PALETTE ((Sint32)&ResourceIdVenusStarFieldPalette)
// The Jupiter star field's colours (btElementGlowInitStarField), and those of Death Plunge and the
// streaks.
extern char ResourceIdJupiterStarFieldPalette;
#define RESOURCE_JUPITER_STAR_FIELD_PALETTE ((Sint32)&ResourceIdJupiterStarFieldPalette)
// Froth and Whirlwind, Bolt, Judgment, the Thor summon, the showers, the casting impact, the status
// motes and the monster strikes.
extern char ResourceIdTornadoSheet;
#define RESOURCE_TORNADO_SHEET ((Sint32)&ResourceIdTornadoSheet)
// Prism, Death Plunge and the Thor summon.
extern char ResourceIdCrystalSheet;
#define RESOURCE_CRYSTAL_SHEET ((Sint32)&ResourceIdCrystalSheet)

// Field, world map and menu resources.

// The world map's tile offsets and packed tiles, which fdWorldMapWriteLayerMetatileTile decodes one
// at a time as the view moves (perspective->tiles).
extern char ResourceIdWorldMapPerspectiveTiles;
#define RESOURCE_WORLD_MAP_PERSPECTIVE_TILES ((Sint32)&ResourceIdWorldMapPerspectiveTiles)
// The world map cells fdWorldMapInitPerspective decodes into RamMapGridMetatileBuffer, and that the
// still images put back on their way out.
extern char ResourceIdDefaultGridMetatiles;
#define RESOURCE_DEFAULT_GRID_METATILES ((Sint32)&ResourceIdDefaultGridMetatiles)
// Colosso's first opponent's recorded path, played by the Azart match.
extern char ResourceIdAzartOpponentPath;
#define RESOURCE_AZART_OPPONENT_PATH ((Sint32)&ResourceIdAzartOpponentPath)
// The world map's tileset animations (fdTilesetAnimationStartAll).
extern char ResourceIdDefaultTileAnimation;
#define RESOURCE_DEFAULT_TILE_ANIMATION ((Sint32)&ResourceIdDefaultTileAnimation)
// The attributes of the world map's cell tiles, loaded with its default cells.
extern char ResourceIdDefaultMetatileTilesAttributes;
#define RESOURCE_DEFAULT_METATILE_TILES_ATTRIBUTES                                                 \
	((Sint32)&ResourceIdDefaultMetatileTilesAttributes)

// The world map's two graphics sets (WorldMapGraphicsResources), each a palette and five tilesets:
// fdWorldMapLoadGraphics decodes the first four into char blocks 2 and 3 and the fifth into
// MapGridAnimationTiles for the tileset animation. The second set is for
// WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD.

extern char ResourceIdWorldMapPaletteA; // The first set's colours.
#define RESOURCE_WORLD_MAP_PALETTE_A ((Sint32)&ResourceIdWorldMapPaletteA)
extern char ResourceIdWorldMapTilesA;   // First set, first tileset.
#define RESOURCE_WORLD_MAP_TILES_A ((Sint32)&ResourceIdWorldMapTilesA)
extern char ResourceIdWorldMapTilesB;   // First set, second tileset.
#define RESOURCE_WORLD_MAP_TILES_B ((Sint32)&ResourceIdWorldMapTilesB)
extern char ResourceIdWorldMapTilesC;   // First set, third tileset.
#define RESOURCE_WORLD_MAP_TILES_C ((Sint32)&ResourceIdWorldMapTilesC)
extern char ResourceIdWorldMapTilesD;   // First set, fourth tileset.
#define RESOURCE_WORLD_MAP_TILES_D ((Sint32)&ResourceIdWorldMapTilesD)
extern char ResourceIdWorldMapTilesE;   // First set, animation pictures.
#define RESOURCE_WORLD_MAP_TILES_E ((Sint32)&ResourceIdWorldMapTilesE)
extern char ResourceIdWorldMapPaletteB; // The snowfield set's colours.
#define RESOURCE_WORLD_MAP_PALETTE_B ((Sint32)&ResourceIdWorldMapPaletteB)
extern char ResourceIdWorldMapTilesF;   // Snowfield set, first tileset.
#define RESOURCE_WORLD_MAP_TILES_F ((Sint32)&ResourceIdWorldMapTilesF)
extern char ResourceIdWorldMapTilesG;   // Snowfield set, second tileset.
#define RESOURCE_WORLD_MAP_TILES_G ((Sint32)&ResourceIdWorldMapTilesG)
extern char ResourceIdWorldMapTilesH;   // Snowfield set, third tileset.
#define RESOURCE_WORLD_MAP_TILES_H ((Sint32)&ResourceIdWorldMapTilesH)
extern char ResourceIdWorldMapTilesI;   // Snowfield set, fourth tileset.
#define RESOURCE_WORLD_MAP_TILES_I ((Sint32)&ResourceIdWorldMapTilesI)
extern char ResourceIdWorldMapTilesJ;   // Snowfield set, animation pictures.
#define RESOURCE_WORLD_MAP_TILES_J ((Sint32)&ResourceIdWorldMapTilesJ)
// The Satrage match's opponent path (scColosseumSatrageMatchStartOpponentPath).
extern char ResourceIdSatrageOpponentPath;
#define RESOURCE_SATRAGE_OPPONENT_PATH ((Sint32)&ResourceIdSatrageOpponentPath)
// The Navampa match's opponent path.
extern char ResourceIdNavampaOpponentPath;
#define RESOURCE_NAVAMPA_OPPONENT_PATH ((Sint32)&ResourceIdNavampaOpponentPath)
// Colosso's lettering pictures, which scColossoLoadLettering decodes with their colours for sprite
// palette 15.
extern char ResourceIdLettering;
#define RESOURCE_LETTERING ((Sint32)&ResourceIdLettering)
// The first of eight 256-byte cursor pictures in consecutive rows; the option menu's slider marker
// adds the picture to it (mnSettingsMenuAnimateCursors).
extern char ResourceIdMenuCursorPictures;
#define RESOURCE_MENU_CURSOR_PICTURES ((Sint32)&ResourceIdMenuCursorPictures)
// The 32x32 icons and portraits, each with 16 colours before its tiles (mnIconLoadWithPalette).
extern char ResourceIdIcons;
#define RESOURCE_ICONS ((Sint32)&ResourceIdIcons)
// The command icons, a halfword offset each and then 1 KB of 256-colour pixels compressed; an
// icon's number is also its name's in MsgCommandName.
extern char ResourceIdCommandIcons;
#define RESOURCE_COMMAND_ICONS ((Sint32)&ResourceIdCommandIcons)
// The empty layout's header (map/empty_map), the first layout row: MapsWork counts its rows from
// here, and fdMapGridLoadMap adds them to it.
extern char ResourceIdEmptyMap;
#define RESOURCE_EMPTY_MAP ((Sint32)&ResourceIdEmptyMap)

// Battle effect pictures, continued.

extern char ResourceIdSheepSheet; // Sleep's sheep, among the status motes.
#define RESOURCE_SHEEP_SHEET ((Sint32)&ResourceIdSheepSheet)
// Poison Beat, Spinning Beat and Spider Web.
extern char ResourceIdMagentaSwirlSheet;
#define RESOURCE_MAGENTA_SWIRL_SHEET ((Sint32)&ResourceIdMagentaSwirlSheet)
// Poison Beat, Spinning Beat and Spider Web, beside MagentaSwirlSheet.
extern char ResourceIdMagentaTailSheet;
#define RESOURCE_MAGENTA_TAIL_SHEET ((Sint32)&ResourceIdMagentaTailSheet)
// Colours borrowed by Atalanta, the rush bursts, the casting impact, the lobbed shells, Spider Web,
// the streaks, the status motes and the strikes; Lucky Wheels steps toward it.
extern char ResourceIdLimePalette;
#define RESOURCE_LIME_PALETTE ((Sint32)&ResourceIdLimePalette)

// Annihilation, Smoke Bomb, Blast and the casting impact.
extern char ResourceIdFireStreakSheet;
#define RESOURCE_FIRE_STREAK_SHEET ((Sint32)&ResourceIdFireStreakSheet)
// Gust, Flying Attack, the beams, the casting impact and the rushing strikes.
extern char ResourceIdYellowOrbSheet;
#define RESOURCE_YELLOW_ORB_SHEET ((Sint32)&ResourceIdYellowOrbSheet)
extern char ResourceIdBlueBurstSheet; // The rushing strikes.
#define RESOURCE_BLUE_BURST_SHEET ((Sint32)&ResourceIdBlueBurstSheet)
// Dragon Cloud and the rushing strikes.
extern char ResourceIdFirePillarSheetB;
#define RESOURCE_FIRE_PILLAR_SHEET_B ((Sint32)&ResourceIdFirePillarSheetB)
// The orbiting skulls, Planet Diver and the casting impact.
extern char ResourceIdSkullSheet;
#define RESOURCE_SKULL_SHEET ((Sint32)&ResourceIdSkullSheet)
// Bolt, Plasma, Ray, Meteor, Tiamat, Titan Blade, the casting impact and the monster strikes.
extern char ResourceIdLightningBoltSheet;
#define RESOURCE_LIGHTNING_BOLT_SHEET ((Sint32)&ResourceIdLightningBoltSheet)
// Bind, Astral Blast, Shining Star and the casting impact.
extern char ResourceIdRuneSheet;
#define RESOURCE_RUNE_SHEET ((Sint32)&ResourceIdRuneSheet)
extern char ResourceIdRockSpireSheet; // The casting impact.
#define RESOURCE_ROCK_SPIRE_SHEET ((Sint32)&ResourceIdRockSpireSheet)
extern char ResourceIdFlameSheetA;    // The casting impact's flames.
#define RESOURCE_FLAME_SHEET_A ((Sint32)&ResourceIdFlameSheetA)
// The small Venus Djinni of the casting impact, the Djinn attacks, Ground and the volleys' prelude.
extern char ResourceIdVenusDjinnSmallSheet;
#define RESOURCE_VENUS_DJINN_SMALL_SHEET ((Sint32)&ResourceIdVenusDjinnSmallSheet)
// The small Mars Djinni of the beams, the casting impact, the Djinn attacks, the healing motes and
// the volleys' prelude.
extern char ResourceIdMarsDjinnSmallSheet;
#define RESOURCE_MARS_DJINN_SMALL_SHEET ((Sint32)&ResourceIdMarsDjinnSmallSheet)
// The small Jupiter Djinni of the casting impact, the Djinn attacks and the volleys' prelude.
extern char ResourceIdJupiterDjinnSmallSheet;
#define RESOURCE_JUPITER_DJINN_SMALL_SHEET ((Sint32)&ResourceIdJupiterDjinnSmallSheet)
extern char ResourceIdBlueBeastSheet;    // The casting impact.
#define RESOURCE_BLUE_BEAST_SHEET ((Sint32)&ResourceIdBlueBeastSheet)
extern char ResourceIdRedCrescentSheetB; // The casting impact.
#define RESOURCE_RED_CRESCENT_SHEET_B ((Sint32)&ResourceIdRedCrescentSheetB)
extern char ResourceIdTanPalette;        // Colours for the casting impact.
#define RESOURCE_TAN_PALETTE ((Sint32)&ResourceIdTanPalette)
// Colours for the casting impact, the streaks and the motes.
extern char ResourceIdCyanPalette;
#define RESOURCE_CYAN_PALETTE ((Sint32)&ResourceIdCyanPalette)
// The title intro's sprite colours and compressed sprite tiles (mnTitleLoadIntroBackgrounds).
extern char ResourceIdIntroSun;
#define RESOURCE_INTRO_SUN ((Sint32)&ResourceIdIntroSun)
// The 256-colour picture mnTitleShowTitleScreen puts on BG2 once the intro's sprites are gone.
extern char ResourceIdTitleScreen;
#define RESOURCE_TITLE_SCREEN ((Sint32)&ResourceIdTitleScreen)
// The title intro's scrolling backgrounds: their colours, then their compressed tiles
// (mnTitleLoadIntroBackgrounds).
extern char ResourceIdIntroPeak;
#define RESOURCE_INTRO_PEAK ((Sint32)&ResourceIdIntroPeak)
extern char ResourceIdCyanSparkSheet;      // Death Plunge.
#define RESOURCE_CYAN_SPARK_SHEET ((Sint32)&ResourceIdCyanSparkSheet)
extern char ResourceIdStarBurstSheet;      // The beams.
#define RESOURCE_STAR_BURST_SHEET ((Sint32)&ResourceIdStarBurstSheet)
extern char ResourceIdCrescentSheet;       // The beams.
#define RESOURCE_CRESCENT_SHEET ((Sint32)&ResourceIdCrescentSheet)
extern char ResourceIdSwordSheet;          // Titan Blade.
#define RESOURCE_SWORD_SHEET ((Sint32)&ResourceIdSwordSheet)
extern char ResourceIdPlasmaVioletPalette; // Colours for Plasma.
#define RESOURCE_PLASMA_VIOLET_PALETTE ((Sint32)&ResourceIdPlasmaVioletPalette)
extern char ResourceIdFireSwirlSheet;      // Helm Splitter.
#define RESOURCE_FIRE_SWIRL_SHEET ((Sint32)&ResourceIdFireSwirlSheet)
extern char ResourceIdFireBurstSheet;      // Annihilation.
#define RESOURCE_FIRE_BURST_SHEET ((Sint32)&ResourceIdFireBurstSheet)
// Heat Wave's fireballs, and the walls of battle_effect_mirrored_wall.c.
extern char ResourceIdFireballSheet;
#define RESOURCE_FIREBALL_SHEET ((Sint32)&ResourceIdFireballSheet)
// The three lights of Demon Night, Rumble and Haunting.
extern char ResourceIdGatheringLightsImage;
#define RESOURCE_GATHERING_LIGHTS_IMAGE ((Sint32)&ResourceIdGatheringLightsImage)
// The beams' waves, and the rush bursts.
extern char ResourceIdWaveSheet;
#define RESOURCE_WAVE_SHEET ((Sint32)&ResourceIdWaveSheet)
extern char ResourceIdPinkStarSheet; // Helm Splitter.
#define RESOURCE_PINK_STAR_SHEET ((Sint32)&ResourceIdPinkStarSheet)
// Gaia, the walls, Revive and its family (battle_effect_djinn_beams.c) and the streaks.
extern char ResourceIdEarthWallSheet;
#define RESOURCE_EARTH_WALL_SHEET ((Sint32)&ResourceIdEarthWallSheet)
// Volcano, Titan Blade and the Nereid summon.
extern char ResourceIdFirePillarSheetA;
#define RESOURCE_FIRE_PILLAR_SHEET_A ((Sint32)&ResourceIdFirePillarSheetA)
// Colours for Volcano and the streaks.
extern char ResourceIdVolcanoRedPalette;
#define RESOURCE_VOLCANO_RED_PALETTE ((Sint32)&ResourceIdVolcanoRedPalette)
// Colours for Plasma and Volcano.
extern char ResourceIdVolcanoOrangePalette;
#define RESOURCE_VOLCANO_ORANGE_PALETTE ((Sint32)&ResourceIdVolcanoOrangePalette)
extern char ResourceIdFlameballSheet; // Planet Diver and the Djinn attacks.
#define RESOURCE_FLAMEBALL_SHEET ((Sint32)&ResourceIdFlameballSheet)
extern char ResourceIdBoulderSheet;   // Spire.
#define RESOURCE_BOULDER_SHEET ((Sint32)&ResourceIdBoulderSheet)
extern char ResourceIdRockWallSheet;  // Quake.
#define RESOURCE_ROCK_WALL_SHEET ((Sint32)&ResourceIdRockWallSheet)
// The large Jupiter Djinni of Bind and the stat changes; Lucky Wheels takes its colours.
extern char ResourceIdJupiterDjinnSheet;
#define RESOURCE_JUPITER_DJINN_SHEET ((Sint32)&ResourceIdJupiterDjinnSheet)
extern char ResourceIdShieldSheet; // The stat changes.
#define RESOURCE_SHIELD_SHEET ((Sint32)&ResourceIdShieldSheet)
extern char ResourceIdDaggerSheet; // The stat changes.
#define RESOURCE_DAGGER_SHEET ((Sint32)&ResourceIdDaggerSheet)
// The shards a stat reduction's dagger or shield breaks into (STAT_REDUCTION_SHARD_PICTURES).
extern char ResourceIdStatReductionShardSheet;
#define RESOURCE_STAT_REDUCTION_SHARD_SHEET ((Sint32)&ResourceIdStatReductionShardSheet)
extern char ResourceIdHeartSheet; // Heartrender, among the status motes.
#define RESOURCE_HEART_SHEET ((Sint32)&ResourceIdHeartSheet)
// Revive and its family (battle_effect_djinn_beams.c).
extern char ResourceIdCrescentMoonSheet;
#define RESOURCE_CRESCENT_MOON_SHEET ((Sint32)&ResourceIdCrescentMoonSheet)
// The beams and the rush bursts.
extern char ResourceIdBlueArcSheetA;
#define RESOURCE_BLUE_ARC_SHEET_A ((Sint32)&ResourceIdBlueArcSheetA)
// The beams and the rush bursts, beside BlueArcSheetA.
extern char ResourceIdBlueArcSheetB;
#define RESOURCE_BLUE_ARC_SHEET_B ((Sint32)&ResourceIdBlueArcSheetB)
// The Djinn attacks, the stat changes and the status motes.
extern char ResourceIdGlowOrbSheet;
#define RESOURCE_GLOW_ORB_SHEET ((Sint32)&ResourceIdGlowOrbSheet)
// The lobbed shells' images, whose colours are Mad Spatter's.
extern char ResourceIdGoldShellSheet;
#define RESOURCE_GOLD_SHELL_SHEET ((Sint32)&ResourceIdGoldShellSheet)
// The beams' BEAM_IMAGE, and the picture of a monster's slash.
extern char ResourceIdBeamImage;
#define RESOURCE_BEAM_IMAGE ((Sint32)&ResourceIdBeamImage)
// Ice and Douse, and the monster strikes.
extern char ResourceIdBlueBeamSheet;
#define RESOURCE_BLUE_BEAM_SHEET ((Sint32)&ResourceIdBlueBeamSheet)
// Froth and Whirlwind, and the status motes.
extern char ResourceIdVortexSheet;
#define RESOURCE_VORTEX_SHEET ((Sint32)&ResourceIdVortexSheet)
extern char ResourceIdLightningPillarSheet; // Plasma and the Thor summon.
#define RESOURCE_LIGHTNING_PILLAR_SHEET ((Sint32)&ResourceIdLightningPillarSheet)
// The shattering opening (btEffectRunShatteringOpening).
extern char ResourceIdShatteringOpeningSheet;
#define RESOURCE_SHATTERING_OPENING_SHEET ((Sint32)&ResourceIdShatteringOpeningSheet)
extern char ResourceIdDeathPlungeVioletPalette; // Colours for Death Plunge.
#define RESOURCE_DEATH_PLUNGE_VIOLET_PALETTE ((Sint32)&ResourceIdDeathPlungeVioletPalette)
extern char ResourceIdDeathPlungeRedPalette;    // Colours for Death Plunge.
#define RESOURCE_DEATH_PLUNGE_RED_PALETTE ((Sint32)&ResourceIdDeathPlungeRedPalette)
extern char ResourceIdDeathPlungeYellowPalette; // Colours for Death Plunge.
#define RESOURCE_DEATH_PLUNGE_YELLOW_PALETTE ((Sint32)&ResourceIdDeathPlungeYellowPalette)
extern char ResourceIdFlameBladeSheet;          // Ragnarok.
#define RESOURCE_FLAME_BLADE_SHEET ((Sint32)&ResourceIdFlameBladeSheet)
extern char ResourceIdIceTileSheet;             // Death Plunge.
#define RESOURCE_ICE_TILE_SHEET ((Sint32)&ResourceIdIceTileSheet)
// The streak volleys' images: three colour rows of six 20x40 images.
extern char ResourceIdFlameArcSheet;
#define RESOURCE_FLAME_ARC_SHEET ((Sint32)&ResourceIdFlameArcSheet)
extern char ResourceIdSpiderWebSheet;        // Spider Web and the lobbed shells.
#define RESOURCE_SPIDER_WEB_SHEET ((Sint32)&ResourceIdSpiderWebSheet)
extern char ResourceIdFlareSheet;            // Helm Splitter.
#define RESOURCE_FLARE_SHEET ((Sint32)&ResourceIdFlareSheet)
extern char ResourceIdFlareImage;            // Helm Splitter, beside FlareSheet.
#define RESOURCE_FLARE_IMAGE ((Sint32)&ResourceIdFlareImage)
extern char ResourceIdVioletLightningSheetB; // The Thor summon.
#define RESOURCE_VIOLET_LIGHTNING_SHEET_B ((Sint32)&ResourceIdVioletLightningSheetB)
extern char ResourceIdBlueCrescentSheet;     // Judgment.
#define RESOURCE_BLUE_CRESCENT_SHEET ((Sint32)&ResourceIdBlueCrescentSheet)
// Judgment's small flash (JUDGMENT_SHEET_FLASH_SMALL).
extern char ResourceIdJudgmentFlashSheet;
#define RESOURCE_JUDGMENT_FLASH_SHEET ((Sint32)&ResourceIdJudgmentFlashSheet)
extern char ResourceIdVioletMoteSheet; // The Thor summon.
#define RESOURCE_VIOLET_MOTE_SHEET ((Sint32)&ResourceIdVioletMoteSheet)
// Judgment's burst (JUDGMENT_SHEET_BURST).
extern char ResourceIdJudgmentBurstSheet;
#define RESOURCE_JUDGMENT_BURST_SHEET ((Sint32)&ResourceIdJudgmentBurstSheet)
extern char ResourceIdSlashSheet; // The Atalanta summon.
#define RESOURCE_SLASH_SHEET ((Sint32)&ResourceIdSlashSheet)
// Judgment's explosion (JUDGMENT_SHEET_EXPLOSION).
extern char ResourceIdExplosionSheet;
#define RESOURCE_EXPLOSION_SHEET ((Sint32)&ResourceIdExplosionSheet)
extern char ResourceIdPaleBlueGlowSheet; // The Thor summon.
#define RESOURCE_PALE_BLUE_GLOW_SHEET ((Sint32)&ResourceIdPaleBlueGlowSheet)
// The Thor summon, beside PaleBlueGlowSheet.
extern char ResourceIdBlueTallGlowSheet;
#define RESOURCE_BLUE_TALL_GLOW_SHEET ((Sint32)&ResourceIdBlueTallGlowSheet)
// Rotten Blood's green, which Decompose shares.
extern char ResourceIdGreenPalette;
#define RESOURCE_GREEN_PALETTE ((Sint32)&ResourceIdGreenPalette)
extern char ResourceIdSwordSlashSheet; // Death Plunge.
#define RESOURCE_SWORD_SLASH_SHEET ((Sint32)&ResourceIdSwordSlashSheet)
// Colours for Gaia and the streaks.
extern char ResourceIdGaiaOrangePalette;
#define RESOURCE_GAIA_ORANGE_PALETTE ((Sint32)&ResourceIdGaiaOrangePalette)
extern char ResourceIdWhirlwindSheet; // The Procne summon.
#define RESOURCE_WHIRLWIND_SHEET ((Sint32)&ResourceIdWhirlwindSheet)
// The Procne summon, after its sky is drawn.
extern char ResourceIdVioletCometSheet;
#define RESOURCE_VIOLET_COMET_SHEET ((Sint32)&ResourceIdVioletCometSheet)
// The large Mercury Djinni, whose colours Revive's family, Haunting's lights and the healing motes
// borrow; Lucky Wheels steps toward it.
extern char ResourceIdMercuryDjinnSheet;
#define RESOURCE_MERCURY_DJINN_SHEET ((Sint32)&ResourceIdMercuryDjinnSheet)
// The large Venus Djinni, whose colours Revive's family and the stat changes borrow; Lucky Wheels
// steps toward it.
extern char ResourceIdVenusDjinnSheet;
#define RESOURCE_VENUS_DJINN_SHEET ((Sint32)&ResourceIdVenusDjinnSheet)
extern char ResourceIdFireBlobSheet;     // The Kirin summon.
#define RESOURCE_FIRE_BLOB_SHEET ((Sint32)&ResourceIdFireBlobSheet)
extern char ResourceIdYellowRingSheet;   // The Cybele summon.
#define RESOURCE_YELLOW_RING_SHEET ((Sint32)&ResourceIdYellowRingSheet)
extern char ResourceIdBluePalette;       // Colours for the lobbed shells.
#define RESOURCE_BLUE_PALETTE ((Sint32)&ResourceIdBluePalette)
extern char ResourceIdWispSheet;         // The stat changes.
#define RESOURCE_WISP_SHEET ((Sint32)&ResourceIdWispSheet)
extern char ResourceIdPortalSheet;       // Hail and Ground.
#define RESOURCE_PORTAL_SHEET ((Sint32)&ResourceIdPortalSheet)
extern char ResourceIdVioletStreakSheet; // The stat changes.
#define RESOURCE_VIOLET_STREAK_SHEET ((Sint32)&ResourceIdVioletStreakSheet)
extern char ResourceIdThornVineSheet;    // The Cybele summon.
#define RESOURCE_THORN_VINE_SHEET ((Sint32)&ResourceIdThornVineSheet)
// Ice and Douse, and the healing motes.
extern char ResourceIdIceShardSheet;
#define RESOURCE_ICE_SHARD_SHEET ((Sint32)&ResourceIdIceShardSheet)
extern char ResourceIdLightFanSheet; // The Thor summon.
#define RESOURCE_LIGHT_FAN_SHEET ((Sint32)&ResourceIdLightFanSheet)
extern char ResourceIdFlameSheetB;   // Meteor and Outer Space.
#define RESOURCE_FLAME_SHEET_B ((Sint32)&ResourceIdFlameSheetB)
// The monster strikes' picture for every kind but a slash.
extern char ResourceIdBlueDropletSheet;
#define RESOURCE_BLUE_DROPLET_SHEET ((Sint32)&ResourceIdBlueDropletSheet)
// The star field's rings (btElementGlowInitStarField), with the Mercury field's colours.
extern char ResourceIdBlueRingSheet;
#define RESOURCE_BLUE_RING_SHEET ((Sint32)&ResourceIdBlueRingSheet)
// The Mars star field's colours (btElementGlowInitStarField).
extern char ResourceIdMarsStarFieldPalette;
#define RESOURCE_MARS_STAR_FIELD_PALETTE ((Sint32)&ResourceIdMarsStarFieldPalette)
// The path Atalanta's particles follow.
extern char ResourceIdAtalantaPath;
#define RESOURCE_ATALANTA_PATH ((Sint32)&ResourceIdAtalantaPath)
// The path of Dragon Cloud's flame.
extern char ResourceIdDragonCloudFlamePath;
#define RESOURCE_DRAGON_CLOUD_FLAME_PATH ((Sint32)&ResourceIdDragonCloudFlamePath)
#endif
