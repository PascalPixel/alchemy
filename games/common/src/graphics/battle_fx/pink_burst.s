	.section .rodata
	.global BattleFx_PinkBurstSheet
BattleFx_PinkBurstSheet:
	.incbin "games/common/src/graphics/battle_fx/pink_burst.gbapal"
	.incbin "games/common/src/graphics/battle_fx/pink_burst.bitmap.lz"
	.byte 0x57	@ PACKER: left in the buffer by BattleFx_IceBlockSheet+0x36b
