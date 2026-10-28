	.section .rodata
	.global BattleEffect_CrossTextureSheet
BattleEffect_CrossTextureSheet:
	.incbin "games/tla/src/graphics/battle_effects/cross_texture.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/cross_texture.bitmap.lz"
	.byte 0x96, 0x1d	@ PACKER: left in the buffer by BattleEffect_CrackedTextureSheet+0x412
