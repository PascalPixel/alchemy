@ When each edition was built: year, month, day, hour, minute and second,
@ two digits each. It follows the resource directory.
	.section .rodata
	.global Resource_BuildStamp
Resource_BuildStamp:
	.ifdef TLA_EDITION_JA
	.asciz "020610172024\n"
	.endif
	.ifdef TLA_EDITION_EN
	.asciz "030205144838\n"
	.endif
	.ifdef TLA_EDITION_DE
	.asciz "030623214832\n"
	.endif
	.ifdef TLA_EDITION_FR
	.asciz "030624204924\n"
	.endif
	.ifdef TLA_EDITION_ES
	.asciz "030707212715\n"
	.endif
	.ifdef TLA_EDITION_IT
	.asciz "030708203047\n"
	.endif
	.balign 4
