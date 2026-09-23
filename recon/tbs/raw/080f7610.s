.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_08002304, 0x08002304
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_08005340, 0x08005340
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_08015000, 0x08015000
	.set sub_08015010, 0x08015010
	.set sub_08015080, 0x08015080
	.set sub_080772e0, 0x080772e0
	.set sub_080c9000, 0x080c9000
	.set sub_080f6038, 0x080f6038
	.set sub_080f6148, 0x080f6148
	.set sub_080f61e8, 0x080f61e8
	.set sub_080f62b8, 0x080f62b8
	.set sub_080f731c, 0x080f731c
	.set sub_080f75dc, 0x080f75dc
	.set sub_080f75e6, 0x080f75e6
	.global Unnamed_080f7610
	.global Func_080f7610
	.thumb_func
Unnamed_080f7610:
Func_080f7610:
	push	{lr}
	lsls	r0, r0, #24
	lsls	r4, r4, #5
	lsls	r0, r0, #12
	cmp	r5, #0
	lsls	r0, r0, #24
	adds	r0, #0
	lsls	r0, r0, #24
	mov	r7, r8
	adds	r3, r7, r5
	strh	r2, [r3, #0]
	movs	r3, #1
	add	sl, r3
	movs	r7, #2
	mov	r3, sl
	adds	r2, #1
	add	r8, r7
	cmp	r3, #32
	bne.n	sub_080f75e6
	adds	r6, #1
	adds	r4, #1
	adds	r0, #32
	cmp	r6, #20
	bne.n	sub_080f75dc
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #64]
	movs	r1, #0
	strh	r1, [r3, #0]
	strh	r1, [r3, #2]
	strh	r1, [r3, #4]
	strh	r1, [r3, #6]
	strh	r1, [r3, #8]
	strh	r1, [r3, #10]
	ldr	r3, [pc, #28]
	adds	r2, #60
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #24]
	adds	r2, #6
	strh	r3, [r2, #0]
	ldr	r3, [pc, #24]
	adds	r2, #2
	b.n	.L_080f7694
	.4byte 0x00000509
	.4byte 0x00000680
	.4byte 0x00003737
	.4byte 0x00002727
	.4byte 0x00003f44
	.4byte 0x00001010
	.4byte 0x0400000a
	.2byte 0x1ad0
	.2byte 0x0300
.L_080f7694:
	strh	r3, [r2, #0]
	ldr	r3, [pc, #76]
	ldr	r2, [pc, #52]
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r1, [r3, #0]
	subs	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	ldr	r0, [pc, #60]
	ldr	r3, [pc, #40]
	ldr	r4, [pc, #60]
	strh	r3, [r0, #0]
	ldr	r2, [pc, #36]
	ldr	r3, [pc, #40]
	strh	r3, [r4, #0]
	strh	r2, [r0, #0]
	ldr	r3, [pc, #52]
	ldr	r0, [pc, #36]
	strh	r0, [r4, #0]
	strh	r2, [r3, #0]
	adds	r3, #4
	strh	r0, [r3, #0]
	ldr	r7, [sp, #36]
	ldr	r2, [sp, #32]
	movs	r0, #128
	lsls	r0, r0, #2
	b.n	.L_080f76f4
	movs	r0, r0
	.4byte 0x0000ff60
	.4byte 0x000028c8
	.4byte 0x000000f0
	.4byte 0x00001878
	.4byte 0x000000a0
	.4byte 0x04000014
	.4byte 0x04000040
	.4byte 0x04000044
	.2byte 0x0042
	.2byte 0x0400
.L_080f76f4:
	adds	r0, r7, r0
	adds	r2, #140
	str	r0, [sp, #24]
	str	r2, [sp, #20]
	str	r1, [r2, #0]
	ldr	r3, [sp, #32]
	adds	r3, #144
	str	r1, [r3, #0]
	ldr	r3, [sp, #32]
	ldr	r0, [pc, #96]
	adds	r3, #148
	str	r1, [r3, #0]
	adds	r3, r7, r0
	str	r1, [r3, #0]
	ldr	r2, [sp, #32]
	adds	r2, #168
	str	r2, [sp, #16]
	str	r1, [r2, #0]
	ldr	r0, [pc, #80]
	bl	sub_08002f40
	movs	r1, #160
	ldr	r3, [pc, #76]
	lsls	r1, r1, #19
	ldr	r2, [pc, #76]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r0, [pc, #68]
	bl	sub_08002f40
	ldr	r3, [pc, #48]
	adds	r4, r0, #0
	ldr	r1, [pc, #60]
	ldr	r2, [pc, #64]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #240
	lsls	r3, r3, #1
	adds	r4, r4, r3
	adds	r0, r4, #0
	ldr	r1, [pc, #52]
	bl	sub_08005340
	ldr	r3, [pc, #24]
	ldr	r0, [pc, #44]
	ldr	r1, [pc, #48]
	ldr	r2, [pc, #48]
	b.n	.L_080f7794
	.4byte 0x00002f8b
	.4byte 0x00005bf6
	.4byte 0x0000778c
	.4byte 0x0000008f
	.4byte 0x040000d4
	.4byte 0x84000020
	.4byte 0x05000080
	.4byte 0x00000040
	.4byte 0x05000200
	.4byte 0x84000078
	.4byte 0x02010000
	.4byte 0x06010000
	.2byte 0x1b30
	.2byte 0x8400
.L_080f7794:
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #156]
	bl	sub_08002f40
	ldr	r3, [pc, #156]
	adds	r4, r0, #0
	ldr	r1, [pc, #156]
	ldr	r2, [pc, #156]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r4, #32
	adds	r0, r4, #0
	ldr	r1, [pc, #152]
	bl	sub_08005340
	ldr	r3, [pc, #132]
	ldr	r0, [pc, #144]
	ldr	r1, [pc, #144]
	ldr	r2, [pc, #148]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bl	sub_08015000
	bl	sub_080f731c
	movs	r7, #0
	mov	sl, r7
	ldr	r7, [sp, #32]
.L_080f77ce:
	movs	r3, #8
	str	r3, [r7, #0]
	movs	r3, #0
	strb	r3, [r7, #25]
	movs	r3, #255
	strb	r3, [r7, #26]
	movs	r6, #0
	adds	r5, r7, #4
.L_080f77de:
	bl	sub_08004458
	movs	r1, #5
	bl	sub_08002304
	adds	r6, #1
	strb	r0, [r5, #0]
	adds	r5, #1
	cmp	r6, #21
	bne.n	.L_080f77de
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	adds	r7, #28
	cmp	r1, #5
	bne.n	.L_080f77ce
	ldr	r3, [sp, #32]
	movs	r2, #0
	add	r7, sp, #84
	mov	sl, r2
	adds	r4, r7, #0
	mov	r9, r3
.L_080f780a:
	movs	r0, #0
	mov	r8, r0
	movs	r5, #0
.L_080f7810:
	str	r4, [sp, #8]
	bl	sub_08004458
	movs	r1, #21
	bl	sub_08002304
	mov	r1, r8
	str	r0, [r5, r7]
	movs	r6, #0
	ldr	r4, [sp, #8]
	cmp	r1, #0
	beq.n	.L_080f786c
	ldr	r3, [r7, #0]
	cmp	r0, r3
	bne.n	.L_080f7854
	movs	r2, #1
	negs	r2, r2
	subs	r5, #4
	add	r8, r2
	b.n	.L_080f786c
	.4byte 0x00000041
	.4byte 0x040000d4
	.4byte 0x050003e0
	.4byte 0x84000008
	.4byte 0x02010000
	.4byte 0x06016e00
	.2byte 0x0480
	.2byte 0x8400
.L_080f7854:
	adds	r6, #1
	cmp	r6, r8
	beq.n	.L_080f786c
	lsls	r3, r6, #2
	ldr	r2, [r5, r4]
	ldr	r3, [r4, r3]
	cmp	r2, r3
	bne.n	.L_080f7854
	movs	r3, #1
	negs	r3, r3
	subs	r5, #4
	add	r8, r3
.L_080f786c:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #4
	cmp	r1, #8
	bne.n	.L_080f7810
	movs	r6, #0
	adds	r1, r7, #0
.L_080f787c:
	adds	r2, r6, #0
	cmp	r6, #5
	ble.n	.L_080f7884
	movs	r2, #5
.L_080f7884:
	ldmia	r1!, {r3}
	mov	r0, r9
	adds	r3, #4
	adds	r6, #1
	strb	r2, [r0, r3]
	cmp	r6, #8
	bne.n	.L_080f787c
	movs	r2, #1
	add	sl, r2
	movs	r1, #28
	mov	r3, sl
	add	r9, r1
	cmp	r3, #5
	bne.n	.L_080f780a
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #8
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	bl	sub_080c9000
	ldr	r5, [pc, #172]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r1, #8
	str	r3, [sp, #48]
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	str	r3, [sp, #0]
	bl	sub_080c9000
	adds	r5, #188
	ldr	r3, [r5, #0]
	mov	r7, sp
	adds	r7, #48
	str	r7, [sp, #12]
	movs	r1, #128
	str	r3, [r7, #4]
	ldr	r0, [sp, #40]
	ldr	r3, [pc, #136]
	lsls	r1, r1, #8
	movs	r2, #0
	bl	sub_080072f0
	ldr	r3, [pc, #132]
	ldr	r0, [sp, #40]
	ldr	r1, [pc, #132]
	ldr	r2, [pc, #132]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #160
	lsls	r0, r0, #19
	ldr	r1, [sp, #36]
	ldr	r2, [pc, #124]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r1, [sp, #24]
	ldr	r0, [pc, #120]
	ldr	r2, [pc, #116]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r5, #128
	lsls	r5, r5, #1
	ldr	r1, [pc, #108]
	ldr	r0, [sp, #24]
	movs	r2, #0
	adds	r3, r5, #0
	bl	sub_080f6038
	movs	r1, #160
	movs	r2, #0
	adds	r3, r5, #0
	lsls	r1, r1, #19
	ldr	r0, [sp, #36]
	bl	sub_080f6038
	movs	r2, #128
	ldr	r3, [pc, #52]
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r0, #228
	bl	sub_080772e0
	cmp	r0, #1
	bne.n	.L_080f7982
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #16
	movs	r2, #18
	movs	r3, #3
	movs	r0, #6
	bl	sub_08015010
	movs	r2, #153
	adds	r1, r0, #0
	ldr	r0, [sp, #32]
	lsls	r2, r2, #3
	adds	r3, r0, r2
	str	r1, [r3, #0]
	ldr	r0, [pc, #40]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08015080
	b.n	.L_080f7980
	.4byte 0x00003740
	.4byte 0x03001e50
	.4byte 0x03000168
	.4byte 0x040000d4
	.4byte 0x06003500
	.4byte 0x84002000
	.4byte 0x84000080
	.4byte 0x05000200
	.2byte 0x0909
	.2byte 0x0000
.L_080f7980:
	b.n	.L_080f79b8
.L_080f7982:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #16
	movs	r2, #26
	movs	r3, #4
	movs	r0, #2
	bl	sub_08015010
	movs	r7, #153
	ldr	r3, [sp, #32]
	ldr	r5, [pc, #596]
	lsls	r7, r7, #3
	adds	r1, r0, #0
	adds	r6, r3, r7
	adds	r0, r5, #0
	str	r1, [r6, #0]
	movs	r2, #0
	movs	r3, #0
	adds	r5, #1
	bl	sub_08015080
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #8
	bl	sub_08015080
.L_080f79b8:
	ldr	r0, [sp, #36]
	ldr	r1, [pc, #564]
	movs	r5, #144
	adds	r2, r0, r1
	movs	r3, #0
	lsls	r5, r5, #3
	str	r3, [r2, #0]
	adds	r1, r5, #0
	ldr	r0, [pc, #552]
	bl	sub_080041d8
	ldr	r0, [pc, #552]
	adds	r1, r5, #0
	bl	sub_080041d8
	ldr	r7, [sp, #20]
	ldr	r3, [r7, #0]
	movs	r2, #0
	mov	fp, r2
	cmp	r3, #10
	bne.n	.L_080f79e4
	b.n	.L_080f7d26
.L_080f79e4:
	mov	r0, fp
	cmp	r0, #16
	bgt.n	.L_080f7a0a
	movs	r6, #128
	lsls	r5, r0, #12
	lsls	r6, r6, #1
	ldr	r1, [pc, #520]
	adds	r2, r5, #0
	adds	r3, r6, #0
	ldr	r0, [sp, #24]
	bl	sub_080f6038
	movs	r1, #160
	ldr	r0, [sp, #36]
	lsls	r1, r1, #19
	adds	r2, r5, #0
	adds	r3, r6, #0
	bl	sub_080f6038
.L_080f7a0a:
	ldr	r1, [sp, #20]
	ldr	r3, [r1, #0]
	cmp	r3, #3
	beq.n	.L_080f7a14
	b.n	.L_080f7b98
.L_080f7a14:
	mov	r0, fp
	movs	r1, #80
	bl	sub_080022fc
	cmp	r0, #15
	bgt.n	.L_080f7a28
	ldr	r0, [pc, #476]
	bl	sub_080f61e8
	b.n	.L_080f7a52
.L_080f7a28:
	cmp	r0, #31
	bgt.n	.L_080f7a34
	ldr	r0, [pc, #468]
	bl	sub_080f61e8
	b.n	.L_080f7a52
.L_080f7a34:
	cmp	r0, #47
	bgt.n	.L_080f7a40
	ldr	r0, [pc, #460]
	bl	sub_080f61e8
	b.n	.L_080f7a52
.L_080f7a40:
	cmp	r0, #63
	bgt.n	.L_080f7a4c
	ldr	r0, [pc, #452]
	bl	sub_080f61e8
	b.n	.L_080f7a52
.L_080f7a4c:
	ldr	r0, [pc, #448]
	bl	sub_080f61e8
.L_080f7a52:
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #0]
	cmp	r3, #15
	bgt.n	.L_080f7a62
	bl	sub_080f6148
	ldr	r7, [sp, #16]
	ldr	r3, [r7, #0]
.L_080f7a62:
	cmp	r3, #16
	ble.n	.L_080f7b00
	movs	r3, #7
	mov	r0, fp
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_080f7b00
	bl	sub_08004458
	movs	r3, #127
	ands	r3, r0
	adds	r5, r3, #0
	bl	sub_08004458
	movs	r3, #31
	ands	r3, r0
	adds	r1, r3, #0
	mov	r2, fp
	adds	r5, #56
	adds	r1, #48
	cmp	r2, #0
	bge.n	.L_080f7a90
	adds	r2, #7
.L_080f7a90:
	movs	r3, #3
	asrs	r2, r2, #3
	ands	r2, r3
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r2, [pc, #376]
	lsls	r3, r3, #10
	adds	r7, r3, r2
	lsls	r5, r5, #16
	movs	r3, #0
	lsls	r1, r1, #16
	mov	r8, r3
	mov	r9, r5
	mov	sl, r1
.L_080f7aac:
	bl	sub_08004458
	movs	r5, #255
	ands	r5, r0
	bl	sub_08004458
	ldr	r3, [pc, #348]
	adds	r6, r0, #0
	ands	r6, r3
	mov	r1, sl
	mov	r0, r9
	str	r1, [r7, #4]
	str	r0, [r7, #0]
	adds	r0, r6, #0
	bl	sub_08002322
	adds	r5, #64
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	negs	r3, r3
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08004458
	movs	r3, #15
	ands	r3, r0
	adds	r3, #16
	str	r3, [r7, #24]
	movs	r2, #1
	movs	r3, #128
	add	r8, r2
	lsls	r3, r3, #1
	adds	r7, #28
	cmp	r8, r3
	bne.n	.L_080f7aac
.L_080f7b00:
	movs	r7, #0
	mov	r8, r7
	ldr	r7, [pc, #268]
.L_080f7b06:
	ldr	r0, [r7, #24]
	cmp	r0, #0
	ble.n	.L_080f7b8a
	ldr	r3, [r7, #0]
	ldr	r1, [pc, #268]
	subs	r0, #1
	str	r0, [r7, #24]
	cmp	r3, r1
	bhi.n	.L_080f7b5c
	ldr	r6, [r7, #4]
	ldr	r2, [pc, #260]
	cmp	r6, r2
	bgt.n	.L_080f7b5c
	cmp	r6, #0
	blt.n	.L_080f7b5c
	movs	r1, #12
	asrs	r5, r3, #16
	bl	sub_080022ec
	adds	r0, #1
	lsls	r4, r0, #1
	mov	r3, r8
	ldr	r1, [pc, #240]
	movs	r2, #1
	ands	r2, r3
	asrs	r6, r6, #16
	subs	r3, r4, #2
	ldrh	r1, [r1, r3]
	subs	r5, r5, r0
	ldr	r3, [sp, #44]
	subs	r6, r6, r0
	str	r4, [sp, #0]
	ldr	r0, [sp, #12]
	str	r4, [sp, #4]
	lsls	r2, r2, #2
	ldr	r4, [r2, r0]
	adds	r1, r3, r1
	ldr	r0, [sp, #40]
	adds	r3, r6, #0
	adds	r2, r5, #0
	bl	sub_080072f4
	ldr	r3, [r7, #0]
.L_080f7b5c:
	ldr	r2, [r7, #12]
	adds	r3, r3, r2
	str	r3, [r7, #0]
	ldr	r1, [r7, #16]
	ldr	r3, [r7, #4]
	adds	r3, r3, r1
	str	r3, [r7, #4]
	lsls	r3, r2, #4
	subs	r3, r3, r2
	lsls	r3, r3, #2
	cmp	r3, #0
	bge.n	.L_080f7b76
	adds	r3, #63
.L_080f7b76:
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	lsls	r3, r1, #4
	subs	r3, r3, r1
	lsls	r3, r3, #2
	cmp	r3, #0
	bge.n	.L_080f7b86
	adds	r3, #63
.L_080f7b86:
	asrs	r3, r3, #6
	str	r3, [r7, #16]
.L_080f7b8a:
	movs	r1, #1
	movs	r2, #128
	add	r8, r1
	lsls	r2, r2, #3
	adds	r7, #28
	cmp	r8, r2
	bne.n	.L_080f7b06
.L_080f7b98:
	ldr	r7, [sp, #20]
	ldr	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L_080f7ba6
	cmp	r3, #2
	beq.n	.L_080f7ba6
	b.n	.L_080f7d08
.L_080f7ba6:
	movs	r0, #0
	add	r5, sp, #56
	mov	r8, r0
	movs	r2, #0
	adds	r3, r5, #0
.L_080f7bb0:
	movs	r1, #1
	add	r8, r1
	mov	r7, r8
	stmia	r3!, {r2}
	cmp	r7, #7
	bne.n	.L_080f7bb0
	ldr	r0, [sp, #20]
	ldr	r3, [r0, #0]
	cmp	r3, #0
	bne.n	.L_080f7c28
	movs	r2, #1
	str	r2, [r5, #12]
	ldr	r1, [sp, #28]
	ldr	r3, [r1, #0]
	cmp	r3, #1
	ble.n	.L_080f7bd6
	str	r2, [r5, #16]
	str	r2, [r5, #8]
	ldr	r3, [r1, #0]
.L_080f7bd6:
	cmp	r3, #2
	ble.n	.L_080f7be2
	str	r2, [r5, #20]
	str	r2, [r5, #4]
	ldr	r7, [sp, #28]
	ldr	r3, [r7, #0]
.L_080f7be2:
	cmp	r3, #3
	ble.n	.L_080f7c4c
	str	r2, [r5, #24]
	str	r2, [r5, #0]
	b.n	.L_080f7c4c
	.4byte 0x00000908
	.4byte 0x00007824
	.4byte 0x080f6441
	.4byte 0x080f60a1
	.4byte 0x05000200
	.4byte 0x00000091
	.4byte 0x00000093
	.4byte 0x000000b4
	.4byte 0x000000a0
	.4byte 0x0000008f
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0x00ffffff
	.4byte 0x007fffff
	.2byte 0x86f8
	.2byte 0x080f
.L_080f7c28:
	mov	r3, fp
	mov	r0, r8
	ands	r3, r0
	cmp	r3, #3
	bgt.n	.L_080f7c4c
	ldr	r2, [sp, #32]
	movs	r1, #0
	mov	r8, r1
	adds	r0, r5, #0
	adds	r2, #172
.L_080f7c3c:
	ldmia	r2!, {r3}
	str	r3, [r1, r0]
	movs	r3, #1
	add	r8, r3
	mov	r7, r8
	adds	r1, #4
	cmp	r7, #7
	bne.n	.L_080f7c3c
.L_080f7c4c:
	movs	r0, #0
	mov	r8, r0
.L_080f7c50:
	movs	r2, #1
	mov	r1, r8
	eors	r2, r1
	negs	r3, r2
	orrs	r3, r2
	lsrs	r6, r3, #31
	movs	r3, #65
	subs	r6, r3, r6
	ldr	r3, [r5, #4]
	cmp	r3, #0
	beq.n	.L_080f7c76
	mov	r3, r8
	adds	r3, #19
	movs	r0, #20
	adds	r1, r3, #0
	movs	r2, #200
	str	r6, [sp, #0]
	bl	sub_080f62b8
.L_080f7c76:
	ldr	r3, [r5, #8]
	cmp	r3, #0
	beq.n	.L_080f7c8c
	mov	r3, r8
	adds	r3, #35
	movs	r0, #28
	adds	r1, r3, #0
	movs	r2, #200
	str	r6, [sp, #0]
	bl	sub_080f62b8
.L_080f7c8c:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	beq.n	.L_080f7ca2
	mov	r3, r8
	adds	r3, #51
	movs	r0, #20
	adds	r1, r3, #0
	movs	r2, #200
	str	r6, [sp, #0]
	bl	sub_080f62b8
.L_080f7ca2:
	ldr	r3, [r5, #16]
	cmp	r3, #0
	beq.n	.L_080f7cb8
	mov	r3, r8
	adds	r3, #67
	movs	r0, #28
	adds	r1, r3, #0
	movs	r2, #200
	str	r6, [sp, #0]
	bl	sub_080f62b8
.L_080f7cb8:
	ldr	r3, [r5, #20]
	cmp	r3, #0
	beq.n	.L_080f7cce
	mov	r3, r8
	adds	r3, #83
	movs	r0, #20
	adds	r1, r3, #0
	movs	r2, #200
	str	r6, [sp, #0]
	bl	sub_080f62b8
.L_080f7cce:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_080f7ce6
	mov	r1, r8
	mov	r3, r8
	adds	r1, #5
	adds	r3, #91
	movs	r0, #28
	movs	r2, #200
	str	r6, [sp, #0]
	bl	sub_080f62b8
.L_080f7ce6:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	beq.n	.L_080f7cfe
	mov	r1, r8
	mov	r3, r8
	adds	r1, #97
	adds	r3, #11
	movs	r0, #28
	movs	r2, #200
	str	r6, [sp, #0]
	bl	sub_080f62b8
.L_080f7cfe:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #3
	bne.n	.L_080f7c50
.L_080f7d08:
	ldr	r7, [sp, #36]
	ldr	r0, [pc, #152]
	movs	r2, #1
	adds	r3, r7, r0
	str	r2, [r3, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [sp, #20]
	ldr	r3, [r2, #0]
	movs	r1, #1
	add	fp, r1
	cmp	r3, #10
	beq.n	.L_080f7d26
	b.n	.L_080f79e4
.L_080f7d26:
	movs	r3, #0
	movs	r6, #128
	mov	r8, r3
	lsls	r6, r6, #1
.L_080f7d2e:
	mov	r7, r8
	movs	r5, #128
	lsls	r3, r7, #12
	lsls	r5, r5, #9
	subs	r5, r5, r3
	ldr	r0, [sp, #24]
	ldr	r1, [pc, #108]
	adds	r2, r5, #0
	adds	r3, r6, #0
	bl	sub_080f6038
	movs	r1, #160
	lsls	r1, r1, #19
	ldr	r0, [sp, #36]
	adds	r2, r5, #0
	adds	r3, r6, #0
	bl	sub_080f6038
	movs	r0, #1
	bl	sub_080030f8
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	cmp	r1, #17
	bne.n	.L_080f7d2e
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r0, [pc, #60]
	bl	sub_08004278
	ldr	r0, [pc, #56]
	bl	sub_08004278
	movs	r0, #45
	bl	sub_08002dd8
	movs	r0, #40
	bl	sub_08002dd8
	movs	r0, #39
	bl	sub_08002dd8
	movs	r0, #41
	bl	sub_08002dd8
	add	sp, #116
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00007824
	.4byte 0x05000200
	.4byte 0x080f60a1
	.4byte 0x080f6441
