	.section .rodata
	.global BattleFx_SmokeSheet
BattleFx_SmokeSheet:
	.incbin "GRAPHICS/BATTLE_FX/SMOKE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SMOKE.bitmap.lz"
	.byte 0xc1	@ PACKER: left in the buffer by BattleFx_DaggerSheet+0x88b
