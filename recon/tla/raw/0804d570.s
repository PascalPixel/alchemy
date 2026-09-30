.syntax unified
	.thumb
	.global Menu_AnimateSelectionToEntry
	.global Func_0804d570
	.thumb_func
Menu_AnimateSelectionToEntry:
Func_0804d570:
	push	{r5, r6, lr}
	adds	r6, r1, #0
	adds	r5, r0, #0
	bl	0x0803f800
	bl	AffineEffect_InitializeWork
	movs	r0, #1
	bl	Menu_AppendResourceEntry
	movs	r0, #15
	bl	Menu_AppendResourceEntry
	movs	r0, #2
	bl	Menu_AppendResourceEntry
	movs	r0, #7
	bl	Menu_AppendResourceEntry
	subs	r1, r6, #1
	adds	r0, r5, #0
	bl	0x0804d28c
	adds	r6, r0, #0
	bl	Menu_EndResourceSelection
	bl	0x0803f810
	adds	r0, r6, #0
	pop	{r5, r6, pc}
