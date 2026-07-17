@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080049ac
	.thumb_func
Func_080049ac:
	push	{r5, lr}
	movs	r1, #48
	movs	r0, #2
	ldr	r5, [pc, #40]
	bl	Func_080048f4
	ldr	r2, [pc, #36]
	movs	r3, #0
	str	r3, [r2, #0]
	str	r0, [r5, #0]
	ldr	r3, [pc, #32]
	adds	r0, r3, #0
	movs	r1, #128
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	lsls	r1, r1, #9
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001d2c
	.4byte 0x03001cc4
	.4byte 0x03000350
