.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_08017e88, 0x08017e88
	.set sub_080048b0, 0x080048b0
	.set sub_08007308, 0x08007308
	.set sub_08017dd4, 0x08017dd4
	.set sub_080196c4, 0x080196c4
	.set sub_080198dc, 0x080198dc
	.set sub_08019944, 0x08019944
	.set sub_08019bac, 0x08019bac
	.set sub_08077008, 0x08077008
	.set sub_0808a5d0, 0x0808a5d0
	.global UiText_BuildRenderEntries
	.global Func_08018038
	.thumb_func
UiText_BuildRenderEntries:
Func_08018038:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #132
	str	r0, [sp, #48]
	ldr	r0, [pc, #292]
	mov	fp, r1
	ldr	r1, [r0, #0]
	movs	r2, #1
	movs	r3, #0
	ldr	r5, [pc, #284]
	str	r1, [sp, #44]
	str	r2, [sp, #40]
	str	r3, [sp, #36]
	adds	r3, r1, r5
	ldrh	r3, [r3, #0]
	str	r2, [sp, #20]
	movs	r2, #235
	str	r3, [sp, #32]
	adds	r6, r3, #0
	lsls	r2, r2, #4
	movs	r5, #1
	ldr	r3, [sp, #48]
	mov	sl, r0
	movs	r7, #0
	movs	r0, #0
	adds	r1, r1, r2
	negs	r5, r5
	str	r0, [sp, #28]
	str	r0, [sp, #24]
	str	r7, [sp, #52]
	mov	r8, r1
	str	r0, [sp, #16]
	cmp	r3, r5
	bne.n	.L_08018092
	ldr	r0, [sp, #44]
	ldr	r1, [pc, #236]
	adds	r3, r0, r1
	ldrh	r3, [r3, #0]
	str	r3, [sp, #32]
	b.n	.L_0801865a
.L_08018092:
	ldr	r5, [pc, #232]
	movs	r0, #50
	adds	r1, r5, #0
	bl	sub_080048b0
	movs	r2, #132
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	adds	r1, r0, #0
	ldr	r3, [pc, #216]
	ldr	r0, [pc, #220]
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r3, sl
	add	r2, sp, #56
	adds	r3, #140
	ldr	r3, [r3, #0]
	mov	sl, r2
	mov	r0, sl
	ldr	r1, [sp, #48]
	mov	r9, r3
	bl	sub_08019bac
	mov	r3, sp
	adds	r3, #84
	str	r3, [sp, #12]
.L_080180c8:
	mov	r0, sl
	bl	sub_08007308
	adds	r5, r7, #0
	adds	r7, r0, #0
	cmp	r7, #255
	bls.n	.L_080180d8
	movs	r7, #64
.L_080180d8:
	ldr	r0, [sp, #16]
	cmp	r0, #0
	beq.n	.L_08018188
	cmp	r7, #31
	bls.n	.L_080180e4
	b.n	.L_08018614
.L_080180e4:
	cmp	r7, #18
	beq.n	.L_0801815e
	cmp	r7, #18
	bhi.n	.L_0801810e
	cmp	r7, #9
	bhi.n	.L_08018102
	cmp	r7, #8
	bcs.n	.L_0801815e
	cmp	r7, #1
	beq.n	.L_08018166
	cmp	r7, #1
	bcc.n	.L_0801813c
	cmp	r7, #2
	beq.n	.L_0801813c
	b.n	.L_08018614
.L_08018102:
	cmp	r7, #16
	bne.n	.L_08018108
	b.n	.L_08018614
.L_08018108:
	cmp	r7, #17
	beq.n	.L_0801815e
	b.n	.L_08018614
.L_0801810e:
	cmp	r7, #22
	beq.n	.L_08018142
	cmp	r7, #22
	bhi.n	.L_08018128
	cmp	r7, #20
	beq.n	.L_08018146
	cmp	r7, #20
	bhi.n	.L_08018150
	mov	r0, sl
	bl	sub_08007308
	movs	r0, #3
	b.n	.L_08018156
.L_08018128:
	cmp	r7, #29
	beq.n	.L_0801815e
	cmp	r7, #29
	bhi.n	.L_08018136
	cmp	r7, #23
	beq.n	.L_08018154
	b.n	.L_08018614
.L_08018136:
	cmp	r7, #30
	beq.n	.L_0801813c
	b.n	.L_08018614
.L_0801813c:
	movs	r1, #0
	str	r1, [sp, #20]
	b.n	.L_08018614
.L_08018142:
	movs	r0, #5
	b.n	.L_08018156
.L_08018146:
	mov	r0, sl
	bl	sub_08007308
	movs	r0, #2
	b.n	.L_08018156
.L_08018150:
	movs	r0, #4
	b.n	.L_08018156
.L_08018154:
	movs	r0, #6
.L_08018156:
	mov	r1, fp
	bl	sub_08019944
	b.n	.L_08018614
.L_0801815e:
	mov	r0, sl
	bl	sub_08007308
	b.n	.L_08018614
.L_08018166:
	movs	r2, #0
	str	r2, [sp, #20]
	movs	r7, #2
	b.n	.L_08018614
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x000012b2
	.4byte 0x000012b4
	.4byte 0x00000140
	.4byte 0x040000d4
	.2byte 0x5430
	.2byte 0x0801
.L_08018188:
	ldr	r0, [sp, #44]
	ldr	r1, [pc, #68]
	adds	r3, r0, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080181b0
	ldr	r2, [sp, #40]
	cmp	r2, #0
	bne.n	.L_080181b0
	cmp	r7, #222
	beq.n	.L_080181b0
	cmp	r7, #223
	beq.n	.L_080181b0
	ldr	r3, [pc, #40]
	lsls	r2, r6, #1
	mov	r0, r8
	ldr	r1, [pc, #40]
	adds	r6, #1
	strh	r3, [r2, r0]
	ands	r6, r1
.L_080181b0:
	ldr	r2, [sp, #44]
	ldr	r0, [pc, #36]
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0801820e
	ldr	r1, [sp, #40]
	cmp	r1, #0
	bne.n	.L_0801820e
	cmp	r7, #222
	beq.n	.L_0801820e
	cmp	r7, #223
	beq.n	.L_0801820e
	b.n	.L_080181dc
	.4byte 0x00000005
	.4byte 0x000012fa
	.4byte 0x000001ff
	.2byte 0x12fb
	.2byte 0x0000
.L_080181dc:
	movs	r2, #128
	lsls	r2, r2, #1
	cmp	r5, r2
	bhi.n	.L_0801820e
	cmp	r5, #127
	bls.n	.L_0801820e
	cmp	r5, #222
	beq.n	.L_0801820e
	cmp	r5, #223
	beq.n	.L_0801820e
	cmp	r5, #32
	beq.n	.L_0801820e
	cmp	r5, #165
	beq.n	.L_0801820e
	cmp	r5, #161
	beq.n	.L_0801820e
	cmp	r5, #164
	beq.n	.L_0801820e
	ldr	r3, [pc, #40]
	lsls	r2, r6, #1
	mov	r5, r8
	ldr	r0, [pc, #40]
	adds	r6, #1
	strh	r3, [r2, r5]
	ands	r6, r0
.L_0801820e:
	cmp	r7, #31
	bls.n	.L_0801828c
	ldr	r1, [sp, #44]
	ldr	r2, [pc, #28]
	adds	r3, r1, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08018268
	cmp	r7, #32
	beq.n	.L_08018228
	ldr	r3, [sp, #28]
	cmp	r3, #10
	bls.n	.L_08018268
.L_08018228:
	ldr	r0, [pc, #4]
	b.n	.L_08018238
	.4byte 0x000000de
	.4byte 0x000001ff
	.2byte 0x12fa
	.2byte 0x0000
.L_08018238:
	lsls	r3, r6, #1
	ldr	r2, [pc, #40]
	adds	r6, #1
	ands	r6, r0
	mov	r5, r8
	strh	r2, [r3, r5]
	lsls	r3, r6, #1
	adds	r6, #1
	ands	r6, r0
	mov	r1, r8
	strh	r2, [r3, r1]
	lsls	r3, r6, #1
	strh	r2, [r3, r5]
	adds	r6, #1
	ldr	r1, [sp, #28]
	ands	r6, r0
	movs	r0, #1
	str	r0, [sp, #16]
	cmp	r1, #10
	bls.n	.L_08018268
	movs	r7, #32
	b.n	.L_08018268
	.2byte 0x002e
	.2byte 0x0000
.L_08018268:
	cmp	r7, #34
	bne.n	.L_0801827a
	ldr	r2, [sp, #36]
	movs	r3, #1
	eors	r2, r3
	str	r2, [sp, #36]
	cmp	r2, #0
	beq.n	.L_0801827a
	movs	r7, #142
.L_0801827a:
	ldr	r0, [pc, #752]
	lsls	r3, r6, #1
	mov	r5, r8
	adds	r6, #1
	movs	r1, #0
	strh	r7, [r3, r5]
	ands	r6, r0
	str	r1, [sp, #40]
	b.n	.L_08018614
.L_0801828c:
	cmp	r7, #20
	bne.n	.L_08018292
	b.n	.L_080183d8
.L_08018292:
	cmp	r7, #20
	bhi.n	.L_080182d2
	cmp	r7, #9
	bhi.n	.L_080182b4
	cmp	r7, #8
	bcs.n	.L_08018320
	cmp	r7, #1
	bne.n	.L_080182a4
	b.n	.L_080185f0
.L_080182a4:
	cmp	r7, #1
	bcc.n	.L_0801831a
	cmp	r7, #2
	beq.n	.L_0801831a
	cmp	r7, #3
	bne.n	.L_080182b2
	b.n	.L_080185f0
.L_080182b2:
	b.n	.L_080185f4
.L_080182b4:
	cmp	r7, #17
	bne.n	.L_080182ba
	b.n	.L_080184e4
.L_080182ba:
	cmp	r7, #17
	bhi.n	.L_080182c6
	cmp	r7, #16
	bne.n	.L_080182c4
	b.n	.L_08018486
.L_080182c4:
	b.n	.L_080185f4
.L_080182c6:
	cmp	r7, #18
	bne.n	.L_080182cc
	b.n	.L_080184aa
.L_080182cc:
	cmp	r7, #19
	beq.n	.L_080183a6
	b.n	.L_080185f4
.L_080182d2:
	cmp	r7, #25
	bne.n	.L_080182d8
	b.n	.L_08018588
.L_080182d8:
	cmp	r7, #25
	bhi.n	.L_080182f4
	cmp	r7, #22
	beq.n	.L_08018344
	cmp	r7, #22
	bcs.n	.L_080182e6
	b.n	.L_0801840e
.L_080182e6:
	cmp	r7, #23
	bne.n	.L_080182ec
	b.n	.L_08018448
.L_080182ec:
	cmp	r7, #24
	bne.n	.L_080182f2
	b.n	.L_08018546
.L_080182f2:
	b.n	.L_080185f4
.L_080182f4:
	cmp	r7, #29
	beq.n	.L_08018320
	cmp	r7, #29
	bhi.n	.L_0801830a
	cmp	r7, #26
	bne.n	.L_08018302
	b.n	.L_0801851e
.L_08018302:
	cmp	r7, #27
	bne.n	.L_08018308
	b.n	.L_080185c0
.L_08018308:
	b.n	.L_080185f4
.L_0801830a:
	cmp	r7, #30
	beq.n	.L_0801831a
	movs	r2, #1
	negs	r2, r2
	cmp	r7, r2
	bne.n	.L_08018318
	b.n	.L_08018614
.L_08018318:
	b.n	.L_080185f4
.L_0801831a:
	movs	r3, #0
	str	r3, [sp, #20]
	b.n	.L_08018614
.L_08018320:
	ldr	r0, [pc, #584]
	lsls	r3, r6, #1
	mov	r5, r8
	adds	r6, #1
	ands	r6, r0
	strh	r7, [r3, r5]
	mov	r0, sl
	bl	sub_08007308
	ldr	r1, [pc, #572]
	lsls	r3, r6, #1
	adds	r0, r0, r1
	mov	r2, r8
	strh	r0, [r3, r2]
	ldr	r3, [pc, #556]
	adds	r6, #1
	ands	r6, r3
	b.n	.L_08018614
.L_08018344:
	mov	r1, fp
	movs	r0, #5
	bl	sub_08019944
	adds	r1, r0, #0
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_08018356
	negs	r3, r1
.L_08018356:
	movs	r5, #1
	str	r5, [sp, #24]
	cmp	r3, #1
	bgt.n	.L_08018362
	movs	r0, #0
	str	r0, [sp, #24]
.L_08018362:
	add	r5, sp, #68
	adds	r0, r5, #0
	movs	r2, #0
	bl	sub_08017dd4
	subs	r4, r0, r5
	cmp	r4, #16
	bne.n	.L_08018374
	b.n	.L_08018614
.L_08018374:
	ldrb	r3, [r5, r4]
	cmp	r3, #0
	bne.n	.L_0801837c
	b.n	.L_08018614
.L_0801837c:
	ldr	r1, [pc, #492]
	adds	r0, r4, r5
	mov	ip, r1
	adds	r1, r0, #0
.L_08018384:
	ldrb	r3, [r1, #0]
	lsls	r2, r6, #1
	mov	r5, r8
	strh	r3, [r2, r5]
	adds	r6, #1
	mov	r2, ip
	adds	r4, #1
	adds	r1, #1
	ands	r6, r2
	cmp	r4, #16
	bne.n	.L_0801839c
	b.n	.L_08018614
.L_0801839c:
	adds	r0, #1
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	bne.n	.L_08018384
	b.n	.L_08018614
.L_080183a6:
	mov	r0, sl
	bl	sub_08007308
	mov	r1, fp
	subs	r5, r0, #1
	movs	r0, #3
	bl	sub_08019944
	adds	r2, r0, #0
	ldr	r0, [pc, #440]
	ldr	r1, [sp, #12]
	adds	r0, r2, r0
	movs	r2, #24
	bl	sub_080196c4
	ldr	r3, [sp, #24]
	str	r3, [sp, #4]
	add	r3, sp, #52
	str	r3, [sp, #8]
	adds	r2, r6, #0
	movs	r0, #0
	ldr	r1, [sp, #12]
	mov	r3, r8
	str	r5, [sp, #0]
	b.n	.L_08018516
.L_080183d8:
	mov	r0, sl
	bl	sub_08007308
	mov	r1, fp
	subs	r5, r0, #1
	movs	r0, #2
	bl	sub_08019944
	adds	r2, r0, #0
	ldr	r0, [pc, #384]
	ands	r2, r0
	ldr	r0, [pc, #392]
	ldr	r1, [sp, #12]
	adds	r0, r2, r0
	movs	r2, #24
	bl	sub_080196c4
	ldr	r1, [sp, #24]
	add	r3, sp, #52
	str	r1, [sp, #4]
	str	r3, [sp, #8]
	adds	r2, r6, #0
	movs	r0, #0
	ldr	r1, [sp, #12]
	mov	r3, r8
	str	r5, [sp, #0]
	b.n	.L_08018516
.L_0801840e:
	mov	r1, fp
	movs	r0, #4
	bl	sub_08019944
	adds	r2, r0, #0
	ldr	r0, [pc, #352]
	ldr	r1, [sp, #12]
	adds	r0, r2, r0
	movs	r2, #24
	bl	sub_080196c4
	ldr	r1, [sp, #12]
	ldrh	r2, [r1, #0]
	adds	r3, r2, #0
	adds	r0, r6, #0
	cmp	r3, #0
	beq.n	.L_0801851a
	ldr	r4, [pc, #312]
.L_08018432:
	lsls	r3, r0, #1
	mov	r5, r8
	strh	r2, [r3, r5]
	adds	r1, #2
	ldrh	r2, [r1, #0]
	adds	r0, #1
	adds	r3, r2, #0
	ands	r0, r4
	cmp	r3, #0
	bne.n	.L_08018432
	b.n	.L_0801851a
.L_08018448:
	mov	r1, fp
	movs	r0, #6
	bl	sub_08019944
	movs	r1, #1
	bl	sub_0808a5d0
	ldr	r3, [pc, #296]
	ldr	r1, [sp, #12]
	adds	r0, r0, r3
	movs	r2, #24
	bl	sub_080196c4
	ldr	r1, [sp, #12]
	ldrh	r2, [r1, #0]
	adds	r3, r2, #0
	adds	r0, r6, #0
	cmp	r3, #0
	beq.n	.L_0801851a
	ldr	r4, [pc, #252]
.L_08018470:
	lsls	r3, r0, #1
	mov	r5, r8
	strh	r2, [r3, r5]
	adds	r1, #2
	ldrh	r2, [r1, #0]
	adds	r0, #1
	adds	r3, r2, #0
	ands	r0, r4
	cmp	r3, #0
	bne.n	.L_08018470
	b.n	.L_0801851a
.L_08018486:
	ldr	r3, [pc, #252]
	movs	r0, #250
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl	sub_08077008
	add	r1, sp, #84
	adds	r2, r1, #0
	movs	r4, #0
.L_0801849a:
	ldrb	r3, [r0, #0]
	adds	r4, #1
	strh	r3, [r2, #0]
	adds	r0, #1
	adds	r2, #2
	cmp	r4, #14
	bls.n	.L_0801849a
	b.n	.L_08018506
.L_080184aa:
	mov	r0, sl
	bl	sub_08007308
	mov	r1, fp
	subs	r5, r0, #1
	movs	r0, #1
	bl	sub_08019944
	bl	sub_08077008
	add	r1, sp, #84
	adds	r2, r1, #0
	movs	r4, #0
.L_080184c4:
	ldrb	r3, [r0, #0]
	adds	r4, #1
	strh	r3, [r2, #0]
	adds	r0, #1
	adds	r2, #2
	cmp	r4, #14
	bls.n	.L_080184c4
	ldr	r2, [sp, #24]
	add	r3, sp, #52
	str	r2, [sp, #4]
	str	r3, [sp, #8]
	adds	r2, r6, #0
	movs	r0, #0
	mov	r3, r8
	str	r5, [sp, #0]
	b.n	.L_08018516
.L_080184e4:
	mov	r0, sl
	bl	sub_08007308
	subs	r2, r0, #1
	adds	r0, r2, #0
	bl	sub_08077008
	add	r1, sp, #84
	adds	r2, r1, #0
	movs	r4, #0
.L_080184f8:
	ldrb	r3, [r0, #0]
	adds	r4, #1
	strh	r3, [r2, #0]
	adds	r0, #1
	adds	r2, #2
	cmp	r4, #14
	bls.n	.L_080184f8
.L_08018506:
	movs	r3, #0
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	add	r3, sp, #52
	str	r3, [sp, #8]
	adds	r2, r6, #0
	movs	r0, #0
	mov	r3, r8
.L_08018516:
	bl	sub_08017e88
.L_0801851a:
	adds	r6, r0, #0
	b.n	.L_08018614
.L_0801851e:
	mov	r0, sl
	bl	sub_08007308
	subs	r0, #1
	lsls	r0, r0, #1
	ldr	r1, [pc, #64]
	lsls	r3, r6, #1
	adds	r2, r0, #0
	adds	r6, #1
	adds	r2, #128
	ands	r6, r1
	mov	r5, r8
	strh	r2, [r3, r5]
	adds	r0, #129
	lsls	r3, r6, #1
	mov	r2, r8
	adds	r6, #1
	strh	r0, [r3, r2]
	ands	r6, r1
	b.n	.L_08018614
.L_08018546:
	ldr	r3, [pc, #28]
	ldr	r0, [pc, #32]
	lsls	r2, r6, #1
	mov	r5, r8
	adds	r6, #1
	strh	r3, [r2, r5]
	ands	r6, r0
	ldr	r3, [pc, #16]
	lsls	r2, r6, #1
	mov	r1, r8
	adds	r6, #1
	strh	r3, [r2, r1]
	ands	r6, r0
	b.n	.L_08018614
	movs	r0, r0
	.4byte 0x0000008f
	.4byte 0x0000002d
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000741
	.4byte 0x00000182
	.4byte 0x00000333
	.4byte 0x0000099b
	.2byte 0x0240
	.2byte 0x0200
.L_08018588:
	ldr	r2, [sp, #24]
	cmp	r2, #0
	beq.n	.L_08018614
	ldr	r3, [sp, #52]
	cmp	r3, #0
	beq.n	.L_080185a2
	ldr	r3, [pc, #28]
	lsls	r2, r6, #1
	mov	r5, r8
	ldr	r0, [pc, #32]
	adds	r6, #1
	strh	r3, [r2, r5]
	ands	r6, r0
.L_080185a2:
	ldr	r3, [pc, #20]
	lsls	r2, r6, #1
	mov	r1, r8
	strh	r3, [r2, r1]
	ldr	r2, [pc, #16]
	adds	r6, #1
	ands	r6, r2
	b.n	.L_08018614
	movs	r0, r0
	.4byte 0x00000065
	.4byte 0x00000073
	.2byte 0x01ff
	.2byte 0x0000
.L_080185c0:
	ldr	r2, [pc, #32]
	lsls	r3, r6, #1
	mov	r5, r8
	strh	r2, [r3, r5]
	ldr	r1, [pc, #32]
	ldr	r3, [sp, #52]
	adds	r6, #1
	ands	r6, r1
	cmp	r3, #0
	bne.n	.L_08018614
	ldr	r3, [pc, #16]
	lsls	r2, r6, #1
	mov	r0, r8
	adds	r6, #1
	strh	r3, [r2, r0]
	ands	r6, r1
	b.n	.L_08018614
	movs	r0, r0
	.4byte 0x00000027
	.4byte 0x00000073
	.2byte 0x01ff
	.2byte 0x0000
.L_080185f0:
	movs	r1, #1
	str	r1, [sp, #40]
.L_080185f4:
	lsls	r3, r6, #1
	mov	r2, r8
	strh	r7, [r3, r2]
	ldr	r3, [pc, #16]
	adds	r6, #1
	ands	r6, r3
	cmp	r7, #115
	beq.n	.L_08018608
	cmp	r7, #83
	bne.n	.L_08018610
.L_08018608:
	movs	r3, #1
	b.n	.L_08018612
	.2byte 0x01ff
	.2byte 0x0000
.L_08018610:
	movs	r3, #0
.L_08018612:
	str	r3, [sp, #52]
.L_08018614:
	ldr	r5, [sp, #28]
	ldr	r0, [sp, #20]
	adds	r5, #1
	str	r5, [sp, #28]
	cmp	r0, #0
	beq.n	.L_08018628
	ldr	r1, [pc, #72]
	cmp	r5, r1
	bhi.n	.L_08018628
	b.n	.L_080180c8
.L_08018628:
	ldr	r1, [pc, #64]
	lsls	r3, r6, #1
	mov	r2, r8
	adds	r6, #1
	strh	r7, [r3, r2]
	ands	r6, r1
	ldr	r3, [pc, #48]
	lsls	r2, r6, #1
	mov	r5, r8
	strh	r3, [r2, r5]
	adds	r3, r6, #1
	ands	r3, r1
	ldr	r0, [sp, #44]
	ldr	r1, [pc, #44]
	adds	r2, r0, r1
	strh	r3, [r2, #0]
	movs	r0, #50
	bl	sub_08002dd8
	ldr	r5, [pc, #36]
	ldr	r2, [sp, #44]
	add	r0, sp, #32
	ldrh	r0, [r0, #0]
	adds	r3, r2, r5
	strh	r0, [r3, #0]
.L_0801865a:
	mov	r1, fp
	cmp	r1, #0
	beq.n	.L_08018678
	bl	sub_080198dc
	b.n	.L_08018678
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x000001ff
	.4byte 0x000012b2
	.2byte 0x12b4
	.2byte 0x0000
.L_08018678:
	ldr	r0, [sp, #32]
	add	sp, #132
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
