.syntax unified
	.thumb
	.set sub_08014878, 0x08014878
	.set sub_08020280, 0x08020280
	.set sub_080c8588, 0x080c8588
	.global Func_0810b168
	.thumb_func
Func_0810b168:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #12
	adds	r5, r7, r2
	movs	r6, #23
.L_0810b17c:
	adds	r0, r5, #0
	subs	r6, #1
	bl	sub_080c8588
	adds	r5, #72
	cmp	r6, #0
	bge.n	.L_0810b17c
	movs	r2, #161
	lsls	r2, r2, #3
	adds	r3, r7, r2
	movs	r5, #0
	ldrsb	r5, [r3, r5]
	movs	r3, #1
	negs	r3, r3
	cmp	r5, r3
	beq.n	.L_0810b1b0
	bl	sub_08014878
	lsls	r3, r5, #2
	lsls	r1, r0, #3
	subs	r1, r1, r0
	adds	r3, #248
	lsrs	r1, r1, #16
	ldr	r0, [r7, r3]
	bl	sub_08020280
.L_0810b1b0:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
