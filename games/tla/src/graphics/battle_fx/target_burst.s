	.section .rodata
	.global BattleEffect_TargetBurstImage
BattleEffect_TargetBurstImage:
	.incbin "games/tla/src/graphics/battle_fx/target_burst.bitmap.lz"
	.byte 0x2f, 0x00	@ PACKER: left in the buffer by BattleEffect_SlashSheet+0x83e
