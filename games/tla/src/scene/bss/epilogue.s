.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.balign 16
	.global gEpilogueNamePanelEntry
gEpilogueNamePanelEntry:
	.space 0x00000110
	.global gEpilogueNamePanel
gEpilogueNamePanel:
	.space 0x00000028
	.global gEpilogueMessage
gEpilogueMessage:
	.space 0x00000004
	.global gEpilogueNameY
gEpilogueNameY:
	.space 0x00000004
	.global gEpilogueNamePanelTile
gEpilogueNamePanelTile:
	.space 0x00000004
	.global gEpilogueNameShown
gEpilogueNameShown:
	.space 0x00000004
	.global gEpilogueNameX
gEpilogueNameX:
