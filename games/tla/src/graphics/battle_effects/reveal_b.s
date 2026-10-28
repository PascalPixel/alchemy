	.section .rodata
	.global BattleEffect_RevealMaskB
BattleEffect_RevealMaskB:
	.incbin "games/tla/src/graphics/battle_effects/reveal_b.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/reveal_b.bitmap.lz"
	.byte 0x00, 0x7b, 0x09	@ PACKER: left in the buffer by BattleEffect_RevealMaskA+0x2e5
