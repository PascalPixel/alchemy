@ A 16-colour palette and one packed stream of 8,672 bytes. The game copies the
@ palette to OBJ palette 15 and the stream's first 8,192 bytes to video memory
@ as two groups of eight 32x32 sprites of 16 colours, 512 bytes a sprite. Side
@ by side each group is a strip 256 pixels wide, and the second continues the
@ first downwards: forest_canopy.png draws them as that one picture. The last
@ 480 bytes are zero.
	.section .rodata
	.global Summon_ForestCanopyTiles
Summon_ForestCanopyTiles:
	.incbin "games/tla/src/graphics/summon/forest_canopy.gbapal"
	.incbin "games/tla/src/graphics/summon/forest_canopy.parts.lz"
	.ifdef TLA_EDITION_EN
	.byte 0x5e, 0x5f	@ PACKER: left in the buffer by LuckyDice_Sprites+0x173e
	.else
	.ifdef TLA_EDITION_DE
	.byte 0xd9, 0x3c	@ PACKER: left in the buffer by LuckyDice_Sprites+0x173e
	.else
	.ifdef TLA_EDITION_ES
	.byte 0x78, 0x9c	@ PACKER: left in the buffer by LuckyDice_Sprites+0x173e
	.else
	.ifdef TLA_EDITION_FR
	.byte 0xf9, 0x84	@ PACKER: left in the buffer by LuckyDice_Sprites+0x173e
	.else
	.ifdef TLA_EDITION_IT
	.byte 0x43, 0x7c	@ PACKER: left in the buffer by LuckyDice_Sprites+0x173e
	.else
	.byte 0x39, 0x4a	@ PACKER: left in the buffer by LuckyDice_Sprites+0x173e
	.endif
	.endif
	.endif
	.endif
	.endif
