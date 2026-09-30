.syntax unified
	.thumb
	.global Djinn_AddToLeastLoadedOwner
	.global Func_080b0ab8
	.thumb_func
Djinn_AddToLeastLoadedOwner:
Func_080b0ab8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	lsls	r3, r7, #2
	adds	r3, r3, r7
	mov	sl, r1
	lsls	r3, r3, #2
	add	r3, sl
	adds	r3, #48
	mov	fp, r3
	movs	r3, #186
	lsls	r3, r3, #2
	movs	r2, #0
	adds	r3, #255
	mov	r0, fp
	mov	r9, r2
	mov	r8, r3
	bl	GameFlag_TestFar
	cmp	r0, #0
	beq.n	.L_080b0af2
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080b0b66
.L_080b0af2:
	bl	Func_080afdbc
	cmp	r9, r0
	bge.n	.L_080b0b3a
	ldr	r3, [pc, #120]
	movs	r2, #134
	lsls	r2, r2, #2
	adds	r6, r3, r2
	adds	r5, r0, #0
.L_080b0b04:
	ldrb	r0, [r6, #0]
	bl	Owner_GetState
	movs	r2, #140
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldrb	r3, [r0, r3]
	cmp	r3, #9
	bhi.n	.L_080b0b32
	adds	r0, r0, r2
	movs	r1, #0
	movs	r2, #3
.L_080b0b1c:
	ldrb	r3, [r0, #0]
	subs	r2, #1
	adds	r0, #1
	adds	r1, r1, r3
	cmp	r2, #0
	bge.n	.L_080b0b1c
	cmp	r8, r1
	ble.n	.L_080b0b32
	ldrb	r3, [r6, #0]
	mov	r8, r1
	mov	r9, r3
.L_080b0b32:
	subs	r5, #1
	adds	r6, #1
	cmp	r5, #0
	bne.n	.L_080b0b04
.L_080b0b3a:
	movs	r2, #186
	lsls	r2, r2, #2
	adds	r2, #255
	cmp	r8, r2
	bne.n	.L_080b0b4a
	movs	r0, #2
	negs	r0, r0
	b.n	.L_080b0b66
.L_080b0b4a:
	adds	r1, r7, #0
	mov	r2, sl
	mov	r0, r9
	bl	Djinn_AddToOwner
	adds	r1, r7, #0
	mov	r2, sl
	mov	r0, r9
	bl	Trade_AddOffer
	mov	r0, fp
	bl	GameFlag_SetBitFar
	mov	r0, r9
.L_080b0b66:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02000240
