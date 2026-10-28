	.section .rodata
@ A 32x32 flare with no palette of its own: Func_0817e0c0 copies it row by
@ row, 32 bytes to a row, and colours it with a palette its argument picks,
@ BattleEffect_BlastSheet's for two of the six values
	.global Resource_Data181
Resource_Data181:
	.incbin "games/tla/src/graphics/battle_effects/sheet_181.bitmap.lz"
	.byte 0xf0	@ PACKER: left in the buffer by BattleEffect_LightFanSheet+0x203
