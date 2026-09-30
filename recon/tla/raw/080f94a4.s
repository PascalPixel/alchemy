.syntax unified
	.thumb
	.global Func_080f94a4
	.thumb_func
Func_080f94a4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #236
	movs	r0, #220
	sub	sp, #16
	bl	Runtime_AllocateHeapBlock
	movs	r1, #128
	lsls	r1, r1, #6
	mov	sl, r1
	adds	r5, r0, #0
	mov	r0, sl
	bl	Runtime_BumpAllocateAlternatePool
	movs	r7, #192
	ldr	r3, [pc, #60]
	lsls	r7, r7, #18
	ldr	r2, [r7, #24]
	mov	fp, r3
	movs	r3, #1
	movs	r1, #0
	strh	r3, [r2, #4]
	adds	r6, r0, #0
	movs	r2, #30
	movs	r3, #20
	movs	r0, #0
	bl	Func_080383e8
	movs	r0, #1
	bl	WaitFrames
	movs	r0, #0
	bl	0x080f80e0
	movs	r1, #129
	lsls	r1, r1, #2
	adds	r0, r5, r1
	bl	0x080ad100
	movs	r2, #139
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r5, r2
	movs	r1, #3
	movs	r2, #0
	b.n	.L_080f9514
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_080f9514:
	strb	r0, [r3, #0]
	movs	r3, #7
	movs	r0, #0
	bl	0x080fa368
	bl	0x080fc480
	movs	r0, #14
	bl	0x080f9108
	ldr	r0, [pc, #196]
	bl	0x080383f8
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #17
	movs	r3, #3
	movs	r0, #13
	bl	UiWindow_CreateFar
	adds	r3, r5, #0
	adds	r3, #240
	str	r0, [r3, #0]
	bl	0x080f80c4
	ldr	r3, [pc, #168]
	ldr	r1, [pc, #172]
	mov	r9, r3
	mov	r2, sl
	adds	r0, r6, #0
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x4b29
	mov	r1, sl
	ldr	r2, [pc, #164]
	ldr	r0, [pc, #152]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2001
	bl	0x080383c0
	bl	0x080f9448
	add	r1, sp, #8
	add	r0, sp, #12
	add	r2, sp, #4
	bl	Func_080f9644
	mov	r8, r0
	bl	0x080f9464
	mov	r1, r8
	cmp	r1, #1
	bne.n	.L_080f95a4
	ldr	r1, [sp, #12]
	ldr	r3, [sp, #4]
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r2, #255
	ands	r3, r2
	ldr	r0, [r7, #108]
	lsls	r1, r1, #10
	orrs	r1, r3
	movs	r3, #180
	lsls	r3, r3, #1
	strh	r1, [r0, r3]
	ldrh	r3, [r5, r3]
	movs	r1, #193
	lsls	r1, r1, #1
	adds	r2, r0, r1
	strh	r3, [r2, #0]
.L_080f95a4:
	ldr	r0, [r5, #40]
	bl	0x08038268
	ldr	r2, [r7, #60]
	ldr	r3, [pc, #60]
	strb	r3, [r2, #6]
	bl	0x080fa478
	movs	r1, #0
	movs	r2, #30
	movs	r3, #20
	movs	r0, #0
	bl	Func_080383e8
	bl	0x08104aa4
	movs	r0, #220
	bl	Runtime_ReleaseHeapBlock
	ldr	r2, [r7, #24]
	movs	r3, #0
	strh	r3, [r2, #4]
	bl	0x08038290
	movs	r0, #0
	bl	0x080383c0
	adds	r1, r6, #0
	mov	r2, sl
	ldr	r0, [pc, #24]
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x6bfb
	mov	r2, fp
	b.n	.L_080f9604
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x06002500
	.4byte 0x03000730
	.4byte 0x06004000
	.4byte 0x03000260
	.2byte 0x3333
	.2byte 0x3333
.L_080f9604:
	strb	r2, [r3, #6]
	adds	r0, r6, #0
	bl	Func_08013164
	movs	r0, #1
	bl	WaitFrames
	bl	Scheduler_DisableOverlayCallbacksWithFlags
	movs	r0, #1
	bl	WaitFrames
	movs	r0, #0
	movs	r2, #30
	movs	r1, #0
	movs	r3, #20
	bl	0x080383f0
	ldr	r3, [r7, #60]
	mov	r1, fp
	strb	r1, [r3, #6]
	bl	Event_ClearInvalidPackedValuesFar
	mov	r0, r8
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
