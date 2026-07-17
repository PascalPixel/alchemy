@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080c1470
	.thumb_func
Func_080c1470:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #36
	str	r0, [sp, #4]
	ldr	r3, [pc, #520]
	ldr	r2, [r3, #0]
	movs	r3, #1
	str	r3, [r2, #8]
	ldr	r1, [pc, #516]
	movs	r0, #39
	bl	Func_080048f4
	movs	r1, #128
	mov	fp, r0
	lsls	r1, r1, #7
	movs	r0, #40
	bl	Func_080048b0
	ldr	r1, [pc, #500]
	movs	r7, #142
	lsls	r7, r7, #5
	movs	r2, #15
	mov	sl, r1
	add	r7, fp
	mov	r9, r2
.L2:
	bl	Func_08004458
	adds	r5, r0, #0
	bl	Func_08004458
	movs	r3, #128
	lsls	r3, r3, #9
	adds	r3, r3, r0
	lsrs	r6, r3, #1
	adds	r0, r5, #0
	mov	r8, r3
	bl	Func_0800231c
	adds	r1, r6, #0
	mov	ip, pc
	bx	sl
	str	r0, [r7, #0]
	adds	r0, r5, #0
	bl	Func_08002322
	adds	r1, r6, #0
	movs	r0, r0
	mov	ip, pc
	bx	sl
	ldr	r2, [r7, #0]
	movs	r1, #1
	adds	r3, r2, #0
	ands	r3, r1
	str	r0, [r7, #4]
	cmp	r3, #0
	beq.n	.L0
	negs	r3, r2
	str	r3, [r7, #0]
.L0:
	ldr	r2, [r7, #4]
	movs	r1, #1
	adds	r3, r2, #0
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L1
	negs	r3, r2
	str	r3, [r7, #4]
.L1:
	bl	Func_08004458
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r0, r0, r2
	lsrs	r0, r0, #2
	ldr	r3, [r7, #4]
	str	r0, [r7, #8]
	ldr	r0, [r7, #0]
	asrs	r2, r3, #8
	negs	r0, r0
	negs	r3, r3
	asrs	r1, r0, #7
	asrs	r3, r3, #7
	asrs	r0, r0, #8
	adds	r1, r1, r2
	adds	r3, r3, r0
	str	r1, [r7, #12]
	str	r3, [r7, #16]
	mov	r1, r8
	movs	r3, #0
	movs	r2, #1
	str	r3, [r7, #20]
	negs	r2, r2
	lsrs	r3, r1, #13
	adds	r3, #1
	add	r9, r2
	str	r3, [r7, #24]
	mov	r3, r9
	adds	r7, #28
	cmp	r3, #0
	bge.n	.L2
	movs	r1, #128
	movs	r5, #156
	lsls	r1, r1, #5
	lsls	r5, r5, #5
	movs	r2, #2
	ldr	r6, [pc, #328]
	mov	r8, r1
	movs	r7, #0
	add	r5, fp
	mov	r9, r2
.L3:
	adds	r0, r7, #0
	bl	Func_0800231c
	mov	r1, r8
	movs	r0, r0
	mov	ip, pc
	bx	r6
	str	r0, [r5, #0]
	adds	r0, r7, #0
	bl	Func_08002322
	mov	r1, r8
	movs	r0, r0
	mov	ip, pc
	bx	r6
	str	r0, [r5, #4]
	adds	r0, r7, #0
	bl	Func_0800231c
	movs	r1, #128
	lsls	r1, r1, #2
	mov	ip, pc
	bx	r6
	str	r0, [r5, #8]
	adds	r0, r7, #0
	bl	Func_08002322
	movs	r1, #128
	lsls	r1, r1, #2
	mov	ip, pc
	bx	r6
	movs	r1, #1
	ldr	r3, [pc, #260]
	negs	r1, r1
	add	r9, r1
	movs	r2, #0
	adds	r7, r7, r3
	mov	r3, r9
	str	r0, [r5, #12]
	str	r2, [r5, #16]
	adds	r5, #20
	cmp	r3, #0
	bge.n	.L3
	ldr	r3, [pc, #240]
	add	r3, fp
	str	r2, [r3, #0]
	movs	r3, #158
	lsls	r3, r3, #5
	add	r3, fp
	str	r2, [r3, #0]
	ldr	r3, [pc, #232]
	add	r3, fp
	str	r2, [r3, #0]
	ldr	r3, [pc, #228]
	movs	r1, #128
	adds	r3, #160
	lsls	r1, r1, #7
	ldr	r0, [r3, #0]
	ldr	r3, [pc, #224]
	bl	Func_080072f0
	ldr	r6, [pc, #220]
	adds	r0, r6, #0
	bl	Func_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	adds	r1, r5, #0
	ldr	r3, [pc, #212]
	movs	r2, #128
	lsls	r0, r0, #19
	adds	r5, #128
	bl	Func_080072f0
	mov	r1, fp
	adds	r0, r5, #0
	bl	Func_08005340
	ldr	r1, [sp, #4]
	cmp	r1, #1
	beq.n	.L4
	cmp	r1, #1
	bgt.n	.L5
	cmp	r1, #0
	beq.n	.L6
	b.n	.L7
.L5:
	ldr	r2, [sp, #4]
	cmp	r2, #2
	beq.n	.L8
	b.n	.L7
.L6:
	ldr	r0, [pc, #172]
	b.n	.L9
.L4:
	adds	r0, r6, #0
	b.n	.L9
.L8:
	ldr	r0, [pc, #168]
	b.n	.L9
.L7:
	ldr	r0, [pc, #168]
.L9:
	bl	Func_08002f40
	adds	r5, r0, #0
	movs	r1, #160
	ldr	r3, [pc, #160]
	adds	r0, r5, #0
	lsls	r1, r1, #19
	ldr	r2, [pc, #160]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #0
	subs	r3, #172
	str	r2, [r3, #0]
	adds	r3, #4
	str	r2, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #1
	subs	r3, #12
	strh	r1, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r1, [r3, #0]
	movs	r3, #3
	movs	r1, #7
	movs	r2, #7
	movs	r0, #46
	str	r3, [sp, #0]
	bl	Func_080c9000
	movs	r5, #200
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r2, #7
	movs	r3, #3
	lsls	r5, r5, #4
	movs	r1, #7
	movs	r0, #47
	bl	Func_080c9000
	adds	r1, r5, #0
	ldr	r0, [pc, #92]
	bl	Func_080041d8
	adds	r1, r5, #0
	ldr	r0, [pc, #88]
	bl	Func_080041d8
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f00
	.4byte 0x000013d0
	.4byte 0x03000118
	.4byte 0x00005555
	.4byte 0x000013bc
	.4byte 0x000013cc
	.4byte 0x03001e50
	.4byte 0x03000164
	.4byte 0x000000c9
	.4byte 0x03001388
	.4byte 0x000000c8
	.4byte 0x000000ca
	.4byte 0x000000cb
	.4byte 0x040000d4
	.4byte 0x84000020
	.4byte 0x080c11ed
	.4byte 0x080c1439
