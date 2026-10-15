	.section .rodata
	.global BattleFx_RevealMaskA
BattleFx_RevealMaskA:
	.incbin "games/tla/src/graphics/battle_fx/reveal_mask_a.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/reveal_mask_a.bitmap.lz"
	.byte 0x52, 0x22	@ PACKER: left in the buffer by BattleFx_StarCrescentSheet+0x6c2
