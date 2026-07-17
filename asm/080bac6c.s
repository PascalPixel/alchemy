@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080bac6c
	.thumb_func
Func_080bac6c:
	push	{r5, r6, lr}
	ldr	r3, [pc, #40]
	adds	r6, r0, #0
	ldr	r5, [r3, #0]
	bl	Func_08077008
	movs	r3, #149
	lsls	r3, r3, #1
	adds	r2, r0, r3
	ldr	r1, [pc, #20]
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r2, #88
	b.n	.L0
.L3:
	adds	r2, #2
.L0:
	ldrsh	r3, [r2, r5]
	cmp	r3, r6
	bne.n	.L1
	strh	r1, [r2, r5]
	b.n	.L2
	.4byte 0x000000fe
	.4byte 0x03001e74
.L1:
	cmp	r3, #255
	bne.n	.L3
	movs	r1, #0
	adds	r0, r5, #2
.L5:
	lsls	r3, r1, #1
	adds	r2, r3, #0
	adds	r2, #100
	ldrsh	r3, [r0, r2]
	cmp	r3, r6
	bne.n	.L4
	ldr	r3, [pc, #12]
	strh	r3, [r0, r2]
	b.n	.L2
.L4:
	adds	r1, #1
	cmp	r3, #255
	bne.n	.L5
	b.n	.L6
	movs	r0, r0
	.4byte 0x000000fe
.L2:
	adds	r0, r6, #0
	bl	Func_080c1ebc
	movs	r2, #187
	movs	r1, #0
	movs	r0, #255
	lsls	r2, r2, #2
.L8:
	ldrsh	r3, [r2, r5]
	cmp	r3, r6
	bne.n	.L7
	strh	r0, [r2, r5]
.L7:
	adds	r1, #1
	adds	r2, #16
	cmp	r1, #19
	bls.n	.L8
.L6:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
