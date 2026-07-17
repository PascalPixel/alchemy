@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0809a3c4
	.thumb_func
Func_0809a3c4:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	adds	r6, r1, #0
	mov	r8, r2
	movs	r0, #138
	adds	r7, r3, #0
	bl	Func_080f9010
	adds	r1, r5, #0
	movs	r0, #215
	adds	r2, r6, #0
	mov	r3, r8
	bl	Func_08096c80
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L0
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	movs	r3, #192
	lsls	r3, r3, #10
	adds	r2, r5, #0
	str	r3, [r5, #48]
	str	r3, [r5, #52]
	adds	r2, #90
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #1
	bl	Func_08009080
	movs	r2, #128
	ldr	r3, [r5, #24]
	lsls	r2, r2, #9
	cmp	r3, r2
	bge.n	.L1
	ldr	r6, [pc, #32]
.L2:
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r3, r3, r2
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	ldrh	r3, [r5, #6]
	adds	r3, r3, r6
	strh	r3, [r5, #6]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [r5, #24]
	ldr	r2, [pc, #8]
	cmp	r3, r2
	ble.n	.L2
	b.n	.L1
	.4byte 0x00002000
	.4byte 0x0000ffff
.L1:
	strh	r7, [r5, #6]
.L0:
	adds	r0, r5, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	ldr	r3, [r0, #8]
	ldr	r2, [r0, #68]
	adds	r3, r3, r2
	str	r3, [r0, #8]
	ldr	r2, [r0, #72]
	ldr	r3, [r0, #12]
	adds	r3, r3, r2
	str	r3, [r0, #12]
	ldr	r2, [r0, #76]
	ldr	r3, [r0, #16]
	adds	r3, r3, r2
	str	r3, [r0, #16]
	ldr	r2, [r0, #48]
	ldr	r3, [r0, #24]
	adds	r3, r3, r2
	str	r3, [r0, #24]
	ldr	r2, [r0, #52]
	ldr	r3, [r0, #28]
	adds	r3, r3, r2
	str	r3, [r0, #28]
	ldr	r1, [r0, #80]
	adds	r0, #100
	ldrh	r3, [r1, #30]
	ldrh	r2, [r0, #0]
	adds	r3, r3, r2
	strh	r3, [r1, #30]
	bx	lr
