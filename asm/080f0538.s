@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080f0538
	.thumb_func
Func_080f0538:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #72]
	ldrh	r3, [r3, #0]
	movs	r4, #7
	ands	r4, r3
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	cmp	r2, #0
	bge.n	.L0
	adds	r2, #7
.L0:
	ldr	r3, [pc, #52]
	asrs	r2, r2, #3
	ands	r2, r3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r1, r3, #3
	ldr	r3, [pc, #48]
	ldr	r3, [r3, #0]
	adds	r0, r3, #0
	negs	r3, r4
	adds	r3, #16
	ldr	r2, [pc, #40]
	mov	lr, r3
	movs	r3, #192
	lsls	r3, r3, #2
	movs	r7, #128
	adds	r0, #192
	movs	r6, #0
	mov	sl, r2
	mov	r8, r3
	lsls	r7, r7, #14
.L3:
	movs	r5, #192
	mov	ip, lr
	lsls	r5, r5, #13
	movs	r4, #5
	b.n	.L1
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x02004c00
	.4byte 0x02004c0c
	.4byte 0x40004000
.L1:
	mov	r3, ip
	mov	r2, sl
	orrs	r3, r5
	orrs	r3, r2
	adds	r2, r0, #0
	stmia	r2!, {r3}
	str	r1, [r2, #0]
	adds	r1, #4
	adds	r0, #8
	cmp	r1, r8
	bne.n	.L2
	movs	r1, #0
.L2:
	subs	r4, #1
	adds	r5, r5, r7
	cmp	r4, #0
	bge.n	.L1
	movs	r3, #8
	adds	r6, #1
	add	lr, r3
	cmp	r6, #15
	ble.n	.L3
	ldr	r2, [pc, #56]
	movs	r1, #224
	ldr	r0, [r2, #0]
	ldr	r3, [pc, #52]
	lsls	r1, r1, #19
	ldr	r2, [pc, #52]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #52]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L4
	ldr	r3, [pc, #44]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L4
	ldr	r2, [pc, #36]
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
.L4:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02004c0c
	.4byte 0x040000d4
	.4byte 0x84000100
	.4byte 0x02004c04
	.4byte 0x03001800
	.4byte 0x02004c00
