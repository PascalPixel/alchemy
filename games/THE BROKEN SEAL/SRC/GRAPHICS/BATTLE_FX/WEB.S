	.section .rodata
	.global BattleFx_SpiderWebSheet
BattleFx_SpiderWebSheet:
	.incbin "GRAPHICS/BATTLE_FX/WEB.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/WEB.bitmap.lz"
	/* PACKER: left in the packer's buffer by BattleFx_FiveModeImage at +0x25a */
	.byte 0x18, 0x85
