	.section .rodata
	.global BattleFx_FireBurstSheet
BattleFx_FireBurstSheet:
	.incbin "graphics/battle_fx/fire_burst.gbapal"
	.incbin "graphics/battle_fx/fire_burst.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
