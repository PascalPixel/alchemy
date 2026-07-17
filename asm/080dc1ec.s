@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080dc1ec
	.thumb_func
Func_080dc1ec:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #560]
	ldmia	r3!, {r2}
	ldr	r5, [pc, #560]
	mov	fp, r2
	ldr	r3, [r3, #0]
	sub	sp, #52
	add	r5, fp
	str	r3, [sp, #16]
	str	r0, [r5, #0]
	movs	r0, #0
	bl	Func_080cd594
	ldr	r0, [pc, #544]
	mov	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #4]
	movs	r3, #1
	eors	r0, r3
	add	r1, sp, #20
	bl	Func_080cef64
	ldr	r5, [pc, #520]
	movs	r3, #255
	movs	r7, #0
	mov	r8, r3
	movs	r6, #0
.L0:
	bl	Func_08004458
	mov	r2, r8
	ands	r0, r2
	subs	r0, #127
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	Func_08004458
	mov	r3, r8
	ands	r0, r3
	subs	r0, #127
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	Func_08004458
	mov	r2, r8
	ands	r0, r2
	subs	r0, #127
	movs	r3, #128
	lsls	r0, r0, #16
	adds	r7, #1
	lsls	r3, r3, #1
	str	r0, [r5, #8]
	str	r6, [r5, #12]
	str	r6, [r5, #16]
	str	r6, [r5, #20]
	str	r6, [r5, #24]
	adds	r5, #28
	cmp	r7, r3
	bne.n	.L0
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r3, [pc, #444]
	movs	r5, #0
	add	r3, fp
	movs	r1, #144
	str	r5, [r3, #0]
	ldr	r0, [pc, #436]
	lsls	r1, r1, #3
	bl	Func_080041d8
	movs	r3, #160
	mov	r2, sp
	adds	r2, #40
	lsls	r3, r3, #15
	str	r2, [sp, #12]
	str	r5, [r2, #0]
	str	r3, [r2, #4]
	str	r5, [r2, #8]
	mov	sl, r5
.L16:
	ldr	r3, [pc, #416]
	ldr	r5, [r3, #0]
	bl	Func_080049ac
	adds	r1, r5, #0
	adds	r1, #12
	adds	r0, r5, #0
	bl	Func_080051d8
	ldr	r0, [sp, #12]
	bl	Func_08004cb4
	mov	r3, sl
	negs	r3, r3
	mov	r2, sl
	str	r3, [sp, #8]
	lsls	r2, r2, #8
	lsls	r3, r3, #8
	ldr	r6, [pc, #368]
	movs	r7, #0
	mov	r8, r3
	mov	r9, r2
.L14:
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L1
	adds	r3, r7, #3
.L1:
	asrs	r3, r3, #2
	cmp	sl, r3
	ble.n	.L2
	ldr	r3, [r6, #24]
	cmp	r3, #0
	bne.n	.L2
	bl	Func_080049e8
	movs	r3, #3
	ands	r3, r7
	cmp	r3, #1
	beq.n	.L3
	cmp	r3, #1
	bgt.n	.L4
	cmp	r3, #0
	beq.n	.L5
	b.n	.L6
.L4:
	cmp	r3, #2
	beq.n	.L7
	cmp	r3, #3
	beq.n	.L8
	b.n	.L6
.L5:
	mov	r0, r9
	bl	Func_08004c1c
	b.n	.L6
.L3:
	mov	r0, r8
	bl	Func_08004bd4
	b.n	.L6
.L7:
	mov	r0, r8
	bl	Func_08004c6c
	b.n	.L6
.L8:
	mov	r0, r8
	bl	Func_08004bd4
	mov	r0, r8
	bl	Func_08004c6c
.L6:
	add	r5, sp, #28
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	Func_080e3944
	ldr	r3, [r5, #0]
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	bl	Func_08004a5c
	ldr	r2, [r5, #8]
	cmp	r2, #249
	bgt.n	.L9
	movs	r3, #250
	str	r3, [r5, #8]
	movs	r2, #250
.L9:
	ldr	r3, [pc, #256]
	cmp	r2, r3
	ble.n	.L10
	str	r3, [r5, #8]
	adds	r2, r3, #0
.L10:
	adds	r3, r2, #0
	subs	r3, #250
	cmp	r3, #0
	bge.n	.L11
	adds	r3, #63
.L11:
	asrs	r3, r3, #6
	movs	r0, #9
	subs	r0, r0, r3
	movs	r3, #3
	ands	r3, r7
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r4, r0, #1
	ldr	r2, [pc, #224]
	lsls	r1, r1, #7
	adds	r1, r1, r3
	subs	r3, r4, #2
	ldrh	r3, [r2, r3]
	lsls	r1, r1, #1
	adds	r1, r1, r3
	lsrs	r3, r0, #31
	ldr	r2, [r5, #0]
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	add	r1, fp
	subs	r3, r3, r0
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #16]
	ldr	r4, [sp, #20]
	bl	Func_080072f4
	adds	r0, r6, #0
	movs	r1, #60
	movs	r2, #0
	bl	Func_080e38b8
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L12
	adds	r3, r7, #3
.L12:
	asrs	r3, r3, #2
	adds	r3, #30
	cmp	sl, r3
	ble.n	.L2
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #12]
	negs	r2, r2
	asrs	r2, r2, #8
	ldr	r1, [r6, #4]
	adds	r3, r3, r2
	negs	r1, r1
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	asrs	r1, r1, #8
	ldr	r0, [r6, #8]
	adds	r3, r3, r1
	negs	r0, r0
	str	r3, [r6, #16]
	ldr	r3, [r6, #20]
	asrs	r0, r0, #8
	adds	r3, r3, r0
	str	r3, [r6, #20]
.L2:
	ldr	r2, [sp, #8]
	lsls	r3, r2, #3
	mov	r2, sl
	add	r8, r3
	adds	r7, #1
	lsls	r3, r2, #3
	add	r9, r3
	adds	r6, #28
	cmp	r7, #64
	beq.n	.L13
	b.n	.L14
.L13:
	ldr	r2, [pc, #100]
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r3, #1
	add	sl, r3
	mov	r2, sl
	cmp	r2, #160
	beq.n	.L15
	b.n	.L16
.L15:
	ldr	r0, [pc, #60]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x0000008c
	.4byte 0x02010000
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x03001e80
	.4byte 0x0000027a
	.4byte 0x080ede48
	.4byte 0x00007824
