.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.global gWhirlwindRiding
gWhirlwindRiding:
	.space 0x00000004
	.global gPaletteCycleColors
gPaletteCycleColors:
	.space 0x00000020
	.global gPaletteCycleUnlitPalette
gPaletteCycleUnlitPalette:
	.space 0x00000004
	.global gPaletteCycleUnlitColors
gPaletteCycleUnlitColors:
	.space 0x0000001c
	.global gLightSpriteTile
gLightSpriteTile:
	.space 0x00000002
	.global gLightSpriteCount
gLightSpriteCount:
	.space 0x00000002
	.global gLightSpriteActors
gLightSpriteActors:
	.space 0x00000040
	.global gLightSpriteOam
gLightSpriteOam:
	.space 0x000000c0
	.global gLightSpriteLargeOam
gLightSpriteLargeOam:
	.space 0x00000002
	.global gJupiterLighthouse_LightLevel
gJupiterLighthouse_LightLevel:
	.space 0x00000002
	.global gCrackedTiles
gCrackedTiles:
