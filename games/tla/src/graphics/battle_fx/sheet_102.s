	.section .rodata
@ A 40x80 spear with no palette of its own: BattleEffect_RunOdyssey draws it at that
@ size, after copying Resource_Data101's palette into BG palette RAM
	.global Resource_Data102
Resource_Data102:
	.incbin "games/tla/src/graphics/battle_fx/sheet_102.bitmap.lz"
