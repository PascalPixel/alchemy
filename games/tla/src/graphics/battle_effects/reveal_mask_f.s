	.section .rodata
	.global BattleEffect_RevealMaskF
BattleEffect_RevealMaskF:
	.incbin "games/tla/src/graphics/battle_effects/reveal_mask_f.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/reveal_mask_f.bitmap.lz"
	.byte 0x30, 0xe1, 0x08	@ PACKER: left in the buffer by BattleEffect_RevealMaskE+0x3e9
