@ BattleFx_GlowOrbSetSheet: the 64-colour palette of its first picture, then its 7 pictures
@ (listed in GLOW_ORB_SET.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_GlowOrbSetSheet
BattleFx_GlowOrbSetSheet:
	.incbin "GRAPHICS/BATTLE_FX/GLOW_ORB_SET_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/GLOW_ORB_SET.parts.lz"
