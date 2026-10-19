	.section .rodata
	.global BattleFx_DustPuffSheet
BattleFx_DustPuffSheet:
	.incbin "games/tbs/src/graphics/battle_fx/dust_puff.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/dust_puff.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
