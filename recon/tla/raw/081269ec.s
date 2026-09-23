.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_0811be3c, 0x0811be3c
	.global Func_081269ec
	.thumb_func
Func_081269ec:
	push	{lr}
	cmp	r0, #0
	bge.n	.L_081269f4
	adds	r0, #15
.L_081269f4:
	asrs	r0, r0, #4
	pop	{pc}
	push	{r5, lr}
	ldr	r5, [pc, #228]
	movs	r2, #8
	ldr	r3, [r5, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08126abe
.L_08126a06:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #36]
	ldr	r3, [r5, #12]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08126a24
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r3, #84
	adds	r2, r1, r3
	ldr	r3, [r2, #0]
	subs	r3, #1
	str	r3, [r2, #0]
.L_08126a24:
	ldr	r3, [r5, #12]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08126a3c
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r3, #84
	adds	r2, r1, r3
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
.L_08126a3c:
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08126a56
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r3, #84
	adds	r2, r1, r3
	ldr	r3, [r2, #0]
	subs	r3, #100
	str	r3, [r2, #0]
.L_08126a56:
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08126a70
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r3, #84
	adds	r2, r1, r3
	ldr	r3, [r2, #0]
	adds	r3, #100
	str	r3, [r2, #0]
.L_08126a70:
	ldr	r3, [r5, #12]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08126a88
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r3, #84
	adds	r2, r1, r3
	ldr	r3, [r2, #0]
	subs	r3, #10
	str	r3, [r2, #0]
.L_08126a88:
	ldr	r3, [r5, #12]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08126aa0
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r3, #84
	adds	r2, r1, r3
	ldr	r3, [r2, #0]
	adds	r3, #10
	str	r3, [r2, #0]
.L_08126aa0:
	ldr	r3, [r5, #4]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08126ab6
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r2, #84
	adds	r3, r1, r2
	ldr	r0, [r3, #0]
	b.n	.L_08126abe
.L_08126ab6:
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_08126a06
.L_08126abe:
	ldr	r3, [r5, #0]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08126acc
	movs	r0, #136
	lsls	r0, r0, #2
.L_08126acc:
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08126adc
	movs	r0, #144
	adds	r0, #255
.L_08126adc:
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	bl	sub_0811be3c
	cmp	r0, #0
	beq.n	.L_08126b60
	ldr	r0, [r0, #0]
	cmp	r0, #0
	beq.n	.L_08126b60
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	movs	r2, #15
	ands	r2, r3
	cmp	r2, #1
	beq.n	.L_08126b0a
	cmp	r2, #2
	beq.n	.L_08126b2e
	b.n	.L_08126b60
.L_08126b0a:
	ldr	r4, [r0, #80]
	movs	r2, #13
	ldrb	r1, [r4, #5]
	movs	r3, #3
	negs	r2, r2
	ands	r5, r3
	adds	r3, r2, #0
	lsls	r0, r5, #2
	ands	r3, r1
	orrs	r3, r0
	strb	r3, [r4, #5]
	adds	r1, r4, #0
	adds	r1, #33
	ldrb	r3, [r1, #0]
	ands	r2, r3
	orrs	r2, r0
	strb	r2, [r1, #0]
	b.n	.L_08126b60
.L_08126b2e:
	movs	r3, #3
	ands	r5, r3
	ldr	r6, [r0, #80]
	lsls	r0, r5, #2
	movs	r5, #13
	movs	r7, #0
	negs	r5, r5
.L_08126b3c:
	ldmia	r6!, {r4}
	cmp	r4, #0
	beq.n	.L_08126b60
	ldrb	r2, [r4, #5]
	adds	r3, r5, #0
	ands	r3, r2
	orrs	r3, r0
	strb	r3, [r4, #5]
	adds	r1, r4, #0
	adds	r1, #33
	ldrb	r2, [r1, #0]
	adds	r3, r5, #0
	ands	r3, r2
	orrs	r3, r0
	adds	r7, #1
	strb	r3, [r1, #0]
	cmp	r7, #3
	ble.n	.L_08126b3c
.L_08126b60:
	pop	{r5, r6, r7, pc}
	.align 2, 0
