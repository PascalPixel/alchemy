	.section .rodata
	.global BattleFx_CrossTextureSheet
BattleFx_CrossTextureSheet:
	.incbin "games/tla/src/graphics/battle_fx/cross_texture.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/cross_texture.bitmap.lz"
	.byte 0x96, 0x1d	@ PACKER: left in the buffer by BattleFx_CrackedTextureSheet+0x412
