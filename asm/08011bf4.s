@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08011bf4
	.thumb_func
Func_08011bf4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #212]
	ldr	r3, [r3, #0]
	mov	sl, r3
	adds	r3, #176
	ldrh	r3, [r3, #0]
	movs	r2, #3
	movs	r1, #0
	mov	lr, r1
	ands	r2, r3
	sub	sp, #32
	cmp	lr, r2
	bcs.n	.L0
	mov	r9, r2
.L8:
	movs	r3, #44
	mov	r2, lr
	muls	r2, r3
	mov	r1, sl
	adds	r3, r2, #0
	adds	r5, r1, r3
	ldrh	r2, [r5, #6]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L1
	movs	r2, #4
	ldrsh	r7, [r5, r2]
	movs	r3, #10
	ldrsh	r2, [r5, r3]
	subs	r3, r2, r7
	lsls	r3, r3, #24
	ldr	r1, [r5, #0]
	lsls	r6, r2, #16
	adds	r4, r5, #0
	lsrs	r0, r3, #24
	lsrs	r3, r6, #16
	mov	r8, r1
	adds	r4, #12
	cmp	r0, r3
	bcs.n	.L2
	mov	r1, sp
	mov	ip, r3
.L3:
	ldrh	r3, [r4, #0]
	lsls	r2, r0, #1
	strh	r3, [r1, r2]
	adds	r3, r0, #1
	lsls	r3, r3, #24
	lsrs	r0, r3, #24
	adds	r4, #2
	cmp	r0, ip
	bcc.n	.L3
.L2:
	lsls	r1, r7, #16
	adds	r7, r1, #0
	lsrs	r2, r6, #16
	lsrs	r3, r7, #16
	movs	r0, #0
	subs	r2, r2, r3
	cmp	r0, r2
	bge.n	.L4
	mov	r1, sp
	mov	ip, r2
.L5:
	ldrh	r3, [r4, #0]
	lsls	r2, r0, #1
	strh	r3, [r1, r2]
	adds	r3, r0, #1
	lsls	r3, r3, #24
	lsrs	r0, r3, #24
	adds	r4, #2
	cmp	r0, ip
	blt.n	.L5
.L4:
	movs	r2, #128
	lsrs	r4, r6, #16
	lsls	r2, r2, #24
	ldr	r3, [pc, #72]
	mov	r0, sp
	mov	r1, r8
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r1, r7, r2
	lsrs	r3, r1, #16
	cmp	r3, r4
	bcc.n	.L6
	movs	r1, #0
.L6:
	lsrs	r3, r1, #16
	strh	r3, [r5, #4]
	ldrh	r3, [r5, #8]
	b.n	.L7
.L1:
	ldr	r1, [pc, #44]
	adds	r3, r2, r1
.L7:
	strh	r3, [r5, #6]
	mov	r3, lr
	adds	r3, #1
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	mov	lr, r3
	cmp	lr, r9
	bcc.n	.L8
.L0:
	add	sp, #32
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ec0
	.4byte 0x040000d4
	.4byte 0x0000ffff
