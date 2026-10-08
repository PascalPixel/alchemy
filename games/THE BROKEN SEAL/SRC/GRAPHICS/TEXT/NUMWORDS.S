	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.ifdef TBS_EDITION_DE
	.balign 4
@ 数の語 {単数, 複数}
	.global UiText_NumberWords
UiText_NumberWords:
	.4byte UiText_NumberWord9, UiText_NumberWord8
	.4byte UiText_NumberWord7, UiText_NumberWord6
	.4byte UiText_NumberWord5, UiText_NumberWord4
	.4byte UiText_NumberWord3, UiText_NumberWord2
	.4byte UiText_NumberWord1, UiText_NumberWord0
	.4byte UiText_ArticleEmpty, UiText_ArticleEmpty
	.4byte UiText_ArticleEmpty, UiText_ArticleEmpty
	.4byte UiText_ArticleEmpty, 0
	.endif
