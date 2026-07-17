@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08097b70
	.thumb_func
Func_08097b70:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	ldr	r0, [r5, #104]
	sub	sp, #12
	cmp	r0, #0
	beq.n	.L0
	ldr	r2, [r0, #8]
	ldr	r3, [r5, #8]
	subs	r1, r2, r3
	ldr	r2, [r0, #16]
	ldr	r3, [r5, #16]
	subs	r0, r2, r3
	cmp	r1, #0
	bne.n	.L1
	cmp	r0, #0
	beq.n	.L2
.L1:
	bl	Func_080044d0
	ldrh	r3, [r5, #6]
	subs	r0, r0, r3
	lsls	r0, r0, #16
	movs	r2, #128
	asrs	r0, r0, #16
	lsls	r2, r2, #5
	cmp	r0, r2
	ble.n	.L3
	adds	r0, r2, #0
.L3:
	ldr	r2, [pc, #128]
	cmp	r0, r2
	bge.n	.L4
	adds	r0, r2, #0
.L4:
	adds	r3, r3, r0
	strh	r3, [r5, #6]
.L2:
	adds	r2, r5, #0
	adds	r2, #90
	movs	r3, #0
	strb	r3, [r2, #0]
.L0:
	ldr	r3, [r5, #8]
	mov	r6, sp
	str	r3, [r6, #0]
	bl	Func_08004458
	ldr	r3, [r5, #12]
	ldr	r2, [pc, #100]
	lsls	r0, r0, #4
	subs	r3, r3, r0
	adds	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	str	r3, [r6, #8]
	bl	Func_08004458
	lsls	r5, r0, #1
	adds	r5, r5, r0
	bl	Func_08004458
	lsls	r5, r5, #4
	adds	r1, r0, #0
	adds	r2, r6, #0
	adds	r0, r5, #0
	bl	Func_0800447c
	ldr	r0, [pc, #64]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	bl	Func_08096c80
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L5
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r3, [pc, #44]
	movs	r1, #0
	str	r3, [r5, #72]
	bl	Func_08009080
	adds	r2, r5, #0
	adds	r2, #94
	movs	r3, #12
	strh	r3, [r2, #0]
	ldr	r1, [pc, #28]
	adds	r0, r5, #0
	bl	Func_08009098
.L5:
	add	sp, #12
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0xfffff000
	.4byte 0xfff80000
	.4byte 0x0000011d
	.4byte 0x00001999
	.4byte 0x0809f0b0
