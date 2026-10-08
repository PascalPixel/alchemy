	.section .rodata
	.global BattleFx_FireballSheet
BattleFx_FireballSheet:
	.incbin "graphics/battle_fx/fireball.gbapal"
	.incbin "graphics/battle_fx/fireball.bitmap.lz"
	.byte 0x32	@ PACKER: left in the buffer by BattleFx_FireBurstSheet+0x5df
