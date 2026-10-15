	.section .rodata
	.global BattleFx_VioletMoteSheet
BattleFx_VioletMoteSheet:
	.incbin "games/tla/src/graphics/battle_fx/violet_mote.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/violet_mote.bitmap.lz"
	.byte 0x28, 0x41, 0x42	@ PACKER: left in the buffer by BattleFx_OverlayImageA+0x111
