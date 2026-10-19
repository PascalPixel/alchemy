	.section .rodata
	.global BattleFx_VineSheet
BattleFx_VineSheet:
	.incbin "games/tbs/src/graphics/battle_fx/vine_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/vine.parts.lz"
	.byte 0xbe	@ PACKER: left in the buffer by BattleFx_WhirlwindSheet+0xdcb
