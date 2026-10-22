	.section .rodata
@ Uncolored art using BattleEffect_EffectPalette02 colors (same effect).
	.global Resource_Data0EF
Resource_Data0EF:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0ef.bitmap.lz"
	.byte 0x41, 0x90	@ PACKER: left in the buffer by Resource_Data0EE+0x52a
