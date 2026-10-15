	.section .rodata
	.global Summon_BronzeAutomatonTiles
Summon_BronzeAutomatonTiles:
	.incbin "games/tla/src/graphics/summon/bronze_automaton.gbapal"
	.incbin "games/tla/src/graphics/summon/bronze_automaton.parts.lz"
	.ifdef TLA_EDITION_EN
	.byte 0xff	@ PACKER: left in the buffer by LuckyDice_Sprites+0x2ec7
	.else
	.ifdef TLA_EDITION_DE
	.byte 0x46	@ PACKER: left in the buffer by LuckyDice_Sprites+0x2ec7
	.else
	.ifdef TLA_EDITION_ES
	.byte 0x37	@ PACKER: left in the buffer by LuckyDice_Sprites+0x2ec7
	.else
	.ifdef TLA_EDITION_FR
	.byte 0x81	@ PACKER: left in the buffer by LuckyDice_Sprites+0x2ec7
	.else
	.ifdef TLA_EDITION_IT
	.byte 0x1d	@ PACKER: left in the buffer by LuckyDice_Sprites+0x2ec7
	.else
	.byte 0xbe	@ PACKER: left in the buffer by LuckyDice_Sprites+0x2ec7
	.endif
	.endif
	.endif
	.endif
	.endif
