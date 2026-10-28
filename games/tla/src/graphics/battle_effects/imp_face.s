	.section .rodata
	.global BattleEffect_ImpFaceSheet
BattleEffect_ImpFaceSheet:
	.incbin "games/tla/src/graphics/battle_effects/imp_face.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/imp_face.bitmap.lz"
	.byte 0x07, 0xc7, 0x80	@ PACKER: left in the buffer by Resource_Data0D8+0x2a9
