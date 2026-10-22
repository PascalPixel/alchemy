	.section .rodata
	.global BattleEffect_SparkleDots
BattleEffect_SparkleDots:
	.incbin "games/tla/src/graphics/battle_fx/sparkle_dots.parts.lz"
	.byte 0x63, 0x34, 0x77	@ PACKER: left in the buffer by BattleEffect_IceBlockSheet+0x61
