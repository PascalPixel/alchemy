	.section .rodata
	.global BattleFx_FireBurstSheet
BattleFx_FireBurstSheet:
	.incbin "GRAPHICS/BATTLE_FX/FIRE_BURST.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FIRE_BURST.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
