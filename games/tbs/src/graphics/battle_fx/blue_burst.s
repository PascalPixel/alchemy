	.section .rodata
	.global BattleFx_BlueBurstSheet
BattleFx_BlueBurstSheet:
	.incbin "graphics/battle_fx/blue_burst.gbapal"
	.incbin "graphics/battle_fx/blue_burst.bitmap.lz"
	.byte 0xc4, 0x99, 0xd3	@ PACKER: left in the buffer by BattleFx_LightFanSheet+0x331
