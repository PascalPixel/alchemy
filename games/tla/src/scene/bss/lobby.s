.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.global gLinkLobby_WaitFrames
gLinkLobby_WaitFrames:
	.space 0x00000004
	.global gLinkLobby_CallFrames
gLinkLobby_CallFrames:
