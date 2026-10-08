	.include "games/common/include/game/ed_asm.h"
	.section .rodata
@ 道具の名の性 (言語ごと)
	.ifdef TBS_EDITION_DE
	.global UiText_ItemGenders
UiText_ItemGenders:
	.incbin "graphics/text/genders_de.table"
	.endif
	.ifdef TBS_EDITION_IT
	.global UiText_ItemGenders
UiText_ItemGenders:
	.incbin "graphics/text/genders_it.table"
	.endif
