.syntax unified
	.thumb
	.set sub_081c0788, 0x081c0788
	.set sub_081c08cc, 0x081c08cc
	.set sub_081c0a68, 0x081c0a68
	.set sub_081c0c0c, 0x081c0c0c
	.global Audio_NoopStubWorkCopy
	.global Func_081c048c
	.thumb_func
Audio_NoopStubWorkCopy:
Func_081c048c:
	bx	lr
	movs	r0, r0
	adds	r2, r0, #0
	ldr	r3, [r2, #52]
	ldr	r0, [pc, #12]
	cmp	r3, r0
	bne.n	.L_081c04a2
	ldr	r0, [r2, #4]
	ldr	r1, [pc, #8]
	ands	r0, r1
	str	r0, [r2, #4]
.L_081c04a2:
	bx	lr
	.4byte 0x68736d53
	.2byte 0xffff
	.2byte 0x7fff
	adds	r2, r0, #0
	lsls	r1, r1, #16
	lsrs	r1, r1, #16
	ldr	r3, [r2, #52]
	ldr	r0, [pc, #16]
	cmp	r3, r0
	bne.n	.L_081c04c4
	strh	r1, [r2, #38]
	strh	r1, [r2, #36]
	movs	r0, #128
	lsls	r0, r0, #1
	strh	r0, [r2, #40]
.L_081c04c4:
	bx	lr
	movs	r0, r0
	.2byte 0x6d53
	.2byte 0x6873
	push	{r4, r5, r6, lr}
	ldr	r0, [pc, #80]
	movs	r1, #2
	negs	r1, r1
	ands	r0, r1
	ldr	r1, [pc, #76]
	ldr	r2, [pc, #76]
.L_081c04da:
	bl	.L_081c04da
	ldr	r0, [pc, #76]
	bl	sub_081c08cc
	ldr	r0, [pc, #72]
	bl	sub_081c0788
	ldr	r0, [pc, #72]
	bl	sub_081c0a68
	ldr	r0, [pc, #68]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	cmp	r0, #0
	beq.n	.L_081c051a
	ldr	r5, [pc, #64]
	adds	r6, r0, #0
.L_081c04fe:
	ldr	r4, [r5, #0]
	ldr	r1, [r5, #4]
	ldrb	r2, [r5, #8]
	adds	r0, r4, #0
	bl	sub_081c0c0c
	ldrh	r0, [r5, #10]
	strb	r0, [r4, #11]
	ldr	r0, [pc, #48]
	str	r0, [r4, #24]
	adds	r5, #12
	subs	r6, #1
	cmp	r6, #0
	bne.n	.L_081c04fe
.L_081c051a:
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x04000100
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
