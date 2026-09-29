@ BattleFx_CrossedDaggersSheet: a 32x128 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_CrossedDaggersSheet
BattleFx_CrossedDaggersSheet:
	.incbin "GRAPHICS/BATTLE_FX/CROSSED_DAGGERS.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/CROSSED_DAGGERS.bitmap.lz"
