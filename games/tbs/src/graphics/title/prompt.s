	.include "games/common/include/game/ed_asm.h"
	.section .rodata
@ 「PRESS START」の文字 (32x8 のスプライトごと)
	.global Title_PromptTiles
Title_PromptTiles:
	.ifdef TBS_EDITION_DE
	.incbin "graphics/title/prompt_de.4bpp32x8.plz"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "graphics/title/prompt_es.4bpp32x8.plz"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "graphics/title/prompt_fr.4bpp32x8.plz"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "graphics/title/prompt_it.4bpp32x8.plz"
	.else
	.incbin "graphics/title/prompt.4bpp32x8.plz"
	.endif
	.endif
	.endif
	.endif
