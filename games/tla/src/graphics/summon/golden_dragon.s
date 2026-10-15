	.section .rodata
	.global Summon_GoldenDragonTiles
Summon_GoldenDragonTiles:
	.incbin "games/tla/src/graphics/summon/golden_dragon.gbapal"
	.incbin "games/tla/src/graphics/summon/golden_dragon.parts.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x66	@ PACKER: left in the buffer by Ending_TheEndPicture+0x483b
	.else
	.ifdef TLA_EDITION_DE
	.byte 0xc4	@ PACKER: left in the buffer by Ending_TheEndPicture+0x483b
	.else
	.ifdef TLA_EDITION_IT
	.byte 0xe4	@ PACKER: left in the buffer by Ending_TheEndPicture+0x483b
	.else
	.byte 0x00	@ PACKER: left in the buffer by Ending_TheEndPicture+0x483b
	.endif
	.endif
	.endif
