	.section .rodata
	.global BattleFx_RevealMaskF
BattleFx_RevealMaskF:
	.incbin "graphics/battle_fx/reveal_f.gbapal"
	.incbin "graphics/battle_fx/reveal_f.bitmap.lz"
	.byte 0x30, 0xe1, 0x08	@ PACKER: left in the buffer by BattleFx_RevealMaskE+0x3e9
