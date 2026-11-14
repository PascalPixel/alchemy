@ The edition for the assembler: EDITION_INTERNATIONAL is 1 in every edition
@ but the Japanese, and building more than one edition at once is an error.
	.ifndef ALCHEMY_EDITION_INC
	.set ALCHEMY_EDITION_INC, 1
	.set EDITION_INTERNATIONAL, 0
	.ifdef TBS_EDITION_JA
	.ifdef EDITION_SELECTED
	.error "multiple game editions selected"
	.endif
	.set EDITION_SELECTED, 1
	.set EDITION_INTERNATIONAL, 0
	.endif
	.ifdef TBS_EDITION_EN
	.ifdef EDITION_SELECTED
	.error "multiple game editions selected"
	.endif
	.set EDITION_SELECTED, 1
	.set EDITION_INTERNATIONAL, 1
	.endif
	.ifdef TBS_EDITION_DE
	.ifdef EDITION_SELECTED
	.error "multiple game editions selected"
	.endif
	.set EDITION_SELECTED, 1
	.set EDITION_INTERNATIONAL, 1
	.endif
	.ifdef TBS_EDITION_ES
	.ifdef EDITION_SELECTED
	.error "multiple game editions selected"
	.endif
	.set EDITION_SELECTED, 1
	.set EDITION_INTERNATIONAL, 1
	.endif
	.ifdef TBS_EDITION_FR
	.ifdef EDITION_SELECTED
	.error "multiple game editions selected"
	.endif
	.set EDITION_SELECTED, 1
	.set EDITION_INTERNATIONAL, 1
	.endif
	.ifdef TBS_EDITION_IT
	.ifdef EDITION_SELECTED
	.error "multiple game editions selected"
	.endif
	.set EDITION_SELECTED, 1
	.set EDITION_INTERNATIONAL, 1
	.endif
	.endif
