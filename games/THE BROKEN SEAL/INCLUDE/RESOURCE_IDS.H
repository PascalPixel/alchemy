#ifndef ALCHEMY_RESOURCE_IDS_H
#define ALCHEMY_RESOURCE_IDS_H

/*
 * Resource numbers are rows of the resource directory
 * (SYSTEM/RESOURCE/DIRECTORY.S), which names each row after what it holds.
 * A name's value is its row number, so it is a link-time value: take its
 * address, and the compiler loads the number from the literal pool as the
 * game does. A switch cannot use one as a case label.
 */
extern char ResourceId_BuildStamp;
extern char ResourceId_PaletteFarCalls;
extern char ResourceId_WindowTiles;
extern char ResourceId_NintendoLogo;
extern char ResourceId_GoldenSunLogo;
extern char ResourceId_WorldMapPicture;
extern char ResourceId_IntroTilesB;
extern char ResourceId_VinasuChojoStormBackdrop;
extern char ResourceId_BeachBackdrop;
extern char ResourceId_SnowfieldBackdrop;
extern char ResourceId_GrasslandBackdrop;
extern char ResourceId_SoruShindenBackdrop;
extern char ResourceId_ForestBackdrop;
extern char ResourceId_VioletSkyBackdrop;
extern char ResourceId_DesertBackdrop;
extern char ResourceId_DuskCloudsBackdrop;
extern char ResourceId_RevealMaskA;
extern char ResourceId_RevealMaskB;
extern char ResourceId_RevealMaskC;
extern char ResourceId_RevealMaskD;
extern char ResourceId_RevealMaskE;
extern char ResourceId_RevealMaskF;
extern char ResourceId_LimeColumnSheet;
extern char ResourceId_DemonFaceSheet;
extern char ResourceId_VioletLightningSheetA;
extern char ResourceId_TargetBurstImage;
extern char ResourceId_WaterSpraySheet;
extern char ResourceId_BlueFlameSheet;
extern char ResourceId_ParticleSpritesA;
extern char ResourceId_ParticleSpritesD;
extern char ResourceId_FlameColumnSheet;
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_ThornSheet;
extern char ResourceId_GreenPillarSheet;
extern char ResourceId_VineSheet;
extern char ResourceId_StarDotSheet;
extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_MercuryDjinnSmallSheet;
extern char ResourceId_SmokeSheet;
extern char ResourceId_PinkPalette;
extern char ResourceId_GreenVineSheet;
extern char ResourceId_CounterRevealSheet;
extern char ResourceId_RedCrescentSheetA;
extern char ResourceId_SpiralSheet;
extern char ResourceId_EmberStreakSheet;
extern char ResourceId_IceChipSheet;
extern char ResourceId_IceBlockSheet;
extern char ResourceId_SparkleDots;
extern char ResourceId_PinkBurstSheet;
extern char ResourceId_DustPuffSheet;
extern char ResourceId_BlastSheet;
extern char ResourceId_YellowPaletteB;
extern char ResourceId_VioletPaletteD;
extern char ResourceId_TornadoSheet;
extern char ResourceId_CrystalSheet;
extern char ResourceId_PerspectiveDataA;
extern char ResourceId_DefaultMapCells;
extern char ResourceId_RivalPathA;
extern char ResourceId_DefaultMapAnimation;
extern char ResourceId_DefaultMetatileAttributes;
extern char ResourceId_RivalPathB;
extern char ResourceId_RivalPathC;
extern char ResourceId_Lettering;
extern char ResourceId_ShopCursorFrames;
extern char ResourceId_Icons;
extern char ResourceId_CommandIcons;
extern char ResourceId_SheepSheet;
extern char ResourceId_MagentaSwirlSheet;
extern char ResourceId_MagentaTailSheet;
extern char ResourceId_LimePalette;

extern char ResourceId_FireStreakSheet;
extern char ResourceId_YellowOrbSheet;
extern char ResourceId_SkullSheet;
extern char ResourceId_LightningBoltSheet;
extern char ResourceId_RuneSheet;
extern char ResourceId_RockSpireSheet;
extern char ResourceId_FlameSheetA;
extern char ResourceId_VenusDjinnSmallSheet;
extern char ResourceId_MarsDjinnSmallSheet;
extern char ResourceId_JupiterDjinnSmallSheet;
extern char ResourceId_BlueBeastSheet;
extern char ResourceId_RedCrescentSheetB;
extern char ResourceId_TanPalette;
extern char ResourceId_CyanPalette;
#endif
