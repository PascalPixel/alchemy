	.include "games/common/include/game/ed_asm.h"
	.section .rodata
@ Grammatical gender of item names, by language
	.ifdef TBS_EDITION_DE
	.global gUiText_ItemGenders
gUiText_ItemGenders:
	.incbin "games/tbs/src/graphics/text/genders_de.table"
	.endif
	.ifdef TBS_EDITION_IT
	.global gUiText_ItemGenders
gUiText_ItemGenders:
	.incbin "games/tbs/src/graphics/text/genders_it.table"
	.endif
