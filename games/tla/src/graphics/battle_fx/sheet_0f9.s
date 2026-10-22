	.section .rodata
@ Uncolored art using BattleEffect_LightningBoltSheet colors (same effect).
	.global Resource_Data0F9
Resource_Data0F9:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0f9.bitmap.lz"
	.byte 0xa3, 0x08, 0x1a	@ PACKER: left in the buffer by Resource_Data0F8+0x3c1
