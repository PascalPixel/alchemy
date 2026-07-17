@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080105d4
	.thumb_func
Func_080105d4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #36
	ldr	r6, [sp, #72]
	lsls	r1, r1, #7
	adds	r1, r1, r0
	ldr	r0, [sp, #68]
	adds	r4, r3, #0
	lsls	r3, r6, #7
	adds	r3, r3, r0
	mov	fp, r2
	ldr	r2, [pc, #240]
	lsls	r1, r1, #2
	lsls	r3, r3, #2
	adds	r1, r1, r2
	adds	r3, r3, r2
	str	r1, [sp, #8]
	str	r3, [sp, #4]
	ldr	r3, [pc, #232]
	movs	r1, #130
	ldr	r3, [r3, #0]
	lsls	r1, r1, #1
	add	r0, sp, #12
	adds	r2, r3, r1
	mov	r9, r0
	movs	r5, #2
.L0:
	ldr	r3, [r2, #0]
	asrs	r3, r3, #20
	str	r3, [r0, #0]
	ldr	r3, [r2, #4]
	subs	r5, #1
	asrs	r3, r3, #20
	str	r3, [r0, #4]
	adds	r2, #48
	adds	r0, #8
	cmp	r5, #0
	bge.n	.L0
	adds	r3, r6, r4
	cmp	r6, r3
	bge.n	.L1
	str	r3, [sp, #0]
	mov	r2, fp
	movs	r3, #128
	subs	r3, r3, r2
	lsls	r3, r3, #2
	mov	r8, r3
.L7:
	ldr	r1, [sp, #68]
	mov	r4, fp
	adds	r3, r1, r4
	cmp	r1, r3
	bge.n	.L2
	mov	ip, r6
	movs	r5, #15
	mov	r0, ip
	ands	r0, r5
	mov	lr, r3
	mov	sl, r5
	mov	ip, r0
.L6:
	ldr	r4, [sp, #8]
	ldmia	r4!, {r3}
	ldr	r0, [sp, #4]
	ldr	r7, [pc, #148]
	adds	r2, r4, #0
	str	r2, [sp, #8]
	mov	r4, ip
	stmia	r0!, {r3}
	ands	r7, r3
	adds	r2, r1, #0
	mov	r3, sl
	ands	r2, r3
	lsls	r3, r4, #5
	adds	r5, r0, #0
	adds	r3, r3, r2
	str	r5, [sp, #4]
	mov	r0, r9
	movs	r5, #0
	lsls	r4, r3, #2
.L5:
	ldr	r3, [r0, #0]
	cmp	r3, r1
	bgt.n	.L3
	adds	r3, #16
	cmp	r3, r1
	ble.n	.L3
	ldr	r3, [r0, #4]
	cmp	r3, r6
	bgt.n	.L3
	adds	r3, #12
	cmp	r3, r6
	ble.n	.L3
	ldr	r5, [pc, #96]
	adds	r0, r4, r5
	ldr	r5, [pc, #96]
	lsls	r3, r7, #3
	adds	r2, r3, r5
	ldr	r2, [r2, #0]
	str	r2, [r0, #0]
	ldr	r0, [pc, #92]
	adds	r2, r3, r0
	ldr	r3, [pc, #92]
	adds	r0, r4, r3
	ldr	r3, [r2, #0]
	str	r3, [r0, #0]
	b.n	.L4
.L3:
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r5, #1
	adds	r4, r4, r2
	adds	r0, #8
	cmp	r5, #2
	ble.n	.L5
.L4:
	adds	r1, #1
	cmp	r1, lr
	blt.n	.L6
.L2:
	ldr	r3, [sp, #8]
	ldr	r4, [sp, #4]
	ldr	r5, [sp, #0]
	add	r3, r8
	add	r4, r8
	adds	r6, #1
	str	r3, [sp, #8]
	str	r4, [sp, #4]
	cmp	r6, r5
	blt.n	.L7
.L1:
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x03001e70
	.4byte 0x00000fff
	.4byte 0x06002800
	.4byte 0x02020000
	.4byte 0x02020004
	.4byte 0x06002840
