	.section .rodata
	.global BattleFx_SpiderWebSheet
BattleFx_SpiderWebSheet:
	.incbin "graphics/battle_fx/web.gbapal"
	.incbin "graphics/battle_fx/web.bitmap.lz"
	.byte 0x18, 0x85	@ PACKER: left in the buffer by BattleFx_FiveModeImage+0x25a
