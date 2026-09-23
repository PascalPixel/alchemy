.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_0802dac0, 0x0802dac0
	.global Func_0802dc48
	.thumb_func
Func_0802dc48:
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r6, r1, #0
	bl	sub_0802dac0
	ldr	r3, [r5, #0]
	adds	r7, r0, #0
	movs	r4, #0
	cmp	r3, #0
	bge.n	.L_0802dc60
	ldr	r2, [pc, #80]
	adds	r3, r3, r2
.L_0802dc60:
	ldr	r0, [r5, #8]
	asrs	r2, r3, #21
	movs	r1, #31
	ands	r2, r1
	cmp	r0, #0
	bge.n	.L_0802dc70
	ldr	r3, [pc, #64]
	adds	r0, r0, r3
.L_0802dc70:
	asrs	r3, r0, #21
	ands	r3, r1
	lsls	r3, r3, #5
	adds	r3, r2, r3
	ldr	r2, [pc, #56]
	lsls	r3, r3, #2
	adds	r0, r3, r2
	ldrb	r2, [r0, #3]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0802dc8a
	movs	r4, #16
.L_0802dc8a:
	ldr	r3, [r0, #0]
	lsls	r3, r3, #2
	lsrs	r0, r3, #26
	str	r0, [r6, #0]
	cmp	r0, #40
	bls.n	.L_0802dc98
	movs	r4, #32
.L_0802dc98:
	cmp	r0, #36
	bne.n	.L_0802dc9e
	movs	r4, #64
.L_0802dc9e:
	cmp	r0, #12
	beq.n	.L_0802dca6
	cmp	r0, #15
	bne.n	.L_0802dca8
.L_0802dca6:
	movs	r4, #48
.L_0802dca8:
	ldr	r3, [pc, #12]
	adds	r2, r4, r7
	ldrb	r0, [r3, r2]
	pop	{r5, r6, r7, pc}
	.4byte 0x001fffff
	.4byte 0x02020000
	.2byte 0xf004
	.2byte 0x0802
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	cmp	r0, #0
	blt.n	.L_0802dcca
	str	r0, [r3, #4]
.L_0802dcca:
	cmp	r1, #0
	blt.n	.L_0802dcd0
	str	r1, [r3, #8]
.L_0802dcd0:
	cmp	r2, #0
	blt.n	.L_0802dcd6
	str	r2, [r3, #12]
.L_0802dcd6:
	pop	{pc}
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #32]
	movs	r6, #0
	b.n	.L_0802dcf4
.L_0802dce4:
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #150
	adds	r6, #1
	lsls	r3, r3, #1
	cmp	r6, r3
	bge.n	.L_0802dd00
.L_0802dcf4:
	ldr	r3, [r5, #4]
	cmp	r3, #255
	bgt.n	.L_0802dce4
	ldr	r3, [r5, #8]
	cmp	r3, #255
	bgt.n	.L_0802dce4
.L_0802dd00:
	movs	r3, #0
	str	r3, [r5, #12]
	pop	{r5, r6, pc}
	.align 2, 0
