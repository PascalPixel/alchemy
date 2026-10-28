	.section .rodata
@ Uncolored art using BattleEffect_BlueFlameSheet colors (same effect).
	.global Resource_Data0CF
Resource_Data0CF:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0cf.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
