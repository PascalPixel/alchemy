	.section .rodata
@ Uncolored art using BattleFx_OrangePaletteB colors (same effect).
	.global Resource_Data0C1
Resource_Data0C1:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0c1.bitmap.lz"
	.byte 0xfc, 0x52, 0x05	@ PACKER: left in the buffer by BattleFx_CrossTextureSheet+0x335
