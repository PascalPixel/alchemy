#ifndef ALCHEMY_RESOURCE_IDS_H
#define ALCHEMY_RESOURCE_IDS_H

// Resource numbers, named where their contents are known, as The Broken Seal's resource_ids.h.
//
// Each ResourceId is an absolute symbol whose value is the resource's row in
// syResourceDirectoryTable (system/resource_directory.s); C names the number by its macro,
// RESOURCE_RUNE_SHEET for ResourceIdRuneSheet.

#include "types.h"

// Battle effects: the sprite sheets, textures and colours the Psynergy, summons, Djinn and attacks
// draw, each with the effects that draw it.

// Drawn by Bind, the casting impacts, Rime, Spider Web, the stars of Astral Blast and Shining Star,
// Thunder Mine and Whorl.
extern char ResourceIdRuneSheet;
#define RESOURCE_RUNE_SHEET ((Sint32)&ResourceIdRuneSheet)
// Drawn by Bind, Charon, Cruel Ruin, the shards of Ice and Douse, Lucky Wheels, Neptune, the effect
// position helpers, Spider Web, the stat changes and the monster strikes.
extern char ResourceIdBlueGlowSheet;
#define RESOURCE_BLUE_GLOW_SHEET ((Sint32)&ResourceIdBlueGlowSheet)
// Drawn by Bind, Gasp, the healing motes, Lucky Wheels, Rime, the stat changes, Thor and Vengeance.
extern char ResourceIdJupiterDjinnSheet;
#define RESOURCE_JUPITER_DJINN_SHEET ((Sint32)&ResourceIdJupiterDjinnSheet)
// Shared by the burst showers, the canvas layer, the casting impacts, the charge of Core and
// Cannon, the glowing lunge, the rushing strike and the monster strikes, and by Catastrophe,
// Charon, Cruel Ruin, Daedalus, Death Plunge, Dragon Cloud, Fume, Legend, Liquifier, Meteor Blow,
// Moloch, Ocean Fist, Plasma, Procne, Raging Heat, Ragnarok, Thunder Mine, Trident, Undead Sword
// and Whorl.
extern char ResourceIdFlashBurstSheet;
#define RESOURCE_FLASH_BURST_SHEET ((Sint32)&ResourceIdFlashBurstSheet)
// Colours shared by the breaths, the burst showers, the canvas layer, the casting impacts, the
// charge of Core and Cannon, the glowing lunge, the healing motes, the rushing strike, the monster
// strikes and the Zagan summon, and by Daedalus, Dragon Cloud, Gasp, Geode, Haures, Heavy Divide,
// Legend, Liquifier, Megiddo, Meteor Blow, Nirvana, Plasma, Raging Heat, Thunder Mine, Trident,
// Vengeance and Volcano.
extern char ResourceIdOrangePaletteB;
#define RESOURCE_ORANGE_PALETTE_B ((Sint32)&ResourceIdOrangePaletteB)
// Drawn by the Cards, the casting impacts and Condemn.
extern char ResourceIdCounterRevealSheet;
#define RESOURCE_COUNTER_REVEAL_SHEET ((Sint32)&ResourceIdCounterRevealSheet)
extern char ResourceIdRedCrescentSheetA;
#define RESOURCE_RED_CRESCENT_SHEET_A ((Sint32)&ResourceIdRedCrescentSheetA)
// Drawn by Cruel Ruin.
extern char ResourceIdShatteringOpeningSheet;
#define RESOURCE_SHATTERING_OPENING_SHEET ((Sint32)&ResourceIdShatteringOpeningSheet)
extern char ResourceIdCyanSparkSheet;
#define RESOURCE_CYAN_SPARK_SHEET ((Sint32)&ResourceIdCyanSparkSheet)
// Colours for Cruel Ruin and the monster strikes.
extern char ResourceIdYellowPaletteA;
#define RESOURCE_YELLOW_PALETTE_A ((Sint32)&ResourceIdYellowPaletteA)
// Drawn by Cruel Ruin and the monster strikes.
extern char ResourceIdIceTileSheet;
#define RESOURCE_ICE_TILE_SHEET ((Sint32)&ResourceIdIceTileSheet)
// Colours for Cruel Ruin and the monster strikes.
extern char ResourceIdRedPaletteA;
#define RESOURCE_RED_PALETTE_A ((Sint32)&ResourceIdRedPaletteA)
extern char ResourceIdVioletPaletteA;
#define RESOURCE_VIOLET_PALETTE_A ((Sint32)&ResourceIdVioletPaletteA)
// Colours for the battle background's star field (btElementGlowInitStarField) and Cruel Ruin.
extern char ResourceIdYellowPaletteB;
#define RESOURCE_YELLOW_PALETTE_B ((Sint32)&ResourceIdYellowPaletteB)
// Drawn by the casting impacts, Cruel Ruin, Legend, Prism, the Djinn openings, Thor and Undead
// Sword.
extern char ResourceIdCrystalSheet;
#define RESOURCE_CRYSTAL_SHEET ((Sint32)&ResourceIdCrystalSheet)
// Drawn by Atalanta, the casting impacts, Cruel Ruin, the one-Djinni summons, Dragon Cloud, Fire,
// Flare, the rush bursts, Haures, Lucky Wheels, Odyssey, Procne, Spider Web, the streak volleys,
// the monster strikes and Thunder Mine.
extern char ResourceIdEmberStreakSheet;
#define RESOURCE_EMBER_STREAK_SHEET ((Sint32)&ResourceIdEmberStreakSheet)
// Colours for the battle background's star field (btElementGlowInitStarField), Cruel Ruin and the
// streak volleys.
extern char ResourceIdVioletPaletteD;
#define RESOURCE_VIOLET_PALETTE_D ((Sint32)&ResourceIdVioletPaletteD)
// Drawn by the casting impacts and Cruel Ruin.
extern char ResourceIdDustPuffSheet;
#define RESOURCE_DUST_PUFF_SHEET ((Sint32)&ResourceIdDustPuffSheet)
// Drawn by the Zagan summon, Fire Puppet, Gasp, the healing motes, Light Surge, Lucky Wheels, the
// shell shot, Nirvana, Planet Diver, Rime, the stat changes, Thor and Vengeance.
extern char ResourceIdMercuryDjinnSheet;
#define RESOURCE_MERCURY_DJINN_SHEET ((Sint32)&ResourceIdMercuryDjinnSheet)
// Drawn by the breaths, the canvas layer, the casting impacts, the one-Djinni summons, the Zagan
// summon, Heavy Divide, Helm Splitter, Liquifier, Neptune, Nirvana, Ocean Fist, Odyssey, Rising
// Dragon, Triple Chomp, Vengeance and Whorl.
extern char ResourceIdBlueFlameSheet;
#define RESOURCE_BLUE_FLAME_SHEET ((Sint32)&ResourceIdBlueFlameSheet)
// Drawn by the breaths, the blessings' burst showers, the casting impacts, Daedalus, the Zagan
// summon, Flora, the rush bursts, Gasp, Haures, the healing motes, the shell shot, Planet Diver,
// Spider Web, the stat changes, the status motes, the monster strikes, Thor, Thunder Mine,
// Vengeance and Whorl.
extern char ResourceIdMarsDjinnSheet;
#define RESOURCE_MARS_DJINN_SHEET ((Sint32)&ResourceIdMarsDjinnSheet)
// Drawn by the shards of Ice and Douse and the healing motes.
extern char ResourceIdIceShardSheet;
#define RESOURCE_ICE_SHARD_SHEET ((Sint32)&ResourceIdIceShardSheet)
// Drawn by Boreas, the shards of Ice and Douse, Frost, the healing motes, the status motes and
// Thunder Mine.
extern char ResourceIdSparkleDots;
#define RESOURCE_SPARKLE_DOTS ((Sint32)&ResourceIdSparkleDots)
// Drawn by Azul, the blessings' burst showers, the shards of Ice and Douse, the healing motes, the
// Djinn openings, the monster strikes and Vengeance.
extern char ResourceIdBlueBeamSheet;
#define RESOURCE_BLUE_BEAM_SHEET ((Sint32)&ResourceIdBlueBeamSheet)
// Drawn by Daedalus, Gaia, the shell shot and the streak volleys.
extern char ResourceIdEarthWallSheet;
#define RESOURCE_EARTH_WALL_SHEET ((Sint32)&ResourceIdEarthWallSheet)
// Drawn by the shell shot.
extern char ResourceIdCrescentMoonSheet;
#define RESOURCE_CRESCENT_MOON_SHEET ((Sint32)&ResourceIdCrescentMoonSheet)
// Drawn by Gasp, the healing motes, Lucky Wheels, the shell shot and the stat changes.
extern char ResourceIdVenusDjinnSheet;
#define RESOURCE_VENUS_DJINN_SHEET ((Sint32)&ResourceIdVenusDjinnSheet)
// Drawn by the shell shot.
extern char ResourceIdGoldShellSheet;
#define RESOURCE_GOLD_SHELL_SHEET ((Sint32)&ResourceIdGoldShellSheet)
// Drawn by the blessings' burst showers, the Cards, the casting impacts, Daedalus, Flora, the rush
// bursts, Gasp, Heavy Divide, Legend, the shell shot, the orbiting skulls, stars and target bursts,
// the Djinn openings, the status motes, the monster strikes and Undead Sword.
extern char ResourceIdSmokeSheet;
#define RESOURCE_SMOKE_SHEET ((Sint32)&ResourceIdSmokeSheet)
// Colours for the shell shot.
extern char ResourceIdBluePalette;
#define RESOURCE_BLUE_PALETTE ((Sint32)&ResourceIdBluePalette)
// Drawn by the shell shot and Spider Web.
extern char ResourceIdSpiderWebSheet;
#define RESOURCE_SPIDER_WEB_SHEET ((Sint32)&ResourceIdSpiderWebSheet)
// Colours for Atalanta, the blessings' burst showers, the casting impacts, the rush bursts, Lucky
// Wheels, the shell shot, the Djinn openings, Spider Web, the streak volleys, the status motes and
// the monster strikes.
extern char ResourceIdLimePalette;
#define RESOURCE_LIME_PALETTE ((Sint32)&ResourceIdLimePalette)
// Colours for the shell shot and the streak volleys.
extern char ResourceIdGreenPalette;
#define RESOURCE_GREEN_PALETTE ((Sint32)&ResourceIdGreenPalette)
// Drawn by the blessings' burst showers, Hail, Neptune, Nereid, the particle streams, the Djinn
// openings and Whorl.
extern char ResourceIdWaterSpraySheet;
#define RESOURCE_WATER_SPRAY_SHEET ((Sint32)&ResourceIdWaterSpraySheet)
// Drawn by Nereid, the particle streams, Ragnarok, Undead Sword and Volcano.
extern char ResourceIdFirePillarSheetA;
#define RESOURCE_FIRE_PILLAR_SHEET_A ((Sint32)&ResourceIdFirePillarSheetA)
// Drawn by the casting impacts, Helm Splitter and Planet Diver.
extern char ResourceIdSkullSheet;
#define RESOURCE_SKULL_SHEET ((Sint32)&ResourceIdSkullSheet)
// Drawn by the casting impacts, the glowing lunge, Judgment, the shell shot, Nirvana, the orbiting
// skulls, stars and target bursts, Planet Diver, Raging Heat, the stat changes and the status
// motes.
extern char ResourceIdPinkBurstSheet;
#define RESOURCE_PINK_BURST_SHEET ((Sint32)&ResourceIdPinkBurstSheet)
// Drawn by Planet Diver.
extern char ResourceIdSpiralSheet;
#define RESOURCE_SPIRAL_SHEET ((Sint32)&ResourceIdSpiralSheet)
extern char ResourceIdMemberBurstImage;
#define RESOURCE_MEMBER_BURST_IMAGE ((Sint32)&ResourceIdMemberBurstImage)
// Drawn by Plasma and Thor.
extern char ResourceIdLightningPillarSheet;
#define RESOURCE_LIGHTNING_PILLAR_SHEET ((Sint32)&ResourceIdLightningPillarSheet)
// Colours for Plasma.
extern char ResourceIdVioletPaletteB;
#define RESOURCE_VIOLET_PALETTE_B ((Sint32)&ResourceIdVioletPaletteB)
// Drawn by the blessings' burst showers, the casting impacts, Nirvana, Prism and Spire.
extern char ResourceIdBoulderSheet;
#define RESOURCE_BOULDER_SHEET ((Sint32)&ResourceIdBoulderSheet)
// Drawn by Procne.
extern char ResourceIdWhirlwindSheet;
#define RESOURCE_WHIRLWIND_SHEET ((Sint32)&ResourceIdWhirlwindSheet)
// Drawn by Procne.
extern char ResourceIdVioletCometSheet;
#define RESOURCE_VIOLET_COMET_SHEET ((Sint32)&ResourceIdVioletCometSheet)
// Drawn by Bolt, the casting impacts, Catastrophe, Gasp, the particle streams, Plasma, Procne,
// Ragnarok, Ray, the monster strikes and Thunder Mine.
extern char ResourceIdLightningBoltSheet;
#define RESOURCE_LIGHTNING_BOLT_SHEET ((Sint32)&ResourceIdLightningBoltSheet)
// Drawn by the streak volleys.
extern char ResourceIdFlameArcSheet;
#define RESOURCE_FLAME_ARC_SHEET ((Sint32)&ResourceIdFlameArcSheet)
// Colours for the casting impacts, the streak volleys and Volcano.
extern char ResourceIdRedPaletteB;
#define RESOURCE_RED_PALETTE_B ((Sint32)&ResourceIdRedPaletteB)
// Colours for the blessings' burst showers, the casting impacts, Flora, the healing motes, Heavy
// Divide, the streak volleys, the status motes and Undead Sword.
extern char ResourceIdCyanPalette;
#define RESOURCE_CYAN_PALETTE ((Sint32)&ResourceIdCyanPalette)
// Colours for Gaia and the streak volleys.
extern char ResourceIdOrangePaletteA;
#define RESOURCE_ORANGE_PALETTE_A ((Sint32)&ResourceIdOrangePaletteA)
// Drawn by the blessings' burst showers, the casting impacts, Crystal, Cybele, Drain, Eruption,
// Gasp, the glowing lunge, the healing motes, Heavy Divide, Nirvana, the particle streams, the
// Djinn openings, Ragnarok, Ramses, the status motes, Thor, Undead Sword and Vengeance.
extern char ResourceIdBlastSheet;
#define RESOURCE_BLAST_SHEET ((Sint32)&ResourceIdBlastSheet)
// Drawn by the one-Djinni summons, the charge of Core and Cannon, the stat changes and the status
// motes.
extern char ResourceIdGlowOrbSheet;
#define RESOURCE_GLOW_ORB_SHEET ((Sint32)&ResourceIdGlowOrbSheet)
// Drawn by the blessings' burst showers and the status motes.
extern char ResourceIdHeartSheet;
#define RESOURCE_HEART_SHEET ((Sint32)&ResourceIdHeartSheet)
// Drawn by the status motes.
extern char ResourceIdFlameColumnSheet;
#define RESOURCE_FLAME_COLUMN_SHEET ((Sint32)&ResourceIdFlameColumnSheet)
// Drawn by the casting impacts, the one-Djinni summons, Gasp, Hail, the healing motes, the Djinn
// openings, the status motes and Undead Sword.
extern char ResourceIdMercuryDjinnSmallSheet;
#define RESOURCE_MERCURY_DJINN_SMALL_SHEET ((Sint32)&ResourceIdMercuryDjinnSmallSheet)
// Drawn by Bolt, the blessings' burst showers, the canvas layer, the casting impacts, Catastrophe,
// the rush bursts, Judgment, the Djinn openings, the status motes, the monster strikes, Thor,
// Thunder Mine, Trident and Froth and Whirlwind.
extern char ResourceIdTornadoSheet;
#define RESOURCE_TORNADO_SHEET ((Sint32)&ResourceIdTornadoSheet)
// Drawn by the effect position helpers and the monster strikes.
extern char ResourceIdBeamImage;
#define RESOURCE_BEAM_IMAGE ((Sint32)&ResourceIdBeamImage)
// Drawn by the casting impacts and the monster strikes.
extern char ResourceIdBlueDropletSheet;
#define RESOURCE_BLUE_DROPLET_SHEET ((Sint32)&ResourceIdBlueDropletSheet)
// Colours for the blessings' burst showers and the monster strikes.
extern char ResourceIdPinkPalette;
#define RESOURCE_PINK_PALETTE ((Sint32)&ResourceIdPinkPalette)
// Drawn by the status motes and the vortices of Froth and Whirlwind.
extern char ResourceIdVortexSheet;
#define RESOURCE_VORTEX_SHEET ((Sint32)&ResourceIdVortexSheet)
// Drawn by Annihilation, the casting impacts and Eruption.
extern char ResourceIdFireStreakSheet;
#define RESOURCE_FIRE_STREAK_SHEET ((Sint32)&ResourceIdFireStreakSheet)
// Drawn by the casting impacts, the one-Djinni summons, Gasp, the healing motes, the Djinn
// openings, Undead Sword and Whorl.
extern char ResourceIdJupiterDjinnSmallSheet;
#define RESOURCE_JUPITER_DJINN_SMALL_SHEET ((Sint32)&ResourceIdJupiterDjinnSmallSheet)
// Drawn by the blessings' burst showers, the casting impacts, Crystal, the one-Djinni summons,
// Gasp, Geode, Hail, the healing motes and the Djinn openings.
extern char ResourceIdVenusDjinnSmallSheet;
#define RESOURCE_VENUS_DJINN_SMALL_SHEET ((Sint32)&ResourceIdVenusDjinnSmallSheet)
// Drawn by the casting impacts, Frost, Hail and the Djinn openings.
extern char ResourceIdIceChipSheet;
#define RESOURCE_ICE_CHIP_SHEET ((Sint32)&ResourceIdIceChipSheet)
// Drawn by Boreas, the blessings' burst showers, the casting impacts, Drain, the glowing lunge, the
// healing motes and the Djinn openings.
extern char ResourceIdIceBlockSheet;
#define RESOURCE_ICE_BLOCK_SHEET ((Sint32)&ResourceIdIceBlockSheet)
// Drawn by the blessings' burst showers and the casting impacts.
extern char ResourceIdRockSpireSheet;
#define RESOURCE_ROCK_SPIRE_SHEET ((Sint32)&ResourceIdRockSpireSheet)
// Drawn by Azul and the stat changes.
extern char ResourceIdWispSheet;
#define RESOURCE_WISP_SHEET ((Sint32)&ResourceIdWispSheet)
// Drawn by the blessings' burst showers, the casting impacts, the one-Djinni summons, the charge of
// Core and Cannon, Gasp, the healing motes, the effect position helpers and the Djinn openings.
extern char ResourceIdMarsDjinnSmallSheet;
#define RESOURCE_MARS_DJINN_SMALL_SHEET ((Sint32)&ResourceIdMarsDjinnSmallSheet)
// Drawn by the Cards and the stat changes.
extern char ResourceIdDaggerSheet;
#define RESOURCE_DAGGER_SHEET ((Sint32)&ResourceIdDaggerSheet)
// Colours for the Cards and the stat changes.
extern char ResourceIdPaletteRampImage;
#define RESOURCE_PALETTE_RAMP_IMAGE ((Sint32)&ResourceIdPaletteRampImage)
// Drawn by Cybele.
extern char ResourceIdThornVineSheet;
#define RESOURCE_THORN_VINE_SHEET ((Sint32)&ResourceIdThornVineSheet)
extern char ResourceIdYellowRingSheet;
#define RESOURCE_YELLOW_RING_SHEET ((Sint32)&ResourceIdYellowRingSheet)
// Drawn by the stat changes.
extern char ResourceIdVioletStreakSheet;
#define RESOURCE_VIOLET_STREAK_SHEET ((Sint32)&ResourceIdVioletStreakSheet)
extern char ResourceIdShieldSheet;
#define RESOURCE_SHIELD_SHEET ((Sint32)&ResourceIdShieldSheet)
// Drawn by Quake.
extern char ResourceIdRockWallSheet;
#define RESOURCE_ROCK_WALL_SHEET ((Sint32)&ResourceIdRockWallSheet)
// Drawn by the blessings' burst showers.
extern char ResourceIdVioletLightningSheetA;
#define RESOURCE_VIOLET_LIGHTNING_SHEET_A ((Sint32)&ResourceIdVioletLightningSheetA)
extern char ResourceIdDemonFaceSheet;
#define RESOURCE_DEMON_FACE_SHEET ((Sint32)&ResourceIdDemonFaceSheet)
// Drawn by the orbiting skulls, stars and target bursts.
extern char ResourceIdStarDotSheet;
#define RESOURCE_STAR_DOT_SHEET ((Sint32)&ResourceIdStarDotSheet)
extern char ResourceIdTargetBurstImage;
#define RESOURCE_TARGET_BURST_IMAGE ((Sint32)&ResourceIdTargetBurstImage)
// Drawn by Atalanta.
extern char ResourceIdSlashSheet;
#define RESOURCE_SLASH_SHEET ((Sint32)&ResourceIdSlashSheet)
// The path Atalanta's particles follow.
extern char ResourceIdAtalantaPath;
#define RESOURCE_ATALANTA_PATH ((Sint32)&ResourceIdAtalantaPath)
// Drawn by the growing columns.
extern char ResourceIdGreenPillarSheet;
#define RESOURCE_GREEN_PILLAR_SHEET ((Sint32)&ResourceIdGreenPillarSheet)
// Drawn by Growth.
extern char ResourceIdVineSheet;
#define RESOURCE_VINE_SHEET ((Sint32)&ResourceIdVineSheet)
// Drawn by Vine: five 32x32 vine cels.
extern char ResourceIdGreenVineSheet;
#define RESOURCE_GREEN_VINE_SHEET ((Sint32)&ResourceIdGreenVineSheet)
// Drawn by Thorn: the large thorn, the small one and the dust cels after them.
extern char ResourceIdThornSheet;
#define RESOURCE_THORN_SHEET ((Sint32)&ResourceIdThornSheet)
// Drawn by the casting impacts, Diamond Berg, the form changes, Geode, the leaping strike, Meteor
// Blow, Plume Edge and Trident.
extern char ResourceIdBillowTexture;
#define RESOURCE_BILLOW_TEXTURE ((Sint32)&ResourceIdBillowTexture)
// The particle sprites nearly every battle effect loads first, 90 of them, from Annihilation to
// Zagan.
extern char ResourceIdYellowGlowSheet;
#define RESOURCE_YELLOW_GLOW_SHEET ((Sint32)&ResourceIdYellowGlowSheet)
// Drawn by the Djinni charge and Zagan.
extern char ResourceIdPetalSheet;
#define RESOURCE_PETAL_SHEET ((Sint32)&ResourceIdPetalSheet)
// Drawn by the breaths, the casting impacts, Diamond Berg, Eclipse, the Gasp family, the leaping
// strike, Meteor Blow, Moloch, Serac, Trident and Zagan.
extern char ResourceIdFlameBandTexture;
#define RESOURCE_FLAME_BAND_TEXTURE ((Sint32)&ResourceIdFlameBandTexture)
extern char ResourceIdSheepCardSheet; // directory row 0xe3
#define RESOURCE_SHEEP_CARD_SHEET ((Sint32)&ResourceIdSheepCardSheet)
// Drawn by the Djinni sign and the target strike.
extern char ResourceIdStarCrossSheet;
#define RESOURCE_STAR_CROSS_SHEET ((Sint32)&ResourceIdStarCrossSheet)
// Drawn by the blade rain and Sabre Dance.
extern char ResourceIdCrossedDaggersSheet;
#define RESOURCE_CROSSED_DAGGERS_SHEET ((Sint32)&ResourceIdCrossedDaggersSheet)
// Drawn by the burst showers, the casting impacts and the stat boosts.
extern char ResourceIdGreyPaletteA;
#define RESOURCE_GREY_PALETTE_A ((Sint32)&ResourceIdGreyPaletteA)
// Drawn by Azul, the breaths, the casting impacts, Catastrophe, Charon, Coatlicue, Cruel Ruin,
// Daedalus, Death Plunge, Diamond Berg, Flora, the Gasp family, Judgment, Legend, Liquifier,
// Megiddo, Odyssey, Radiant Fire, Raging Heat, the rushing strike and Triple Chomp.
extern char ResourceIdFireRingTexture;
#define RESOURCE_FIRE_RING_TEXTURE ((Sint32)&ResourceIdFireRingTexture)
// Drawn by the breaths, Cruel Ruin, Daedalus's second strike, Eclipse, the form changes, the Gasp
// family, Light Surge, Liquifier and Megiddo.
extern char ResourceIdGlowBandTextureA;
#define RESOURCE_GLOW_BAND_TEXTURE_A ((Sint32)&ResourceIdGlowBandTextureA)
// Drawn by the bolt trail and the drain motes.
extern char ResourceIdVioletPaletteC;
#define RESOURCE_VIOLET_PALETTE_C ((Sint32)&ResourceIdVioletPaletteC)
// Drawn by the burst showers and the Gasp family.
extern char ResourceIdMusicNoteSheet;
#define RESOURCE_MUSIC_NOTE_SHEET ((Sint32)&ResourceIdMusicNoteSheet)
// Drawn by Liquifier.
extern char ResourceIdFlameWallTexture;
#define RESOURCE_FLAME_WALL_TEXTURE ((Sint32)&ResourceIdFlameWallTexture)
// Drawn by Atalanta.
extern char ResourceIdLeafWarriorTiles;
#define RESOURCE_LEAF_WARRIOR_TILES ((Sint32)&ResourceIdLeafWarriorTiles)
// Drawn by the form changes.
extern char ResourceIdOrangePaletteC;
#define RESOURCE_ORANGE_PALETTE_C ((Sint32)&ResourceIdOrangePaletteC)
extern char ResourceIdCometTexture; // directory row 0xd0
#define RESOURCE_COMET_TEXTURE ((Sint32)&ResourceIdCometTexture)
extern char ResourceIdFeatherSheet; // directory row 0xd1
#define RESOURCE_FEATHER_SHEET ((Sint32)&ResourceIdFeatherSheet)
// Drawn by Zagan.
extern char ResourceIdCrescentRowSheet;
#define RESOURCE_CRESCENT_ROW_SHEET ((Sint32)&ResourceIdCrescentRowSheet)
// Drawn by Azul, the casting impacts, Catastrophe, Charon, Cruel Ruin, Eclipse, the Gasp family,
// Haures, the water column and the water sheet.
extern char ResourceIdSwirlTexture;
#define RESOURCE_SWIRL_TEXTURE ((Sint32)&ResourceIdSwirlTexture)
// Drawn by Cruel Ruin and Eclipse.
extern char ResourceIdGlowBandTextureB;
#define RESOURCE_GLOW_BAND_TEXTURE_B ((Sint32)&ResourceIdGlowBandTextureB)
// Drawn by Cruel Ruin.
extern char ResourceIdPinkBarSheet;
#define RESOURCE_PINK_BAR_SHEET ((Sint32)&ResourceIdPinkBarSheet)
// Drawn by Azul, Cruel Ruin and Legend.
extern char ResourceIdCornerRuneASheet;
#define RESOURCE_CORNER_RUNE_A_SHEET ((Sint32)&ResourceIdCornerRuneASheet)
// Drawn by Cruel Ruin and Daedalus.
extern char ResourceIdForestCanopyTiles;
#define RESOURCE_FOREST_CANOPY_TILES ((Sint32)&ResourceIdForestCanopyTiles)
// Drawn by Eclipse.
extern char ResourceIdGoldenDragonTiles;
#define RESOURCE_GOLDEN_DRAGON_TILES ((Sint32)&ResourceIdGoldenDragonTiles)
extern char ResourceIdGoldenDragonHeadTiles;
#define RESOURCE_GOLDEN_DRAGON_HEAD_TILES ((Sint32)&ResourceIdGoldenDragonHeadTiles)
// Drawn by the Djinni charge, Eclipse, Megiddo, Meteor Blow, Odyssey and Serac.
extern char ResourceIdDustBlobSheet;
#define RESOURCE_DUST_BLOB_SHEET ((Sint32)&ResourceIdDustBlobSheet)
// Drawn by the projectile volleys.
extern char ResourceIdCometSheetA;
#define RESOURCE_COMET_SHEET_A ((Sint32)&ResourceIdCometSheetA)
// Drawn by the projectile volleys and Void Beam.
extern char ResourceIdCometSheetB;
#define RESOURCE_COMET_SHEET_B ((Sint32)&ResourceIdCometSheetB)
// Drawn by the projectile volleys.
extern char ResourceIdCometSheetC;
#define RESOURCE_COMET_SHEET_C ((Sint32)&ResourceIdCometSheetC)
extern char ResourceIdWindStreakSheet;
#define RESOURCE_WIND_STREAK_SHEET ((Sint32)&ResourceIdWindStreakSheet)
extern char ResourceIdShortStreakPicture;
#define RESOURCE_SHORT_STREAK_PICTURE ((Sint32)&ResourceIdShortStreakPicture)
extern char ResourceIdShurikenSheet;
#define RESOURCE_SHURIKEN_SHEET ((Sint32)&ResourceIdShurikenSheet)
extern char ResourceIdProjectileVolleyImage;
#define RESOURCE_PROJECTILE_VOLLEY_IMAGE ((Sint32)&ResourceIdProjectileVolleyImage)
extern char ResourceIdYellowSparkSheet;
#define RESOURCE_YELLOW_SPARK_SHEET ((Sint32)&ResourceIdYellowSparkSheet)
// Drawn by the casting impacts, the mirrored wall and the projectile volleys.
extern char ResourceIdFireballSheet;
#define RESOURCE_FIREBALL_SHEET ((Sint32)&ResourceIdFireballSheet)
// Drawn by Jupiter and the leaping strike.
extern char ResourceIdFlameballSheet;
#define RESOURCE_FLAMEBALL_SHEET ((Sint32)&ResourceIdFlameballSheet)
// Drawn by Ground.
extern char ResourceIdPortalSheet;
#define RESOURCE_PORTAL_SHEET ((Sint32)&ResourceIdPortalSheet)
// Drawn by Thor.
extern char ResourceIdPaleBlueGlowSheet;
#define RESOURCE_PALE_BLUE_GLOW_SHEET ((Sint32)&ResourceIdPaleBlueGlowSheet)
extern char ResourceIdLightFanSheet;
#define RESOURCE_LIGHT_FAN_SHEET ((Sint32)&ResourceIdLightFanSheet)
extern char ResourceIdBlueTallGlowSheet;
#define RESOURCE_BLUE_TALL_GLOW_SHEET ((Sint32)&ResourceIdBlueTallGlowSheet)
extern char ResourceIdVioletLightningSheetB;
#define RESOURCE_VIOLET_LIGHTNING_SHEET_B ((Sint32)&ResourceIdVioletLightningSheetB)
extern char ResourceIdVioletMoteSheet;
#define RESOURCE_VIOLET_MOTE_SHEET ((Sint32)&ResourceIdVioletMoteSheet)
// Drawn by the attack slashes, the casting impacts, the flurry and Ulysses.
extern char ResourceIdYellowOrbSheet;
#define RESOURCE_YELLOW_ORB_SHEET ((Sint32)&ResourceIdYellowOrbSheet)
// Drawn by the attack slashes and the flurry.
extern char ResourceIdBlueArcSheetA;
#define RESOURCE_BLUE_ARC_SHEET_A ((Sint32)&ResourceIdBlueArcSheetA)
// Drawn by the attack slashes, the flurry and Haures.
extern char ResourceIdBlueArcSheetB;
#define RESOURCE_BLUE_ARC_SHEET_B ((Sint32)&ResourceIdBlueArcSheetB)
// Drawn by the attack slashes and the flurry.
extern char ResourceIdWaveSheet;
#define RESOURCE_WAVE_SHEET ((Sint32)&ResourceIdWaveSheet)
// Drawn by Ulysses.
extern char ResourceIdUlyssesSilhouette;
#define RESOURCE_ULYSSES_SILHOUETTE ((Sint32)&ResourceIdUlyssesSilhouette)
extern char ResourceIdFlutteringFlakeSheet;
#define RESOURCE_FLUTTERING_FLAKE_SHEET ((Sint32)&ResourceIdFlutteringFlakeSheet)
extern char ResourceIdRuneCircleSheet;
#define RESOURCE_RUNE_CIRCLE_SHEET ((Sint32)&ResourceIdRuneCircleSheet)
extern char ResourceIdFadingCrescentSheet;
#define RESOURCE_FADING_CRESCENT_SHEET ((Sint32)&ResourceIdFadingCrescentSheet)
// Drawn by the swirl swipe.
extern char ResourceIdMagentaSwirlSheet;
#define RESOURCE_MAGENTA_SWIRL_SHEET ((Sint32)&ResourceIdMagentaSwirlSheet)
extern char ResourceIdMagentaTailSheet;
#define RESOURCE_MAGENTA_TAIL_SHEET ((Sint32)&ResourceIdMagentaTailSheet)
// Drawn by the attack slashes, the crescent slash and the critical hits.
extern char ResourceIdStarCrescentSheet;
#define RESOURCE_STAR_CRESCENT_SHEET ((Sint32)&ResourceIdStarCrescentSheet)
// Drawn by the crescent slash and the critical hits.
extern char ResourceIdGreyCrescentSheet;
#define RESOURCE_GREY_CRESCENT_SHEET ((Sint32)&ResourceIdGreyCrescentSheet)
// Drawn by the casting impacts.
extern char ResourceIdImpFaceSheet;
#define RESOURCE_IMP_FACE_SHEET ((Sint32)&ResourceIdImpFaceSheet)
extern char ResourceIdApocalypseSheet;
#define RESOURCE_APOCALYPSE_SHEET ((Sint32)&ResourceIdApocalypseSheet)
extern char ResourceIdBoulderPicture;
#define RESOURCE_BOULDER_PICTURE ((Sint32)&ResourceIdBoulderPicture)
extern char ResourceIdSarcophagusSheet;
#define RESOURCE_SARCOPHAGUS_SHEET ((Sint32)&ResourceIdSarcophagusSheet)
extern char ResourceIdRaidensWrathSheet;
#define RESOURCE_RAIDENS_WRATH_SHEET ((Sint32)&ResourceIdRaidensWrathSheet)
extern char ResourceIdCloudPuffSheet;
#define RESOURCE_CLOUD_PUFF_SHEET ((Sint32)&ResourceIdCloudPuffSheet)
extern char ResourceIdFlameSheetA;
#define RESOURCE_FLAME_SHEET_A ((Sint32)&ResourceIdFlameSheetA)
// Drawn by the casting impacts, Flora, Megaera and the wind funnel.
extern char ResourceIdTanPalette;
#define RESOURCE_TAN_PALETTE ((Sint32)&ResourceIdTanPalette)
// Drawn by the casting impacts.
extern char ResourceIdVioletPaletteG;
#define RESOURCE_VIOLET_PALETTE_G ((Sint32)&ResourceIdVioletPaletteG)
extern char ResourceIdBlueBeastSheet;
#define RESOURCE_BLUE_BEAST_SHEET ((Sint32)&ResourceIdBlueBeastSheet)
extern char ResourceIdRedCrescentSheetB;
#define RESOURCE_RED_CRESCENT_SHEET_B ((Sint32)&ResourceIdRedCrescentSheetB)
// Drawn by the impact burst.
extern char ResourceIdFlareSheet;
#define RESOURCE_FLARE_SHEET ((Sint32)&ResourceIdFlareSheet)
extern char ResourceIdFlareImage;
#define RESOURCE_FLARE_IMAGE ((Sint32)&ResourceIdFlareImage)
// Drawn by the particle streams.
extern char ResourceIdFlameSheetB;
#define RESOURCE_FLAME_SHEET_B ((Sint32)&ResourceIdFlameSheetB)
// Drawn by Annihilation.
extern char ResourceIdFireBurstSheet;
#define RESOURCE_FIRE_BURST_SHEET ((Sint32)&ResourceIdFireBurstSheet)
// Drawn by Ragnarok.
extern char ResourceIdFlameBladeSheet;
#define RESOURCE_FLAME_BLADE_SHEET ((Sint32)&ResourceIdFlameBladeSheet)
// Drawn by the falling sword.
extern char ResourceIdSwordSheet;
#define RESOURCE_SWORD_SHEET ((Sint32)&ResourceIdSwordSheet)
// Drawn by Judgment.
extern char ResourceIdWingedKnightATiles;
#define RESOURCE_WINGED_KNIGHT_A_TILES ((Sint32)&ResourceIdWingedKnightATiles)
extern char ResourceIdWingedKnightBTiles;
#define RESOURCE_WINGED_KNIGHT_B_TILES ((Sint32)&ResourceIdWingedKnightBTiles)
extern char ResourceIdWingedKnightCTiles;
#define RESOURCE_WINGED_KNIGHT_C_TILES ((Sint32)&ResourceIdWingedKnightCTiles)
// Drawn by Judgment and Star Mine.
extern char ResourceIdBlueCrescentSheet;
#define RESOURCE_BLUE_CRESCENT_SHEET ((Sint32)&ResourceIdBlueCrescentSheet)
// Drawn by Judgment.
extern char ResourceIdExplosionSheet;
#define RESOURCE_EXPLOSION_SHEET ((Sint32)&ResourceIdExplosionSheet)
extern char ResourceIdJudgmentFlashSheet;
#define RESOURCE_JUDGMENT_FLASH_SHEET ((Sint32)&ResourceIdJudgmentFlashSheet)
// Drawn by Kirin.
extern char ResourceIdFireBlobSheet;
#define RESOURCE_FIRE_BLOB_SHEET ((Sint32)&ResourceIdFireBlobSheet)
// Drawn by Neptune.
extern char ResourceIdSeaSerpentTiles;
#define RESOURCE_SEA_SERPENT_TILES ((Sint32)&ResourceIdSeaSerpentTiles)
// Drawn by the flashing strikes.
extern char ResourceIdArcFragmentsSheet;
#define RESOURCE_ARC_FRAGMENTS_SHEET ((Sint32)&ResourceIdArcFragmentsSheet)
extern char ResourceIdVorpalSlashImageA;
#define RESOURCE_VORPAL_SLASH_IMAGE_A ((Sint32)&ResourceIdVorpalSlashImageA)
extern char ResourceIdVorpalSlashImageB;
#define RESOURCE_VORPAL_SLASH_IMAGE_B ((Sint32)&ResourceIdVorpalSlashImageB)
extern char ResourceIdQuickStrikeImageA;
#define RESOURCE_QUICK_STRIKE_IMAGE_A ((Sint32)&ResourceIdQuickStrikeImageA)
extern char ResourceIdQuickStrikeImageB;
#define RESOURCE_QUICK_STRIKE_IMAGE_B ((Sint32)&ResourceIdQuickStrikeImageB)
extern char ResourceIdUnusedFlashingStrikeImageA;
#define RESOURCE_UNUSED_FLASHING_STRIKE_IMAGE_A ((Sint32)&ResourceIdUnusedFlashingStrikeImageA)
extern char ResourceIdUnusedFlashingStrikeImageB;
#define RESOURCE_UNUSED_FLASHING_STRIKE_IMAGE_B ((Sint32)&ResourceIdUnusedFlashingStrikeImageB)
// Drawn by Soul Shatter and Undead Sword.
extern char ResourceIdLimeColumnSheet;
#define RESOURCE_LIME_COLUMN_SHEET ((Sint32)&ResourceIdLimeColumnSheet)
// Drawn by the water sheet.
extern char ResourceIdCrackedTextureSheet;
#define RESOURCE_CRACKED_TEXTURE_SHEET ((Sint32)&ResourceIdCrackedTextureSheet)
// Drawn by Fire Puppet, the water column and the wind funnel.
extern char ResourceIdStreakTexture;
#define RESOURCE_STREAK_TEXTURE ((Sint32)&ResourceIdStreakTexture)
// Drawn by Diamond Berg.
extern char ResourceIdCrystalBitmapSheet;
#define RESOURCE_CRYSTAL_BITMAP_SHEET ((Sint32)&ResourceIdCrystalBitmapSheet)
// Drawn by Coatlicue and Crystal.
extern char ResourceIdCogTexture;
#define RESOURCE_COG_TEXTURE ((Sint32)&ResourceIdCogTexture)
// Drawn by the Cards and Serac.
extern char ResourceIdBlueCloudPuffSheet;
#define RESOURCE_BLUE_CLOUD_PUFF_SHEET ((Sint32)&ResourceIdBlueCloudPuffSheet)
// Drawn by the heavy blow.
extern char ResourceIdGreatSwordSheet;
#define RESOURCE_GREAT_SWORD_SHEET ((Sint32)&ResourceIdGreatSwordSheet)
extern char ResourceIdAxeSheet;
#define RESOURCE_AXE_SHEET ((Sint32)&ResourceIdAxeSheet)
// Drawn by Cool.
extern char ResourceIdCrestSheet;
#define RESOURCE_CREST_SHEET ((Sint32)&ResourceIdCrestSheet)
// Drawn by the Gasp family and Wyrd Curse.
extern char ResourceIdBurningSkullSheet;
#define RESOURCE_BURNING_SKULL_SHEET ((Sint32)&ResourceIdBurningSkullSheet)
// Drawn by Megaera and Ocean Fist.
extern char ResourceIdWhirlBeamSheet;
#define RESOURCE_WHIRL_BEAM_SHEET ((Sint32)&ResourceIdWhirlBeamSheet)
// Drawn by Light Surge.
extern char ResourceIdGlowOrbTexture;
#define RESOURCE_GLOW_ORB_TEXTURE ((Sint32)&ResourceIdGlowOrbTexture)
// Drawn by Catastrophe, Death Plunge, the element cards, the Gasp family, Light Surge, Trident and
// Whiplash.
extern char ResourceIdVioletPaletteF;
#define RESOURCE_VIOLET_PALETTE_F ((Sint32)&ResourceIdVioletPaletteF)
// Drawn by the geyser and the water column.
extern char ResourceIdEmberTexture;
#define RESOURCE_EMBER_TEXTURE ((Sint32)&ResourceIdEmberTexture)
// Drawn by the Gasp family.
extern char ResourceIdLightBurstSheet;
#define RESOURCE_LIGHT_BURST_SHEET ((Sint32)&ResourceIdLightBurstSheet)
extern char ResourceIdCornerGlowSheet;
#define RESOURCE_CORNER_GLOW_SHEET ((Sint32)&ResourceIdCornerGlowSheet)
// Drawn by Catastrophe, Fume, the Gasp family, Haures, Rising Dragon and the enemies' Rising
// Dragon.
extern char ResourceIdSmokeBandSheet;
#define RESOURCE_SMOKE_BAND_SHEET ((Sint32)&ResourceIdSmokeBandSheet)
// Drawn by Charon.
extern char ResourceIdCrimsonBeastTiles;
#define RESOURCE_CRIMSON_BEAST_TILES ((Sint32)&ResourceIdCrimsonBeastTiles)
extern char ResourceIdRedDomeTiles;
#define RESOURCE_RED_DOME_TILES ((Sint32)&ResourceIdRedDomeTiles)
extern char ResourceIdRedOrbTiles;
#define RESOURCE_RED_ORB_TILES ((Sint32)&ResourceIdRedOrbTiles)
extern char ResourceIdTumblingShardTiles;
#define RESOURCE_TUMBLING_SHARD_TILES ((Sint32)&ResourceIdTumblingShardTiles)
extern char ResourceIdWhiteWispsTiles;
#define RESOURCE_WHITE_WISPS_TILES ((Sint32)&ResourceIdWhiteWispsTiles)
extern char ResourceIdRuneSealSheet;
#define RESOURCE_RUNE_SEAL_SHEET ((Sint32)&ResourceIdRuneSealSheet)
extern char ResourceIdCharmRingSheet;
#define RESOURCE_CHARM_RING_SHEET ((Sint32)&ResourceIdCharmRingSheet)
extern char ResourceIdVioletPaletteE;
#define RESOURCE_VIOLET_PALETTE_E ((Sint32)&ResourceIdVioletPaletteE)
extern char ResourceIdBoltSheet;
#define RESOURCE_BOLT_SHEET ((Sint32)&ResourceIdBoltSheet)
extern char ResourceIdCurseSealSheet;
#define RESOURCE_CURSE_SEAL_SHEET ((Sint32)&ResourceIdCurseSealSheet)
extern char ResourceIdGlowRingTexture;
#define RESOURCE_GLOW_RING_TEXTURE ((Sint32)&ResourceIdGlowRingTexture)
// Drawn by Catastrophe, Fume, the leaping strike, Rising Dragon and the enemies' Rising Dragon.
extern char ResourceIdClawSlashSheet;
#define RESOURCE_CLAW_SLASH_SHEET ((Sint32)&ResourceIdClawSlashSheet)
// Drawn by Heartbreak.
extern char ResourceIdHeartbreakSheet;
#define RESOURCE_HEARTBREAK_SHEET ((Sint32)&ResourceIdHeartbreakSheet)
// Drawn by Raging Heat.
extern char ResourceIdCornerRuneBSheet;
#define RESOURCE_CORNER_RUNE_B_SHEET ((Sint32)&ResourceIdCornerRuneBSheet)
// Drawn by Juggle.
extern char ResourceIdSmallFlameSheet;
#define RESOURCE_SMALL_FLAME_SHEET ((Sint32)&ResourceIdSmallFlameSheet)
// Drawn by Flora.
extern char ResourceIdGreenHairedMaidenTiles;
#define RESOURCE_GREEN_HAIRED_MAIDEN_TILES ((Sint32)&ResourceIdGreenHairedMaidenTiles)
// Drawn by Flora and Iris.
extern char ResourceIdPinkPetalsTiles;
#define RESOURCE_PINK_PETALS_TILES ((Sint32)&ResourceIdPinkPetalsTiles)
// Drawn by Backstab and Helm Splitter.
extern char ResourceIdPinkStarSheet;
#define RESOURCE_PINK_STAR_SHEET ((Sint32)&ResourceIdPinkStarSheet)
// Drawn by Fire Puppet.
extern char ResourceIdGhostSkullTexture;
#define RESOURCE_GHOST_SKULL_TEXTURE ((Sint32)&ResourceIdGhostSkullTexture)
// Drawn by Whiplash.
extern char ResourceIdSlashFlameSheet;
#define RESOURCE_SLASH_FLAME_SHEET ((Sint32)&ResourceIdSlashFlameSheet)
// Drawn by Catastrophe, Megiddo, Odyssey and the enemies' Rising Dragon.
extern char ResourceIdRedHazeBackdrop;
#define RESOURCE_RED_HAZE_BACKDROP ((Sint32)&ResourceIdRedHazeBackdrop)
// Drawn by the rushing strike.
extern char ResourceIdFlamePlumeSheet;
#define RESOURCE_FLAME_PLUME_SHEET ((Sint32)&ResourceIdFlamePlumeSheet)
// Drawn by Dragon Cloud and the rushing strike.
extern char ResourceIdFirePillarSheetB;
#define RESOURCE_FIRE_PILLAR_SHEET_B ((Sint32)&ResourceIdFirePillarSheetB)
// Drawn by Daedalus.
extern char ResourceIdBronzeAutomatonTiles;
#define RESOURCE_BRONZE_AUTOMATON_TILES ((Sint32)&ResourceIdBronzeAutomatonTiles)
extern char ResourceIdMissileTiles;
#define RESOURCE_MISSILE_TILES ((Sint32)&ResourceIdMissileTiles)
extern char ResourceIdRocketFlameSheet;
#define RESOURCE_ROCKET_FLAME_SHEET ((Sint32)&ResourceIdRocketFlameSheet)
extern char ResourceIdBirdFlockSheet;
#define RESOURCE_BIRD_FLOCK_SHEET ((Sint32)&ResourceIdBirdFlockSheet)
// Drawn by Vengeance.
extern char ResourceIdArcherSheet;
#define RESOURCE_ARCHER_SHEET ((Sint32)&ResourceIdArcherSheet)
// Drawn by Life Shear.
extern char ResourceIdLifeShearSheet;
#define RESOURCE_LIFE_SHEAR_SHEET ((Sint32)&ResourceIdLifeShearSheet)
// Drawn by the geyser.
extern char ResourceIdCrossTextureSheet;
#define RESOURCE_CROSS_TEXTURE_SHEET ((Sint32)&ResourceIdCrossTextureSheet)
// Drawn by the bubble column and Coatlicue.
extern char ResourceIdBubbleAnimationSheet;
#define RESOURCE_BUBBLE_ANIMATION_SHEET ((Sint32)&ResourceIdBubbleAnimationSheet)
// Drawn by Coatlicue.
extern char ResourceIdCoatlicueTiles;
#define RESOURCE_COATLICUE_TILES ((Sint32)&ResourceIdCoatlicueTiles)
extern char ResourceIdScaleTexture;
#define RESOURCE_SCALE_TEXTURE ((Sint32)&ResourceIdScaleTexture)
// Drawn by Catastrophe.
extern char ResourceIdChainedDemonTiles;
#define RESOURCE_CHAINED_DEMON_TILES ((Sint32)&ResourceIdChainedDemonTiles)
extern char ResourceIdTriangleSealTexture;
#define RESOURCE_TRIANGLE_SEAL_TEXTURE ((Sint32)&ResourceIdTriangleSealTexture)
extern char ResourceIdPentagonSealTexture;
#define RESOURCE_PENTAGON_SEAL_TEXTURE ((Sint32)&ResourceIdPentagonSealTexture)
// Drawn by Catastrophe, the element cards and Trident.
extern char ResourceIdJaggedStreakSheet;
#define RESOURCE_JAGGED_STREAK_SHEET ((Sint32)&ResourceIdJaggedStreakSheet)
// Drawn by Catastrophe.
extern char ResourceIdLightningTexture;
#define RESOURCE_LIGHTNING_TEXTURE ((Sint32)&ResourceIdLightningTexture)
extern char ResourceIdGoldenScalesTiles;
#define RESOURCE_GOLDEN_SCALES_TILES ((Sint32)&ResourceIdGoldenScalesTiles)
extern char ResourceIdPurpleArmourTiles;
#define RESOURCE_PURPLE_ARMOUR_TILES ((Sint32)&ResourceIdPurpleArmourTiles)
extern char ResourceIdWhiteSpikesTiles;
#define RESOURCE_WHITE_SPIKES_TILES ((Sint32)&ResourceIdWhiteSpikesTiles)
// Drawn by Haures.
extern char ResourceIdChainTexture;
#define RESOURCE_CHAIN_TEXTURE ((Sint32)&ResourceIdChainTexture)
// Drawn by Helm Splitter.
extern char ResourceIdFireSwirlSheet;
#define RESOURCE_FIRE_SWIRL_SHEET ((Sint32)&ResourceIdFireSwirlSheet)
// Drawn by Dragon Cloud.
extern char ResourceIdDragonCloudFlamePath;
#define RESOURCE_DRAGON_CLOUD_FLAME_PATH ((Sint32)&ResourceIdDragonCloudFlamePath)
// Drawn by Legend.
extern char ResourceIdBladeSheet;
#define RESOURCE_BLADE_SHEET ((Sint32)&ResourceIdBladeSheet)
// Drawn by Legend, Megiddo and Odyssey.
extern char ResourceIdFlameGradientTexture;
#define RESOURCE_FLAME_GRADIENT_TEXTURE ((Sint32)&ResourceIdFlameGradientTexture)
// Drawn by the element cards.
extern char ResourceIdVineCardSheet;
#define RESOURCE_VINE_CARD_SHEET ((Sint32)&ResourceIdVineCardSheet)
extern char ResourceIdDragonCardSheet;
#define RESOURCE_DRAGON_CARD_SHEET ((Sint32)&ResourceIdDragonCardSheet)
extern char ResourceIdSunCardSheet;
#define RESOURCE_SUN_CARD_SHEET ((Sint32)&ResourceIdSunCardSheet)
extern char ResourceIdBoltCardSheet;
#define RESOURCE_BOLT_CARD_SHEET ((Sint32)&ResourceIdBoltCardSheet)
// Drawn by the Cards.
extern char ResourceIdButterflyCardSheet;
#define RESOURCE_BUTTERFLY_CARD_SHEET ((Sint32)&ResourceIdButterflyCardSheet)
extern char ResourceIdSwordCardSheet;
#define RESOURCE_SWORD_CARD_SHEET ((Sint32)&ResourceIdSwordCardSheet)
extern char ResourceIdSkeletonCardSheet;
#define RESOURCE_SKELETON_CARD_SHEET ((Sint32)&ResourceIdSkeletonCardSheet)
// Drawn by the breaths.
extern char ResourceIdDustCloudSheet;
#define RESOURCE_DUST_CLOUD_SHEET ((Sint32)&ResourceIdDustCloudSheet)
// Drawn by Trident.
extern char ResourceIdFlameZigzagTexture;
#define RESOURCE_FLAME_ZIGZAG_TEXTURE ((Sint32)&ResourceIdFlameZigzagTexture)
// Drawn by Meteor Blow.
extern char ResourceIdGlowOrbSetSheet;
#define RESOURCE_GLOW_ORB_SET_SHEET ((Sint32)&ResourceIdGlowOrbSetSheet)
// Drawn by Megiddo.
extern char ResourceIdFireStripesTexture;
#define RESOURCE_FIRE_STRIPES_TEXTURE ((Sint32)&ResourceIdFireStripesTexture)
// Drawn by Odyssey.
extern char ResourceIdGiantSwordSheet;
#define RESOURCE_GIANT_SWORD_SHEET ((Sint32)&ResourceIdGiantSwordSheet)
extern char ResourceIdGiantSwordBladeSheet;
#define RESOURCE_GIANT_SWORD_BLADE_SHEET ((Sint32)&ResourceIdGiantSwordBladeSheet)
// Drawn by the cloud swirl.
extern char ResourceIdCloudSwirlSheet;
#define RESOURCE_CLOUD_SWIRL_SHEET ((Sint32)&ResourceIdCloudSwirlSheet)
// Drawn by Iris.
extern char ResourceIdSunsetBackdrop;
#define RESOURCE_SUNSET_BACKDROP ((Sint32)&ResourceIdSunsetBackdrop)
extern char ResourceIdStarfieldFlareBackdrop;
#define RESOURCE_STARFIELD_FLARE_BACKDROP ((Sint32)&ResourceIdStarfieldFlareBackdrop)
extern char ResourceIdRobedGoddessTiles;
#define RESOURCE_ROBED_GODDESS_TILES ((Sint32)&ResourceIdRobedGoddessTiles)

// The skies a summon or Psynergy shows behind the battle.

// The sky of Atalanta, Boreas, Cruel Ruin, Daedalus, Flora, Judgment, Moloch and Procne; only the
// unused slideshow list names it (DisplayScrollUnusedSlideResources).
extern char ResourceIdVioletSkyBackdrop;
#define RESOURCE_VIOLET_SKY_BACKDROP ((Sint32)&ResourceIdVioletSkyBackdrop)
// The sky of Neptune and Thor; the credits' slideshow shows it (DisplayScrollSlideResources).
extern char ResourceIdDuskCloudsBackdrop;
#define RESOURCE_DUSK_CLOUDS_BACKDROP ((Sint32)&ResourceIdDuskCloudsBackdrop)
// The sky of Judgment; the credits' slideshow shows it (DisplayScrollSlideResources).
extern char ResourceIdBeachBackdrop;
#define RESOURCE_BEACH_BACKDROP ((Sint32)&ResourceIdBeachBackdrop)
// The sky of Judgment and Kirin; only the unused slideshow list names it
// (DisplayScrollUnusedSlideResources).
extern char ResourceIdForestBackdrop;
#define RESOURCE_FOREST_BACKDROP ((Sint32)&ResourceIdForestBackdrop)
// Drawn by Cruel Ruin.
extern char ResourceIdSunbeamCloudsBackdrop;
#define RESOURCE_SUNBEAM_CLOUDS_BACKDROP ((Sint32)&ResourceIdSunbeamCloudsBackdrop)
// The sky Eclipse opens on.
extern char ResourceIdEarthHorizonBackdrop;
#define RESOURCE_EARTH_HORIZON_BACKDROP ((Sint32)&ResourceIdEarthHorizonBackdrop)
// The land far below that Eclipse's blast lands on; Charon draws it too.
extern char ResourceIdAerialViewBackdrop;
#define RESOURCE_AERIAL_VIEW_BACKDROP ((Sint32)&ResourceIdAerialViewBackdrop)

// Battle backdrops: the places battles are fought in, The Broken Seal's that only the credits'
// slideshow shows, and the battle background's star field.

// The Colosso's backdrop, which the debug battle menu fights on (btDebugBattleRunMenu) and the
// credits' slideshow shows (DisplayScrollSlideResources).
extern char ResourceIdColossoBackdrop;
#define RESOURCE_COLOSSO_BACKDROP ((Sint32)&ResourceIdColossoBackdrop)
// The desert's battle backdrop (fdBattleBackgroundSelectForEnvironment), and the sky of the Ramses
// summon.
extern char ResourceIdDesertBackdrop;
#define RESOURCE_DESERT_BACKDROP ((Sint32)&ResourceIdDesertBackdrop)
// Battle backdrops of The Broken Seal's places, which no battle in this game is fought on: the
// credits' slideshow shows those DisplayScrollSlideResources lists, and only the unused list names
// the rest.
extern char ResourceIdSuhallaGateBackdrop;
#define RESOURCE_SUHALLA_GATE_BACKDROP ((Sint32)&ResourceIdSuhallaGateBackdrop)
extern char ResourceIdPirateShipBackdrop;
#define RESOURCE_PIRATE_SHIP_BACKDROP ((Sint32)&ResourceIdPirateShipBackdrop)
extern char ResourceIdCrossboneIsleCaveBackdrop;
#define RESOURCE_CROSSBONE_ISLE_CAVE_BACKDROP ((Sint32)&ResourceIdCrossboneIsleCaveBackdrop)
extern char ResourceIdVenusLighthouseAerieStormBackdrop;
#define RESOURCE_VENUS_LIGHTHOUSE_AERIE_STORM_BACKDROP                                             \
    ((Sint32)&ResourceIdVenusLighthouseAerieStormBackdrop)
extern char ResourceIdVenusLighthouseAerieBackdrop;
#define RESOURCE_VENUS_LIGHTHOUSE_AERIE_BACKDROP ((Sint32)&ResourceIdVenusLighthouseAerieBackdrop)
extern char ResourceIdVenusLighthouseBackdrop;
#define RESOURCE_VENUS_LIGHTHOUSE_BACKDROP ((Sint32)&ResourceIdVenusLighthouseBackdrop)
extern char ResourceIdSuhallaDesertBackdrop;
#define RESOURCE_SUHALLA_DESERT_BACKDROP ((Sint32)&ResourceIdSuhallaDesertBackdrop)
extern char ResourceIdAltmillerCaveBackdrop;
#define RESOURCE_ALTMILLER_CAVE_BACKDROP ((Sint32)&ResourceIdAltmillerCaveBackdrop)
extern char ResourceIdTunnelRuinsBackdrop;
#define RESOURCE_TUNNEL_RUINS_BACKDROP ((Sint32)&ResourceIdTunnelRuinsBackdrop)
extern char ResourceIdBabiLighthouseBackdrop;
#define RESOURCE_BABI_LIGHTHOUSE_BACKDROP ((Sint32)&ResourceIdBabiLighthouseBackdrop)
extern char ResourceIdLamakanDesertBackdrop;
#define RESOURCE_LAMAKAN_DESERT_BACKDROP ((Sint32)&ResourceIdLamakanDesertBackdrop)
extern char ResourceIdMogallForestBackdrop;
#define RESOURCE_MOGALL_FOREST_BACKDROP ((Sint32)&ResourceIdMogallForestBackdrop)
extern char ResourceIdMercuryLighthouseAerieBackdrop;
#define RESOURCE_MERCURY_LIGHTHOUSE_AERIE_BACKDROP                                                 \
    ((Sint32)&ResourceIdMercuryLighthouseAerieBackdrop)
extern char ResourceIdTolbiShipRoomsBackdrop;
#define RESOURCE_TOLBI_SHIP_ROOMS_BACKDROP ((Sint32)&ResourceIdTolbiShipRoomsBackdrop)
extern char ResourceIdImilFuchinFallsBackdrop;
#define RESOURCE_IMIL_FUCHIN_FALLS_BACKDROP ((Sint32)&ResourceIdImilFuchinFallsBackdrop)
extern char ResourceIdKolimaForestBackdrop;
#define RESOURCE_KOLIMA_FOREST_BACKDROP ((Sint32)&ResourceIdKolimaForestBackdrop)
extern char ResourceIdTretTreeRoomsBackdrop;
#define RESOURCE_TRET_TREE_ROOMS_BACKDROP ((Sint32)&ResourceIdTretTreeRoomsBackdrop)
extern char ResourceIdRoomsBackdrop;
#define RESOURCE_ROOMS_BACKDROP ((Sint32)&ResourceIdRoomsBackdrop)
extern char ResourceIdAltinPeakBackdrop;
#define RESOURCE_ALTIN_PEAK_BACKDROP ((Sint32)&ResourceIdAltinPeakBackdrop)
extern char ResourceIdTolbiShipDeckBackdrop;
#define RESOURCE_TOLBI_SHIP_DECK_BACKDROP ((Sint32)&ResourceIdTolbiShipDeckBackdrop)
extern char ResourceIdMercuryLighthouseBackdrop;
#define RESOURCE_MERCURY_LIGHTHOUSE_BACKDROP ((Sint32)&ResourceIdMercuryLighthouseBackdrop)
extern char ResourceIdValeStormBackdrop;
#define RESOURCE_VALE_STORM_BACKDROP ((Sint32)&ResourceIdValeStormBackdrop)
extern char ResourceIdTretTreeBranchBackdrop;
#define RESOURCE_TRET_TREE_BRANCH_BACKDROP ((Sint32)&ResourceIdTretTreeBranchBackdrop)
extern char ResourceIdSnowfieldBackdrop;
#define RESOURCE_SNOWFIELD_BACKDROP ((Sint32)&ResourceIdSnowfieldBackdrop)
extern char ResourceIdGrasslandBackdrop;
#define RESOURCE_GRASSLAND_BACKDROP ((Sint32)&ResourceIdGrasslandBackdrop)
extern char ResourceIdSolSanctumBackdrop;
#define RESOURCE_SOL_SANCTUM_BACKDROP ((Sint32)&ResourceIdSolSanctumBackdrop)
// Loaded by btTransitionRun; listed in SceneBattleBackgroundRules.
extern char ResourceIdSerpentStarsBackdrop;
#define RESOURCE_SERPENT_STARS_BACKDROP ((Sint32)&ResourceIdSerpentStarsBackdrop)
// The battle background's star field (btElementGlowInitStarField): its sprites, and its colours,
// which Mercury's star field keeps.
extern char ResourceIdBlueRingSheet;
#define RESOURCE_BLUE_RING_SHEET ((Sint32)&ResourceIdBlueRingSheet)
// The colours of Mars's star field.
extern char ResourceIdRedPaletteC;
#define RESOURCE_RED_PALETTE_C ((Sint32)&ResourceIdRedPaletteC)
// Drawn by Haures; listed in SceneBattleBackgroundRules.
extern char ResourceIdMossCaveBackdrop;
#define RESOURCE_MOSS_CAVE_BACKDROP ((Sint32)&ResourceIdMossCaveBackdrop)
// The lava cave backdrop of the Valukar battle in the Yampi Desert Cave.
extern char ResourceIdLavaCaveBackdrop;
#define RESOURCE_LAVA_CAVE_BACKDROP ((Sint32)&ResourceIdLavaCaveBackdrop)
// The beast-wall backdrop of the Flame Dragons' battle in the Mars Star room.
extern char ResourceIdBeastWallBackdrop;
#define RESOURCE_BEAST_WALL_BACKDROP ((Sint32)&ResourceIdBeastWallBackdrop)
// Loaded by fdBattleBackgroundSelectForMap.
extern char ResourceIdDragonHallABackdrop;
#define RESOURCE_DRAGON_HALL_A_BACKDROP ((Sint32)&ResourceIdDragonHallABackdrop)
// Loaded by fdBattleBackgroundSelectForMap; listed in SceneBattleBackgroundRules.
extern char ResourceIdDragonHallBBackdrop;
#define RESOURCE_DRAGON_HALL_B_BACKDROP ((Sint32)&ResourceIdDragonHallBBackdrop)
// Loaded by fdBattleBackgroundSelectForEnvironment.
extern char ResourceIdGrasslandBBackdrop;
#define RESOURCE_GRASSLAND_B_BACKDROP ((Sint32)&ResourceIdGrasslandBBackdrop)
extern char ResourceIdRockyBeachBackdrop;
#define RESOURCE_ROCKY_BEACH_BACKDROP ((Sint32)&ResourceIdRockyBeachBackdrop)
// Loaded by fdBattleBackgroundSelectForEnvironment; listed in SceneBattleBackgroundRules.
extern char ResourceIdDenseForestBackdrop;
#define RESOURCE_DENSE_FOREST_BACKDROP ((Sint32)&ResourceIdDenseForestBackdrop)
extern char ResourceIdShipDeckBackdrop;
#define RESOURCE_SHIP_DECK_BACKDROP ((Sint32)&ResourceIdShipDeckBackdrop)
// Loaded by fdBattleBackgroundSelectForEnvironment.
extern char ResourceIdSnowfieldBBackdrop;
#define RESOURCE_SNOWFIELD_B_BACKDROP ((Sint32)&ResourceIdSnowfieldBBackdrop)
extern char ResourceIdBlizzardBackdrop;
#define RESOURCE_BLIZZARD_BACKDROP ((Sint32)&ResourceIdBlizzardBackdrop)

// The logos, the intro and the title screen.

// Shown first by mnTitleShowPublisherLogo.
extern char ResourceIdPublisherLogo;
#define RESOURCE_PUBLISHER_LOGO ((Sint32)&ResourceIdPublisherLogo)
// The Camelot logo mnTitleShowCamelotLogo shows after the publisher's.
extern char ResourceIdCamelotLogo;
#define RESOURCE_CAMELOT_LOGO ((Sint32)&ResourceIdCamelotLogo)
// The title's lettering sprites and their colours, which scTitleLoadSprites loads into sprite
// palette 15.
extern char ResourceIdIntroTilesB;
#define RESOURCE_INTRO_TILES_B ((Sint32)&ResourceIdIntroTilesB)
// Loaded by scClearLoadBackground and scTitleLoadBackground.
extern char ResourceIdLogoOnBlack;
#define RESOURCE_LOGO_ON_BLACK ((Sint32)&ResourceIdLogoOnBlack)
// Loaded by mnTitleIntroLoadPictureTiles.
extern char ResourceIdIntroClouds;
#define RESOURCE_INTRO_CLOUDS ((Sint32)&ResourceIdIntroClouds)
// Loaded by mnTitleIntroUpdateLogo.
extern char ResourceIdSubtitlePalette;
#define RESOURCE_SUBTITLE_PALETTE ((Sint32)&ResourceIdSubtitlePalette)
// Logo tiles mnTitleIntroUpdateLogo loads (LOAD_LOGO_TILES) as the logo assembles.
extern char ResourceIdSubtitleA;
#define RESOURCE_SUBTITLE_A ((Sint32)&ResourceIdSubtitleA)
extern char ResourceIdSubtitleB;
#define RESOURCE_SUBTITLE_B ((Sint32)&ResourceIdSubtitleB)
extern char ResourceIdSubtitleC;
#define RESOURCE_SUBTITLE_C ((Sint32)&ResourceIdSubtitleC)
extern char ResourceIdSubtitleD;
#define RESOURCE_SUBTITLE_D ((Sint32)&ResourceIdSubtitleD)
// Loaded by mnTitleIntroShowLighthouse.
extern char ResourceIdIntroLighthousePicture;
#define RESOURCE_INTRO_LIGHTHOUSE_PICTURE ((Sint32)&ResourceIdIntroLighthousePicture)
extern char ResourceIdIntroGullSprites;
#define RESOURCE_INTRO_GULL_SPRITES ((Sint32)&ResourceIdIntroGullSprites)
// Loaded by mnTitleShowTitleScreen.
extern char ResourceIdTitleScreenPicture;
#define RESOURCE_TITLE_SCREEN_PICTURE ((Sint32)&ResourceIdTitleScreenPicture)

// Windows, menus and icons.

// The window font and border tiles. syWindowLoadGraphics copies them into BG character block 0, and
// mnGlyphDraw draws its glyphs from here.
extern char ResourceIdWindowTiles;
#define RESOURCE_WINDOW_TILES ((Sint32)&ResourceIdWindowTiles)
// The command icons; an icon's number is also its name's in MsgCommandName
// (mnCommandIconLoadIntoSlot).
extern char ResourceIdCommandIcons;
#define RESOURCE_COMMAND_ICONS ((Sint32)&ResourceIdCommandIcons)
// Loaded by fdHeadingShow.
extern char ResourceIdHeadings;
#define RESOURCE_HEADINGS ((Sint32)&ResourceIdHeadings)
// Loaded by mnNameEntryEditMemberName and mnNameCaretLoadIntoSlot.
extern char ResourceIdNameCaretTiles;
#define RESOURCE_NAME_CARET_TILES ((Sint32)&ResourceIdNameCaretTiles)
// Loaded by mnIconCreateHandCursor, mnNameEntryEditMemberName, mnPointingHandLoadIntoSlot,
// mnPointingHandLoadIntoFreeSlot and mnShopInitState.
extern char ResourceIdUiIconTiles;
#define RESOURCE_UI_ICON_TILES ((Sint32)&ResourceIdUiIconTiles)
// Loaded by mnOpenBackdropScreen and mnShopInitState.
extern char ResourceIdShopGemTiles;
#define RESOURCE_SHOP_GEM_TILES ((Sint32)&ResourceIdShopGemTiles)
// Loaded by mnLoadDjinnArrows.
extern char ResourceIdItemMenuTilesA;
#define RESOURCE_ITEM_MENU_TILES_A ((Sint32)&ResourceIdItemMenuTilesA)
extern char ResourceIdItemMenuTilesB;
#define RESOURCE_ITEM_MENU_TILES_B ((Sint32)&ResourceIdItemMenuTilesB)
// The small up arrow beside a stat that equipping raises (mnLoadStatArrows and mnShopInitState).
extern char ResourceIdMenuArrowTiles;
#define RESOURCE_MENU_ARROW_TILES ((Sint32)&ResourceIdMenuArrowTiles)
// The small down arrow beside a stat that equipping lowers.
extern char ResourceIdMenuSmallDownArrowTiles;
#define RESOURCE_MENU_SMALL_DOWN_ARROW_TILES ((Sint32)&ResourceIdMenuSmallDownArrowTiles)
// Loaded by mnCursorCreate and mnShopInitState.
extern char ResourceIdShopUpArrowTiles;
#define RESOURCE_SHOP_UP_ARROW_TILES ((Sint32)&ResourceIdShopUpArrowTiles)
extern char ResourceIdShopDownArrowTiles;
#define RESOURCE_SHOP_DOWN_ARROW_TILES ((Sint32)&ResourceIdShopDownArrowTiles)
// Loaded by mnIconLoadWithPalette.
extern char ResourceIdIcons;
#define RESOURCE_ICONS ((Sint32)&ResourceIdIcons)
// Loaded by mnWindowShowFlashFrame and mnWindowStepSwellEffect.
extern char ResourceIdWindowBorderFlash;
#define RESOURCE_WINDOW_BORDER_FLASH ((Sint32)&ResourceIdWindowBorderFlash)
// Loaded by mnSettingsMenuAnimateCursors.
extern char ResourceIdShopCursorPictures;
#define RESOURCE_SHOP_CURSOR_PICTURES ((Sint32)&ResourceIdShopCursorPictures)
// Loaded by mnGlyphDraw.
extern char ResourceIdSmallFont;
#define RESOURCE_SMALL_FONT ((Sint32)&ResourceIdSmallFont)
// Loaded by fdMapScreenUpdateMarkers.
extern char ResourceIdWorldMapPicture;
#define RESOURCE_WORLD_MAP_PICTURE ((Sint32)&ResourceIdWorldMapPicture)
extern char ResourceIdWorldMapPictureLarge;
#define RESOURCE_WORLD_MAP_PICTURE_LARGE ((Sint32)&ResourceIdWorldMapPictureLarge)

// The World Map: its four graphics sets, its cells and its perspective view.

// The colours of the world map's first graphics set (WorldMapGraphicsResources).
extern char ResourceIdWorldMapPaletteA;
#define RESOURCE_WORLD_MAP_PALETTE_A ((Sint32)&ResourceIdWorldMapPaletteA)
// The world map's cell tiles attributes in its first graphics set (WorldMapGraphicsResources).
extern char ResourceIdDefaultMetatileTilesAttributes;
#define RESOURCE_DEFAULT_METATILE_TILES_ATTRIBUTES                                                 \
    ((Sint32)&ResourceIdDefaultMetatileTilesAttributes)
// The colours of the world map's third graphics set (WorldMapGraphicsResources).
extern char ResourceIdWorldMapPaletteB;
#define RESOURCE_WORLD_MAP_PALETTE_B ((Sint32)&ResourceIdWorldMapPaletteB)
// The world map's tile offsets and packed tiles, read by fdWorldMapInitPerspective.
extern char ResourceIdPerspectiveDataA;
#define RESOURCE_PERSPECTIVE_DATA_A ((Sint32)&ResourceIdPerspectiveDataA)
// Loaded by fdWorldMapInitPerspective, fdWorldMapLoadAreaGraphics and
// fdWorldMapLoadDefaultMetatilesAndUpdateBlock.
extern char ResourceIdDefaultMapMetatiles;
#define RESOURCE_DEFAULT_MAP_METATILES ((Sint32)&ResourceIdDefaultMapMetatiles)
// The tile animation of world map set A: a row of WorldMapGraphicsResources.
extern char ResourceIdDefaultMapAnimation;
#define RESOURCE_DEFAULT_MAP_ANIMATION ((Sint32)&ResourceIdDefaultMapAnimation)
// Tiles 1 of 6 of world map set A, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapTilesA1;
#define RESOURCE_WORLD_MAP_TILES_A1 ((Sint32)&ResourceIdWorldMapTilesA1)
// Tiles 2 of 6 of world map set A, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapTilesA2;
#define RESOURCE_WORLD_MAP_TILES_A2 ((Sint32)&ResourceIdWorldMapTilesA2)
// Tiles 3 of 6 of world map set A, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapTilesA3;
#define RESOURCE_WORLD_MAP_TILES_A3 ((Sint32)&ResourceIdWorldMapTilesA3)
// Tiles 4 of 6 of world map set A, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapTilesA4;
#define RESOURCE_WORLD_MAP_TILES_A4 ((Sint32)&ResourceIdWorldMapTilesA4)
// Tiles 5 of 6 of world map set A, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapTilesA5;
#define RESOURCE_WORLD_MAP_TILES_A5 ((Sint32)&ResourceIdWorldMapTilesA5)
// Tiles 6 of 6 of world map set A, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapTilesA6;
#define RESOURCE_WORLD_MAP_TILES_A6 ((Sint32)&ResourceIdWorldMapTilesA6)
// The palette of world map set C, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapPaletteC;
#define RESOURCE_WORLD_MAP_PALETTE_C ((Sint32)&ResourceIdWorldMapPaletteC)
// The tile animation of world map set C, in DefaultMapAnimation's format: a row of
// WorldMapGraphicsResources.
extern char ResourceIdWorldMapAnimationC;
#define RESOURCE_WORLD_MAP_ANIMATION_C ((Sint32)&ResourceIdWorldMapAnimationC)
// The cell tiles attributes of world map set C, in DefaultMetatileTilesAttributes' format: a row of
// WorldMapGraphicsResources.
extern char ResourceIdWorldMapMetatileTilesAttributesC;
#define RESOURCE_WORLD_MAP_METATILE_TILES_ATTRIBUTES_C                                             \
    ((Sint32)&ResourceIdWorldMapMetatileTilesAttributesC)
// Tiles 1 of 6 of world map set C, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapTilesC1;
#define RESOURCE_WORLD_MAP_TILES_C1 ((Sint32)&ResourceIdWorldMapTilesC1)
// Tiles 2 of 6 of world map set C, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapTilesC2;
#define RESOURCE_WORLD_MAP_TILES_C2 ((Sint32)&ResourceIdWorldMapTilesC2)
// Tiles 3 of 6 of world map set C, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapTilesC3;
#define RESOURCE_WORLD_MAP_TILES_C3 ((Sint32)&ResourceIdWorldMapTilesC3)
// Tiles 4 of 6 of world map set C, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapTilesC4;
#define RESOURCE_WORLD_MAP_TILES_C4 ((Sint32)&ResourceIdWorldMapTilesC4)
// Tiles 5 of 6 of world map set C, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapTilesC5;
#define RESOURCE_WORLD_MAP_TILES_C5 ((Sint32)&ResourceIdWorldMapTilesC5)
// Tiles 6 of 6 of world map set C, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapTilesC6;
#define RESOURCE_WORLD_MAP_TILES_C6 ((Sint32)&ResourceIdWorldMapTilesC6)
// The tile animation of world map set B, in DefaultMapAnimation's format: a row of
// WorldMapGraphicsResources.
extern char ResourceIdWorldMapAnimationB;
#define RESOURCE_WORLD_MAP_ANIMATION_B ((Sint32)&ResourceIdWorldMapAnimationB)
// The cell tiles attributes of world map set B, in DefaultMetatileTilesAttributes' format: a row of
// WorldMapGraphicsResources.
extern char ResourceIdWorldMapMetatileTilesAttributesB;
#define RESOURCE_WORLD_MAP_METATILE_TILES_ATTRIBUTES_B                                             \
    ((Sint32)&ResourceIdWorldMapMetatileTilesAttributesB)
// Tiles 1 of 6 of world map set B, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapTilesB1;
#define RESOURCE_WORLD_MAP_TILES_B1 ((Sint32)&ResourceIdWorldMapTilesB1)
// Tiles 2 of 6 of world map set B, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapTilesB2;
#define RESOURCE_WORLD_MAP_TILES_B2 ((Sint32)&ResourceIdWorldMapTilesB2)
// Tiles 3 of 6 of world map set B, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapTilesB3;
#define RESOURCE_WORLD_MAP_TILES_B3 ((Sint32)&ResourceIdWorldMapTilesB3)
// Tiles 4 of 6 of world map set B, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapTilesB4;
#define RESOURCE_WORLD_MAP_TILES_B4 ((Sint32)&ResourceIdWorldMapTilesB4)
// Tiles 5 of 6 of world map set B, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapTilesB5;
#define RESOURCE_WORLD_MAP_TILES_B5 ((Sint32)&ResourceIdWorldMapTilesB5)
// Tiles 6 of 6 of world map set B, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapTilesB6;
#define RESOURCE_WORLD_MAP_TILES_B6 ((Sint32)&ResourceIdWorldMapTilesB6)
// The palette of world map set D, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapPaletteD;
#define RESOURCE_WORLD_MAP_PALETTE_D ((Sint32)&ResourceIdWorldMapPaletteD)
// The tile animation of world map set D, in DefaultMapAnimation's format: a row of
// WorldMapGraphicsResources.
extern char ResourceIdWorldMapAnimationD;
#define RESOURCE_WORLD_MAP_ANIMATION_D ((Sint32)&ResourceIdWorldMapAnimationD)
// The cell tiles attributes of world map set D, in DefaultMetatileTilesAttributes' format: a row of
// WorldMapGraphicsResources.
extern char ResourceIdWorldMapMetatileTilesAttributesD;
#define RESOURCE_WORLD_MAP_METATILE_TILES_ATTRIBUTES_D                                             \
    ((Sint32)&ResourceIdWorldMapMetatileTilesAttributesD)
// Tiles 1 of 6 of world map set D, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapTilesD1;
#define RESOURCE_WORLD_MAP_TILES_D1 ((Sint32)&ResourceIdWorldMapTilesD1)
// Tiles 2 of 6 of world map set D, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapTilesD2;
#define RESOURCE_WORLD_MAP_TILES_D2 ((Sint32)&ResourceIdWorldMapTilesD2)
// Tiles 3 of 6 of world map set D, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapTilesD3;
#define RESOURCE_WORLD_MAP_TILES_D3 ((Sint32)&ResourceIdWorldMapTilesD3)
// Tiles 4 of 6 of world map set D, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapTilesD4;
#define RESOURCE_WORLD_MAP_TILES_D4 ((Sint32)&ResourceIdWorldMapTilesD4)
// Tiles 5 of 6 of world map set D, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapTilesD5;
#define RESOURCE_WORLD_MAP_TILES_D5 ((Sint32)&ResourceIdWorldMapTilesD5)
// Tiles 6 of 6 of world map set D, a row of WorldMapGraphicsResources.
extern char ResourceIdWorldMapTilesD6;
#define RESOURCE_WORLD_MAP_TILES_D6 ((Sint32)&ResourceIdWorldMapTilesD6)
// Loaded by fdWorldMapInitPerspective.
extern char ResourceIdDefaultMapEntrances;
#define RESOURCE_DEFAULT_MAP_ENTRANCES ((Sint32)&ResourceIdDefaultMapEntrances)

// Field effects: the field Psynergy and the scenes' own effects.

// Loaded by fdParticleBurstStart.
extern char ResourceIdLeaves;
#define RESOURCE_LEAVES ((Sint32)&ResourceIdLeaves)
// Loaded by fdActorAppearInSparks, fdPsynergyEndRing and fdPsynergyLash.
extern char ResourceIdSparkleFade;
#define RESOURCE_SPARKLE_FADE ((Sint32)&ResourceIdSparkleFade)
// Loaded by scGabombaStatueStartClock.
extern char ResourceIdClockDigits;
#define RESOURCE_CLOCK_DIGITS ((Sint32)&ResourceIdClockDigits)
// Loaded by scAirsRockWhirlwindClearsFog, scAirsRockWhirlwindClearsWall and
// scShamanVillageBlowAwaySand.
extern char ResourceIdWhirlwindMap;
#define RESOURCE_WHIRLWIND_MAP ((Sint32)&ResourceIdWhirlwindMap)
extern char ResourceIdWhirlwindTiles;
#define RESOURCE_WHIRLWIND_TILES ((Sint32)&ResourceIdWhirlwindTiles)
// Whirlwind's picture is drawn on BG0, the windows' layer: its tiles, two rows of
// WHIRLWIND_ROW_TILES (the top halves, then the bottom ones), go over BG tiles WHIRLWIND_TOP_TILE
// and WHIRLWIND_BOTTOM_TILE of character block 3, and its map, WHIRLWIND_MAP_WIDTH by
// WHIRLWIND_MAP_HEIGHT cells numbering them from WHIRLWIND_MAP_FIRST_TILE, into the windows' screen
// block, whose other entries take WHIRLWIND_FILL_TILE.
#define WHIRLWIND_ROW_TILES      26
#define WHIRLWIND_TOP_TILE       320
#define WHIRLWIND_BOTTOM_TILE    352
#define WHIRLWIND_MAP_FIRST_TILE 512
#define WHIRLWIND_MAP_WIDTH      16
#define WHIRLWIND_MAP_HEIGHT     18
#define WHIRLWIND_FILL_TILE      16
// Whirlwind's sway (field/field_effect_wave_scroll.c): each line of its picture swings by half its
// height above WAVE_SCROLL_CALM_LINE, the picture's foot, and a line swung past WAVE_SCROLL_MAX
// pixels one way or WAVE_SCROLL_MIN the other is pulled back by as much.
#define WAVE_SCROLL_CALM_LINE    (WHIRLWIND_MAP_HEIGHT * 8)
#define WAVE_SCROLL_MAX          56
#define WAVE_SCROLL_MIN          (-64)
// Loaded by scAirsRockInsideWakeHallStatue, scAirsRockInsideWakeVialChestStatue,
// scAirsRockWhirlwindOpensTheTop and scJupiterLighthouseFlashSky.
extern char ResourceIdLightningMap;
#define RESOURCE_LIGHTNING_MAP ((Sint32)&ResourceIdLightningMap)
extern char ResourceIdLightningTiles;
#define RESOURCE_LIGHTNING_TILES ((Sint32)&ResourceIdLightningTiles)
// Loaded by scChampaCliffCavesStartFlash, fdPsynergyBurst, scLohoFireCannon and
// scNorthernReachesShatterIceWall.
extern char ResourceIdFieldEffectExplosion;
#define RESOURCE_FIELD_EFFECT_EXPLOSION ((Sint32)&ResourceIdFieldEffectExplosion)
// Loaded by ChampaCliffCavesLoadDisc, fdPsynergyBurst, fdPsynergyParch and fdPsynergyPound.
extern char ResourceIdGlowDisc;
#define RESOURCE_GLOW_DISC ((Sint32)&ResourceIdGlowDisc)
// Loaded by scNorthernReachesShatterIceWall.
extern char ResourceIdFieldEffectIceShard;
#define RESOURCE_FIELD_EFFECT_ICE_SHARD ((Sint32)&ResourceIdFieldEffectIceShard)
// Loaded by scEasternAlhafraStartFallingRocks, scMagmaRockInsideStartFallingRocks,
// scMagmaRockStartFallingRocks, scProxStartFallingRocks, dbPsynergyTest2StartRocks and
// scTundariaTowerStartFallingRocks.
extern char ResourceIdFallingRock;
#define RESOURCE_FALLING_ROCK ((Sint32)&ResourceIdFallingRock)
// Loaded by fdPsynergyBlaze, fdPsynergyEndSand, fdPsynergySand, fdPsynergyScoop and
// scVenusLighthouseEntranceStartDust.
extern char ResourceIdDustPuffA;
#define RESOURCE_DUST_PUFF_A ((Sint32)&ResourceIdDustPuffA)
// Loaded by scJupiterLighthouseStartSparkRing, fdTeleportPlaySparks and fdTeleportPlayWhirl.
extern char ResourceIdColouredOrbs;
#define RESOURCE_COLOURED_ORBS ((Sint32)&ResourceIdColouredOrbs)
// Loaded by fdPsynergyLash, fdPsynergyRing and scJupiterLighthouseStartBeam.
extern char ResourceIdStarSparkleFade;
#define RESOURCE_STAR_SPARKLE_FADE ((Sint32)&ResourceIdStarSparkleFade)
// Loaded by fdPsynergyGrindOnTarget.
extern char ResourceIdWaterSplash;
#define RESOURCE_WATER_SPLASH ((Sint32)&ResourceIdWaterSplash)
// Loaded by fdPsynergyHover.
extern char ResourceIdRingOrb;
#define RESOURCE_RING_ORB ((Sint32)&ResourceIdRingOrb)
// The two 8x8 tiles of the sprites Tremor shakes (FieldEffectShakeTiles), loaded by
// fdPsynergyTremorOnTarget.
extern char ResourceIdShakeTiles;
#define RESOURCE_SHAKE_TILES ((Sint32)&ResourceIdShakeTiles)
// Loaded by scChampaCliffCavesLoadColumn.
extern char ResourceIdFireColumn;
#define RESOURCE_FIRE_COLUMN ((Sint32)&ResourceIdFireColumn)
// Loaded by scJupiterLighthouseStartBeam.
extern char ResourceIdGoldenTrident;
#define RESOURCE_GOLDEN_TRIDENT ((Sint32)&ResourceIdGoldenTrident)
// Loaded by scShamanVillageLoadLettering.
extern char ResourceIdLetteringBanner;
#define RESOURCE_LETTERING_BANNER ((Sint32)&ResourceIdLetteringBanner)
// Loaded by fdPsynergyBlaze.
extern char ResourceIdRedFlameBurst;
#define RESOURCE_RED_FLAME_BURST ((Sint32)&ResourceIdRedFlameBurst)
extern char ResourceIdVioletFlameBurst;
#define RESOURCE_VIOLET_FLAME_BURST ((Sint32)&ResourceIdVioletFlameBurst)
extern char ResourceIdYellowFlameBurst;
#define RESOURCE_YELLOW_FLAME_BURST ((Sint32)&ResourceIdYellowFlameBurst)
extern char ResourceIdBlueFlameBurst;
#define RESOURCE_BLUE_FLAME_BURST ((Sint32)&ResourceIdBlueFlameBurst)
// Loaded by fdPsynergyParch.
extern char ResourceIdWhitePuff;
#define RESOURCE_WHITE_PUFF ((Sint32)&ResourceIdWhitePuff)
// Loaded by fdPsynergyPound.
extern char ResourceIdGoldStar;
#define RESOURCE_GOLD_STAR ((Sint32)&ResourceIdGoldStar)
// Loaded by fdPsynergyArrow.
extern char ResourceIdBowDraw;
#define RESOURCE_BOW_DRAW ((Sint32)&ResourceIdBowDraw)
extern char ResourceIdFlamingArrow;
#define RESOURCE_FLAMING_ARROW ((Sint32)&ResourceIdFlamingArrow)
// Loaded by fdPsynergyScoop.
extern char ResourceIdDustPuffB;
#define RESOURCE_DUST_PUFF_B ((Sint32)&ResourceIdDustPuffB)
extern char ResourceIdWhiteSparkle;
#define RESOURCE_WHITE_SPARKLE ((Sint32)&ResourceIdWhiteSparkle)
extern char ResourceIdScoopEarth;
#define RESOURCE_SCOOP_EARTH ((Sint32)&ResourceIdScoopEarth)
// Loaded by fdSummonTabletPlayRunes.
extern char ResourceIdRunes;
#define RESOURCE_RUNES ((Sint32)&ResourceIdRunes)
extern char ResourceIdPebbles;
#define RESOURCE_PEBBLES ((Sint32)&ResourceIdPebbles)
// Loaded by fdPsynergyRing.
extern char ResourceIdRingArcs;
#define RESOURCE_RING_ARCS ((Sint32)&ResourceIdRingArcs)
extern char ResourceIdRingWaves;
#define RESOURCE_RING_WAVES ((Sint32)&ResourceIdRingWaves)

// Lucky Dice, Lucky Wheels and Super Lucky Dice.

// The Lucky Dice board, with its colours, on the background (fdLuckyDiceRun).
extern char ResourceIdLuckyDiceBoard;
#define RESOURCE_LUCKY_DICE_BOARD ((Sint32)&ResourceIdLuckyDiceBoard)
// The Lucky Dice sprites and their colours (fdLuckyDiceRun).
extern char ResourceIdLuckyDiceSprites;
#define RESOURCE_LUCKY_DICE_SPRITES ((Sint32)&ResourceIdLuckyDiceSprites)
// Loaded by fdLuckyWheelsRun.
extern char ResourceIdSlotsMachinePicture;
#define RESOURCE_SLOTS_MACHINE_PICTURE ((Sint32)&ResourceIdSlotsMachinePicture)
extern char ResourceIdSlotsIcons;
#define RESOURCE_SLOTS_ICONS ((Sint32)&ResourceIdSlotsIcons)
extern char ResourceIdSlotsJackpotBanner;
#define RESOURCE_SLOTS_JACKPOT_BANNER ((Sint32)&ResourceIdSlotsJackpotBanner)
// Super Lucky Dice's dome, its dice and its digits and banners (fdSuperLuckyDiceRun).
extern char ResourceIdSlotsDomePicture;
#define RESOURCE_SLOTS_DOME_PICTURE ((Sint32)&ResourceIdSlotsDomePicture)
extern char ResourceIdDiceFaces;
#define RESOURCE_DICE_FACES ((Sint32)&ResourceIdDiceFaces)
extern char ResourceIdSlotsSmallDoubleUpBanner;
#define RESOURCE_SLOTS_SMALL_DOUBLE_UP_BANNER ((Sint32)&ResourceIdSlotsSmallDoubleUpBanner)
extern char ResourceIdSlotsFinalGameBanner;
#define RESOURCE_SLOTS_FINAL_GAME_BANNER ((Sint32)&ResourceIdSlotsFinalGameBanner)
extern char ResourceIdSlotsOrangeDigits;
#define RESOURCE_SLOTS_ORANGE_DIGITS ((Sint32)&ResourceIdSlotsOrangeDigits)
extern char ResourceIdSlotsTimesSign;
#define RESOURCE_SLOTS_TIMES_SIGN ((Sint32)&ResourceIdSlotsTimesSign)
extern char ResourceIdSlotsPinkDigits;
#define RESOURCE_SLOTS_PINK_DIGITS ((Sint32)&ResourceIdSlotsPinkDigits)
extern char ResourceIdSlotsHigherBanner;
#define RESOURCE_SLOTS_HIGHER_BANNER ((Sint32)&ResourceIdSlotsHigherBanner)
extern char ResourceIdSlotsLowerBanner;
#define RESOURCE_SLOTS_LOWER_BANNER ((Sint32)&ResourceIdSlotsLowerBanner)
extern char ResourceIdSlotsYouLoseBanner;
#define RESOURCE_SLOTS_YOU_LOSE_BANNER ((Sint32)&ResourceIdSlotsYouLoseBanner)
extern char ResourceIdSlotsYouWinBanner;
#define RESOURCE_SLOTS_YOU_WIN_BANNER ((Sint32)&ResourceIdSlotsYouWinBanner)
extern char ResourceIdSlotsOnePairBanner;
#define RESOURCE_SLOTS_ONE_PAIR_BANNER ((Sint32)&ResourceIdSlotsOnePairBanner)
extern char ResourceIdSlotsTwoPairBanner;
#define RESOURCE_SLOTS_TWO_PAIR_BANNER ((Sint32)&ResourceIdSlotsTwoPairBanner)
extern char ResourceIdSlotsTripleBanner;
#define RESOURCE_SLOTS_TRIPLE_BANNER ((Sint32)&ResourceIdSlotsTripleBanner)
extern char ResourceIdSlotsPerfectBanner;
#define RESOURCE_SLOTS_PERFECT_BANNER ((Sint32)&ResourceIdSlotsPerfectBanner)
extern char ResourceIdSlotsGreenDigits;
#define RESOURCE_SLOTS_GREEN_DIGITS ((Sint32)&ResourceIdSlotsGreenDigits)
extern char ResourceIdSlotsDoubleUpBanner;
#define RESOURCE_SLOTS_DOUBLE_UP_BANNER ((Sint32)&ResourceIdSlotsDoubleUpBanner)
extern char ResourceIdSlotsCashOutBanner;
#define RESOURCE_SLOTS_CASH_OUT_BANNER ((Sint32)&ResourceIdSlotsCashOutBanner)

// The ending and the credits.

// The green panel inside the epilogue's portrait frame (EpilogueLoadNamePanel).
extern char ResourceIdGreenPanel;
#define RESOURCE_GREEN_PANEL ((Sint32)&ResourceIdGreenPanel)
// The ending's pictures, each 512 pixels wide, which the epilogue pans across under its lines
// (mnEndingImageShow). The first: three men on a cliff above the plain by day.
extern char ResourceIdEndingCliffDayPicture;
#define RESOURCE_ENDING_CLIFF_DAY_PICTURE ((Sint32)&ResourceIdEndingCliffDayPicture)
// The three on the cliff at sunset.
extern char ResourceIdEndingCliffSunsetPicture;
#define RESOURCE_ENDING_CLIFF_SUNSET_PICTURE ((Sint32)&ResourceIdEndingCliffSunsetPicture)
// The villagers among autumn trees, Dora alone on the right.
extern char ResourceIdEndingVillagersPictureA;
#define RESOURCE_ENDING_VILLAGERS_PICTURE_A ((Sint32)&ResourceIdEndingVillagersPictureA)
// The embrace, held until a button is pressed.
extern char ResourceIdEndingEmbracePicture;
#define RESOURCE_ENDING_EMBRACE_PICTURE ((Sint32)&ResourceIdEndingEmbracePicture)
// The party together, panned along.
extern char ResourceIdEndingReunionPicture;
#define RESOURCE_ENDING_REUNION_PICTURE ((Sint32)&ResourceIdEndingReunionPicture)
// The plain at sunset.
extern char ResourceIdEndingSunsetPlainPicture;
#define RESOURCE_ENDING_SUNSET_PLAIN_PICTURE ((Sint32)&ResourceIdEndingSunsetPlainPicture)
// The plain at sunset, with "The End" in each language.
extern char ResourceIdEndingTheEndPicture;
#define RESOURCE_ENDING_THE_END_PICTURE ((Sint32)&ResourceIdEndingTheEndPicture)

// Data that is no picture.

// The build's date and time as text, different in each edition; syCopyBuildStamp copies it into
// BuildStampWork.
extern char ResourceIdBuildStamp;
#define RESOURCE_BUILD_STAMP ((Sint32)&ResourceIdBuildStamp)
// Stub tables, which the title, the credits and the dice and wheel games hand to
// syResourceReservedNoOperation, which ignores them.
extern char ResourceIdTitleStubs;
#define RESOURCE_TITLE_STUBS ((Sint32)&ResourceIdTitleStubs)
extern char ResourceIdCreditsStubs;
#define RESOURCE_CREDITS_STUBS ((Sint32)&ResourceIdCreditsStubs)
extern char ResourceIdBattleEffectStubs;
#define RESOURCE_BATTLE_EFFECT_STUBS ((Sint32)&ResourceIdBattleEffectStubs)
// The sprites' animation data, which the debug sprite viewer passes to a stub (fdSpriteViewerRun).
extern char ResourceIdSpriteMetadata;
#define RESOURCE_SPRITE_METADATA ((Sint32)&ResourceIdSpriteMetadata)
// Loaded by fdTileMapConvertBuffer.
extern char ResourceIdEmptyMap;
#define RESOURCE_EMPTY_MAP ((Sint32)&ResourceIdEmptyMap)

// The battle backdrops that SceneBattleBackgroundRules chooses, named by their pictures.
extern char ResourceIdCanyonMeadowBackdrop;
#define RESOURCE_CANYON_MEADOW_BACKDROP ((Sint32)&ResourceIdCanyonMeadowBackdrop)
extern char ResourceIdFireCavePoolBackdrop;
#define RESOURCE_FIRE_CAVE_POOL_BACKDROP ((Sint32)&ResourceIdFireCavePoolBackdrop)
extern char ResourceIdBlueCaveBackdrop;
#define RESOURCE_BLUE_CAVE_BACKDROP ((Sint32)&ResourceIdBlueCaveBackdrop)
extern char ResourceIdRedMesaBackdrop;
#define RESOURCE_RED_MESA_BACKDROP ((Sint32)&ResourceIdRedMesaBackdrop)
extern char ResourceIdPinkRocksBackdrop;
#define RESOURCE_PINK_ROCKS_BACKDROP ((Sint32)&ResourceIdPinkRocksBackdrop)
extern char ResourceIdPinkTotemBackdrop;
#define RESOURCE_PINK_TOTEM_BACKDROP ((Sint32)&ResourceIdPinkTotemBackdrop)
extern char ResourceIdStoneTownBackdrop;
#define RESOURCE_STONE_TOWN_BACKDROP ((Sint32)&ResourceIdStoneTownBackdrop)
extern char ResourceIdPalmCliffBackdrop;
#define RESOURCE_PALM_CLIFF_BACKDROP ((Sint32)&ResourceIdPalmCliffBackdrop)
extern char ResourceIdToyRoomBackdrop;
#define RESOURCE_TOY_ROOM_BACKDROP ((Sint32)&ResourceIdToyRoomBackdrop)
extern char ResourceIdShipCabinBackdrop;
#define RESOURCE_SHIP_CABIN_BACKDROP ((Sint32)&ResourceIdShipCabinBackdrop)
extern char ResourceIdGoldRocksBackdrop;
#define RESOURCE_GOLD_ROCKS_BACKDROP ((Sint32)&ResourceIdGoldRocksBackdrop)
extern char ResourceIdStoneCorridorBackdrop;
#define RESOURCE_STONE_CORRIDOR_BACKDROP ((Sint32)&ResourceIdStoneCorridorBackdrop)
extern char ResourceIdFaceTempleBackdrop;
#define RESOURCE_FACE_TEMPLE_BACKDROP ((Sint32)&ResourceIdFaceTempleBackdrop)
extern char ResourceIdMistyHillsBackdrop;
#define RESOURCE_MISTY_HILLS_BACKDROP ((Sint32)&ResourceIdMistyHillsBackdrop)
extern char ResourceIdWaterfallGorgeBackdrop;
#define RESOURCE_WATERFALL_GORGE_BACKDROP ((Sint32)&ResourceIdWaterfallGorgeBackdrop)
extern char ResourceIdBlueRuinsBackdrop;
#define RESOURCE_BLUE_RUINS_BACKDROP ((Sint32)&ResourceIdBlueRuinsBackdrop)
extern char ResourceIdGreenRuinsBackdrop;
#define RESOURCE_GREEN_RUINS_BACKDROP ((Sint32)&ResourceIdGreenRuinsBackdrop)
extern char ResourceIdStatueCliffBackdrop;
#define RESOURCE_STATUE_CLIFF_BACKDROP ((Sint32)&ResourceIdStatueCliffBackdrop)
extern char ResourceIdLavaTempleBackdrop;
#define RESOURCE_LAVA_TEMPLE_BACKDROP ((Sint32)&ResourceIdLavaTempleBackdrop)
extern char ResourceIdArrowBeachBackdrop;
#define RESOURCE_ARROW_BEACH_BACKDROP ((Sint32)&ResourceIdArrowBeachBackdrop)
extern char ResourceIdStormySeaBackdrop;
#define RESOURCE_STORMY_SEA_BACKDROP ((Sint32)&ResourceIdStormySeaBackdrop)
extern char ResourceIdAngelHallBackdrop;
#define RESOURCE_ANGEL_HALL_BACKDROP ((Sint32)&ResourceIdAngelHallBackdrop)
extern char ResourceIdPurpleLighthouseBackdrop;
#define RESOURCE_PURPLE_LIGHTHOUSE_BACKDROP ((Sint32)&ResourceIdPurpleLighthouseBackdrop)
extern char ResourceIdStormyLighthouseBackdrop;
#define RESOURCE_STORMY_LIGHTHOUSE_BACKDROP ((Sint32)&ResourceIdStormyLighthouseBackdrop)
extern char ResourceIdIceHallBackdrop;
#define RESOURCE_ICE_HALL_BACKDROP ((Sint32)&ResourceIdIceHallBackdrop)
extern char ResourceIdOrbAltarBackdrop;
#define RESOURCE_ORB_ALTAR_BACKDROP ((Sint32)&ResourceIdOrbAltarBackdrop)
extern char ResourceIdPurpleCaveBackdrop;
#define RESOURCE_PURPLE_CAVE_BACKDROP ((Sint32)&ResourceIdPurpleCaveBackdrop)
extern char ResourceIdGorgePoolBackdrop;
#define RESOURCE_GORGE_POOL_BACKDROP ((Sint32)&ResourceIdGorgePoolBackdrop)
extern char ResourceIdSpikeHallBackdrop;
#define RESOURCE_SPIKE_HALL_BACKDROP ((Sint32)&ResourceIdSpikeHallBackdrop)

// Moapa's recorded runs through the Trial Road's stages, in TrialRoadRivalRecordings' order, which
// scTrialRoadPlayRivalRecording plays.
extern char ResourceIdTrialRoadPillarRecording;    // the pillar stage
#define RESOURCE_TRIAL_ROAD_PILLAR_RECORDING ((Sint32)&ResourceIdTrialRoadPillarRecording)
extern char ResourceIdTrialRoadTorchRecording;     // the torch stage
#define RESOURCE_TRIAL_ROAD_TORCH_RECORDING ((Sint32)&ResourceIdTrialRoadTorchRecording)
extern char ResourceIdTrialRoadWeedsRecording;     // the weeds stage
#define RESOURCE_TRIAL_ROAD_WEEDS_RECORDING ((Sint32)&ResourceIdTrialRoadWeedsRecording)
extern char ResourceIdTrialRoadPoundRecording;     // the Pound stage
#define RESOURCE_TRIAL_ROAD_POUND_RECORDING ((Sint32)&ResourceIdTrialRoadPoundRecording)
extern char ResourceIdTrialRoadCrumblingRecording; // the crumbling stage
#define RESOURCE_TRIAL_ROAD_CRUMBLING_RECORDING ((Sint32)&ResourceIdTrialRoadCrumblingRecording)
extern char ResourceIdTrialRoadLogRecording;       // the log stage
#define RESOURCE_TRIAL_ROAD_LOG_RECORDING ((Sint32)&ResourceIdTrialRoadLogRecording)
extern char ResourceIdTrialRoadFountainRecording;  // the fountain stage
#define RESOURCE_TRIAL_ROAD_FOUNTAIN_RECORDING ((Sint32)&ResourceIdTrialRoadFountainRecording)
extern char ResourceIdTrialRoadSandRecording;      // the sand stage
#define RESOURCE_TRIAL_ROAD_SAND_RECORDING ((Sint32)&ResourceIdTrialRoadSandRecording)
// the chests, Moapa at the switch first
extern char ResourceIdTrialRoadChestsMoapaFirstRecording;
#define RESOURCE_TRIAL_ROAD_CHESTS_MOAPA_FIRST_RECORDING                                           \
    ((Sint32)&ResourceIdTrialRoadChestsMoapaFirstRecording)
// the chests, Felix at the switch first
extern char ResourceIdTrialRoadChestsFelixFirstRecording;
#define RESOURCE_TRIAL_ROAD_CHESTS_FELIX_FIRST_RECORDING                                           \
    ((Sint32)&ResourceIdTrialRoadChestsFelixFirstRecording)

#endif
