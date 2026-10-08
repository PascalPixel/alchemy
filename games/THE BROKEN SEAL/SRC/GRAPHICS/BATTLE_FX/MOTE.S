	.section .rodata
	.global BattleFx_VioletMoteSheet
BattleFx_VioletMoteSheet:
	.incbin "GRAPHICS/BATTLE_FX/MOTE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/MOTE.bitmap.lz"
	.byte 0x28, 0x41, 0x42	@ PACKER: left in the buffer by BattleFx_OverlayImageA+0x111
