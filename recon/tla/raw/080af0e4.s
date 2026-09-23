.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_080ad3f8, 0x080ad3f8
	.global Func_080af0e4
	.thumb_func
Func_080af0e4:
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	adds	r7, r0, #0
	bl	sub_08016ca4
	ldr	r3, [pc, #36]
	adds	r6, r0, #0
	lsls	r5, r5, #1
	adds	r5, #216
	adds	r6, #216
	strh	r3, [r0, r5]
	adds	r4, r6, #0
	movs	r5, #0
	adds	r1, r6, #0
	movs	r0, #14
.L_080af102:
	ldrh	r2, [r4, #0]
	adds	r4, #2
	lsls	r3, r2, #16
	cmp	r3, #0
	beq.n	.L_080af118
	strh	r2, [r1, #0]
	adds	r5, #1
	adds	r1, #2
	b.n	.L_080af118
	.2byte 0x0000
	.2byte 0x0000
.L_080af118:
	subs	r0, #1
	cmp	r0, #0
	bge.n	.L_080af102
	cmp	r5, #14
	bgt.n	.L_080af13c
	lsls	r3, r5, #1
	ldr	r2, [pc, #16]
	adds	r0, r3, r6
	movs	r3, #15
	subs	r5, r3, r5
.L_080af12c:
	subs	r5, #1
	strh	r2, [r0, #0]
	adds	r0, #2
	cmp	r5, #0
	bne.n	.L_080af12c
	b.n	.L_080af13c
	.2byte 0x0000
	.2byte 0x0000
.L_080af13c:
	adds	r0, r7, #0
	bl	sub_080ad3f8
	movs	r0, #2
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
