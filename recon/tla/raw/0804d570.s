.syntax unified
	.thumb
	.set sub_0803f800, 0x0803f800
	.set sub_0803f810, 0x0803f810
	.set sub_0804d0dc, 0x0804d0dc
	.set sub_0804d118, 0x0804d118
	.set sub_0804d28c, 0x0804d28c
	.set sub_0804d38c, 0x0804d38c
	.global Menu_AnimateSelectionToEntry
	.global Func_0804d570
	.thumb_func
Menu_AnimateSelectionToEntry:
Func_0804d570:
	push	{r5, r6, lr}
	adds	r6, r1, #0
	adds	r5, r0, #0
	bl	sub_0803f800
	bl	sub_0804d0dc
	movs	r0, #1
	bl	sub_0804d38c
	movs	r0, #15
	bl	sub_0804d38c
	movs	r0, #2
	bl	sub_0804d38c
	movs	r0, #7
	bl	sub_0804d38c
	subs	r1, r6, #1
	adds	r0, r5, #0
	bl	sub_0804d28c
	adds	r6, r0, #0
	bl	sub_0804d118
	bl	sub_0803f810
	adds	r0, r6, #0
	pop	{r5, r6, pc}
