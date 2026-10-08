	.section .rodata
	.global BattleFx_BlueBurstSheet
BattleFx_BlueBurstSheet:
	.incbin "GRAPHICS/BATTLE_FX/BLUE_BURST.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BLUE_BURST.bitmap.lz"
	.byte 0xc4, 0x99, 0xd3	@ PACKER: left in the buffer by BattleFx_LightFanSheet+0x331
