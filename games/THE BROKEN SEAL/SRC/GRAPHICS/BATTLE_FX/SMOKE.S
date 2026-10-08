	.section .rodata
	.global BattleFx_SmokeSheet
BattleFx_SmokeSheet:
	.incbin "GRAPHICS/BATTLE_FX/SMOKE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SMOKE.bitmap.lz"
	/* PACKER: left in the packer's buffer by BattleFx_DaggerSheet at +0x88b */
	.byte 0xc1
