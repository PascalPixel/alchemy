.syntax unified
	.thumb
	.global Func_08021918
	.thumb_func
Func_08021918:
	push	{lr}
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #255
	adds	r2, r0, #0
	ands	r2, r3
	movs	r3, #179
	lsls	r3, r3, #2
	cmp	r2, r3
	bcc.n	.L_0802192e
	movs	r2, #0
.L_0802192e:
	lsls	r0, r2, #2
	ldr	r3, [pc, #8]
	adds	r0, r0, r2
	lsls	r0, r0, #2
	adds	r0, r0, r3
	pop	{pc}
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0830
	push	{r5, r6, r7, lr}
	ldrb	r3, [r0, #1]
	ldrb	r4, [r0, #0]
	lsls	r3, r3, #8
	orrs	r4, r3
	adds	r5, r1, #0
	adds	r6, r0, #2
	cmp	r4, #0
	bne.n	.L_08021956
	adds	r0, r6, #0
	b.n	.L_080219c8
.L_08021956:
	adds	r0, r0, r4
	ldrb	r1, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r7, r6, #0
	mov	ip, r5
	b.n	.L_0802199c
.L_08021964:
	asrs	r2, r4, #12
	cmp	r2, #0
	bne.n	.L_08021972
	ldrb	r3, [r0, #0]
	adds	r0, #1
	adds	r2, r3, #0
	adds	r2, #16
.L_08021972:
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #255
	ands	r4, r3
	adds	r2, #2
	subs	r4, r7, r4
	cmp	r2, #0
	beq.n	.L_08021990
.L_08021982:
	ldrb	r3, [r4, #0]
	subs	r2, #1
	strb	r3, [r5, #0]
	adds	r4, #1
	adds	r5, #1
	cmp	r2, #0
	bne.n	.L_08021982
.L_08021990:
	lsrs	r1, r1, #1
	cmp	r1, #0
	bne.n	.L_080219a0
	ldrb	r1, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #1
.L_0802199c:
	adds	r0, #1
	orrs	r1, r3
.L_080219a0:
	movs	r3, #1
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080219b6
	cmp	r1, #1
	beq.n	.L_08021990
	ldrb	r3, [r6, #0]
	adds	r6, #1
	strb	r3, [r5, #0]
	adds	r5, #1
	b.n	.L_08021990
.L_080219b6:
	ldrb	r3, [r0, #0]
	adds	r0, #1
	lsls	r4, r3, #8
	ldrb	r3, [r0, #0]
	adds	r0, #1
	orrs	r4, r3
	cmp	r4, #0
	bne.n	.L_08021964
	mov	r0, ip
.L_080219c8:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
