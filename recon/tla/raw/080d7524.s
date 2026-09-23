.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_0801314c, 0x0801314c
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014cc0, 0x08014cc0
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016cfc, 0x08016cfc
	.set sub_08020090, 0x08020090
	.set sub_080200c0, 0x080200c0
	.set sub_080200e8, 0x080200e8
	.set sub_08020218, 0x08020218
	.set sub_08020278, 0x08020278
	.set sub_08038328, 0x08038328
	.set sub_080ad248, 0x080ad248
	.set sub_080cad84, 0x080cad84
	.set sub_080cb82c, 0x080cb82c
	.set sub_080cb8a4, 0x080cb8a4
	.set sub_080cccb8, 0x080cccb8
	.set sub_080d22a8, 0x080d22a8
	.set sub_080d2350, 0x080d2350
	.set sub_080d2d84, 0x080d2d84
	.set sub_080d32c8, 0x080d32c8
	.set sub_080d332c, 0x080d332c
	.set sub_080d3838, 0x080d3838
	.set sub_080d47b4, 0x080d47b4
	.set sub_080d7c04, 0x080d7c04
	.set sub_080d7f80, 0x080d7f80
	.set sub_080d82e0, 0x080d82e0
	.set sub_080d8740, 0x080d8740
	.set sub_080db974, 0x080db974
	.set sub_080dc0d8, 0x080dc0d8
	.set sub_080ebc30, 0x080ebc30
	.set sub_080ebf68, 0x080ebf68
	.set sub_081c0010, 0x081c0010
	.global Func_080d7524
	.thumb_func
Func_080d7524:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #52
	mov	r9, r0
	bl	sub_080d2d84
	ldr	r3, [pc, #564]
	adds	r7, r0, #0
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl	sub_080d2d84
	str	r0, [sp, #16]
	movs	r1, #128
	ldrh	r3, [r0, #6]
	lsls	r1, r1, #6
	adds	r1, r3, r1
	movs	r3, #192
	lsls	r3, r3, #8
	ands	r1, r3
	str	r1, [sp, #8]
	bl	sub_080d22a8
	movs	r0, #10
	bl	sub_08013560
	movs	r0, #173
	bl	sub_081c0010
	movs	r1, #1
	mov	r0, r9
	bl	sub_080d332c
	movs	r0, #175
	bl	sub_081c0010
	movs	r1, #1
	mov	r0, r9
	bl	sub_080d332c
	movs	r0, #10
	bl	sub_08013560
	ldr	r2, [sp, #8]
	movs	r3, #128
	lsls	r3, r3, #8
	adds	r5, r2, r3
	adds	r1, r5, #0
	movs	r2, #0
	mov	r0, r9
	bl	sub_080d3838
	movs	r0, #10
	bl	sub_08013560
	movs	r1, #4
	adds	r1, #255
	movs	r2, #50
	strh	r5, [r7, #6]
	mov	r0, r9
	bl	sub_080d47b4
	mov	r0, r9
	bl	sub_080d2d84
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	adds	r0, r7, #0
	adds	r0, #85
	movs	r3, #2
	str	r0, [sp, #4]
	strb	r3, [r0, #0]
	ldr	r3, [pc, #424]
	movs	r0, #152
	str	r3, [r7, #108]
	bl	sub_081c0010
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #72]
	movs	r3, #144
	lsls	r3, r3, #11
	str	r3, [r7, #40]
	movs	r0, #33
	bl	sub_08013560
	ldr	r1, [r7, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r1, #9]
	movs	r0, #152
	bl	sub_081c0010
	ldr	r3, [pc, #384]
	movs	r0, #35
	str	r3, [r7, #40]
	bl	sub_08013560
	movs	r0, #152
	bl	sub_081c0010
	ldr	r3, [pc, #372]
	movs	r0, #38
	str	r3, [r7, #40]
	bl	sub_08013560
	ldr	r3, [r7, #80]
	movs	r6, #0
	ldr	r3, [r3, #40]
	str	r6, [r7, #108]
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	movs	r3, #9
	strh	r5, [r7, #6]
	mov	r8, r1
	mov	fp, r3
	cmp	r1, #243
	bne.n	.L_080d762c
	movs	r0, #2
	mov	fp, r0
.L_080d762c:
	mov	r1, r8
	cmp	r1, #245
	bne.n	.L_080d7636
	movs	r2, #10
	mov	fp, r2
.L_080d7636:
	mov	r3, r8
	cmp	r3, #244
	bne.n	.L_080d7640
	movs	r0, #9
	mov	fp, r0
.L_080d7640:
	mov	r2, sp
	movs	r1, #0
	adds	r2, #20
	str	r1, [sp, #12]
	str	r2, [sp, #0]
	mov	sl, r7
	movs	r6, #0
.L_080d764e:
	ldr	r3, [r7, #16]
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	mov	r0, r8
	bl	sub_080200c0
	adds	r5, r0, #0
	ldr	r0, [sp, #0]
	lsls	r3, r6, #2
	str	r5, [r0, r3]
	cmp	r5, #0
	beq.n	.L_080d76c6
	movs	r3, #240
	lsls	r3, r3, #8
	adds	r2, r5, #0
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	subs	r2, #50
	movs	r3, #2
	strb	r3, [r2, #0]
	adds	r1, r5, #0
	adds	r1, #90
	ldrb	r3, [r1, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	ldr	r3, [pc, #244]
	ldr	r1, [r5, #80]
	str	r3, [r5, #108]
	ldrh	r3, [r7, #6]
	movs	r0, #13
	strh	r3, [r5, #6]
	ldrb	r3, [r1, #9]
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	strb	r3, [r1, #9]
	adds	r0, r5, #0
	mov	r1, fp
	bl	sub_08020278
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_08020090
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_08020218
	ldr	r0, [r5, #80]
	ldr	r1, [sp, #12]
	bl	sub_080dc0d8
	mov	r1, sl
	str	r0, [sp, #12]
	str	r1, [r5, #104]
	mov	sl, r5
.L_080d76c6:
	adds	r6, #1
	cmp	r6, #7
	ble.n	.L_080d764e
	movs	r3, #152
	lsls	r3, r3, #7
	adds	r3, #204
	str	r3, [r7, #48]
	str	r3, [r7, #52]
	ldr	r3, [sp, #8]
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r1, #128
	adds	r2, r3, r0
	lsls	r1, r1, #13
	adds	r0, r7, #0
	bl	sub_080db974
	movs	r0, #153
	bl	sub_081c0010
	mov	r0, r9
	movs	r1, #6
	movs	r2, #5
	bl	sub_080d32c8
	movs	r0, #24
	bl	sub_08013560
	movs	r6, #0
	b.n	.L_080d770a
.L_080d7702:
	movs	r0, #1
	bl	sub_08013560
	adds	r6, #1
.L_080d770a:
	cmp	r6, #119
	bgt.n	.L_080d772a
	ldr	r2, [sp, #16]
	ldr	r1, [r7, #12]
	ldr	r3, [r2, #12]
	subs	r2, r1, r3
	cmp	r2, #0
	blt.n	.L_080d7722
	ldr	r3, [pc, #104]
	cmp	r2, r3
	ble.n	.L_080d772a
	b.n	.L_080d7702
.L_080d7722:
	ldr	r0, [pc, #96]
	subs	r3, r3, r1
	cmp	r3, r0
	bgt.n	.L_080d7702
.L_080d772a:
	ldr	r1, [sp, #4]
	movs	r3, #0
	strb	r3, [r1, #0]
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	str	r3, [r7, #40]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r7, #56]
	str	r3, [r7, #64]
	str	r3, [r7, #60]
	adds	r0, r7, #0
	movs	r1, #0
	bl	sub_08020090
	ldr	r1, [r7, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	strb	r3, [r1, #9]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #72]
	bl	sub_080d2350
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x080d74e5
	.4byte 0x0004cccc
	.4byte 0x0004e666
	.4byte 0x080d74f1
	.2byte 0xffff
	.2byte 0x0013
.L_080d7788:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r2, #0
	mov	r8, r0
	adds	r5, r1, #0
	cmp	r6, #19
	bne.n	.L_080d77aa
	movs	r1, #200
	ldr	r2, [pc, #168]
	lsls	r1, r1, #5
	adds	r1, #80
	adds	r3, r5, r1
	ldrsb	r1, [r2, r3]
	cmp	r1, #0
	beq.n	.L_080d783e
	subs	r6, r1, #1
.L_080d77aa:
	movs	r0, #183
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080d77d0
	lsls	r0, r5, #2
	ldr	r3, [pc, #140]
	adds	r0, r0, r5
	movs	r2, #133
	lsls	r0, r0, #2
	lsls	r2, r2, #2
	adds	r0, r0, r6
	adds	r3, r3, r2
	adds	r0, #48
	ldr	r7, [r3, #0]
	bl	sub_08016cfc
	b.n	.L_080d77da
.L_080d77d0:
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	sub_080ad248
	adds	r7, r0, #0
.L_080d77da:
	cmp	r7, #0
	blt.n	.L_080d783e
	bl	sub_080d22a8
	movs	r3, #1
	negs	r3, r3
	cmp	r8, r3
	beq.n	.L_080d7818
	cmp	r5, #0
	bne.n	.L_080d77f6
	mov	r0, r8
	bl	sub_080d82e0
	b.n	.L_080d7818
.L_080d77f6:
	cmp	r5, #1
	bne.n	.L_080d7802
	mov	r0, r8
	bl	sub_080d8740
	b.n	.L_080d7818
.L_080d7802:
	cmp	r5, #2
	bne.n	.L_080d780e
	mov	r0, r8
	bl	sub_080d7f80
	b.n	.L_080d7818
.L_080d780e:
	cmp	r5, #3
	bne.n	.L_080d7818
	mov	r0, r8
	bl	sub_080d7c04
.L_080d7818:
	bl	sub_080cb82c
	adds	r1, r5, #0
	adds	r2, r6, #0
	adds	r0, r7, #0
	bl	sub_08038328
	bl	sub_080cb8a4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	mov	r2, r8
	lsls	r3, r2, #2
	adds	r3, #20
	movs	r2, #0
	str	r2, [r1, r3]
	bl	sub_080d2350
.L_080d783e:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x02001000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r5, [pc, #216]
	movs	r0, #149
	lsls	r0, r0, #2
	adds	r3, r5, r0
	movs	r1, #0
	ldrsh	r7, [r3, r1]
	ldrh	r2, [r3, #0]
	movs	r3, #240
	lsls	r3, r3, #8
	ands	r7, r3
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #255
	mov	sl, r3
	mov	r0, sl
	ands	r0, r2
	mov	sl, r0
	movs	r0, #10
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080d7926
	cmp	r7, #0
	bne.n	.L_080d7926
	movs	r3, #224
	movs	r7, #128
	lsls	r3, r3, #3
	mov	r1, sl
	adds	r3, #255
	lsls	r7, r7, #4
	ands	r7, r1
	ands	r1, r3
	ldr	r3, [pc, #152]
	mov	sl, r1
	add	r3, sl
	cmp	r3, #80
	bhi.n	.L_080d7926
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #86
	adds	r3, r5, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	ble.n	.L_080d7926
	subs	r2, #66
	movs	r1, #8
	adds	r2, r2, r5
	mov	r8, r1
	mov	r9, r2
.L_080d78bc:
	mov	r0, r8
	bl	sub_080cccb8
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080d791c
	movs	r0, #2
	ldrsh	r3, [r5, r0]
	ldr	r2, [pc, #100]
	subs	r3, #48
	add	r2, sl
	cmp	r3, r2
	bne.n	.L_080d791c
	mov	r0, r8
	bl	sub_080cad84
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_080d791c
	cmp	r7, #0
	bne.n	.L_080d78fc
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #3
	str	r7, [r6, #20]
	strb	r3, [r2, #0]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	bl	sub_080200e8
	b.n	.L_080d7914
.L_080d78fc:
	mov	r1, r9
	ldr	r0, [r1, #0]
	bl	sub_080cad84
	ldr	r3, [r0, #16]
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #12]
	ldr	r0, [pc, #44]
	adds	r3, r3, r0
	adds	r0, r6, #0
	bl	sub_080200e8
.L_080d7914:
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_08020090
.L_080d791c:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #79
	ble.n	.L_080d78bc
.L_080d7926:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0xfffffed4
	.2byte 0x0000
	.2byte 0xffe0
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	ldr	r1, [pc, #268]
	movs	r2, #149
	lsls	r2, r2, #2
	adds	r7, r1, r2
	movs	r3, #0
	ldrsh	r5, [r7, r3]
	ldrh	r2, [r7, #0]
	movs	r6, #240
	movs	r3, #240
	lsls	r6, r6, #4
	lsls	r3, r3, #8
	adds	r6, #255
	sub	sp, #4
	ands	r5, r3
	ands	r6, r2
	cmp	r0, #0
	bne.n	.L_080d79cc
	cmp	r5, #0
	bne.n	.L_080d799e
	movs	r3, #224
	lsls	r3, r3, #3
	ldr	r0, [pc, #228]
	adds	r3, #255
	ands	r6, r3
	adds	r3, r6, r0
	cmp	r3, #80
	bhi.n	.L_080d7a48
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #86
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	cmp	r2, #0
	ble.n	.L_080d7992
	movs	r1, #186
	lsls	r1, r1, #2
	adds	r1, #255
	cmp	r2, r1
	bne.n	.L_080d7a48
.L_080d7992:
	adds	r0, r6, #0
	subs	r0, #172
	bl	sub_08016cfc
	strh	r5, [r7, #0]
	b.n	.L_080d7a48
.L_080d799e:
	movs	r2, #128
	lsls	r2, r2, #5
	cmp	r5, r2
	bne.n	.L_080d7a48
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #86
	adds	r3, r1, r0
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	ble.n	.L_080d7a3e
	movs	r0, #186
	lsls	r0, r0, #2
	adds	r0, #255
	cmp	r3, r0
	beq.n	.L_080d7a3e
	adds	r0, r6, #0
	str	r1, [sp, #0]
	bl	sub_08016cfc
	ldr	r1, [sp, #0]
	b.n	.L_080d7a3e
.L_080d79cc:
	cmp	r5, #0
	bne.n	.L_080d7a3e
	movs	r2, #224
	lsls	r2, r2, #3
	ldr	r0, [pc, #124]
	adds	r2, #255
	ands	r6, r2
	adds	r3, r6, r0
	cmp	r3, #80
	bhi.n	.L_080d7a3e
	ands	r6, r2
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #86
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	ble.n	.L_080d7a3e
	ldr	r1, [pc, #96]
	adds	r5, r6, r1
	adds	r0, r5, #0
	movs	r1, #20
	bl	sub_08002054
	movs	r1, #20
	mov	r8, r0
	adds	r0, r5, #0
	bl	sub_08002064
	movs	r5, #8
	adds	r7, r0, #0
	b.n	.L_080d7a10
.L_080d7a0e:
	adds	r5, #1
.L_080d7a10:
	cmp	r5, #79
	bgt.n	.L_080d7a3c
	adds	r0, r5, #0
	bl	sub_080cccb8
	cmp	r0, #0
	beq.n	.L_080d7a0e
	movs	r2, #2
	ldrsh	r3, [r0, r2]
	ldr	r0, [pc, #48]
	subs	r3, #48
	adds	r2, r6, r0
	cmp	r3, r2
	bne.n	.L_080d7a0e
	movs	r0, #40
	bl	sub_08013560
	mov	r1, r8
	adds	r0, r5, #0
	adds	r2, r7, #0
	bl	.L_080d7788
.L_080d7a3c:
	ldr	r1, [pc, #16]
.L_080d7a3e:
	movs	r3, #149
	lsls	r3, r3, #2
	adds	r2, r1, r3
	movs	r3, #0
	strh	r3, [r2, #0]
.L_080d7a48:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0xfed4
	.2byte 0xffff
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	movs	r6, #23
	adds	r5, r3, #0
	adds	r5, #80
.L_080d7a68:
	adds	r0, r5, #0
	subs	r6, #1
	bl	sub_080ebc30
	adds	r5, #72
	cmp	r6, #0
	bge.n	.L_080d7a68
	pop	{r5, r6, pc}
	push	{lr}
	movs	r1, #224
	lsls	r1, r1, #3
	adds	r1, #244
	movs	r0, #224
	sub	sp, #4
	bl	sub_08014cc0
	movs	r3, #0
	adds	r1, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r2, [pc, #20]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #12]
	bl	sub_080145a8
	add	sp, #4
	pop	{pc}
	movs	r0, r0
	.4byte 0x850001fd
	.2byte 0x7a59
	.2byte 0x080d
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r0, [pc, #52]
	ldr	r5, [r3, #0]
	bl	sub_08014644
	adds	r6, r5, #0
	adds	r6, #149
	adds	r5, #80
	movs	r7, #23
.L_080d7acc:
	ldrb	r3, [r6, #0]
	adds	r6, #72
	lsls	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080d7adc
	adds	r0, r5, #0
	bl	sub_080ebf68
.L_080d7adc:
	subs	r7, #1
	adds	r5, #72
	cmp	r7, #0
	bge.n	.L_080d7acc
	movs	r0, #224
	bl	sub_0801314c
	movs	r0, #1
	bl	sub_08013560
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x7a59
	.2byte 0x080d
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	strh	r3, [r0, #6]
	bx	lr
