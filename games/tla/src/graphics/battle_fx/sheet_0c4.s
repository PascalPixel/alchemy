	.section .rodata
@ Uncolored art using BattleFx_EffectPalette02 colors (same effect).
	.global Resource_Data0C4
Resource_Data0C4:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0c4.bitmap.lz"
	.byte 0xaa, 0x32	@ PACKER: left in the buffer by Resource_Data0C3+0x152
