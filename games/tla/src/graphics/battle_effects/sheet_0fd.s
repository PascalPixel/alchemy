	.section .rodata
@ Fifteen cels of a glowing oval shrinking from 16x32 to 3x4, with no palette of
@ their own: the sizes and offsets of Func_08182898's cel tables. That effect
@ loads BattleEffect_BlueBeamSheet's palette to colour them
	.global Resource_Data0FD
Resource_Data0FD:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0fd.parts.lz"
