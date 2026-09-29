@ BattleFx_LightningPillarSheet: a 48x224 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_LightningPillarSheet
BattleFx_LightningPillarSheet:
	.incbin "GRAPHICS/BATTLE_FX/LIGHTNING_PILLAR.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/LIGHTNING_PILLAR.bitmap.lz"
