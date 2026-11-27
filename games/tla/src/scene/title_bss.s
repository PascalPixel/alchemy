@ The title's globals. They start on a halfword, right after its code, where a
@ C file holding its sprites' words would start them on a word.
.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.space 0x00000036
	.global TitleRevealFrame
TitleRevealFrame:
	.space 0x00000014
	.global TitleSprites
TitleSprites:
