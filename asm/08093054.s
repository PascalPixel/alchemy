@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08093054
	.thumb_func
Func_08093054:
	push	{r5, r6, r7, lr}
	adds	r6, r1, #0
	adds	r5, r0, #0
	bl	Func_08092c40
	ldr	r3, [pc, #80]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl	Func_08091c7c
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L0
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	Func_08092f84
	ldr	r3, [pc, #52]
	ldr	r2, [r3, #0]
	movs	r3, #236
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L1
.L0:
	ldr	r3, [pc, #36]
	ldr	r2, [r3, #0]
	movs	r3, #236
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	Func_08092f84
.L1:
	adds	r0, r7, #0
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x03001ebc
	bx	lr
