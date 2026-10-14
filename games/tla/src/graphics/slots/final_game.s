	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Slots_FinalGameBanner
Slots_FinalGameBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/slots/final_game_ja.gbapal"
	.incbin "games/tla/src/graphics/slots/final_game_ja.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/slots/final_game_de.gbapal"
	.incbin "games/tla/src/graphics/slots/final_game_de.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/slots/final_game_es.gbapal"
	.incbin "games/tla/src/graphics/slots/final_game_es.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/slots/final_game_fr.gbapal"
	.incbin "games/tla/src/graphics/slots/final_game_fr.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/slots/final_game_it.gbapal"
	.incbin "games/tla/src/graphics/slots/final_game_it.4bpp16x16.lz"
	.else
	.incbin "games/tla/src/graphics/slots/final_game.gbapal"
	.incbin "games/tla/src/graphics/slots/final_game.4bpp16x16.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
