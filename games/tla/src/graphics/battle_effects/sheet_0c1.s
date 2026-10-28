	.section .rodata
@ Uncolored art using BattleEffect_OrangePaletteB colors (same effect).
	.global Resource_Data0C1
Resource_Data0C1:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0c1.bitmap.lz"
	.byte 0xfc, 0x52, 0x05	@ PACKER: left in the buffer by BattleEffect_CrossTextureSheet+0x335
