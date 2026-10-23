	.section .rodata
@ Layouts, indexed from ResourceId_Layout001
	.global gLayouts
gLayouts:
	.incbin "games/tbs/src/field/layouts.table"

	.global gLayoutsTail
gLayoutsTail:
	.4byte 0, 3, 7, 9
