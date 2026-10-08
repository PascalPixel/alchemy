	.section .rodata
	.global BattleFx_SmokeSheet
BattleFx_SmokeSheet:
	.incbin "graphics/battle_fx/smoke.gbapal"
	.incbin "graphics/battle_fx/smoke.bitmap.lz"
	.byte 0xc1	@ PACKER: left in the buffer by BattleFx_DaggerSheet+0x88b
