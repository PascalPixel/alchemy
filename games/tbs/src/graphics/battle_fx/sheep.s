	.section .rodata
	.global BattleFx_SheepSheet
BattleFx_SheepSheet:
	.incbin "graphics/battle_fx/sheep.gbapal"
	.incbin "graphics/battle_fx/sheep.bitmap.lz"
	.byte 0x13, 0x4e, 0x03	@ PACKER: left in the buffer by BattleFx_SmokeSheet+0x865
