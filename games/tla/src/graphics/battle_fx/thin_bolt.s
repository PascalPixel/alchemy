	.section .rodata
	.global BattleEffect_ThinBoltSheet
BattleEffect_ThinBoltSheet:
	.incbin "games/tla/src/graphics/battle_fx/thin_bolt.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/thin_bolt.bitmap.lz"
	.byte 0x30, 0x19, 0x25	@ PACKER: left in the buffer by BattleEffect_WhirlBeamSheet+0x8b9
