@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801c49c
	.thumb_func
Func_0801c49c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	movs	r0, #0
	str	r0, [sp, #4]
	mov	r8, r0
	movs	r0, #224
	movs	r1, #1
	lsls	r0, r0, #3
	mov	fp, r1
	bl	Func_08004938
	movs	r1, #140
	adds	r7, r0, #0
	movs	r0, #0
	bl	Func_08077088
	movs	r1, #140
	movs	r0, #1
	bl	Func_08077088
	movs	r1, #140
	movs	r0, #2
	bl	Func_08077088
	movs	r1, #141
	movs	r0, #2
	bl	Func_08077088
	movs	r1, #78
	movs	r0, #2
	bl	Func_08077088
	movs	r1, #93
	movs	r0, #3
	bl	Func_08077088
	movs	r1, #140
	movs	r0, #5
	bl	Func_08077088
	movs	r3, #0
	adds	r0, r7, #0
	str	r3, [sp, #12]
	str	r3, [sp, #16]
	bl	Func_0801c7fc
	mov	sl, r0
	cmp	r0, #0
	bne.n	.L0
	b.n	.L1
.L0:
	add	r0, sp, #16
	add	r1, sp, #12
	adds	r2, r7, #0
	bl	Func_0801c8a0
	movs	r5, #2
	movs	r1, #6
	movs	r2, #20
	movs	r3, #7
	movs	r0, #4
	str	r5, [sp, #0]
	bl	Func_080162d4
	movs	r1, #3
	movs	r2, #20
	movs	r3, #3
	adds	r6, r0, #0
	movs	r0, #4
	str	r5, [sp, #0]
	bl	Func_080162d4
	movs	r1, #14
	str	r0, [sp, #8]
	movs	r2, #20
	movs	r3, #5
	movs	r0, #4
	str	r5, [sp, #0]
	bl	Func_080162d4
	mov	r9, r0
	bl	Func_08004080
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L2
	ldr	r2, [pc, #636]
	movs	r1, #128
	bl	Func_08003fa4
	movs	r0, #0
	movs	r1, #128
	str	r0, [sp, #0]
	lsls	r1, r1, #23
	adds	r0, r5, #0
	adds	r2, r6, #0
	movs	r3, #0
	bl	Func_0801eadc
	str	r0, [sp, #4]
.L2:
	ldr	r5, [pc, #612]
	ldr	r1, [sp, #8]
	adds	r0, r5, #0
	movs	r2, #16
	movs	r3, #0
	bl	Func_0801e7c0
.L19:
	mov	r1, fp
	cmp	r1, #0
	bne.n	.L3
	b.n	.L4
.L3:
	ldr	r0, [sp, #16]
	movs	r3, #0
	mov	r1, sl
	add	r0, sl
	mov	fp, r3
	bl	Func_08002304
	str	r0, [sp, #16]
	ldr	r0, [sp, #12]
	mov	r1, sl
	add	r0, sl
	bl	Func_08002304
	mov	r2, r8
	adds	r2, #2
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	lsls	r3, r3, #1
	subs	r2, r2, r3
	ldrh	r3, [r6, #14]
	mov	r8, r2
	lsls	r3, r3, #3
	lsls	r2, r2, #4
	str	r0, [sp, #12]
	adds	r2, r2, r3
	ldr	r0, [sp, #4]
	adds	r2, #28
	strh	r2, [r0, #8]
	strb	r2, [r0, #20]
	adds	r0, r6, #0
	bl	Func_08016498
	movs	r3, #2
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #1
	movs	r2, #2
	movs	r3, #17
	bl	Func_0801e41c
	ldr	r0, [pc, #512]
	adds	r1, r6, #0
	movs	r2, #48
	movs	r3, #0
	bl	Func_0801e7c0
	ldr	r3, [sp, #16]
	lsls	r3, r3, #2
	adds	r3, r3, r7
	ldr	r5, [pc, #496]
	ldrh	r0, [r3, #2]
	adds	r1, r6, #0
	adds	r0, r0, r5
	movs	r2, #56
	movs	r3, #16
	bl	Func_0801e7c0
	ldr	r3, [sp, #12]
	lsls	r3, r3, #2
	adds	r3, r3, r7
	ldrh	r0, [r3, #2]
	adds	r1, r6, #0
	adds	r0, r0, r5
	movs	r2, #56
	movs	r3, #32
	bl	Func_0801e7c0
	ldr	r0, [pc, #456]
	adds	r1, r6, #0
	subs	r0, #2
	movs	r2, #16
	movs	r3, #16
	bl	Func_0801e7c0
	ldr	r0, [pc, #444]
	adds	r1, r6, #0
	subs	r0, #1
	movs	r2, #16
	movs	r3, #32
	bl	Func_0801e7c0
	ldr	r3, [sp, #16]
	lsls	r3, r3, #2
	ldrh	r0, [r3, r7]
	ldr	r5, [pc, #432]
	adds	r1, r6, #0
	adds	r0, r0, r5
	movs	r2, #104
	movs	r3, #16
	bl	Func_0801e7c0
	ldr	r3, [sp, #12]
	lsls	r3, r3, #2
	ldrh	r0, [r3, r7]
	adds	r1, r6, #0
	movs	r2, #104
	movs	r3, #32
	adds	r0, r0, r5
	bl	Func_0801e7c0
	mov	r0, r9
	bl	Func_08016498
	mov	r1, r9
	ldr	r0, [pc, #396]
	movs	r2, #0
	movs	r3, #16
	bl	Func_0801e7c0
	mov	r1, r8
	cmp	r1, #0
	beq.n	.L5
	ldr	r3, [sp, #12]
	lsls	r3, r3, #2
	adds	r3, r3, r7
	ldrh	r0, [r3, #2]
	bl	Func_08077080
	ldr	r3, [sp, #12]
	b.n	.L6
.L5:
	ldr	r3, [sp, #16]
	lsls	r3, r3, #2
	adds	r3, r3, r7
	ldrh	r0, [r3, #2]
	bl	Func_08077080
	ldr	r3, [sp, #16]
.L6:
	lsls	r3, r3, #2
	adds	r3, r3, r7
	ldrb	r0, [r0, #9]
	ldrh	r5, [r3, #2]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r1, #2
	mov	r2, r9
	movs	r3, #64
	bl	Func_0801e9d4
	ldr	r0, [pc, #332]
	mov	r1, r9
	adds	r0, r5, r0
	movs	r2, #0
	movs	r3, #0
	bl	Func_0801e7c0
.L4:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r5, [pc, #316]
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L7
	movs	r0, #111
	bl	Func_080f9010
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L8
	ldr	r3, [sp, #12]
	subs	r3, #1
	str	r3, [sp, #12]
	b.n	.L9
.L8:
	ldr	r3, [sp, #16]
	subs	r3, #1
	str	r3, [sp, #16]
.L9:
	movs	r0, #1
	ldr	r5, [pc, #276]
	mov	fp, r0
.L7:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L10
	movs	r0, #111
	bl	Func_080f9010
	mov	r1, r8
	cmp	r1, #0
	beq.n	.L11
	ldr	r3, [sp, #12]
	adds	r3, #1
	str	r3, [sp, #12]
	b.n	.L12
.L11:
	ldr	r3, [sp, #16]
	adds	r3, #1
	str	r3, [sp, #16]
.L12:
	movs	r3, #1
	ldr	r5, [pc, #232]
	mov	fp, r3
.L10:
	ldr	r3, [r5, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L13
	movs	r0, #111
	bl	Func_080f9010
	movs	r0, #1
	negs	r0, r0
	movs	r1, #1
	add	r8, r0
	mov	fp, r1
.L13:
	ldr	r3, [r5, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L14
	movs	r0, #111
	bl	Func_080f9010
	movs	r3, #1
	add	r8, r3
	mov	fp, r3
.L14:
	ldr	r1, [pc, #184]
	ldr	r3, [r1, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L15
	movs	r0, #112
	bl	Func_080f9010
	b.n	.L16
.L15:
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L17
	movs	r0, #113
	bl	Func_080f9010
	b.n	.L16
.L17:
	ldr	r3, [r1, #0]
	movs	r2, #8
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L18
	b.n	.L19
.L18:
	movs	r0, #113
	bl	Func_080f9010
.L16:
	ldr	r3, [sp, #16]
	lsls	r3, r3, #2
	adds	r3, r3, r7
	ldrh	r2, [r3, #0]
	ldr	r1, [pc, #124]
	ldrh	r3, [r3, #2]
	movs	r0, #136
	lsls	r0, r0, #2
	lsls	r2, r2, #10
	orrs	r2, r3
	adds	r3, r1, r0
	strh	r2, [r3, #0]
	ldr	r3, [sp, #12]
	lsls	r3, r3, #2
	adds	r3, r3, r7
	ldrh	r2, [r3, #0]
	ldrh	r3, [r3, #2]
	lsls	r2, r2, #10
	orrs	r2, r3
	ldr	r3, [pc, #100]
	adds	r1, r1, r3
	strh	r2, [r1, #0]
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_08016418
	movs	r1, #1
	ldr	r0, [sp, #8]
	bl	Func_08016418
	mov	r0, r9
	movs	r1, #1
	bl	Func_08016418
	movs	r0, #1
	bl	Func_080030f8
.L1:
	adds	r0, r7, #0
	bl	Func_08002df0
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x080310a4
	.4byte 0x00000b19
	.4byte 0x00000b1e
	.4byte 0x00000333
	.4byte 0x00000066
	.4byte 0x00000aec
	.4byte 0x0000053a
	.4byte 0x03001b04
	.4byte 0x03001c94
	.4byte 0x02000240
	.4byte 0x00000222
