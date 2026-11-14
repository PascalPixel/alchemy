	.include "games/tbs/include/edition.inc"
	.section .rodata
	.global LuckyDice_Board
LuckyDice_Board:
	.incbin "games/tbs/src/graphics/lucky_dice/board.gbapal"
	.incbin "games/tbs/src/graphics/lucky_dice/board.8bpp.lz"
	.byte 0x81, 0x61, 0xa6	@ PACKER: left in the buffer by Battle_AerialViewBackdrop+0x2e1d
	.global LuckyDice_Sprites
LuckyDice_Sprites:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tbs/src/graphics/lucky_dice/sprites_1_ja.gbapal"
	.incbin "games/tbs/src/graphics/lucky_dice/sprites_ja.parts.lz"
	.else
	.ifdef TBS_EDITION_DE
	.incbin "games/tbs/src/graphics/lucky_dice/sprites_1_de.gbapal"
	.incbin "games/tbs/src/graphics/lucky_dice/sprites_de.parts.lz"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "games/tbs/src/graphics/lucky_dice/sprites_1_es.gbapal"
	.incbin "games/tbs/src/graphics/lucky_dice/sprites_es.parts.lz"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "games/tbs/src/graphics/lucky_dice/sprites_1_fr.gbapal"
	.incbin "games/tbs/src/graphics/lucky_dice/sprites_fr.parts.lz"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "games/tbs/src/graphics/lucky_dice/sprites_1_it.gbapal"
	.incbin "games/tbs/src/graphics/lucky_dice/sprites_it.parts.lz"
	.else
	.incbin "games/tbs/src/graphics/lucky_dice/sprites_1.gbapal"
	.incbin "games/tbs/src/graphics/lucky_dice/sprites.parts.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x0f, 0x72, 0xb1	@ PACKER: left in the buffer by Battle_AerialViewBackdrop+0x3021
	.else
	.ifdef TBS_EDITION_ES
	.byte 0x5a, 0x9b	@ PACKER: left in the buffer by Battle_AerialViewBackdrop+0x2f7a
	.else
	.ifdef TBS_EDITION_FR
	.byte 0xac	@ PACKER: left in the buffer by Battle_AerialViewBackdrop+0x31a7
	.endif
	.endif
	.endif
