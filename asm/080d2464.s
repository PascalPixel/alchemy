@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080d2464
	.thumb_func
Func_080d2464:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #72
	str	r1, [sp, #56]
	ldr	r5, [pc, #104]
	ldr	r1, [r5, #0]
	adds	r3, r5, #0
	str	r1, [sp, #52]
	subs	r3, #112
	ldr	r3, [r3, #0]
	str	r3, [sp, #48]
	subs	r3, r5, #4
	ldr	r3, [r3, #0]
	ldr	r7, [pc, #88]
	mov	sl, r3
	ldr	r2, [r5, #4]
	add	r7, sl
	str	r2, [sp, #32]
	str	r0, [r7, #0]
	movs	r0, #0
	bl	Func_080cd594
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	subs	r2, #70
	strh	r3, [r2, #0]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	mov	r8, r3
	movs	r0, #46
	movs	r3, #3
	bl	Func_080ed408
	ldr	r4, [r5, #24]
	movs	r3, #3
	movs	r1, #7
	movs	r2, #7
	movs	r0, #47
	str	r4, [sp, #40]
	str	r3, [sp, #0]
	bl	Func_080ed408
	ldr	r5, [r5, #28]
	ldr	r0, [pc, #28]
	str	r5, [sp, #44]
	bl	Func_08002f40
	adds	r5, r0, #0
	b.n	.L0
	.4byte 0x00001010
	.4byte 0x00000784
	.4byte 0x03001ef0
	.4byte 0x00007828
	.4byte 0x04000052
	.4byte 0x0000007d
.L0:
	movs	r0, #160
	movs	r2, #128
	adds	r1, r5, #0
	ldr	r6, [pc, #848]
	adds	r5, #128
	lsls	r0, r0, #19
	bl	Func_080072fc
	mov	r1, sl
	adds	r0, r5, #0
	bl	Func_08005340
	ldr	r0, [pc, #832]
	bl	Func_08002f40
	movs	r1, #192
	adds	r5, r0, #0
	adds	r5, #128
	lsls	r1, r1, #6
	add	r1, sl
	adds	r0, r5, #0
	bl	Func_08005340
	ldr	r0, [pc, #816]
	bl	Func_08002f40
	ldr	r1, [sp, #32]
	bl	Func_08005340
	ldr	r0, [sp, #56]
	cmp	r0, #1
	bne.n	.L1
	ldr	r0, [pc, #800]
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	lsls	r0, r0, #19
	movs	r2, #128
	bl	Func_080072fc
.L1:
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r2, [pc, #784]
	add	r3, sl
	mov	r1, r8
	str	r1, [r3, #0]
	add	r2, sl
	movs	r3, #75
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #772]
	lsls	r1, r1, #3
	bl	Func_080041d8
	ldr	r3, [sp, #56]
	movs	r2, #1
	str	r2, [sp, #20]
	cmp	r3, #0
	beq.n	.L2
	ldr	r3, [r7, #0]
	movs	r4, #1
	ldr	r3, [r3, #4]
	negs	r4, r4
	str	r4, [sp, #20]
	cmp	r3, #1
	beq.n	.L2
	movs	r6, #1
	str	r6, [sp, #20]
.L2:
	ldr	r0, [sp, #56]
	cmp	r0, #1
	bne.n	.L3
	ldr	r5, [pc, #732]
	add	r5, sl
	ldr	r3, [r5, #0]
	add	r6, sp, #60
	ldr	r0, [r3, #8]
	adds	r1, r6, #0
	bl	Func_080e396c
	ldr	r3, [r6, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #0]
	movs	r3, #66
	str	r3, [r6, #4]
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L4
	movs	r3, #76
	b.n	.L5
.L4:
	movs	r3, #44
.L5:
	str	r3, [r6, #0]
.L3:
	ldr	r2, [pc, #692]
	movs	r1, #212
	lsls	r1, r1, #16
	ldr	r3, [pc, #688]
	str	r2, [sp, #28]
	str	r1, [sp, #24]
	movs	r2, #1
	movs	r7, #0
	negs	r2, r2
	add	r3, sl
.L6:
	adds	r7, #1
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r7, #64
	bne.n	.L6
	ldr	r5, [pc, #668]
	movs	r7, #0
	movs	r6, #127
	add	r5, sl
.L9:
	ldr	r3, [sp, #20]
	cmp	r3, #1
	bne.n	.L7
	bl	Func_08004458
	ands	r0, r6
	adds	r0, #128
	b.n	.L8
.L7:
	bl	Func_08004458
	ands	r0, r6
	subs	r0, #128
.L8:
	str	r0, [r5, #0]
	bl	Func_08004458
	movs	r3, #7
	ands	r3, r0
	subs	r3, #72
	str	r3, [r5, #4]
	bl	Func_08004458
	movs	r3, #31
	ands	r3, r0
	negs	r3, r3
	adds	r7, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #16
	bne.n	.L9
	movs	r1, #1
	movs	r2, #128
	ldr	r3, [pc, #604]
	movs	r7, #0
	negs	r1, r1
	lsls	r2, r2, #2
.L10:
	adds	r7, #1
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r7, r2
	bne.n	.L10
	ldr	r4, [sp, #56]
	cmp	r4, #0
	bne.n	.L11
	ldr	r3, [pc, #564]
	add	r3, sl
	ldr	r0, [r3, #0]
	bl	Func_080d6750
	movs	r0, #1
	bl	Func_080030f8
	ldr	r1, [pc, #568]
	movs	r0, #8
	movs	r2, #2
	bl	Func_080dbb24
.L11:
	ldr	r0, [sp, #48]
	movs	r6, #0
	adds	r0, #12
	str	r6, [sp, #36]
	str	r0, [sp, #12]
.L66:
	ldr	r3, [pc, #552]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L12
	ldr	r1, [sp, #36]
	cmp	r1, #48
	ble.n	.L12
	cmp	r1, #159
	bgt.n	.L12
	ldr	r2, [sp, #56]
	cmp	r2, #0
	bne.n	.L13
	ldr	r3, [pc, #528]
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #8
	bl	Func_08009020
	ldr	r3, [pc, #520]
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #9
	bl	Func_08009020
	ldr	r3, [pc, #512]
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #10
	bl	Func_08009020
	ldr	r3, [pc, #504]
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #11
	bl	Func_08009020
.L13:
	ldr	r3, [pc, #452]
	mov	r4, sl
	ldr	r3, [r4, r3]
	ldr	r3, [r3, #20]
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L14
	ldr	r5, [pc, #436]
	movs	r6, #36
	add	r5, sl
.L15:
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r1, #10
	subs	r3, #1
	bl	Func_080d6888
	ldr	r3, [r5, #0]
	movs	r1, #4
	ldrsh	r0, [r3, r6]
	bl	Func_080b5088
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	adds	r7, #1
	adds	r6, #2
	cmp	r7, r3
	bne.n	.L15
.L14:
	movs	r3, #160
	str	r3, [sp, #36]
.L12:
	bl	Func_080049ac
	ldr	r0, [sp, #48]
	ldr	r1, [sp, #12]
	bl	Func_080051d8
	ldr	r4, [sp, #36]
	cmp	r4, #178
	bne.n	.L16
	movs	r0, #134
	bl	Func_080b50e8
.L16:
	ldr	r6, [sp, #36]
	cmp	r6, #128
	bne.n	.L17
	ldr	r2, [pc, #348]
	movs	r3, #50
	add	r2, sl
	str	r3, [r2, #0]
.L17:
	ldr	r0, [sp, #36]
	cmp	r0, #176
	bne.n	.L18
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #3
	str	r3, [r2, #0]
	ldr	r2, [pc, #324]
	ldr	r3, [pc, #376]
	add	r2, sl
	str	r3, [r2, #0]
	ldr	r0, [pc, #304]
	bl	Func_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #292]
	lsls	r0, r0, #19
	adds	r1, r5, #0
	movs	r2, #128
	bl	Func_080072f0
	b.n	.L19
.L18:
	ldr	r3, [sp, #36]
	subs	r3, #160
	cmp	r3, #15
	bhi.n	.L19
	movs	r3, #239
	lsls	r3, r3, #7
	add	r3, sl
	movs	r2, #1
	str	r2, [r3, #0]
	ldr	r2, [pc, #276]
	ldr	r3, [pc, #328]
	add	r2, sl
	str	r3, [r2, #0]
	ldr	r1, [sp, #36]
	cmp	r1, #173
	ble.n	.L20
	ldr	r3, [pc, #320]
	str	r3, [r2, #0]
.L20:
	movs	r0, #2
	movs	r1, #2
	movs	r2, #2
	bl	Func_080e727c
.L19:
	ldr	r3, [sp, #36]
	subs	r3, #33
	cmp	r3, #142
	bhi.n	.L21
	ldr	r4, [sp, #36]
	movs	r2, #0
	movs	r3, #1
	mov	r8, r2
	str	r3, [sp, #16]
	cmp	r4, #103
	ble.n	.L22
	movs	r6, #8
	str	r6, [sp, #16]
.L22:
	ldr	r1, [pc, #284]
	movs	r0, #127
	ldr	r6, [pc, #284]
	movs	r7, #0
	mov	fp, r0
	mov	r9, r1
.L24:
	movs	r2, #1
	ldr	r3, [r6, #24]
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L23
	bl	Func_08004458
	movs	r3, #255
	ands	r3, r0
	subs	r3, #32
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	movs	r3, #224
	lsls	r3, r3, #15
	str	r3, [r6, #4]
	bl	Func_08004458
	movs	r5, #3
	mov	r4, r9
	mov	r3, fp
	ands	r5, r7
	ands	r0, r3
	ldrb	r3, [r4, r5]
	adds	r0, r0, r3
	lsls	r0, r0, #9
	str	r0, [r6, #12]
	bl	Func_08004458
	mov	r2, r9
	ldrb	r3, [r2, r5]
	mov	r1, fp
	ands	r0, r1
	adds	r0, r0, r3
	negs	r0, r0
	movs	r3, #0
	lsls	r0, r0, #11
	str	r3, [r6, #24]
	str	r0, [r6, #16]
	movs	r3, #1
	ldr	r4, [sp, #16]
	add	r8, r3
	cmp	r8, r4
	beq.n	.L21
.L23:
	movs	r0, #128
	adds	r7, #1
	lsls	r0, r0, #2
	adds	r6, #28
	cmp	r7, r0
	bne.n	.L24
.L21:
	ldr	r3, [sp, #36]
	subs	r3, #41
	cmp	r3, #86
	bls.n	.L25
	b.n	.L26
.L25:
	ldr	r2, [sp, #36]
	movs	r3, #1
	movs	r1, #0
	ands	r3, r2
	mov	fp, r1
	cmp	r3, #0
	beq.n	.L26
	movs	r3, #60
	ldr	r5, [pc, #156]
	add	r3, sp
	movs	r7, #0
	mov	r9, r3
	add	r5, sl
.L30:
	movs	r4, #1
	ldr	r3, [r5, #24]
	negs	r4, r4
	cmp	r3, r4
	bne.n	.L27
	bl	Func_08004458
	movs	r3, #255
	ands	r3, r0
	adds	r3, #128
	mov	r8, r3
	bl	Func_08004458
	ldr	r3, [pc, #124]
	ldr	r1, [sp, #56]
	ands	r3, r0
	ldr	r0, [pc, #124]
	adds	r6, r3, r0
	cmp	r1, #0
	bne.n	.L28
	bl	Func_08004458
	movs	r2, #7
	ands	r0, r2
	adds	r0, #78
	movs	r3, #140
	lsls	r0, r0, #16
	lsls	r3, r3, #15
	str	r0, [r5, #0]
	b.n	.L29
	movs	r0, r0
	.4byte 0x03001388
	.4byte 0x000000b4
	.4byte 0x00000073
	.4byte 0x000000c4
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007828
	.4byte 0xffc40000
	.4byte 0x00007098
	.4byte 0x00007320
	.4byte 0x02010018
	.4byte 0x00000179
	.4byte 0x03001b04
	.4byte 0x000077d8
	.4byte 0x000077dc
	.4byte 0x000077e4
	.4byte 0x000077e8
	.4byte 0x02020202
	.4byte 0x10101010
	.4byte 0x3f3f3f3f
	.4byte 0x080ee184
	.4byte 0x02010000
	.4byte 0x000074e0
	.4byte 0x00001fff
	.4byte 0x00004e20
.L28:
	bl	Func_08004458
	movs	r3, #7
	mov	r4, r9
	ands	r0, r3
	ldr	r3, [r4, #0]
	adds	r0, r0, r3
	subs	r0, #8
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	ldr	r3, [r4, #4]
	lsls	r3, r3, #16
.L29:
	str	r3, [r5, #4]
	adds	r0, r6, #0
	bl	Func_08002322
	mov	r3, r8
	muls	r3, r0
	asrs	r3, r3, #9
	str	r3, [r5, #12]
	adds	r0, r6, #0
	bl	Func_0800231c
	mov	r3, r8
	muls	r3, r0
	movs	r6, #1
	asrs	r3, r3, #9
	add	fp, r6
	str	r3, [r5, #16]
	mov	r0, fp
	movs	r3, #0
	str	r3, [r5, #24]
	cmp	r0, #1
	beq.n	.L26
.L27:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #24
	bne.n	.L30
.L26:
	ldr	r1, [sp, #36]
	cmp	r1, #48
	bne.n	.L31
	movs	r0, #141
	bl	Func_080f9010
.L31:
	ldr	r2, [sp, #36]
	cmp	r2, #128
	bne.n	.L32
	movs	r0, #145
	bl	Func_080f9010
.L32:
	ldr	r3, [sp, #36]
	subs	r3, #129
	cmp	r3, #46
	bhi.n	.L33
	movs	r4, #60
	movs	r5, #225
	movs	r3, #0
	add	r4, sp
	lsls	r5, r5, #7
	mov	fp, r3
	movs	r7, #0
	mov	r9, r4
	add	r5, sl
.L37:
	movs	r6, #1
	.4byte 0x427669ab
	cmp	r3, r6
	bne.n	.L34
	bl	Func_08004458
	movs	r3, #255
	ands	r3, r0
	adds	r3, #128
	mov	r8, r3
	bl	Func_08004458
	ldr	r3, [pc, #788]
	ldr	r1, [sp, #56]
	ands	r3, r0
	ldr	r0, [pc, #788]
	adds	r6, r3, r0
	cmp	r1, #0
	bne.n	.L35
	movs	r3, #136
	lsls	r3, r3, #15
	str	r3, [r5, #0]
	movs	r3, #128
	lsls	r3, r3, #15
	b.n	.L36
.L35:
	mov	r2, r9
	ldr	r3, [r2, #0]
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldr	r3, [r2, #4]
	lsls	r3, r3, #16
.L36:
	str	r3, [r5, #4]
	adds	r0, r6, #0
	bl	Func_08002322
	mov	r3, r8
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r5, #12]
	adds	r0, r6, #0
	bl	Func_0800231c
	mov	r3, r8
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r5, #16]
	movs	r3, #0
	str	r3, [r5, #24]
	movs	r3, #1
	add	fp, r3
	mov	r4, fp
	cmp	r4, #1
	beq.n	.L33
.L34:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #24
	bne.n	.L37
.L33:
	ldr	r6, [sp, #36]
	cmp	r6, #175
	bgt.n	.L38
	movs	r5, #225
	lsls	r5, r5, #7
	movs	r7, #0
	add	r5, sl
.L40:
	ldr	r1, [r5, #24]
	cmp	r1, #0
	blt.n	.L39
	movs	r0, #2
	ldrsh	r2, [r5, r0]
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	movs	r0, #32
	asrs	r1, r1, #2
	str	r0, [sp, #0]
	lsls	r1, r1, #11
	movs	r0, #64
	subs	r2, #16
	subs	r3, #32
	str	r0, [sp, #4]
	add	r1, sl
	ldr	r0, [sp, #52]
	ldr	r6, [sp, #40]
	bl	Func_080072fc
	ldr	r3, [r5, #12]
	ldr	r0, [sp, #20]
	adds	r2, r0, #0
	muls	r2, r3
	ldr	r3, [r5, #0]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #24
	bne.n	.L39
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L39:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #24
	bne.n	.L40
.L38:
	ldr	r5, [pc, #608]
	movs	r7, #0
	movs	r6, #1
	add	r5, sl
.L42:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L41
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	movs	r1, #2
	ldrsh	r2, [r5, r1]
	movs	r1, #2
	subs	r3, #1
	str	r1, [sp, #4]
	str	r6, [sp, #0]
	ldr	r1, [pc, #580]
	ldr	r0, [sp, #52]
	ldr	r4, [sp, #44]
	bl	Func_080072f4
	ldr	r3, [r5, #12]
	ldr	r0, [sp, #20]
	adds	r2, r0, #0
	muls	r2, r3
	ldr	r3, [r5, #0]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r1, [pc, #552]
	ldr	r3, [r5, #24]
	adds	r2, r2, r1
	adds	r3, #1
	str	r2, [r5, #16]
	str	r3, [r5, #24]
	cmp	r3, #48
	bne.n	.L41
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L41:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #24
	bne.n	.L42
	ldr	r2, [sp, #36]
	cmp	r2, #175
	bgt.n	.L43
	ldr	r5, [pc, #520]
	movs	r7, #0
.L48:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L44
	ldr	r2, [pc, #516]
	movs	r3, #3
	ands	r3, r7
	ldrb	r0, [r2, r3]
	ldr	r2, [pc, #512]
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #32]
	movs	r6, #2
	ldrsh	r2, [r5, r6]
	adds	r1, r3, r1
	lsrs	r3, r0, #1
	subs	r2, r2, r3
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #52]
	ldr	r4, [sp, #40]
	bl	Func_080072f4
	ldr	r1, [r5, #12]
	ldr	r6, [sp, #20]
	adds	r2, r6, #0
	muls	r2, r1
	ldr	r3, [r5, #0]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r0, [sp, #36]
	cmp	r0, #128
	ble.n	.L45
	movs	r3, #1
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L46
	ldr	r2, [pc, #440]
	adds	r3, r1, r2
	str	r3, [r5, #12]
	b.n	.L47
.L46:
	ldr	r4, [pc, #436]
	adds	r3, r1, r4
	str	r3, [r5, #12]
	b.n	.L47
.L45:
	ldr	r6, [pc, #408]
	adds	r3, r2, r6
	str	r3, [r5, #16]
.L47:
	ldr	r3, [r5, #24]
	movs	r0, #128
	adds	r3, #1
	lsls	r0, r0, #1
	str	r3, [r5, #24]
	cmp	r3, r0
	bne.n	.L44
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L44:
	movs	r1, #128
	adds	r7, #1
	lsls	r1, r1, #2
	adds	r5, #28
	cmp	r7, r1
	bne.n	.L48
.L43:
	ldr	r2, [sp, #36]
	cmp	r2, #128
	bne.n	.L49
	ldr	r2, [pc, #388]
	movs	r3, #48
	add	r2, sl
	str	r3, [r2, #0]
.L49:
	ldr	r3, [sp, #56]
	cmp	r3, #0
	bne.n	.L50
	ldr	r4, [sp, #36]
	cmp	r4, #48
	bne.n	.L50
	ldr	r2, [pc, #368]
	movs	r3, #8
	add	r2, sl
	str	r3, [r2, #0]
.L50:
	ldr	r3, [sp, #36]
	subs	r3, #40
	cmp	r3, #7
	bhi.n	.L51
	ldr	r6, [sp, #24]
	ldr	r1, [sp, #28]
	ldr	r0, [pc, #352]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r0, r6, r0
	adds	r2, r1, r2
	str	r0, [sp, #24]
	str	r2, [sp, #28]
.L51:
	ldr	r3, [sp, #56]
	cmp	r3, #0
	bne.n	.L52
	ldr	r4, [sp, #36]
	cmp	r4, #128
	bne.n	.L53
	ldr	r3, [pc, #332]
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #8
	bl	Func_08009020
	ldr	r3, [pc, #324]
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #9
	bl	Func_08009020
	ldr	r3, [pc, #316]
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #10
	bl	Func_08009020
	ldr	r3, [pc, #308]
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #11
	bl	Func_08009020
.L53:
	ldr	r6, [sp, #36]
	cmp	r6, #176
	bne.n	.L54
	ldr	r3, [pc, #276]
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl	Func_08009020
	ldr	r3, [pc, #268]
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	Func_08009020
	ldr	r3, [pc, #260]
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #3
	bl	Func_08009020
	ldr	r3, [pc, #252]
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #4
	bl	Func_08009020
.L54:
	movs	r0, #3
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #28]
	bl	Func_080e6d3c
.L52:
	ldr	r0, [sp, #36]
	cmp	r0, #138
	bne.n	.L55
	ldr	r3, [pc, #228]
	mov	r1, sl
	ldr	r3, [r1, r3]
	ldr	r3, [r3, #20]
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L55
	ldr	r5, [pc, #216]
	movs	r6, #36
	add	r5, sl
.L56:
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #10
	movs	r2, #5
	subs	r3, #1
	bl	Func_080d6888
	ldr	r3, [r5, #0]
	movs	r1, #4
	ldrsh	r0, [r3, r6]
	bl	Func_080b5088
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	adds	r7, #1
	adds	r6, #2
	cmp	r7, r3
	bne.n	.L56
.L55:
	ldr	r6, [sp, #36]
	cmp	r6, #175
	ble.n	.L57
	b.n	.L58
.L57:
	ldr	r5, [pc, #164]
	movs	r0, #0
	movs	r1, #20
	movs	r2, #5
	movs	r7, #0
	mov	fp, r0
	mov	r8, r1
	mov	r9, r2
	add	r5, sl
.L64:
	ldr	r6, [r5, #4]
	cmp	r6, #55
	ble.n	.L59
	ldr	r3, [r5, #24]
	cmp	r3, #11
	bhi.n	.L60
	lsrs	r4, r3, #31
	adds	r4, r3, r4
	asrs	r4, r4, #1
	ldr	r2, [pc, #132]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	movs	r3, #192
	lsls	r3, r3, #6
	add	r1, sl
	adds	r1, r1, r3
	ldr	r3, [pc, #124]
	ldrb	r0, [r3, r4]
	ldr	r2, [r5, #0]
	lsrs	r3, r0, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #116]
	ldrb	r3, [r3, r4]
	str	r0, [sp, #0]
	ldr	r0, [pc, #116]
	ldrb	r0, [r0, r4]
	adds	r3, r6, r3
	str	r0, [sp, #4]
	ldr	r4, [sp, #40]
	ldr	r0, [sp, #52]
	bl	Func_080072f4
	ldr	r3, [r5, #24]
.L60:
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #12
	bne.n	.L61
	mov	r6, fp
	str	r6, [r5, #24]
	b.n	.L61
	movs	r0, r0
	.4byte 0x00001fff
	.4byte 0xffffb1e0
	.4byte 0x000074e0
	.4byte 0x080ee188
	.4byte 0xfffffc00
	.4byte 0x02010000
	.4byte 0x080ee18a
	.4byte 0x080ede48
	.4byte 0xffff8000
	.4byte 0xffffe000
	.4byte 0x000077a8
	.4byte 0xfff80000
	.4byte 0x000077d8
	.4byte 0x000077dc
	.4byte 0x000077e4
	.4byte 0x000077e8
	.4byte 0x00007828
	.4byte 0x00007320
	.4byte 0x080ee1a0
	.4byte 0x080ee18e
	.4byte 0x080ee19a
	.4byte 0x080ee194
.L59:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L62
	ldr	r0, [sp, #20]
	lsls	r3, r0, #1
	ldr	r2, [r5, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #1
	subs	r2, r2, r3
	adds	r3, r6, #6
	str	r2, [r5, #0]
	str	r3, [r5, #4]
	ldr	r1, [sp, #36]
	movs	r4, #10
	cmp	r1, #47
	bgt.n	.L63
	cmp	r3, #55
	ble.n	.L63
	movs	r0, #136
	str	r4, [sp, #8]
	bl	Func_080f9010
	ldr	r4, [sp, #8]
.L63:
	ldr	r2, [pc, #164]
	mov	r3, r8
	subs	r3, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #32]
	adds	r1, r2, r1
	ldr	r2, [r5, #0]
	mov	r3, r9
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r4, [sp, #0]
	mov	r4, r8
	adds	r3, #30
	str	r4, [sp, #4]
	ldr	r0, [sp, #52]
	ldr	r6, [sp, #40]
	bl	Func_080072fc
	b.n	.L61
.L62:
	adds	r3, #1
	str	r3, [r5, #24]
.L61:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #16
	beq.n	.L58
	b.n	.L64
.L58:
	movs	r0, #8
	movs	r1, #8
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r2, [pc, #104]
	movs	r3, #1
	add	r2, sl
	movs	r0, #1
	str	r3, [r2, #0]
	bl	Func_080030f8
	ldr	r0, [sp, #36]
	adds	r0, #1
	str	r0, [sp, #36]
	cmp	r0, #208
	beq.n	.L65
	b.n	.L66
.L65:
	ldr	r0, [pc, #80]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	ldr	r1, [sp, #56]
	cmp	r1, #0
	bne.n	.L67
	movs	r0, #3
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #28]
	bl	Func_080e6eac
	ldr	r5, [pc, #52]
	movs	r7, #0
	add	r5, sl
.L68:
	ldmia	r5!, {r0}
	adds	r7, #1
	bl	Func_08009038
	cmp	r7, #8
	bne.n	.L68
.L67:
	bl	Func_080cdbc0
	add	sp, #72
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
	.4byte 0x000077d8
