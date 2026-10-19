	.section .rodata
	.global BattleFx_LightningPillarSheet
BattleFx_LightningPillarSheet:
	.incbin "games/tla/src/graphics/battle_fx/lightning_pillar.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/lightning_pillar.bitmap.lz"
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
