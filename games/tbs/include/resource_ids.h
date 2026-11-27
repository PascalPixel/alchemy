#ifndef ALCHEMY_RESOURCE_IDS_H
#define ALCHEMY_RESOURCE_IDS_H

/* @file */
/* Resource numbers, named where their contents are known. */
/* */
/* Each ResourceId_ is an absolute symbol whose value is the resource's row */
/* in syResourceDirectoryTable (system/resource_directory.s); C names the */
/* number by its macro, RESOURCE_WINDOW_TILES for ResourceId_WindowTiles. */

/* Twelve ASCII digits, yymmddhhmmss, and a newline, different in each */
/* edition. syCopyBuildStamp copies it into BuildStampWork, and */
/* gmBuildStampGetTime turns its time into the number saves compare. */
extern char ResourceId_BuildStamp;
#define RESOURCE_BUILD_STAMP ((s32)&ResourceId_BuildStamp)
/* The row of Stubs_Title. The Title hands it to syDummyNoOpHook, which */
/* ignores it. */
extern char ResourceId_TitleStubs;
#define RESOURCE_TITLE_STUBS ((s32)&ResourceId_TitleStubs)
/* The row of btStubsBattleEffect. Lucky Dice and Lucky Wheels hand it to */
/* syDummyNoOpHook as they start. */
extern char ResourceId_BattleEffectStubs;
#define RESOURCE_BATTLE_EFFECT_STUBS ((s32)&ResourceId_BattleEffectStubs)
/* The row of Stubs_Resource, which the sprite viewer passes to */
/* syDummyReturnZero when Select is pressed with B held. */
extern char ResourceId_ResourceStubs;
#define RESOURCE_RESOURCE_STUBS ((s32)&ResourceId_ResourceStubs)
/* A second row that also holds Stubs_Resource; the sprite viewer */
/* passes it when Select is pressed without B. */
extern char ResourceId_ResourceStubsAlternate;
#define RESOURCE_RESOURCE_FAR_CALLS_ALTERNATE                                  \
	((s32)&ResourceId_ResourceStubsAlternate)
/* The window font and border tiles, 256 tiles of 4 bits a pixel. */
/* syWindowLoadGraphics copies them into BG character block 0, and the */
/* window text functions draw their glyphs from here. */
extern char ResourceId_WindowTiles;
#define RESOURCE_WINDOW_TILES ((s32)&ResourceId_WindowTiles)
/* The narrow glyphs the Japanese edition draws in the battle message */
/* window; the other editions look it up there and then draw from */
/* WindowTiles. */
extern char ResourceId_SmallFont;
#define RESOURCE_SMALL_FONT ((s32)&ResourceId_SmallFont)
extern char
	ResourceId_PublisherLogo; /* Shown first by mnTitleShowPublisherLogo. */
#define RESOURCE_PUBLISHER_LOGO ((s32)&ResourceId_PublisherLogo)
/* The Camelot logo's four frames of tiles, which mnTitleShowCamelotLogo */
/* cycles. */
extern char ResourceId_CamelotLogo;
#define RESOURCE_CAMELOT_LOGO ((s32)&ResourceId_CamelotLogo)
/* The title picture on BG2, loaded by scTitleLoadBackground and, for the main
 */
/* menu, by scFileScrLoadBackground. */
extern char ResourceId_GoldenSunLogo;
#define RESOURCE_GOLDEN_SUN_LOGO ((s32)&ResourceId_GoldenSunLogo)
/* The still world map fdMapScrShow shows for a */
/* FIELD_RAISED_TRIGGER_MAP_SCREEN event; it loads this one whatever picture */
/* it's passed. */
extern char ResourceId_WorldMapPicture;
#define RESOURCE_WORLD_MAP_PICTURE ((s32)&ResourceId_WorldMapPicture)
/* The lettering sprites scTitleLoadSprs loads: 40 tiles of 16 colours in */
/* English and Japanese, a whole 224-colour screen in the other editions, of */
/* which it keeps the bottom 32 rows. */
extern char ResourceId_ToBeContinuedLettering;
#define RESOURCE_TO_BE_CONTINUED_LETTERING                                     \
	((s32)&ResourceId_ToBeContinuedLettering)

/* Battle backgrounds. fdBattleBackgroundSelectForMap picks one for the current
 */
/* scene from Scene_BattleBackgroundRules (field/battle_background_rules.c), */
/* and on the world map fdBattleBackgroundSelectForEnvironment picks one by */
/* battle environment; either leaves it in MainRoutineStateWork.battleBg
 */
/* for the battle. The credits (Credits_Backgrounds) shows every one but */
/* DesertBackdrop and AerialViewBackdrop, and some summons draw one as their */
/* sky. */

/* Suhalla Gate's checkpoint and Dew rooms. The debug battle menu fights on it
 */
/* too. */
extern char ResourceId_SuhallaGateBackdrop;
#define RESOURCE_SUHALLA_GATE_BACKDROP ((s32)&ResourceId_SuhallaGateBackdrop)
extern char ResourceId_PirateShipBackdrop; /* SceneId_PirateShip. */
#define RESOURCE_PIRATE_SHIP_BACKDROP ((s32)&ResourceId_PirateShipBackdrop)
extern char ResourceId_CrossboneIsleCaveBackdrop; /* Crossbone Isle's floors. */
#define RESOURCE_CROSSBONE_ISLE_CAVE_BACKDROP                                  \
	((s32)&ResourceId_CrossboneIsleCaveBackdrop)
/* Colosso's three matches (Azart, Satrage and Navampa) and the Battle Arena. */
extern char ResourceId_ColossoBackdrop;
#define RESOURCE_COLOSSO_BACKDROP ((s32)&ResourceId_ColossoBackdrop)
/* SceneId_VenusLighthouseAerie. */
extern char ResourceId_VenusLighthouseAerieBackdrop;
#define RESOURCE_VENUS_LIGHTHOUSE_AERIE_BACKDROP                               \
	((s32)&ResourceId_VenusLighthouseAerieBackdrop)
/* Venus Lighthouse's floors, from the ground floor to the fourth. */
extern char ResourceId_VenusLighthouseBackdrop;
#define RESOURCE_VENUS_LIGHTHOUSE_BACKDROP                                     \
	((s32)&ResourceId_VenusLighthouseBackdrop)
/* Suhalla Desert, north and south. */
extern char ResourceId_SuhallaDesertBackdrop;
#define RESOURCE_SUHALLA_DESERT_BACKDROP                                       \
	((s32)&ResourceId_SuhallaDesertBackdrop)
/* Altmiller Cave and Lunpa Fortress. */
extern char ResourceId_AltmillerCaveBackdrop;
#define RESOURCE_ALTMILLER_CAVE_BACKDROP                                       \
	((s32)&ResourceId_AltmillerCaveBackdrop)
extern char
	ResourceId_TunnelRuinsBackdrop; /* Tunnel Ruins' two statue rooms. */
#define RESOURCE_TUNNEL_RUINS_BACKDROP ((s32)&ResourceId_TunnelRuinsBackdrop)
/* Babi Lighthouse, and SceneId_CrossboneIsleBane on Crossbone Isle. */
extern char ResourceId_BabiLighthouseBackdrop;
#define RESOURCE_BABI_LIGHTHOUSE_BACKDROP                                      \
	((s32)&ResourceId_BabiLighthouseBackdrop)
/* Lamakan Desert's three scenes. */
extern char ResourceId_LamakanDesertBackdrop;
#define RESOURCE_LAMAKAN_DESERT_BACKDROP                                       \
	((s32)&ResourceId_LamakanDesertBackdrop)
extern char ResourceId_MogallForestBackdrop; /* Mogall Forest's three scenes. */
#define RESOURCE_MOGALL_FOREST_BACKDROP ((s32)&ResourceId_MogallForestBackdrop)
/* SceneId_MercuryLighthouseAerie. */
extern char ResourceId_MercuryLighthouseAerieBackdrop;
#define RESOURCE_MERCURY_LIGHTHOUSE_AERIE_BACKDROP                             \
	((s32)&ResourceId_MercuryLighthouseAerieBackdrop)
/* SceneId_ShipRooms. */
extern char ResourceId_TolbiShipRoomsBackdrop;
#define RESOURCE_TOLBI_SHIP_ROOMS_BACKDROP                                     \
	((s32)&ResourceId_TolbiShipRoomsBackdrop)
/* Fuchin Falls Cave, and SceneId_CrossboneIsleFloatingLogs on Crossbone */
/* Isle. */
extern char ResourceId_FuchinFallsCaveBackdrop;
#define RESOURCE_FUCHIN_FALLS_CAVE_BACKDROP                                    \
	((s32)&ResourceId_FuchinFallsCaveBackdrop)
extern char ResourceId_KolimaForestBackdrop; /* Kolima Forest's five scenes. */
#define RESOURCE_KOLIMA_FOREST_BACKDROP ((s32)&ResourceId_KolimaForestBackdrop)
/* SceneId_TreeRooms. */
extern char ResourceId_TretTreeRoomsBackdrop;
#define RESOURCE_TRET_TREE_ROOMS_BACKDROP                                      \
	((s32)&ResourceId_TretTreeRoomsBackdrop)
/* Goma Cave, Kolima Tunnel, Bilibin Cave and Gateway Cave, and any scene the */
/* table doesn't list, since it comes last. */
extern char ResourceId_GomaCaveBackdrop;
#define RESOURCE_GOMA_CAVE_BACKDROP ((s32)&ResourceId_GomaCaveBackdrop)
/* The vault rooms and Lalivero's scenes, Faran's house included. */
extern char ResourceId_RoomsBackdrop;
#define RESOURCE_ROOMS_BACKDROP ((s32)&ResourceId_RoomsBackdrop)
/* Altin Peak's scenes but the Hydros statue. */
extern char ResourceId_AltinPeakBackdrop;
#define RESOURCE_ALTIN_PEAK_BACKDROP ((s32)&ResourceId_AltinPeakBackdrop)
/* SceneId_TolbiShipDeck and SceneId_TolbiShipMast. */
extern char ResourceId_TolbiShipDeckBackdrop;
#define RESOURCE_TOLBI_SHIP_DECK_BACKDROP                                      \
	((s32)&ResourceId_TolbiShipDeckBackdrop)
/* Mercury Lighthouse's floors below the aerie and its exterior. */
extern char ResourceId_MercuryLighthouseBackdrop;
#define RESOURCE_MERCURY_LIGHTHOUSE_BACKDROP                                   \
	((s32)&ResourceId_MercuryLighthouseBackdrop)
/* Vale's scenes, the stormy night included; the credits open on it. */
extern char ResourceId_ValeStormBackdrop;
#define RESOURCE_VALE_STORM_BACKDROP ((s32)&ResourceId_ValeStormBackdrop)
extern char ResourceId_TretTreeBranchBackdrop; /* SceneId_TretTreeBranch. */
#define RESOURCE_TRET_TREE_BRANCH_BACKDROP                                     \
	((s32)&ResourceId_TretTreeBranchBackdrop)
/* Set by the first slot of ENCOUNTER_ZONE_SCRIPTED */
/* (fdRequestBattleFromEncounterTable), and by the storm opening of */
/* btTransitionRun, where the enemies alone take part. */
extern char ResourceId_VenusLighthouseAerieStormBackdrop;
#define RESOURCE_VENUS_LIGHTHOUSE_AERIE_STORM_BACKDROP                         \
	((s32)&ResourceId_VenusLighthouseAerieStormBackdrop)
/* World map beaches and shallows; Judgment draws it too. */
extern char ResourceId_BeachBackdrop;
#define RESOURCE_BEACH_BACKDROP ((s32)&ResourceId_BeachBackdrop)
/* World map snowfields and snowy forests. */
extern char ResourceId_SnowfieldBackdrop;
#define RESOURCE_SNOWFIELD_BACKDROP ((s32)&ResourceId_SnowfieldBackdrop)
extern char ResourceId_GrasslandBackdrop; /* World map grassland. */
#define RESOURCE_GRASSLAND_BACKDROP ((s32)&ResourceId_GrasslandBackdrop)
/* Sol Sanctum's rooms, SceneId_AltinPeakHydrosStatue and */
/* SceneId_CrossboneIsleSteppingStones, and any battle environment without a */
/* battle background of its own. */
extern char ResourceId_SolSanctumBackdrop;
#define RESOURCE_SOL_SANCTUM_BACKDROP ((s32)&ResourceId_SolSanctumBackdrop)
/* World map forests; the Judgment and Kirin summons draw it too. */
extern char ResourceId_ForestBackdrop;
#define RESOURCE_FOREST_BACKDROP ((s32)&ResourceId_ForestBackdrop)
/* No battle is fought on it: the Atalanta, Boreas, Judgment and Procne */
/* summons draw it as their sky, and the credits close on it. */
extern char ResourceId_VioletSkyBackdrop;
#define RESOURCE_VIOLET_SKY_BACKDROP ((s32)&ResourceId_VioletSkyBackdrop)
/* World map desert, and the sky of the Ramses summon. */
extern char ResourceId_DesertBackdrop;
#define RESOURCE_DESERT_BACKDROP ((s32)&ResourceId_DesertBackdrop)
/* The sky of the Neptune and Thor summons. */
extern char ResourceId_DuskCloudsBackdrop;
#define RESOURCE_DUSK_CLOUDS_BACKDROP ((s32)&ResourceId_DuskCloudsBackdrop)
extern char ResourceId_AerialViewBackdrop; /* Drawn only by Judgment. */
#define RESOURCE_AERIAL_VIEW_BACKDROP ((s32)&ResourceId_AerialViewBackdrop)

/* The Lucky Wheels machine around the wheels (fdLuckyWheelsRun): 16 colours */
/* for BG palette 10, then the picture. */
extern char ResourceId_LuckyWheelsMachinePicture;
#define RESOURCE_LUCKY_WHEELS_MACHINE_PICTURE                                  \
	((s32)&ResourceId_LuckyWheelsMachinePicture)
/* The wheels' symbol sprites: 480 bytes of sprite colours, then the */
/* compressed tiles. */
extern char ResourceId_LuckyWheelsIcons;
#define RESOURCE_LUCKY_WHEELS_ICONS ((s32)&ResourceId_LuckyWheelsIcons)
/* The jackpot sprites, with their colours for sprite palette 15. */
extern char ResourceId_LuckyWheelsJackpot;
#define RESOURCE_LUCKY_WHEELS_JACKPOT ((s32)&ResourceId_LuckyWheelsJackpot)
/* The Lucky Dice board, with its colours, on the background (fdLuckyDiceRun).
 */
extern char ResourceId_LuckyDiceBoard;
#define RESOURCE_LUCKY_DICE_BOARD ((s32)&ResourceId_LuckyDiceBoard)
/* The Lucky Dice sprites and their colours. */
extern char ResourceId_LuckyDiceSprites;
#define RESOURCE_LUCKY_DICE_SPRITES ((s32)&ResourceId_LuckyDiceSprites)

/* The flashing strikes' pictures (battle_effect/battle_effect_thrash.c). Each
 */
/* holds two pictures; the first of a pair fills state->imageStorage, the second
 */
/* the start of Ram_MapGridCellBuffer. */

extern char ResourceId_VorpalSlashImageA; /* Vorpal Slash, first of its pair. */
#define RESOURCE_VORPAL_SLASH_IMAGE_A ((s32)&ResourceId_VorpalSlashImageA)
extern char
	ResourceId_VorpalSlashImageB; /* Vorpal Slash, second of its pair. */
#define RESOURCE_VORPAL_SLASH_IMAGE_B ((s32)&ResourceId_VorpalSlashImageB)
extern char ResourceId_QuickStrikeImageA; /* Quick Strike, first of its pair. */
#define RESOURCE_QUICK_STRIKE_IMAGE_A ((s32)&ResourceId_QuickStrikeImageA)
extern char
	ResourceId_QuickStrikeImageB; /* Quick Strike, second of its pair. */
#define RESOURCE_QUICK_STRIKE_IMAGE_B ((s32)&ResourceId_QuickStrikeImageB)
/* First of the pair for FLASHING_STRIKE_UNUSED, which nothing passes. */
extern char ResourceId_UnusedFlashingStrikeImageA;
#define RESOURCE_UNUSED_FLASHING_STRIKE_IMAGE_A                                \
	((s32)&ResourceId_UnusedFlashingStrikeImageA)
/* Second of the pair for FLASHING_STRIKE_UNUSED. */
extern char ResourceId_UnusedFlashingStrikeImageB;
#define RESOURCE_UNUSED_FLASHING_STRIKE_IMAGE_B                                \
	((s32)&ResourceId_UnusedFlashingStrikeImageB)

/* Battle effect pictures. A sprite sheet holds 64 colours (128 bytes) and then
 */
/* its compressed pictures, which the animation parts load with */
/* btResourceLoadAndDecompress; a palette holds colours alone, which a part */
/* copies over a shared sprite sheet's to recolour it, and some parts borrow a
 */
/* sprite sheet's colours the same way. Each note names the animations that read
 */
/* it. */

extern char ResourceId_LimeColumnSheet; /* Undead Sword. */
#define RESOURCE_LIME_COLUMN_SHEET ((s32)&ResourceId_LimeColumnSheet)
extern char ResourceId_DemonFaceSheet; /* The blessings' burst showers. */
#define RESOURCE_DEMON_FACE_SHEET ((s32)&ResourceId_DemonFaceSheet)
/* The blessings' burst showers, beside DemonFaceSheet. */
extern char ResourceId_VioletLightningSheetA;
#define RESOURCE_VIOLET_LIGHTNING_SHEET_A                                      \
	((s32)&ResourceId_VioletLightningSheetA)
/* The yellow sparks of Wing Flutter, Wing Beat and Wing Stroke. */
extern char ResourceId_YellowSparkSheet;
#define RESOURCE_YELLOW_SPARK_SHEET ((s32)&ResourceId_YellowSparkSheet)
/* Crazy Voice's bouncing pellets, drawn in SlashCometSheet's colours. */
extern char ResourceId_ProjectileVolleyImage;
#define RESOURCE_PROJECTILE_VOLLEY_IMAGE                                       \
	((s32)&ResourceId_ProjectileVolleyImage)
/* The Shuriken volley's spinning shurikens. */
extern char ResourceId_ShurikenSheet;
#define RESOURCE_SHURIKEN_SHEET ((s32)&ResourceId_ShurikenSheet)
extern char ResourceId_TargetBurstImage; /* The target bursts of */
										 /* battle_effect_orbiting.c. */
#define RESOURCE_TARGET_BURST_IMAGE ((s32)&ResourceId_TargetBurstImage)
/* Hail, the Neptune summon and the blessings' burst showers; Nereid takes */
/* its colours. */
extern char ResourceId_WaterSpraySheet;
#define RESOURCE_WATER_SPRAY_SHEET ((s32)&ResourceId_WaterSpraySheet)
/* The casting impact, the Djinn attacks and the Neptune summon. */
extern char ResourceId_BlueFlameSheet;
#define RESOURCE_BLUE_FLAME_SHEET ((s32)&ResourceId_BlueFlameSheet)
/* The particles nearly every animation loads, from Annihilation to Volcano and
 */
/* most summons. */
extern char ResourceId_YellowGlowSheet;
#define RESOURCE_YELLOW_GLOW_SHEET ((s32)&ResourceId_YellowGlowSheet)
/* A second shared particle sprite sheet: the beams, Bind, Death Plunge, */
/* Ice and Douse, the Neptune summon, the stat changes, and Lucky Wheels. */
extern char ResourceId_BlueGlowSheet;
#define RESOURCE_BLUE_GLOW_SHEET ((s32)&ResourceId_BlueGlowSheet)
/* Curse's flames (btEffectRunCurse). */
extern char ResourceId_FlameColumnSheet;
#define RESOURCE_FLAME_COLUMN_SHEET ((s32)&ResourceId_FlameColumnSheet)
/* Shared by the blessings' showers, the casting impact, Death Plunge, Dragon */
/* Cloud, Planet Diver, Plasma, Ragnarok, Tiamat and Titan Blade. */
extern char ResourceId_FlashBurstSheet;
#define RESOURCE_FLASH_BURST_SHEET ((s32)&ResourceId_FlashBurstSheet)
extern char ResourceId_ThornSheet;
#define RESOURCE_THORN_SHEET ((s32)&ResourceId_ThornSheet)
/* Slash's comet, which the impact's comets (PROJECTILE_VOLLEY_IMPACT_COMETS) */
/* fire too, and the colours of Crazy Voice's pellets. */
extern char ResourceId_SlashCometSheet;
#define RESOURCE_SLASH_COMET_SHEET ((s32)&ResourceId_SlashCometSheet)
/* Wind Slash's comet. */
extern char ResourceId_WindSlashCometSheet;
#define RESOURCE_WIND_SLASH_COMET_SHEET ((s32)&ResourceId_WindSlashCometSheet)
/* Sonic Slash's comet, and Cutting Edge's in ice colours. */
extern char ResourceId_SonicSlashCometSheet;
#define RESOURCE_SONIC_SLASH_COMET_SHEET ((s32)&ResourceId_SonicSlashCometSheet)
/* GROWING_COLUMNS_PILLARS, in btEffectRunGrowingColumns. */
extern char ResourceId_GreenPillarSheet;
#define RESOURCE_GREEN_PILLAR_SHEET ((s32)&ResourceId_GreenPillarSheet)
/* btEffectRunGrowingColumns' other modes. */
extern char ResourceId_VineSheet;
#define RESOURCE_VINE_SHEET ((s32)&ResourceId_VineSheet)
extern char ResourceId_StarDotSheet; /* The swirling stars of */
									 /* battle_effect_orbiting.c. */
#define RESOURCE_STAR_DOT_SHEET ((s32)&ResourceId_StarDotSheet)
/* Borrowed mostly for its colours: by the showers, the casting impact, */
/* Rumble's lights, the healing motes, the lobbed shells, the walls, the */
/* stat changes, the status motes and the monster strikes. */
extern char ResourceId_MarsDjinnSheet;
#define RESOURCE_MARS_DJINN_SHEET ((s32)&ResourceId_MarsDjinnSheet)
/* The small Mercury Djinni of the casting impact, the Djinn attacks, Djinn */
/* healing, Hail and the ice volleys' prelude. */
extern char ResourceId_MercuryDjinnSmallSheet;
#define RESOURCE_MERCURY_DJINN_SMALL_SHEET                                     \
	((s32)&ResourceId_MercuryDjinnSmallSheet)
/* The casting impact, the lobbed shells, the orbiting bursts, the status */
/* motes and the monster strikes. */
extern char ResourceId_SmokeSheet;
#define RESOURCE_SMOKE_SHEET ((s32)&ResourceId_SmokeSheet)
/* Colours for one style of the blessings' showers and of the monster */
/* strikes. */
extern char ResourceId_PinkPalette;
#define RESOURCE_PINK_PALETTE ((s32)&ResourceId_PinkPalette)
extern char ResourceId_GreenVineSheet;
#define RESOURCE_GREEN_VINE_SHEET ((s32)&ResourceId_GreenVineSheet)
extern char ResourceId_ReaperSheet; /* Condemn and the casting impact. */
#define RESOURCE_REAPER_SHEET ((s32)&ResourceId_ReaperSheet)
/* Condemn and the casting impact, beside ReaperSheet. */
extern char ResourceId_RedCrescentSheetA;
#define RESOURCE_RED_CRESCENT_SHEET_A ((s32)&ResourceId_RedCrescentSheetA)
extern char ResourceId_SpiralSheet; /* The orbiting spirals of */
									/* battle_effect_planet_diver.c. */
#define RESOURCE_SPIRAL_SHEET ((s32)&ResourceId_SpiralSheet)
/* The fire family: Fire, Flare, Atalanta, Dragon Cloud, Tiamat, the casting */
/* impact, the Djinn attacks, Death Plunge, the streaks and the strikes; */
/* Lucky Wheels steps its colours toward it. */
extern char ResourceId_EmberStreakSheet;
#define RESOURCE_EMBER_STREAK_SHEET ((s32)&ResourceId_EmberStreakSheet)
/* Frost, Hail, the ice volleys and the casting impact. */
extern char ResourceId_IceChipSheet;
#define RESOURCE_ICE_CHIP_SHEET ((s32)&ResourceId_IceChipSheet)
/* The Boreas summon; the showers, the casting impact, Break and Drain, the */
/* healing motes, Cutting Edge and the rushing strikes take its colours. */
extern char ResourceId_IceBlockSheet;
#define RESOURCE_ICE_BLOCK_SHEET ((s32)&ResourceId_IceBlockSheet)
/* Boreas, Djinn healing, Ice and Douse, Frost and the healing motes. */
extern char ResourceId_SparkleDots;
#define RESOURCE_SPARKLE_DOTS ((s32)&ResourceId_SparkleDots)
/* Judgment's gathering, and the colours of Demon Night, the orbiting bursts, */
/* Planet Diver, the rushing strikes, the stat changes and the status motes. */
extern char ResourceId_PinkBurstSheet;
#define RESOURCE_PINK_BURST_SHEET ((s32)&ResourceId_PinkBurstSheet)
/* Colours of the element streaks for ELEMENT_NONE. */
extern char ResourceId_DustPuffSheet;
#define RESOURCE_DUST_PUFF_SHEET ((s32)&ResourceId_DustPuffSheet)
/* Smoke Bomb and Blast, Cybele, Meteor and Outer Space, Ragnarok, Ramses, */
/* Titan Blade, Undead Sword, the casting impact and the status motes; Break */
/* and Drain, the healing motes and the rushing strikes take its colours. */
extern char ResourceId_BlastSheet;
#define RESOURCE_BLAST_SHEET ((s32)&ResourceId_BlastSheet)
/* The streaks behind the comets of Slash, Cutting Edge and the impact. */
extern char ResourceId_WindStreakSheet;
#define RESOURCE_WIND_STREAK_SHEET ((s32)&ResourceId_WindStreakSheet)
/* The Venus star field's colours (btElementGlowInitStarField), and */
/* Death Plunge's. */
extern char ResourceId_VenusStarFieldPalette;
#define RESOURCE_VENUS_STAR_FIELD_PALETTE                                      \
	((s32)&ResourceId_VenusStarFieldPalette)
/* The Jupiter star field's colours (btElementGlowInitStarField), and */
/* those of Death Plunge and the streaks. */
extern char ResourceId_JupiterStarFieldPalette;
#define RESOURCE_JUPITER_STAR_FIELD_PALETTE                                    \
	((s32)&ResourceId_JupiterStarFieldPalette)
/* Froth and Whirlwind, Bolt, Judgment, the Thor summon, the showers, the */
/* casting impact, the status motes and the monster strikes. */
extern char ResourceId_TornadoSheet;
#define RESOURCE_TORNADO_SHEET ((s32)&ResourceId_TornadoSheet)
/* Prism, Death Plunge and the Thor summon. */
extern char ResourceId_CrystalSheet;
#define RESOURCE_CRYSTAL_SHEET ((s32)&ResourceId_CrystalSheet)

/* Field, world map and menu resources. */

/* The world map's tile offsets and packed tiles, which */
/* fdWorldMapWriteLayerCellTile decodes one at a time as the view moves */
/* (perspective->tiles). */
extern char ResourceId_WorldMapPerspectiveTiles;
#define RESOURCE_WORLD_MAP_PERSPECTIVE_TILES                                   \
	((s32)&ResourceId_WorldMapPerspectiveTiles)
/* The world map cells fdWorldMapInitPerspective decodes into */
/* Ram_MapGridCellBuffer, and that the still images put back on their way */
/* out. */
extern char ResourceId_DefaultGridCells;
#define RESOURCE_DEFAULT_GRID_CELLS ((s32)&ResourceId_DefaultGridCells)
/* Colosso's first opponent's recorded path, played by the Azart match. */
extern char ResourceId_AzartOpponentPath;
#define RESOURCE_AZART_OPPONENT_PATH ((s32)&ResourceId_AzartOpponentPath)
/* The world map's tileset animations (fdTilesetAnimStartAll). */
extern char ResourceId_DefaultTileAnimation;
#define RESOURCE_DEFAULT_TILE_ANIMATION ((s32)&ResourceId_DefaultTileAnimation)
/* The attributes of the world map's cell tiles, loaded with its default cells.
 */
extern char ResourceId_DefaultCellTilesAttributes;
#define RESOURCE_DEFAULT_CELL_TILES_ATTRIBUTES                                 \
	((s32)&ResourceId_DefaultCellTilesAttributes)

/* The world map's two graphics sets (WorldMap_GraphicsResources), each a */
/* palette and five tilesets: fdWorldMapLoadGraphics decodes the first four */
/* into char blocks 2 and 3 and the fifth into MapGridAnimTiles for the */
/* tileset animation. The second set is for */
/* WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD. */

extern char ResourceId_WorldMapPaletteA; /* The first set's colours. */
#define RESOURCE_WORLD_MAP_PALETTE_A ((s32)&ResourceId_WorldMapPaletteA)
extern char ResourceId_WorldMapTilesA; /* First set, first tileset. */
#define RESOURCE_WORLD_MAP_TILES_A ((s32)&ResourceId_WorldMapTilesA)
extern char ResourceId_WorldMapTilesB; /* First set, second tileset. */
#define RESOURCE_WORLD_MAP_TILES_B ((s32)&ResourceId_WorldMapTilesB)
extern char ResourceId_WorldMapTilesC; /* First set, third tileset. */
#define RESOURCE_WORLD_MAP_TILES_C ((s32)&ResourceId_WorldMapTilesC)
extern char ResourceId_WorldMapTilesD; /* First set, fourth tileset. */
#define RESOURCE_WORLD_MAP_TILES_D ((s32)&ResourceId_WorldMapTilesD)
extern char ResourceId_WorldMapTilesE; /* First set, animation pictures. */
#define RESOURCE_WORLD_MAP_TILES_E ((s32)&ResourceId_WorldMapTilesE)
extern char ResourceId_WorldMapPaletteB; /* The snowfield set's colours. */
#define RESOURCE_WORLD_MAP_PALETTE_B ((s32)&ResourceId_WorldMapPaletteB)
extern char ResourceId_WorldMapTilesF; /* Snowfield set, first tileset. */
#define RESOURCE_WORLD_MAP_TILES_F ((s32)&ResourceId_WorldMapTilesF)
extern char ResourceId_WorldMapTilesG; /* Snowfield set, second tileset. */
#define RESOURCE_WORLD_MAP_TILES_G ((s32)&ResourceId_WorldMapTilesG)
extern char ResourceId_WorldMapTilesH; /* Snowfield set, third tileset. */
#define RESOURCE_WORLD_MAP_TILES_H ((s32)&ResourceId_WorldMapTilesH)
extern char ResourceId_WorldMapTilesI; /* Snowfield set, fourth tileset. */
#define RESOURCE_WORLD_MAP_TILES_I ((s32)&ResourceId_WorldMapTilesI)
extern char ResourceId_WorldMapTilesJ; /* Snowfield set, animation pictures. */
#define RESOURCE_WORLD_MAP_TILES_J ((s32)&ResourceId_WorldMapTilesJ)
/* The Satrage match's opponent path */
/* (scColosseumSatrageMatchStartOpponentPath). */
extern char ResourceId_SatrageOpponentPath;
#define RESOURCE_SATRAGE_OPPONENT_PATH ((s32)&ResourceId_SatrageOpponentPath)
extern char
	ResourceId_NavampaOpponentPath; /* The Navampa match's opponent path. */
#define RESOURCE_NAVAMPA_OPPONENT_PATH ((s32)&ResourceId_NavampaOpponentPath)
/* Colosso's lettering pictures, which scColossoLoadLettering decodes with */
/* their colours for sprite palette 15. */
extern char ResourceId_Lettering;
#define RESOURCE_LETTERING ((s32)&ResourceId_Lettering)
/* The first of eight 256-byte cursor pictures in consecutive rows; the option
 */
/* menu's slider marker adds the picture to it (mnSettingsMenuAnimateCursors).
 */
extern char ResourceId_MenuCursorPictures;
#define RESOURCE_MENU_CURSOR_PICTURES ((s32)&ResourceId_MenuCursorPictures)
/* The 32x32 icons and portraits, each with 16 colours before its tiles */
/* (mnIconLoadWithPalette). */
extern char ResourceId_Icons;
#define RESOURCE_ICONS ((s32)&ResourceId_Icons)
/* The command icons, a halfword offset each and then 1 KB of 256-colour */
/* pixels compressed; an icon's number is also its name's in MsgCommandName. */
extern char ResourceId_CommandIcons;
#define RESOURCE_COMMAND_ICONS ((s32)&ResourceId_CommandIcons)
/* The empty layout's header (map/empty_map), the first layout row: MapsWork */
/* counts its rows from here, and fdMapGridLoadMap adds them to it. */
extern char ResourceId_EmptyMap;
#define RESOURCE_EMPTY_MAP ((s32)&ResourceId_EmptyMap)

/* Battle effect pictures, continued. */

extern char ResourceId_SheepSheet; /* Sleep's sheep, among the status motes. */
#define RESOURCE_SHEEP_SHEET ((s32)&ResourceId_SheepSheet)
/* Poison Beat, Spinning Beat and Spider Web. */
extern char ResourceId_MagentaSwirlSheet;
#define RESOURCE_MAGENTA_SWIRL_SHEET ((s32)&ResourceId_MagentaSwirlSheet)
/* Poison Beat, Spinning Beat and Spider Web, beside MagentaSwirlSheet. */
extern char ResourceId_MagentaTailSheet;
#define RESOURCE_MAGENTA_TAIL_SHEET ((s32)&ResourceId_MagentaTailSheet)
/* Colours borrowed by Atalanta, the rush bursts, the casting impact, the */
/* lobbed shells, Spider Web, the streaks, the status motes and the strikes; */
/* Lucky Wheels steps toward it. */
extern char ResourceId_LimePalette;
#define RESOURCE_LIME_PALETTE ((s32)&ResourceId_LimePalette)

/* Annihilation, Smoke Bomb, Blast and the casting impact. */
extern char ResourceId_FireStreakSheet;
#define RESOURCE_FIRE_STREAK_SHEET ((s32)&ResourceId_FireStreakSheet)
/* Gust, Flying Attack, the beams, the casting impact and the */
/* rushing strikes. */
extern char ResourceId_YellowOrbSheet;
#define RESOURCE_YELLOW_ORB_SHEET ((s32)&ResourceId_YellowOrbSheet)
extern char ResourceId_BlueBurstSheet; /* The rushing strikes. */
#define RESOURCE_BLUE_BURST_SHEET ((s32)&ResourceId_BlueBurstSheet)
/* Dragon Cloud and the rushing strikes. */
extern char ResourceId_FirePillarSheetB;
#define RESOURCE_FIRE_PILLAR_SHEET_B ((s32)&ResourceId_FirePillarSheetB)
/* The orbiting skulls, Planet Diver and the casting impact. */
extern char ResourceId_SkullSheet;
#define RESOURCE_SKULL_SHEET ((s32)&ResourceId_SkullSheet)
/* Bolt, Plasma, Ray, Meteor, Tiamat, Titan Blade, the casting impact and the */
/* monster strikes. */
extern char ResourceId_LightningBoltSheet;
#define RESOURCE_LIGHTNING_BOLT_SHEET ((s32)&ResourceId_LightningBoltSheet)
/* Bind, Astral Blast, Shining Star and the casting impact. */
extern char ResourceId_RuneSheet;
#define RESOURCE_RUNE_SHEET ((s32)&ResourceId_RuneSheet)
extern char ResourceId_RockSpireSheet; /* The casting impact. */
#define RESOURCE_ROCK_SPIRE_SHEET ((s32)&ResourceId_RockSpireSheet)
extern char ResourceId_FlameSheetA; /* The casting impact's flames. */
#define RESOURCE_FLAME_SHEET_A ((s32)&ResourceId_FlameSheetA)
/* The small Venus Djinni of the casting impact, the Djinn attacks, Ground and
 */
/* the volleys' prelude. */
extern char ResourceId_VenusDjinnSmallSheet;
#define RESOURCE_VENUS_DJINN_SMALL_SHEET ((s32)&ResourceId_VenusDjinnSmallSheet)
/* The small Mars Djinni of the beams, the casting impact, the Djinn */
/* attacks, the healing motes and the volleys' prelude. */
extern char ResourceId_MarsDjinnSmallSheet;
#define RESOURCE_MARS_DJINN_SMALL_SHEET ((s32)&ResourceId_MarsDjinnSmallSheet)
/* The small Jupiter Djinni of the casting impact, the Djinn attacks and the */
/* volleys' prelude. */
extern char ResourceId_JupiterDjinnSmallSheet;
#define RESOURCE_JUPITER_DJINN_SMALL_SHEET                                     \
	((s32)&ResourceId_JupiterDjinnSmallSheet)
extern char ResourceId_BlueBeastSheet; /* The casting impact. */
#define RESOURCE_BLUE_BEAST_SHEET ((s32)&ResourceId_BlueBeastSheet)
extern char ResourceId_RedCrescentSheetB; /* The casting impact. */
#define RESOURCE_RED_CRESCENT_SHEET_B ((s32)&ResourceId_RedCrescentSheetB)
extern char ResourceId_TanPalette; /* Colours for the casting impact. */
#define RESOURCE_TAN_PALETTE ((s32)&ResourceId_TanPalette)
/* Colours for the casting impact, the streaks and the motes. */
extern char ResourceId_CyanPalette;
#define RESOURCE_CYAN_PALETTE ((s32)&ResourceId_CyanPalette)
/* The title intro's sprite colours and compressed sprite tiles */
/* (mnTitleLoadIntroBackgrounds). */
extern char ResourceId_IntroSun;
#define RESOURCE_INTRO_SUN ((s32)&ResourceId_IntroSun)
/* The 256-colour picture mnTitleShowTitleScr puts on BG2 once the intro's */
/* sprites are gone. */
extern char ResourceId_TitleScreen;
#define RESOURCE_TITLE_SCREEN ((s32)&ResourceId_TitleScreen)
/* The title intro's scrolling backgrounds: their colours, then their */
/* compressed tiles (mnTitleLoadIntroBackgrounds). */
extern char ResourceId_IntroPeak;
#define RESOURCE_INTRO_PEAK ((s32)&ResourceId_IntroPeak)
extern char ResourceId_CyanSparkSheet; /* Death Plunge. */
#define RESOURCE_CYAN_SPARK_SHEET ((s32)&ResourceId_CyanSparkSheet)
extern char ResourceId_StarBurstSheet; /* The beams. */
#define RESOURCE_STAR_BURST_SHEET ((s32)&ResourceId_StarBurstSheet)
extern char ResourceId_CrescentSheet; /* The beams. */
#define RESOURCE_CRESCENT_SHEET ((s32)&ResourceId_CrescentSheet)
extern char ResourceId_SwordSheet; /* Titan Blade. */
#define RESOURCE_SWORD_SHEET ((s32)&ResourceId_SwordSheet)
extern char ResourceId_PlasmaVioletPalette; /* Colours for Plasma. */
#define RESOURCE_PLASMA_VIOLET_PALETTE ((s32)&ResourceId_PlasmaVioletPalette)
extern char ResourceId_FireSwirlSheet; /* Helm Splitter. */
#define RESOURCE_FIRE_SWIRL_SHEET ((s32)&ResourceId_FireSwirlSheet)
extern char ResourceId_FireBurstSheet; /* Annihilation. */
#define RESOURCE_FIRE_BURST_SHEET ((s32)&ResourceId_FireBurstSheet)
/* Heat Wave's fireballs, and the walls of battle_effect_mirrored_wall.c. */
extern char ResourceId_FireballSheet;
#define RESOURCE_FIREBALL_SHEET ((s32)&ResourceId_FireballSheet)
/* The three lights of Demon Night, Rumble and Haunting. */
extern char ResourceId_GatheringLightsImage;
#define RESOURCE_GATHERING_LIGHTS_IMAGE ((s32)&ResourceId_GatheringLightsImage)
/* The beams' waves, and the rush bursts. */
extern char ResourceId_WaveSheet;
#define RESOURCE_WAVE_SHEET ((s32)&ResourceId_WaveSheet)
extern char ResourceId_PinkStarSheet; /* Helm Splitter. */
#define RESOURCE_PINK_STAR_SHEET ((s32)&ResourceId_PinkStarSheet)
/* Gaia, the walls, Revive and its family (battle_effect_djinn_beams.c) and the
 */
/* streaks. */
extern char ResourceId_EarthWallSheet;
#define RESOURCE_EARTH_WALL_SHEET ((s32)&ResourceId_EarthWallSheet)
/* Volcano, Titan Blade and the Nereid summon. */
extern char ResourceId_FirePillarSheetA;
#define RESOURCE_FIRE_PILLAR_SHEET_A ((s32)&ResourceId_FirePillarSheetA)
extern char
	ResourceId_VolcanoRedPalette; /* Colours for Volcano and the streaks. */
#define RESOURCE_VOLCANO_RED_PALETTE ((s32)&ResourceId_VolcanoRedPalette)
extern char
	ResourceId_VolcanoOrangePalette; /* Colours for Plasma and Volcano. */
#define RESOURCE_VOLCANO_ORANGE_PALETTE ((s32)&ResourceId_VolcanoOrangePalette)
extern char ResourceId_FlameballSheet; /* Planet Diver and the Djinn attacks. */
#define RESOURCE_FLAMEBALL_SHEET ((s32)&ResourceId_FlameballSheet)
extern char ResourceId_BoulderSheet; /* Spire. */
#define RESOURCE_BOULDER_SHEET ((s32)&ResourceId_BoulderSheet)
extern char ResourceId_RockWallSheet; /* Quake. */
#define RESOURCE_ROCK_WALL_SHEET ((s32)&ResourceId_RockWallSheet)
/* The large Jupiter Djinni of Bind and the stat changes; Lucky Wheels */
/* takes its colours. */
extern char ResourceId_JupiterDjinnSheet;
#define RESOURCE_JUPITER_DJINN_SHEET ((s32)&ResourceId_JupiterDjinnSheet)
extern char ResourceId_ShieldSheet; /* The stat changes. */
#define RESOURCE_SHIELD_SHEET ((s32)&ResourceId_ShieldSheet)
extern char ResourceId_DaggerSheet; /* The stat changes. */
#define RESOURCE_DAGGER_SHEET ((s32)&ResourceId_DaggerSheet)
/* The shards a stat reduction's dagger or shield breaks into */
/* (STAT_REDUCTION_SHARD_PICTURES). */
extern char ResourceId_StatReductionShardSheet;
#define RESOURCE_STAT_REDUCTION_SHARD_SHEET                                    \
	((s32)&ResourceId_StatReductionShardSheet)
extern char ResourceId_HeartSheet; /* Heartrender, among the status motes. */
#define RESOURCE_HEART_SHEET ((s32)&ResourceId_HeartSheet)
/* Revive and its family (battle_effect_djinn_beams.c). */
extern char ResourceId_CrescentMoonSheet;
#define RESOURCE_CRESCENT_MOON_SHEET ((s32)&ResourceId_CrescentMoonSheet)
/* The beams and the rush bursts. */
extern char ResourceId_BlueArcSheetA;
#define RESOURCE_BLUE_ARC_SHEET_A ((s32)&ResourceId_BlueArcSheetA)
/* The beams and the rush bursts, beside BlueArcSheetA. */
extern char ResourceId_BlueArcSheetB;
#define RESOURCE_BLUE_ARC_SHEET_B ((s32)&ResourceId_BlueArcSheetB)
/* The Djinn attacks, the stat changes and the status motes. */
extern char ResourceId_GlowOrbSheet;
#define RESOURCE_GLOW_ORB_SHEET ((s32)&ResourceId_GlowOrbSheet)
/* The lobbed shells' images, whose colours are Mad Spatter's. */
extern char ResourceId_GoldShellSheet;
#define RESOURCE_GOLD_SHELL_SHEET ((s32)&ResourceId_GoldShellSheet)
/* The beams' BEAM_IMAGE, and the picture of a monster's slash. */
extern char ResourceId_BeamImage;
#define RESOURCE_BEAM_IMAGE ((s32)&ResourceId_BeamImage)
/* Ice and Douse, and the monster strikes. */
extern char ResourceId_BlueBeamSheet;
#define RESOURCE_BLUE_BEAM_SHEET ((s32)&ResourceId_BlueBeamSheet)
/* Froth and Whirlwind, and the status motes. */
extern char ResourceId_VortexSheet;
#define RESOURCE_VORTEX_SHEET ((s32)&ResourceId_VortexSheet)
extern char ResourceId_LightningPillarSheet; /* Plasma and the Thor summon. */
#define RESOURCE_LIGHTNING_PILLAR_SHEET ((s32)&ResourceId_LightningPillarSheet)
/* The shattering opening (btEffectRunShatteringOpening). */
extern char ResourceId_ShatteringOpeningSheet;
#define RESOURCE_SHATTERING_OPENING_SHEET                                      \
	((s32)&ResourceId_ShatteringOpeningSheet)
extern char ResourceId_DeathPlungeVioletPalette; /* Colours for Death Plunge. */
#define RESOURCE_DEATH_PLUNGE_VIOLET_PALETTE                                   \
	((s32)&ResourceId_DeathPlungeVioletPalette)
extern char ResourceId_DeathPlungeRedPalette; /* Colours for Death Plunge. */
#define RESOURCE_DEATH_PLUNGE_RED_PALETTE                                      \
	((s32)&ResourceId_DeathPlungeRedPalette)
extern char ResourceId_DeathPlungeYellowPalette; /* Colours for Death Plunge. */
#define RESOURCE_DEATH_PLUNGE_YELLOW_PALETTE                                   \
	((s32)&ResourceId_DeathPlungeYellowPalette)
extern char ResourceId_FlameBladeSheet; /* Ragnarok. */
#define RESOURCE_FLAME_BLADE_SHEET ((s32)&ResourceId_FlameBladeSheet)
extern char ResourceId_IceTileSheet; /* Death Plunge. */
#define RESOURCE_ICE_TILE_SHEET ((s32)&ResourceId_IceTileSheet)
/* The streak volleys' images: three colour rows of six 20x40 images. */
extern char ResourceId_FlameArcSheet;
#define RESOURCE_FLAME_ARC_SHEET ((s32)&ResourceId_FlameArcSheet)
extern char ResourceId_SpiderWebSheet; /* Spider Web and the lobbed shells. */
#define RESOURCE_SPIDER_WEB_SHEET ((s32)&ResourceId_SpiderWebSheet)
extern char ResourceId_FlareSheet; /* Helm Splitter. */
#define RESOURCE_FLARE_SHEET ((s32)&ResourceId_FlareSheet)
extern char ResourceId_FlareImage; /* Helm Splitter, beside FlareSheet. */
#define RESOURCE_FLARE_IMAGE ((s32)&ResourceId_FlareImage)
extern char ResourceId_VioletLightningSheetB; /* The Thor summon. */
#define RESOURCE_VIOLET_LIGHTNING_SHEET_B                                      \
	((s32)&ResourceId_VioletLightningSheetB)
extern char ResourceId_BlueCrescentSheet; /* Judgment. */
#define RESOURCE_BLUE_CRESCENT_SHEET ((s32)&ResourceId_BlueCrescentSheet)
/* Judgment's small flash (JUDGMENT_SHEET_FLASH_SMALL). */
extern char ResourceId_JudgmentFlashSheet;
#define RESOURCE_JUDGMENT_FLASH_SHEET ((s32)&ResourceId_JudgmentFlashSheet)
extern char ResourceId_VioletMoteSheet; /* The Thor summon. */
#define RESOURCE_VIOLET_MOTE_SHEET ((s32)&ResourceId_VioletMoteSheet)
/* Judgment's burst (JUDGMENT_SHEET_BURST). */
extern char ResourceId_JudgmentBurstSheet;
#define RESOURCE_JUDGMENT_BURST_SHEET ((s32)&ResourceId_JudgmentBurstSheet)
extern char ResourceId_SlashSheet; /* The Atalanta summon. */
#define RESOURCE_SLASH_SHEET ((s32)&ResourceId_SlashSheet)
/* Judgment's explosion (JUDGMENT_SHEET_EXPLOSION). */
extern char ResourceId_ExplosionSheet;
#define RESOURCE_EXPLOSION_SHEET ((s32)&ResourceId_ExplosionSheet)
extern char ResourceId_PaleBlueGlowSheet; /* The Thor summon. */
#define RESOURCE_PALE_BLUE_GLOW_SHEET ((s32)&ResourceId_PaleBlueGlowSheet)
/* The Thor summon, beside PaleBlueGlowSheet. */
extern char ResourceId_BlueTallGlowSheet;
#define RESOURCE_BLUE_TALL_GLOW_SHEET ((s32)&ResourceId_BlueTallGlowSheet)
/* Rotten Blood's green, which Decompose shares. */
extern char ResourceId_GreenPalette;
#define RESOURCE_GREEN_PALETTE ((s32)&ResourceId_GreenPalette)
extern char ResourceId_SwordSlashSheet; /* Death Plunge. */
#define RESOURCE_SWORD_SLASH_SHEET ((s32)&ResourceId_SwordSlashSheet)
extern char
	ResourceId_GaiaOrangePalette; /* Colours for Gaia and the streaks. */
#define RESOURCE_GAIA_ORANGE_PALETTE ((s32)&ResourceId_GaiaOrangePalette)
extern char ResourceId_WhirlwindSheet; /* The Procne summon. */
#define RESOURCE_WHIRLWIND_SHEET ((s32)&ResourceId_WhirlwindSheet)
/* The Procne summon, after its sky is drawn. */
extern char ResourceId_VioletCometSheet;
#define RESOURCE_VIOLET_COMET_SHEET ((s32)&ResourceId_VioletCometSheet)
/* The large Mercury Djinni, whose colours Revive's family, Haunting's */
/* lights and the healing motes borrow; Lucky Wheels steps toward it. */
extern char ResourceId_MercuryDjinnSheet;
#define RESOURCE_MERCURY_DJINN_SHEET ((s32)&ResourceId_MercuryDjinnSheet)
/* The large Venus Djinni, whose colours Revive's family and the stat changes */
/* borrow; Lucky Wheels steps toward it. */
extern char ResourceId_VenusDjinnSheet;
#define RESOURCE_VENUS_DJINN_SHEET ((s32)&ResourceId_VenusDjinnSheet)
extern char ResourceId_FireBlobSheet; /* The Kirin summon. */
#define RESOURCE_FIRE_BLOB_SHEET ((s32)&ResourceId_FireBlobSheet)
extern char ResourceId_YellowRingSheet; /* The Cybele summon. */
#define RESOURCE_YELLOW_RING_SHEET ((s32)&ResourceId_YellowRingSheet)
extern char ResourceId_BluePalette; /* Colours for the lobbed shells. */
#define RESOURCE_BLUE_PALETTE ((s32)&ResourceId_BluePalette)
extern char ResourceId_WispSheet; /* The stat changes. */
#define RESOURCE_WISP_SHEET ((s32)&ResourceId_WispSheet)
extern char ResourceId_PortalSheet; /* Hail and Ground. */
#define RESOURCE_PORTAL_SHEET ((s32)&ResourceId_PortalSheet)
extern char ResourceId_VioletStreakSheet; /* The stat changes. */
#define RESOURCE_VIOLET_STREAK_SHEET ((s32)&ResourceId_VioletStreakSheet)
extern char ResourceId_ThornVineSheet; /* The Cybele summon. */
#define RESOURCE_THORN_VINE_SHEET ((s32)&ResourceId_ThornVineSheet)
extern char
	ResourceId_IceShardSheet; /* Ice and Douse, and the healing motes. */
#define RESOURCE_ICE_SHARD_SHEET ((s32)&ResourceId_IceShardSheet)
extern char ResourceId_LightFanSheet; /* The Thor summon. */
#define RESOURCE_LIGHT_FAN_SHEET ((s32)&ResourceId_LightFanSheet)
extern char ResourceId_FlameSheetB; /* Meteor and Outer Space. */
#define RESOURCE_FLAME_SHEET_B ((s32)&ResourceId_FlameSheetB)
/* The monster strikes' picture for every kind but a slash. */
extern char ResourceId_BlueDropletSheet;
#define RESOURCE_BLUE_DROPLET_SHEET ((s32)&ResourceId_BlueDropletSheet)
/* The star field's rings (btElementGlowInitStarField), with the Mercury */
/* field's colours. */
extern char ResourceId_BlueRingSheet;
#define RESOURCE_BLUE_RING_SHEET ((s32)&ResourceId_BlueRingSheet)
/* The Mars star field's colours (btElementGlowInitStarField). */
extern char ResourceId_MarsStarFieldPalette;
#define RESOURCE_MARS_STAR_FIELD_PALETTE ((s32)&ResourceId_MarsStarFieldPalette)
/* The path Atalanta's particles follow. */
extern char ResourceId_AtalantaPath;
#define RESOURCE_ATALANTA_PATH ((s32)&ResourceId_AtalantaPath)
extern char
	ResourceId_DragonCloudFlamePath; /* The path of Dragon Cloud's flame. */
#define RESOURCE_DRAGON_CLOUD_FLAME_PATH ((s32)&ResourceId_DragonCloudFlamePath)
#endif
