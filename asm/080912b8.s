@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080912b8
	.thumb_func
Func_080912b8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #208]
	ldr	r1, [r3, #0]
	subs	r3, #112
	ldr	r3, [r3, #0]
	adds	r3, #228
	mov	fp, r1
	movs	r1, #2
	ldrsh	r2, [r3, r1]
	sub	sp, #16
	str	r2, [sp, #8]
	movs	r1, #6
	ldrsh	r2, [r3, r1]
	str	r2, [sp, #4]
	mov	r5, fp
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L0
	b.n	.L1
.L0:
	ldr	r2, [r3, #16]
	mov	sl, r2
	movs	r2, #22
	ldrsh	r1, [r3, r2]
	ldr	r7, [r3, #8]
	str	r1, [sp, #0]
	adds	r3, #34
	ldrb	r3, [r3, #0]
	movs	r1, #189
	mov	r8, r3
	ldr	r3, [pc, #156]
	lsls	r1, r1, #1
	ldr	r2, [pc, #156]
	adds	r3, r3, r1
	adds	r7, r7, r2
	ldrh	r3, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #13
	lsrs	r3, r3, #5
	add	r2, sl
	adds	r1, r7, #0
	mov	r0, r8
	mov	r9, r3
	bl	Func_080091a8
	movs	r2, #128
	lsls	r2, r2, #14
	asrs	r6, r0, #16
	add	r2, sl
	mov	r0, r8
	adds	r1, r7, #0
	bl	Func_080091a8
	asrs	r0, r0, #16
	subs	r0, #16
	cmp	r0, r6
	ble.n	.L2
	adds	r6, r0, #0
.L2:
	cmp	r6, #0
	ble.n	.L3
	ldr	r3, [sp, #0]
	cmp	r6, r3
	ble.n	.L3
	ldr	r3, [pc, #96]
	movs	r1, #13
	str	r3, [r5, #4]
	movs	r3, #128
	ldrb	r2, [r5, #9]
	lsls	r3, r3, #3
	negs	r1, r1
	str	r3, [r5, #8]
	adds	r3, r1, #0
	ands	r3, r2
	strb	r3, [r5, #9]
	ldr	r3, [pc, #52]
	mov	r2, r9
	ands	r2, r3
	ldrh	r0, [r5, #8]
	ldr	r3, [pc, #72]
	ands	r3, r0
	orrs	r3, r2
	strh	r3, [r5, #8]
	ldrb	r3, [r5, #5]
	ands	r1, r3
	movs	r3, #4
	orrs	r1, r3
	strb	r1, [r5, #5]
	ldr	r3, [pc, #28]
	ldr	r1, [sp, #8]
	asrs	r2, r7, #16
	ands	r2, r3
	ldr	r3, [pc, #24]
	subs	r2, r2, r1
	ands	r2, r3
	ldrh	r1, [r5, #6]
	ldr	r3, [pc, #40]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #6]
	mov	r2, sl
	b.n	.L4
	.4byte 0x000003ff
	.4byte 0x0000fff0
	.4byte 0x000001ff
	.4byte 0x03001ee0
	.4byte 0x03001b10
	.4byte 0xfff80000
	.4byte 0x40000800
	.4byte 0xfffffc00
	.4byte 0xfffffe00
.L4:
	ldr	r1, [sp, #4]
	asrs	r3, r2, #16
	movs	r2, #240
	ands	r3, r2
	subs	r3, r3, r1
	subs	r3, r3, r6
	adds	r3, #16
	strb	r3, [r5, #4]
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_08003dec
.L3:
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r7, r7, r2
	adds	r1, r7, #0
	add	r2, sl
	mov	r0, r8
	bl	Func_080091a8
	movs	r2, #128
	lsls	r2, r2, #14
	asrs	r6, r0, #16
	add	r2, sl
	mov	r0, r8
	adds	r1, r7, #0
	bl	Func_080091a8
	asrs	r0, r0, #16
	mov	r5, fp
	subs	r0, #16
	adds	r5, #12
	cmp	r0, r6
	ble.n	.L5
	adds	r6, r0, #0
.L5:
	cmp	r6, #0
	ble.n	.L1
	ldr	r2, [sp, #0]
	cmp	r6, r2
	ble.n	.L1
	ldr	r3, [pc, #84]
	movs	r2, #13
	ldrb	r1, [r5, #9]
	str	r3, [r5, #4]
	negs	r2, r2
	movs	r3, #0
	str	r3, [r5, #8]
	adds	r3, r2, #0
	ands	r3, r1
	strb	r3, [r5, #9]
	ldr	r3, [pc, #52]
	mov	r1, r9
	ands	r1, r3
	mov	r9, r1
	ldr	r3, [pc, #60]
	ldrh	r1, [r5, #8]
	ands	r3, r1
	mov	r1, r9
	orrs	r3, r1
	strh	r3, [r5, #8]
	ldrb	r3, [r5, #5]
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	ldr	r3, [pc, #28]
	strb	r2, [r5, #5]
	asrs	r2, r7, #16
	ands	r2, r3
	ldr	r3, [sp, #8]
	subs	r2, r2, r3
	ldr	r3, [pc, #20]
	ldrh	r1, [r5, #6]
	ands	r2, r3
	ldr	r3, [pc, #28]
	ands	r3, r1
	orrs	r3, r2
	b.n	.L6
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x0000fff0
	.4byte 0x000001ff
	.4byte 0x40000800
	.4byte 0xfffffc00
	.4byte 0xfffffe00
.L6:
	mov	r1, sl
	strh	r3, [r5, #6]
	movs	r2, #240
	asrs	r3, r1, #16
	ands	r3, r2
	ldr	r2, [sp, #4]
	subs	r3, r3, r2
	subs	r3, r3, r6
	adds	r3, #16
	strb	r3, [r5, #4]
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_08003dec
.L1:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
