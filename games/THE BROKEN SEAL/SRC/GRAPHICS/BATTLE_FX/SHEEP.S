	.section .rodata
	.global BattleFx_SheepSheet
BattleFx_SheepSheet:
	.incbin "GRAPHICS/BATTLE_FX/SHEEP.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SHEEP.bitmap.lz"
	/* PACKER: left in the packer's buffer by BattleFx_SmokeSheet at +0x865 */
	.byte 0x13, 0x4e, 0x03
