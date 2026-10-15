	.section .rodata
	.global Resource_Data0DC
Resource_Data0DC:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0dc_00.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/sheet_0dc.parts.lz"
	.byte 0x54	@ PACKER: left in the buffer by Summon_ChainedDemonTiles+0x2aaf
