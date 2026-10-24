	.ifndef ALCHEMY_ED_ASM_H
	.set ALCHEMY_ED_ASM_H, 1
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
