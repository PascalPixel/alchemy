@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08003a7c
	.thumb_func
Func_08003a7c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r6, [pc, #68]
	ldrh	r5, [r6, #0]
	mov	r7, sp
	cmp	r5, #0
	beq.n	.L0
	ldr	r2, [pc, #64]
	movs	r1, #0
	lsrs	r2, r2, #2
	lsls	r3, r2, #2
	mov	sl, r1
	movs	r4, #132
	mov	r1, sp
	subs	r1, r1, r3
	lsls	r4, r4, #24
	mov	r8, sp
	ldr	r3, [pc, #48]
	mov	sp, r1
	ldr	r0, [pc, #48]
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	Func_08007318
	mov	r3, sl
	strh	r3, [r6, #0]
	mov	sp, r8
.L0:
	mov	sp, r7
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02002090
	.4byte 0x00000068
	.4byte 0x040000d4
	.4byte 0x08002cf4
