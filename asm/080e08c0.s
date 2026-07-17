@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080e08c0
	.thumb_func
Func_080e08c0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #868]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #32
	str	r3, [sp, #20]
	ldr	r3, [pc, #860]
	mov	sl, r1
	ldr	r2, [r2, #8]
	add	r3, sl
	str	r2, [sp, #16]
	str	r0, [r3, #0]
	movs	r0, #0
	bl	Func_080cd594
	mov	r2, sp
	adds	r2, #24
	adds	r1, r2, #0
	movs	r0, #0
	str	r2, [sp, #12]
	bl	Func_080cef64
	ldr	r0, [pc, #832]
	ldr	r1, [sp, #16]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080e0524
	ldr	r0, [pc, #824]
	mov	r1, sl
	movs	r2, #1
	movs	r3, #0
	bl	Func_080e0524
	movs	r1, #200
	lsls	r1, r1, #2
	ldr	r0, [pc, #812]
	add	r1, sl
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #792]
	movs	r3, #75
	add	r2, sl
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #788]
	lsls	r1, r1, #3
	bl	Func_080041d8
	movs	r6, #225
	movs	r3, #0
	lsls	r6, r6, #7
	str	r3, [sp, #8]
	movs	r7, #0
	add	r6, sl
.L3:
	lsls	r5, r7, #11
	adds	r0, r5, #0
	bl	Func_08002322
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #3
	asrs	r3, r3, #16
	str	r3, [r6, #0]
	adds	r0, r5, #0
	bl	Func_0800231c
	lsls	r0, r0, #2
	asrs	r0, r0, #16
	movs	r3, #1
	adds	r0, #52
	ands	r3, r7
	str	r0, [r6, #4]
	cmp	r3, #0
	beq.n	.L0
	ldr	r2, [r6, #0]
	movs	r3, #32
	subs	r3, r3, r2
	b.n	.L1
.L0:
	ldr	r3, [r6, #0]
	adds	r3, #32
.L1:
	str	r3, [r6, #0]
	lsls	r3, r7, #1
	negs	r3, r3
	str	r3, [r6, #24]
	ldr	r1, [pc, #712]
	ldr	r2, [sp, #8]
	ldr	r3, [pc, #712]
	movs	r4, #0
	movs	r0, #127
	mov	r8, r4
	mov	fp, r0
	mov	r9, r1
	adds	r5, r2, r3
.L2:
	bl	Func_08004458
	ldr	r2, [r6, #0]
	movs	r3, #15
	ands	r3, r0
	adds	r3, r3, r2
	subs	r3, #8
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	bl	Func_08004458
	movs	r3, #7
	ands	r3, r0
	adds	r3, #96
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	Func_08004458
	mov	r4, fp
	ands	r0, r4
	subs	r0, #64
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	Func_08004458
	mov	r1, fp
	ands	r0, r1
	subs	r0, #64
	lsls	r0, r0, #10
	str	r0, [r5, #16]
	bl	Func_08004458
	mov	r2, r9
	ands	r0, r2
	str	r0, [r5, #8]
	bl	Func_08004458
	mov	r3, r9
	movs	r4, #1
	ands	r0, r3
	add	r8, r4
	str	r0, [r5, #20]
	mov	r0, r8
	adds	r5, #28
	cmp	r0, #16
	bne.n	.L2
	ldr	r1, [sp, #8]
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r1, r1, r2
	adds	r7, #1
	adds	r6, #28
	str	r1, [sp, #8]
	cmp	r7, #9
	bne.n	.L3
	movs	r0, #136
	bl	Func_080f9010
	movs	r4, #172
	movs	r3, #0
	negs	r4, r4
	mov	r8, r3
	mov	r9, r4
.L21:
	mov	r0, r8
	cmp	r0, #56
	bne.n	.L4
	movs	r0, #133
	bl	Func_080b50e8
.L4:
	mov	r1, r8
	cmp	r1, #23
	bgt.n	.L5
	mov	r3, r8
	cmp	r1, #0
	bge.n	.L6
	adds	r3, #3
.L6:
	asrs	r3, r3, #2
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #3
	adds	r1, r1, r3
	lsls	r1, r1, #6
	movs	r2, #200
	movs	r3, #40
	lsls	r2, r2, #2
	add	r1, sl
	adds	r1, r1, r2
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	ldr	r4, [sp, #24]
	ldr	r0, [sp, #20]
	movs	r2, #40
	movs	r3, #20
	bl	Func_080072f4
.L5:
	mov	r3, r8
	cmp	r3, #20
	bne.n	.L7
	ldr	r0, [pc, #480]
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #496]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	Func_080072f0
.L7:
	mov	r3, r8
	subs	r3, #20
	cmp	r3, #11
	bhi.n	.L8
	mov	r4, r8
	cmp	r4, #23
	ble.n	.L9
	lsls	r3, r4, #2
	movs	r2, #146
	subs	r2, r2, r3
	movs	r3, #20
	str	r3, [sp, #0]
	movs	r3, #40
	str	r3, [sp, #4]
	ldr	r0, [sp, #12]
	mov	r1, sl
	ldr	r4, [r0, #4]
	mov	r3, r9
	ldr	r0, [sp, #20]
	bl	Func_080072f4
	b.n	.L8
.L9:
	movs	r3, #20
	str	r3, [sp, #0]
	movs	r3, #40
	str	r3, [sp, #4]
	ldr	r4, [sp, #24]
	ldr	r0, [sp, #20]
	mov	r1, sl
	movs	r2, #50
	movs	r3, #20
	bl	Func_080072f4
.L8:
	mov	r1, r8
	cmp	r1, #32
	bne.n	.L10
	movs	r0, #145
	bl	Func_080f9010
	ldr	r2, [pc, #412]
	movs	r3, #8
	add	r2, sl
	str	r3, [r2, #0]
	ldr	r0, [pc, #408]
	mov	r1, sl
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
.L10:
	mov	r2, r8
	cmp	r2, #31
	ble.n	.L11
	movs	r6, #225
	lsls	r6, r6, #7
	movs	r7, #0
	add	r6, sl
.L14:
	ldr	r3, [r6, #24]
	cmp	r3, #47
	bhi.n	.L12
	adds	r5, r3, #0
	cmp	r3, #0
	bge.n	.L13
	adds	r5, r3, #7
.L13:
	asrs	r5, r5, #3
	ldr	r2, [pc, #368]
	lsls	r3, r5, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #368]
	ldrb	r4, [r3, r5]
	ldr	r2, [r6, #0]
	lsrs	r3, r4, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #360]
	ldrb	r0, [r3, r5]
	ldr	r3, [r6, #4]
	str	r4, [sp, #0]
	adds	r3, r3, r0
	ldr	r0, [pc, #356]
	ldrb	r0, [r0, r5]
	add	r1, sl
	str	r0, [sp, #4]
	ldr	r4, [sp, #24]
	ldr	r0, [sp, #20]
	bl	Func_080072f4
	ldr	r3, [r6, #24]
.L12:
	adds	r3, #1
	adds	r7, #1
	str	r3, [r6, #24]
	adds	r6, #28
	cmp	r7, #9
	bne.n	.L14
.L11:
	ldr	r6, [pc, #296]
	movs	r7, #0
.L17:
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L15
	adds	r3, #15
.L15:
	asrs	r3, r3, #4
	lsls	r3, r3, #1
	adds	r3, #40
	cmp	r8, r3
	blt.n	.L16
	ldr	r0, [r6, #8]
	bl	Func_08002322
	movs	r5, #1
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	ands	r5, r7
	lsls	r0, r0, #2
	adds	r5, #3
	asrs	r0, r0, #16
	adds	r2, r2, r0
	ldr	r1, [pc, #284]
	lsls	r0, r5, #1
	subs	r3, r0, #2
	ldrh	r1, [r1, r3]
	ldr	r4, [sp, #16]
	lsrs	r3, r5, #1
	adds	r1, r4, r1
	subs	r2, r2, r3
	movs	r4, #6
	ldrsh	r3, [r6, r4]
	str	r0, [sp, #4]
	str	r5, [sp, #0]
	ldr	r0, [sp, #12]
	subs	r3, r3, r5
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #20]
	bl	Func_080072f4
	movs	r1, #64
	ldr	r2, [pc, #252]
	adds	r0, r6, #0
	bl	Func_080e3908
	ldr	r2, [r6, #8]
	movs	r1, #128
	lsls	r1, r1, #4
	ldr	r4, [pc, #196]
	adds	r3, r2, r1
	str	r3, [r6, #8]
	cmp	r3, r4
	ble.n	.L16
	ldr	r0, [pc, #232]
	adds	r3, r2, r0
	str	r3, [r6, #8]
.L16:
	adds	r7, #1
	adds	r6, #28
	cmp	r7, #144
	bne.n	.L17
	mov	r1, r8
	cmp	r1, #38
	bne.n	.L18
	ldr	r3, [pc, #144]
	mov	r2, sl
	ldr	r3, [r2, r3]
	ldr	r3, [r3, #20]
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L18
	ldr	r5, [pc, #128]
	movs	r6, #36
	add	r5, sl
.L19:
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r1, #7
	adds	r3, r7, #0
	movs	r2, #5
	bl	Func_080d6888
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r1, #6
	bl	Func_080b5088
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	adds	r7, #1
	adds	r6, #2
	cmp	r7, r3
	bne.n	.L19
.L18:
	movs	r0, #8
	movs	r1, #8
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r2, [pc, #144]
	movs	r3, #1
	add	r2, sl
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r3, #1
	add	r8, r3
	movs	r2, #8
	mov	r4, r8
	add	r9, r2
	cmp	r4, #112
	beq.n	.L20
	b.n	.L21
.L20:
	ldr	r0, [pc, #60]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x00000073
	.4byte 0x0000008e
	.4byte 0x000000b7
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x0000ffff
	.4byte 0x02010000
	.4byte 0x03001388
	.4byte 0x000077a8
	.4byte 0x000000b4
	.4byte 0x080edeb2
	.4byte 0x080ede9f
	.4byte 0x080edeab
	.4byte 0x080edea5
	.4byte 0x080ede48
	.4byte 0xffffe000
	.4byte 0xffff0801
	.4byte 0x00007824
