@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080cdbc0
	.thumb_func
Func_080cdbc0:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #88]
	ldr	r0, [pc, #88]
	ldr	r5, [r3, #0]
	subs	r3, #120
	ldr	r7, [r3, #0]
	bl	Func_080f9010
	ldr	r2, [pc, #80]
	adds	r3, r5, r2
	ldr	r6, [pc, #80]
	ldr	r3, [r3, #0]
	strh	r3, [r6, #4]
	ldr	r3, [pc, #80]
	adds	r5, r5, r3
	ldr	r3, [r5, #0]
	ldr	r2, [pc, #76]
	strh	r3, [r6, #6]
	movs	r3, #120
	str	r3, [r2, #12]
	str	r3, [r2, #16]
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #40]
	movs	r1, #128
	strh	r3, [r2, #0]
	lsls	r1, r1, #7
	ldr	r3, [pc, #64]
	ldr	r0, [pc, #68]
	bl	Func_080072f0
	ldr	r0, [pc, #64]
	bl	Func_08004278
	ldr	r1, [pc, #64]
	movs	r3, #32
	strh	r3, [r6, #6]
	ldr	r0, [pc, #60]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L0
	b.n	.L1
	.4byte 0x00000787
	.4byte 0x03001eec
	.4byte 0x00000121
	.4byte 0x000077a0
	.4byte 0x03001ad0
	.4byte 0x000077a4
	.4byte 0x03001ce0
	.4byte 0x0400000c
	.4byte 0x03000164
	.4byte 0x06004000
	.4byte 0x080cd4b5
	.4byte 0x02002090
	.4byte 0x04000208
.L1:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r1
	strh	r2, [r1, #0]
	ldr	r2, [pc, #72]
	adds	r3, #4
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L0:
	strh	r4, [r0, #0]
	ldr	r2, [pc, #56]
	ldr	r3, [pc, #48]
	movs	r0, #1
	strh	r3, [r2, #0]
	bl	Func_080030f8
	movs	r2, #201
	lsls	r2, r2, #3
	adds	r3, r7, r2
	ldrh	r1, [r3, #0]
	movs	r2, #7
	movs	r0, #2
	bl	Func_080b5038
	movs	r0, #1
	bl	Func_080030f8
	movs	r3, #201
	lsls	r3, r3, #3
	movs	r2, #21
	movs	r6, #0
	adds	r7, r7, r3
	movs	r5, #0
	mov	r8, r2
	b.n	.L2
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x00007341
	.4byte 0x04000050
.L2:
	mov	r3, r8
	subs	r1, r3, r5
	ldrh	r0, [r7, #0]
	bl	Func_080b5048
	adds	r6, #1
	movs	r0, #1
	bl	Func_080030f8
	adds	r5, #3
	cmp	r6, #8
	bne.n	.L2
	ldr	r1, [pc, #60]
	ldr	r0, [pc, #64]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r1
	strh	r2, [r1, #0]
	ldr	r2, [pc, #40]
	adds	r3, #4
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L3:
	strh	r4, [r0, #0]
	movs	r0, #1
	bl	Func_080030f8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x00007541
