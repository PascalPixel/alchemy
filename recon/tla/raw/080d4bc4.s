.syntax unified
	.thumb
	.set sub_080148e8, 0x080148e8
	.set sub_080202f0, 0x080202f0
	.global Object_FindNearestFacingTarget
	.global Func_080d4bc4
	.thumb_func
Object_FindNearestFacingTarget:
Func_080d4bc4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #40
	mov	r9, r3
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r2, #0
	ldr	r5, [r3, #20]
	mov	sl, r2
	movs	r2, #63
	adds	r7, r0, #0
	mov	fp, r1
	mov	r8, r2
.L_080d4be8:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_080d4c8c
	cmp	r5, r7
	beq.n	.L_080d4c8c
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_080d4c8c
	ldr	r1, [r5, #12]
	ldr	r3, [r7, #12]
	subs	r2, r1, r3
	cmp	r2, #0
	blt.n	.L_080d4c0e
	ldr	r3, [pc, #184]
	cmp	r2, r3
	ble.n	.L_080d4c16
	b.n	.L_080d4c8c
.L_080d4c0e:
	ldr	r2, [pc, #176]
	subs	r3, r3, r1
	cmp	r3, r2
	bgt.n	.L_080d4c8c
.L_080d4c16:
	ldr	r2, [r5, #8]
	ldr	r3, [r7, #8]
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L_080d4c28
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r0, r0, r3
.L_080d4c28:
	ldr	r2, [r5, #16]
	ldr	r3, [r7, #16]
	asrs	r0, r0, #16
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_080d4c3c
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r2, r2, r3
.L_080d4c3c:
	asrs	r3, r2, #16
	adds	r2, r0, #0
	muls	r2, r0
	adds	r0, r2, #0
	adds	r2, r3, #0
	muls	r2, r3
	adds	r3, r2, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #116]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c06
	cmp	r6, r9
	bge.n	.L_080d4c8c
	ldr	r3, [r7, #16]
	ldr	r0, [r5, #16]
	ldr	r1, [r5, #8]
	subs	r0, r0, r3
	ldr	r3, [r7, #8]
	subs	r1, r1, r3
	bl	sub_080148e8
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	cmp	r6, #23
	ble.n	.L_080d4c88
	ldrh	r3, [r7, #6]
	subs	r3, r0, r3
	lsls	r3, r3, #16
	asrs	r0, r3, #16
	ldr	r3, [pc, #76]
	cmp	r0, r3
	blt.n	.L_080d4c8c
	movs	r2, #188
	lsls	r2, r2, #6
	adds	r2, #255
	cmp	r0, r2
	bgt.n	.L_080d4c8c
.L_080d4c88:
	mov	sl, r5
	mov	r9, r6
.L_080d4c8c:
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r2, r8
	adds	r5, #128
	cmp	r2, #0
	bge.n	.L_080d4be8
	mov	r3, sl
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_080d4cb2
	mov	r2, sl
	ldr	r3, [r2, #80]
	ldr	r3, [r3, #40]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, fp
	bne.n	.L_080d4cb2
	mov	r0, sl
.L_080d4cb2:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x002fffff
	.4byte 0x030002d4
	.2byte 0xd001
	.2byte 0xffff
	.2byte 0xb500
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	adds	r4, r1, #0
	ldr	r3, [r3, #108]
	cmp	r2, #0
	beq.n	.L_080d4cfe
	movs	r2, #197
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	beq.n	.L_080d4cfe
	adds	r3, r0, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r1, [r4, #0]
	ldr	r2, [r4, #8]
	bl	sub_080202f0
	cmp	r0, #2
	bhi.n	.L_080d4d02
.L_080d4cfe:
	movs	r0, #0
	b.n	.L_080d4d04
.L_080d4d02:
	movs	r0, #1
.L_080d4d04:
	pop	{pc}
	.2byte 0x0000
