	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global LuckyDice_Board
LuckyDice_Board:
	.incbin "games/common/src/graphics/lucky_dice/board.gbapal"
	.incbin "games/common/src/graphics/lucky_dice/board.8bpp.lz"
	.ifeq GAME_TLA
	.byte 0x81, 0x61, 0xa6	@ PACKER: left in the buffer by Battle_AerialViewBackdrop+0x2e1d
	.else
	.ifdef TLA_EDITION_JA
	.byte 0x4a, 0x1d, 0xa3	@ PACKER: left in the buffer by Ending_TheEndPicture+0x2e1d
	.else
	.ifdef TLA_EDITION_DE
	.byte 0x1d, 0x43, 0x46	@ PACKER: left in the buffer by Ending_TheEndPicture+0x2e1d
	.else
	.ifdef TLA_EDITION_ES
	.byte 0xe9, 0x3d, 0xef	@ PACKER: left in the buffer by Ending_TheEndPicture+0x2e1d
	.else
	.ifdef TLA_EDITION_FR
	.byte 0xe9, 0x3d, 0xef	@ PACKER: left in the buffer by Ending_TheEndPicture+0x2e1d
	.else
	.byte 0x00, 0x00, 0x00	@ PACKER: left in the buffer by Ending_TheEndPicture+0x2e1d
	.endif
	.endif
	.endif
	.endif
	.endif
