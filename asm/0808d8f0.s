@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0808d8f0
	.thumb_func
Func_0808d8f0:
	push	{r5, lr}
	ldr	r3, [pc, #164]
	movs	r2, #250
	ldr	r5, [r3, #0]
	ldr	r3, [pc, #160]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	cmp	r0, #252
	beq.n	.L0
	cmp	r0, #252
	bgt.n	.L1
	cmp	r0, #249
	beq.n	.L2
	b.n	.L3
.L1:
	cmp	r0, #253
	beq.n	.L4
	cmp	r0, #254
	beq.n	.L2
	b.n	.L3
.L0:
	movs	r3, #206
	lsls	r3, r3, #1
	adds	r5, r5, r3
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #12
	ble.n	.L3
	ldr	r3, [pc, #120]
	ldr	r3, [r3, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L3
	adds	r0, r1, #0
	movs	r2, #0
	movs	r1, #6
	bl	Func_08092708
	b.n	.L5
.L2:
	movs	r3, #206
	lsls	r3, r3, #1
	adds	r5, r5, r3
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #12
	ble.n	.L3
	bl	Func_08093c00
.L5:
	movs	r3, #0
	strh	r3, [r5, #0]
	b.n	.L3
.L4:
	movs	r2, #206
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #12
	ble.n	.L3
	ldr	r1, [pc, #56]
	ldr	r3, [r1, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L6
	bl	Func_08093e28
	b.n	.L7
.L6:
	ldr	r3, [r1, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L7
	bl	Func_08093fa0
.L7:
	movs	r3, #206
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #0
	strh	r3, [r2, #0]
.L3:
	movs	r0, #0
	pop	{r5}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x03001ae8
