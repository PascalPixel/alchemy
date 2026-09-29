.syntax unified
	.thumb
	.section .text.x0200a47c,"ax",%progbits
	.balign 4
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #132
	movs	r2, #0
	str	r2, [sp, #16]
	str	r2, [sp, #12]
	bl 0x0200ce94
	mov	r2, sp
	movs	r3, #0
	adds	r2, #20
	mov	r8, r3
	str	r2, [sp, #8]
	movs	r3, #10
	mov	fp, r3
.L_020024a4:
	mov	r0, fp
	bl 0x0200ceac
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	asrs	r3, r3, #20
	mov	r9, r3
	cmp	r3, #13
	bne.n	.L_0200250a
	ldr	r3, [r6, #16]
	asrs	r3, r3, #20
	mov	sl, r3
	cmp	r3, #7
	bne.n	.L_0200250a
	movs	r5, #128
	lsls	r5, r5, #2
	add	r5, r8
	adds	r0, r5, #0
	bl 0x0200ce74
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L_0200250a
	adds	r0, r6, #0
	bl OverlayObject_WaitUntilIdle
	adds	r0, r5, #0
	bl 0x0200ce7c
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #2
	orrs	r2, r3
	adds	r3, r6, #0
	adds	r3, #89
	strb	r2, [r1, #0]
	strb	r7, [r3, #0]
	subs	r3, #4
	strb	r7, [r3, #0]
	mov	r2, r9
	mov	r3, sl
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #4
	movs	r1, #19
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ce34
	b.n	.L_020026ca
.L_0200250a:
	ldr	r3, [r6, #80]
	ldrb	r2, [r3, #9]
	movs	r3, #12
	ands	r3, r2
	cmp	r3, #12
	bne.n	.L_020025b0
	movs	r7, #128
	lsls	r7, r7, #2
	add	r7, r8
	adds	r0, r7, #0
	bl 0x0200ce74
	cmp	r0, #0
	bne.n	.L_020025b0
	mov	r0, fp
	movs	r1, #1
	bl 0x0200cf64
	ldr	r3, [r6, #16]
	movs	r5, #0
	asrs	r3, r3, #20
	str	r5, [r6, #68]
	cmp	r3, #12
	bgt.n	.L_0200255a
	movs	r2, #224
	movs	r1, #0
	lsls	r2, r2, #16
	movs	r3, #253
	ldr	r0, [r6, #8]
	bl OverlayObject_SpawnWithMode14
	str	r0, [sp, #16]
	movs	r2, #240
	ldr	r0, [r6, #8]
	movs	r1, #0
	lsls	r2, r2, #16
	movs	r3, #253
	bl OverlayObject_SpawnWithMode14
	str	r0, [sp, #12]
.L_0200255a:
	adds	r0, r6, #0
	bl OverlayObject_WaitUntilIdle
	movs	r1, #0
	movs	r2, #0
	mov	r0, fp
	bl 0x0200cef4
	ldr	r0, [sp, #16]
	bl 0x0200cdec
	ldr	r0, [sp, #12]
	bl 0x0200cdec
	adds	r0, r7, #0
	bl 0x0200ce7c
	b.n	.L_020026ca
.L_0200257e:
	adds	r0, r5, #0
	adds	r0, #10
	bl 0x0200ceac
	ldr	r3, [r6, #8]
	ldr	r2, [sp, #8]
	str	r3, [r2, #8]
	ldr	r3, [r6, #12]
	str	r3, [r2, #12]
	ldr	r3, [r6, #16]
	str	r3, [r2, #16]
	ldr	r3, [r0, #8]
	str	r3, [r6, #8]
	ldr	r3, [r0, #12]
	str	r3, [r6, #12]
	ldr	r3, [r0, #16]
.L_0200259e:
	str	r3, [r6, #16]
	ldr	r3, [r2, #8]
	str	r3, [r0, #8]
	ldr	r3, [r2, #12]
	str	r3, [r0, #12]
	ldr	r3, [r2, #16]
	adds	r7, r5, #0
	str	r3, [r0, #16]
.L_020025ae:
	b.n	.L_020025f8
.L_020025b0:
	ldr	r3, [r6, #16]
	asrs	r3, r3, #20
	cmp	r3, #19
	beq.n	.L_020025ba
	b.n	.L_020026bc
.L_020025ba:
	movs	r0, #128
	lsls	r0, r0, #2
	add	r0, r8
	bl 0x0200ce74
	cmp	r0, #0
	bne.n	.L_020026bc
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r6, #60]
	adds	r3, r6, #0
	adds	r3, #85
	str	r0, [r6, #20]
	str	r0, [r6, #40]
	movs	r5, #0
	strb	r0, [r3, #0]
	adds	r3, #15
	strh	r0, [r3, #0]
	mov	r7, r8
	cmp	r5, r8
	bge.n	.L_020025f8
.L_020025e4:
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r0, r5, r3
	bl 0x0200ce74
	cmp	r0, #0
	beq.n	.L_0200257e
	adds	r5, #1
	cmp	r5, r8
	blt.n	.L_020025e4
.L_020025f8:
	adds	r5, r7, #0
	adds	r5, #10
	adds	r0, r5, #0
	bl 0x0200ceac
	movs	r3, #128
	lsls	r3, r3, #24
.L_02002606:
	str	r3, [r0, #60]
	adds	r3, r0, #0
	movs	r2, #0
	adds	r3, #85
	str	r2, [r0, #20]
	str	r2, [r0, #40]
	movs	r1, #192
	strb	r2, [r3, #0]
	movs	r0, #192
	adds	r3, #15
	strh	r2, [r3, #0]
	lsls	r1, r1, #7
	lsls	r0, r0, #10
	bl 0x0200cf7c
	bl 0x0200cf9c
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #128
	movs	r0, #136
	movs	r2, #172
	movs	r3, #1
	lsls	r1, r1, #12
	lsls	r2, r2, #17
	lsls	r0, r0, #16
	bl 0x0200cf84
	bl 0x0200cf8c
	adds	r0, r5, #0
	.2byte 0xf7ff
	.2byte 0xfec5
	adds	r0, r5, #0
	bl 0x0200ceac
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	cmp	r3, #6
	bne.n	.L_02002674
	movs	r0, #8
	bl 0x0200ceac
	adds	r0, #100
	ldrh	r3, [r0, #0]
	adds	r3, #1
	strh	r3, [r0, #0]
	movs	r0, #9
	bl 0x0200ceac
	adds	r0, #100
	ldrh	r3, [r0, #0]
	subs	r3, #1
	b.n	.L_0200268e
.L_02002674:
	movs	r0, #8
	bl 0x0200ceac
	adds	r0, #100
	ldrh	r3, [r0, #0]
	subs	r3, #1
	strh	r3, [r0, #0]
	movs	r0, #9
	bl 0x0200ceac
	adds	r0, #100
	ldrh	r3, [r0, #0]
	adds	r3, #1
.L_0200268e:
	strh	r3, [r0, #0]
	adds	r0, r5, #0
	bl 0x0200ceac
	ldr	r3, [pc, #72]
	str	r3, [r0, #108]
	movs	r0, #40
	bl VinasuHeya_LowerFloatingBlocks
	adds	r0, r5, #0
	bl 0x0200ceac
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	movs	r2, #128
	lsls	r2, r2, #2
	strb	r3, [r0, #0]
	adds	r0, r7, r2
	bl 0x0200ce7c
	b.n	.L_020026ca
.L_020026bc:
	movs	r3, #1
	add	r8, r3
.L_020026c0:
	mov	r2, r8
	add	fp, r3
	cmp	r2, #3
	bgt.n	.L_020026ca
	b.n	.L_020024a4
.L_020026ca:
	bl 0x0200ce9c
	add	sp, #132
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0xa2a5
	.2byte 0x0200
	.section .text.x0200ab14,"ax",%progbits
	.balign 4
	.global Scene_RunScene3c8SequenceA
	.thumb_func
Scene_RunScene3c8SequenceA:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	movs	r1, #0
	movs	r0, #0
	str	r1, [sp, #12]
	bl 0x0200ceac
	str	r0, [sp, #8]
	bl 0x0200ce94
	movs	r3, #5
	movs	r2, #48
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #69
	movs	r1, #48
	movs	r2, #4
	movs	r3, #2
	bl 0x0200ce34
	movs	r3, #9
	movs	r2, #37
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #73
	movs	r2, #9
	movs	r1, #37
	movs	r3, #13
	bl 0x0200ce34
	movs	r2, #15
	mov	sl, r2
.L_02002b60:
	mov	r0, sl
	bl 0x0200ceac
	movs	r3, #35
	mov	r8, r0
	add	r3, r8
	mov	fp, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #2
	beq.n	.L_02002b8e
	ldr	r2, [r0, #8]
	ldr	r3, [r0, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #72
	movs	r1, #48
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ce34
	b.n	.L_02002ba8
.L_02002b8e:
	mov	r1, r8
	ldr	r2, [r1, #8]
	ldr	r3, [r1, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #73
	movs	r1, #48
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ce34
.L_02002ba8:
	mov	r2, r8
	ldr	r4, [pc, #784]
	movs	r6, #0
	ldr	r0, [r2, #8]
	ldr	r3, [r4, r6]
	asrs	r2, r0, #20
	movs	r5, #8
	cmp	r2, r3
	bne.n	.L_02002bd0
	mov	r1, r8
	ldr	r3, [r1, #16]
	ldr	r2, [r4, #4]
	asrs	r3, r3, #20
	cmp	r3, r2
	bne.n	.L_02002bd0
	ldr	r3, [r1, #12]
	cmp	r3, #0
	blt.n	.L_02002bd0
	movs	r5, #0
	b.n	.L_02002bf8
.L_02002bd0:
	adds	r6, #1
	cmp	r6, #7
	bhi.n	.L_02002bf8
	lsls	r1, r6, #3
	ldr	r3, [r4, r1]
	asrs	r2, r0, #20
	cmp	r2, r3
	bne.n	.L_02002bd0
	mov	r2, r8
	ldr	r3, [r2, #16]
	adds	r2, r1, #4
	ldr	r2, [r4, r2]
	asrs	r3, r3, #20
	cmp	r3, r2
	bne.n	.L_02002bd0
	mov	r1, r8
	ldr	r3, [r1, #12]
	cmp	r3, #0
	blt.n	.L_02002bd0
	adds	r5, r6, #0
.L_02002bf8:
	cmp	r5, #8
.L_02002bfa:
	bne.n	.L_02002bfe
	b.n	.L_02002e98
.L_02002bfe:
	movs	r6, #15
	b.n	.L_02002c04
.L_02002c02:
	adds	r6, #1
.L_02002c04:
	cmp	r6, #18
	bhi.n	.L_02002c30
	adds	r0, r6, #0
	bl 0x0200ceac
	cmp	sl, r6
	beq.n	.L_02002c02
	mov	r3, r8
	ldr	r2, [r3, #8]
	ldr	r3, [r0, #8]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02002c02
	mov	r1, r8
	ldr	r2, [r1, #16]
	ldr	r3, [r0, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02002c02
	movs	r5, #8
.L_02002c30:
	cmp	r5, #8
	bne.n	.L_02002c36
	b.n	.L_02002e98
.L_02002c36:
	ldr	r2, [sp, #8]
	ldr	r3, [r2, #80]
	ldrb	r3, [r3, #9]
	lsls	r3, r3, #28
	lsrs	r3, r3, #30
	lsls	r7, r5, #3
	ldr	r1, [pc, #632]
	mov	r9, r3
	ldr	r2, [r2, #16]
	adds	r3, r7, #4
	ldr	r3, [r1, r3]
	asrs	r2, r2, #20
	cmp	r2, r3
	bhi.n	.L_02002c6e
	mov	r2, r8
	ldr	r1, [r2, #12]
	ldr	r0, [r2, #8]
	ldr	r3, [pc, #612]
	ldr	r2, [r2, #16]
	adds	r2, r2, r3
	movs	r3, #20
	bl OverlayObject_PrepareObjectWithCommand15
	movs	r1, #3
	str	r0, [sp, #12]
	movs	r0, #0
	bl 0x0200cf64
.L_02002c6e:
	movs	r6, #15
.L_02002c70:
	adds	r0, r6, #0
	bl 0x0200ceac
	cmp	sl, r6
	beq.n	.L_02002c9e
	mov	r1, r8
	ldr	r2, [r1, #8]
	ldr	r3, [r0, #8]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02002c9e
	ldr	r2, [r1, #16]
	ldr	r3, [r0, #16]
	asrs	r2, r2, #20
	subs	r2, #1
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02002c9e
	adds	r0, r6, #0
	movs	r1, #3
	bl 0x0200cf64
.L_02002c9e:
	adds	r6, #1
	cmp	r6, #18
	bls.n	.L_02002c70
	mov	r0, sl
	bl 0x0200ceac
	movs	r1, #0
	bl 0x0200ce44
	mov	r3, r8
	adds	r3, #34
	movs	r2, #0
	mov	r6, r8
	strb	r2, [r3, #0]
	adds	r6, #85
	movs	r3, #3
	strb	r3, [r6, #0]
	ldr	r3, [pc, #512]
	mov	r1, r8
	movs	r2, #0
	str	r3, [r1, #72]
	str	r2, [r1, #68]
	ldr	r1, [pc, #496]
	adds	r5, r7, #4
	ldr	r3, [r1, r7]
	ldr	r2, [r1, r5]
	movs	r0, #6
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #44
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ce34
	mov	r0, r8
	bl OverlayObject_WaitUntilIdle
	movs	r0, #188
	bl 0x0200cffc
	mov	r3, r8
	movs	r2, #0
	adds	r3, #89
	strb	r2, [r3, #0]
	ldr	r3, [pc, #464]
	mov	r1, r8
.L_02002cfa:
	strb	r2, [r6, #0]
	mov	r0, sl
	str	r3, [r1, #12]
	movs	r1, #3
	bl 0x0200cf64
	movs	r3, #2
	mov	r2, fp
	strb	r3, [r2, #0]
	ldr	r1, [pc, #428]
	ldr	r3, [r1, r7]
	ldr	r2, [r1, r5]
	movs	r0, #73
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r2, #1
	movs	r1, #48
	bl 0x0200ce34
	movs	r0, #0
	mov	r1, r9
	bl 0x0200cf64
	movs	r0, #0
	bl 0x0200ceac
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r6, #15
.L_02002d3c:
	adds	r0, r6, #0
	bl 0x0200ceac
	cmp	sl, r6
	beq.n	.L_02002d7c
	mov	r3, r8
	ldr	r2, [r3, #8]
	ldr	r3, [r0, #8]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02002d7c
	mov	r1, r8
	ldr	r2, [r1, #16]
	ldr	r3, [r0, #16]
	asrs	r2, r2, #20
	subs	r2, #1
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02002d7c
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200cf64
	adds	r0, r6, #0
	bl 0x0200ceac
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
.L_02002d7c:
	adds	r6, #1
	cmp	r6, #18
	bls.n	.L_02002d3c
	ldr	r0, [sp, #12]
	bl 0x0200cdec
	movs	r0, #194
	lsls	r0, r0, #2
	bl 0x0200ce74
	cmp	r0, #0
	beq.n	.L_02002d9a
	bl 0x0200ce9c
	b.n	.L_02002ea8
.L_02002d9a:
	movs	r0, #15
	bl 0x0200ceac
	mov	r8, r0
	movs	r0, #16
	bl 0x0200ceac
	adds	r5, r0, #0
	movs	r0, #17
	bl 0x0200ceac
	adds	r6, r0, #0
	movs	r0, #18
	bl 0x0200ceac
	movs	r2, #35
	add	r8, r2
	mov	r3, r8
	adds	r5, #35
	ldrb	r2, [r3, #0]
	ldrb	r3, [r5, #0]
	adds	r6, #35
	ands	r3, r2
	ldrb	r2, [r6, #0]
	adds	r0, #35
	ands	r3, r2
	ldrb	r2, [r0, #0]
	ands	r3, r2
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02002e98
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x0200cf7c
	movs	r0, #14
	movs	r1, #1
	bl 0x0200cf94
	bl 0x0200cf8c
	movs	r1, #194
	ldr	r2, [pc, #212]
	lsls	r1, r1, #2
	movs	r0, #136
	bl SceneEffect_SpawnEffect284AtCell
	adds	r6, r0, #0
	movs	r0, #30
	bl 0x0200ce8c
	ldr	r0, [pc, #200]
	ldr	r1, [pc, #200]
	bl 0x0200cf7c
	movs	r0, #216
	movs	r1, #1
	movs	r2, #158
	movs	r3, #1
	lsls	r2, r2, #18
	negs	r1, r1
	lsls	r0, r0, #16
	bl 0x0200cf84
	adds	r0, r6, #0
.L_02002e22:
	bl 0x0200cdfc
	adds	r0, r6, #0
	ldr	r1, [pc, #172]
	bl 0x0200cddc
	movs	r1, #190
	lsls	r1, r1, #2
	movs	r0, #216
	ldr	r2, [pc, #164]
	.2byte 0xf7fd
	.2byte 0xff11
	movs	r1, #99
	ldr	r3, [r6, #0]
	adds	r1, r1, r6
	adds	r5, r0, #0
	mov	r8, r1
	b.n	.L_02002e8e
.L_02002e46:
	mov	r1, r8
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L_02002e58
	adds	r3, r5, #0
	adds	r3, #99
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02002e86
.L_02002e58:
	movs	r0, #30
	bl 0x0200ce8c
	ldr	r0, [pc, #128]
	movs	r1, #77
	movs	r2, #35
	bl 0x0200ce14
	movs	r3, #13
	movs	r2, #36
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #13
	movs	r1, #35
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ce34
	movs	r0, #194
	lsls	r0, r0, #2
	bl 0x0200ce7c
	b.n	.L_02002e98
.L_02002e86:
	movs	r0, #1
	bl 0x0200cda4
	ldr	r3, [r6, #0]
.L_02002e8e:
	cmp	r3, #0
	bne.n	.L_02002e46
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_02002e46
.L_02002e98:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #18
	bhi.n	.L_02002ea4
	b.n	.L_02002b60
.L_02002ea4:
	bl 0x0200ce9c
.L_02002ea8:
	add	sp, #16
.L_02002eaa:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x0200d164
	.4byte 0xfffc0000
	.4byte 0x00001999
	.4byte 0xfff00000
	.4byte 0x0200d77c
	.4byte 0x00006666
	.4byte 0x00000ccc
	.4byte 0x0200d7c8
	.4byte 0x0200dac8
	.2byte 0xdd3c
	.2byte 0x0200
	.section .text.x0200b068,"ax",%progbits
	.balign 4
	.global Func_02003068
	.thumb_func
Func_02003068:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #1
	sub	sp, #12
	bl 0x0200ce8c
	ldr	r0, [pc, #976]
	bl 0x0200ce74
	cmp	r0, #0
	beq.n	.L_0200308c
	bl SceneActor_ApplySlotsMatchingKind212
.L_0200308c:
	movs	r0, #136
	lsls	r0, r0, #1
	bl 0x0200ce7c
	ldr	r3, [pc, #956]
	movs	r2, #224
	ldr	r3, [r3, #0]
	lsls	r2, r2, #1
	adds	r0, r3, r2
	movs	r3, #129
	lsls	r3, r3, #2
	str	r3, [r0, #0]
	ldr	r1, [pc, #944]
	ldrsh	r2, [r1, r2]
	ldr	r3, [pc, #944]
	cmp	r2, r3
	bne.n	.L_02003112
	movs	r3, #128
	lsls	r3, r3, #1
	str	r3, [r0, #0]
	ldr	r0, [pc, #936]
	bl 0x0200ce74
	cmp	r0, #0
	bne.n	.L_020030c6
	movs	r0, #8
	bl SceneActor_ClearActorModeAndSetState5
	b.n	.L_020030da
.L_020030c6:
	movs	r3, #7
	movs	r2, #16
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #7
	movs	r1, #17
	movs	r2, #2
	movs	r3, #1
	bl 0x0200ce34
.L_020030da:
	movs	r0, #9
	bl SceneActor_ClearActorModeAndSetState5
	movs	r0, #10
	bl SceneActor_ClearActorModeAndSetState5
	movs	r0, #11
	bl SceneActor_ClearActorModeAndSetState5
	movs	r1, #2
	movs	r0, #11
	bl 0x0200cf64
	movs	r0, #12
	bl SceneActor_ClearActorModeAndSetState5
	movs	r0, #12
	movs	r1, #2
	bl 0x0200cf64
	movs	r0, #13
	bl SceneActor_ClearActorModeAndSetState5
	movs	r0, #14
	bl SceneActor_ClearActorModeAndSetState5
	bl 0x0200bfa6
.L_02003112:
	ldr	r3, [pc, #848]
	cmp	r2, r3
	beq.n	.L_0200311a
	b.n	.L_020033a8
.L_0200311a:
	movs	r2, #225
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	subs	r3, #1
	cmp	r3, #25
	bls.n	.L_0200312e
	bl 0x0200bfa6
.L_0200312e:
	ldr	r2, [pc, #824]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0x0000
	.4byte 0x0200b1a0
	.4byte 0x0200b1a0
	.4byte 0x0200b1b2
	.4byte 0x0200b1b2
	.4byte 0x0200b1aa
	.4byte 0x0200b1b2
	.4byte 0x0200b376
	.4byte 0x0200b376
	.4byte 0x0200b376
	.4byte 0x0200b376
	.4byte 0x0200b376
	.4byte 0x0200bfa6
	.4byte 0x0200bfa6
	.4byte 0x0200bfa6
	.4byte 0x0200bfa6
	.4byte 0x0200bfa6
	.4byte 0x0200bfa6
	.4byte 0x0200bfa6
	.4byte 0x0200bfa6
	.4byte 0x0200b1bc
	.4byte 0x0200b1bc
	.4byte 0x0200b348
	.4byte 0x0200b348
	.4byte 0x0200bfa6
	.4byte 0x0200bfa6
	.4byte 0x0200b342
	.4byte 0xf7ff2008
	.4byte 0xf000ff33
	.4byte 0x2090fefe
	.4byte 0xf0010040
	.4byte 0x2009fe69
	.4byte 0xff2af7ff
	.4byte 0xfef5f000
	.4byte 0xf00148ab
	.4byte 0x2800fe59
	.4byte 0x2307d050
	.4byte 0x93002208
	.4byte 0x20679201
	.4byte 0x2259211b
	.4byte 0xf001231b
	.4byte 0x2503fe21
	.4byte 0x20292602
	.4byte 0x221b215a
	.4byte 0x9500235c
	.4byte 0xf0019601
	.4byte 0x2029fe17
	.4byte 0x221d215a
	.4byte 0x9500235d
	.4byte 0xf0019601
	.4byte 0x2029fe0f
	.4byte 0x221b215a
	.4byte 0x9500235e
	.4byte 0xf0019601
	.4byte 0x2029fe07
	.4byte 0x221b215a
	.4byte 0x95002360
	.4byte 0xf0019601
	.4byte 0x2029fdff
	.4byte 0x221d215a
	.4byte 0x95002361
	.4byte 0xf0019601
	.4byte 0x2029fdf7
	.4byte 0x22192160
	.4byte 0x9500235b
	.4byte 0xf0019601
	.4byte 0x2029fdef
	.4byte 0x2219215c
	.4byte 0x9500235d
	.4byte 0xf0019601
	.4byte 0x2029fde7
	.4byte 0x22192160
	.4byte 0x9500235f
	.4byte 0xf0019601
	.4byte 0x2029fddf
	.4byte 0x22192160
	.4byte 0xe0562361
	.4byte 0xf0014881
	.4byte 0x2800fe03
	.4byte 0xf000d101
	.4byte 0x2307fe98
	.4byte 0x93002208
	.4byte 0x206f9201
	.4byte 0x2259211b
	.4byte 0xf001231b
	.4byte 0x2503fdc9
	.4byte 0x20292602
	.4byte 0x2219215a
	.4byte 0x9500235b
	.4byte 0xf0019601
	.4byte 0x2029fdbf
	.4byte 0x2219215a
	.4byte 0x9500235d
	.4byte 0xf0019601
	.4byte 0x2029fdb7
	.4byte 0x2219215a
	.4byte 0x9500235f
	.4byte 0xf0019601
	.4byte 0x2029fdaf
	.4byte 0x2219215a
	.4byte 0x95002361
	.4byte 0xf0019601
	.4byte 0x2029fda7
	.4byte 0x221b215a
	.4byte 0x95002360
	.4byte 0xf0019601
	.4byte 0x2029fd9f
	.4byte 0x221d215a
	.4byte 0x95002361
	.4byte 0xf0019601
	.4byte 0x2029fd97
	.4byte 0x221b215e
	.4byte 0x9500235c
	.4byte 0xf0019601
	.4byte 0x2029fd8f
	.4byte 0x221d2160
	.4byte 0x9500235d
	.4byte 0xf0019601
	.4byte 0x2029fd87
	.4byte 0x221b215e
	.4byte 0x9500235e
	.4byte 0xf0019601
	.4byte 0x2029fd7f
	.4byte 0x221b2160
	.4byte 0x95002360
	.4byte 0xf0019601
	.4byte 0x2029fd77
	.4byte 0x221d2160
	.4byte 0x95002361
	.4byte 0xf0019601
	.4byte 0xf000fd6f
	.4byte 0x484cfe32
	.4byte 0xfd9ef001
	.4byte 0xf001484b
	.4byte 0x2080fd9b
	.4byte 0xf0010080
	.4byte 0x2800fd8f
	.4byte 0xf000d101
	.4byte 0x2303fe24
	.4byte 0x93002205
	.4byte 0x202c9201
	.4byte 0x22292175
	.4byte 0xf0012375
	.4byte 0xf000fd55
	.4byte 0x4841fe18
	.4byte 0xfd7cf001
	.4byte 0xd1012800
	.4byte 0xfe11f000
	.4byte 0x22b021da
	.4byte 0x0489200c
	.4byte 0xf00103d2
	.4byte 0x200cfdb1
	.4byte 0xfd8af001
	.4byte 0x1c074b39
	.4byte 0x238060fb
	.4byte 0x63fb061b
	.2byte 0xf000
	.2byte 0xfdff
.L_020033a8:
	ldr	r3, [pc, #216]
	cmp	r2, r3
	beq.n	.L_020033b0
	b.n	.L_020035b4
.L_020033b0:
	movs	r2, #225
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	subs	r3, #1
	cmp	r3, #20
	bls.n	.L_020033c4
	bl 0x0200bfa6
.L_020033c4:
	ldr	r2, [pc, #192]
	lsls	r3, r3, #2
.L_020033c8:
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0200bc86
	.4byte 0x0200bc86
	.4byte 0x0200bc86
	.4byte 0x0200bc86
	.4byte 0x0200bfa6
	.4byte 0x0200bfa6
	.4byte 0x0200b4dc
	.4byte 0x0200b4dc
	.4byte 0x0200b48c
	.2byte 0xb48c
	.2byte 0x0200
	push	{r2, r5, lr}
	lsls	r0, r0, #8
	push	{r2, r5, lr}
	lsls	r0, r0, #8
	push	{r2, r5, lr}
	lsls	r0, r0, #8
	push	{r2, r5, lr}
	lsls	r0, r0, #8
	push	{r2, r5, lr}
	lsls	r0, r0, #8
	push	{r5}
	lsls	r0, r0, #8
	.2byte 0xbfa6
	.2byte 0x0200
	.2byte 0xbfa6
	.2byte 0x0200
	.2byte 0xbc86
	.2byte 0x0200
	push	{r1, r4, r5}
	lsls	r0, r0, #8
	push	{r1, r3, r5}
	lsls	r0, r0, #8
	ldr	r0, [pc, #84]
	bl 0x0200ce84
	bl 0x0200bfa6
	bl 0x0200c2bc
	bl 0x0200bfa6
	movs	r0, #170
	bl 0x0200cfec
	ldr	r0, [pc, #20]
	bl 0x0200ce74
	cmp	r0, #0
	beq.n	.L_02003446
	bl 0x0200bfa6
.L_02003446:
	bl FieldScene_RunSupplementalSequenceOne
	bl 0x0200bfa6
	movs	r0, r0
	lsls	r1, r1, #4
	movs	r0, r0
.L_02003454:
	subs	r4, r7, #2
	lsls	r0, r0, #12
	lsls	r0, r0, #9
	lsls	r0, r0, #8
	lsls	r5, r6, #2
	movs	r0, r0
	lsrs	r1, r0, #6
	movs	r0, r0
	lsls	r6, r6, #2
	movs	r0, r0
	.2byte 0xb138
	lsls	r0, r0, #8
	lsrs	r2, r0, #6
	movs	r0, r0
	lsrs	r3, r0, #6
	movs	r0, r0
	lsls	r1, r4, #4
	movs	r0, r0
	lsls	r7, r5, #4
	movs	r0, r0
	lsrs	r7, r0, #6
	movs	r0, r0
	movs	r0, r0
	.2byte 0xffe8
	.2byte 0x00b7
	movs	r0, r0
	.2byte 0xb3cc
	lsls	r0, r0, #8
	movs	r0, #11
	bl 0x0200ceac
	adds	r7, r0, #0
	bl SceneActor_ApplyPositionsOfActors11And12
	ldr	r3, [r7, #8]
	asrs	r3, r3, #20
	cmp	r3, #8
	bne.n	.L_020034a6
	adds	r0, r7, #0
	bl SceneState_MarkActorAndApplyRectAtTile
.L_020034a6:
	movs	r0, #12
	bl 0x0200ceac
	adds	r7, r0, #0
	ldr	r3, [r7, #8]
	asrs	r3, r3, #20
	cmp	r3, #7
	bne.n	.L_020034ba
	bl SceneState_MarkActorAndApplyRectAtTile
.L_020034ba:
	ldr	r5, [pc, #880]
	movs	r0, #206
	movs	r1, #0
	adds	r2, r5, #0
	movs	r3, #223
	lsls	r0, r0, #16
	bl OverlayObject_SpawnWithMode14
	movs	r0, #210
	lsls	r0, r0, #16
.L_020034ce:
	movs	r1, #0
	adds	r2, r5, #0
	movs	r3, #223
	bl OverlayObject_SpawnWithMode14
	bl 0x0200bfa6
	movs	r0, #0
	bl 0x0200cfdc
	movs	r0, #2
	bl 0x0200cda4
	movs	r0, #8
	bl 0x0200ceac
	adds	r7, r0, #0
.L_020034f0:
	ldr	r5, [pc, #828]
	adds	r3, r7, #0
	adds	r3, #85
	movs	r6, #0
	strb	r6, [r3, #0]
	movs	r0, #9
	str	r5, [r7, #108]
	bl 0x0200ceac
	adds	r7, r0, #0
	adds	r3, r7, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	movs	r0, #10
	str	r5, [r7, #108]
	bl 0x0200ceac
.L_02003512:
	adds	r7, r0, #0
	adds	r3, r7, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	str	r5, [r7, #108]
	bl FieldScene_PlaceAndPinSlots8To10
.L_02003520:
	bl 0x0200bfa6
	movs	r0, #170
	bl 0x0200cfec
	movs	r0, #0
	bl 0x0200cfdc
	movs	r0, #2
	bl 0x0200cda4
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200ce74
	cmp	r0, #0
	bne.n	.L_02003546
	bl 0x0200bfa6
.L_02003546:
	movs	r5, #5
	movs	r6, #2
	movs	r0, #111
	movs	r1, #5
	movs	r2, #117
	movs	r3, #5
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ce1c
	movs	r0, #111
	movs	r1, #10
	movs	r2, #117
	movs	r3, #10
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ce1c
	movs	r0, #111
	movs	r1, #7
	movs	r2, #111
	movs	r3, #5
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ce1c
	movs	r0, #111
	movs	r1, #7
	movs	r2, #111
	movs	r3, #10
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ce1c
	movs	r3, #54
	movs	r5, #3
	str	r3, [sp, #0]
	movs	r0, #48
	movs	r1, #3
	movs	r2, #3
	movs	r3, #10
	str	r5, [sp, #4]
	bl 0x0200ce2c
	movs	r3, #48
	str	r3, [sp, #0]
	movs	r0, #55
	movs	r1, #26
	movs	r2, #3
	movs	r3, #10
	str	r5, [sp, #4]
	bl 0x0200ce2c
	bl 0x0200bfa6
.L_020035b4:
	ldr	r3, [pc, #636]
	cmp	r2, r3
	beq.n	.L_020035bc
	b.n	.L_020037bc
.L_020035bc:
	movs	r2, #225
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	subs	r3, #1
	cmp	r3, #10
	bls.n	.L_020035d0
	bl 0x0200bfa6
.L_020035d0:
	ldr	r2, [pc, #612]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0200bc86
	.4byte 0x0200b604
	.4byte 0x0200bfa6
	.4byte 0x0200b60a
	.4byte 0x0200bfa6
	.4byte 0x0200b60a
	.4byte 0x0200b706
	.4byte 0x0200b706
	.4byte 0x0200b614
	.4byte 0x0200b614
	.4byte 0x0200b6ee
	.4byte 0xf8e8f7fd
	.4byte 0x2000e33d
	.4byte 0xfce6f001
	.4byte 0xfcc9f000
	.4byte 0xf0012008
	.4byte 0x2200fc49
	.4byte 0x46901c07
	.4byte 0x33551c3b
	.4byte 0x70194641
	.4byte 0x60fa2009
	.4byte 0xfc3ef001
	.4byte 0x1c072355
	.4byte 0x464119db
	.4byte 0x469a7019
	.4byte 0x33591c3b
	.4byte 0x487e7019
	.4byte 0xfc16f001
	.4byte 0xd04c2800
	.4byte 0xf0012001
	.4byte 0x2301fba9
	.4byte 0x25029300
	.4byte 0x2129207c
	.4byte 0x2329226e
	.4byte 0xf0019501
	.4byte 0x232afbdb
	.4byte 0x262e9301
	.4byte 0x202e2301
	.4byte 0x22012129
	.4byte 0xf0019600
	.4byte 0x21bafbdd
	.4byte 0x200922b6
	.4byte 0x04920489
	.4byte 0xfc36f001
	.4byte 0x46534642
	.4byte 0x4b6c701a
	.4byte 0x60fb2009
	.4byte 0xf0012103
	.4byte 0x1c3bfc65
	.4byte 0x701d3323
	.4byte 0x9301232d
	.4byte 0x23012201
	.4byte 0x212d202d
	.4byte 0xf0019600
	.4byte 0x200afbc1
	.4byte 0xf0012107
	.4byte 0x2101fc21
	.4byte 0xf001200a
	.4byte 0x200afc51
	.4byte 0xfbf2f001
	.4byte 0x1c3b1c07
	.4byte 0x46413359
	.4byte 0x22ae7019
	.4byte 0x701d3b36
	.4byte 0x495a200a
	.4byte 0xf0010492
	.4byte 0x4b59fc09
	.4byte 0xf7fe66fb
	.4byte 0xf000fb2f
	.4byte 0x4b57fc5c
	.4byte 0x681b22e0
	.4byte 0x189b0052
	.4byte 0x601a3242
	.4byte 0xf0012000
	.4byte 0x4b53fbd5
	.4byte 0x20aa60c3
	.4byte 0xfc70f001
	.4byte 0xfb8af001
	.4byte 0x4a502300
	.4byte 0x20c08013
	.4byte 0xf0010080
	.4byte 0x2800fbab
	.4byte 0x2503d034
	.4byte 0x2160200f
	.4byte 0x23602209
	.4byte 0x95019500
	.4byte 0xfb74f001
	.4byte 0x2160200c
	.4byte 0x2360220f
	.4byte 0x95019500
	.4byte 0xfb6cf001
	.4byte 0x20052604
	.4byte 0x220f2132
	.4byte 0x95002320
	.4byte 0xf0019601
	.4byte 0x2019fb63
	.4byte 0x2209212d
	.4byte 0x95002320
	.4byte 0xf0019601
	.4byte 0x2309fb5b
	.4byte 0x93002520
	.4byte 0x2120200f
	.4byte 0x23012203
	.4byte 0xf0019501
	.4byte 0x230ffb5d
	.4byte 0x200c9300
	.4byte 0x22032120
	.4byte 0x95012301
	.4byte 0xfb54f001
	.4byte 0x21e14b32
	.4byte 0x185b0049
	.4byte 0x5e9b2200
	.4byte 0xd0012b0b
	.4byte 0xfc03f000
	.4byte 0xfc10f001
	.4byte 0xfc16f001
	.4byte 0x21e04b28
	.4byte 0x0049681b
	.4byte 0x185b2281
	.4byte 0x601a0092
	.2byte 0xf000
	.2byte 0xfbf5
.L_020037bc:
	ldr	r3, [pc, #156]
	cmp	r2, r3
	beq.n	.L_020037c4
	.2byte 0xe159
.L_020037c4:
	movs	r2, #225
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	subs	r3, #4
	cmp	r3, #16
	bls.n	.L_020037d6
	.2byte 0xe3e7
.L_020037d6:
	ldr	r2, [pc, #136]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0x0000
	.4byte 0x0200b910
	.4byte 0x0200b910
	.4byte 0x0200bc86
	.4byte 0x0200bfa6
	.4byte 0x0200bfa6
	.4byte 0x0200b864
	.4byte 0x0200b864
	.4byte 0x0200b864
	.4byte 0x0200b864
	.4byte 0x0200b864
	.4byte 0x0200b864
	.4byte 0x0200b916
	.4byte 0x0200b916
	.4byte 0x0200bfa6
	.4byte 0x0200bfa6
	.4byte 0x0200b824
	.4byte 0x0200b864
	.4byte 0xfef4f000
	.4byte 0x0000e3bd
	.4byte 0x01c10000
	.4byte SceneActor_SetFlagBitByRelativeDepth
	.2byte 0x00b8
	.2byte 0x0000
	push	{r3, r4, r6, r7, lr}
	lsls	r0, r0, #8
	lsls	r1, r0, #12
	movs	r0, r0
	movs	r0, r0
	.2byte 0xfff0
	.2byte 0x0000
	lsls	r7, r4, #11
	ldrh	r1, [r3, #28]
	lsls	r0, r0, #8
	subs	r4, r7, #2
	lsls	r0, r0, #12
	movs	r0, r0
	.2byte 0xfffe
	.2byte 0x0050
	lsls	r0, r0, #16
	lsls	r0, r0, #9
	lsls	r0, r0, #8
	lsls	r1, r7, #2
	movs	r0, r0
	.2byte 0xb7e0
	lsls	r0, r0, #8
	movs	r0, #170
	bl 0x0200cfec
	ldr	r0, [pc, #724]
	bl 0x0200ce74
	cmp	r0, #0
	beq.n	.L_020038ac
	movs	r3, #26
	movs	r2, #12
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #53
	movs	r1, #12
	movs	r2, #3
	movs	r3, #13
	bl 0x0200ce2c
	movs	r3, #9
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #41
	movs	r0, #81
	movs	r2, #89
	movs	r3, #14
	bl 0x0200ce1c
	movs	r0, #1
	bl 0x0200cda4
	movs	r1, #200
	ldr	r0, [pc, #668]
	lsls	r1, r1, #4
	bl 0x0200cdac
.L_020038ac:
	ldr	r0, [pc, #664]
	bl 0x0200ce74
	cmp	r0, #0
	beq.n	.L_020038ee
	movs	r3, #34
	movs	r2, #12
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #58
	movs	r1, #12
	movs	r2, #3
	movs	r3, #13
	bl 0x0200ce2c
	movs	r3, #5
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #41
	movs	r0, #81
	movs	r2, #97
	movs	r3, #14
	bl 0x0200ce1c
	movs	r0, #1
	bl 0x0200cda4
	movs	r1, #200
	ldr	r0, [pc, #612]
	lsls	r1, r1, #4
	bl 0x0200cdac
.L_020038ee:
	ldr	r3, [pc, #608]
	movs	r2, #225
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #11
	bne.n	.L_02003904
	bl FieldScene_RunOpeningAuxiliarySequence
	b.n	.L_02003fa6
.L_02003904:
	cmp	r3, #20
	beq.n	.L_0200390a
	b.n	.L_02003fa6
.L_0200390a:
	bl Scene_RunEastParticleWaveSequence
	b.n	.L_02003fa6
	.4byte 0xff62f7fc
	.4byte 0x2008e1b7
	.4byte 0xfac8f001
	.4byte 0x1c3b1c07
	.4byte 0x33552500
	.4byte 0x2009701d
	.4byte 0xf00160fd
	.4byte 0x3055fabf
	.4byte 0x200a7005
	.4byte 0xfabaf001
	.4byte 0x70053055
	.4byte 0xf001200b
	.4byte 0x3055fab5
	.4byte 0x20c17005
	.4byte 0xf0010080
	.4byte 0x2800fa93
	.4byte 0xe08ed100
	.4byte 0xf0012001
	.4byte 0x2301fa25
	.4byte 0x93002202
	.4byte 0x206f9201
	.4byte 0x226d213b
	.4byte 0xf0012325
	.4byte 0x232dfa57
	.4byte 0x93002226
	.4byte 0x202d9201
	.4byte 0x22012125
	.4byte 0xf0012301
	.4byte 0x4874fa59
	.4byte 0xfa76f001
	.4byte 0xd0152800
	.4byte 0x22a621c2
	.4byte 0x04892009
	.4byte 0xf0010492
	.4byte 0x21d2faad
	.4byte 0x200a22a6
	.4byte 0x04920489
	.4byte 0xfaa6f001
	.4byte 0x22ae21c2
	.4byte 0x0489200b
	.4byte 0xf0010492
	.4byte 0xe014fa9f
	.4byte 0x22a621d2
	.4byte 0x04892009
	.4byte 0xf0010492
	.4byte 0x21c2fa97
	.4byte 0x200a22ae
	.4byte 0x04920489
	.4byte 0xfa90f001
	.4byte 0x22ae21d2
	.4byte 0x0489200b
	.4byte 0xf0010492
	.4byte 0x2103fa89
	.4byte 0xf0012009
	.4byte 0x2009fabd
	.4byte 0xfa5ef001
	.4byte 0x4d591c07
	.4byte 0x33231c3b
	.4byte 0x22002602
	.4byte 0x210360fd
	.4byte 0x200a701e
	.4byte 0xf0014690
	.4byte 0x200afaad
	.4byte 0xfa4ef001
	.4byte 0x1c3b1c07
	.4byte 0x60fd3323
	.4byte 0x701e2103
	.4byte 0xf001200b
	.4byte 0x200bfaa1
	.4byte 0xfa42f001
	.4byte 0x1c3b1c07
	.4byte 0x60fd3323
	.4byte 0x701e2107
	.4byte 0xf001200c
	.4byte 0x200cfa61
	.4byte 0xfa36f001
	.4byte 0xf0012100
	.4byte 0x2101f9ff
	.4byte 0xf001200c
	.4byte 0x200cfa8b
	.4byte 0xfa2cf001
	.4byte 0x1c3b1c07
	.4byte 0x46413359
	.4byte 0x229e7019
	.4byte 0x701e3b36
	.4byte 0x493d200c
	.4byte 0xf0010492
	.4byte 0x4b3cfa43
	.4byte 0xf7fe66fb
	.2byte 0xfa75
	.2byte 0xe296
.L_02003a78:
	ldr	r3, [pc, #232]
	cmp	r2, r3
	beq.n	.L_02003a80
	b.n	.L_02003fa6
.L_02003a80:
	movs	r2, #225
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	subs	r3, #1
	cmp	r3, #19
	bls.n	.L_02003a92
	b.n	.L_02003fa6
.L_02003a92:
	ldr	r2, [pc, #212]
.L_02003a94:
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0x0000
	.4byte 0x0200baec
	.4byte 0x0200baec
	.4byte 0x0200bca6
	.4byte 0x0200bca6
	.4byte 0x0200bca6
	.4byte 0x0200bca6
	.4byte 0x0200bca6
	.4byte 0x0200bca6
	.4byte 0x0200bca6
	.4byte 0x0200bca6
	.4byte 0x0200bca6
	.4byte 0x0200bca6
	.4byte 0x0200bc86
	.4byte 0x0200bc86
	.4byte 0x0200be34
	.4byte 0x0200be34
	.4byte 0x0200bfa6
	.4byte 0x0200bc8e
	.4byte 0x0200bc8e
	.4byte 0x0200bca6
	.4byte 0xf001481f
	.4byte 0x2800f9c1
	.4byte 0x2000d03c
	.4byte 0xfbe6f7fe
	.4byte 0xf0012000
	.4byte 0xf7fcf9d5
	.4byte 0x2500ff23
	.4byte 0x300a1c28
	.4byte 0xf9cef001
	.4byte 0x68bb1c07
	.4byte 0x2c0d151c
	.4byte 0x693bd10d
	.4byte 0x2e07151e
	.4byte 0x2280d109
	.4byte 0x18a80092
	.4byte 0xf0019402
	.4byte 0x9c02f9a3
	.4byte 0xd0002800
	.4byte 0x3501e169
	.4byte 0xd9e52d03
	.4byte 0x0000e233
	.4byte 0x00000306
	.4byte SceneState_CallWith432And32
	.4byte 0x00000307
	.4byte FieldScene_CallWith560And44
	.4byte 0x02000240
	.4byte 0x00000302
	.4byte 0xfff00000
	.4byte 0x02d70000
	.4byte SceneEffect_SpawnRandomizedParticleEveryFourFrames
	.4byte 0x000000ba
	.4byte 0x0200ba9c
	.4byte 0x00000109
	.4byte 0xf0012008
	.4byte 0x2100f99b
	.4byte 0x46891c07
	.4byte 0x33551c3b
	.4byte 0x701a464a
	.4byte 0x1c3a4b18
	.4byte 0x322360fb
	.4byte 0x7813469b
	.4byte 0x43332602
	.4byte 0x1c397013
	.4byte 0x780a3159
	.4byte 0x1c2b25fe
	.4byte 0x700b4013
	.4byte 0x490f2203
	.4byte 0x1c3b4692
	.4byte 0x33644688
	.4byte 0x80194651
	.4byte 0x21012008
	.4byte 0xf9d4f001
	.4byte 0xf0012009
	.4byte 0x1c07f975
	.4byte 0x33551c3b
	.4byte 0x701a4642
	.4byte 0x60fb465b
	.4byte 0x32231c3a
	.4byte 0x431e7813
	.4byte 0x32367016
	.4byte 0x401d7813
	.4byte 0xe0031c3b
	.4byte 0x00000000
	.4byte 0xffd00000
	.4byte 0x46513364
	.4byte 0x20097015
	.4byte 0x21018019
	.4byte 0xf9b4f001
	.4byte 0xf001200a
	.4byte 0x1c07f955
	.4byte 0x46421c3b
	.4byte 0x701a3355
	.4byte 0x330f4649
	.4byte 0x200a8019
	.4byte 0xf94af001
	.4byte 0xf0012100
	.4byte 0x200bf913
	.4byte 0xf944f001
	.4byte 0x1c3b1c07
	.4byte 0x33554642
	.4byte 0x4649701a
	.4byte 0x8019330f
	.4byte 0xf001200b
	.4byte 0x2100f939
	.4byte 0xf902f001
	.4byte 0xf001200c
	.4byte 0x1c07f933
	.4byte 0x46421c3b
	.4byte 0x701a3355
	.4byte 0x330f4649
	.4byte 0x200c8019
	.4byte 0xf928f001
	.4byte 0xf0012100
	.4byte 0x200df8f1
	.4byte 0xf922f001
	.4byte 0x1c3b1c07
	.4byte 0x46423355
	.4byte 0x4649701a
	.4byte 0x8019330f
	.4byte 0xf001200d
	.4byte 0x2100f917
	.4byte 0xf8e0f001
	.4byte 0x20aae18f
	.4byte 0xf9b0f001
	.4byte 0x4bcbe18b
	.4byte 0x681b22e0
	.4byte 0x189b0052
	.4byte 0x601a3242
	.4byte 0xf0012000
	.4byte 0x4bc7f905
	.4byte 0x201460c3
	.4byte 0xf900f001
	.4byte 0x30552704
	.4byte 0x20147007
	.4byte 0xf8faf001
	.4byte 0x78023023
	.4byte 0x43132302
	.4byte 0x20147003
	.4byte 0xf8f2f001
	.4byte 0x60c34bbe
	.4byte 0xf8aaf001
	.4byte 0x4bbd2500
	.4byte 0x48bd801d
	.4byte 0xf8ccf001
	.4byte 0xd0382800
	.4byte 0xf00120aa
	.4byte 0x2603f983
	.4byte 0x20242502
	.4byte 0x22202151
	.4byte 0x95012351
	.4byte 0xf0019600
	.4byte 0x2024f891
	.4byte 0x22242153
	.4byte 0x95012351
	.4byte 0xf0019600
	.4byte 0x2320f889
	.4byte 0x25119300
	.4byte 0x21112024
	.4byte 0x23012203
	.4byte 0xf0019501
	.4byte 0x2324f88b
	.4byte 0x20249300
	.4byte 0x22032112
	.4byte 0x95012301
	.4byte 0xf882f001
	.4byte 0x93002301
	.4byte 0x203f9301
	.4byte 0x2221211d
	.4byte 0xf0012314
	.4byte 0x2014f86d
	.4byte 0x22242138
	.4byte 0x96002311
	.4byte 0xf0019701
	.4byte 0x489ff865
	.4byte 0xf88ef001
	.4byte 0xd0352800
	.4byte 0x25022603
	.4byte 0x2151202c
	.4byte 0x23512230
	.4byte 0x96009501
	.4byte 0xf856f001
	.4byte 0x2153202c
	.4byte 0x2351222c
	.4byte 0x96009501
	.4byte 0xf84ef001
	.4byte 0x93002330
	.4byte 0x202c2511
	.4byte 0x22032111
	.4byte 0x95012301
	.4byte 0xf850f001
	.4byte 0x9300232c
	.4byte 0x2112202c
	.4byte 0x23012203
	.4byte 0xf0019501
	.4byte 0x2301f847
	.4byte 0x93019300
	.4byte 0x211d203f
	.4byte 0x23142231
	.4byte 0xf832f001
	.4byte 0x21382029
	.4byte 0x2311222c
	.4byte 0x97019600
	.4byte 0xf82af001
	.4byte 0x21e14b82
	.4byte 0x185d0049
	.4byte 0x1c13882a
	.4byte 0x21803b12
	.4byte 0x0249041b
	.4byte 0xd80b428b
	.4byte 0xf8f0f001
	.4byte 0xf8f6f001
	.4byte 0x22e04b74
	.4byte 0x0052681b
	.4byte 0x3244189b
	.4byte 0x882a601a
	.4byte 0x041321a0
	.4byte 0x428b0349
	.4byte 0xe0d0d000
	.4byte 0xfe12f000
	.4byte 0x1c38e0cd
	.4byte 0x78023023
	.4byte 0x43132302
	.4byte 0x1c3b7003
	.4byte 0x33592100
	.4byte 0x3b047019
	.4byte 0x20047019
	.4byte 0x22012113
	.4byte 0x94002301
	.4byte 0xf0019601
	.4byte 0xe0b8f801
	.4byte 0xf0002001
	.4byte 0x21c8ffb5
	.4byte 0x48660109
	.4byte 0xffb4f000
	.4byte 0xf001200e
	.4byte 0x2200f831
	.4byte 0x46901c07
	.4byte 0x46411c3b
	.4byte 0x70193355
	.4byte 0x60fa200f
	.4byte 0xf826f001
	.4byte 0x30554643
	.4byte 0x20107003
	.4byte 0xf820f001
	.4byte 0x30554641
	.4byte 0x20117001
	.4byte 0xf81af001
	.4byte 0x30554642
	.4byte 0x20127002
	.4byte 0xf814f001
	.4byte 0x30554643
	.4byte 0x20c27003
	.4byte 0xf0000080
	.4byte 0x2800fff1
	.4byte 0xe084d100
	.4byte 0xf0002001
	.4byte 0x2301ff83
	.4byte 0x26029300
	.4byte 0x2138205f
	.4byte 0x2323224d
	.4byte 0xf0009601
	.4byte 0x230dffb5
	.4byte 0x93002224
	.4byte 0x23019201
	.4byte 0x2123200d
	.4byte 0xf0002201
	.4byte 0x2184ffb7
	.4byte 0x049222ba
	.4byte 0x200f0449
	.4byte 0xf810f001
	.4byte 0xf000200f
	.4byte 0x1c07ffe9
	.4byte 0x1c3b4d3f
	.4byte 0x60fd3323
	.4byte 0x701e200f
	.4byte 0xf0012103
	.4byte 0x21b8f83b
	.4byte 0x0492229e
	.4byte 0x20100409
	.4byte 0xfffcf000
	.4byte 0xf0002010
	.4byte 0x1c07ffd5
	.4byte 0x33231c3b
	.4byte 0x201060fd
	.4byte 0x2103701e
	.4byte 0xf828f001
	.4byte 0x22ae21e8
	.4byte 0x04090492
	.4byte 0xf0002011
	.4byte 0x2011ffe9
	.4byte 0xffc2f000
	.4byte 0x1c3b1c07
	.4byte 0x60fd3323
	.4byte 0x701e2011
	.4byte 0xf0012103
	.4byte 0x21b8f815
	.4byte 0x049222a6
	.4byte 0x20120409
	.4byte 0xffd6f000
	.4byte 0xf0002012
	.4byte 0x1c07ffaf
	.4byte 0x33231c3b
	.4byte 0x201260fd
	.4byte 0x2103701e
	.4byte 0xf802f001
	.4byte 0x20132107
	.4byte 0xffcaf000
	.4byte 0xf0002013
	.4byte 0x2100ff9f
	.4byte 0xff68f000
	.4byte 0x20132101
	.4byte 0xfff4f000
	.4byte 0xf0002013
	.4byte 0x1c07ff95
	.4byte 0x33591c3b
	.4byte 0x70194641
	.4byte 0x3b362296
	.4byte 0x701e21d7
	.4byte 0x04092013
	.4byte 0xf0000492
	.4byte 0x4b10ffab
	.4byte 0xf7fe66fb
	.2byte 0xfdb7
.L_02003fa6:
	movs	r0, #0
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0xfffe0000
	.4byte 0xffef8000
	.4byte 0x04000050
	.4byte 0x00000306
	.4byte 0x00000307
	.4byte 0x02000240
	.4byte SceneState_ApplyStepToSlots15To18
	.4byte 0xfff00000
	.2byte 0x8b99
	.2byte 0x0200
	.section .text.x0200c2bc,"ax",%progbits
	.balign 4
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #580]
	movs	r2, #224
	ldr	r3, [r3, #0]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #66
	str	r2, [r3, #0]
	sub	sp, #56
	bl 0x0200ce94
	movs	r0, #0
	bl 0x0200ceac
	movs	r1, #0
	bl 0x0200ce44
	movs	r1, #15
	movs	r0, #0
	bl 0x0200cf34
	movs	r0, #170
	bl 0x0200cfec
	bl 0x0200cfc4
	bl 0x0200cfd4
	movs	r0, #40
	bl 0x0200ce8c
	movs	r0, #162
	bl 0x0200cffc
	movs	r2, #16
	movs	r3, #0
	add	r2, sp
	mov	sl, r3
	mov	r8, r3
	mov	r9, r2
	mov	fp, r3
.L_0200431a:
	bl 0x0200cdb4
	ldr	r2, [pc, #500]
	lsls	r0, r0, #1
	lsrs	r0, r0, #16
	adds	r3, r0, #0
	muls	r3, r2
	ldr	r2, [pc, #492]
	adds	r3, r3, r2
	str	r3, [sp, #24]
	bl 0x0200cdb4
	ldr	r2, [pc, #480]
	lsls	r0, r0, #1
	lsrs	r0, r0, #16
	adds	r3, r0, #0
	muls	r3, r2
	ldr	r2, [pc, #472]
	adds	r3, r3, r2
	str	r3, [sp, #28]
	bl 0x0200cdb4
	movs	r3, #248
	lsls	r0, r0, #12
	lsrs	r0, r0, #16
	lsls	r3, r3, #8
	movs	r2, #50
	adds	r0, r0, r3
	add	r2, sp
	strh	r0, [r2, #0]
.L_02004356:
	mov	r3, r8
	movs	r6, #0
	cmp	r3, #7
	bhi.n	.L_020043a4
	movs	r5, #192
	lsls	r5, r5, #14
	movs	r7, #0
	add	r5, fp
.L_02004366:
	bl 0x0200cdb4
	adds	r3, r0, #0
	lsls	r0, r3, #3
	subs	r0, r0, r3
	movs	r3, #136
	lsls	r3, r3, #16
	lsrs	r0, r0, #16
	movs	r2, #216
	lsls	r2, r2, #18
	str	r3, [sp, #8]
	lsls	r0, r0, #19
	mov	r3, r9
	adds	r0, r0, r2
	str	r3, [sp, #12]
	adds	r2, r5, #0
	movs	r1, #0
	movs	r3, #0
	str	r7, [sp, #0]
	str	r7, [sp, #4]
	bl Effect_Spawn
	movs	r2, #128
	lsls	r2, r2, #11
	adds	r6, #1
	adds	r5, r5, r2
	cmp	r6, #3
	bhi.n	.L_020043a4
	mov	r3, r8
	cmp	r3, #7
	bls.n	.L_02004366
.L_020043a4:
	movs	r0, #3
	bl 0x0200cda4
	mov	r2, r8
	cmp	r2, #3
	bne.n	.L_020043bc
	mov	r3, sl
	cmp	r3, #2
	bhi.n	.L_020043bc
	movs	r2, #1
	add	sl, r2
	b.n	.L_02004356
.L_020043bc:
	mov	r3, r8
	adds	r3, #3
	movs	r2, #3
	movs	r1, #1
	str	r2, [sp, #0]
	str	r1, [sp, #4]
	movs	r2, #54
	adds	r1, r3, #0
	movs	r0, #48
	bl 0x0200ce1c
	movs	r3, #128
	movs	r2, #1
	lsls	r3, r3, #13
	add	r8, r2
	add	fp, r3
	mov	r3, r8
	cmp	r3, #9
	bls.n	.L_0200431a
	movs	r5, #5
	movs	r6, #2
	movs	r0, #111
	movs	r1, #5
	movs	r2, #117
	movs	r3, #5
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ce1c
	movs	r0, #111
	movs	r1, #10
	movs	r2, #117
	movs	r3, #10
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ce1c
	movs	r0, #111
	movs	r1, #7
	movs	r2, #111
	movs	r3, #5
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ce1c
	movs	r2, #111
	movs	r0, #111
	movs	r1, #7
	movs	r3, #10
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ce1c
	movs	r2, #0
	mov	r8, r2
	mov	sl, r9
	mov	fp, r2
.L_0200442e:
	bl 0x0200cdb4
	ldr	r2, [pc, #224]
	lsls	r0, r0, #1
	lsrs	r0, r0, #16
	adds	r3, r0, #0
	muls	r3, r2
	ldr	r2, [pc, #216]
	adds	r3, r3, r2
	mov	r2, sl
	str	r3, [r2, #8]
	bl 0x0200cdb4
	ldr	r2, [pc, #200]
	lsls	r0, r0, #1
	lsrs	r0, r0, #16
	adds	r3, r0, #0
	muls	r3, r2
	ldr	r2, [pc, #196]
	adds	r3, r3, r2
	mov	r2, sl
	str	r3, [r2, #12]
	bl 0x0200cdb4
	movs	r3, #248
	lsls	r0, r0, #12
	lsls	r3, r3, #8
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	mov	r2, sl
	mov	r3, r8
	strh	r0, [r2, #34]
	movs	r6, #0
	cmp	r3, #7
	bhi.n	.L_020044ba
	movs	r5, #192
	lsls	r5, r5, #14
	movs	r7, #0
	add	r5, fp
.L_0200447c:
	bl 0x0200cdb4
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsrs	r3, r3, #16
	movs	r2, #192
	lsls	r2, r2, #18
	lsls	r3, r3, #19
	adds	r3, r3, r2
	movs	r2, #136
	lsls	r2, r2, #16
	str	r2, [sp, #8]
	mov	r2, r9
	str	r2, [sp, #12]
	adds	r0, r3, #0
	adds	r2, r5, #0
	movs	r3, #0
	movs	r1, #0
	str	r7, [sp, #0]
	str	r7, [sp, #4]
	bl Effect_Spawn
.L_020044a8:
	movs	r3, #128
	lsls	r3, r3, #11
	adds	r6, #1
	adds	r5, r5, r3
	cmp	r6, #3
	bhi.n	.L_020044ba
	mov	r2, r8
	cmp	r2, #7
	bls.n	.L_0200447c
.L_020044ba:
	movs	r0, #3
	bl 0x0200cda4
	mov	r1, r8
	mov	r3, r8
	movs	r2, #3
	movs	r0, #1
	adds	r3, #3
	str	r2, [sp, #0]
	str	r0, [sp, #4]
	movs	r2, #48
	adds	r1, #26
	movs	r0, #55
	bl 0x0200ce1c
	movs	r3, #128
	movs	r2, #1
	lsls	r3, r3, #13
	add	r8, r2
	add	fp, r3
	mov	r3, r8
	cmp	r3, #9
	bls.n	.L_0200442e
	ldr	r0, [pc, #48]
	bl 0x0200cffc
	movs	r0, #60
	bl 0x0200ce8c
	movs	r0, #21
	bl 0x0200cfa4
	bl 0x0200ce9c
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001ebc
	.4byte 0x00004ccc
	.4byte 0x00017ffc
	.2byte 0x0121
	.2byte 0x0000
	.global VinasuHeya_SpawnRandomParticles
	.thumb_func
VinasuHeya_SpawnRandomParticles:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #184]
	sub	sp, #56
	add	r2, sp, #16
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	mov	r8, r2
	adds	r7, r0, #0
	mov	sl, r1
	bl 0x0200cdb4
	movs	r3, #248
	lsls	r0, r0, #12
	lsls	r3, r3, #8
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	mov	r2, r8
	ldr	r3, [pc, #156]
	strh	r0, [r2, #34]
	ldr	r6, [r3, #0]
	movs	r3, #3
	ands	r6, r3
	cmp	r6, #0
	bne.n	.L_020045d6
	bl 0x0200cdb4
	lsls	r0, r0, #1
	lsrs	r5, r0, #16
	cmp	r5, #0
	beq.n	.L_020045aa
	bl 0x0200cdb4
	adds	r5, r0, #0
	bl 0x0200cdb4
	adds	r3, r0, #0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsrs	r0, r0, #16
	movs	r3, #224
	lsls	r3, r3, #11
	lsls	r0, r0, #16
	adds	r0, r0, r3
	movs	r1, #10
	bl 0x0200cd9c
	lsls	r5, r5, #1
	mov	r3, sl
	lsrs	r5, r5, #16
	lsls	r5, r5, #4
	lsls	r2, r3, #19
	movs	r3, #136
	lsls	r3, r3, #16
	adds	r5, r7, r5
	lsls	r5, r5, #16
	str	r3, [sp, #8]
	mov	r3, r8
	str	r0, [sp, #4]
	str	r3, [sp, #12]
	adds	r0, r5, #0
	movs	r1, #0
	movs	r3, #0
	str	r6, [sp, #0]
	bl Effect_Spawn
	b.n	.L_020045d6
.L_020045aa:
	bl 0x0200cdb4
	adds	r3, r0, #0
	lsls	r0, r3, #4
	adds	r0, r0, r3
	ldr	r3, [pc, #52]
	lsls	r2, r7, #19
	adds	r2, r2, r3
	movs	r3, #136
	lsls	r3, r3, #16
.L_020045be:
	lsrs	r0, r0, #16
	adds	r0, r7, r0
	str	r3, [sp, #8]
	mov	r3, r8
	str	r3, [sp, #12]
	lsls	r0, r0, #16
	movs	r1, #0
	movs	r3, #0
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl Effect_Spawn
.L_020045d6:
	add	sp, #56
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0000b333
	.4byte 0x03001e40
	.2byte 0x0000
	.2byte 0xfffc
	.section .text.x0200cbd8,"ax",%progbits
	.balign 4
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	sub	sp, #12
	mov	r0, sp
	str	r3, [r0, #0]
	ldr	r1, [pc, #376]
	ldr	r3, [r5, #12]
	adds	r3, r3, r1
	str	r3, [r0, #4]
	ldr	r3, [r5, #16]
	movs	r1, #0
	str	r3, [r0, #8]
	bl SceneData_FindSlotAtPosition
	adds	r7, r0, #0
	ldr	r6, [r7, #80]
	ldr	r3, [r6, #40]
	movs	r1, #128
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	lsls	r1, r1, #1
	cmp	r3, r1
	beq.n	.L_02004c0a
	b.n	.L_02004d4c
.L_02004c0a:
	ldr	r2, [r5, #36]
	adds	r4, r2, #0
	cmp	r2, #0
	bge.n	.L_02004c14
	negs	r4, r2
.L_02004c14:
	ldr	r3, [r5, #44]
	adds	r1, r3, #0
	cmp	r3, #0
	bge.n	.L_02004c1e
	negs	r1, r3
.L_02004c1e:
	cmp	r4, r1
	ble.n	.L_02004c38
	adds	r3, r2, #0
	cmp	r3, #0
	bge.n	.L_02004c2c
	ldr	r2, [pc, #312]
	adds	r3, r3, r2
.L_02004c2c:
	cmp	r3, #0
	bge.n	.L_02004c34
	ldr	r4, [pc, #308]
	b.n	.L_02004c4a
.L_02004c34:
	ldr	r4, [pc, #308]
	b.n	.L_02004c4a
.L_02004c38:
	cmp	r3, #0
	bge.n	.L_02004c40
	ldr	r1, [pc, #292]
	adds	r3, r3, r1
.L_02004c40:
	cmp	r3, #0
	bge.n	.L_02004c48
	ldr	r4, [pc, #296]
	b.n	.L_02004c4a
.L_02004c48:
	ldr	r4, [pc, #296]
.L_02004c4a:
	ldrb	r1, [r4, #0]
	adds	r0, r1, #0
	cmp	r0, #0
	beq.n	.L_02004c74
	adds	r2, r6, #0
	adds	r2, #36
	ldrb	r3, [r2, #0]
	cmp	r3, r0
	beq.n	.L_02004c6e
	adds	r6, r2, #0
.L_02004c5e:
	adds	r4, #1
	ldrb	r1, [r4, #0]
	adds	r2, r1, #0
	cmp	r2, #0
	beq.n	.L_02004c74
	ldrb	r3, [r6, #0]
	cmp	r3, r2
	bne.n	.L_02004c5e
.L_02004c6e:
	adds	r3, r1, #0
	cmp	r3, #0
	bne.n	.L_02004c7e
.L_02004c74:
	adds	r0, r5, #0
	ldr	r1, [pc, #256]
	bl 0x0200cddc
	b.n	.L_02004d54
.L_02004c7e:
	ldr	r3, [pc, #252]
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #244]
	cmp	r2, r3
	bne.n	.L_02004ce8
	ldr	r0, [pc, #240]
	movs	r4, #0
	ldr	r6, [r5, #8]
	ldr	r3, [r0, r4]
	asrs	r2, r6, #20
	cmp	r2, r3
	bne.n	.L_02004ca8
	ldr	r3, [r5, #16]
	ldr	r2, [r0, #4]
	asrs	r3, r3, #20
	cmp	r3, r2
	beq.n	.L_02004cc4
.L_02004ca8:
	adds	r4, #1
	cmp	r4, #3
	bhi.n	.L_02004cc4
	lsls	r1, r4, #3
	ldr	r3, [r0, r1]
	asrs	r2, r6, #20
	cmp	r2, r3
	bne.n	.L_02004ca8
	ldr	r3, [r5, #16]
	adds	r2, r1, #4
	ldr	r2, [r0, r2]
	asrs	r3, r3, #20
	cmp	r3, r2
	bne.n	.L_02004ca8
.L_02004cc4:
	movs	r6, #0
	lsls	r4, r4, #2
	b.n	.L_02004cd0
.L_02004cca:
	adds	r3, r1, #1
	str	r3, [r0, r4]
	adds	r6, #1
.L_02004cd0:
	ldr	r0, [pc, #180]
	ldr	r1, [r0, r4]
	ldrb	r2, [r1, #0]
	cmp	r2, #0
	beq.n	.L_02004c74
	ldr	r3, [r7, #80]
	adds	r3, #36
	ldrb	r3, [r3, #0]
	cmp	r2, r3
	bne.n	.L_02004cca
	ldr	r3, [pc, #164]
	b.n	.L_02004d3e
.L_02004ce8:
	ldr	r0, [pc, #164]
	movs	r4, #0
	ldr	r6, [r5, #8]
	ldr	r3, [r0, r4]
	asrs	r2, r6, #20
	cmp	r2, r3
	bne.n	.L_02004d00
	ldr	r3, [r5, #16]
	ldr	r2, [r0, #4]
	asrs	r3, r3, #20
	cmp	r3, r2
	beq.n	.L_02004d1c
.L_02004d00:
	adds	r4, #1
	cmp	r4, #7
	bhi.n	.L_02004d1c
	lsls	r1, r4, #3
	ldr	r3, [r0, r1]
	asrs	r2, r6, #20
	cmp	r2, r3
	bne.n	.L_02004d00
	ldr	r3, [r5, #16]
	adds	r2, r1, #4
	ldr	r2, [r0, r2]
	asrs	r3, r3, #20
	cmp	r3, r2
	bne.n	.L_02004d00
.L_02004d1c:
	movs	r6, #0
	lsls	r4, r4, #2
	b.n	.L_02004d28
.L_02004d22:
	adds	r3, r1, #1
	str	r3, [r0, r4]
	adds	r6, #1
.L_02004d28:
	ldr	r0, [pc, #104]
	ldr	r1, [r0, r4]
	ldrb	r2, [r1, #0]
	cmp	r2, #0
	beq.n	.L_02004c74
	ldr	r3, [r7, #80]
	adds	r3, #36
	ldrb	r3, [r3, #0]
	cmp	r2, r3
	bne.n	.L_02004d22
	ldr	r3, [pc, #88]
.L_02004d3e:
	ldr	r2, [r3, r4]
	lsls	r3, r6, #2
	ldr	r1, [r3, r2]
	adds	r0, r5, #0
	bl 0x0200cddc
	b.n	.L_02004d54
.L_02004d4c:
	ldr	r1, [pc, #40]
	adds	r0, r5, #0
	bl 0x0200cddc
.L_02004d54:
	movs	r0, #0
	add	sp, #12
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0xfff00000
	.4byte 0x0000ffff
	.4byte 0x0200d1a4
	.4byte 0x0200d1a8
	.4byte 0x0200d1ac
	.4byte 0x0200d1b0
	.4byte 0x0200d564
	.4byte 0x02000240
	.4byte 0x000000b9
	.4byte 0x0200d128
	.4byte 0x0200f72c
	.4byte 0x0200f77c
	.4byte 0x0200d164
	.4byte 0x0200f78c
	.4byte 0x0200f7ec
@ The compiler library links here from its licensed container.
	.section .rodata.part1,"a",%progbits
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.global StagedActor_DirectionSteps
StagedActor_DirectionSteps:
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
	.global gVinasuSwitchCells
gVinasuSwitchCells:
	.4byte 0x00000030
	.4byte 0x00000029
	.4byte 0x00000034
	.4byte 0x00000029
	.4byte 0x00000030
	.4byte 0x0000002b
	.4byte 0x00000034
	.4byte 0x0000002b
	.global gVinasuBlockHeights
gVinasuBlockHeights:
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xffe00000
	.4byte 0xffd00000
	.4byte 0xffc00000
	.4byte 0xffb00000
	.4byte 0xffb00000
	.4byte 0x0000000b
	.4byte 0x00000027
	.4byte 0x0000000e
	.4byte 0x00000027
	.4byte 0x0000000b
	.4byte 0x00000029
	.4byte 0x00000010
	.4byte 0x0000002a
	.4byte 0x0000000a
	.4byte 0x0000002b
	.4byte 0x0000000e
	.4byte 0x0000002b
	.4byte 0x0000000a
	.4byte 0x0000002e
	.4byte 0x00000010
	.4byte 0x0000002e
	.4byte 0x00070605
	.4byte 0x00080604
	.4byte 0x00080703
	.4byte 0x00050403
	.4byte 0x00080706
	.4byte 0x00080706
	.4byte 0x00060504
	.4byte 0x00060504
	.4byte 0x05000007
	.4byte 0x05000800
	.4byte 0x00080700
	.4byte 0x00030003
	.global gEffectScripts
gEffectScripts:
	.4byte 0x0200d040
	.4byte 0x0200d078
	.4byte 0x0200d0b0
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00002126
	.4byte 0x00000015
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte OverlayObject_ApplyLowNibbleOfField100
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00002126
	.4byte 0x00000022
	.4byte OverlayObject_ApplyLowNibbleOfField100
	.4byte 0x00000010
	.global gVinasuLeaderApproachScript
gVinasuLeaderApproachScript:
	.4byte 0x0000001c
	.4byte 0x0000000c
	.4byte 0x00000015
	.4byte 0x00000026
	.4byte OverlayObject_UpdateEveryFourFrames
	.4byte 0x80010000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00011999
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00011999
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000e666
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000e666
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x0000000c
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00009999
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00009999
	.4byte 0x00000015
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte OverlayObject_ApplyZero
	.4byte 0x00000010
	.global gVinasuSprayScript
gVinasuSprayScript:
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000a000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00004000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000a000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x0000000c
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000004
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000006
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000007
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000008
	.4byte 0x00000010
	.global gVinasuPushScript
gVinasuPushScript:
	.4byte 0x00000022
	.4byte OverlayObject_ApplyZero
	.4byte 0x0000001c
	.4byte 0x00000007
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x00000026
	.4byte SceneEffect_SpawnRandomizedParticleEveryFourFrames
	.4byte 0x00000003
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000003
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000003
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02e70000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000003
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02e70000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global gVinasuSettleScriptA
gVinasuSettleScriptA:
	.4byte 0x00000003
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000003
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000003
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x03480000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.global gVinasuSettleScriptB
gVinasuSettleScriptB:
	.4byte 0x00000003
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000003
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x03480000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02b80000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000001b
	.4byte 0x00000003
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000003
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000015
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x03080000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02b80000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x03080000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x03480000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02b80000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x03080000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000003
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000015
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x03480000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x03080000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02b80000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x01080000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02e80000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000003
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000003
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x00e80000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02b80000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x00b80000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x01080000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02a80000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x00b80000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02780000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000003
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000003
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000003
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000003
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x00e80000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02780000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x00a80000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02e80000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x00a80000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02b80000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x00b80000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000015
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0x00000010
	.global gVinasuPushCells
gVinasuPushCells:
	.4byte 0x0032007c
	.4byte 0x00020001
	.4byte 0x007c0006
	.4byte 0x0001002f
	.4byte 0x00060002
	.4byte 0x002c007c
	.4byte 0x00020001
	.4byte 0x007c0006
	.4byte 0x00010029
	.4byte 0x00060002
	.2byte 0xffff
	.global gVinasuSettleCells
gVinasuSettleCells:
	.2byte 0x0075
	.4byte 0x0001003b
	.4byte 0x00060002
	.4byte 0x003b0073
	.4byte 0x00020001
	.4byte 0x00710006
	.4byte 0x0001003b
	.4byte 0x00060002
	.4byte 0x003b006f
	.4byte 0x00020001
	.4byte 0xffff0006
	.4byte 0x003a0060
	.4byte 0x00020001
	.4byte 0x005f0006
	.4byte 0x0001003a
	.4byte 0x00060002
	.4byte 0x00380060
	.4byte 0x00020001
	.4byte 0x005f0006
	.4byte 0x00010038
	.4byte 0x00060002
	.4byte 0x0000ffff
	.global gVinasuHeyaEntrances1
gVinasuHeyaEntrances1:
	.4byte 0xffff0001
	.4byte 0x00000028
	.4byte 0x00000108
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00000220
	.4byte 0xffff0002
	.4byte 0x00000178
	.4byte 0x40000058
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00000220
	.4byte 0xffff0003
	.4byte 0x000001e8
	.4byte 0x800001a8
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00000220
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaEntrancesOther
gVinasuHeyaEntrancesOther:
	.4byte 0xffff0001
	.4byte 0x00000078
	.4byte 0xc00000c8
	.4byte 0x00000000
	.4byte 0x00f00038
	.4byte 0x000000d8
	.4byte 0xffff0002
	.4byte 0x00000078
	.4byte 0x40000068
	.4byte 0x00000000
	.4byte 0x00f00038
	.4byte 0x000000d8
	.4byte 0xffff0003
	.4byte 0x00000158
	.4byte 0xc00000e8
	.4byte 0x01000000
	.4byte 0x02000030
	.4byte 0x000000f8
	.4byte 0xffff0004
	.4byte 0x000001d8
	.4byte 0x400000b8
	.4byte 0x01000000
	.4byte 0x02000030
	.4byte 0x000000f8
	.4byte 0xffff0005
	.4byte 0x00000178
	.4byte 0x400000b8
	.4byte 0x01000000
	.4byte 0x02000030
	.4byte 0x000000f8
	.4byte 0xffff0006
	.4byte 0x00000158
	.4byte 0x40000068
	.4byte 0x01000000
	.4byte 0x02000030
	.4byte 0x000000f8
	.4byte 0xffff0007
	.4byte 0x00000348
	.4byte 0xc00000f8
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff0008
	.4byte 0x000003c8
	.4byte 0x400000a8
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff0009
	.4byte 0x00000358
	.4byte 0x40000058
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff000a
	.4byte 0x00000328
	.4byte 0x40000058
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff000b
	.4byte 0x000002c8
	.4byte 0x400000a8
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff000c
	.4byte 0x000000c8
	.4byte 0xc0000198
	.4byte 0x00100000
	.4byte 0x01000120
	.4byte 0x00000208
	.4byte 0xffff000d
	.4byte 0x00000068
	.4byte 0xc00001f8
	.4byte 0x00100000
	.4byte 0x01000120
	.4byte 0x00000208
	.4byte 0xffff000e
	.4byte 0x00000068
	.4byte 0x400001d8
	.4byte 0x00100000
	.4byte 0x01000120
	.4byte 0x00000208
	.4byte 0xffff0014
	.4byte 0x00000208
	.4byte 0x400002b8
	.4byte 0x01300000
	.4byte 0x02400158
	.4byte 0x000002d8
	.4byte 0xffff0015
	.4byte 0x00000158
	.4byte 0x40000238
	.4byte 0x01300000
	.4byte 0x02400158
	.4byte 0x000002d8
	.4byte 0xffff0016
	.4byte 0x000002d8
	.4byte 0x400003a8
	.4byte 0x02580000
	.4byte 0x034802e0
	.4byte 0x000003b8
	.4byte 0xffff0017
	.4byte 0x000002e8
	.4byte 0x40000318
	.4byte 0x02580000
	.4byte 0x034802e0
	.4byte 0x000003b8
	.4byte 0xffff0018
	.4byte 0x00000188
	.4byte 0x400003b8
	.4byte 0x01100000
	.4byte 0x02000328
	.4byte 0x000003d8
	.4byte 0xffff0019
	.4byte 0x00000188
	.4byte 0x40000358
	.4byte 0x01100000
	.4byte 0x02000328
	.4byte 0x000003d8
	.4byte 0xffff001a
	.4byte 0x000002f8
	.4byte 0x400003a8
	.4byte 0x02580000
	.4byte 0x034802e0
	.4byte 0x000003b8
	.4byte 0xffff001e
	.4byte 0x00000058
	.4byte 0x40000368
	.4byte 0x00100000
	.4byte 0x01000258
	.4byte 0x00000398
	.4byte 0xffff001f
	.4byte 0x000000b8
	.4byte 0x40000368
	.4byte 0x00100000
	.4byte 0x01000258
	.4byte 0x00000398
	.4byte 0xffff0020
	.4byte 0x000000b8
	.4byte 0x40000288
	.4byte 0x00100000
	.4byte 0x01000258
	.4byte 0x00000398
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaEntrances3
gVinasuHeyaEntrances3:
	.4byte 0xffff0001
	.4byte 0x000001c8
	.4byte 0x40000228
	.4byte 0x01780000
	.4byte 0x02e00158
	.4byte 0x00000270
	.4byte 0xffff0002
	.4byte 0x000001c8
	.4byte 0xc0000250
	.4byte 0x01780000
	.4byte 0x02e00158
	.4byte 0x00000270
	.4byte 0xffff0003
	.4byte 0x000002b8
	.4byte 0x400001d8
	.4byte 0x01780000
	.4byte 0x02e00158
	.4byte 0x00000270
	.4byte 0xffff0004
	.4byte 0x000002b8
	.4byte 0xc0000200
	.4byte 0x01780000
	.4byte 0x02e00158
	.4byte 0x00000270
	.4byte 0xffff0005
	.4byte 0x000001a8
	.4byte 0x40000088
	.4byte 0x01580000
	.4byte 0x02500030
	.4byte 0x000000d8
	.4byte 0xffff0006
	.4byte 0x00000208
	.4byte 0xc00000b0
	.4byte 0x01580000
	.4byte 0x02500030
	.4byte 0x000000d8
	.4byte 0xffff0007
	.4byte 0x00000068
	.4byte 0xc00002f0
	.4byte 0x00380000
	.4byte 0x01300248
	.4byte 0x00000370
	.4byte 0xffff0008
	.4byte 0x000000c8
	.4byte 0x400002a8
	.4byte 0x00380000
	.4byte 0x01300248
	.4byte 0x00000370
	.4byte 0xffff0009
	.4byte 0x00000058
	.4byte 0x40000158
	.4byte 0x00280000
	.4byte 0x01200118
	.4byte 0x000001f0
	.4byte 0xffff000a
	.4byte 0x000000f8
	.4byte 0xc00001b0
	.4byte 0x00280000
	.4byte 0x01200118
	.4byte 0x000001f0
	.4byte 0xffff000b
	.4byte 0x00000298
	.4byte 0x40000108
	.4byte 0x02680000
	.4byte 0x03e80028
	.4byte 0x00000168
	.4byte 0xffff000c
	.4byte 0x000002d8
	.4byte 0x40000088
	.4byte 0x02680000
	.4byte 0x03e80028
	.4byte 0x00000168
	.4byte 0xffff000d
	.4byte 0x00000348
	.4byte 0x40000088
	.4byte 0x02680000
	.4byte 0x03e80028
	.4byte 0x00000168
	.4byte 0xffff000e
	.4byte 0x000003b8
	.4byte 0x40000088
	.4byte 0x02680000
	.4byte 0x03e80028
	.4byte 0x00000168
	.4byte 0xffff000f
	.4byte 0x00000348
	.4byte 0xc0000168
	.4byte 0x02680000
	.4byte 0x03e80028
	.4byte 0x00000168
	.4byte 0xffff0010
	.4byte 0x00000058
	.4byte 0x40000098
	.4byte 0x00280000
	.4byte 0x01480028
	.4byte 0x000000d0
	.4byte 0xffff0011
	.4byte 0x000000b8
	.4byte 0x40000068
	.4byte 0x00280000
	.4byte 0x01480028
	.4byte 0x000000d0
	.4byte 0xffff0012
	.4byte 0x00000118
	.4byte 0x40000098
	.4byte 0x00280000
	.4byte 0x01480028
	.4byte 0x000000d0
	.4byte 0xffff0013
	.4byte 0x000001d8
	.4byte 0x40000308
	.4byte 0x01a80000
	.4byte 0x02a002b0
	.4byte 0x00000350
	.4byte 0xffff0014
	.4byte 0x00000258
	.4byte 0x40000308
	.4byte 0x01a80000
	.4byte 0x02a002b0
	.4byte 0x00000350
	.4byte 0xffff0015
	.4byte 0x00000348
	.4byte 0x40000088
	.4byte 0x02680000
	.4byte 0x03e80028
	.4byte 0x00000168
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaEntrances4
gVinasuHeyaEntrances4:
	.4byte 0xffff0001
	.4byte 0x00000068
	.4byte 0x40000078
	.4byte 0x00380000
	.4byte 0x01700038
	.4byte 0x00000110
	.4byte 0xffff0002
	.4byte 0x00000138
	.4byte 0x400000c8
	.4byte 0x00380000
	.4byte 0x01700038
	.4byte 0x00000110
	.4byte 0xffff0003
	.4byte 0x00000258
	.4byte 0xc00000d8
	.4byte 0x02280000
	.4byte 0x03500058
	.4byte 0x00000220
	.4byte 0xffff0004
	.4byte 0x000002b8
	.4byte 0x40000138
	.4byte 0x02280000
	.4byte 0x03500058
	.4byte 0x00000220
	.4byte 0xffff0005
	.4byte 0x00000288
	.4byte 0xc0000218
	.4byte 0x02280000
	.4byte 0x03500058
	.4byte 0x00000220
	.4byte 0xffff0006
	.4byte 0x00000308
	.4byte 0x40000218
	.4byte 0x02280000
	.4byte 0x03500058
	.4byte 0x00000220
	.4byte 0xffff0007
	.4byte 0x00000068
	.4byte 0x40000208
	.4byte 0x00380000
	.4byte 0x01600188
	.4byte 0x00000260
	.4byte 0xffff0008
	.4byte 0x00000138
	.4byte 0xc0000248
	.4byte 0x00380000
	.4byte 0x01600188
	.4byte 0x00000260
	.4byte 0xffff0009
	.4byte 0x000002e8
	.4byte 0x400002b8
	.4byte 0x02880000
	.4byte 0x03800278
	.4byte 0x00000320
	.4byte 0xffff000a
	.4byte 0x00000328
	.4byte 0x400002b8
	.4byte 0x02880000
	.4byte 0x03800278
	.4byte 0x00000320
	.4byte 0xffff000b
	.4byte 0x00000138
	.4byte 0x40000218
	.4byte 0x00380000
	.4byte 0x01600188
	.4byte 0x00000260
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaEntrances5
gVinasuHeyaEntrances5:
	.4byte 0xffff0001
	.4byte 0x00000088
	.4byte 0x40000088
	.4byte 0x00080000
	.4byte 0x01000038
	.4byte 0x000000e0
	.4byte 0xffff0002
	.4byte 0x00000088
	.4byte 0xc00000b8
	.4byte 0x00080000
	.4byte 0x01000038
	.4byte 0x000000e0
	.4byte 0xffff0003
	.4byte 0x00000088
	.4byte 0xc0000188
	.4byte 0x00080000
	.4byte 0x01000108
	.4byte 0x000001b0
	.4byte 0xffff0004
	.4byte 0x00000138
	.4byte 0x400002a8
	.4byte 0x00f80000
	.4byte 0x02000258
	.4byte 0x00000300
	.4byte 0xffff0005
	.4byte 0x00000188
	.4byte 0x400002a8
	.4byte 0x00f80000
	.4byte 0x02000258
	.4byte 0x00000300
	.4byte 0xffff0006
	.4byte 0x000001d8
	.4byte 0x400002a8
	.4byte 0x00f80000
	.4byte 0x02000258
	.4byte 0x00000300
	.4byte 0xffff0007
	.4byte 0x00000088
	.4byte 0x40000318
	.4byte 0x00080000
	.4byte 0x010002d8
	.4byte 0x000003a0
	.4byte 0xffff0008
	.4byte 0x00000088
	.4byte 0xc0000388
	.4byte 0x00080000
	.4byte 0x010002d8
	.4byte 0x000003a0
	.4byte 0xffff0009
	.4byte 0x00000138
	.4byte 0xc00000e8
	.4byte 0x01080000
	.4byte 0x02d00038
	.4byte 0x000001e0
	.4byte 0xffff000a
	.4byte 0x00000178
	.4byte 0x400000b8
	.4byte 0x01080000
	.4byte 0x02d00038
	.4byte 0x000001e0
	.4byte 0xffff000b
	.4byte 0x000001f8
	.4byte 0x400000f8
	.4byte 0x01080000
	.4byte 0x02d00038
	.4byte 0x000001e0
	.4byte 0xffff000c
	.4byte 0x000002a8
	.4byte 0x40000078
	.4byte 0x01080000
	.4byte 0x02d00038
	.4byte 0x000001e0
	.4byte 0xffff000d
	.4byte 0x00000138
	.4byte 0x400001b8
	.4byte 0x01080000
	.4byte 0x02d00038
	.4byte 0x000001e0
	.4byte 0xffff000e
	.4byte 0x00000278
	.4byte 0xc0000188
	.4byte 0x01080000
	.4byte 0x02d00038
	.4byte 0x000001e0
	.4byte 0xffff000f
	.4byte 0x000002d8
	.4byte 0x40000278
	.4byte 0x02a80000
	.4byte 0x03a00238
	.4byte 0x00000300
	.4byte 0xffff0010
	.4byte 0x00000378
	.4byte 0x40000278
	.4byte 0x02a80000
	.4byte 0x03a00238
	.4byte 0x00000300
	.4byte 0xffff0011
	.4byte 0x00000088
	.4byte 0x40000238
	.4byte 0x00080000
	.4byte 0x010001e0
	.4byte 0x00000288
	.4byte 0xffff0012
	.4byte 0x00000088
	.4byte 0xc0000258
	.4byte 0x00080000
	.4byte 0x010001e0
	.4byte 0x00000288
	.4byte 0xffff0013
	.4byte 0x000001b8
	.4byte 0xc0000108
	.4byte 0x01080000
	.4byte 0x02d00038
	.4byte 0x000001e0
	.4byte 0xffff0014
	.4byte 0x00000238
	.4byte 0xc0000108
	.4byte 0x01080000
	.4byte 0x02d00038
	.4byte 0x000001e0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaEntrances6
gVinasuHeyaEntrances6:
	.4byte 0xffff0001
	.4byte 0x00000078
	.4byte 0x400000f8
	.4byte 0x00480000
	.4byte 0x01400038
	.4byte 0x000001a0
	.4byte 0xffff0002
	.4byte 0x00000118
	.4byte 0xc0000118
	.4byte 0x00480000
	.4byte 0x01400038
	.4byte 0x000001a0
	.4byte 0xffff0003
	.4byte 0x000001d8
	.4byte 0x40000148
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff0004
	.4byte 0x00000218
	.4byte 0x400000d8
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff0005
	.4byte 0x00000298
	.4byte 0x40000058
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff0006
	.4byte 0x000002e8
	.4byte 0x40000098
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff0007
	.4byte 0x00000348
	.4byte 0x40000098
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff0008
	.4byte 0x000003a8
	.4byte 0xc0000118
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff0009
	.4byte 0x000001d8
	.4byte 0x400001d8
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff000a
	.4byte 0x00000268
	.4byte 0xc00001c8
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff000b
	.4byte 0x000002d8
	.4byte 0xc00001c8
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff000c
	.4byte 0x00000338
	.4byte 0xc00001c8
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff000d
	.4byte 0x00000318
	.4byte 0x40000288
	.4byte 0x02c80000
	.4byte 0x03d00248
	.4byte 0x00000360
	.4byte 0xffff000e
	.4byte 0x00000388
	.4byte 0x40000288
	.4byte 0x02c80000
	.4byte 0x03d00248
	.4byte 0x00000360
	.4byte 0xffff000f
	.4byte 0x00000068
	.4byte 0x40000308
	.4byte 0x00380000
	.4byte 0x01300218
	.4byte 0x00000350
	.4byte 0xffff0010
	.4byte 0x000000d8
	.4byte 0x40000258
	.4byte 0x00380000
	.4byte 0x01300218
	.4byte 0x00000350
	.4byte 0xffff0011
	.4byte 0x00000258
	.4byte 0x400002e8
	.4byte 0x01880000
	.4byte 0x02800288
	.4byte 0x00000340
	.4byte 0xffff0012
	.4byte 0x00000218
	.4byte 0x40000148
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff0013
	.4byte 0x00000318
	.4byte 0x40000148
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff0014
	.4byte 0x00000298
	.4byte 0x40000088
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaPrimaryTable
gVinasuHeyaPrimaryTable:
	.4byte 0x000000b5
	.4byte 0x00120002
	.4byte 0x002010b6
	.4byte 0x0033d002
	.4byte 0x000000b6
	.4byte 0x001020b5
	.4byte 0x002030b6
	.4byte 0x003020b6
	.4byte 0x0041f0b6
	.4byte 0x0051e0b6
	.4byte 0x006070b6
	.4byte 0x007060b6
	.4byte 0x008200b6
	.4byte 0x009160b6
	.4byte 0x00a0c0b6
	.4byte 0x00b0d0b6
	.4byte 0x00c0a0b6
	.4byte 0x00d0b0b6
	.4byte 0x00e180b6
	.4byte 0x0141a0b6
	.4byte 0x015190b6
	.4byte 0x016090b6
	.4byte 0x017100b7
	.4byte 0x0180e0b6
	.4byte 0x019150b6
	.4byte 0x01a140b6
	.4byte 0x01e050b6
	.4byte 0x01f040b6
	.4byte 0x020080b6
	.4byte 0x000000b7
	.4byte 0x001130b7
	.4byte 0x002090b7
	.4byte 0x003010b8
	.4byte 0x004050b7
	.4byte 0x005040b7
	.4byte 0x0060d0b7
	.4byte 0x0070e0b7
	.4byte 0x008040b8
	.4byte 0x009020b7
	.4byte 0x00a0b0b7
	.4byte 0x00b0a0b7
	.4byte 0x00c070b8
	.4byte 0x00d060b7
	.4byte 0x00e070b7
	.4byte 0x00f110b7
	.4byte 0x010170b6
	.4byte 0x0110f0b7
	.4byte 0x012100ad
	.4byte 0x013010b7
	.4byte 0x014140b7
	.4byte 0x0150b0b8
	.4byte 0x000000b8
	.4byte 0x001030b7
	.4byte 0x002030b8
	.4byte 0x003020b8
	.4byte 0x004080b7
	.4byte 0x0050a0b8
	.4byte 0x006110b9
	.4byte 0x0070c0b7
	.4byte 0x008090b8
	.4byte 0x009080b8
	.4byte 0x00a050b8
	.4byte 0x00b150b7
	.4byte 0x000000b9
	.4byte 0x001010ba
	.4byte 0x002040b9
	.4byte 0x003050b9
	.4byte 0x004020b9
	.4byte 0x005030b9
	.4byte 0x006090b9
	.4byte 0x007050ba
	.4byte 0x0080b0b9
	.4byte 0x009060b9
	.4byte 0x00a040ba
	.4byte 0x00b080b9
	.4byte 0x00c070ba
	.4byte 0x00d090ba
	.4byte 0x00e0f0b9
	.4byte 0x00f0e0b9
	.4byte 0x010120b9
	.4byte 0x011060b8
	.4byte 0x012100b9
	.4byte 0x013120ba
	.4byte 0x014130ba
	.4byte 0x000000ba
	.4byte 0x001010b9
	.4byte 0x002030ba
	.4byte 0x003020ba
	.4byte 0x0040a0b9
	.4byte 0x005070b9
	.4byte 0x006110ba
	.4byte 0x0070c0b9
	.4byte 0x008100ba
	.4byte 0x0090d0b9
	.4byte 0x00a0d0ba
	.4byte 0x00b0e0ba
	.4byte 0x00c0f0ba
	.4byte 0x00d0a0ba
	.4byte 0x00e0b0ba
	.4byte 0x00f0c0ba
	.4byte 0x010080ba
	.4byte 0x011060ba
	.4byte 0x012130b9
	.4byte 0x013140b9
	.4byte 0x014140ba
	.4byte 0x015010bb
	.4byte 0x000001ff
	.global gVinasuHeyaPlacementsOther
gVinasuHeyaPlacementsOther:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaPlacements1
gVinasuHeyaPlacements1:
	.4byte 0x09810098
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x010c0000
	.4byte 0x00024000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00024000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x01ac0000
	.4byte 0x00024000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x008c0000
	.4byte 0x00024000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x01900000
	.4byte 0x00000000
	.4byte 0x00ac0000
	.4byte 0x00024000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x019c0000
	.4byte 0x00024000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x016c0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaPlacements2
gVinasuHeyaPlacements2:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x00000070
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x008c0000
	.4byte 0x00024000
	.4byte 0x00000071
	.4byte 0x00000001
	.4byte 0x01300000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x00024000
	.4byte 0x00000070
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x02500000
	.4byte 0x00000000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x0102c000
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00024000
	.4byte 0x00000114
	.4byte 0x0200d29c
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00024000
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00024000
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01024000
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01024000
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01024000
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00024000
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaPlacements3
gVinasuHeyaPlacements3:
	.4byte 0x000000fd
	.4byte 0x0200d204
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00024000
	.4byte 0x000000fd
	.4byte 0x0200d204
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x01024000
	.4byte 0x000000fd
	.4byte 0x0200d204
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x01024000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0102c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaPlacements4
gVinasuHeyaPlacements4:
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x0200d37c
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x0002c000
	.4byte 0xffff011c
	.4byte 0x00000007
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaPlacements5
gVinasuHeyaPlacements5:
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x0200d3ac
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x0200d3a0
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x0200d388
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x0002c000
	.4byte 0xffff011c
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaPlacements6
gVinasuHeyaPlacements6:
	.4byte 0x00000101
	.4byte 0x00000007
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0002c000
	.4byte 0x00000101
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0102c000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0102c000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0102c000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0102c000
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x0200d37c
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x0200d3ac
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x0200d3b8
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x0200d394
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0002c000
	.4byte 0xffff011c
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0x000000f2
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaEvents1
gVinasuHeyaEvents1:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte FieldScene_RunActorEightTenStepLoop
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000266e
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0000266f
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte SceneDialogue_RunActorElevenDialogue
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002672
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002673
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002674
	.4byte 0x00008d15
	.4byte 0xffff0408
	.4byte FieldScene_RunActorEightTenStepLoop
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002675
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002676
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002677
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002678
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002679
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000267a
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaEvents2
gVinasuHeyaEvents2:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000031
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000021
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000021
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000031
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000031
	.4byte 0xffff0015
	.4byte 0x00000015
	.4byte 0x00000031
	.4byte 0xffff0016
	.4byte 0x00000016
	.4byte 0x00000031
	.4byte 0xffff0017
	.4byte 0x00000017
	.4byte 0x00000031
	.4byte 0xffff0018
	.4byte 0x00000018
	.4byte 0x00000021
	.4byte 0xffff0019
	.4byte 0x00000019
	.4byte 0x00000021
	.4byte 0xffff001a
	.4byte 0x0000001a
	.4byte 0x00000021
	.4byte 0xffff001e
	.4byte 0x0000001e
	.4byte 0x00000021
	.4byte 0xffff001f
	.4byte 0x0000001f
	.4byte 0x00000021
	.4byte 0xffff0020
	.4byte 0x00000020
	.4byte 0x00000002
	.4byte 0x0200002d
	.4byte FieldScene_RunLeaderSurpriseApproach
	.4byte 0x00000002
	.4byte 0xffff002e
	.4byte VinasuHeya_UpdateFloorSwitch
	.4byte 0x00000202
	.4byte 0xffff0032
	.4byte FieldScene_RunFiveCallSequence
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000267b
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000267c
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte FieldScene_SetupActorTenCamera
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000267f
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002680
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002681
	.4byte 0x00000003
	.4byte 0xffff0023
	.4byte SceneDialogue_ReadRelief
	.4byte 0x00000003
	.4byte 0xffff0029
	.4byte FieldScene_RunStatueDialogueSequence
	.4byte 0x00000013
	.4byte 0x0f370064
	.4byte 0x001000a1
	.4byte 0x00000013
	.4byte 0x0f380065
	.4byte 0x001000ce
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte FieldScene_RunFlag986ActorOneScene
	.4byte 0x00008c15
	.4byte 0x0200000d
	.4byte SceneState_RunActor13AtColumn42Setup
	.4byte 0x00009315
	.4byte 0xffff000c
	.4byte FieldScene_SetFlag987AtActorTwelveTile
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte SceneState_ApplySixRectsAfterFlag161
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte SceneState_ApplySixRectsAfter161
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaEvents3
gVinasuHeyaEvents3:
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000021
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000021
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000021
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000031
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000021
	.4byte 0xffff0013
	.4byte 0x00000013
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte SceneState_PassRange0To1
	.4byte 0x00000002
	.4byte 0xffff0023
	.4byte Scene_RunActorLeapSequence
	.4byte 0x00000202
	.4byte 0xffff001f
	.4byte SceneState_ApplyRectAt19_44AndRunThree
	.4byte 0x00000202
	.4byte 0xffff0020
	.4byte SceneState_ApplyRectAt19_44AndRunThree
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte SceneState_ClearWorkspaceWord24
	.4byte 0x00000002
	.4byte 0xffff0020
	.4byte SceneState_StoreLookupZeroToWord24
	.4byte 0x00000202
	.4byte 0xffff002d
	.4byte FieldScene_RunGuardedThreeStepSetup
	.4byte 0x00000013
	.4byte 0x0f340064
	.4byte 0x001000a2
	.4byte 0x00000003
	.4byte 0x0350006e
	.4byte 0x00300000
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte FieldScene_PlaceAndPinSlots8To10
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte FieldScene_PlaceAndPinSlots8To10
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte FieldScene_PlaceAndPinSlots8To10
	.4byte 0x10008c15
	.4byte 0xffff000b
	.4byte SceneActor_ApplyKind45AtActorsElevenAndTwelve
	.4byte 0x10008c15
	.4byte 0xffff000c
	.4byte SceneActor_ApplyKind45AtActorsElevenAndTwelve
	.4byte 0x10009315
	.4byte 0xffff000b
	.4byte SceneActor_ApplyKind45AtActorsElevenAndTwelve
	.4byte 0x10009315
	.4byte 0xffff000c
	.4byte SceneActor_ApplyKind45AtActorsElevenAndTwelve
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte SceneActor_ApplyPositionsOfActors11And12
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte SceneActor_ApplyPositionsOfActors11And12
	.4byte 0x00009315
	.4byte 0xffff000b
	.4byte SceneActor_UpdateSlots11And12ByTile
	.4byte 0x00009315
	.4byte 0xffff000c
	.4byte SceneActor_UpdateSlots11And12ByTile
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaEvents4
gVinasuHeyaEvents4:
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte SceneEffect_RunObjectZeroColorSequence
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000021
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000002
	.4byte 0xffff0023
	.4byte VinasuHeya_ShiftBridge
	.4byte 0x00000202
	.4byte 0x03010024
	.4byte FieldScene_RunGuardedRectStep
	.4byte 0x00000003
	.4byte 0xffff0019
	.4byte SceneState_SetFlag953
	.4byte 0x00008c15
	.4byte 0x03010009
	.4byte VinasuHeya_RunCellPushScene
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaEvents5
gVinasuHeyaEvents5:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte SceneEffect_RunObjectZeroColorSequence
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte SceneEffect_RunObjectZeroColorSequence
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000021
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte SceneEffect_RunObjectZeroColorSequence
	.4byte 0x00000021
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000021
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000031
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte SceneState_PassRange0To1
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte SceneState_PassRange0To1
	.4byte 0x00000202
	.4byte 0xffff0023
	.4byte SceneState_RunConditionalStep
	.4byte 0x00000003
	.4byte 0xffff0019
	.4byte SceneState_SetFlag953
	.4byte 0x00000013
	.4byte 0x0f350064
	.4byte 0x00100052
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte VinasuHeya_SettlePushedBlocks
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte VinasuHeya_SettlePushedBlocks
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte VinasuHeya_SettlePushedBlocks
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gVinasuHeyaEvents6
gVinasuHeyaEvents6:
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000021
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000031
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000031
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00004602
	.4byte 0xffff0012
	.4byte FieldScene_RunApproachAndSpawnEffect
	.4byte 0x00000202
	.4byte 0xffff0023
	.4byte FieldScene_RunThreeCallSequence
	.4byte 0x00008602
	.4byte 0xffff0024
	.4byte SceneActor_TryMoveActorZeroTwoTilesAhead
	.4byte 0x00000602
	.4byte 0xffff0024
	.4byte SceneActor_TryMoveActorZeroTwoTilesAhead
	.4byte 0x00000202
	.4byte 0xffff0024
	.4byte FieldScene_RunThreeCallSequence
	.4byte 0x00000602
	.4byte 0xffff0025
	.4byte SceneActor_TryMoveActorZeroTwoTilesAhead
	.4byte 0x00000202
	.4byte 0xffff0025
	.4byte FieldScene_RunThreeCallSequence
	.4byte 0x00000002
	.4byte 0xffff0028
	.4byte VinasuHeya_RetractBridge
	.4byte 0x00000002
	.4byte 0xffff0029
	.4byte VinasuHeya_ExtendBridge
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte SceneState_PassRange0To1
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte SceneState_PassZeroAndMinusOneRecord
	.4byte 0x00000002
	.4byte 0xffff0020
	.4byte SceneState_PassRangeNeg1To0
	.4byte 0x00000002
	.4byte 0xffff0021
	.4byte SceneState_CallHandlerWithFlagPair
	.4byte 0x00000202
	.4byte 0xffff002d
	.4byte FieldScene_DrawTilesWhenCheckClear
	.4byte 0x00000003
	.4byte 0xffff0019
	.4byte SceneState_SetFlag953
	.4byte 0x00000013
	.4byte 0x0f360064
	.4byte 0x00100009
	.4byte 0x00000003
	.4byte 0x0351006e
	.4byte 0x00300000
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000d
	.4byte 0x00000000
	.4byte 0x00009315
	.4byte 0xffff000a
	.4byte 0x0200a47d
	.4byte 0x00009315
	.4byte 0xffff000b
	.4byte 0x0200a47d
	.4byte 0x00009315
	.4byte 0xffff000c
	.4byte 0x0200a47d
	.4byte 0x00009315
	.4byte 0xffff000d
	.4byte 0x0200a47d
	.4byte 0x00008c15
	.4byte 0xffff000f
	.4byte 0x0200ab15
	.4byte 0x00008c15
	.4byte 0xffff0010
	.4byte 0x0200ab15
	.4byte 0x00008c15
	.4byte 0xffff0011
	.4byte 0x0200ab15
	.4byte 0x00008c15
	.4byte 0xffff0012
	.4byte 0x0200ab15
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0200d1b4
	.4byte 0x0200d1b8
	.4byte 0x0200d1bc
	.4byte 0x0200d1c0
	.4byte 0x0200d57c
	.4byte 0x0200d5d4
	.4byte 0x0200d564
	.4byte 0x0200d564
	.4byte 0x0200d60c
	.4byte 0x0200d644
	.4byte 0x0200d564
	.4byte 0x0200d564
	.4byte 0x0200d6b4
	.4byte 0x0200d67c
	.4byte 0x0200d6b4
	.4byte 0x0200d564
	.4byte 0x0200d744
	.4byte 0x0200d70c
	.4byte 0x0200d744
	.4byte 0x0200d564
	.4byte 0x0200f73c
	.4byte 0x0200f74c
	.4byte 0x0200f75c
	.4byte 0x0200f76c
	.4byte 0x0200d1c4
	.4byte 0x0200d1c6
	.4byte 0x0200d1c7
	.4byte 0x0200d1c9
	.4byte 0x0200d1cb
	.4byte 0x0200d1cd
	.4byte 0x0200d1d0
	.4byte 0x0200d1d2
	.4byte 0x0200dc90
	.4byte 0x0200d564
	.4byte 0x0200d564
	.4byte 0x0200d980
	.4byte 0x0200d564
	.4byte 0x0200d9b8
	.4byte 0x0200d564
	.4byte 0x0200dc34
	.4byte 0x0200d564
	.4byte 0x0200d924
	.4byte 0x0200d8a4
	.4byte 0x0200d564
	.4byte 0x0200db90
	.4byte 0x0200d564
	.4byte 0x0200d824
	.4byte 0x0200d564
	.4byte 0x0200f7ac
	.4byte 0x0200f7b4
	.4byte 0x0200f7b8
	.4byte 0x0200f7c0
	.4byte 0x0200f7c8
	.4byte 0x0200f7d0
	.4byte 0x0200f7dc
	.4byte 0x0200f7e4
