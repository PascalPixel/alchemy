	.section .rodata
	.global BattleFx_RuneSheet
BattleFx_RuneSheet:
	.incbin "games/tbs/src/graphics/battle_fx/rune.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/rune.bitmap.lz"
	.byte 0x43, 0x21, 0xbe	@ PACKER: left in the buffer by BattleFx_SwordSlashSheet+0x3d9
