.syntax unified
	.thumb
	.global Func_080b5f0c
	.thumb_func
Func_080b5f0c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r5, #170
	lsls	r5, r5, #1
	adds	r0, r5, #0
	sub	sp, #16
	bl	Runtime_BumpAllocateAlternatePool
	ldr	r3, [pc, #316]
	ldr	r3, [r3, #0]
	adds	r6, r0, #0
	mov	r9, r3
	movs	r2, #255
	movs	r5, #7
	adds	r3, #79
.L_080b5f34:
	subs	r5, #1
	strb	r2, [r3, #0]
	subs	r3, #1
	cmp	r5, #0
	bge.n	.L_080b5f34
	mov	r7, sp
	adds	r0, r7, #0
	bl	BattleParty_PrepareActiveOwners
	movs	r5, #0
	mov	r8, r0
	cmp	r5, r8
	bge.n	.L_080b5fa8
	movs	r1, #149
	lsls	r1, r1, #1
	adds	r1, r1, r6
	mov	sl, r7
	mov	fp, r1
	movs	r7, #0
.L_080b5f5a:
	mov	r2, sl
	ldrh	r0, [r7, r2]
	bl	Owner_GetStateFar
	movs	r2, #170
	adds	r1, r0, #0
	lsls	r2, r2, #1
	ldr	r3, [pc, #252]
	adds	r0, r6, #0
	bl	_call_via_r3
	movs	r3, #2
	mov	r4, fp
	mov	r1, sl
	strb	r3, [r4, #0]
	ldrh	r3, [r7, r1]
	adds	r2, r5, #0
	adds	r3, #72
	subs	r2, #128
	mov	r4, r9
	movs	r1, #170
	lsls	r1, r1, #1
	strb	r2, [r4, r3]
	adds	r0, r6, #0
	bl	SerialRuntime_BeginTransferA
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	beq.n	.L_080b5fa8
	bl	SerialRuntime_WaitForTransferA
	adds	r5, #1
	movs	r0, #2
	bl	WaitFrames
	adds	r7, #2
	cmp	r5, r8
	blt.n	.L_080b5f5a
.L_080b5fa8:
	movs	r2, #149
	lsls	r2, r2, #1
	movs	r3, #0
	adds	r7, r6, r2
	mov	r8, r3
	b.n	.L_080b5fc0
.L_080b5fb4:
	bl	SerialRuntime_WaitForTransferA
	movs	r0, #2
	bl	WaitFrames
	adds	r5, #1
.L_080b5fc0:
	cmp	r5, #2
	bgt.n	.L_080b5fda
	mov	r4, r8
	movs	r1, #170
	lsls	r1, r1, #1
	strb	r4, [r7, #0]
	adds	r0, r6, #0
	bl	SerialRuntime_BeginTransferA
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	bne.n	.L_080b5fb4
.L_080b5fda:
	movs	r5, #160
	adds	r0, r6, #0
	lsls	r5, r5, #1
	bl	Party_Do
	adds	r0, r5, #0
	bl	Runtime_BumpAllocateAlternatePool
	adds	r6, r0, #0
	movs	r0, #0
	bl	Trade_GetOfferStateFar
	ldr	r3, [pc, #116]
	adds	r1, r0, #0
	adds	r2, r5, #0
	adds	r0, r6, #0
	bl	_call_via_r3
	adds	r4, r6, #0
	movs	r3, #132
	lsls	r3, r3, #1
	adds	r2, r6, r3
	ldr	r3, [r2, #0]
	movs	r1, #0
	adds	r4, #8
	cmp	r1, r3
	bge.n	.L_080b6028
	adds	r0, r2, #0
	adds	r2, r4, #0
.L_080b6014:
	ldrb	r3, [r2, #2]
	mov	r4, r9
	adds	r3, #72
	ldrb	r3, [r4, r3]
	strb	r3, [r2, #2]
	ldr	r3, [r0, #0]
	adds	r1, #1
	adds	r2, #4
	cmp	r1, r3
	blt.n	.L_080b6014
.L_080b6028:
	movs	r1, #160
	lsls	r1, r1, #1
	adds	r0, r6, #0
	bl	SerialRuntime_BeginTransferA
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	beq.n	.L_080b604a
	bl	SerialRuntime_WaitForTransferA
	movs	r0, #1
	bl	WaitFrames
	movs	r0, #2
	bl	WaitFrames
.L_080b604a:
	adds	r0, r6, #0
	bl	Party_Do
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e74
	.4byte 0x03001388
