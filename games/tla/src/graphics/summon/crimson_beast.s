	.section .rodata
	.global Summon_CrimsonBeastTiles
Summon_CrimsonBeastTiles:
	.incbin "games/tla/src/graphics/summon/crimson_beast.gbapal"
	.incbin "games/tla/src/graphics/summon/crimson_beast.parts.lz"
	.ifdef TLA_EDITION_EN
	.byte 0x8f	@ PACKER: left in the buffer by LuckyDice_Sprites+0x1643
	.else
	.ifdef TLA_EDITION_DE
	.byte 0x00	@ PACKER: left in the buffer by LuckyDice_Sprites+0x1643
	.else
	.ifdef TLA_EDITION_ES
	.byte 0x8e	@ PACKER: left in the buffer by LuckyDice_Sprites+0x1643
	.else
	.ifdef TLA_EDITION_FR
	.byte 0x1f	@ PACKER: left in the buffer by LuckyDice_Sprites+0x1643
	.else
	.ifdef TLA_EDITION_IT
	.byte 0x61	@ PACKER: left in the buffer by LuckyDice_Sprites+0x1643
	.else
	.byte 0x31	@ PACKER: left in the buffer by LuckyDice_Sprites+0x1643
	.endif
	.endif
	.endif
	.endif
	.endif
