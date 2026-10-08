	.section .rodata
	.global BattleFx_RevealMaskE
BattleFx_RevealMaskE:
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_E.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_E.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
