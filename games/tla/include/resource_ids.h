#ifndef ALCHEMY_RESOURCE_IDS_H
#define ALCHEMY_RESOURCE_IDS_H

/* Resource numbers, named where their contents are known, as The Broken Seal's
   resource_ids.h.

   Each ResourceId_ is an absolute symbol whose value is the resource's row in
   syResourceDirectoryTable (system/resource_directory.s); C names the number by
   its macro, RESOURCE_RUNE_SHEET for ResourceId_RuneSheet. */

#include "types.h"

/* Battle effects: the sprite sheets, textures and colours the Psynergy,
   summons, Djinn and attacks draw, each with the effects that draw it. */

/* Drawn by Bind, the casting impacts, Rime, Spider Web, the stars of Astral
   Blast and Shining Star, Thunder Mine and Whorl. */
extern char ResourceId_RuneSheet;
#define RESOURCE_RUNE_SHEET ((Sint32)&ResourceId_RuneSheet)
/* Drawn by Bind, Charon, Cruel Ruin, the shards of Ice and Douse, Lucky Wheels,
   Neptune, the effect position helpers, Spider Web, the stat changes and the
   monster strikes. */
extern char ResourceId_BlueGlowSheet;
#define RESOURCE_BLUE_GLOW_SHEET ((Sint32)&ResourceId_BlueGlowSheet)
/* Drawn by Bind, Gasp, the healing motes, Lucky Wheels, Rime, the stat changes,
   Thor and Vengeance. */
extern char ResourceId_JupiterDjinnSheet;
#define RESOURCE_JUPITER_DJINN_SHEET ((Sint32)&ResourceId_JupiterDjinnSheet)
/* Shared by the burst showers, the canvas layer, the casting impacts, the
   charge of Core and Cannon, the glowing lunge, the rushing strike and the
   monster strikes, and by Catastrophe, Charon, Cruel Ruin, Daedalus, Death
   Plunge, Dragon Cloud, Fume, Legend, Liquifier, Meteor Blow, Moloch, Ocean
   Fist, Plasma, Procne, Raging Heat, Ragnarok, Thunder Mine, Trident, Undead
   Sword and Whorl. */
extern char ResourceId_FlashBurstSheet;
#define RESOURCE_FLASH_BURST_SHEET ((Sint32)&ResourceId_FlashBurstSheet)
/* Colours shared by the breaths, the burst showers, the canvas layer, the
   casting impacts, the charge of Core and Cannon, the glowing lunge, the
   healing motes, the rushing strike, the monster strikes and the Zagan summon,
   and by Daedalus, Dragon Cloud, Gasp, Geode, Haures, Heavy Divide, Legend,
   Liquifier, Megiddo, Meteor Blow, Nirvana, Plasma, Raging Heat, Thunder Mine,
   Trident, Vengeance and Volcano. */
extern char ResourceId_OrangePaletteB;
#define RESOURCE_ORANGE_PALETTE_B ((Sint32)&ResourceId_OrangePaletteB)
/* Drawn by the Cards, the casting impacts and Condemn. */
extern char ResourceId_CounterRevealSheet;
#define RESOURCE_COUNTER_REVEAL_SHEET ((Sint32)&ResourceId_CounterRevealSheet)
extern char ResourceId_RedCrescentSheetA;
#define RESOURCE_RED_CRESCENT_SHEET_A ((Sint32)&ResourceId_RedCrescentSheetA)
/* Drawn by Cruel Ruin. */
extern char ResourceId_ShatteringOpeningSheet;
#define RESOURCE_SHATTERING_OPENING_SHEET \
	((Sint32)&ResourceId_ShatteringOpeningSheet)
extern char ResourceId_CyanSparkSheet;
#define RESOURCE_CYAN_SPARK_SHEET ((Sint32)&ResourceId_CyanSparkSheet)
/* Colours for Cruel Ruin and the monster strikes. */
extern char ResourceId_YellowPaletteA;
#define RESOURCE_YELLOW_PALETTE_A ((Sint32)&ResourceId_YellowPaletteA)
/* Drawn by Cruel Ruin and the monster strikes. */
extern char ResourceId_IceTileSheet;
#define RESOURCE_ICE_TILE_SHEET ((Sint32)&ResourceId_IceTileSheet)
/* Colours for Cruel Ruin and the monster strikes. */
extern char ResourceId_RedPaletteA;
#define RESOURCE_RED_PALETTE_A ((Sint32)&ResourceId_RedPaletteA)
extern char ResourceId_VioletPaletteA;
#define RESOURCE_VIOLET_PALETTE_A ((Sint32)&ResourceId_VioletPaletteA)
/* Colours for the battle background's star field (btElementGlowInitStarField)
   and Cruel Ruin. */
extern char ResourceId_YellowPaletteB;
#define RESOURCE_YELLOW_PALETTE_B ((Sint32)&ResourceId_YellowPaletteB)
/* Drawn by the casting impacts, Cruel Ruin, Legend, Prism, the Djinn openings,
   Thor and Undead Sword. */
extern char ResourceId_CrystalSheet;
#define RESOURCE_CRYSTAL_SHEET ((Sint32)&ResourceId_CrystalSheet)
/* Drawn by Atalanta, the casting impacts, Cruel Ruin, the one-Djinni summons,
   Dragon Cloud, Fire, Flare, the rush bursts, Haures, Lucky Wheels, Odyssey,
   Procne, Spider Web, the streak volleys, the monster strikes and Thunder
   Mine. */
extern char ResourceId_EmberStreakSheet;
#define RESOURCE_EMBER_STREAK_SHEET ((Sint32)&ResourceId_EmberStreakSheet)
/* Colours for the battle background's star field (btElementGlowInitStarField),
   Cruel Ruin and the streak volleys. */
extern char ResourceId_VioletPaletteD;
#define RESOURCE_VIOLET_PALETTE_D ((Sint32)&ResourceId_VioletPaletteD)
/* Drawn by the casting impacts and Cruel Ruin. */
extern char ResourceId_DustPuffSheet;
#define RESOURCE_DUST_PUFF_SHEET ((Sint32)&ResourceId_DustPuffSheet)
/* Drawn by the Zagan summon, Fire Puppet, Gasp, the healing motes, Light Surge,
   Lucky Wheels, the shell shot, Nirvana, Planet Diver, Rime, the stat changes,
   Thor and Vengeance. */
extern char ResourceId_MercuryDjinnSheet;
#define RESOURCE_MERCURY_DJINN_SHEET ((Sint32)&ResourceId_MercuryDjinnSheet)
/* Drawn by the breaths, the canvas layer, the casting impacts, the one-Djinni
   summons, the Zagan summon, Heavy Divide, Helm Splitter, Liquifier, Neptune,
   Nirvana, Ocean Fist, Odyssey, Rising Dragon, Triple Chomp, Vengeance and
   Whorl. */
extern char ResourceId_BlueFlameSheet;
#define RESOURCE_BLUE_FLAME_SHEET ((Sint32)&ResourceId_BlueFlameSheet)
/* Drawn by the breaths, the blessings' burst showers, the casting impacts,
   Daedalus, the Zagan summon, Flora, the rush bursts, Gasp, Haures, the healing
   motes, the shell shot, Planet Diver, Spider Web, the stat changes, the status
   motes, the monster strikes, Thor, Thunder Mine, Vengeance and Whorl. */
extern char ResourceId_MarsDjinnSheet;
#define RESOURCE_MARS_DJINN_SHEET ((Sint32)&ResourceId_MarsDjinnSheet)
/* Drawn by the shards of Ice and Douse and the healing motes. */
extern char ResourceId_IceShardSheet;
#define RESOURCE_ICE_SHARD_SHEET ((Sint32)&ResourceId_IceShardSheet)
/* Drawn by Boreas, the shards of Ice and Douse, Frost, the healing motes, the
   status motes and Thunder Mine. */
extern char ResourceId_SparkleDots;
#define RESOURCE_SPARKLE_DOTS ((Sint32)&ResourceId_SparkleDots)
/* Drawn by Azul, the blessings' burst showers, the shards of Ice and Douse, the
   healing motes, the Djinn openings, the monster strikes and Vengeance. */
extern char ResourceId_BlueBeamSheet;
#define RESOURCE_BLUE_BEAM_SHEET ((Sint32)&ResourceId_BlueBeamSheet)
/* Drawn by Daedalus, Gaia, the shell shot and the streak volleys. */
extern char ResourceId_EarthWallSheet;
#define RESOURCE_EARTH_WALL_SHEET ((Sint32)&ResourceId_EarthWallSheet)
/* Drawn by the shell shot. */
extern char ResourceId_CrescentMoonSheet;
#define RESOURCE_CRESCENT_MOON_SHEET ((Sint32)&ResourceId_CrescentMoonSheet)
/* Drawn by Gasp, the healing motes, Lucky Wheels, the shell shot and the stat
   changes. */
extern char ResourceId_VenusDjinnSheet;
#define RESOURCE_VENUS_DJINN_SHEET ((Sint32)&ResourceId_VenusDjinnSheet)
/* Drawn by the shell shot. */
extern char ResourceId_GoldShellSheet;
#define RESOURCE_GOLD_SHELL_SHEET ((Sint32)&ResourceId_GoldShellSheet)
/* Drawn by the blessings' burst showers, the Cards, the casting impacts,
   Daedalus, Flora, the rush bursts, Gasp, Heavy Divide, Legend, the shell shot,
   the orbiting skulls, stars and target bursts, the Djinn openings, the status
   motes, the monster strikes and Undead Sword. */
extern char ResourceId_SmokeSheet;
#define RESOURCE_SMOKE_SHEET ((Sint32)&ResourceId_SmokeSheet)
/* Colours for the shell shot. */
extern char ResourceId_BluePalette;
#define RESOURCE_BLUE_PALETTE ((Sint32)&ResourceId_BluePalette)
/* Drawn by the shell shot and Spider Web. */
extern char ResourceId_SpiderWebSheet;
#define RESOURCE_SPIDER_WEB_SHEET ((Sint32)&ResourceId_SpiderWebSheet)
/* Colours for Atalanta, the blessings' burst showers, the casting impacts, the
   rush bursts, Lucky Wheels, the shell shot, the Djinn openings, Spider Web,
   the streak volleys, the status motes and the monster strikes. */
extern char ResourceId_LimePalette;
#define RESOURCE_LIME_PALETTE ((Sint32)&ResourceId_LimePalette)
/* Colours for the shell shot and the streak volleys. */
extern char ResourceId_GreenPalette;
#define RESOURCE_GREEN_PALETTE ((Sint32)&ResourceId_GreenPalette)
/* Drawn by the blessings' burst showers, Hail, Neptune, Nereid, the particle
   streams, the Djinn openings and Whorl. */
extern char ResourceId_WaterSpraySheet;
#define RESOURCE_WATER_SPRAY_SHEET ((Sint32)&ResourceId_WaterSpraySheet)
/* Drawn by Nereid, the particle streams, Ragnarok, Undead Sword and Volcano. */
extern char ResourceId_FirePillarSheetA;
#define RESOURCE_FIRE_PILLAR_SHEET_A ((Sint32)&ResourceId_FirePillarSheetA)
/* Drawn by the casting impacts, Helm Splitter and Planet Diver. */
extern char ResourceId_SkullSheet;
#define RESOURCE_SKULL_SHEET ((Sint32)&ResourceId_SkullSheet)
/* Drawn by the casting impacts, the glowing lunge, Judgment, the shell shot,
   Nirvana, the orbiting skulls, stars and target bursts, Planet Diver, Raging
   Heat, the stat changes and the status motes. */
extern char ResourceId_PinkBurstSheet;
#define RESOURCE_PINK_BURST_SHEET ((Sint32)&ResourceId_PinkBurstSheet)
/* Drawn by Planet Diver. */
extern char ResourceId_SpiralSheet;
#define RESOURCE_SPIRAL_SHEET ((Sint32)&ResourceId_SpiralSheet)
extern char ResourceId_MemberBurstImage;
#define RESOURCE_MEMBER_BURST_IMAGE ((Sint32)&ResourceId_MemberBurstImage)
/* Drawn by Plasma and Thor. */
extern char ResourceId_LightningPillarSheet;
#define RESOURCE_LIGHTNING_PILLAR_SHEET \
	((Sint32)&ResourceId_LightningPillarSheet)
/* Colours for Plasma. */
extern char ResourceId_VioletPaletteB;
#define RESOURCE_VIOLET_PALETTE_B ((Sint32)&ResourceId_VioletPaletteB)
/* Drawn by the blessings' burst showers, the casting impacts, Nirvana, Prism
   and Spire. */
extern char ResourceId_BoulderSheet;
#define RESOURCE_BOULDER_SHEET ((Sint32)&ResourceId_BoulderSheet)
/* Drawn by Procne. */
extern char ResourceId_WhirlwindSheet;
#define RESOURCE_WHIRLWIND_SHEET ((Sint32)&ResourceId_WhirlwindSheet)
/* Drawn by Procne. */
extern char ResourceId_VioletCometSheet;
#define RESOURCE_VIOLET_COMET_SHEET ((Sint32)&ResourceId_VioletCometSheet)
/* Drawn by Bolt, the casting impacts, Catastrophe, Gasp, the particle streams,
   Plasma, Procne, Ragnarok, Ray, the monster strikes and Thunder Mine. */
extern char ResourceId_LightningBoltSheet;
#define RESOURCE_LIGHTNING_BOLT_SHEET ((Sint32)&ResourceId_LightningBoltSheet)
/* Drawn by the streak volleys. */
extern char ResourceId_FlameArcSheet;
#define RESOURCE_FLAME_ARC_SHEET ((Sint32)&ResourceId_FlameArcSheet)
/* Colours for the casting impacts, the streak volleys and Volcano. */
extern char ResourceId_RedPaletteB;
#define RESOURCE_RED_PALETTE_B ((Sint32)&ResourceId_RedPaletteB)
/* Colours for the blessings' burst showers, the casting impacts, Flora, the
   healing motes, Heavy Divide, the streak volleys, the status motes and Undead
   Sword. */
extern char ResourceId_CyanPalette;
#define RESOURCE_CYAN_PALETTE ((Sint32)&ResourceId_CyanPalette)
/* Colours for Gaia and the streak volleys. */
extern char ResourceId_OrangePaletteA;
#define RESOURCE_ORANGE_PALETTE_A ((Sint32)&ResourceId_OrangePaletteA)
/* Drawn by the blessings' burst showers, the casting impacts, Crystal, Cybele,
   Drain, Eruption, Gasp, the glowing lunge, the healing motes, Heavy Divide,
   Nirvana, the particle streams, the Djinn openings, Ragnarok, Ramses, the
   status motes, Thor, Undead Sword and Vengeance. */
extern char ResourceId_BlastSheet;
#define RESOURCE_BLAST_SHEET ((Sint32)&ResourceId_BlastSheet)
/* Drawn by the one-Djinni summons, the charge of Core and Cannon, the stat
   changes and the status motes. */
extern char ResourceId_GlowOrbSheet;
#define RESOURCE_GLOW_ORB_SHEET ((Sint32)&ResourceId_GlowOrbSheet)
/* Drawn by the blessings' burst showers and the status motes. */
extern char ResourceId_HeartSheet;
#define RESOURCE_HEART_SHEET ((Sint32)&ResourceId_HeartSheet)
/* Drawn by the status motes. */
extern char ResourceId_FlameColumnSheet;
#define RESOURCE_FLAME_COLUMN_SHEET ((Sint32)&ResourceId_FlameColumnSheet)
/* Drawn by the casting impacts, the one-Djinni summons, Gasp, Hail, the healing
   motes, the Djinn openings, the status motes and Undead Sword. */
extern char ResourceId_MercuryDjinnSmallSheet;
#define RESOURCE_MERCURY_DJINN_SMALL_SHEET \
	((Sint32)&ResourceId_MercuryDjinnSmallSheet)
/* Drawn by Bolt, the blessings' burst showers, the canvas layer, the casting
   impacts, Catastrophe, the rush bursts, Judgment, the Djinn openings, the
   status motes, the monster strikes, Thor, Thunder Mine, Trident and Froth and
   Whirlwind. */
extern char ResourceId_TornadoSheet;
#define RESOURCE_TORNADO_SHEET ((Sint32)&ResourceId_TornadoSheet)
/* Drawn by the effect position helpers and the monster strikes. */
extern char ResourceId_BeamImage;
#define RESOURCE_BEAM_IMAGE ((Sint32)&ResourceId_BeamImage)
/* Drawn by the casting impacts and the monster strikes. */
extern char ResourceId_BlueDropletSheet;
#define RESOURCE_BLUE_DROPLET_SHEET ((Sint32)&ResourceId_BlueDropletSheet)
/* Colours for the blessings' burst showers and the monster strikes. */
extern char ResourceId_PinkPalette;
#define RESOURCE_PINK_PALETTE ((Sint32)&ResourceId_PinkPalette)
/* Drawn by the status motes and the vortices of Froth and Whirlwind. */
extern char ResourceId_VortexSheet;
#define RESOURCE_VORTEX_SHEET ((Sint32)&ResourceId_VortexSheet)
/* Drawn by Annihilation, the casting impacts and Eruption. */
extern char ResourceId_FireStreakSheet;
#define RESOURCE_FIRE_STREAK_SHEET ((Sint32)&ResourceId_FireStreakSheet)
/* Drawn by the casting impacts, the one-Djinni summons, Gasp, the healing
   motes, the Djinn openings, Undead Sword and Whorl. */
extern char ResourceId_JupiterDjinnSmallSheet;
#define RESOURCE_JUPITER_DJINN_SMALL_SHEET \
	((Sint32)&ResourceId_JupiterDjinnSmallSheet)
/* Drawn by the blessings' burst showers, the casting impacts, Crystal, the
   one-Djinni summons, Gasp, Geode, Hail, the healing motes and the Djinn
   openings. */
extern char ResourceId_VenusDjinnSmallSheet;
#define RESOURCE_VENUS_DJINN_SMALL_SHEET \
	((Sint32)&ResourceId_VenusDjinnSmallSheet)
/* Drawn by the casting impacts, Frost, Hail and the Djinn openings. */
extern char ResourceId_IceChipSheet;
#define RESOURCE_ICE_CHIP_SHEET ((Sint32)&ResourceId_IceChipSheet)
/* Drawn by Boreas, the blessings' burst showers, the casting impacts, Drain,
   the glowing lunge, the healing motes and the Djinn openings. */
extern char ResourceId_IceBlockSheet;
#define RESOURCE_ICE_BLOCK_SHEET ((Sint32)&ResourceId_IceBlockSheet)
/* Drawn by the blessings' burst showers and the casting impacts. */
extern char ResourceId_RockSpireSheet;
#define RESOURCE_ROCK_SPIRE_SHEET ((Sint32)&ResourceId_RockSpireSheet)
/* Drawn by Azul and the stat changes. */
extern char ResourceId_WispSheet;
#define RESOURCE_WISP_SHEET ((Sint32)&ResourceId_WispSheet)
/* Drawn by the blessings' burst showers, the casting impacts, the one-Djinni
   summons, the charge of Core and Cannon, Gasp, the healing motes, the effect
   position helpers and the Djinn openings. */
extern char ResourceId_MarsDjinnSmallSheet;
#define RESOURCE_MARS_DJINN_SMALL_SHEET \
	((Sint32)&ResourceId_MarsDjinnSmallSheet)
/* Drawn by the Cards and the stat changes. */
extern char ResourceId_DaggerSheet;
#define RESOURCE_DAGGER_SHEET ((Sint32)&ResourceId_DaggerSheet)
/* Colours for the Cards and the stat changes. */
extern char ResourceId_PaletteRampImage;
#define RESOURCE_PALETTE_RAMP_IMAGE ((Sint32)&ResourceId_PaletteRampImage)
/* Drawn by Cybele. */
extern char ResourceId_ThornVineSheet;
#define RESOURCE_THORN_VINE_SHEET ((Sint32)&ResourceId_ThornVineSheet)
extern char ResourceId_YellowRingSheet;
#define RESOURCE_YELLOW_RING_SHEET ((Sint32)&ResourceId_YellowRingSheet)
/* Drawn by the stat changes. */
extern char ResourceId_VioletStreakSheet;
#define RESOURCE_VIOLET_STREAK_SHEET ((Sint32)&ResourceId_VioletStreakSheet)
extern char ResourceId_ShieldSheet;
#define RESOURCE_SHIELD_SHEET ((Sint32)&ResourceId_ShieldSheet)
/* Drawn by Quake. */
extern char ResourceId_RockWallSheet;
#define RESOURCE_ROCK_WALL_SHEET ((Sint32)&ResourceId_RockWallSheet)
/* Drawn by the blessings' burst showers. */
extern char ResourceId_VioletLightningSheetA;
#define RESOURCE_VIOLET_LIGHTNING_SHEET_A \
	((Sint32)&ResourceId_VioletLightningSheetA)
extern char ResourceId_DemonFaceSheet;
#define RESOURCE_DEMON_FACE_SHEET ((Sint32)&ResourceId_DemonFaceSheet)
/* Drawn by the orbiting skulls, stars and target bursts. */
extern char ResourceId_StarDotSheet;
#define RESOURCE_STAR_DOT_SHEET ((Sint32)&ResourceId_StarDotSheet)
extern char ResourceId_TargetBurstImage;
#define RESOURCE_TARGET_BURST_IMAGE ((Sint32)&ResourceId_TargetBurstImage)
/* Drawn by Atalanta. */
extern char ResourceId_SlashSheet;
#define RESOURCE_SLASH_SHEET ((Sint32)&ResourceId_SlashSheet)
/* The path Atalanta's particles follow. */
extern char ResourceId_AtalantaPath;
#define RESOURCE_ATALANTA_PATH ((Sint32)&ResourceId_AtalantaPath)
/* Drawn by the growing columns. */
extern char ResourceId_GreenPillarSheet;
#define RESOURCE_GREEN_PILLAR_SHEET ((Sint32)&ResourceId_GreenPillarSheet)
/* Drawn by Growth. */
extern char ResourceId_VineSheet;
#define RESOURCE_VINE_SHEET ((Sint32)&ResourceId_VineSheet)
/* Drawn by Vine: five 32x32 vine cels. */
extern char ResourceId_GreenVineSheet;
#define RESOURCE_GREEN_VINE_SHEET ((Sint32)&ResourceId_GreenVineSheet)
/* Drawn by Thorn: the large thorn, the small one and the dust cels after
   them. */
extern char ResourceId_ThornSheet;
#define RESOURCE_THORN_SHEET ((Sint32)&ResourceId_ThornSheet)
/* Drawn by the casting impacts, Diamond Berg, the form changes, Geode, the
   leaping strike, Meteor Blow, Plume Edge and Trident. */
extern char ResourceId_BillowTexture;
#define RESOURCE_BILLOW_TEXTURE ((Sint32)&ResourceId_BillowTexture)
/* The particle sprites nearly every battle effect loads first, 90 of them, from
   Annihilation to Zagan. */
extern char ResourceId_YellowGlowSheet;
#define RESOURCE_YELLOW_GLOW_SHEET ((Sint32)&ResourceId_YellowGlowSheet)
/* Drawn by the Djinni charge and Zagan. */
extern char ResourceId_PetalSheet;
#define RESOURCE_PETAL_SHEET ((Sint32)&ResourceId_PetalSheet)
/* Drawn by the breaths, the casting impacts, Diamond Berg, Eclipse, the Gasp
   family, the leaping strike, Meteor Blow, Moloch, Serac, Trident and Zagan. */
extern char ResourceId_FlameBandTexture;
#define RESOURCE_FLAME_BAND_TEXTURE ((Sint32)&ResourceId_FlameBandTexture)
extern char ResourceId_SheepCardSheet; /* directory row 0xe3 */
#define RESOURCE_SHEEP_CARD_SHEET ((Sint32)&ResourceId_SheepCardSheet)
/* Drawn by the Djinni sign and the target strike. */
extern char ResourceId_StarCrossSheet;
#define RESOURCE_STAR_CROSS_SHEET ((Sint32)&ResourceId_StarCrossSheet)
/* Drawn by the blade rain and Sabre Dance. */
extern char ResourceId_CrossedDaggersSheet;
#define RESOURCE_CROSSED_DAGGERS_SHEET ((Sint32)&ResourceId_CrossedDaggersSheet)
/* Drawn by the burst showers, the casting impacts and the stat boosts. */
extern char ResourceId_GreyPaletteA;
#define RESOURCE_GREY_PALETTE_A ((Sint32)&ResourceId_GreyPaletteA)
/* Drawn by Azul, the breaths, the casting impacts, Catastrophe, Charon,
   Coatlicue, Cruel Ruin, Daedalus, Death Plunge, Diamond Berg, Flora, the Gasp
   family, Judgment, Legend, Liquifier, Megiddo, Odyssey, Radiant Fire, Raging
   Heat, the rushing strike and Triple Chomp. */
extern char ResourceId_FireRingTexture;
#define RESOURCE_FIRE_RING_TEXTURE ((Sint32)&ResourceId_FireRingTexture)
/* Drawn by the breaths, Cruel Ruin, Daedalus's second strike, Eclipse, the form
   changes, the Gasp family, Light Surge, Liquifier and Megiddo. */
extern char ResourceId_GlowBandTextureA;
#define RESOURCE_GLOW_BAND_TEXTURE_A ((Sint32)&ResourceId_GlowBandTextureA)
/* Drawn by the bolt trail and the drain motes. */
extern char ResourceId_VioletPaletteC;
#define RESOURCE_VIOLET_PALETTE_C ((Sint32)&ResourceId_VioletPaletteC)
/* Drawn by the burst showers and the Gasp family. */
extern char ResourceId_MusicNoteSheet;
#define RESOURCE_MUSIC_NOTE_SHEET ((Sint32)&ResourceId_MusicNoteSheet)
/* Drawn by Liquifier. */
extern char ResourceId_FlameWallTexture;
#define RESOURCE_FLAME_WALL_TEXTURE ((Sint32)&ResourceId_FlameWallTexture)
/* Drawn by Atalanta. */
extern char ResourceId_LeafWarriorTiles;
#define RESOURCE_LEAF_WARRIOR_TILES ((Sint32)&ResourceId_LeafWarriorTiles)
/* Drawn by the form changes. */
extern char ResourceId_OrangePaletteC;
#define RESOURCE_ORANGE_PALETTE_C ((Sint32)&ResourceId_OrangePaletteC)
extern char ResourceId_CometTexture; /* directory row 0xd0 */
#define RESOURCE_COMET_TEXTURE ((Sint32)&ResourceId_CometTexture)
extern char ResourceId_FeatherSheet; /* directory row 0xd1 */
#define RESOURCE_FEATHER_SHEET ((Sint32)&ResourceId_FeatherSheet)
/* Drawn by Zagan. */
extern char ResourceId_CrescentRowSheet;
#define RESOURCE_CRESCENT_ROW_SHEET ((Sint32)&ResourceId_CrescentRowSheet)
/* Drawn by Azul, the casting impacts, Catastrophe, Charon, Cruel Ruin, Eclipse,
   the Gasp family, Haures, the water column and the water sheet. */
extern char ResourceId_SwirlTexture;
#define RESOURCE_SWIRL_TEXTURE ((Sint32)&ResourceId_SwirlTexture)
/* Drawn by Cruel Ruin and Eclipse. */
extern char ResourceId_GlowBandTextureB;
#define RESOURCE_GLOW_BAND_TEXTURE_B ((Sint32)&ResourceId_GlowBandTextureB)
/* Drawn by Cruel Ruin. */
extern char ResourceId_PinkBarSheet;
#define RESOURCE_PINK_BAR_SHEET ((Sint32)&ResourceId_PinkBarSheet)
/* Drawn by Azul, Cruel Ruin and Legend. */
extern char ResourceId_CornerRuneASheet;
#define RESOURCE_CORNER_RUNE_A_SHEET ((Sint32)&ResourceId_CornerRuneASheet)
/* Drawn by Cruel Ruin and Daedalus. */
extern char ResourceId_ForestCanopyTiles;
#define RESOURCE_FOREST_CANOPY_TILES ((Sint32)&ResourceId_ForestCanopyTiles)
/* Drawn by Eclipse. */
extern char ResourceId_GoldenDragonTiles;
#define RESOURCE_GOLDEN_DRAGON_TILES ((Sint32)&ResourceId_GoldenDragonTiles)
extern char ResourceId_GoldenDragonHeadTiles;
#define RESOURCE_GOLDEN_DRAGON_HEAD_TILES \
	((Sint32)&ResourceId_GoldenDragonHeadTiles)
/* Drawn by the Djinni charge, Eclipse, Megiddo, Meteor Blow, Odyssey and
   Serac. */
extern char ResourceId_DustBlobSheet;
#define RESOURCE_DUST_BLOB_SHEET ((Sint32)&ResourceId_DustBlobSheet)
/* Drawn by the projectile volleys. */
extern char ResourceId_CometSheetA;
#define RESOURCE_COMET_SHEET_A ((Sint32)&ResourceId_CometSheetA)
/* Drawn by the projectile volleys and Void Beam. */
extern char ResourceId_CometSheetB;
#define RESOURCE_COMET_SHEET_B ((Sint32)&ResourceId_CometSheetB)
/* Drawn by the projectile volleys. */
extern char ResourceId_CometSheetC;
#define RESOURCE_COMET_SHEET_C ((Sint32)&ResourceId_CometSheetC)
extern char ResourceId_WindStreakSheet;
#define RESOURCE_WIND_STREAK_SHEET ((Sint32)&ResourceId_WindStreakSheet)
extern char ResourceId_ShortStreakPicture;
#define RESOURCE_SHORT_STREAK_PICTURE ((Sint32)&ResourceId_ShortStreakPicture)
extern char ResourceId_ShurikenSheet;
#define RESOURCE_SHURIKEN_SHEET ((Sint32)&ResourceId_ShurikenSheet)
extern char ResourceId_ProjectileVolleyImage;
#define RESOURCE_PROJECTILE_VOLLEY_IMAGE \
	((Sint32)&ResourceId_ProjectileVolleyImage)
extern char ResourceId_YellowSparkSheet;
#define RESOURCE_YELLOW_SPARK_SHEET ((Sint32)&ResourceId_YellowSparkSheet)
/* Drawn by the casting impacts, the mirrored wall and the projectile
   volleys. */
extern char ResourceId_FireballSheet;
#define RESOURCE_FIREBALL_SHEET ((Sint32)&ResourceId_FireballSheet)
/* Drawn by Jupiter and the leaping strike. */
extern char ResourceId_FlameballSheet;
#define RESOURCE_FLAMEBALL_SHEET ((Sint32)&ResourceId_FlameballSheet)
/* Drawn by Ground. */
extern char ResourceId_PortalSheet;
#define RESOURCE_PORTAL_SHEET ((Sint32)&ResourceId_PortalSheet)
/* Drawn by Thor. */
extern char ResourceId_PaleBlueGlowSheet;
#define RESOURCE_PALE_BLUE_GLOW_SHEET ((Sint32)&ResourceId_PaleBlueGlowSheet)
extern char ResourceId_LightFanSheet;
#define RESOURCE_LIGHT_FAN_SHEET ((Sint32)&ResourceId_LightFanSheet)
extern char ResourceId_BlueTallGlowSheet;
#define RESOURCE_BLUE_TALL_GLOW_SHEET ((Sint32)&ResourceId_BlueTallGlowSheet)
extern char ResourceId_VioletLightningSheetB;
#define RESOURCE_VIOLET_LIGHTNING_SHEET_B \
	((Sint32)&ResourceId_VioletLightningSheetB)
extern char ResourceId_VioletMoteSheet;
#define RESOURCE_VIOLET_MOTE_SHEET ((Sint32)&ResourceId_VioletMoteSheet)
/* Drawn by the attack slashes, the casting impacts, the flurry and Ulysses. */
extern char ResourceId_YellowOrbSheet;
#define RESOURCE_YELLOW_ORB_SHEET ((Sint32)&ResourceId_YellowOrbSheet)
/* Drawn by the attack slashes and the flurry. */
extern char ResourceId_BlueArcSheetA;
#define RESOURCE_BLUE_ARC_SHEET_A ((Sint32)&ResourceId_BlueArcSheetA)
/* Drawn by the attack slashes, the flurry and Haures. */
extern char ResourceId_BlueArcSheetB;
#define RESOURCE_BLUE_ARC_SHEET_B ((Sint32)&ResourceId_BlueArcSheetB)
/* Drawn by the attack slashes and the flurry. */
extern char ResourceId_WaveSheet;
#define RESOURCE_WAVE_SHEET ((Sint32)&ResourceId_WaveSheet)
/* Drawn by Ulysses. */
extern char ResourceId_UlyssesSilhouette;
#define RESOURCE_ULYSSES_SILHOUETTE ((Sint32)&ResourceId_UlyssesSilhouette)
extern char ResourceId_FlutteringFlakeSheet;
#define RESOURCE_FLUTTERING_FLAKE_SHEET \
	((Sint32)&ResourceId_FlutteringFlakeSheet)
extern char ResourceId_RuneCircleSheet;
#define RESOURCE_RUNE_CIRCLE_SHEET ((Sint32)&ResourceId_RuneCircleSheet)
extern char ResourceId_FadingCrescentSheet;
#define RESOURCE_FADING_CRESCENT_SHEET ((Sint32)&ResourceId_FadingCrescentSheet)
/* Drawn by the swirl swipe. */
extern char ResourceId_MagentaSwirlSheet;
#define RESOURCE_MAGENTA_SWIRL_SHEET ((Sint32)&ResourceId_MagentaSwirlSheet)
extern char ResourceId_MagentaTailSheet;
#define RESOURCE_MAGENTA_TAIL_SHEET ((Sint32)&ResourceId_MagentaTailSheet)
/* Drawn by the attack slashes, the crescent slash and the critical hits. */
extern char ResourceId_StarCrescentSheet;
#define RESOURCE_STAR_CRESCENT_SHEET ((Sint32)&ResourceId_StarCrescentSheet)
/* Drawn by the crescent slash and the critical hits. */
extern char ResourceId_GreyCrescentSheet;
#define RESOURCE_GREY_CRESCENT_SHEET ((Sint32)&ResourceId_GreyCrescentSheet)
/* Drawn by the casting impacts. */
extern char ResourceId_ImpFaceSheet;
#define RESOURCE_IMP_FACE_SHEET ((Sint32)&ResourceId_ImpFaceSheet)
extern char ResourceId_ApocalypseSheet;
#define RESOURCE_APOCALYPSE_SHEET ((Sint32)&ResourceId_ApocalypseSheet)
extern char ResourceId_BoulderPicture;
#define RESOURCE_BOULDER_PICTURE ((Sint32)&ResourceId_BoulderPicture)
extern char ResourceId_SarcophagusSheet;
#define RESOURCE_SARCOPHAGUS_SHEET ((Sint32)&ResourceId_SarcophagusSheet)
extern char ResourceId_RaidensWrathSheet;
#define RESOURCE_RAIDENS_WRATH_SHEET ((Sint32)&ResourceId_RaidensWrathSheet)
extern char ResourceId_CloudPuffSheet;
#define RESOURCE_CLOUD_PUFF_SHEET ((Sint32)&ResourceId_CloudPuffSheet)
extern char ResourceId_FlameSheetA;
#define RESOURCE_FLAME_SHEET_A ((Sint32)&ResourceId_FlameSheetA)
/* Drawn by the casting impacts, Flora, Megaera and the wind funnel. */
extern char ResourceId_TanPalette;
#define RESOURCE_TAN_PALETTE ((Sint32)&ResourceId_TanPalette)
/* Drawn by the casting impacts. */
extern char ResourceId_VioletPaletteG;
#define RESOURCE_VIOLET_PALETTE_G ((Sint32)&ResourceId_VioletPaletteG)
extern char ResourceId_BlueBeastSheet;
#define RESOURCE_BLUE_BEAST_SHEET ((Sint32)&ResourceId_BlueBeastSheet)
extern char ResourceId_RedCrescentSheetB;
#define RESOURCE_RED_CRESCENT_SHEET_B ((Sint32)&ResourceId_RedCrescentSheetB)
/* Drawn by the impact burst. */
extern char ResourceId_FlareSheet;
#define RESOURCE_FLARE_SHEET ((Sint32)&ResourceId_FlareSheet)
extern char ResourceId_FlareImage;
#define RESOURCE_FLARE_IMAGE ((Sint32)&ResourceId_FlareImage)
/* Drawn by the particle streams. */
extern char ResourceId_FlameSheetB;
#define RESOURCE_FLAME_SHEET_B ((Sint32)&ResourceId_FlameSheetB)
/* Drawn by Annihilation. */
extern char ResourceId_FireBurstSheet;
#define RESOURCE_FIRE_BURST_SHEET ((Sint32)&ResourceId_FireBurstSheet)
/* Drawn by Ragnarok. */
extern char ResourceId_FlameBladeSheet;
#define RESOURCE_FLAME_BLADE_SHEET ((Sint32)&ResourceId_FlameBladeSheet)
/* Drawn by the falling sword. */
extern char ResourceId_SwordSheet;
#define RESOURCE_SWORD_SHEET ((Sint32)&ResourceId_SwordSheet)
/* Drawn by Judgment. */
extern char ResourceId_WingedKnightATiles;
#define RESOURCE_WINGED_KNIGHT_A_TILES ((Sint32)&ResourceId_WingedKnightATiles)
extern char ResourceId_WingedKnightBTiles;
#define RESOURCE_WINGED_KNIGHT_B_TILES ((Sint32)&ResourceId_WingedKnightBTiles)
extern char ResourceId_WingedKnightCTiles;
#define RESOURCE_WINGED_KNIGHT_C_TILES ((Sint32)&ResourceId_WingedKnightCTiles)
/* Drawn by Judgment and Star Mine. */
extern char ResourceId_BlueCrescentSheet;
#define RESOURCE_BLUE_CRESCENT_SHEET ((Sint32)&ResourceId_BlueCrescentSheet)
/* Drawn by Judgment. */
extern char ResourceId_ExplosionSheet;
#define RESOURCE_EXPLOSION_SHEET ((Sint32)&ResourceId_ExplosionSheet)
extern char ResourceId_JudgmentFlashSheet;
#define RESOURCE_JUDGMENT_FLASH_SHEET ((Sint32)&ResourceId_JudgmentFlashSheet)
/* Drawn by Kirin. */
extern char ResourceId_FireBlobSheet;
#define RESOURCE_FIRE_BLOB_SHEET ((Sint32)&ResourceId_FireBlobSheet)
/* Drawn by Neptune. */
extern char ResourceId_SeaSerpentTiles;
#define RESOURCE_SEA_SERPENT_TILES ((Sint32)&ResourceId_SeaSerpentTiles)
/* Drawn by the flashing strikes. */
extern char ResourceId_ArcFragmentsSheet;
#define RESOURCE_ARC_FRAGMENTS_SHEET ((Sint32)&ResourceId_ArcFragmentsSheet)
extern char ResourceId_VorpalSlashImageA;
#define RESOURCE_VORPAL_SLASH_IMAGE_A ((Sint32)&ResourceId_VorpalSlashImageA)
extern char ResourceId_VorpalSlashImageB;
#define RESOURCE_VORPAL_SLASH_IMAGE_B ((Sint32)&ResourceId_VorpalSlashImageB)
extern char ResourceId_QuickStrikeImageA;
#define RESOURCE_QUICK_STRIKE_IMAGE_A ((Sint32)&ResourceId_QuickStrikeImageA)
extern char ResourceId_QuickStrikeImageB;
#define RESOURCE_QUICK_STRIKE_IMAGE_B ((Sint32)&ResourceId_QuickStrikeImageB)
extern char ResourceId_UnusedFlashingStrikeImageA;
#define RESOURCE_UNUSED_FLASHING_STRIKE_IMAGE_A \
	((Sint32)&ResourceId_UnusedFlashingStrikeImageA)
extern char ResourceId_UnusedFlashingStrikeImageB;
#define RESOURCE_UNUSED_FLASHING_STRIKE_IMAGE_B \
	((Sint32)&ResourceId_UnusedFlashingStrikeImageB)
/* Drawn by Soul Shatter and Undead Sword. */
extern char ResourceId_LimeColumnSheet;
#define RESOURCE_LIME_COLUMN_SHEET ((Sint32)&ResourceId_LimeColumnSheet)
/* Drawn by the water sheet. */
extern char ResourceId_CrackedTextureSheet;
#define RESOURCE_CRACKED_TEXTURE_SHEET ((Sint32)&ResourceId_CrackedTextureSheet)
/* Drawn by Fire Puppet, the water column and the wind funnel. */
extern char ResourceId_StreakTexture;
#define RESOURCE_STREAK_TEXTURE ((Sint32)&ResourceId_StreakTexture)
/* Drawn by Diamond Berg. */
extern char ResourceId_CrystalBitmapSheet;
#define RESOURCE_CRYSTAL_BITMAP_SHEET ((Sint32)&ResourceId_CrystalBitmapSheet)
/* Drawn by Coatlicue and Crystal. */
extern char ResourceId_CogTexture;
#define RESOURCE_COG_TEXTURE ((Sint32)&ResourceId_CogTexture)
/* Drawn by the Cards and Serac. */
extern char ResourceId_BlueCloudPuffSheet;
#define RESOURCE_BLUE_CLOUD_PUFF_SHEET ((Sint32)&ResourceId_BlueCloudPuffSheet)
/* Drawn by the heavy blow. */
extern char ResourceId_GreatSwordSheet;
#define RESOURCE_GREAT_SWORD_SHEET ((Sint32)&ResourceId_GreatSwordSheet)
extern char ResourceId_AxeSheet;
#define RESOURCE_AXE_SHEET ((Sint32)&ResourceId_AxeSheet)
/* Drawn by Cool. */
extern char ResourceId_CrestSheet;
#define RESOURCE_CREST_SHEET ((Sint32)&ResourceId_CrestSheet)
/* Drawn by the Gasp family and Wyrd Curse. */
extern char ResourceId_BurningSkullSheet;
#define RESOURCE_BURNING_SKULL_SHEET ((Sint32)&ResourceId_BurningSkullSheet)
/* Drawn by Megaera and Ocean Fist. */
extern char ResourceId_WhirlBeamSheet;
#define RESOURCE_WHIRL_BEAM_SHEET ((Sint32)&ResourceId_WhirlBeamSheet)
/* Drawn by Light Surge. */
extern char ResourceId_GlowOrbTexture;
#define RESOURCE_GLOW_ORB_TEXTURE ((Sint32)&ResourceId_GlowOrbTexture)
/* Drawn by Catastrophe, Death Plunge, the element cards, the Gasp family, Light
   Surge, Trident and Whiplash. */
extern char ResourceId_VioletPaletteF;
#define RESOURCE_VIOLET_PALETTE_F ((Sint32)&ResourceId_VioletPaletteF)
/* Drawn by the geyser and the water column. */
extern char ResourceId_EmberTexture;
#define RESOURCE_EMBER_TEXTURE ((Sint32)&ResourceId_EmberTexture)
/* Drawn by the Gasp family. */
extern char ResourceId_LightBurstSheet;
#define RESOURCE_LIGHT_BURST_SHEET ((Sint32)&ResourceId_LightBurstSheet)
extern char ResourceId_CornerGlowSheet;
#define RESOURCE_CORNER_GLOW_SHEET ((Sint32)&ResourceId_CornerGlowSheet)
/* Drawn by Catastrophe, Fume, the Gasp family, Haures, Rising Dragon and the
   enemies' Rising Dragon. */
extern char ResourceId_SmokeBandSheet;
#define RESOURCE_SMOKE_BAND_SHEET ((Sint32)&ResourceId_SmokeBandSheet)
/* Drawn by Charon. */
extern char ResourceId_CrimsonBeastTiles;
#define RESOURCE_CRIMSON_BEAST_TILES ((Sint32)&ResourceId_CrimsonBeastTiles)
extern char ResourceId_RedDomeTiles;
#define RESOURCE_RED_DOME_TILES ((Sint32)&ResourceId_RedDomeTiles)
extern char ResourceId_RedOrbTiles;
#define RESOURCE_RED_ORB_TILES ((Sint32)&ResourceId_RedOrbTiles)
extern char ResourceId_TumblingShardTiles;
#define RESOURCE_TUMBLING_SHARD_TILES ((Sint32)&ResourceId_TumblingShardTiles)
extern char ResourceId_WhiteWispsTiles;
#define RESOURCE_WHITE_WISPS_TILES ((Sint32)&ResourceId_WhiteWispsTiles)
extern char ResourceId_RuneSealSheet;
#define RESOURCE_RUNE_SEAL_SHEET ((Sint32)&ResourceId_RuneSealSheet)
extern char ResourceId_CharmRingSheet;
#define RESOURCE_CHARM_RING_SHEET ((Sint32)&ResourceId_CharmRingSheet)
extern char ResourceId_VioletPaletteE;
#define RESOURCE_VIOLET_PALETTE_E ((Sint32)&ResourceId_VioletPaletteE)
extern char ResourceId_BoltSheet;
#define RESOURCE_BOLT_SHEET ((Sint32)&ResourceId_BoltSheet)
extern char ResourceId_CurseSealSheet;
#define RESOURCE_CURSE_SEAL_SHEET ((Sint32)&ResourceId_CurseSealSheet)
extern char ResourceId_GlowRingTexture;
#define RESOURCE_GLOW_RING_TEXTURE ((Sint32)&ResourceId_GlowRingTexture)
/* Drawn by Catastrophe, Fume, the leaping strike, Rising Dragon and the
   enemies' Rising Dragon. */
extern char ResourceId_ClawSlashSheet;
#define RESOURCE_CLAW_SLASH_SHEET ((Sint32)&ResourceId_ClawSlashSheet)
/* Drawn by Heartbreak. */
extern char ResourceId_HeartbreakSheet;
#define RESOURCE_HEARTBREAK_SHEET ((Sint32)&ResourceId_HeartbreakSheet)
/* Drawn by Raging Heat. */
extern char ResourceId_CornerRuneBSheet;
#define RESOURCE_CORNER_RUNE_B_SHEET ((Sint32)&ResourceId_CornerRuneBSheet)
/* Drawn by Juggle. */
extern char ResourceId_SmallFlameSheet;
#define RESOURCE_SMALL_FLAME_SHEET ((Sint32)&ResourceId_SmallFlameSheet)
/* Drawn by Flora. */
extern char ResourceId_GreenHairedMaidenTiles;
#define RESOURCE_GREEN_HAIRED_MAIDEN_TILES \
	((Sint32)&ResourceId_GreenHairedMaidenTiles)
/* Drawn by Flora and Iris. */
extern char ResourceId_PinkPetalsTiles;
#define RESOURCE_PINK_PETALS_TILES ((Sint32)&ResourceId_PinkPetalsTiles)
/* Drawn by Backstab and Helm Splitter. */
extern char ResourceId_PinkStarSheet;
#define RESOURCE_PINK_STAR_SHEET ((Sint32)&ResourceId_PinkStarSheet)
/* Drawn by Fire Puppet. */
extern char ResourceId_GhostSkullTexture;
#define RESOURCE_GHOST_SKULL_TEXTURE ((Sint32)&ResourceId_GhostSkullTexture)
/* Drawn by Whiplash. */
extern char ResourceId_SlashFlameSheet;
#define RESOURCE_SLASH_FLAME_SHEET ((Sint32)&ResourceId_SlashFlameSheet)
/* Drawn by Catastrophe, Megiddo, Odyssey and the enemies' Rising Dragon. */
extern char ResourceId_RedHazeBackdrop;
#define RESOURCE_RED_HAZE_BACKDROP ((Sint32)&ResourceId_RedHazeBackdrop)
/* Drawn by the rushing strike. */
extern char ResourceId_FlamePlumeSheet;
#define RESOURCE_FLAME_PLUME_SHEET ((Sint32)&ResourceId_FlamePlumeSheet)
/* Drawn by Dragon Cloud and the rushing strike. */
extern char ResourceId_FirePillarSheetB;
#define RESOURCE_FIRE_PILLAR_SHEET_B ((Sint32)&ResourceId_FirePillarSheetB)
/* Drawn by Daedalus. */
extern char ResourceId_BronzeAutomatonTiles;
#define RESOURCE_BRONZE_AUTOMATON_TILES \
	((Sint32)&ResourceId_BronzeAutomatonTiles)
extern char ResourceId_MissileTiles;
#define RESOURCE_MISSILE_TILES ((Sint32)&ResourceId_MissileTiles)
extern char ResourceId_RocketFlameSheet;
#define RESOURCE_ROCKET_FLAME_SHEET ((Sint32)&ResourceId_RocketFlameSheet)
extern char ResourceId_BirdFlockSheet;
#define RESOURCE_BIRD_FLOCK_SHEET ((Sint32)&ResourceId_BirdFlockSheet)
/* Drawn by Vengeance. */
extern char ResourceId_ArcherSheet;
#define RESOURCE_ARCHER_SHEET ((Sint32)&ResourceId_ArcherSheet)
/* Drawn by Life Shear. */
extern char ResourceId_LifeShearSheet;
#define RESOURCE_LIFE_SHEAR_SHEET ((Sint32)&ResourceId_LifeShearSheet)
/* Drawn by the geyser. */
extern char ResourceId_CrossTextureSheet;
#define RESOURCE_CROSS_TEXTURE_SHEET ((Sint32)&ResourceId_CrossTextureSheet)
/* Drawn by the bubble column and Coatlicue. */
extern char ResourceId_BubbleAnimationSheet;
#define RESOURCE_BUBBLE_ANIMATION_SHEET \
	((Sint32)&ResourceId_BubbleAnimationSheet)
/* Drawn by Coatlicue. */
extern char ResourceId_CoatlicueTiles;
#define RESOURCE_COATLICUE_TILES ((Sint32)&ResourceId_CoatlicueTiles)
extern char ResourceId_ScaleTexture;
#define RESOURCE_SCALE_TEXTURE ((Sint32)&ResourceId_ScaleTexture)
/* Drawn by Catastrophe. */
extern char ResourceId_ChainedDemonTiles;
#define RESOURCE_CHAINED_DEMON_TILES ((Sint32)&ResourceId_ChainedDemonTiles)
extern char ResourceId_TriangleSealTexture;
#define RESOURCE_TRIANGLE_SEAL_TEXTURE ((Sint32)&ResourceId_TriangleSealTexture)
extern char ResourceId_PentagonSealTexture;
#define RESOURCE_PENTAGON_SEAL_TEXTURE ((Sint32)&ResourceId_PentagonSealTexture)
/* Drawn by Catastrophe, the element cards and Trident. */
extern char ResourceId_JaggedStreakSheet;
#define RESOURCE_JAGGED_STREAK_SHEET ((Sint32)&ResourceId_JaggedStreakSheet)
/* Drawn by Catastrophe. */
extern char ResourceId_LightningTexture;
#define RESOURCE_LIGHTNING_TEXTURE ((Sint32)&ResourceId_LightningTexture)
extern char ResourceId_GoldenScalesTiles;
#define RESOURCE_GOLDEN_SCALES_TILES ((Sint32)&ResourceId_GoldenScalesTiles)
extern char ResourceId_PurpleArmourTiles;
#define RESOURCE_PURPLE_ARMOUR_TILES ((Sint32)&ResourceId_PurpleArmourTiles)
extern char ResourceId_WhiteSpikesTiles;
#define RESOURCE_WHITE_SPIKES_TILES ((Sint32)&ResourceId_WhiteSpikesTiles)
/* Drawn by Haures. */
extern char ResourceId_ChainTexture;
#define RESOURCE_CHAIN_TEXTURE ((Sint32)&ResourceId_ChainTexture)
/* Drawn by Helm Splitter. */
extern char ResourceId_FireSwirlSheet;
#define RESOURCE_FIRE_SWIRL_SHEET ((Sint32)&ResourceId_FireSwirlSheet)
/* Drawn by Dragon Cloud. */
extern char ResourceId_DragonCloudFlamePath;
#define RESOURCE_DRAGON_CLOUD_FLAME_PATH \
	((Sint32)&ResourceId_DragonCloudFlamePath)
/* Drawn by Legend. */
extern char ResourceId_BladeSheet;
#define RESOURCE_BLADE_SHEET ((Sint32)&ResourceId_BladeSheet)
/* Drawn by Legend, Megiddo and Odyssey. */
extern char ResourceId_FlameGradientTexture;
#define RESOURCE_FLAME_GRADIENT_TEXTURE \
	((Sint32)&ResourceId_FlameGradientTexture)
/* Drawn by the element cards. */
extern char ResourceId_VineCardSheet;
#define RESOURCE_VINE_CARD_SHEET ((Sint32)&ResourceId_VineCardSheet)
extern char ResourceId_DragonCardSheet;
#define RESOURCE_DRAGON_CARD_SHEET ((Sint32)&ResourceId_DragonCardSheet)
extern char ResourceId_SunCardSheet;
#define RESOURCE_SUN_CARD_SHEET ((Sint32)&ResourceId_SunCardSheet)
extern char ResourceId_BoltCardSheet;
#define RESOURCE_BOLT_CARD_SHEET ((Sint32)&ResourceId_BoltCardSheet)
/* Drawn by the Cards. */
extern char ResourceId_ButterflyCardSheet;
#define RESOURCE_BUTTERFLY_CARD_SHEET ((Sint32)&ResourceId_ButterflyCardSheet)
extern char ResourceId_SwordCardSheet;
#define RESOURCE_SWORD_CARD_SHEET ((Sint32)&ResourceId_SwordCardSheet)
extern char ResourceId_SkeletonCardSheet;
#define RESOURCE_SKELETON_CARD_SHEET ((Sint32)&ResourceId_SkeletonCardSheet)
/* Drawn by the breaths. */
extern char ResourceId_DustCloudSheet;
#define RESOURCE_DUST_CLOUD_SHEET ((Sint32)&ResourceId_DustCloudSheet)
/* Drawn by Trident. */
extern char ResourceId_FlameZigzagTexture;
#define RESOURCE_FLAME_ZIGZAG_TEXTURE ((Sint32)&ResourceId_FlameZigzagTexture)
/* Drawn by Meteor Blow. */
extern char ResourceId_GlowOrbSetSheet;
#define RESOURCE_GLOW_ORB_SET_SHEET ((Sint32)&ResourceId_GlowOrbSetSheet)
/* Drawn by Megiddo. */
extern char ResourceId_FireStripesTexture;
#define RESOURCE_FIRE_STRIPES_TEXTURE ((Sint32)&ResourceId_FireStripesTexture)
/* Drawn by Odyssey. */
extern char ResourceId_GiantSwordSheet;
#define RESOURCE_GIANT_SWORD_SHEET ((Sint32)&ResourceId_GiantSwordSheet)
extern char ResourceId_GiantSwordBladeSheet;
#define RESOURCE_GIANT_SWORD_BLADE_SHEET \
	((Sint32)&ResourceId_GiantSwordBladeSheet)
/* Drawn by the cloud swirl. */
extern char ResourceId_CloudSwirlSheet;
#define RESOURCE_CLOUD_SWIRL_SHEET ((Sint32)&ResourceId_CloudSwirlSheet)
/* Drawn by Iris. */
extern char ResourceId_SunsetBackdrop;
#define RESOURCE_SUNSET_BACKDROP ((Sint32)&ResourceId_SunsetBackdrop)
extern char ResourceId_StarfieldFlareBackdrop;
#define RESOURCE_STARFIELD_FLARE_BACKDROP \
	((Sint32)&ResourceId_StarfieldFlareBackdrop)
extern char ResourceId_RobedGoddessTiles;
#define RESOURCE_ROBED_GODDESS_TILES ((Sint32)&ResourceId_RobedGoddessTiles)

/* The skies a summon or Psynergy shows behind the battle. */

/* The sky of Atalanta, Boreas, Cruel Ruin, Daedalus, Flora, Judgment, Moloch
   and Procne; only the unused slideshow list names it
   (DisplayScroll_UnusedSlideResources). */
extern char ResourceId_VioletSkyBackdrop;
#define RESOURCE_VIOLET_SKY_BACKDROP ((Sint32)&ResourceId_VioletSkyBackdrop)
/* The sky of Neptune and Thor; the credits' slideshow shows it
   (DisplayScroll_SlideResources). */
extern char ResourceId_DuskCloudsBackdrop;
#define RESOURCE_DUSK_CLOUDS_BACKDROP ((Sint32)&ResourceId_DuskCloudsBackdrop)
/* The sky of Judgment; the credits' slideshow shows it
   (DisplayScroll_SlideResources). */
extern char ResourceId_BeachBackdrop;
#define RESOURCE_BEACH_BACKDROP ((Sint32)&ResourceId_BeachBackdrop)
/* The sky of Judgment and Kirin; only the unused slideshow list names it
   (DisplayScroll_UnusedSlideResources). */
extern char ResourceId_ForestBackdrop;
#define RESOURCE_FOREST_BACKDROP ((Sint32)&ResourceId_ForestBackdrop)
/* Drawn by Cruel Ruin. */
extern char ResourceId_SunbeamCloudsBackdrop;
#define RESOURCE_SUNBEAM_CLOUDS_BACKDROP \
	((Sint32)&ResourceId_SunbeamCloudsBackdrop)
/* The sky Eclipse opens on. */
extern char ResourceId_EarthHorizonBackdrop;
#define RESOURCE_EARTH_HORIZON_BACKDROP \
	((Sint32)&ResourceId_EarthHorizonBackdrop)
/* The land far below that Eclipse's blast lands on; Charon draws it too. */
extern char ResourceId_AerialViewBackdrop;
#define RESOURCE_AERIAL_VIEW_BACKDROP ((Sint32)&ResourceId_AerialViewBackdrop)

/* Battle backdrops: the places battles are fought in, The Broken Seal's that
   only the credits' slideshow shows, and the battle background's star field. */

/* The Colosso's backdrop, which the debug battle menu fights on
   (btDebugBattleRunMenu) and the credits' slideshow shows
   (DisplayScroll_SlideResources). */
extern char ResourceId_ColossoBackdrop;
#define RESOURCE_COLOSSO_BACKDROP ((Sint32)&ResourceId_ColossoBackdrop)
/* The desert's battle backdrop (fdBattleBackgroundSelectForEnvironment), and
   the sky of the Ramses summon. */
extern char ResourceId_DesertBackdrop;
#define RESOURCE_DESERT_BACKDROP ((Sint32)&ResourceId_DesertBackdrop)
/* Battle backdrops of The Broken Seal's places, which no battle in this game is
   fought on: the credits' slideshow shows those DisplayScroll_SlideResources
   lists, and only the unused list names the rest. */
extern char ResourceId_SuhallaGateBackdrop;
#define RESOURCE_SUHALLA_GATE_BACKDROP ((Sint32)&ResourceId_SuhallaGateBackdrop)
extern char ResourceId_PirateShipBackdrop;
#define RESOURCE_PIRATE_SHIP_BACKDROP ((Sint32)&ResourceId_PirateShipBackdrop)
extern char ResourceId_CrossboneIsleCaveBackdrop;
#define RESOURCE_CROSSBONE_ISLE_CAVE_BACKDROP \
	((Sint32)&ResourceId_CrossboneIsleCaveBackdrop)
extern char ResourceId_VenusLighthouseAerieStormBackdrop;
#define RESOURCE_VENUS_LIGHTHOUSE_AERIE_STORM_BACKDROP \
	((Sint32)&ResourceId_VenusLighthouseAerieStormBackdrop)
extern char ResourceId_VenusLighthouseAerieBackdrop;
#define RESOURCE_VENUS_LIGHTHOUSE_AERIE_BACKDROP \
	((Sint32)&ResourceId_VenusLighthouseAerieBackdrop)
extern char ResourceId_VenusLighthouseBackdrop;
#define RESOURCE_VENUS_LIGHTHOUSE_BACKDROP \
	((Sint32)&ResourceId_VenusLighthouseBackdrop)
extern char ResourceId_SuhallaDesertBackdrop;
#define RESOURCE_SUHALLA_DESERT_BACKDROP \
	((Sint32)&ResourceId_SuhallaDesertBackdrop)
extern char ResourceId_AltmillerCaveBackdrop;
#define RESOURCE_ALTMILLER_CAVE_BACKDROP \
	((Sint32)&ResourceId_AltmillerCaveBackdrop)
extern char ResourceId_TunnelRuinsBackdrop;
#define RESOURCE_TUNNEL_RUINS_BACKDROP ((Sint32)&ResourceId_TunnelRuinsBackdrop)
extern char ResourceId_BabiLighthouseBackdrop;
#define RESOURCE_BABI_LIGHTHOUSE_BACKDROP \
	((Sint32)&ResourceId_BabiLighthouseBackdrop)
extern char ResourceId_LamakanDesertBackdrop;
#define RESOURCE_LAMAKAN_DESERT_BACKDROP \
	((Sint32)&ResourceId_LamakanDesertBackdrop)
extern char ResourceId_MogallForestBackdrop;
#define RESOURCE_MOGALL_FOREST_BACKDROP \
	((Sint32)&ResourceId_MogallForestBackdrop)
extern char ResourceId_MercuryLighthouseAerieBackdrop;
#define RESOURCE_MERCURY_LIGHTHOUSE_AERIE_BACKDROP \
	((Sint32)&ResourceId_MercuryLighthouseAerieBackdrop)
extern char ResourceId_TolbiShipRoomsBackdrop;
#define RESOURCE_TOLBI_SHIP_ROOMS_BACKDROP \
	((Sint32)&ResourceId_TolbiShipRoomsBackdrop)
extern char ResourceId_ImilFuchinFallsBackdrop;
#define RESOURCE_IMIL_FUCHIN_FALLS_BACKDROP \
	((Sint32)&ResourceId_ImilFuchinFallsBackdrop)
extern char ResourceId_KolimaForestBackdrop;
#define RESOURCE_KOLIMA_FOREST_BACKDROP \
	((Sint32)&ResourceId_KolimaForestBackdrop)
extern char ResourceId_TretTreeRoomsBackdrop;
#define RESOURCE_TRET_TREE_ROOMS_BACKDROP \
	((Sint32)&ResourceId_TretTreeRoomsBackdrop)
extern char ResourceId_RoomsBackdrop;
#define RESOURCE_ROOMS_BACKDROP ((Sint32)&ResourceId_RoomsBackdrop)
extern char ResourceId_AltinPeakBackdrop;
#define RESOURCE_ALTIN_PEAK_BACKDROP ((Sint32)&ResourceId_AltinPeakBackdrop)
extern char ResourceId_TolbiShipDeckBackdrop;
#define RESOURCE_TOLBI_SHIP_DECK_BACKDROP \
	((Sint32)&ResourceId_TolbiShipDeckBackdrop)
extern char ResourceId_MercuryLighthouseBackdrop;
#define RESOURCE_MERCURY_LIGHTHOUSE_BACKDROP \
	((Sint32)&ResourceId_MercuryLighthouseBackdrop)
extern char ResourceId_ValeStormBackdrop;
#define RESOURCE_VALE_STORM_BACKDROP ((Sint32)&ResourceId_ValeStormBackdrop)
extern char ResourceId_TretTreeBranchBackdrop;
#define RESOURCE_TRET_TREE_BRANCH_BACKDROP \
	((Sint32)&ResourceId_TretTreeBranchBackdrop)
extern char ResourceId_SnowfieldBackdrop;
#define RESOURCE_SNOWFIELD_BACKDROP ((Sint32)&ResourceId_SnowfieldBackdrop)
extern char ResourceId_GrasslandBackdrop;
#define RESOURCE_GRASSLAND_BACKDROP ((Sint32)&ResourceId_GrasslandBackdrop)
extern char ResourceId_SolSanctumBackdrop;
#define RESOURCE_SOL_SANCTUM_BACKDROP ((Sint32)&ResourceId_SolSanctumBackdrop)
/* Loaded by btTransitionRun; listed in Scene_BattleBackgroundRules. */
extern char ResourceId_SerpentStarsBackdrop;
#define RESOURCE_SERPENT_STARS_BACKDROP \
	((Sint32)&ResourceId_SerpentStarsBackdrop)
/* The battle background's star field (btElementGlowInitStarField): its sprites,
   and its colours, which Mercury's star field keeps. */
extern char ResourceId_BlueRingSheet;
#define RESOURCE_BLUE_RING_SHEET ((Sint32)&ResourceId_BlueRingSheet)
/* The colours of Mars's star field. */
extern char ResourceId_RedPaletteC;
#define RESOURCE_RED_PALETTE_C ((Sint32)&ResourceId_RedPaletteC)
/* Drawn by Haures; listed in Scene_BattleBackgroundRules. */
extern char ResourceId_MossCaveBackdrop;
#define RESOURCE_MOSS_CAVE_BACKDROP ((Sint32)&ResourceId_MossCaveBackdrop)
/* The lava cave backdrop of the Valukar battle in the Yampi Desert Cave. */
extern char ResourceId_LavaCaveBackdrop;
#define RESOURCE_LAVA_CAVE_BACKDROP ((Sint32)&ResourceId_LavaCaveBackdrop)
/* The beast-wall backdrop of the Flame Dragons' battle in the Mars Star
   room. */
extern char ResourceId_BeastWallBackdrop;
#define RESOURCE_BEAST_WALL_BACKDROP ((Sint32)&ResourceId_BeastWallBackdrop)
/* Loaded by fdBattleBackgroundSelectForMap. */
extern char ResourceId_DragonHallABackdrop;
#define RESOURCE_DRAGON_HALL_A_BACKDROP \
	((Sint32)&ResourceId_DragonHallABackdrop)
/* Loaded by fdBattleBackgroundSelectForMap; listed in
   Scene_BattleBackgroundRules. */
extern char ResourceId_DragonHallBBackdrop;
#define RESOURCE_DRAGON_HALL_B_BACKDROP \
	((Sint32)&ResourceId_DragonHallBBackdrop)
/* Loaded by fdBattleBackgroundSelectForEnvironment. */
extern char ResourceId_GrasslandBBackdrop;
#define RESOURCE_GRASSLAND_B_BACKDROP ((Sint32)&ResourceId_GrasslandBBackdrop)
extern char ResourceId_RockyBeachBackdrop;
#define RESOURCE_ROCKY_BEACH_BACKDROP ((Sint32)&ResourceId_RockyBeachBackdrop)
/* Loaded by fdBattleBackgroundSelectForEnvironment; listed in
   Scene_BattleBackgroundRules. */
extern char ResourceId_DenseForestBackdrop;
#define RESOURCE_DENSE_FOREST_BACKDROP ((Sint32)&ResourceId_DenseForestBackdrop)
extern char ResourceId_ShipDeckBackdrop;
#define RESOURCE_SHIP_DECK_BACKDROP ((Sint32)&ResourceId_ShipDeckBackdrop)
/* Loaded by fdBattleBackgroundSelectForEnvironment. */
extern char ResourceId_SnowfieldBBackdrop;
#define RESOURCE_SNOWFIELD_B_BACKDROP ((Sint32)&ResourceId_SnowfieldBBackdrop)
extern char ResourceId_BlizzardBackdrop;
#define RESOURCE_BLIZZARD_BACKDROP ((Sint32)&ResourceId_BlizzardBackdrop)

/* The logos, the intro and the title screen. */

/* Shown first by mnTitleShowPublisherLogo. */
extern char ResourceId_PublisherLogo;
#define RESOURCE_PUBLISHER_LOGO ((Sint32)&ResourceId_PublisherLogo)
/* The Camelot logo mnTitleShowCamelotLogo shows after the publisher's. */
extern char ResourceId_CamelotLogo;
#define RESOURCE_CAMELOT_LOGO ((Sint32)&ResourceId_CamelotLogo)
/* The title's lettering sprites and their colours, which scTitleLoadSprs loads
   into sprite palette 15. */
extern char ResourceId_IntroTilesB;
#define RESOURCE_INTRO_TILES_B ((Sint32)&ResourceId_IntroTilesB)
/* Loaded by scClearLoadBackground and scTitleLoadBackground. */
extern char ResourceId_LogoOnBlack;
#define RESOURCE_LOGO_ON_BLACK ((Sint32)&ResourceId_LogoOnBlack)
/* Loaded by mnTitleIntroLoadPictureTiles. */
extern char ResourceId_IntroClouds;
#define RESOURCE_INTRO_CLOUDS ((Sint32)&ResourceId_IntroClouds)
/* Loaded by mnTitleIntroUpdateLogo. */
extern char ResourceId_SubtitlePalette;
#define RESOURCE_SUBTITLE_PALETTE ((Sint32)&ResourceId_SubtitlePalette)
/* Logo tiles mnTitleIntroUpdateLogo loads (LOAD_LOGO_TILES) as the logo
   assembles. */
extern char ResourceId_SubtitleA;
#define RESOURCE_SUBTITLE_A ((Sint32)&ResourceId_SubtitleA)
extern char ResourceId_SubtitleB;
#define RESOURCE_SUBTITLE_B ((Sint32)&ResourceId_SubtitleB)
extern char ResourceId_SubtitleC;
#define RESOURCE_SUBTITLE_C ((Sint32)&ResourceId_SubtitleC)
extern char ResourceId_SubtitleD;
#define RESOURCE_SUBTITLE_D ((Sint32)&ResourceId_SubtitleD)
/* Loaded by mnTitleIntroShowLighthouse. */
extern char ResourceId_IntroLighthousePicture;
#define RESOURCE_INTRO_LIGHTHOUSE_PICTURE \
	((Sint32)&ResourceId_IntroLighthousePicture)
extern char ResourceId_IntroGullSprites;
#define RESOURCE_INTRO_GULL_SPRITES ((Sint32)&ResourceId_IntroGullSprites)
/* Loaded by mnTitleShowTitleScreen. */
extern char ResourceId_TitleScreenPicture;
#define RESOURCE_TITLE_SCREEN_PICTURE ((Sint32)&ResourceId_TitleScreenPicture)

/* Windows, menus and icons. */

/* The window font and border tiles. syWindowLoadGraphics copies them into BG
   character block 0, and mnGlyphDraw draws its glyphs from here. */
extern char ResourceId_WindowTiles;
#define RESOURCE_WINDOW_TILES ((Sint32)&ResourceId_WindowTiles)
/* The command icons; an icon's number is also its name's in MsgCommandName
   (mnCommandIconLoadIntoSlot). */
extern char ResourceId_CommandIcons;
#define RESOURCE_COMMAND_ICONS ((Sint32)&ResourceId_CommandIcons)
/* Loaded by fdHeadingShow. */
extern char ResourceId_Headings;
#define RESOURCE_HEADINGS ((Sint32)&ResourceId_Headings)
/* Loaded by mnNameEntryEditMemberName and mnNameCaretLoadIntoSlot. */
extern char ResourceId_NameCaretTiles;
#define RESOURCE_NAME_CARET_TILES ((Sint32)&ResourceId_NameCaretTiles)
/* Loaded by mnIconCreateHandCursor, mnNameEntryEditMemberName,
   mnPointingHandLoadIntoSlot, mnPointingHandLoadIntoFreeSlot and
   mnShopInitState. */
extern char ResourceId_UiIconTiles;
#define RESOURCE_UI_ICON_TILES ((Sint32)&ResourceId_UiIconTiles)
/* Loaded by mnOpenBackdropScreen and mnShopInitState. */
extern char ResourceId_ShopGemTiles;
#define RESOURCE_SHOP_GEM_TILES ((Sint32)&ResourceId_ShopGemTiles)
/* Loaded by mnLoadDjinnArrows. */
extern char ResourceId_ItemMenuTilesA;
#define RESOURCE_ITEM_MENU_TILES_A ((Sint32)&ResourceId_ItemMenuTilesA)
extern char ResourceId_ItemMenuTilesB;
#define RESOURCE_ITEM_MENU_TILES_B ((Sint32)&ResourceId_ItemMenuTilesB)
/* The small up arrow beside a stat that equipping raises (mnLoadStatArrows and
   mnShopInitState). */
extern char ResourceId_MenuArrowTiles;
#define RESOURCE_MENU_ARROW_TILES ((Sint32)&ResourceId_MenuArrowTiles)
/* The small down arrow beside a stat that equipping lowers. */
extern char ResourceId_MenuSmallDownArrowTiles;
#define RESOURCE_MENU_SMALL_DOWN_ARROW_TILES \
	((Sint32)&ResourceId_MenuSmallDownArrowTiles)
/* Loaded by mnCursorCreate and mnShopInitState. */
extern char ResourceId_ShopUpArrowTiles;
#define RESOURCE_SHOP_UP_ARROW_TILES ((Sint32)&ResourceId_ShopUpArrowTiles)
extern char ResourceId_ShopDownArrowTiles;
#define RESOURCE_SHOP_DOWN_ARROW_TILES ((Sint32)&ResourceId_ShopDownArrowTiles)
/* Loaded by mnIconLoadWithPalette. */
extern char ResourceId_Icons;
#define RESOURCE_ICONS ((Sint32)&ResourceId_Icons)
/* Loaded by mnWindowShowFlashFrame and mnWindowStepSwellEffect. */
extern char ResourceId_WindowBorderFlash;
#define RESOURCE_WINDOW_BORDER_FLASH ((Sint32)&ResourceId_WindowBorderFlash)
/* Loaded by mnSettingsMenuAnimateCursors. */
extern char ResourceId_ShopCursorPictures;
#define RESOURCE_SHOP_CURSOR_PICTURES ((Sint32)&ResourceId_ShopCursorPictures)
/* Loaded by mnGlyphDraw. */
extern char ResourceId_SmallFont;
#define RESOURCE_SMALL_FONT ((Sint32)&ResourceId_SmallFont)
/* Loaded by fdMapScreenUpdateMarkers. */
extern char ResourceId_WorldMapPicture;
#define RESOURCE_WORLD_MAP_PICTURE ((Sint32)&ResourceId_WorldMapPicture)
extern char ResourceId_WorldMapPictureLarge;
#define RESOURCE_WORLD_MAP_PICTURE_LARGE \
	((Sint32)&ResourceId_WorldMapPictureLarge)

/* The World Map: its four graphics sets, its cells and its perspective view. */

/* The colours of the world map's first graphics set
   (WorldMap_GraphicsResources). */
extern char ResourceId_WorldMapPaletteA;
#define RESOURCE_WORLD_MAP_PALETTE_A ((Sint32)&ResourceId_WorldMapPaletteA)
/* The world map's cell tiles attributes in its first graphics set
   (WorldMap_GraphicsResources). */
extern char ResourceId_DefaultMetatileTilesAttributes;
#define RESOURCE_DEFAULT_METATILE_TILES_ATTRIBUTES \
	((Sint32)&ResourceId_DefaultMetatileTilesAttributes)
/* The colours of the world map's third graphics set
   (WorldMap_GraphicsResources). */
extern char ResourceId_WorldMapPaletteB;
#define RESOURCE_WORLD_MAP_PALETTE_B ((Sint32)&ResourceId_WorldMapPaletteB)
/* The world map's tile offsets and packed tiles, read by
   fdWorldMapInitPerspective. */
extern char ResourceId_PerspectiveDataA;
#define RESOURCE_PERSPECTIVE_DATA_A ((Sint32)&ResourceId_PerspectiveDataA)
/* Loaded by fdWorldMapInitPerspective, fdWorldMapLoadAreaGraphics and
   fdWorldMapLoadDefaultMetatilesAndUpdateBlock. */
extern char ResourceId_DefaultMapMetatiles;
#define RESOURCE_DEFAULT_MAP_METATILES ((Sint32)&ResourceId_DefaultMapMetatiles)
/* The tile animation of world map set A: a row of
   WorldMap_GraphicsResources. */
extern char ResourceId_DefaultMapAnimation;
#define RESOURCE_DEFAULT_MAP_ANIMATION ((Sint32)&ResourceId_DefaultMapAnimation)
/* Tiles 1 of 6 of world map set A, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapTilesA1;
#define RESOURCE_WORLD_MAP_TILES_A1 ((Sint32)&ResourceId_WorldMapTilesA1)
/* Tiles 2 of 6 of world map set A, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapTilesA2;
#define RESOURCE_WORLD_MAP_TILES_A2 ((Sint32)&ResourceId_WorldMapTilesA2)
/* Tiles 3 of 6 of world map set A, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapTilesA3;
#define RESOURCE_WORLD_MAP_TILES_A3 ((Sint32)&ResourceId_WorldMapTilesA3)
/* Tiles 4 of 6 of world map set A, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapTilesA4;
#define RESOURCE_WORLD_MAP_TILES_A4 ((Sint32)&ResourceId_WorldMapTilesA4)
/* Tiles 5 of 6 of world map set A, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapTilesA5;
#define RESOURCE_WORLD_MAP_TILES_A5 ((Sint32)&ResourceId_WorldMapTilesA5)
/* Tiles 6 of 6 of world map set A, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapTilesA6;
#define RESOURCE_WORLD_MAP_TILES_A6 ((Sint32)&ResourceId_WorldMapTilesA6)
/* The palette of world map set C, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapPaletteC;
#define RESOURCE_WORLD_MAP_PALETTE_C ((Sint32)&ResourceId_WorldMapPaletteC)
/* The tile animation of world map set C, in DefaultMapAnimation's format: a row
   of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapAnimationC;
#define RESOURCE_WORLD_MAP_ANIMATION_C ((Sint32)&ResourceId_WorldMapAnimationC)
/* The cell tiles attributes of world map set C, in
   DefaultMetatileTilesAttributes' format: a row of
   WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapMetatileTilesAttributesC;
#define RESOURCE_WORLD_MAP_METATILE_TILES_ATTRIBUTES_C \
	((Sint32)&ResourceId_WorldMapMetatileTilesAttributesC)
/* Tiles 1 of 6 of world map set C, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapTilesC1;
#define RESOURCE_WORLD_MAP_TILES_C1 ((Sint32)&ResourceId_WorldMapTilesC1)
/* Tiles 2 of 6 of world map set C, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapTilesC2;
#define RESOURCE_WORLD_MAP_TILES_C2 ((Sint32)&ResourceId_WorldMapTilesC2)
/* Tiles 3 of 6 of world map set C, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapTilesC3;
#define RESOURCE_WORLD_MAP_TILES_C3 ((Sint32)&ResourceId_WorldMapTilesC3)
/* Tiles 4 of 6 of world map set C, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapTilesC4;
#define RESOURCE_WORLD_MAP_TILES_C4 ((Sint32)&ResourceId_WorldMapTilesC4)
/* Tiles 5 of 6 of world map set C, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapTilesC5;
#define RESOURCE_WORLD_MAP_TILES_C5 ((Sint32)&ResourceId_WorldMapTilesC5)
/* Tiles 6 of 6 of world map set C, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapTilesC6;
#define RESOURCE_WORLD_MAP_TILES_C6 ((Sint32)&ResourceId_WorldMapTilesC6)
/* The tile animation of world map set B, in DefaultMapAnimation's format: a row
   of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapAnimationB;
#define RESOURCE_WORLD_MAP_ANIMATION_B ((Sint32)&ResourceId_WorldMapAnimationB)
/* The cell tiles attributes of world map set B, in
   DefaultMetatileTilesAttributes' format: a row of
   WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapMetatileTilesAttributesB;
#define RESOURCE_WORLD_MAP_METATILE_TILES_ATTRIBUTES_B \
	((Sint32)&ResourceId_WorldMapMetatileTilesAttributesB)
/* Tiles 1 of 6 of world map set B, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapTilesB1;
#define RESOURCE_WORLD_MAP_TILES_B1 ((Sint32)&ResourceId_WorldMapTilesB1)
/* Tiles 2 of 6 of world map set B, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapTilesB2;
#define RESOURCE_WORLD_MAP_TILES_B2 ((Sint32)&ResourceId_WorldMapTilesB2)
/* Tiles 3 of 6 of world map set B, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapTilesB3;
#define RESOURCE_WORLD_MAP_TILES_B3 ((Sint32)&ResourceId_WorldMapTilesB3)
/* Tiles 4 of 6 of world map set B, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapTilesB4;
#define RESOURCE_WORLD_MAP_TILES_B4 ((Sint32)&ResourceId_WorldMapTilesB4)
/* Tiles 5 of 6 of world map set B, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapTilesB5;
#define RESOURCE_WORLD_MAP_TILES_B5 ((Sint32)&ResourceId_WorldMapTilesB5)
/* Tiles 6 of 6 of world map set B, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapTilesB6;
#define RESOURCE_WORLD_MAP_TILES_B6 ((Sint32)&ResourceId_WorldMapTilesB6)
/* The palette of world map set D, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapPaletteD;
#define RESOURCE_WORLD_MAP_PALETTE_D ((Sint32)&ResourceId_WorldMapPaletteD)
/* The tile animation of world map set D, in DefaultMapAnimation's format: a row
   of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapAnimationD;
#define RESOURCE_WORLD_MAP_ANIMATION_D ((Sint32)&ResourceId_WorldMapAnimationD)
/* The cell tiles attributes of world map set D, in
   DefaultMetatileTilesAttributes' format: a row of
   WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapMetatileTilesAttributesD;
#define RESOURCE_WORLD_MAP_METATILE_TILES_ATTRIBUTES_D \
	((Sint32)&ResourceId_WorldMapMetatileTilesAttributesD)
/* Tiles 1 of 6 of world map set D, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapTilesD1;
#define RESOURCE_WORLD_MAP_TILES_D1 ((Sint32)&ResourceId_WorldMapTilesD1)
/* Tiles 2 of 6 of world map set D, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapTilesD2;
#define RESOURCE_WORLD_MAP_TILES_D2 ((Sint32)&ResourceId_WorldMapTilesD2)
/* Tiles 3 of 6 of world map set D, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapTilesD3;
#define RESOURCE_WORLD_MAP_TILES_D3 ((Sint32)&ResourceId_WorldMapTilesD3)
/* Tiles 4 of 6 of world map set D, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapTilesD4;
#define RESOURCE_WORLD_MAP_TILES_D4 ((Sint32)&ResourceId_WorldMapTilesD4)
/* Tiles 5 of 6 of world map set D, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapTilesD5;
#define RESOURCE_WORLD_MAP_TILES_D5 ((Sint32)&ResourceId_WorldMapTilesD5)
/* Tiles 6 of 6 of world map set D, a row of WorldMap_GraphicsResources. */
extern char ResourceId_WorldMapTilesD6;
#define RESOURCE_WORLD_MAP_TILES_D6 ((Sint32)&ResourceId_WorldMapTilesD6)
/* Loaded by fdWorldMapInitPerspective. */
extern char ResourceId_DefaultMapEntrances;
#define RESOURCE_DEFAULT_MAP_ENTRANCES ((Sint32)&ResourceId_DefaultMapEntrances)

/* Field effects: the field Psynergy and the scenes' own effects. */

/* Loaded by fdParticleBurstStart. */
extern char ResourceId_Leaves;
#define RESOURCE_LEAVES ((Sint32)&ResourceId_Leaves)
/* Loaded by fdActorAppearInSparks, fdPsynergyEndRing and fdPsynergyLash. */
extern char ResourceId_SparkleFade;
#define RESOURCE_SPARKLE_FADE ((Sint32)&ResourceId_SparkleFade)
/* Loaded by scGabombaStatueStartClock. */
extern char ResourceId_ClockDigits;
#define RESOURCE_CLOCK_DIGITS ((Sint32)&ResourceId_ClockDigits)
/* Loaded by scAirsRockWhirlwindClearsFog, scAirsRockWhirlwindClearsWall and
   scShamanVillageBlowAwaySand. */
extern char ResourceId_WhirlwindMap;
#define RESOURCE_WHIRLWIND_MAP ((Sint32)&ResourceId_WhirlwindMap)
extern char ResourceId_WhirlwindTiles;
#define RESOURCE_WHIRLWIND_TILES ((Sint32)&ResourceId_WhirlwindTiles)
/* Whirlwind's picture is drawn on BG0, the windows' layer: its tiles, two rows
   of WHIRLWIND_ROW_TILES (the top halves, then the bottom ones), go over BG
   tiles WHIRLWIND_TOP_TILE and WHIRLWIND_BOTTOM_TILE of character block 3, and
   its map, WHIRLWIND_MAP_WIDTH by WHIRLWIND_MAP_HEIGHT cells numbering them
   from WHIRLWIND_MAP_FIRST_TILE, into the windows' screen block, whose other
   entries take WHIRLWIND_FILL_TILE. */
#define WHIRLWIND_ROW_TILES		 26
#define WHIRLWIND_TOP_TILE		 320
#define WHIRLWIND_BOTTOM_TILE	 352
#define WHIRLWIND_MAP_FIRST_TILE 512
#define WHIRLWIND_MAP_WIDTH		 16
#define WHIRLWIND_MAP_HEIGHT	 18
#define WHIRLWIND_FILL_TILE		 16
/* Whirlwind's sway (field/field_effect_wave_scroll.c): each line of its picture
   swings by half its height above WAVE_SCROLL_CALM_LINE, the picture's foot,
   and a line swung past WAVE_SCROLL_MAX pixels one way or WAVE_SCROLL_MIN the
   other is pulled back by as much. */
#define WAVE_SCROLL_CALM_LINE	 (WHIRLWIND_MAP_HEIGHT * 8)
#define WAVE_SCROLL_MAX			 56
#define WAVE_SCROLL_MIN			 (-64)
/* Loaded by scAirsRockInsideWakeHallStatue,
   scAirsRockInsideWakeVialChestStatue, scAirsRockWhirlwindOpensTheTop and
   scJupiterLighthouseFlashSky. */
extern char ResourceId_LightningMap;
#define RESOURCE_LIGHTNING_MAP ((Sint32)&ResourceId_LightningMap)
extern char ResourceId_LightningTiles;
#define RESOURCE_LIGHTNING_TILES ((Sint32)&ResourceId_LightningTiles)
/* Loaded by scChampaCliffCavesStartFlash, fdPsynergyBurst, scLohoFireCannon and
   scNorthernReachesShatterIceWall. */
extern char ResourceId_FieldEffectExplosion;
#define RESOURCE_FIELD_EFFECT_EXPLOSION \
	((Sint32)&ResourceId_FieldEffectExplosion)
/* Loaded by ChampaCliffCaves_LoadDisc, fdPsynergyBurst, fdPsynergyParch and
   fdPsynergyPound. */
extern char ResourceId_GlowDisc;
#define RESOURCE_GLOW_DISC ((Sint32)&ResourceId_GlowDisc)
/* Loaded by scNorthernReachesShatterIceWall. */
extern char ResourceId_FieldEffectIceShard;
#define RESOURCE_FIELD_EFFECT_ICE_SHARD \
	((Sint32)&ResourceId_FieldEffectIceShard)
/* Loaded by scEasternAlhafraStartFallingRocks,
   scMagmaRockInsideStartFallingRocks, scMagmaRockStartFallingRocks,
   scProxStartFallingRocks, dbPsynergyTest2StartRocks and
   scTundariaTowerStartFallingRocks. */
extern char ResourceId_FallingRock;
#define RESOURCE_FALLING_ROCK ((Sint32)&ResourceId_FallingRock)
/* Loaded by fdPsynergyBlaze, fdPsynergyEndSand, fdPsynergySand, fdPsynergyScoop
   and scVenusLighthouseEntranceStartDust. */
extern char ResourceId_DustPuffA;
#define RESOURCE_DUST_PUFF_A ((Sint32)&ResourceId_DustPuffA)
/* Loaded by scJupiterLighthouseStartSparkRing, fdTeleportPlaySparks and
   fdTeleportPlayWhirl. */
extern char ResourceId_ColouredOrbs;
#define RESOURCE_COLOURED_ORBS ((Sint32)&ResourceId_ColouredOrbs)
/* Loaded by fdPsynergyLash, fdPsynergyRing and scJupiterLighthouseStartBeam. */
extern char ResourceId_StarSparkleFade;
#define RESOURCE_STAR_SPARKLE_FADE ((Sint32)&ResourceId_StarSparkleFade)
/* Loaded by fdPsynergyGrindOnTarget. */
extern char ResourceId_WaterSplash;
#define RESOURCE_WATER_SPLASH ((Sint32)&ResourceId_WaterSplash)
/* Loaded by fdPsynergyHover. */
extern char ResourceId_RingOrb;
#define RESOURCE_RING_ORB ((Sint32)&ResourceId_RingOrb)
/* The two 8x8 tiles of the sprites Tremor shakes (FieldEffect_ShakeTiles),
   loaded by fdPsynergyTremorOnTarget. */
extern char ResourceId_ShakeTiles;
#define RESOURCE_SHAKE_TILES ((Sint32)&ResourceId_ShakeTiles)
/* Loaded by scChampaCliffCavesLoadColumn. */
extern char ResourceId_FireColumn;
#define RESOURCE_FIRE_COLUMN ((Sint32)&ResourceId_FireColumn)
/* Loaded by scJupiterLighthouseStartBeam. */
extern char ResourceId_GoldenTrident;
#define RESOURCE_GOLDEN_TRIDENT ((Sint32)&ResourceId_GoldenTrident)
/* Loaded by scShamanVillageLoadLettering. */
extern char ResourceId_LetteringBanner;
#define RESOURCE_LETTERING_BANNER ((Sint32)&ResourceId_LetteringBanner)
/* Loaded by fdPsynergyBlaze. */
extern char ResourceId_RedFlameBurst;
#define RESOURCE_RED_FLAME_BURST ((Sint32)&ResourceId_RedFlameBurst)
extern char ResourceId_VioletFlameBurst;
#define RESOURCE_VIOLET_FLAME_BURST ((Sint32)&ResourceId_VioletFlameBurst)
extern char ResourceId_YellowFlameBurst;
#define RESOURCE_YELLOW_FLAME_BURST ((Sint32)&ResourceId_YellowFlameBurst)
extern char ResourceId_BlueFlameBurst;
#define RESOURCE_BLUE_FLAME_BURST ((Sint32)&ResourceId_BlueFlameBurst)
/* Loaded by fdPsynergyParch. */
extern char ResourceId_WhitePuff;
#define RESOURCE_WHITE_PUFF ((Sint32)&ResourceId_WhitePuff)
/* Loaded by fdPsynergyPound. */
extern char ResourceId_GoldStar;
#define RESOURCE_GOLD_STAR ((Sint32)&ResourceId_GoldStar)
/* Loaded by fdPsynergyArrow. */
extern char ResourceId_BowDraw;
#define RESOURCE_BOW_DRAW ((Sint32)&ResourceId_BowDraw)
extern char ResourceId_FlamingArrow;
#define RESOURCE_FLAMING_ARROW ((Sint32)&ResourceId_FlamingArrow)
/* Loaded by fdPsynergyScoop. */
extern char ResourceId_DustPuffB;
#define RESOURCE_DUST_PUFF_B ((Sint32)&ResourceId_DustPuffB)
extern char ResourceId_WhiteSparkle;
#define RESOURCE_WHITE_SPARKLE ((Sint32)&ResourceId_WhiteSparkle)
extern char ResourceId_ScoopEarth;
#define RESOURCE_SCOOP_EARTH ((Sint32)&ResourceId_ScoopEarth)
/* Loaded by fdSummonTabletPlayRunes. */
extern char ResourceId_Runes;
#define RESOURCE_RUNES ((Sint32)&ResourceId_Runes)
extern char ResourceId_Pebbles;
#define RESOURCE_PEBBLES ((Sint32)&ResourceId_Pebbles)
/* Loaded by fdPsynergyRing. */
extern char ResourceId_RingArcs;
#define RESOURCE_RING_ARCS ((Sint32)&ResourceId_RingArcs)
extern char ResourceId_RingWaves;
#define RESOURCE_RING_WAVES ((Sint32)&ResourceId_RingWaves)

/* Lucky Dice, Lucky Wheels and Super Lucky Dice. */

/* The Lucky Dice board, with its colours, on the background
   (fdLuckyDiceRun). */
extern char ResourceId_LuckyDiceBoard;
#define RESOURCE_LUCKY_DICE_BOARD ((Sint32)&ResourceId_LuckyDiceBoard)
/* The Lucky Dice sprites and their colours (fdLuckyDiceRun). */
extern char ResourceId_LuckyDiceSprites;
#define RESOURCE_LUCKY_DICE_SPRITES ((Sint32)&ResourceId_LuckyDiceSprites)
/* Loaded by fdLuckyWheelsRun. */
extern char ResourceId_SlotsMachinePicture;
#define RESOURCE_SLOTS_MACHINE_PICTURE ((Sint32)&ResourceId_SlotsMachinePicture)
extern char ResourceId_SlotsIcons;
#define RESOURCE_SLOTS_ICONS ((Sint32)&ResourceId_SlotsIcons)
extern char ResourceId_SlotsJackpotBanner;
#define RESOURCE_SLOTS_JACKPOT_BANNER ((Sint32)&ResourceId_SlotsJackpotBanner)
/* Super Lucky Dice's dome, its dice and its digits and banners
   (fdSuperLuckyDiceRun). */
extern char ResourceId_SlotsDomePicture;
#define RESOURCE_SLOTS_DOME_PICTURE ((Sint32)&ResourceId_SlotsDomePicture)
extern char ResourceId_DiceFaces;
#define RESOURCE_DICE_FACES ((Sint32)&ResourceId_DiceFaces)
extern char ResourceId_SlotsSmallDoubleUpBanner;
#define RESOURCE_SLOTS_SMALL_DOUBLE_UP_BANNER \
	((Sint32)&ResourceId_SlotsSmallDoubleUpBanner)
extern char ResourceId_SlotsFinalGameBanner;
#define RESOURCE_SLOTS_FINAL_GAME_BANNER \
	((Sint32)&ResourceId_SlotsFinalGameBanner)
extern char ResourceId_SlotsOrangeDigits;
#define RESOURCE_SLOTS_ORANGE_DIGITS ((Sint32)&ResourceId_SlotsOrangeDigits)
extern char ResourceId_SlotsTimesSign;
#define RESOURCE_SLOTS_TIMES_SIGN ((Sint32)&ResourceId_SlotsTimesSign)
extern char ResourceId_SlotsPinkDigits;
#define RESOURCE_SLOTS_PINK_DIGITS ((Sint32)&ResourceId_SlotsPinkDigits)
extern char ResourceId_SlotsHigherBanner;
#define RESOURCE_SLOTS_HIGHER_BANNER ((Sint32)&ResourceId_SlotsHigherBanner)
extern char ResourceId_SlotsLowerBanner;
#define RESOURCE_SLOTS_LOWER_BANNER ((Sint32)&ResourceId_SlotsLowerBanner)
extern char ResourceId_SlotsYouLoseBanner;
#define RESOURCE_SLOTS_YOU_LOSE_BANNER ((Sint32)&ResourceId_SlotsYouLoseBanner)
extern char ResourceId_SlotsYouWinBanner;
#define RESOURCE_SLOTS_YOU_WIN_BANNER ((Sint32)&ResourceId_SlotsYouWinBanner)
extern char ResourceId_SlotsOnePairBanner;
#define RESOURCE_SLOTS_ONE_PAIR_BANNER ((Sint32)&ResourceId_SlotsOnePairBanner)
extern char ResourceId_SlotsTwoPairBanner;
#define RESOURCE_SLOTS_TWO_PAIR_BANNER ((Sint32)&ResourceId_SlotsTwoPairBanner)
extern char ResourceId_SlotsTripleBanner;
#define RESOURCE_SLOTS_TRIPLE_BANNER ((Sint32)&ResourceId_SlotsTripleBanner)
extern char ResourceId_SlotsPerfectBanner;
#define RESOURCE_SLOTS_PERFECT_BANNER ((Sint32)&ResourceId_SlotsPerfectBanner)
extern char ResourceId_SlotsGreenDigits;
#define RESOURCE_SLOTS_GREEN_DIGITS ((Sint32)&ResourceId_SlotsGreenDigits)
extern char ResourceId_SlotsDoubleUpBanner;
#define RESOURCE_SLOTS_DOUBLE_UP_BANNER \
	((Sint32)&ResourceId_SlotsDoubleUpBanner)
extern char ResourceId_SlotsCashOutBanner;
#define RESOURCE_SLOTS_CASH_OUT_BANNER ((Sint32)&ResourceId_SlotsCashOutBanner)

/* The ending and the credits. */

/* The green panel inside the epilogue's portrait frame
   (Epilogue_LoadNamePanel). */
extern char ResourceId_GreenPanel;
#define RESOURCE_GREEN_PANEL ((Sint32)&ResourceId_GreenPanel)
/* The ending's pictures, each 512 pixels wide, which the epilogue pans across
   under its lines (mnEndingImageShow). The first: three men on a cliff above
   the plain by day. */
extern char ResourceId_EndingCliffDayPicture;
#define RESOURCE_ENDING_CLIFF_DAY_PICTURE \
	((Sint32)&ResourceId_EndingCliffDayPicture)
/* The three on the cliff at sunset. */
extern char ResourceId_EndingCliffSunsetPicture;
#define RESOURCE_ENDING_CLIFF_SUNSET_PICTURE \
	((Sint32)&ResourceId_EndingCliffSunsetPicture)
/* The villagers among autumn trees, Dora alone on the right. */
extern char ResourceId_EndingVillagersPictureA;
#define RESOURCE_ENDING_VILLAGERS_PICTURE_A \
	((Sint32)&ResourceId_EndingVillagersPictureA)
/* The embrace, held until a button is pressed. */
extern char ResourceId_EndingEmbracePicture;
#define RESOURCE_ENDING_EMBRACE_PICTURE \
	((Sint32)&ResourceId_EndingEmbracePicture)
/* The party together, panned along. */
extern char ResourceId_EndingReunionPicture;
#define RESOURCE_ENDING_REUNION_PICTURE \
	((Sint32)&ResourceId_EndingReunionPicture)
/* The plain at sunset. */
extern char ResourceId_EndingSunsetPlainPicture;
#define RESOURCE_ENDING_SUNSET_PLAIN_PICTURE \
	((Sint32)&ResourceId_EndingSunsetPlainPicture)
/* The plain at sunset, with "The End" in each language. */
extern char ResourceId_EndingTheEndPicture;
#define RESOURCE_ENDING_THE_END_PICTURE \
	((Sint32)&ResourceId_EndingTheEndPicture)

/* Data that is no picture. */

/* The build's date and time as text, different in each edition;
   syCopyBuildStamp copies it into BuildStampWork. */
extern char ResourceId_BuildStamp;
#define RESOURCE_BUILD_STAMP ((Sint32)&ResourceId_BuildStamp)
/* Stub tables, which the title, the credits and the dice and wheel games hand
   to syResourceReservedNoOperation, which ignores them. */
extern char ResourceId_TitleStubs;
#define RESOURCE_TITLE_STUBS ((Sint32)&ResourceId_TitleStubs)
extern char ResourceId_CreditsStubs;
#define RESOURCE_CREDITS_STUBS ((Sint32)&ResourceId_CreditsStubs)
extern char ResourceId_BattleEffectStubs;
#define RESOURCE_BATTLE_EFFECT_STUBS ((Sint32)&ResourceId_BattleEffectStubs)
/* The sprites' animation data, which the debug sprite viewer passes to a stub
   (fdSprViewerRun). */
extern char ResourceId_SpriteMetadata;
#define RESOURCE_SPRITE_METADATA ((Sint32)&ResourceId_SpriteMetadata)
/* Loaded by fdTileMapConvertBuffer. */
extern char ResourceId_EmptyMap;
#define RESOURCE_EMPTY_MAP ((Sint32)&ResourceId_EmptyMap)

/* The battle backdrops that Scene_BattleBackgroundRules chooses, named by their
   pictures. */
extern char ResourceId_CanyonMeadowBackdrop;
#define RESOURCE_CANYON_MEADOW_BACKDROP \
	((Sint32)&ResourceId_CanyonMeadowBackdrop)
extern char ResourceId_FireCavePoolBackdrop;
#define RESOURCE_FIRE_CAVE_POOL_BACKDROP \
	((Sint32)&ResourceId_FireCavePoolBackdrop)
extern char ResourceId_BlueCaveBackdrop;
#define RESOURCE_BLUE_CAVE_BACKDROP ((Sint32)&ResourceId_BlueCaveBackdrop)
extern char ResourceId_RedMesaBackdrop;
#define RESOURCE_RED_MESA_BACKDROP ((Sint32)&ResourceId_RedMesaBackdrop)
extern char ResourceId_PinkRocksBackdrop;
#define RESOURCE_PINK_ROCKS_BACKDROP ((Sint32)&ResourceId_PinkRocksBackdrop)
extern char ResourceId_PinkTotemBackdrop;
#define RESOURCE_PINK_TOTEM_BACKDROP ((Sint32)&ResourceId_PinkTotemBackdrop)
extern char ResourceId_StoneTownBackdrop;
#define RESOURCE_STONE_TOWN_BACKDROP ((Sint32)&ResourceId_StoneTownBackdrop)
extern char ResourceId_PalmCliffBackdrop;
#define RESOURCE_PALM_CLIFF_BACKDROP ((Sint32)&ResourceId_PalmCliffBackdrop)
extern char ResourceId_ToyRoomBackdrop;
#define RESOURCE_TOY_ROOM_BACKDROP ((Sint32)&ResourceId_ToyRoomBackdrop)
extern char ResourceId_ShipCabinBackdrop;
#define RESOURCE_SHIP_CABIN_BACKDROP ((Sint32)&ResourceId_ShipCabinBackdrop)
extern char ResourceId_GoldRocksBackdrop;
#define RESOURCE_GOLD_ROCKS_BACKDROP ((Sint32)&ResourceId_GoldRocksBackdrop)
extern char ResourceId_StoneCorridorBackdrop;
#define RESOURCE_STONE_CORRIDOR_BACKDROP \
	((Sint32)&ResourceId_StoneCorridorBackdrop)
extern char ResourceId_FaceTempleBackdrop;
#define RESOURCE_FACE_TEMPLE_BACKDROP ((Sint32)&ResourceId_FaceTempleBackdrop)
extern char ResourceId_MistyHillsBackdrop;
#define RESOURCE_MISTY_HILLS_BACKDROP ((Sint32)&ResourceId_MistyHillsBackdrop)
extern char ResourceId_WaterfallGorgeBackdrop;
#define RESOURCE_WATERFALL_GORGE_BACKDROP \
	((Sint32)&ResourceId_WaterfallGorgeBackdrop)
extern char ResourceId_BlueRuinsBackdrop;
#define RESOURCE_BLUE_RUINS_BACKDROP ((Sint32)&ResourceId_BlueRuinsBackdrop)
extern char ResourceId_GreenRuinsBackdrop;
#define RESOURCE_GREEN_RUINS_BACKDROP ((Sint32)&ResourceId_GreenRuinsBackdrop)
extern char ResourceId_StatueCliffBackdrop;
#define RESOURCE_STATUE_CLIFF_BACKDROP ((Sint32)&ResourceId_StatueCliffBackdrop)
extern char ResourceId_LavaTempleBackdrop;
#define RESOURCE_LAVA_TEMPLE_BACKDROP ((Sint32)&ResourceId_LavaTempleBackdrop)
extern char ResourceId_ArrowBeachBackdrop;
#define RESOURCE_ARROW_BEACH_BACKDROP ((Sint32)&ResourceId_ArrowBeachBackdrop)
extern char ResourceId_StormySeaBackdrop;
#define RESOURCE_STORMY_SEA_BACKDROP ((Sint32)&ResourceId_StormySeaBackdrop)
extern char ResourceId_AngelHallBackdrop;
#define RESOURCE_ANGEL_HALL_BACKDROP ((Sint32)&ResourceId_AngelHallBackdrop)
extern char ResourceId_PurpleLighthouseBackdrop;
#define RESOURCE_PURPLE_LIGHTHOUSE_BACKDROP \
	((Sint32)&ResourceId_PurpleLighthouseBackdrop)
extern char ResourceId_StormyLighthouseBackdrop;
#define RESOURCE_STORMY_LIGHTHOUSE_BACKDROP \
	((Sint32)&ResourceId_StormyLighthouseBackdrop)
extern char ResourceId_IceHallBackdrop;
#define RESOURCE_ICE_HALL_BACKDROP ((Sint32)&ResourceId_IceHallBackdrop)
extern char ResourceId_OrbAltarBackdrop;
#define RESOURCE_ORB_ALTAR_BACKDROP ((Sint32)&ResourceId_OrbAltarBackdrop)
extern char ResourceId_PurpleCaveBackdrop;
#define RESOURCE_PURPLE_CAVE_BACKDROP ((Sint32)&ResourceId_PurpleCaveBackdrop)
extern char ResourceId_GorgePoolBackdrop;
#define RESOURCE_GORGE_POOL_BACKDROP ((Sint32)&ResourceId_GorgePoolBackdrop)
extern char ResourceId_SpikeHallBackdrop;
#define RESOURCE_SPIKE_HALL_BACKDROP ((Sint32)&ResourceId_SpikeHallBackdrop)

/* Moapa's recorded runs through the Trial Road's stages, in
   TrialRoad_RivalRecordings' order, which scTrialRoadPlayRivalRecording
   plays. */
extern char ResourceId_TrialRoadPillarRecording; /* the pillar stage */
#define RESOURCE_TRIAL_ROAD_PILLAR_RECORDING \
	((Sint32)&ResourceId_TrialRoadPillarRecording)
extern char ResourceId_TrialRoadTorchRecording; /* the torch stage */
#define RESOURCE_TRIAL_ROAD_TORCH_RECORDING \
	((Sint32)&ResourceId_TrialRoadTorchRecording)
extern char ResourceId_TrialRoadWeedsRecording; /* the weeds stage */
#define RESOURCE_TRIAL_ROAD_WEEDS_RECORDING \
	((Sint32)&ResourceId_TrialRoadWeedsRecording)
extern char ResourceId_TrialRoadPoundRecording; /* the Pound stage */
#define RESOURCE_TRIAL_ROAD_POUND_RECORDING \
	((Sint32)&ResourceId_TrialRoadPoundRecording)
extern char ResourceId_TrialRoadCrumblingRecording; /* the crumbling stage */
#define RESOURCE_TRIAL_ROAD_CRUMBLING_RECORDING \
	((Sint32)&ResourceId_TrialRoadCrumblingRecording)
extern char ResourceId_TrialRoadLogRecording; /* the log stage */
#define RESOURCE_TRIAL_ROAD_LOG_RECORDING \
	((Sint32)&ResourceId_TrialRoadLogRecording)
extern char ResourceId_TrialRoadFountainRecording; /* the fountain stage */
#define RESOURCE_TRIAL_ROAD_FOUNTAIN_RECORDING \
	((Sint32)&ResourceId_TrialRoadFountainRecording)
extern char ResourceId_TrialRoadSandRecording; /* the sand stage */
#define RESOURCE_TRIAL_ROAD_SAND_RECORDING \
	((Sint32)&ResourceId_TrialRoadSandRecording)
/* the chests, Moapa at the switch first */
extern char ResourceId_TrialRoadChestsMoapaFirstRecording;
#define RESOURCE_TRIAL_ROAD_CHESTS_MOAPA_FIRST_RECORDING \
	((Sint32)&ResourceId_TrialRoadChestsMoapaFirstRecording)
/* the chests, Felix at the switch first */
extern char ResourceId_TrialRoadChestsFelixFirstRecording;
#define RESOURCE_TRIAL_ROAD_CHESTS_FELIX_FIRST_RECORDING \
	((Sint32)&ResourceId_TrialRoadChestsFelixFirstRecording)

#endif
