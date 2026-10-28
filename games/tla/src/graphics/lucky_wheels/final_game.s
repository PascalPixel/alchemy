	.include "games/tla/include/ed_asm.h"
	.section .rodata
	.global Slots_FinalGameBanner
Slots_FinalGameBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/lucky_wheels/final_game_ja.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/final_game_ja.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/lucky_wheels/final_game_de.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/final_game_de.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/lucky_wheels/final_game_es.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/final_game_es.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/lucky_wheels/final_game_fr.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/final_game_fr.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/lucky_wheels/final_game_it.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/final_game_it.4bpp16x16.lz"
	.else
	.incbin "games/tla/src/graphics/lucky_wheels/final_game.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/final_game.4bpp16x16.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_JA
	.byte 0xf0, 0x71	@ PACKER: left in the buffer by Slots_PerfectBanner+0x272
	.else
	.ifdef TLA_EDITION_DE
	.byte 0x27, 0x70, 0x24	@ PACKER: left in the buffer by Slots_PerfectBanner+0x201
	.else
	.ifdef TLA_EDITION_ES
	.byte 0x1e	@ PACKER: left in the buffer by Slots_PerfectBanner+0x237
	.else
	.ifdef TLA_EDITION_IT
	.byte 0x74	@ PACKER: left in the buffer by Slots_PerfectBanner+0x23f
	.endif
	.endif
	.endif
	.endif
