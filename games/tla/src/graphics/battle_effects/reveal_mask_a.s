	.section .rodata
	.global BattleEffect_RevealMaskA
BattleEffect_RevealMaskA:
	.incbin "games/tla/src/graphics/battle_effects/reveal_mask_a.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/reveal_mask_a.bitmap.lz"
	.byte 0x52, 0x22	@ PACKER: left in the buffer by BattleEffect_StarCrescentSheet+0x6c2
