@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080d41a4
	.thumb_func
Func_080d41a4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #96]
	adds	r3, r5, #0
	ldmia	r3!, {r1}
	sub	sp, #52
	str	r1, [sp, #40]
	ldr	r3, [r3, #0]
	str	r3, [sp, #36]
	ldr	r3, [pc, #84]
	ldr	r2, [r5, #8]
	adds	r6, r1, r3
	str	r2, [sp, #28]
	str	r0, [r6, #0]
	movs	r0, #1
	bl	Func_080cd594
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #60]
	movs	r1, #7
	strh	r3, [r2, #0]
	movs	r0, #46
	movs	r3, #3
	movs	r2, #7
	str	r3, [sp, #0]
	bl	Func_080ed408
	ldr	r3, [r5, #28]
	str	r3, [sp, #44]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	bl	Func_080ed408
	ldr	r3, [r5, #32]
	mov	r5, sp
	adds	r5, #44
	str	r5, [sp, #12]
	str	r3, [r5, #4]
	ldr	r1, [sp, #40]
	ldr	r0, [pc, #24]
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	b.n	.L0
	.4byte 0x00001010
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000052
	.4byte 0x000000d1
.L0:
	movs	r3, #0
	ldr	r0, [pc, #700]
	ldr	r1, [sp, #28]
	movs	r2, #0
	bl	Func_080e0524
	ldr	r3, [r6, #0]
	ldr	r3, [r3, #24]
	cmp	r3, #2
	beq.n	.L1
	ldr	r0, [pc, #684]
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #680]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	Func_080072f0
.L1:
	movs	r7, #0
	movs	r2, #128
	ldr	r3, [pc, #668]
	mov	r8, r7
	movs	r1, #0
	lsls	r2, r2, #3
.L2:
	movs	r0, #1
	add	r8, r0
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L2
	ldr	r1, [sp, #40]
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r5, [pc, #648]
	adds	r2, r1, r3
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r1, r5
	movs	r3, #50
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #632]
	bl	Func_080041d8
	ldr	r0, [pc, #632]
	ldr	r7, [sp, #40]
	adds	r3, r7, r0
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #628]
	ldr	r3, [r3, #24]
	ldrb	r2, [r2, r3]
	str	r2, [sp, #24]
	ldr	r5, [sp, #24]
	lsls	r2, r2, #3
	movs	r7, #48
	movs	r1, #0
	subs	r3, r2, r5
	negs	r7, r7
	mov	fp, r1
	str	r2, [sp, #8]
	cmp	r3, r7
	bne.n	.L3
	b.n	.L4
.L3:
	ldr	r1, [pc, #592]
	ldr	r0, [sp, #40]
	adds	r1, r0, r1
	str	r1, [sp, #20]
.L28:
	ldr	r2, [sp, #20]
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #24]
	cmp	r3, #2
	bne.n	.L5
	mov	r3, fp
	cmp	r3, #63
	bgt.n	.L5
	ldr	r3, [pc, #576]
	movs	r2, #128
	mov	r5, fp
	ldr	r1, [r3, #0]
	lsls	r2, r2, #1
	cmp	r5, #55
	ble.n	.L6
	mov	r7, fp
	movs	r3, #176
	lsls	r2, r7, #3
	lsls	r3, r3, #2
	subs	r2, r3, r2
.L6:
	ldr	r0, [sp, #20]
	ldr	r3, [r0, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L7
	ldrh	r3, [r1, #54]
	subs	r3, r3, r2
	b.n	.L8
.L7:
	ldrh	r3, [r1, #54]
	adds	r3, r3, r2
.L8:
	strh	r3, [r1, #54]
.L5:
	mov	r1, fp
	cmp	r1, #32
	bne.n	.L9
	movs	r0, #134
	bl	Func_080b50e8
.L9:
	ldr	r3, [sp, #24]
	movs	r2, #0
	str	r2, [sp, #32]
	cmp	r3, #0
	bne.n	.L10
	b.n	.L11
.L10:
	ldr	r7, [sp, #32]
	lsls	r5, r7, #3
	cmp	fp, r5
	bne.n	.L12
	movs	r0, #134
	bl	Func_080f9010
	movs	r1, #128
	ldr	r3, [pc, #496]
	ldr	r0, [sp, #36]
	lsls	r1, r1, #7
	ldr	r2, [pc, #492]
	bl	Func_080072f0
.L12:
	adds	r0, r5, #4
	mov	sl, r0
	cmp	fp, r5
	bge.n	.L13
	b.n	.L14
.L13:
	adds	r3, r5, #0
	adds	r3, #9
	cmp	fp, r3
	blt.n	.L15
	b.n	.L14
.L15:
	adds	r3, r5, #1
	adds	r6, r5, #2
	cmp	fp, r3
	blt.n	.L16
	cmp	fp, r6
	bge.n	.L17
	ldr	r2, [sp, #20]
	ldr	r3, [r2, #0]
	ldr	r2, [r3, #4]
	ldr	r7, [sp, #32]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r0, [pc, #448]
	adds	r3, r7, r3
	ldrb	r2, [r0, r3]
	movs	r3, #48
	str	r3, [sp, #0]
	movs	r3, #112
	str	r3, [sp, #4]
	subs	r2, #24
	ldr	r4, [sp, #44]
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #40]
	movs	r3, #0
	bl	Func_080072f4
.L16:
	adds	r1, r5, #4
	mov	sl, r1
	cmp	fp, r6
	blt.n	.L14
.L17:
	adds	r5, #4
	mov	sl, r5
	cmp	fp, sl
	bge.n	.L18
	ldr	r2, [sp, #20]
	ldr	r3, [r2, #0]
	ldr	r2, [r3, #4]
	ldr	r5, [sp, #32]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r7, [pc, #392]
	adds	r3, r5, r3
	ldrb	r2, [r7, r3]
	movs	r3, #48
	str	r3, [sp, #0]
	movs	r3, #112
	str	r3, [sp, #4]
	movs	r5, #168
	ldr	r3, [sp, #40]
	lsls	r5, r5, #5
	adds	r1, r3, r5
	subs	r2, #24
	ldr	r4, [sp, #44]
	ldr	r0, [sp, #36]
	movs	r3, #0
	bl	Func_080072f4
.L18:
	cmp	fp, r6
	bne.n	.L14
	ldr	r1, [sp, #40]
	ldr	r2, [pc, #332]
	movs	r7, #0
	str	r7, [sp, #16]
	movs	r0, #0
	adds	r1, r1, r2
	ldr	r7, [pc, #344]
	mov	r8, r0
	mov	r9, r1
.L21:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	bne.n	.L19
	bl	Func_08004458
	ldr	r6, [pc, #332]
	ands	r6, r0
	bl	Func_08004458
	ldr	r5, [pc, #328]
	ldr	r3, [pc, #332]
	ands	r5, r0
	mov	r0, r9
	adds	r5, r5, r3
	ldr	r3, [r0, #0]
	ldr	r2, [r3, #4]
	ldr	r1, [sp, #32]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r2, [pc, #296]
	adds	r3, r1, r3
	ldrb	r3, [r2, r3]
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	movs	r3, #208
	lsls	r3, r3, #15
	str	r3, [r7, #4]
	adds	r0, r5, #0
	bl	Func_08002322
	adds	r6, #32
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	Func_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #7
	str	r3, [r7, #16]
	bl	Func_08004458
	movs	r3, #7
	ands	r3, r0
	adds	r3, #32
	str	r3, [r7, #24]
	ldr	r3, [sp, #16]
	adds	r3, #1
	str	r3, [sp, #16]
	mov	r5, r9
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #24]
	ldr	r1, [pc, #244]
	lsls	r3, r3, #2
	ldrh	r3, [r1, r3]
	ldr	r0, [sp, #16]
	cmp	r0, r3
	beq.n	.L20
.L19:
	movs	r1, #1
	movs	r2, #128
	add	r8, r1
	lsls	r2, r2, #3
	adds	r7, #28
	cmp	r8, r2
	bne.n	.L21
	ldr	r1, [pc, #216]
.L20:
	ldr	r3, [sp, #40]
	ldr	r7, [sp, #20]
	ldr	r5, [pc, #216]
	adds	r2, r3, r5
	ldr	r3, [r7, #0]
	ldr	r3, [r3, #24]
	lsls	r3, r3, #2
	adds	r3, #2
	ldrh	r3, [r1, r3]
	str	r3, [r2, #0]
.L14:
	cmp	fp, sl
	bne.n	.L22
	ldr	r3, [pc, #152]
	ldr	r1, [sp, #40]
	ldr	r3, [r1, r3]
	ldr	r3, [r3, #20]
	movs	r0, #0
	mov	r8, r0
	cmp	r3, #0
	beq.n	.L22
	ldr	r2, [pc, #136]
	movs	r6, #36
	adds	r5, r1, r2
.L23:
	ldr	r3, [r5, #0]
	movs	r1, #1
	ldrsh	r0, [r3, r6]
	bl	Func_080b5088
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r2, #5
	mov	r3, r8
	movs	r1, #7
	bl	Func_080d6888
	ldr	r3, [r5, #0]
	movs	r2, #1
	ldr	r3, [r3, #20]
	add	r8, r2
	adds	r6, #2
	cmp	r8, r3
	bne.n	.L23
.L22:
	ldr	r3, [sp, #32]
	ldr	r5, [sp, #24]
	adds	r3, #1
	str	r3, [sp, #32]
	cmp	r3, r5
	beq.n	.L11
	b.n	.L10
.L11:
	movs	r7, #0
	ldr	r6, [pc, #96]
	mov	r8, r7
.L27:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	ble.n	.L24
	subs	r3, #1
	movs	r2, #128
	adds	r0, r6, #0
	lsls	r2, r2, #5
	str	r3, [r6, #24]
	movs	r1, #60
	bl	Func_080e3908
	movs	r0, #208
	ldr	r2, [r6, #4]
	lsls	r0, r0, #15
	cmp	r2, r0
	ble.n	.L25
	ldr	r3, [r6, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #16]
	b.n	.L24
	.4byte 0x00000073
	.4byte 0x00000060
	.4byte 0x03001388
	.4byte 0x02010018
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007828
	.4byte 0x080ee25e
	.4byte 0x03001e80
	.4byte 0x03000168
	.4byte 0x10101010
	.4byte 0x080ee250
	.4byte 0x02010000
	.4byte 0x000003ff
	.4byte 0x00007fff
	.4byte 0xffffc000
	.4byte 0x080ee244
	.4byte 0x000077a8
.L25:
	ldr	r3, [r6, #0]
	asrs	r7, r3, #16
	cmp	r3, #0
	blt.n	.L24
	cmp	r7, #119
	bgt.n	.L24
	cmp	r2, #0
	blt.n	.L24
	ldr	r0, [r6, #24]
	asrs	r2, r2, #16
	mov	ip, r2
	cmp	r0, #0
	bge.n	.L26
	adds	r0, #7
.L26:
	asrs	r0, r0, #3
	adds	r0, #1
	lsls	r5, r0, #1
	ldr	r2, [pc, #144]
	mov	r1, r8
	subs	r3, r5, #2
	movs	r4, #1
	ands	r4, r1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #28]
	adds	r1, r2, r1
	lsrs	r2, r0, #31
	adds	r2, r0, r2
	asrs	r2, r2, #1
	subs	r2, r7, r2
	mov	r7, ip
	str	r0, [sp, #0]
	subs	r3, r7, r0
	str	r5, [sp, #4]
	ldr	r0, [sp, #12]
	lsls	r4, r4, #2
	ldr	r4, [r4, r0]
	ldr	r0, [sp, #36]
	bl	Func_080072f4
.L24:
	movs	r1, #1
	movs	r2, #128
	add	r8, r1
	lsls	r2, r2, #3
	adds	r6, #28
	cmp	r8, r2
	bne.n	.L27
	movs	r1, #16
	movs	r0, #8
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r5, [pc, #76]
	ldr	r3, [sp, #40]
	adds	r2, r3, r5
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r0, [sp, #8]
	ldr	r1, [sp, #24]
	movs	r7, #1
	subs	r3, r0, r1
	add	fp, r7
	adds	r3, #48
	cmp	fp, r3
	beq.n	.L4
	b.n	.L28
.L4:
	ldr	r0, [pc, #48]
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
	.4byte 0x080ede48
	.4byte 0x00007824
	.4byte 0x080cd261
