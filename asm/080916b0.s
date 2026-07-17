@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080916b0
	.thumb_func
Func_080916b0:
	push	{r5, r6, lr}
	ldr	r3, [pc, #132]
	ldr	r6, [r3, #0]
	bl	Func_08015208
	bl	Func_08091660
	ldr	r2, [pc, #124]
	adds	r3, r6, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L0
	bl	Func_0808e118
.L0:
	ldr	r2, [pc, #112]
	movs	r5, #0
	adds	r3, r6, r2
	adds	r2, #2
	strh	r5, [r3, #0]
	adds	r3, r6, r2
	strh	r5, [r3, #0]
	movs	r3, #228
	lsls	r3, r3, #1
	adds	r2, r6, r3
	movs	r3, #16
	str	r3, [r2, #0]
	movs	r2, #230
	lsls	r2, r2, #1
	adds	r3, r6, r2
	str	r5, [r3, #0]
	movs	r3, #237
	lsls	r3, r3, #1
	adds	r2, r6, r3
	ldr	r3, [pc, #76]
	strh	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #1
	adds	r2, r6, r3
	movs	r3, #1
	negs	r3, r3
	strh	r3, [r2, #0]
	movs	r3, #239
	lsls	r3, r3, #1
	adds	r2, r6, r3
	movs	r3, #1
	negs	r3, r3
	movs	r1, #200
	strh	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #48]
	bl	Func_080041d8
	movs	r0, #153
	lsls	r0, r0, #1
	bl	Func_080770d0
	ldr	r3, [pc, #40]
	movs	r2, #250
	lsls	r2, r2, #1
	ldr	r3, [r3, r2]
	str	r3, [r6, r2]
	adds	r2, #4
	adds	r3, r6, r2
	str	r5, [r3, #0]
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001ebc
	.4byte 0x00000cb6
	.4byte 0x00000cc2
	.4byte 0x0000ffff
	.4byte 0x080915ed
	.4byte 0x02000240
