@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080e0c84
	.thumb_func
Func_080e0c84:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #344]
	adds	r6, r0, #0
	ldmia	r3!, {r0}
	ldr	r5, [pc, #340]
	mov	fp, r0
	ldr	r3, [r3, #0]
	sub	sp, #48
	add	r5, fp
	str	r3, [sp, #16]
	movs	r0, #0
	str	r6, [r5, #0]
	bl	Func_080cd594
	ldr	r3, [r5, #0]
	ldr	r2, [r3, #4]
	add	r3, sp, #32
	str	r3, [sp, #0]
	add	r3, sp, #28
	str	r3, [sp, #4]
	adds	r0, r6, #0
	movs	r1, #1
	movs	r3, #2
	bl	Func_080de2f8
	ldr	r3, [r5, #0]
	mov	r1, sp
	ldr	r0, [r3, #4]
	adds	r1, #20
	str	r1, [sp, #12]
	bl	Func_080cef64
	ldr	r0, [pc, #288]
	mov	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #272]
	movs	r3, #75
	add	r2, fp
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #264]
	bl	Func_080041d8
	ldr	r3, [r5, #0]
	add	r5, sp, #36
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	adds	r1, r5, #0
	bl	Func_080e3980
	ldr	r4, [pc, #248]
	movs	r3, #15
	movs	r0, #127
	movs	r7, #0
	mov	r9, r3
	mov	r8, r4
	mov	sl, r0
.L0:
	bl	Func_08004458
	ldr	r6, [pc, #232]
	movs	r1, #128
	lsls	r1, r1, #7
	ands	r6, r0
	adds	r6, r6, r1
	bl	Func_08004458
	ldr	r5, [pc, #224]
	ands	r5, r0
	bl	Func_08004458
	ldr	r3, [sp, #36]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	mov	r2, r9
	ands	r0, r2
	asrs	r3, r3, #1
	adds	r3, r3, r0
	subs	r3, #8
	mov	r4, r8
	lsls	r3, r3, #16
	str	r3, [r4, #0]
	ldr	r3, [sp, #40]
	adds	r3, #8
	lsls	r3, r3, #16
	str	r3, [r4, #4]
	adds	r0, r6, #0
	bl	Func_08002322
	adds	r5, #128
	adds	r3, r5, #0
	muls	r3, r0
	mov	r0, r8
	asrs	r3, r3, #9
	str	r3, [r0, #12]
	adds	r0, r6, #0
	bl	Func_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	mov	r1, r8
	asrs	r3, r3, #6
	str	r3, [r1, #16]
	bl	Func_08004458
	mov	r2, sl
	mov	r3, r8
	ands	r0, r2
	str	r0, [r3, #8]
	bl	Func_08004458
	mov	r4, sl
	ands	r0, r4
	mov	r1, r8
	str	r0, [r1, #20]
	bl	Func_08004458
	mov	r2, r9
	ands	r0, r2
	mov	r3, r8
	adds	r0, #32
	movs	r4, #28
	adds	r7, #1
	str	r0, [r3, #24]
	add	r8, r4
	cmp	r7, #64
	bne.n	.L0
	ldr	r1, [pc, #80]
	add	r1, fp
	movs	r0, #0
	str	r1, [sp, #8]
	mov	sl, r0
.L18:
	mov	r2, sl
	cmp	r2, #47
	ble.n	.L1
	ldr	r2, [pc, #52]
	mov	r4, sl
	ldr	r1, [pc, #52]
	ldr	r3, [pc, #84]
	subs	r2, r2, r4
	orrs	r2, r1
	strh	r2, [r3, #0]
.L1:
	mov	r0, sl
	cmp	r0, #1
	bne.n	.L2
	movs	r1, #128
	lsls	r1, r1, #3
	ldr	r0, [pc, #72]
	add	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	ldr	r1, [pc, #64]
	ldr	r0, [pc, #64]
	add	r1, fp
	movs	r2, #1
	movs	r3, #0
	bl	Func_080e0524
	b.n	.L2
	movs	r0, r0
	.4byte 0x00000040
	.4byte 0x00001000
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x0000006e
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x02010000
	.4byte 0x00007fff
	.4byte 0x000001ff
	.4byte 0x04000052
	.4byte 0x000000b8
	.4byte 0x000065c0
	.4byte 0x00000092
.L2:
	ldr	r1, [sp, #8]
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #28]
	cmp	r3, #1
	bne.n	.L3
	mov	r2, sl
	lsls	r5, r2, #11
	adds	r0, r5, #0
	bl	Func_08002322
	ldr	r3, [sp, #32]
	negs	r0, r0
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	lsls	r0, r0, #2
	asrs	r3, r3, #1
	asrs	r0, r0, #16
	adds	r0, r0, r3
	subs	r0, #10
	mov	r9, r0
	adds	r0, r5, #0
	bl	Func_0800231c
	ldr	r3, [sp, #28]
	lsls	r0, r0, #1
	asrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r5, r0, #0
	mov	r3, sl
	subs	r5, #22
	cmp	r3, #69
	ble.n	.L4
	lsls	r3, r3, #1
	subs	r3, r5, r3
	adds	r5, r3, #0
	adds	r5, #138
.L4:
	movs	r4, #20
	ldr	r6, [pc, #440]
	movs	r7, #40
	ldr	r0, [sp, #12]
	str	r4, [sp, #0]
	str	r7, [sp, #4]
	add	r6, fp
	adds	r1, r6, #0
	mov	r8, r4
	mov	r2, r9
	ldr	r4, [r0, #4]
	adds	r3, r5, #0
	ldr	r0, [sp, #16]
	bl	Func_080072f4
	mov	r1, sl
	cmp	r1, #3
	bgt.n	.L3
	mov	r2, r8
	str	r2, [sp, #0]
	ldr	r3, [sp, #12]
	str	r7, [sp, #4]
	ldr	r0, [sp, #16]
	ldr	r4, [r3, #4]
	adds	r1, r6, #0
	mov	r2, r9
	adds	r3, r5, #0
	bl	Func_080072f4
.L3:
	ldr	r4, [pc, #388]
	movs	r7, #0
	mov	r8, r4
.L8:
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L5
	adds	r3, r7, #3
.L5:
	asrs	r3, r3, #2
	adds	r3, #4
	cmp	sl, r3
	blt.n	.L6
	mov	r0, r8
	ldr	r4, [r0, #8]
	cmp	r4, #0
	bge.n	.L7
	adds	r4, #127
.L7:
	movs	r3, #3
	asrs	r4, r4, #7
	ands	r4, r3
	ldr	r2, [pc, #352]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	movs	r2, #128
	lsls	r2, r2, #3
	mov	r0, r8
	add	r1, fp
	adds	r1, r1, r2
	movs	r3, #2
	ldrsh	r2, [r0, r3]
	ldr	r3, [pc, #336]
	ldrb	r6, [r3, r4]
	lsrs	r3, r6, #1
	subs	r2, r2, r3
	movs	r3, #6
	ldrsh	r0, [r0, r3]
	mov	ip, r0
	ldr	r0, [pc, #328]
	ldrb	r4, [r0, r4]
	mov	r3, ip
	lsrs	r0, r4, #1
	movs	r5, #1
	subs	r3, r3, r0
	str	r6, [sp, #0]
	ldr	r0, [sp, #12]
	str	r4, [sp, #4]
	ands	r5, r7
	lsls	r5, r5, #2
	ldr	r4, [r5, r0]
	ldr	r0, [sp, #16]
	bl	Func_080072f4
	movs	r2, #128
	mov	r0, r8
	movs	r1, #63
	lsls	r2, r2, #5
	bl	Func_080e38b8
.L6:
	movs	r1, #28
	adds	r7, #1
	add	r8, r1
	cmp	r7, #64
	bne.n	.L8
	mov	r2, sl
	cmp	r2, #8
	bne.n	.L9
	ldr	r3, [pc, #272]
	add	r3, fp
	str	r2, [r3, #0]
	movs	r0, #134
	bl	Func_080b50e8
	ldr	r4, [sp, #8]
	ldr	r3, [r4, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	Func_080d6888
	ldr	r2, [sp, #8]
	ldr	r3, [r2, #0]
	movs	r1, #3
	movs	r4, #36
	ldrsh	r0, [r3, r4]
	bl	Func_080b5088
.L9:
	mov	r0, sl
	lsls	r5, r0, #2
	cmp	r5, #32
	ble.n	.L10
	movs	r5, #32
.L10:
	ldr	r1, [sp, #8]
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L11
	movs	r2, #120
	movs	r7, #0
	movs	r6, #32
	mov	r8, r2
.L13:
	mov	r2, sl
	lsls	r1, r7, #5
	cmp	r2, #0
	bge.n	.L12
	adds	r2, #3
.L12:
	movs	r3, #31
	asrs	r2, r2, #2
	ands	r2, r3
	ldr	r3, [sp, #20]
	mov	r4, r8
	subs	r2, r1, r2
	mov	ip, r3
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	ldr	r0, [sp, #16]
	mov	r1, fp
	subs	r3, r4, r5
	adds	r7, #1
	bl	Func_08007314
	cmp	r7, #5
	bne.n	.L13
	b.n	.L14
.L11:
	movs	r0, #120
	movs	r7, #0
	movs	r6, #32
	mov	r8, r0
.L16:
	mov	r2, sl
	lsls	r1, r7, #5
	cmp	r2, #0
	bge.n	.L15
	adds	r2, #3
.L15:
	movs	r3, #31
	asrs	r2, r2, #2
	ands	r2, r3
	adds	r2, r1, r2
	ldr	r1, [sp, #20]
	mov	r4, r8
	mov	ip, r1
	subs	r2, #32
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	ldr	r0, [sp, #16]
	mov	r1, fp
	subs	r3, r4, r5
	adds	r7, #1
	bl	Func_08007314
	cmp	r7, #5
	bne.n	.L16
.L14:
	movs	r1, #8
	movs	r0, #4
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r2, [pc, #88]
	movs	r3, #1
	add	r2, fp
	movs	r0, #1
	str	r3, [r2, #0]
	bl	Func_080030f8
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	cmp	r1, #64
	beq.n	.L17
	b.n	.L18
.L17:
	ldr	r0, [pc, #68]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x000065c0
	.4byte 0x02010000
	.4byte 0x080eec68
	.4byte 0x080eec5f
	.4byte 0x080eec63
	.4byte 0x000077a8
	.4byte 0x00007824
	.4byte 0x080cd261
