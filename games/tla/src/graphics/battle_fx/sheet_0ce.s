	.section .rodata
@ Uncolored art using BattleEffect_BlueFlameSheet colors (same effect).
	.global Resource_Data0CE
Resource_Data0CE:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0ce.bitmap.lz"
	.byte 0xf7, 0x1c	@ PACKER: left in the buffer by BattleEffect_CloudSwirlSheet+0x526
