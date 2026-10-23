	.section .rodata
	.global BattleAnim_LightningPillarSheet
BattleAnim_LightningPillarSheet:
	.incbin "games/tbs/src/graphics/battle_anims/lightning_pillar.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/lightning_pillar.bitmap.lz"
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
