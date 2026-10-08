	.section .rodata
	.global BattleFx_SheepSheet
BattleFx_SheepSheet:
	.incbin "GRAPHICS/BATTLE_FX/SHEEP.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SHEEP.bitmap.lz"
	.byte 0x13, 0x4e, 0x03	@ PACKER: left in the buffer by BattleFx_SmokeSheet+0x865
