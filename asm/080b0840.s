@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b0840
	.thumb_func
Func_080b0840:
	push	{r5, r6, lr}
	ldr	r3, [pc, #60]
	movs	r2, #224
	ldr	r5, [r3, #20]
	lsls	r2, r2, #4
	ldr	r1, [r3, #0]
	adds	r4, r5, r2
	ldr	r2, [pc, #52]
	adds	r6, r0, #0
	adds	r1, r1, r2
	ldr	r3, [pc, #48]
	adds	r0, r4, #0
	ldr	r2, [pc, #48]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #224
	lsls	r2, r2, #2
	adds	r1, r5, r2
	adds	r0, r4, #0
	ldr	r2, [pc, #40]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_0808a330
	movs	r0, #16
	bl	Func_0808a348
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001ebc
	.4byte 0x00000236
	.4byte 0x040000d4
	.4byte 0x84000150
	.4byte 0x840002a0
