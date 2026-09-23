.syntax unified
	.thumb
	.set sub_080f07f0, 0x080f07f0
	.global Unnamed_080f0614
	.global Func_080f0614
	.thumb_func
Unnamed_080f0614:
Func_080f0614:
	push	{r5, r6, lr}
	ldr	r5, [pc, #80]
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, #0
	bne.n	.L_080f0662
	ldr	r3, [pc, #72]
	movs	r6, #0
	ldrsh	r2, [r3, r6]
	ldrh	r4, [r3, #0]
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_080f0630
	adds	r3, r2, #7
.L_080f0630:
	ldr	r0, [pc, #60]
	asrs	r1, r3, #3
	movs	r6, #0
	ldrsh	r3, [r0, r6]
	cmp	r3, #0
	bge.n	.L_080f063e
	adds	r3, #7
.L_080f063e:
	asrs	r3, r3, #3
	cmp	r1, r3
	beq.n	.L_080f0662
	ldr	r2, [pc, #44]
	lsls	r3, r1, #2
	strh	r4, [r0, #0]
	ldr	r0, [r2, r3]
	adds	r3, r1, #0
	movs	r2, #31
	adds	r3, #16
	ands	r3, r2
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #3
	movs	r2, #1
	bl	sub_080f07f0
	strh	r0, [r5, #0]
.L_080f0662:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x02004c04
	.4byte 0x02004c00
	.4byte 0x02004c08
	.4byte 0x080f1220
