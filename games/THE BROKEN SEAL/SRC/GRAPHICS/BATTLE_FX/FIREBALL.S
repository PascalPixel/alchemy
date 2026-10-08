	.section .rodata
	.global BattleFx_FireballSheet
BattleFx_FireballSheet:
	.incbin "GRAPHICS/BATTLE_FX/FIREBALL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FIREBALL.bitmap.lz"
	/* PACKER: left in the packer's buffer by BattleFx_FireBurstSheet at +0x5df */
	.byte 0x32
