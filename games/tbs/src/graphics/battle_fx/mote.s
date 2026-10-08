	.section .rodata
	.global BattleFx_VioletMoteSheet
BattleFx_VioletMoteSheet:
	.incbin "graphics/battle_fx/mote.gbapal"
	.incbin "graphics/battle_fx/mote.bitmap.lz"
	.byte 0x28, 0x41, 0x42	@ PACKER: left in the buffer by BattleFx_OverlayImageA+0x111
