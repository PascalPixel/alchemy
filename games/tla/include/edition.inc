	.ifndef GUARD_ED_ASM_H
	.set GUARD_ED_ASM_H, 1
	.set EDITION_INTERNATIONAL, 0
	.ifdef TLA_EDITION_JA
	.ifdef EDITION_SELECTED
	.error "multiple game editions selected"
	.endif
	.set EDITION_SELECTED, 1
	.set EDITION_INTERNATIONAL, 0
	.endif
	.ifdef TLA_EDITION_EN
	.ifdef EDITION_SELECTED
	.error "multiple game editions selected"
	.endif
	.set EDITION_SELECTED, 1
	.set EDITION_INTERNATIONAL, 1
	.endif
	.ifdef TLA_EDITION_DE
	.ifdef EDITION_SELECTED
	.error "multiple game editions selected"
	.endif
	.set EDITION_SELECTED, 1
	.set EDITION_INTERNATIONAL, 1
	.endif
	.ifdef TLA_EDITION_ES
	.ifdef EDITION_SELECTED
	.error "multiple game editions selected"
	.endif
	.set EDITION_SELECTED, 1
	.set EDITION_INTERNATIONAL, 1
	.endif
	.ifdef TLA_EDITION_FR
	.ifdef EDITION_SELECTED
	.error "multiple game editions selected"
	.endif
	.set EDITION_SELECTED, 1
	.set EDITION_INTERNATIONAL, 1
	.endif
	.ifdef TLA_EDITION_IT
	.ifdef EDITION_SELECTED
	.error "multiple game editions selected"
	.endif
	.set EDITION_SELECTED, 1
	.set EDITION_INTERNATIONAL, 1
	.endif
	.endif
