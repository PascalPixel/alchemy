	.section .rodata
@ Five cels of a flame with no palette of their own, at the sizes and offsets
@ BattleEffect_RunWhiplash draws them at; that effect loads BattleEffect_EffectPalette02 to
@ colour them
	.global Resource_Data0F3
Resource_Data0F3:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0f3.parts.lz"
	.byte 0xa8	@ PACKER: left in the buffer by Resource_Data0F0+0x1f4b
