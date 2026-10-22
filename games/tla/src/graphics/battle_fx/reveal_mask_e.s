	.section .rodata
	.global BattleEffect_RevealMaskE
BattleEffect_RevealMaskE:
	.incbin "games/tla/src/graphics/battle_fx/reveal_mask_e.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/reveal_mask_e.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
