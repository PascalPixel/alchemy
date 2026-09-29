@ BattleFx_CrossTextureSheet: a 32x32 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_CrossTextureSheet
BattleFx_CrossTextureSheet:
	.incbin "GRAPHICS/BATTLE_FX/CROSS_TEXTURE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/CROSS_TEXTURE.bitmap.lz"
