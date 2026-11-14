.syntax unified
	.thumb
	.section .bss,"aw",%nobits
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
	.global gAnemosInnerSanctumEntrance_LightLevel
gAnemosInnerSanctumEntrance_LightLevel:
	.space 0x00000002
	.global gAnemosInnerSanctumEntrance_LightPicture
gAnemosInnerSanctumEntrance_LightPicture:
