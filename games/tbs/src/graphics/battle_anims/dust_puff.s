	.section .rodata
	.global BattleAnim_DustPuffSheet
BattleAnim_DustPuffSheet:
	.incbin "games/tbs/src/graphics/battle_anims/dust_puff.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/dust_puff.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
