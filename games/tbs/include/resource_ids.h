#ifndef ALCHEMY_RESOURCE_IDS_H
#define ALCHEMY_RESOURCE_IDS_H

/// @file
/// Resource numbers, named where their contents are known.
///
/// Each ResourceId_ is an absolute symbol whose value is the resource's row
/// in Resource_DirectoryTable (system/resource/directory.s); C takes the
/// number as (s32)&ResourceId_Name.

/// Twelve ASCII digits, yymmddhhmmss, and a newline, different in each
/// edition. System_CopyBuildStamp copies it into gBuildStamp, and
/// BuildStamp_GetTime turns its time into the number saves compare.
extern char ResourceId_BuildStamp;
/// The row of FarCall_TitleTable. The title screen hands it to
/// Stub_NoOpHook, which ignores it.
extern char ResourceId_TitleFarCalls;
/// The row of FarCall_BattleAnimTable. Lucky Dice and Lucky Wheels hand it to
/// Stub_NoOpHook as they start.
extern char ResourceId_BattleAnimFarCalls;
/// The row of FarCall_ResourceTable, which the sprite viewer passes to
/// Stub_ReturnZero when Select is pressed with B held.
extern char ResourceId_ResourceFarCalls;
/// A second row that also holds FarCall_ResourceTable; the sprite viewer
/// passes it when Select is pressed without B.
extern char ResourceId_ResourceFarCallsAlternate;
/// The window font and frame tiles, 256 tiles of 4 bits a pixel.
/// Window_LoadGraphics copies them into BG character block 0, and the
/// window text routines draw their glyphs from here.
extern char ResourceId_WindowTiles;
/// The narrow glyphs the Japanese edition draws in the battle message
/// window; the other editions look it up there and then draw from
/// WindowTiles.
extern char ResourceId_SmallFont;
extern char ResourceId_NintendoLogo; ///< Shown first by Title_ShowNintendoLogo.
/// The Camelot logo's four frames of tiles, which Title_ShowCamelotLogo
/// cycles.
extern char ResourceId_CamelotLogo;
/// The title picture on BG2, loaded by Title_LoadBackground and, for the main
/// menu, by MainMenu_LoadBackground.
extern char ResourceId_GoldenSunLogo;
/// The still world map RegionMap_Show shows for a
/// FIELD_RAISED_EVENT_REGION_MAP event; it loads this one whatever picture
/// it's passed.
extern char ResourceId_WorldMapPicture;
/// The lettering sprites Title_LoadSprites loads: 40 tiles of 16 colours in
/// English and Japanese, a whole 224-colour screen in the other editions, of
/// which it keeps the bottom 32 rows.
extern char ResourceId_IntroTilesB;

// Battle backgrounds. BattleBackground_SelectForMap picks one for the current
// map from gBattleBackground_Maps (overworld/map_rules.s), and on the world
// map BattleBackground_SelectForEnvironment picks one by battle environment;
// either leaves it in gSaveBlock.special for the battle. The credits
// (gCredits_Backgrounds) shows every one but DesertBackdrop and
// AerialViewBackdrop, and some summons draw one as their sky.

/// Suhalla Gate's checkpoint and Dew rooms. The debug battle menu fights on it
/// too.
extern char ResourceId_SuhallaGateBackdrop;
extern char ResourceId_PirateShipBackdrop;        ///< MapId_PirateShip.
extern char ResourceId_CrossboneIsleCaveBackdrop; ///< Crossbone Isle's floors.
/// Colosso's three matches (Azart, Satrage and Navampa) and the Battle Arena.
extern char ResourceId_ColossoBackdrop;
/// MapId_VenusLighthouseAerie.
extern char ResourceId_VenusLighthouseAerieBackdrop;
/// Venus Lighthouse's floors, from the ground floor to the fourth.
extern char ResourceId_VenusLighthouseBackdrop;
/// Suhalla Desert, north and south.
extern char ResourceId_SuhallaDesertBackdrop;
/// Altmiller Cave and Lunpa Fortress.
extern char ResourceId_AltmillerCaveBackdrop;
extern char ResourceId_TunnelRuinsBackdrop; ///< Tunnel Ruins' two statue rooms.
/// Babi Lighthouse, and MapId_CrossboneIsleBane on Crossbone Isle.
extern char ResourceId_BabiLighthouseBackdrop;
/// Lamakan Desert's three maps.
extern char ResourceId_LamakanDesertBackdrop;
extern char ResourceId_MogallForestBackdrop; ///< Mogall Forest's three maps.
/// MapId_MercuryLighthouseAerie.
extern char ResourceId_MercuryLighthouseAerieBackdrop;
/// MapId_TolbiShipInteriors.
extern char ResourceId_TolbiShipInteriorsBackdrop;
/// Fuchin Falls Cave, and MapId_CrossboneIsleFloatingLogs on Crossbone
/// Isle.
extern char ResourceId_ImilFuchinFallsBackdrop;
extern char ResourceId_KolimaForestBackdrop; ///< Kolima Forest's five maps.
/// MapId_TretTreeInteriors.
extern char ResourceId_TretTreeInteriorsBackdrop;
/// Goma Cave, Kolima Tunnel, Bilibin Cave and Gateway Cave, and any map the
/// table doesn't list, since it comes last.
extern char ResourceId_GomaCaveBackdrop;
/// The vault interiors and Lalivero's maps, Faran's house included.
extern char ResourceId_InteriorBackdrop;
/// Altin Peak's maps but the Hydros statue.
extern char ResourceId_AltinPeakBackdrop;
/// MapId_TolbiShipDeck and MapId_TolbiShipMast.
extern char ResourceId_TolbiShipDeckBackdrop;
/// Mercury Lighthouse's floors below the aerie and its exterior.
extern char ResourceId_MercuryLighthouseBackdrop;
/// Vale's maps, the stormy night included; the credits open on it.
extern char ResourceId_ValeStormBackdrop;
extern char ResourceId_TretTreeBranchBackdrop; ///< MapId_TretTreeBranch.
/// Set by the first slot of ENCOUNTER_ZONE_SCRIPTED
/// (Field_RequestBattleFromEncounterTable), and by the storm opening of
/// BattleTransition_Run, where the enemies alone take part.
extern char ResourceId_VenusLighthouseAerieStormBackdrop;
/// World map beaches and shallows; Judgment draws it too.
extern char ResourceId_BeachBackdrop;
/// World map snowfields and snowy forests.
extern char ResourceId_SnowfieldBackdrop;
extern char ResourceId_GrasslandBackdrop; ///< World map grassland.
/// Sol Sanctum's rooms, MapId_AltinPeakHydrosStatue and
/// MapId_CrossboneIsleSteppingStones, and any battle environment without a
/// battle background of its own.
extern char ResourceId_SolSanctumBackdrop;
/// World map forests; the Judgment and Kirin summons draw it too.
extern char ResourceId_ForestBackdrop;
/// No battle is fought on it: the Atalanta, Boreas, Judgment and Procne
/// summons draw it as their sky, and the credits close on it.
extern char ResourceId_VioletSkyBackdrop;
/// World map desert, and the sky of the Ramses summon.
extern char ResourceId_DesertBackdrop;
/// The sky of the Neptune and Thor summons.
extern char ResourceId_DuskCloudsBackdrop;
extern char ResourceId_AerialViewBackdrop; ///< Drawn only by Judgment.

/// Lucky Wheels' frame (LuckyWheels_Run): 16 colours for BG palette 10,
/// then the picture.
extern char ResourceId_LuckyWheelsFramePicture;
/// The wheels' symbol sprites: 480 bytes of sprite colours, then the
/// compressed tiles.
extern char ResourceId_LuckyWheelsIcons;
/// The jackpot sprites, with their colours for sprite palette 15.
extern char ResourceId_LuckyWheelsJackpot;
/// The Lucky Dice board, with its colours, on the background (LuckyDice_Run).
extern char ResourceId_LuckyDiceBoard;
/// The Lucky Dice sprites and their colours.
extern char ResourceId_LuckyDiceSprites;

// The flashing strikes' pictures (battle_anim/parts/thrash.c). Each holds two
// pictures; the first of a pair fills work->imageStorage, the second the start
// of Ram_MapGridCellBuffer.

extern char ResourceId_RevealMaskA; ///< Vorpal Slash, first of its pair.
extern char ResourceId_RevealMaskB; ///< Vorpal Slash, second of its pair.
extern char ResourceId_RevealMaskC; ///< Quick Strike, first of its pair.
extern char ResourceId_RevealMaskD; ///< Quick Strike, second of its pair.
/// First of the pair for FLASHING_STRIKE_UNUSED, which nothing passes.
extern char ResourceId_RevealMaskE;
/// Second of the pair for FLASHING_STRIKE_UNUSED.
extern char ResourceId_RevealMaskF;

// Battle animation pictures. A sheet holds 64 colours (128 bytes) and then its
// compressed pictures, which the animation parts load with
// Resource_LoadAndDecompress; a palette holds colours alone, which a part
// copies over a shared sheet's to recolour it, and some parts borrow a sheet's
// colours the same way. Each note names the animations that read it.

extern char ResourceId_LimeColumnSheet; ///< Undead Sword.
extern char ResourceId_DemonFaceSheet;  ///< The blessings' burst showers.
/// The blessings' burst showers, beside DemonFaceSheet.
extern char ResourceId_VioletLightningSheetA;
/// The yellow sparks of Wing Flutter, Wing Beat and Wing Stroke.
extern char ResourceId_YellowSparkSheet;
/// Crazy Voice's bouncing pellets, drawn in CometSheetA's colours.
extern char ResourceId_ProjectileVolleyImage;
/// The Shuriken volley's spinning shurikens.
extern char ResourceId_ShurikenSheet;
extern char ResourceId_TargetBurstImage; ///< The target bursts of orbiting.c.
/// Hail, the Neptune summon and the blessings' burst showers; Nereid takes
/// its colours.
extern char ResourceId_WaterSpraySheet;
/// The casting impact, the Djinn attacks and the Neptune summon.
extern char ResourceId_BlueFlameSheet;
/// The particles nearly every animation loads, from Annihilation to Volcano and
/// most summons.
extern char ResourceId_ParticleSpritesA;
/// A second shared particle sheet: the beam sequences, Bind, Death Plunge,
/// Ice and Douse, the Neptune summon, the stat changes, and Lucky Wheels.
extern char ResourceId_ParticleSpritesD;
/// Curse's flames (BattleAnim_RunCurse).
extern char ResourceId_FlameColumnSheet;
/// Shared by the blessings' showers, the casting impact, Death Plunge, Dragon
/// Cloud, Planet Diver, Plasma, Ragnarok, Tiamat and Titan Blade.
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_ThornSheet; ///< Thorn.
/// The comet of the impact's comets (PROJECTILE_VOLLEY_IMPACT_COMETS), and
/// the colours of Crazy Voice's pellets.
extern char ResourceId_CometSheetA;
/// One of Slash's comets, which the animation's variant picks.
extern char ResourceId_CometSheetB;
/// Cutting Edge's large comet, and one of Slash's.
extern char ResourceId_CometSheetC;
/// GROWING_COLUMNS_PILLARS, in BattleAnim_RunGrowingColumns.
extern char ResourceId_GreenPillarSheet;
extern char
    ResourceId_VineSheet; ///< BattleAnim_RunGrowingColumns' other modes.
extern char ResourceId_StarDotSheet; ///< The swirling stars of orbiting.c.
/// Borrowed mostly for its colours: by the showers, the casting impact,
/// Rumble's lights, the healing motes, the lobbed shells, the walls, the
/// stat changes, the status motes and the monster strikes.
extern char ResourceId_MarsDjinnSheet;
/// The small Mercury Djinni of the casting impact, the Djinn attacks, Djinn
/// healing, Hail and the ice volleys' prelude.
extern char ResourceId_MercuryDjinnSmallSheet;
/// The casting impact, the lobbed shells, the orbiting bursts, the status
/// motes and the monster strikes.
extern char ResourceId_SmokeSheet;
/// Colours for one style of the blessings' showers and of the monster
/// strikes.
extern char ResourceId_PinkPalette;
extern char ResourceId_GreenVineSheet;     ///< Vine.
extern char ResourceId_CounterRevealSheet; ///< Condemn and the casting impact.
/// Condemn and the casting impact, beside CounterRevealSheet.
extern char ResourceId_RedCrescentSheetA;
extern char ResourceId_SpiralSheet; ///< The orbiting spirals of planet_diver.c.
/// The fire family: Fire, Flare, Atalanta, Dragon Cloud, Tiamat, the casting
/// impact, the Djinn attacks, Death Plunge, the streaks and the strikes;
/// Lucky Wheels steps its colours toward it.
extern char ResourceId_EmberStreakSheet;
/// Frost, Hail, the ice volleys and the casting impact.
extern char ResourceId_IceChipSheet;
/// The Boreas summon; the showers, the casting impact, Break and Drain, the
/// healing motes, Cutting Edge and the rushing strikes take its colours.
extern char ResourceId_IceBlockSheet;
/// Boreas, Djinn healing, Ice and Douse, Frost and the healing motes.
extern char ResourceId_SparkleDots;
/// Judgment's gathering, and the colours of Demon Night, the orbiting bursts,
/// Planet Diver, the rushing strikes, the stat changes and the status motes.
extern char ResourceId_PinkBurstSheet;
/// Colours of the element streaks for ELEMENT_NONE.
extern char ResourceId_DustPuffSheet;
/// Smoke Bomb and Blast, Cybele, Meteor and Outer Space, Ragnarok, Ramses,
/// Titan Blade, Undead Sword, the casting impact and the status motes; Break
/// and Drain, the healing motes and the rushing strikes take its colours.
extern char ResourceId_BlastSheet;
/// The streaks behind the comets of Slash, Cutting Edge and the impact.
extern char ResourceId_WindStreakSheet;
/// The Venus star field's colours (ElementGlow_InitializeStarField), and
/// Death Plunge's.
extern char ResourceId_YellowPaletteB;
/// The Jupiter star field's colours (ElementGlow_InitializeStarField), and
/// those of Death Plunge and the streaks.
extern char ResourceId_VioletPaletteD;
/// Froth and Whirlwind, Bolt, Judgment, the Thor summon, the showers, the
/// casting impact, the status motes and the monster strikes.
extern char ResourceId_TornadoSheet;
/// Prism, Death Plunge and the Thor summon.
extern char ResourceId_CrystalSheet;

// Field, world map and menu resources.

/// The world map's tile offsets and packed tiles, which
/// WorldMap_WriteLayerCellTile decodes one at a time as the view moves
/// (perspective->tiles).
extern char ResourceId_PerspectiveDataA;
/// The world map cells WorldMap_InitializePerspective decodes into
/// Ram_MapGridCellBuffer, and that the still views put back on their way out.
extern char ResourceId_DefaultGridCells;
/// Colosso's first opponent's recorded path, played by the Azart match.
extern char ResourceId_OpponentPathA;
/// The world map's tileset animations (TilesetAnim_StartAll).
extern char ResourceId_DefaultTileAnimation;
/// The world map's metatile attributes, loaded with its default cells.
extern char ResourceId_DefaultMetatileAttributes;

// The world map's two graphics sets (gWorldMap_GraphicsResources), each a
// palette and five tilesets: WorldMap_LoadGraphics decodes the first four
// into char blocks 2 and 3 and the fifth into gMapGridAnimTiles for the
// tileset animation. The second set is for
// WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD.

extern char ResourceId_WorldMapPaletteA; ///< The first set's colours.
extern char ResourceId_WorldMapTilesA;   ///< First set, first tileset.
extern char ResourceId_WorldMapTilesB;   ///< First set, second tileset.
extern char ResourceId_WorldMapTilesC;   ///< First set, third tileset.
extern char ResourceId_WorldMapTilesD;   ///< First set, fourth tileset.
extern char ResourceId_WorldMapTilesE;   ///< First set, animation frames.
extern char ResourceId_WorldMapPaletteB; ///< The snowfield set's colours.
extern char ResourceId_WorldMapTilesF;   ///< Snowfield set, first tileset.
extern char ResourceId_WorldMapTilesG;   ///< Snowfield set, second tileset.
extern char ResourceId_WorldMapTilesH;   ///< Snowfield set, third tileset.
extern char ResourceId_WorldMapTilesI;   ///< Snowfield set, fourth tileset.
extern char ResourceId_WorldMapTilesJ;   ///< Snowfield set, animation frames.
/// The Satrage match's opponent path (ColosseumSatrageMatch_StartOpponentPath).
extern char ResourceId_OpponentPathB;
extern char ResourceId_OpponentPathC; ///< The Navampa match's opponent path.
/// Colosso's lettering pictures, which Colosso_LoadLettering decodes with
/// their colours for sprite palette 15.
extern char ResourceId_Lettering;
/// The first of eight 256-byte cursor frames in consecutive rows; the option
/// menu's slider marker adds the frame to it (OptionMenu_AnimateCursors).
extern char ResourceId_MenuCursorFrames;
/// The 32x32 icons and portraits, each with 16 colours before its tiles
/// (Icon_LoadWithPalette).
extern char ResourceId_Icons;
/// The command icons, a halfword offset each and then 1 KB of 256-colour
/// pixels compressed; an icon's number is also its name's in MsgCommandName.
extern char ResourceId_CommandIcons;
/// The first layout row, layout 1's header (layout/layout_001), whose number
/// it is named for: gLayouts counts its rows from here, and
/// MapGrid_LoadLayout adds them to it.
extern char ResourceId_Layout001;

// Battle animation pictures, continued.

extern char ResourceId_SheepSheet; ///< Sleep's sheep, among the status motes.
/// Poison Beat, Spinning Beat and Spider Web.
extern char ResourceId_MagentaSwirlSheet;
/// Poison Beat, Spinning Beat and Spider Web, beside MagentaSwirlSheet.
extern char ResourceId_MagentaTailSheet;
/// Colours borrowed by Atalanta, the rush bursts, the casting impact, the
/// lobbed shells, Spider Web, the streaks, the status motes and the strikes;
/// Lucky Wheels steps toward it.
extern char ResourceId_LimePalette;

/// Annihilation, Smoke Bomb, Blast and the casting impact.
extern char ResourceId_FireStreakSheet;
/// Gust, Flying Attack, the beam sequences, the casting impact and the
/// rushing strikes.
extern char ResourceId_YellowOrbSheet;
extern char ResourceId_BlueBurstSheet; ///< The rushing strikes.
/// Dragon Cloud and the rushing strikes.
extern char ResourceId_FirePillarSheetB;
/// The orbiting skulls, Planet Diver and the casting impact.
extern char ResourceId_SkullSheet;
/// Bolt, Plasma, Ray, Meteor, Tiamat, Titan Blade, the casting impact and the
/// monster strikes.
extern char ResourceId_LightningBoltSheet;
/// Bind, Astral Blast, Shining Star and the casting impact.
extern char ResourceId_RuneSheet;
extern char ResourceId_RockSpireSheet; ///< The casting impact.
extern char ResourceId_FlameSheetA;    ///< The casting impact's flames.
/// The small Venus Djinni of the casting impact, the Djinn attacks, Ground and
/// the volleys' prelude.
extern char ResourceId_VenusDjinnSmallSheet;
/// The small Mars Djinni of the beam sequences, the casting impact, the Djinn
/// attacks, the healing motes and the volleys' prelude.
extern char ResourceId_MarsDjinnSmallSheet;
/// The small Jupiter Djinni of the casting impact, the Djinn attacks and the
/// volleys' prelude.
extern char ResourceId_JupiterDjinnSmallSheet;
extern char ResourceId_BlueBeastSheet;    ///< The casting impact.
extern char ResourceId_RedCrescentSheetB; ///< The casting impact.
extern char ResourceId_TanPalette;        ///< Colours for the casting impact.
/// Colours for the casting impact, the streaks and the motes.
extern char ResourceId_CyanPalette;
/// The title intro's sprite colours and compressed sprite tiles
/// (Title_LoadIntroBackgrounds).
extern char ResourceId_IntroGraphicsA;
/// The 256-colour picture Title_ShowIntro puts on BG2 once the intro's
/// sprites are gone.
extern char ResourceId_IntroGraphicsB;
/// The title intro's scrolling backgrounds: their colours, then their
/// compressed tiles (Title_LoadIntroBackgrounds).
extern char ResourceId_IntroGraphicsC;
extern char ResourceId_CyanSparkSheet; ///< Death Plunge.
extern char ResourceId_StarBurstSheet; ///< The beam sequences.
extern char ResourceId_CrescentSheet;  ///< The beam sequences.
extern char ResourceId_SwordSheet;     ///< Titan Blade.
extern char ResourceId_VioletPaletteB; ///< Colours for Plasma.
extern char ResourceId_FireSwirlSheet; ///< Helm Splitter.
extern char ResourceId_FireBurstSheet; ///< Annihilation.
/// Heat Wave's fireballs, and the walls of mirrored_wall.c.
extern char ResourceId_FireballSheet;
/// The three lights of Demon Night, Rumble and Haunting.
extern char ResourceId_MemberBurstImage;
/// The beam sequences' waves, and the rush bursts.
extern char ResourceId_WaveSheet;
extern char ResourceId_PinkStarSheet; ///< Helm Splitter.
/// Gaia, the walls, Revive and its family (djinn_beams.c) and the streaks.
extern char ResourceId_EarthWallSheet;
/// Volcano, Titan Blade and the Nereid summon.
extern char ResourceId_FirePillarSheetA;
extern char ResourceId_RedPaletteB;    ///< Colours for Volcano and the streaks.
extern char ResourceId_OrangePaletteB; ///< Colours for Plasma and Volcano.
extern char ResourceId_FlameballSheet; ///< Planet Diver and the Djinn attacks.
extern char ResourceId_BoulderSheet;   ///< Spire.
extern char ResourceId_RockWallSheet;  ///< Quake.
/// The large Jupiter Djinni of Bind and the stat changes; Lucky Wheels
/// takes its colours.
extern char ResourceId_JupiterDjinnSheet;
extern char ResourceId_ShieldSheet; ///< The stat changes.
extern char ResourceId_DaggerSheet; ///< The stat changes.
/// The stat reductions' picture after their shards
/// (STAT_REDUCTION_SHARD_CELLS).
extern char ResourceId_PaletteRampImage;
extern char ResourceId_HeartSheet; ///< Heartrender, among the status motes.
/// Revive and its family (djinn_beams.c).
extern char ResourceId_CrescentMoonSheet;
/// The beam sequences and the rush bursts.
extern char ResourceId_BlueArcSheetA;
/// The beam sequences and the rush bursts, beside BlueArcSheetA.
extern char ResourceId_BlueArcSheetB;
/// The Djinn attacks, the stat changes and the status motes.
extern char ResourceId_GlowOrbSheet;
/// The lobbed shells' images, whose colours are Mad Spatter's.
extern char ResourceId_GoldShellSheet;
/// The beam sequences' BEAM_IMAGE, and the picture of a monster's slash.
extern char ResourceId_BeamSequenceImage;
/// Ice and Douse, and the monster strikes.
extern char ResourceId_BlueBeamSheet;
/// Froth and Whirlwind, and the status motes.
extern char ResourceId_VortexSheet;
extern char ResourceId_LightningPillarSheet; ///< Plasma and the Thor summon.
/// The shattering opening (BattleAnim_RunShatteringOpening).
extern char ResourceId_TileAnimationMask;
extern char ResourceId_VioletPaletteA;  ///< Colours for Death Plunge.
extern char ResourceId_RedPaletteA;     ///< Colours for Death Plunge.
extern char ResourceId_YellowPaletteA;  ///< Colours for Death Plunge.
extern char ResourceId_FlameBladeSheet; ///< Ragnarok.
extern char ResourceId_IceTileSheet;    ///< Death Plunge.
/// The streak volleys' images: three colour rows of six 20x40 images.
extern char ResourceId_FiveModeImage;
extern char ResourceId_SpiderWebSheet; ///< Spider Web and the lobbed shells.
extern char ResourceId_FlareSheet;     ///< Helm Splitter.
extern char ResourceId_FlareImage;     ///< Helm Splitter, beside FlareSheet.
extern char ResourceId_VioletLightningSheetB; ///< The Thor summon.
extern char ResourceId_BlueCrescentSheet;     ///< Judgment.
/// Judgment's small flash (JUDGMENT_SHEET_FLASH_SMALL).
extern char ResourceId_OverlayImageA;
extern char ResourceId_VioletMoteSheet; ///< The Thor summon.
/// Judgment's burst (JUDGMENT_SHEET_BURST).
extern char ResourceId_OverlayImageB;
extern char ResourceId_SlashSheet; ///< The Atalanta summon.
/// Judgment's explosion (JUDGMENT_SHEET_EXPLOSION).
extern char ResourceId_ExplosionSheet;
extern char ResourceId_ParticleSpritesB; ///< The Thor summon.
/// The Thor summon, beside ParticleSpritesB.
extern char ResourceId_ParticleSpritesC;
/// Rotten Blood's green, which Decompose shares.
extern char ResourceId_GreenPalette;
extern char ResourceId_SwordSlashSheet; ///< Death Plunge.
extern char ResourceId_OrangePaletteA;  ///< Colours for Gaia and the streaks.
extern char ResourceId_WhirlwindSheet;  ///< The Procne summon.
/// The Procne summon, after its sky is drawn.
extern char ResourceId_VioletCometSheet;
/// The large Mercury Djinni, whose colours Revive's family, Haunting's
/// lights and the healing motes borrow; Lucky Wheels steps toward it.
extern char ResourceId_MercuryDjinnSheet;
/// The large Venus Djinni, whose colours Revive's family and the stat changes
/// borrow; Lucky Wheels steps toward it.
extern char ResourceId_VenusDjinnSheet;
extern char ResourceId_FireBlobSheet;     ///< The Kirin summon.
extern char ResourceId_YellowRingSheet;   ///< The Cybele summon.
extern char ResourceId_BluePalette;       ///< Colours for the lobbed shells.
extern char ResourceId_WispSheet;         ///< The stat changes.
extern char ResourceId_PortalSheet;       ///< Hail and Ground.
extern char ResourceId_VioletStreakSheet; ///< The stat changes.
extern char ResourceId_ThornVineSheet;    ///< The Cybele summon.
extern char ResourceId_IceShardSheet; ///< Ice and Douse, and the healing motes.
extern char ResourceId_LightFanSheet; ///< The Thor summon.
extern char ResourceId_FlameSheetB;   ///< Meteor and Outer Space.
/// The monster strikes' picture for every kind but a slash.
extern char ResourceId_TwelveModeImage;
/// The star field's rings (ElementGlow_InitializeStarField), with the Mercury
/// field's colours.
extern char ResourceId_BlueRingSheet;
/// The Mars star field's colours (ElementGlow_InitializeStarField).
extern char ResourceId_RedPaletteC;
/// The path Atalanta's particles follow.
extern char ResourceId_StagedParticlePath;
extern char ResourceId_DualStreamPath; ///< The path of Dragon Cloud's streams.
#endif
