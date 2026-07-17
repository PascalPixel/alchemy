@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801d980
	.thumb_func
Func_0801d980:
	push	{lr}
	movs	r1, #197
	lsls	r1, r1, #3
	movs	r0, #20
	sub	sp, #4
	bl	Func_080048f4
	movs	r3, #0
	adds	r1, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	ldr	r2, [pc, #24]
	ldr	r3, [pc, #24]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #20]
	bl	Func_080041d8
	add	sp, #4
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x8500018a
	.4byte 0x040000d4
	.4byte 0x0801d94d
