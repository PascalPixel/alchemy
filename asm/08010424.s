@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08010424
	.thumb_func
Func_08010424:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r4, r3, #0
	mov	fp, r2
	lsls	r1, r1, #7
	lsls	r3, r4, #7
	ldr	r2, [pc, #260]
	adds	r1, r1, r0
	add	r3, fp
	lsls	r1, r1, #2
	lsls	r3, r3, #2
	sub	sp, #36
	adds	r3, r3, r2
	adds	r1, r1, r2
	str	r3, [sp, #4]
	str	r1, [sp, #8]
	ldr	r3, [pc, #244]
	movs	r0, #130
	ldr	r3, [r3, #0]
	lsls	r0, r0, #1
	adds	r2, r3, r0
	add	r0, sp, #12
	mov	r9, r0
	movs	r6, #2
.L0:
	ldr	r3, [r2, #0]
	asrs	r3, r3, #20
	str	r3, [r0, #0]
	ldr	r3, [r2, #4]
	subs	r6, #1
	asrs	r3, r3, #20
	str	r3, [r0, #4]
	adds	r2, #48
	adds	r0, #8
	cmp	r6, #0
	bge.n	.L0
	ldr	r3, [sp, #72]
	adds	r7, r4, #0
	adds	r3, r7, r3
	cmp	r7, r3
	bcs.n	.L1
	ldr	r1, [sp, #68]
	str	r3, [sp, #0]
	movs	r3, #128
	subs	r3, r3, r1
	lsls	r3, r3, #2
	mov	r8, r3
.L7:
	ldr	r2, [sp, #68]
	mov	r1, fp
	adds	r3, r1, r2
	cmp	r1, r3
	bcs.n	.L2
	mov	ip, r7
	mov	r4, ip
	mov	lr, r3
	movs	r3, #15
	ands	r4, r3
	mov	sl, r3
	mov	ip, r4
.L6:
	ldr	r2, [sp, #8]
	ldmia	r2!, {r5}
	adds	r0, r2, #0
	ldr	r4, [sp, #4]
	str	r0, [sp, #8]
	ldr	r3, [pc, #152]
	ldr	r2, [pc, #156]
	ands	r5, r3
	ldr	r3, [r4, #0]
	ands	r3, r2
	orrs	r3, r5
	stmia	r4!, {r3}
	adds	r2, r1, #0
	adds	r0, r4, #0
	mov	r3, sl
	mov	r4, ip
	ands	r2, r3
	lsls	r3, r4, #5
	adds	r3, r3, r2
	str	r0, [sp, #4]
	movs	r6, #0
	mov	r0, r9
	lsls	r4, r3, #2
.L5:
	ldr	r3, [r0, #0]
	cmp	r3, r1
	bgt.n	.L3
	adds	r3, #16
	cmp	r3, r1
	ble.n	.L3
	ldr	r3, [r0, #4]
	cmp	r3, r7
	bgt.n	.L3
	adds	r3, #12
	cmp	r3, r7
	ble.n	.L3
	lsls	r3, r5, #3
	ldr	r2, [pc, #100]
	ldr	r5, [pc, #100]
	adds	r0, r4, r2
	adds	r2, r3, r5
	ldr	r2, [r2, #0]
	str	r2, [r0, #0]
	ldr	r0, [pc, #96]
	adds	r2, r3, r0
	ldr	r3, [pc, #96]
	adds	r0, r4, r3
	ldr	r3, [r2, #0]
	str	r3, [r0, #0]
	b.n	.L4
.L3:
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r6, #1
	adds	r4, r4, r2
	adds	r0, #8
	cmp	r6, #2
	ble.n	.L5
.L4:
	adds	r1, #1
	cmp	r1, lr
	bcc.n	.L6
.L2:
	ldr	r3, [sp, #8]
	ldr	r4, [sp, #4]
	ldr	r5, [sp, #0]
	add	r3, r8
	add	r4, r8
	adds	r7, #1
	str	r3, [sp, #8]
	str	r4, [sp, #4]
	cmp	r7, r5
	bcc.n	.L7
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
	.4byte 0xfffff000
	.4byte 0x06002800
	.4byte 0x02020000
	.4byte 0x02020004
	.4byte 0x06002840
