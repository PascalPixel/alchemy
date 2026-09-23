.syntax unified
	.thumb
	.set sub_080b7f70, 0x080b7f70
	.global Unnamed_080ba918
	.global Func_080ba918
	.thumb_func
Unnamed_080ba918:
Func_080ba918:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #255
	mov	r8, r0
	adds	r7, r1, #0
	movs	r6, #0
	mov	sl, r3
	b.n	.L_080ba95e
.L_080ba92c:
	ldr	r2, [r0, #40]
	ldrb	r3, [r2, #22]
	mov	r4, sl
	orrs	r3, r4
	strb	r3, [r2, #22]
	adds	r3, r0, #0
	adds	r3, #39
	adds	r1, r0, #0
	ldrb	r0, [r3, #0]
	adds	r1, #44
	strb	r7, [r2, #5]
	cmp	r0, #1
	ble.n	.L_080ba95c
	movs	r5, #0
	movs	r4, #255
	subs	r0, #1
.L_080ba94c:
	ldmia	r1!, {r2}
	ldrb	r3, [r2, #22]
	subs	r0, #1
	orrs	r3, r4
	strb	r5, [r2, #5]
	strb	r3, [r2, #22]
	cmp	r0, #0
	bne.n	.L_080ba94c
.L_080ba95c:
	adds	r6, #1
.L_080ba95e:
	mov	r0, r8
	adds	r1, r6, #0
	bl	sub_080b7f70
	cmp	r0, #0
	bne.n	.L_080ba92c
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.2byte 0x0000
