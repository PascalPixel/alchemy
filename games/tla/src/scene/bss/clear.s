.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.global gClear_PasswordText
gClear_PasswordText:
	.space 0x00000104
	.global gClear_PasswordCodes
gClear_PasswordCodes:
	.space 0x0000003c
	.global gClear_PasswordWork
gClear_PasswordWork:
