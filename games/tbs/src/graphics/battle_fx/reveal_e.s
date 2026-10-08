	.section .rodata
	.global BattleFx_RevealMaskE
BattleFx_RevealMaskE:
	.incbin "graphics/battle_fx/reveal_e.gbapal"
	.incbin "graphics/battle_fx/reveal_e.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
