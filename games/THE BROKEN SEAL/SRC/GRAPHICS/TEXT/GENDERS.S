	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
@ 道具の名の性 (言語ごと)
	.ifdef TBS_EDITION_DE
	.global UiText_ItemGenders
UiText_ItemGenders:
	.incbin "GRAPHICS/TEXT/GENDERS_DE.table"
	.endif
	.ifdef TBS_EDITION_IT
	.global UiText_ItemGenders
UiText_ItemGenders:
	.incbin "GRAPHICS/TEXT/GENDERS_IT.table"
	.endif
