	.section .rodata
	.global BattleEffect_RuneSheet
BattleEffect_RuneSheet:
	.incbin "games/tla/src/graphics/battle_fx/rune.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/rune.bitmap.lz"
	.byte 0xf8, 0x44, 0x14	@ PACKER: left in the buffer by BattleEffect_CrossedDaggersSheet+0x3d9
