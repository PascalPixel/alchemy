	.section .rodata
@ Five cels of a flame with no palette of their own, at the sizes and offsets
@ Func_0817c27c draws them at; that effect loads BattleFx_EffectPalette02 to
@ colour them
	.global Resource_Data0F3
Resource_Data0F3:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0f3.parts.lz"
