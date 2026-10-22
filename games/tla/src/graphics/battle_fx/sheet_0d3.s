	.section .rodata
@ Uncolored art using BattleEffect_BlueFlameSheet colors (same effect).
	.global Resource_Data0D3
Resource_Data0D3:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0d3.bitmap.lz"
	.byte 0x84, 0x2a, 0x0c	@ PACKER: left in the buffer by BattleEffect_FeatherSheet+0x41d
