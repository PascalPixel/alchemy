.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.space 0x00000004
	.global gJupiterLighthouse_GlowSprite
gJupiterLighthouse_GlowSprite:
	.space 0x00000028
	.global gJupiterLighthouse_GlowPosition
gJupiterLighthouse_GlowPosition:
	.space 0x0000000c
	.global gWhirlwindRiding
gWhirlwindRiding:
	.space 0x00000004
	.global gPaletteCycleColors
gPaletteCycleColors:
	.space 0x00000020
	.global gLighthousePalette13
gLighthousePalette13:
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
	.global gLightSpriteOams
gLightSpriteOams:
	.space 0x000000c0
	.global gLightSpritesLarge
gLightSpritesLarge:
	.space 0x00000002
	.global gLightSpritePriority
gLightSpritePriority:
	.space 0x00000002
	.global gCrackedTiles
gCrackedTiles:
