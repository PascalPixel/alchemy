	.section .rodata
@ Each item name's gender, by item; German and Italian only
	.ifdef TLA_EDITION_DE
	.global gLocalizedWord_ItemGenders
gLocalizedWord_ItemGenders:
	.incbin "games/tla/src/graphics/text/genders_de.table"
	.endif
	.ifdef TLA_EDITION_IT
	.global gLocalizedWord_ItemGenders
gLocalizedWord_ItemGenders:
	.incbin "games/tla/src/graphics/text/genders_it.table"
	.endif
