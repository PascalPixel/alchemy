@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08006dec
	.thumb_func
Func_08006dec:
	push	{r4, lr}
	ldr	r4, [pc, #40]
	movs	r2, #170
	strb	r2, [r4, #0]
	ldr	r3, [pc, #36]
	movs	r2, #85
	strb	r2, [r3, #0]
	movs	r2, #160
	strb	r2, [r4, #0]
	ldrb	r2, [r0, #0]
	strb	r2, [r1, #0]
	ldr	r3, [pc, #28]
	ldrb	r2, [r0, #0]
	ldr	r3, [r3, #0]
	movs	r0, #1
	bl	Func_080072f0
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	pop	{r4}
	pop	{r1}
	bx	r1
	.4byte 0x0e005555
	.4byte 0x0e002aaa
	.4byte 0x02004c00
