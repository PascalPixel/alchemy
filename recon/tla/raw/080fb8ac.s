.syntax unified
	.thumb
	.global ItemMenu_DrawItemDetails
	.thumb_func
ItemMenu_DrawItemDetails:
	push {lr}
	movs r2, #1
	negs r2, r2
	bl Func_080fb8b8
	pop {pc}
