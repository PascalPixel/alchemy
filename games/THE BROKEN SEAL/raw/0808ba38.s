.syntax unified
	.thumb
	.set sub_08009080, 0x08009080
	.set sub_08009140, 0x08009140
	.set sub_080091e0, 0x080091e0
	.set sub_0808ba1c, 0x0808ba1c
	.global Overlay_0808ba38
Overlay_0808ba38:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r7, [pc, #212]
	movs	r2, #224
	lsls	r2, r2, #4
	adds	r2, r2, r7
	mov	fp, r2
	movs	r3, #226
	movs	r2, #228
	lsls	r3, r3, #4
	lsls	r2, r2, #4
	adds	r3, r3, r7
	adds	r2, r2, r7
	sub	sp, #4
	mov	r9, r3
	mov	sl, r2
	movs	r3, #0
	movs	r2, #66
	str	r2, [sp, #0]
	mov	r8, r3
	ldr	r3, [pc, #180]
	movs	r2, #207
	ldr	r3, [r3, #0]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	adds	r6, r7, #0
	subs	r6, #32
	cmp	r3, #3
	bne.n	.L_0808ba84
	movs	r3, #8
	str	r3, [sp, #0]
.L_0808ba84:
	ldr	r2, [sp, #0]
	movs	r5, #0
	cmp	r5, r2
	bge.n	.L_0808baf2
.L_0808ba8c:
	adds	r0, r5, #0
	bl	sub_0808ba1c
	adds	r4, r0, #0
	cmp	r4, #0
	beq.n	.L_0808baea
	strb	r5, [r6, #0]
	ldr	r3, [pc, #136]
	adds	r6, #1
	adds	r1, r7, #0
	ldr	r2, [pc, #132]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r3, r4, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_0808bac4
	ldr	r2, [r4, #80]
	adds	r3, r2, #0
	adds	r3, #36
	ldrb	r4, [r3, #0]
	adds	r3, #2
	ldrb	r1, [r3, #0]
	ldrb	r3, [r2, #9]
	lsls	r3, r3, #28
	lsrs	r0, r3, #30
	b.n	.L_0808baca
.L_0808bac4:
	movs	r4, #0
	movs	r1, #0
	movs	r0, #0
.L_0808baca:
	mov	r3, fp
	strb	r4, [r3, #0]
	mov	r3, r9
	movs	r2, #1
	strb	r1, [r3, #0]
	movs	r3, #1
	add	fp, r2
	add	r9, r2
	add	r8, r3
	mov	r2, sl
	strb	r0, [r2, #0]
	mov	r2, r8
	add	sl, r3
	adds	r7, #112
	cmp	r2, #31
	bhi.n	.L_0808baf2
.L_0808baea:
	ldr	r3, [sp, #0]
	adds	r5, #1
	cmp	r5, r3
	blt.n	.L_0808ba8c
.L_0808baf2:
	mov	r5, r8
	cmp	r5, #31
	bgt.n	.L_0808bb08
	movs	r3, #32
	movs	r2, #255
	subs	r5, r3, r5
.L_0808bafe:
	subs	r5, #1
	strb	r2, [r6, #0]
	adds	r6, #1
	cmp	r5, #0
	bne.n	.L_0808bafe
.L_0808bb08:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02001124
	.4byte 0x03001ebc
	.4byte 0x040000d4
	.2byte 0x001c
	.2byte 0x8400
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r1, [pc, #240]
	movs	r2, #32
	mov	r8, r1
	negs	r2, r2
	add	r2, r8
	movs	r3, #224
	movs	r1, #226
	mov	sl, r2
	lsls	r3, r3, #4
	lsls	r1, r1, #4
	movs	r2, #228
	sub	sp, #8
	add	r3, r8
	add	r1, r8
	lsls	r2, r2, #4
	str	r3, [sp, #4]
	str	r1, [sp, #0]
	add	r2, r8
	mov	r1, sl
	mov	fp, r2
	movs	r2, #31
	negs	r2, r2
	ldrb	r7, [r1, #0]
	movs	r3, #0
	add	r2, r8
	mov	r9, r3
	mov	sl, r2
	cmp	r7, #255
	beq.n	.L_0808bc18
.L_0808bb74:
	adds	r0, r7, #0
	bl	sub_0808ba1c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0808bbf2
	ldr	r6, [r5, #80]
	ldr	r3, [pc, #172]
	mov	r0, r8
	adds	r1, r5, #0
	ldr	r2, [pc, #168]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [sp, #4]
	ldrb	r1, [r3, #0]
	cmp	r1, #0
	beq.n	.L_0808bb9c
	adds	r0, r5, #0
	bl	sub_08009080
.L_0808bb9c:
	ldr	r2, [sp, #0]
	adds	r0, r5, #0
	ldrb	r1, [r2, #0]
	bl	sub_080091e0
	mov	r3, fp
	ldrb	r1, [r3, #0]
	movs	r3, #3
	ldrb	r2, [r6, #9]
	ands	r1, r3
	movs	r3, #13
	negs	r3, r3
	lsls	r1, r1, #2
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r6, #9]
	ldrb	r2, [r6, #21]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r6, #21]
	ldr	r1, [pc, #108]
	ldr	r3, [r1, #0]
	str	r6, [r5, #80]
	cmp	r7, r3
	bne.n	.L_0808bbf2
	ldr	r2, [pc, #104]
	movs	r1, #240
	ldr	r3, [r2, #0]
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldr	r1, [pc, #96]
	ldr	r2, [r3, #0]
	ldr	r3, [r1, #0]
	ldr	r1, [r3, #0]
	ldr	r3, [r5, #12]
	adds	r0, r5, #0
	str	r3, [r2, #20]
	str	r3, [r2, #12]
	str	r3, [r1, #4]
	bl	sub_08009140
.L_0808bbf2:
	movs	r2, #112
	ldr	r3, [sp, #4]
	ldr	r1, [sp, #0]
	add	r8, r2
	movs	r2, #1
	adds	r3, #1
	add	r9, r2
	str	r3, [sp, #4]
	adds	r1, #1
	mov	r3, r9
	str	r1, [sp, #0]
	add	fp, r2
	cmp	r3, #31
	bgt.n	.L_0808bc18
	mov	r1, sl
	ldrb	r7, [r1, #0]
	add	sl, r2
	cmp	r7, #255
	bne.n	.L_0808bb74
.L_0808bc18:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02001124
	.4byte 0x040000d4
	.4byte 0x8400001c
	.4byte 0x02000434
	.4byte 0x03001ebc
	.4byte 0x03001e70
