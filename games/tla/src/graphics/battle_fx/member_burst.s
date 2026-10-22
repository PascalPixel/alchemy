	.section .rodata
	.global BattleEffect_MemberBurstImage
BattleEffect_MemberBurstImage:
	.incbin "games/tla/src/graphics/battle_fx/member_burst.bitmap.lz"
	.byte 0x19, 0x14, 0x56	@ PACKER: left in the buffer by BattleEffect_FireballSheet+0x49d
