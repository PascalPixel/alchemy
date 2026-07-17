@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800f7f4
	.thumb_func
Func_0800f7f4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r4, #128
	movs	r3, #128
	adds	r6, r0, #0
	lsls	r3, r3, #7
	lsls	r4, r4, #8
	str	r3, [r6, #52]
	str	r4, [r6, #48]
	ldr	r3, [pc, #412]
	ldr	r3, [r3, #0]
	movs	r2, #15
	lsrs	r3, r3, #4
	ands	r3, r2
	ldr	r1, [pc, #408]
	lsls	r3, r3, #1
	ldrsh	r3, [r1, r3]
	movs	r0, #12
	lsls	r1, r3, #16
	ldr	r2, [pc, #400]
	mov	r9, r0
	movs	r3, #4
	lsrs	r0, r1, #16
	sub	sp, #80
	mov	sl, r3
	cmp	r0, r2
	bne.n	.L0
	b.n	.L1
.L0:
	movs	r2, #240
	lsls	r2, r2, #8
	movs	r3, #14
	ands	r2, r0
	mov	r9, r3
	cmp	r2, #0
	beq.n	.L2
	movs	r0, #15
	mov	r9, r0
	cmp	r2, r4
	beq.n	.L2
	movs	r2, #10
	mov	r9, r2
.L2:
	add	r5, sp, #68
	movs	r0, #128
	movs	r3, #0
	adds	r2, r5, #0
	lsls	r0, r0, #12
	lsrs	r1, r1, #16
	mov	sl, r3
	str	r3, [r5, #0]
	str	r3, [r5, #4]
	str	r3, [r5, #8]
	bl	Func_0800447c
	ldr	r3, [r5, #0]
	ldr	r2, [r6, #8]
	adds	r3, r3, r2
	ldr	r2, [r5, #8]
	str	r3, [r5, #0]
	cmp	r2, #0
	bge.n	.L3
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r6, #6]
.L3:
	cmp	r2, #0
	ble.n	.L4
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
.L4:
	ldr	r2, [r5, #8]
	ldr	r3, [r6, #12]
	subs	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r2, [r6, #16]
	ldr	r3, [r6, #8]
	str	r2, [r5, #8]
	cmp	r3, #0
	bge.n	.L5
	ldr	r0, [pc, #288]
	adds	r3, r3, r0
.L5:
	asrs	r0, r3, #20
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L6
	ldr	r1, [pc, #276]
	adds	r3, r2, r1
.L6:
	asrs	r3, r3, #20
	lsls	r1, r3, #7
	adds	r3, r0, r1
	ldr	r0, [pc, #272]
	lsls	r3, r3, #2
	adds	r0, r0, r3
	ldr	r3, [r5, #0]
	mov	r8, r0
	cmp	r3, #0
	bge.n	.L7
	ldr	r0, [pc, #252]
	adds	r3, r3, r0
.L7:
	asrs	r3, r3, #20
	adds	r3, r3, r1
	ldr	r1, [pc, #248]
	lsls	r3, r3, #2
	adds	r0, r5, #0
	adds	r7, r3, r1
	bl	Func_0801219c
	cmp	r0, #0
	bne.n	.L8
	mov	r3, r8
	ldrb	r2, [r3, #2]
	ldrb	r3, [r7, #2]
	cmp	r2, r3
	beq.n	.L9
.L8:
	movs	r0, #4
	movs	r1, #12
	mov	sl, r0
	mov	r9, r1
	b.n	.L1
.L9:
	ldr	r1, [pc, #196]
	ldr	r3, [r1, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L10
	adds	r3, r6, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r2, [r5, #8]
	ldr	r3, [pc, #196]
	ldr	r1, [r5, #0]
	adds	r2, r2, r3
	bl	Func_08011f54
	ldr	r3, [r6, #12]
	subs	r3, r0, r3
	movs	r0, #128
	lsls	r0, r0, #13
	cmp	r3, r0
	bge.n	.L1
	b.n	.L11
.L10:
	ldr	r3, [r1, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	adds	r3, r6, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #8]
	bl	Func_08011f54
	ldr	r3, [r6, #12]
	ldr	r1, [pc, #152]
	subs	r3, r0, r3
	cmp	r3, r1
	ble.n	.L1
.L11:
	movs	r2, #1
	movs	r3, #12
	mov	sl, r2
	mov	r9, r3
.L1:
	ldr	r3, [pc, #140]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L12
	movs	r2, #3
	mov	r0, sl
	ands	r2, r0
	cmp	r2, #0
	beq.n	.L13
	movs	r1, #206
	lsls	r1, r1, #1
	adds	r2, r3, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L12
.L13:
	movs	r0, #206
	lsls	r0, r0, #1
	adds	r3, r3, r0
	strh	r2, [r3, #0]
.L12:
	mov	r1, r9
	adds	r0, r6, #0
	bl	Func_0800c300
	mov	r1, sl
	cmp	r1, #0
	beq.n	.L14
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r6, #56]
	str	r3, [r6, #60]
	str	r3, [r6, #64]
	movs	r3, #0
	str	r3, [r6, #36]
	str	r3, [r6, #40]
	str	r3, [r6, #44]
	b.n	.L15
.L14:
	add	r3, sp, #68
	ldr	r1, [r3, #0]
	ldr	r2, [r3, #4]
	adds	r0, r6, #0
	ldr	r3, [r3, #8]
	bl	Func_0800d14c
.L15:
	ldrh	r3, [r6, #4]
	adds	r3, #1
	movs	r0, #1
	strh	r3, [r6, #4]
	add	sp, #80
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001ae8
	.4byte 0x08013254
	.4byte 0x0000ffff
	.4byte 0x000fffff
	.4byte 0x02010000
	.4byte 0xfff00000
	.4byte 0xfff80000
	.4byte 0x03001ebc
