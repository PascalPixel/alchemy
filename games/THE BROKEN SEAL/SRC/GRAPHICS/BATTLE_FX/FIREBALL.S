	.section .rodata
	.global BattleFx_FireballSheet
BattleFx_FireballSheet:
	.incbin "GRAPHICS/BATTLE_FX/FIREBALL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FIREBALL.bitmap.lz"
	.byte 0x32	@ PACKER: left in the buffer by BattleFx_FireBurstSheet+0x5df
