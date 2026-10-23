	.section .rodata
	.global BattleAnim_RevealMaskB
BattleAnim_RevealMaskB:
	.incbin "games/tbs/src/graphics/battle_anims/reveal_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/reveal_b.bitmap.lz"
	.byte 0x00, 0x7b, 0x09	@ PACKER: left in the buffer by BattleAnim_RevealMaskA+0x2e5
