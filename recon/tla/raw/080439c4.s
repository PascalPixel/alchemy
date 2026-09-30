.syntax unified
	.thumb
	.balign 4
	push	{lr}
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #8]
	bl	Func_080145a8
	pop	{pc}
	movs	r0, r0
	.2byte 0x3979
	.2byte 0x0804
	push	{lr}
	ldr	r0, [pc, #8]
	bl	Func_08014644
	pop	{pc}
	movs	r0, r0
	.2byte 0x3979
	.2byte 0x0804
	push	{r5, r6, r7, lr}
	adds	r5, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #60]
	movs	r7, #12
	ldrsh	r3, [r0, r7]
	ldr	r4, [sp, #16]
	adds	r3, r1, r3
	adds	r1, r3, #1
	movs	r7, #14
	ldrsh	r3, [r0, r7]
	adds	r3, r2, r3
	adds	r2, r3, #1
	cmp	r1, #0
	bge.n	.L_08043a0c
	adds	r5, r5, r1
	movs	r1, #0
.L_08043a0c:
	adds	r3, r1, r5
	cmp	r3, #29
	ble.n	.L_08043a16
	movs	r3, #30
	subs	r5, r3, r1
.L_08043a16:
	cmp	r2, #0
	bge.n	.L_08043a1e
	adds	r4, r4, r2
	movs	r2, #0
.L_08043a1e:
	adds	r3, r2, r4
	cmp	r3, #29
	ble.n	.L_08043a28
	movs	r3, #20
	subs	r4, r3, r2
.L_08043a28:
	cmp	r5, #0
	ble.n	.L_08043a60
	cmp	r4, #0
	ble.n	.L_08043a60
	lsls	r3, r1, #1
	lsls	r2, r2, #6
	adds	r3, r6, r3
	adds	r1, r2, r3
.L_08043a38:
	adds	r2, r1, #0
	adds	r3, r5, #0
	adds	r2, #8
	cmp	r3, #0
	beq.n	.L_08043a54
	ldr	r0, [pc, #12]
.L_08043a44:
	subs	r3, #1
	strh	r0, [r2, #0]
	adds	r2, #2
	cmp	r3, #0
	bne.n	.L_08043a44
	b.n	.L_08043a54
	.2byte 0xe006
	.2byte 0x0000
.L_08043a54:
	subs	r4, #1
	adds	r1, #64
	cmp	r4, #0
	bne.n	.L_08043a38
	movs	r3, #1
	strb	r3, [r6, #3]
.L_08043a60:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000