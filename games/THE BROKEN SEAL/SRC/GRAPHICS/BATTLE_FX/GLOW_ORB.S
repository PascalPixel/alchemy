@ BattleFx_GlowOrbSheet: a 40x240 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_GlowOrbSheet
BattleFx_GlowOrbSheet:
	.incbin "GRAPHICS/BATTLE_FX/GLOW_ORB.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/GLOW_ORB.bitmap.lz"
