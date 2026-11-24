@ When each edition was built: year, month, day, hour, minute and second,
@ two digits each. It follows the resource directory.
	.section .rodata
	.global Resource_BuildStamp
Resource_BuildStamp:
	.ifdef TBS_EDITION_JA
	.asciz "010628184502\n"
	.endif
	.ifdef TBS_EDITION_EN
	.asciz "010918151347\n"
	.endif
	.ifdef TBS_EDITION_FR
	.asciz "011219190202\n"
	.endif
	.ifdef TBS_EDITION_DE
	.asciz "020109152237\n"
	.endif
	.ifdef TBS_EDITION_ES
	.asciz "020205181231\n"
	.endif
	.ifdef TBS_EDITION_IT
	.asciz "020206164027\n"
	.endif
	.balign 4
