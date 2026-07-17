@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08091254
	.thumb_func
Func_08091254:
	push	{r5, lr}
	ldr	r3, [pc, #52]
	ldr	r4, [r3, #0]
	adds	r5, r0, #0
	cmp	r4, #0
	beq.n	.L0
	ldr	r1, [pc, #44]
	adds	r3, r4, r1
	adds	r1, #1
	movs	r2, #0
	strb	r5, [r3, #0]
	adds	r3, r4, r1
	strb	r2, [r3, #0]
	movs	r3, #224
	lsls	r3, r3, #4
	movs	r2, #224
	adds	r1, r4, r3
	movs	r3, #196
	lsls	r2, r2, #2
	lsls	r3, r3, #5
	adds	r0, r4, r2
	adds	r2, r4, r3
	adds	r3, r5, #0
	bl	Func_0809088c
.L0:
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x03001ed0
	.4byte 0x00002a01
