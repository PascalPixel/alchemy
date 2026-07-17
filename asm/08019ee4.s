@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08019ee4
	.thumb_func
Func_08019ee4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r1, #0
	movs	r1, #193
	sub	sp, #4
	adds	r6, r0, #0
	lsls	r1, r1, #3
	movs	r0, #17
	str	r3, [sp, #0]
	mov	fp, r2
	bl	Func_080048b0
	ldr	r3, [pc, #128]
	lsls	r5, r5, #2
	ldr	r2, [pc, #128]
	ldr	r3, [r3, r5]
	adds	r7, r0, #0
	adds	r2, r2, r7
	str	r3, [r2, #0]
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, r3, r7
	mov	sl, r3
	ldr	r3, [pc, #112]
	adds	r3, r3, r7
	movs	r5, #2
	mov	r8, r3
	mov	r9, r2
	mov	r2, sl
	strh	r5, [r2, #0]
	mov	r2, r8
	strh	r5, [r2, #0]
	movs	r1, #0
	bl	Func_0801a5a4
	ldr	r3, [pc, #92]
	lsls	r6, r6, #2
	ldr	r3, [r3, r6]
	mov	r2, r9
	str	r3, [r2, #0]
	mov	r3, sl
	mov	r2, r8
	strh	r5, [r3, #0]
	adds	r0, r7, #0
	strh	r5, [r2, #0]
	movs	r1, #1
	bl	Func_0801a5a4
	ldr	r3, [sp, #36]
	cmp	r3, #0
	bne.n	.L0
	bl	Func_08004080
	mov	r2, fp
	str	r0, [r2, #0]
.L0:
	mov	r3, fp
	ldr	r0, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r2, r7, r3
	movs	r1, #128
	bl	Func_08003fa4
	ldr	r2, [sp, #0]
	str	r0, [r2, #0]
	movs	r0, #17
	bl	Func_08002dd8
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x08029a10
	.4byte 0x00000604
	.4byte 0x00000602
	.4byte 0x08029e00
