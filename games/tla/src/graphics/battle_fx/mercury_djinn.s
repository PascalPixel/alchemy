	.section .rodata
	.global BattleFx_MercuryDjinnSheet
BattleFx_MercuryDjinnSheet:
	.incbin "games/tla/src/graphics/battle_fx/mercury_djinn.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/mercury_djinn.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
