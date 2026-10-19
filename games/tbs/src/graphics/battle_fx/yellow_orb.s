	.section .rodata
	.global BattleFx_YellowOrbSheet
BattleFx_YellowOrbSheet:
	.incbin "games/tbs/src/graphics/battle_fx/yellow_orb.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/yellow_orb.bitmap.lz"
	.byte 0x01, 0x9d	@ PACKER: left in the buffer by BattleFx_LavaOrbSheet+0xa12
