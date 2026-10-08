	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.balign 4
@ 冠詞と所有の語 (言語ごと)
	.ifdef TBS_EDITION_DE
	.global UiText_ArticleEmpty
UiText_ArticleEmpty:
	.byte 0
	.balign 4, 0
.LArticle0:
	.asciz "ein "
	.balign 4, 0
.LArticle1:
	.asciz "eine "
	.balign 4, 0
.LArticle2:
	.asciz "die "
	.balign 4, 0
.LArticle3:
	.asciz "das "
	.balign 4, 0
	.global UiText_ArticleTable
UiText_ArticleTable:
	.4byte UiText_ArticleEmpty
	.4byte .LArticle0
	.4byte .LArticle1
	.4byte .LArticle2
	.4byte .LArticle3
	.4byte UiText_ArticleEmpty
	.4byte UiText_ArticleEmpty
	.4byte UiText_ArticleEmpty
	.global UiText_NumberWord0
UiText_NumberWord0:
	.ascii "K"
	.byte 0xe4
	.ascii "pfe"
	.byte 0
	.balign 4, 0
	.global UiText_NumberWord1
UiText_NumberWord1:
	.asciz "Kampf"
	.balign 4, 0
	.global UiText_NumberWord2
UiText_NumberWord2:
	.asciz "Tickets"
	.global UiText_NumberWord3
UiText_NumberWord3:
	.asciz "Ticket"
	.balign 4, 0
	.global UiText_NumberWord4
UiText_NumberWord4:
	.asciz "Runden"
	.balign 4, 0
	.global UiText_NumberWord5
UiText_NumberWord5:
	.asciz "Runde"
	.balign 4, 0
	.global UiText_NumberWord6
UiText_NumberWord6:
	.ascii "M"
	.byte 0xfc
	.ascii "nzen"
	.byte 0
	.balign 4, 0
	.global UiText_NumberWord7
UiText_NumberWord7:
	.ascii "M"
	.byte 0xfc
	.ascii "nze"
	.byte 0
	.balign 4, 0
	.global UiText_NumberWord8
UiText_NumberWord8:
	.asciz "Punkte"
	.balign 4, 0
	.global UiText_NumberWord9
UiText_NumberWord9:
	.asciz "Punkt"
	.balign 4, 0
.LArticle4:
	.asciz "dein "
	.balign 4, 0
.LArticle5:
	.asciz "deine "
	.balign 4, 0
	.global UiText_ItemPossessives
UiText_ItemPossessives:
	.4byte .LArticle4
	.4byte .LArticle5
	.4byte .LArticle4
	.4byte UiText_ArticleEmpty
	.else
	.ifdef TBS_EDITION_ES
	.global UiText_ArticleEmpty
UiText_ArticleEmpty:
	.byte 0
	.balign 4, 0
.LArticle6:
	.asciz "el "
.LArticle7:
	.asciz "la "
.LArticle8:
	.asciz "los "
	.balign 4, 0
.LArticle9:
	.asciz "las "
	.balign 4, 0
	.global UiText_ArticleTable
UiText_ArticleTable:
	.4byte UiText_ArticleEmpty
	.4byte .LArticle6
	.4byte .LArticle7
	.4byte .LArticle8
	.4byte .LArticle9
.LArticle10:
	.asciz "un "
.LArticle11:
	.asciz "una "
	.balign 4, 0
.LArticle12:
	.asciz "unos "
	.balign 4, 0
.LArticle13:
	.asciz "unas "
	.balign 4, 0
	.global UiText_IndefiniteArticles
UiText_IndefiniteArticles:
	.4byte UiText_ArticleEmpty
	.4byte .LArticle10
	.4byte .LArticle11
	.4byte .LArticle12
	.4byte .LArticle13
.LArticle14:
	.asciz "tu "
.LArticle15:
	.asciz "tus "
	.balign 4, 0
	.global UiText_ItemPossessives
UiText_ItemPossessives:
	.4byte .LArticle14
	.4byte .LArticle15
	.else
	.ifdef TBS_EDITION_FR
	.global UiText_ArticleEmpty
UiText_ArticleEmpty:
	.byte 0
	.balign 4, 0
.LArticle16:
	.asciz "le "
.LArticle17:
	.asciz "la "
.LArticle18:
	.asciz "l' "
.LArticle19:
	.asciz "les "
	.balign 4, 0
.LArticle20:
	.asciz "un "
.LArticle21:
	.asciz "une "
	.balign 4, 0
.LArticle22:
	.asciz "des "
	.balign 4, 0
	.global UiText_ArticleTable
UiText_ArticleTable:
	.4byte UiText_ArticleEmpty
	.4byte .LArticle16
	.4byte .LArticle17
	.4byte .LArticle18
	.4byte .LArticle19
	.4byte .LArticle20
	.4byte .LArticle21
	.4byte .LArticle22
	.global UiText_ItemPronoun
UiText_ItemPronoun:
.LArticle23:
	.asciz "vous "
	.else
	.ifdef TBS_EDITION_IT
	.global UiText_ArticleEmpty
UiText_ArticleEmpty:
	.byte 0
	.balign 4, 0
.LArticle24:
	.asciz "il "
.LArticle25:
	.asciz "l' "
.LArticle26:
	.asciz "la "
.LArticle27:
	.asciz "lo "
.LArticle28:
	.asciz "le "
.LArticle29:
	.asciz "gli "
	.balign 4, 0
.LArticle30:
	.asciz "un "
.LArticle31:
	.asciz "un' "
	.balign 4, 0
.LArticle32:
	.asciz "uno "
	.balign 4, 0
.LArticle33:
	.asciz "una "
	.balign 4, 0
	.global UiText_ArticleTable
UiText_ArticleTable:
	.4byte UiText_ArticleEmpty
	.4byte .LArticle24
	.4byte .LArticle25
	.4byte .LArticle26
	.4byte .LArticle27
	.4byte .LArticle28
	.4byte .LArticle29
	.4byte .LArticle30
	.4byte .LArticle31
	.4byte .LArticle32
	.4byte .LArticle33
.LArticle34:
	.asciz "Il tuo "
.LArticle35:
	.asciz "la tua "
	.global UiText_ItemPossessives
UiText_ItemPossessives:
	.4byte .LArticle34
	.4byte .LArticle35
	.4byte .LArticle34
	.4byte UiText_ArticleEmpty
	.else
	.global UiText_ArticleEmpty
UiText_ArticleEmpty:
	.byte 0
	.balign 4, 0
.LArticle36:
	.asciz "a "
	.balign 4, 0
.LArticle37:
	.asciz "an "
.LArticle38:
	.asciz "some "
	.balign 4, 0
.LArticle39:
	.asciz "the "
	.balign 4, 0
	.global UiText_ArticleTable
UiText_ArticleTable:
	.4byte UiText_ArticleEmpty
	.4byte .LArticle36
	.4byte .LArticle37
	.4byte .LArticle38
	.4byte UiText_ArticleEmpty
	.4byte .LArticle39
	.4byte UiText_ArticleEmpty
	.4byte UiText_ArticleEmpty
	.endif
	.endif
	.endif
	.endif
