.syntax unified
	.thumb
	.set sub_08018efc, 0x08018efc
	.global UiText_RenderWideStringInWindow
	.global Func_08017c8c
	.thumb_func
UiText_RenderWideStringInWindow:
Func_08017c8c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r4, r3, #0
	ldr	r3, [pc, #92]
	ldr	r3, [r3, #0]
	adds	r7, r2, #0
	mov	r8, r3
	lsls	r3, r7, #16
	asrs	r3, r3, #16
	adds	r6, r0, #0
	sub	sp, #8
	mov	r9, r1
	mov	sl, r3
	cmp	r6, #0
	bne.n	.L_08017cd0
	ldr	r1, [pc, #72]
	add	r1, r8
	ldrh	r3, [r1, #0]
	movs	r2, #235
	lsls	r2, r2, #4
	mov	r0, r8
	lsls	r3, r3, #1
	adds	r3, r3, r2
	adds	r6, r0, r2
	ldr	r2, [pc, #44]
	strh	r2, [r0, r3]
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #40]
	adds	r3, #1
	ands	r3, r2
	strh	r3, [r1, #0]
.L_08017cd0:
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	lsls	r1, r3, #16
	adds	r6, #2
	cmp	r1, #0
	beq.n	.L_08017db2
.L_08017cdc:
	lsrs	r5, r1, #16
	cmp	r5, #30
	bhi.n	.L_08017d80
	subs	r1, r5, #3
	cmp	r1, #26
	bhi.n	.L_08017da4
	ldr	r2, [pc, #20]
	lsls	r3, r1, #2
	ldr	r3, [r3, r2]
	b.n	.L_08017d04
	.4byte 0x00000000
	.4byte 0x000001ff
	.4byte 0x03001e8c
	.4byte 0x000012b2
	.2byte 0x7d08
	.2byte 0x0801
.L_08017d04:
	mov	pc, r3
	movs	r0, r0
	ldrb	r4, [r6, #21]
	lsrs	r1, r0, #32
	ldrb	r4, [r4, #22]
	lsrs	r1, r0, #32
	ldrb	r4, [r4, #22]
	lsrs	r1, r0, #32
	ldrb	r4, [r4, #22]
	lsrs	r1, r0, #32
	ldrb	r4, [r7, #21]
	lsrs	r1, r0, #32
	ldrb	r4, [r7, #21]
	lsrs	r1, r0, #32
	ldrb	r4, [r7, #21]
	lsrs	r1, r0, #32
	ldrb	r4, [r7, #21]
	lsrs	r1, r0, #32
	ldrb	r4, [r7, #21]
	lsrs	r1, r0, #32
	ldrb	r4, [r7, #21]
	lsrs	r1, r0, #32
	ldrb	r4, [r4, #22]
	lsrs	r1, r0, #32
	ldrb	r2, [r7, #21]
	lsrs	r1, r0, #32
	ldrb	r2, [r7, #21]
	lsrs	r1, r0, #32
	ldrb	r4, [r4, #22]
	lsrs	r1, r0, #32
	ldrb	r4, [r7, #21]
	lsrs	r1, r0, #32
	ldrb	r4, [r4, #22]
	lsrs	r1, r0, #32
	ldrb	r4, [r4, #22]
	lsrs	r1, r0, #32
	ldrb	r4, [r4, #22]
	lsrs	r1, r0, #32
	ldrb	r4, [r4, #22]
	lsrs	r1, r0, #32
	ldrb	r4, [r4, #22]
	lsrs	r1, r0, #32
	ldrb	r4, [r4, #22]
	lsrs	r1, r0, #32
	ldrb	r4, [r4, #22]
	lsrs	r1, r0, #32
	ldrb	r4, [r4, #22]
	lsrs	r1, r0, #32
	ldrb	r4, [r4, #22]
	lsrs	r1, r0, #32
	ldrb	r4, [r4, #22]
	lsrs	r1, r0, #32
	ldrb	r2, [r7, #21]
	lsrs	r1, r0, #32
	ldrb	r4, [r7, #21]
	lsrs	r1, r0, #32
	mov	r7, sl
	adds	r4, #1
	b.n	.L_08017da4
	adds	r6, #2
	adds	r6, #2
	b.n	.L_08017da4
.L_08017d80:
	movs	r3, #0
	str	r3, [sp, #0]
	adds	r2, r7, #0
	adds	r3, r4, #0
	mov	r0, r9
	adds	r1, r5, #0
	str	r4, [sp, #4]
	bl	sub_08018efc
	ldr	r0, [pc, #56]
	movs	r2, #128
	adds	r3, r5, r0
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	ldr	r4, [sp, #4]
	cmp	r3, r2
	bls.n	.L_08017da4
	adds	r7, #1
.L_08017da4:
	movs	r0, #0
	ldrsh	r3, [r6, r0]
	lsls	r3, r3, #16
	adds	r6, #2
	adds	r1, r3, #0
	cmp	r3, #0
	bne.n	.L_08017cdc
.L_08017db2:
	ldr	r2, [pc, #28]
	movs	r3, #1
	add	r2, r8
	strb	r3, [r2, #0]
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000ff22
	.4byte 0x00000ea3
