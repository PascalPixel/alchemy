@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080e94b8
	.thumb_func
Func_080e94b8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #112]
	adds	r3, r2, #0
	adds	r5, r0, #0
	ldmia	r3!, {r0}
	ldr	r3, [r3, #0]
	sub	sp, #52
	str	r3, [sp, #40]
	ldr	r2, [r2, #8]
	str	r2, [sp, #32]
	mov	fp, r0
	ldr	r0, [r5, #8]
	bl	Func_080b5098
	ldr	r6, [pc, #88]
	ldr	r0, [r0, #0]
	movs	r3, #1
	add	r6, fp
	str	r0, [sp, #28]
	str	r3, [r5, #24]
	movs	r0, #1
	str	r5, [r6, #0]
	bl	Func_080cd594
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #60]
	strh	r3, [r2, #0]
	ldr	r3, [r6, #0]
	mov	r1, sp
	ldr	r0, [r3, #4]
	adds	r1, #44
	str	r1, [sp, #24]
	bl	Func_080cef64
	movs	r1, #2
	ldr	r0, [sp, #28]
	bl	Func_08009080
	movs	r1, #48
	ldr	r0, [sp, #28]
	bl	Func_08009088
	ldr	r0, [pc, #40]
	mov	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	movs	r1, #128
	lsls	r1, r1, #6
	ldr	r0, [pc, #28]
	add	r1, fp
	movs	r2, #1
	movs	r3, #0
	b.n	.L0
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000052
	.4byte 0x00000055
	.4byte 0x0000007d
.L0:
	bl	Func_080e0524
	movs	r2, #0
	ldr	r0, [pc, #776]
	ldr	r1, [sp, #32]
	movs	r3, #0
	bl	Func_080e0524
	movs	r2, #0
	str	r2, [sp, #36]
	mov	r9, r2
	mov	r8, fp
.L3:
	movs	r7, #225
	movs	r3, #0
	lsls	r7, r7, #7
	mov	sl, r3
	add	r7, r8
.L1:
	mov	r0, sl
	lsls	r6, r0, #1
	bl	Func_08004458
	ldr	r3, [pc, #744]
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	Func_08002322
	adds	r3, r6, #0
	muls	r3, r0
	adds	r0, r5, #0
	str	r3, [r7, #0]
	bl	Func_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	mov	r1, sl
	negs	r3, r3
	str	r3, [r7, #4]
	lsrs	r3, r1, #31
	add	r3, sl
	asrs	r3, r3, #1
	movs	r2, #1
	adds	r3, #25
	add	sl, r2
	str	r3, [r7, #24]
	mov	r3, sl
	adds	r7, #28
	cmp	r3, #16
	bne.n	.L1
	mov	r1, r9
	lsls	r3, r1, #3
	subs	r3, r3, r1
	ldr	r2, [pc, #684]
	movs	r0, #0
	lsls	r3, r3, #2
	mov	sl, r0
	adds	r7, r3, r2
.L2:
	bl	Func_08004458
	ldr	r5, [pc, #676]
	ands	r5, r0
	bl	Func_08004458
	ldr	r3, [pc, #660]
	adds	r6, r0, #0
	ands	r6, r3
	ldr	r3, [pc, #664]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r2, [r3, #4]
	ldr	r0, [sp, #36]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r1, [pc, #656]
	adds	r3, r0, r3
	ldrb	r3, [r1, r3]
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	movs	r3, #176
	lsls	r3, r3, #15
	str	r3, [r7, #4]
	adds	r0, r6, #0
	bl	Func_08002322
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	Func_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	Func_08004458
	movs	r3, #7
	ands	r3, r0
	adds	r3, #32
	str	r3, [r7, #24]
	movs	r2, #1
	movs	r3, #170
	add	sl, r2
	lsls	r3, r3, #1
	adds	r7, #28
	cmp	sl, r3
	bne.n	.L2
	ldr	r1, [sp, #36]
	movs	r0, #224
	lsls	r0, r0, #1
	adds	r1, #1
	add	r9, r3
	add	r8, r0
	str	r1, [sp, #36]
	cmp	r1, #3
	bne.n	.L3
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #556]
	movs	r3, #75
	add	r2, fp
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #548]
	lsls	r1, r1, #3
	bl	Func_080041d8
	movs	r2, #0
	mov	r9, r2
.L35:
	mov	r3, r9
	cmp	r3, #4
	bne.n	.L4
	movs	r0, #212
	bl	Func_080f9010
.L4:
	mov	r0, r9
	cmp	r0, #8
	bne.n	.L5
	ldr	r3, [pc, #524]
	add	r3, fp
	str	r0, [r3, #0]
.L5:
	mov	r1, r9
	cmp	r1, #18
	bne.n	.L6
	movs	r0, #145
	bl	Func_080f9010
.L6:
	mov	r2, r9
	cmp	r2, #40
	bne.n	.L7
	movs	r0, #134
	bl	Func_080b50e8
.L7:
	mov	r3, r9
	cmp	r3, #39
	bgt.n	.L8
	ldr	r3, [pc, #472]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	movs	r1, #128
	cmp	r3, #1
	bne.n	.L9
	mov	r0, r9
	cmp	r0, #9
	bgt.n	.L10
	lsls	r3, r0, #2
	add	r3, r9
	lsls	r3, r3, #1
	adds	r2, r3, #0
	lsls	r3, r0, #4
	adds	r5, r3, #0
	subs	r2, #8
	subs	r5, #128
	b.n	.L11
.L10:
	mov	r2, r9
	cmp	r2, #20
	ble.n	.L12
	mov	r0, r9
	lsls	r3, r0, #1
	adds	r5, r3, #0
	adds	r2, #62
	subs	r5, #24
	b.n	.L11
.L12:
	movs	r2, #82
	b.n	.L13
.L9:
	mov	r2, r9
	cmp	r2, #9
	bgt.n	.L14
	lsls	r3, r2, #2
	add	r3, r9
	lsls	r3, r3, #1
	mov	r0, r9
	subs	r2, r1, r3
	lsls	r3, r0, #4
	adds	r5, r3, #0
	subs	r5, #128
	b.n	.L11
.L14:
	mov	r2, r9
	cmp	r2, #20
	ble.n	.L15
	movs	r3, #58
	mov	r0, r9
	subs	r2, r3, r2
	lsls	r3, r0, #1
	adds	r5, r3, #0
	subs	r5, #24
	b.n	.L11
.L15:
	movs	r2, #38
.L13:
	movs	r5, #16
.L11:
	adds	r3, r5, #0
	adds	r3, #128
	cmp	r3, #104
	ble.n	.L16
	subs	r3, r1, r5
	adds	r1, r3, #0
	subs	r1, #24
.L16:
	cmp	r1, #0
	ble.n	.L8
	movs	r3, #64
	str	r3, [sp, #0]
	str	r1, [sp, #4]
	subs	r2, #32
	ldr	r4, [sp, #44]
	ldr	r0, [sp, #40]
	mov	r1, fp
	adds	r3, r5, #0
	bl	Func_080072f4
.L8:
	mov	r1, r9
	cmp	r1, #16
	ble.n	.L17
	ldr	r0, [pc, #340]
	bl	Func_080e46f0
.L17:
	movs	r2, #0
	movs	r3, #22
	movs	r0, #16
	mov	r1, fp
	str	r2, [sp, #36]
	str	r3, [sp, #20]
	str	r2, [sp, #16]
	str	r0, [sp, #12]
	str	r1, [sp, #8]
.L33:
	ldr	r2, [sp, #36]
	ldr	r3, [sp, #12]
	lsls	r1, r2, #3
	cmp	r9, r3
	bne.n	.L18
	ldr	r2, [pc, #300]
	movs	r3, #12
	add	r2, fp
	str	r3, [r2, #0]
.L18:
	ldr	r0, [sp, #12]
	cmp	r9, r0
	blt.n	.L19
	adds	r3, r1, #0
	adds	r3, #18
	cmp	r9, r3
	bge.n	.L20
	ldr	r3, [pc, #264]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r2, [r3, #4]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r2, [sp, #36]
	ldr	r1, [pc, #252]
	adds	r3, r2, r3
	ldrb	r2, [r1, r3]
	movs	r3, #32
	movs	r1, #128
	str	r3, [sp, #0]
	lsls	r1, r1, #6
	movs	r3, #64
	str	r3, [sp, #4]
	subs	r2, #16
	ldr	r4, [sp, #44]
	ldr	r0, [sp, #40]
	add	r1, fp
	movs	r3, #56
	bl	Func_080072f4
.L20:
	ldr	r0, [pc, #240]
	ldr	r1, [sp, #8]
	movs	r2, #225
	movs	r3, #0
	lsls	r2, r2, #7
	mov	sl, r3
	mov	r8, r0
	adds	r5, r1, r2
.L24:
	movs	r3, #6
	ldrsh	r7, [r5, r3]
	ldr	r3, [pc, #196]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r2, [r3, #4]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r2, [sp, #36]
	movs	r0, #2
	ldrsh	r1, [r5, r0]
	ldr	r0, [pc, #184]
	adds	r3, r2, r3
	ldrb	r3, [r0, r3]
	ldr	r0, [r5, #24]
	adds	r6, r1, r3
	cmp	r0, #17
	bhi.n	.L21
	movs	r1, #3
	bl	Func_080022ec
	mov	r2, r8
	ldrb	r1, [r2, r0]
	movs	r3, #128
	lsls	r1, r1, #11
	lsls	r3, r3, #6
	movs	r0, #32
	add	r1, fp
	adds	r1, r1, r3
	str	r0, [sp, #0]
	adds	r2, r6, #0
	movs	r0, #64
	adds	r3, r7, #0
	str	r0, [sp, #4]
	subs	r2, #16
	adds	r3, #56
	ldr	r4, [sp, #44]
	ldr	r0, [sp, #40]
	bl	Func_080072f4
	ldr	r0, [r5, #24]
.L21:
	cmp	r0, #0
	ble.n	.L22
	subs	r3, r0, #1
	b.n	.L23
.L22:
	movs	r3, #1
	negs	r3, r3
.L23:
	str	r3, [r5, #24]
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	adds	r5, #28
	cmp	r1, #12
	bne.n	.L24
.L19:
	ldr	r3, [sp, #12]
	adds	r3, #5
	cmp	r9, r3
	ble.n	.L25
	movs	r2, #0
	ldr	r7, [sp, #16]
	mov	sl, r2
.L28:
	lsls	r3, r7, #4
	adds	r3, r7, r3
	lsls	r3, r3, #2
	add	r3, sl
	lsls	r2, r3, #3
	subs	r2, r2, r3
	ldr	r3, [pc, #60]
	lsls	r2, r2, #2
	adds	r6, r2, r3
	ldr	r3, [r6, #24]
	cmp	r3, #0
	ble.n	.L26
	movs	r2, #128
	adds	r0, r6, #0
	movs	r1, #64
	lsls	r2, r2, #5
	bl	Func_080e3908
	ldr	r3, [r6, #24]
	movs	r0, #216
	ldr	r1, [r6, #4]
	subs	r3, #1
	lsls	r0, r0, #15
	str	r3, [r6, #24]
	cmp	r1, r0
	ble.n	.L27
	ldr	r3, [r6, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #16]
	b.n	.L26
	.4byte 0x00000073
	.4byte 0x0000ffff
	.4byte 0x02010000
	.4byte 0x000001ff
	.4byte 0x00007828
	.4byte 0x080eef06
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x000077a8
	.4byte 0x000000c0
	.4byte 0x080eef0c
.L27:
	ldr	r0, [r6, #0]
	ldr	r2, [pc, #288]
	cmp	r0, r2
	bhi.n	.L26
	cmp	r1, #0
	blt.n	.L26
	asrs	r1, r1, #16
	mov	r8, r1
	asrs	r6, r0, #16
	movs	r1, #5
	adds	r0, r3, #0
	bl	Func_080022ec
	adds	r0, #1
	lsls	r5, r0, #1
	mov	r3, sl
	ldr	r2, [pc, #260]
	movs	r4, #1
	ands	r4, r3
	subs	r3, r5, #2
	ldrh	r1, [r2, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r6, r6, r3
	mov	r3, r8
	ldr	r2, [sp, #32]
	subs	r3, r3, r0
	str	r0, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #24]
	lsls	r4, r4, #2
	adds	r1, r2, r1
	ldr	r4, [r4, r0]
	adds	r2, r6, #0
	ldr	r0, [sp, #40]
	mov	r8, r3
	bl	Func_080072f4
.L26:
	movs	r1, #1
	movs	r2, #128
	add	sl, r1
	lsls	r2, r2, #1
	cmp	sl, r2
	bne.n	.L28
.L25:
	ldr	r2, [pc, #208]
	movs	r3, #0
	mov	r0, fp
	mov	sl, r3
	ldr	r3, [r0, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L29
	ldr	r1, [sp, #20]
	movs	r6, #36
	mov	r8, r1
.L31:
	cmp	r9, r8
	bne.n	.L30
	mov	r3, fp
	adds	r5, r3, r2
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #10
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	mov	r3, sl
	bl	Func_080d6888
	ldr	r3, [r5, #0]
	movs	r1, #4
	ldrsh	r0, [r3, r6]
	bl	Func_080b5088
.L30:
	ldr	r2, [pc, #148]
	movs	r3, #1
	mov	r0, fp
	add	sl, r3
	ldr	r3, [r0, r2]
	ldr	r3, [r3, #20]
	adds	r6, #2
	cmp	sl, r3
	bne.n	.L31
.L29:
	ldr	r1, [sp, #20]
	ldr	r2, [sp, #16]
	adds	r1, #8
	adds	r2, #5
	ldr	r3, [sp, #12]
	ldr	r0, [sp, #8]
	str	r1, [sp, #20]
	str	r2, [sp, #16]
	movs	r1, #224
	ldr	r2, [sp, #36]
	lsls	r1, r1, #1
	adds	r3, #8
	adds	r0, r0, r1
	adds	r2, #1
	str	r3, [sp, #12]
	str	r0, [sp, #8]
	str	r2, [sp, #36]
	cmp	r2, #2
	beq.n	.L32
	b.n	.L33
.L32:
	movs	r0, #16
	movs	r1, #16
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r3, [pc, #84]
	movs	r5, #1
	add	r3, fp
	str	r5, [r3, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r3, #1
	add	r9, r3
	mov	r0, r9
	cmp	r0, #80
	beq.n	.L34
	b.n	.L35
.L34:
	ldr	r0, [sp, #28]
	movs	r1, #16
	bl	Func_08009088
	ldr	r0, [pc, #52]
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
	.4byte 0x007effff
	.4byte 0x080ede48
	.4byte 0x00007828
	.4byte 0x00007824
	.4byte 0x080cd261
