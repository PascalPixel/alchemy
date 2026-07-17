@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08093304
	.thumb_func
Func_08093304:
	push	{r5, lr}
	ldr	r3, [pc, #64]
	movs	r1, #128
	lsls	r1, r1, #24
	ldr	r5, [r3, #0]
	cmp	r0, r1
	bne.n	.L0
	ldr	r2, [pc, #56]
	ldr	r1, [pc, #56]
	adds	r3, r5, r2
	movs	r2, #0
	strh	r2, [r3, #0]
	b.n	.L1
.L0:
	bl	Func_08092ba8
	bl	Func_080915ac
	ldr	r3, [pc, #44]
	movs	r1, #131
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldrb	r3, [r3, #0]
	ldr	r1, [pc, #24]
	ldr	r2, [pc, #36]
	ldrb	r2, [r2, r3]
	adds	r3, r5, r1
	adds	r1, #2
	strh	r0, [r3, #0]
.L1:
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x000012f4
	.4byte 0x000012f6
	.4byte 0x02000240
	.4byte 0x0809fc28
