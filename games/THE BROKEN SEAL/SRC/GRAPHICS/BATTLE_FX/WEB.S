	.section .rodata
	.global BattleFx_SpiderWebSheet
BattleFx_SpiderWebSheet:
	.incbin "GRAPHICS/BATTLE_FX/WEB.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/WEB.bitmap.lz"
	.byte 0x18, 0x85	@ PACKER: left in the buffer by BattleFx_FiveModeImage+0x25a
