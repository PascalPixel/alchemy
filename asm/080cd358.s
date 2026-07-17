@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080cd358
	.thumb_func
Func_080cd358:
	push	{r5, lr}
	ldr	r3, [pc, #160]
	ldr	r2, [pc, #160]
	ldr	r5, [r3, #0]
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L0
	subs	r2, #164
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	cmp	r3, #1
	beq.n	.L1
	cmp	r3, #1
	bgt.n	.L2
	cmp	r3, #0
	beq.n	.L3
	b.n	.L4
.L2:
	cmp	r3, #2
	beq.n	.L5
	cmp	r3, #3
	beq.n	.L6
	b.n	.L4
.L3:
	movs	r2, #240
	ldr	r3, [pc, #120]
	ldr	r0, [pc, #124]
	ldr	r1, [pc, #124]
	lsls	r2, r2, #7
	bl	Func_080072f0
	b.n	.L4
.L1:
	movs	r2, #240
	ldr	r3, [pc, #104]
	ldr	r1, [pc, #112]
	lsls	r2, r2, #7
	ldr	r0, [pc, #104]
	bl	Func_080072f0
	ldr	r2, [pc, #104]
	movs	r1, #240
	adds	r3, r5, r2
	ldr	r2, [r3, #0]
	ldr	r0, [pc, #92]
	ldr	r3, [pc, #100]
	lsls	r1, r1, #7
	bl	Func_080072f0
	b.n	.L4
.L5:
	ldr	r2, [pc, #84]
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	cmp	r3, #50
	bne.n	.L7
	ldr	r1, [pc, #68]
	adds	r2, #124
	ldr	r0, [pc, #68]
	bl	Func_08005534
	b.n	.L4
.L7:
	movs	r2, #240
	ldr	r1, [pc, #52]
	lsls	r2, r2, #7
	ldr	r0, [pc, #52]
	bl	Func_080054e4
	b.n	.L4
.L6:
	ldr	r2, [pc, #48]
	adds	r3, r5, r2
	ldr	r1, [r3, #0]
	movs	r3, #240
	ldr	r2, [pc, #32]
	lsls	r3, r3, #7
	ldr	r0, [pc, #32]
	bl	Func_08005490
.L4:
	ldr	r3, [pc, #16]
	adds	r2, r5, r3
	movs	r3, #0
	str	r3, [r2, #0]
.L0:
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x03001eec
	.4byte 0x00007824
	.4byte 0x03001388
	.4byte 0x06008000
	.4byte 0x02010000
	.4byte 0x00007784
	.4byte 0x03000168
	ldr	r3, [pc, #92]
	ldr	r0, [pc, #96]
	ldr	r2, [r3, #0]
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	ldr	r1, [pc, #92]
	adds	r0, #2
	strh	r3, [r1, #0]
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	adds	r1, #4
	strh	r3, [r1, #0]
	adds	r0, #2
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	subs	r1, #2
	strh	r3, [r1, #0]
	adds	r0, #2
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	adds	r1, #4
	strh	r3, [r1, #0]
	adds	r0, #2
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	adds	r1, #2
	strh	r3, [r1, #0]
	adds	r0, #2
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	adds	r1, #2
	strh	r3, [r1, #0]
	adds	r0, #2
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	subs	r1, #74
	strh	r3, [r1, #0]
	adds	r0, #2
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	adds	r1, #80
	strh	r3, [r1, #0]
	ldr	r3, [pc, #20]
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r1, #2
	strh	r3, [r1, #0]
	bx	lr
	.4byte 0x03001eec
	.4byte 0x000077bc
	.4byte 0x04000040
	.4byte 0x000077cc
