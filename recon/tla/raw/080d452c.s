.syntax unified
	.thumb
	.global Func_080d452c
	.thumb_func
Func_080d452c:
	push	{r5, lr}
	adds	r5, r1, #0
	bl	ObjectTable_Get
	adds	r3, r0, #0
	cmp	r3, #0
	beq.n	.L_080d4546
	ldr	r0, [r3, #8]
	ldr	r1, [r3, #12]
	ldr	r2, [r3, #16]
	adds	r3, r5, #0
	bl	Motion_CamBounds
.L_080d4546:
	pop	{r5, pc}
	.global Func_080d4548
	.thumb_func
Func_080d4548:
	push	{lr}
	movs	r1, #213
	lsls	r1, r1, #4
	movs	r0, #108
	bl	Runtime_AllocateBlock
	movs	r3, #230
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldr	r0, [r0, #0]
	bl	Object_CommitPosition
	movs	r0, #2
	bl	Battle_WaitMode0
	pop	{pc}
	.global Func_080d4568
	.thumb_func
Func_080d4568:
	push	{lr}
	movs	r1, #213
	lsls	r1, r1, #4
	movs	r0, #108
	bl	Runtime_AllocateBlock
	movs	r3, #230
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldr	r0, [r0, #0]
	pop	{pc}
	.2byte 0x0000
