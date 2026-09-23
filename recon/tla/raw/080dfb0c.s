.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_08013560, 0x08013560
	.set sub_08014878, 0x08014878
	.set sub_0801489c, 0x0801489c
	.set sub_08020090, 0x08020090
	.set sub_080200a8, 0x080200a8
	.set sub_080200c8, 0x080200c8
	.set sub_080200e8, 0x080200e8
	.set sub_08020340, 0x08020340
	.set sub_080db0b0, 0x080db0b0
	.set sub_080dc10c, 0x080dc10c
	.set sub_080dc294, 0x080dc294
	.set sub_080dc384, 0x080dc384
	.set sub_080df8e0, 0x080df8e0
	.set sub_080e1420, 0x080e1420
	.set sub_081c0010, 0x081c0010
	.global Func_080dfb0c
	.thumb_func
Func_080dfb0c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #68
	mov	sl, r0
	add	r0, sp, #28
	movs	r3, #0
	str	r3, [r0, #4]
	ldr	r3, [pc, #96]
	mov	r8, r0
	str	r3, [r0, #36]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	str	r3, [r0, #8]
	str	r3, [r0, #12]
	movs	r7, #0
	add	r6, sp, #16
.L_080dfb32:
	lsls	r5, r7, #12
	adds	r0, r5, #0
	bl	sub_08002090
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #0]
	adds	r0, r5, #0
	movs	r3, #0
	str	r3, [r6, #4]
	bl	sub_08002096
	str	r0, [r6, #8]
	mov	r2, sl
	ldr	r5, [r2, #8]
	ldr	r1, [r2, #12]
	ldr	r3, [r6, #0]
	ldr	r2, [r2, #16]
	ldr	r4, [r6, #4]
	str	r0, [sp, #4]
	ldr	r0, [pc, #32]
	adds	r7, #1
	str	r0, [sp, #8]
	mov	r0, r8
	str	r0, [sp, #12]
	adds	r0, r5, #0
	str	r4, [sp, #0]
	bl	sub_080df8e0
	cmp	r7, #16
	bls.n	.L_080dfb32
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x080dfab5
	.2byte 0x0001
	.2byte 0x0109
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	bl	sub_08014878
	ldrh	r6, [r7, #6]
	movs	r1, #128
	lsls	r1, r1, #10
	adds	r5, r0, #0
	adds	r0, r6, #0
	adds	r5, r5, r1
	bl	sub_08002090
	ldr	r2, [pc, #140]
	adds	r1, r0, #0
	mov	r8, r2
	adds	r0, r5, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x4682
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r1, r0, #0
	adds	r0, r5, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x68bb
	movs	r1, #255
	add	r3, sl
	str	r3, [r7, #8]
	ldr	r3, [r7, #16]
	lsls	r1, r1, #8
	adds	r3, r3, r0
	str	r3, [r7, #16]
	ldrh	r3, [r7, #6]
	adds	r1, #240
	adds	r3, r3, r1
	strh	r3, [r7, #6]
	adds	r5, r7, #0
	adds	r5, #102
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	ldrh	r2, [r5, #0]
	cmp	r3, #0
	beq.n	.L_080dfbf8
	subs	r3, r2, #1
	strh	r3, [r5, #0]
	ldrh	r3, [r7, #6]
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r3, r3, r2
	strh	r3, [r7, #6]
	b.n	.L_080dfc10
.L_080dfbf8:
	bl	sub_08014878
	lsls	r0, r0, #5
	lsrs	r0, r0, #16
	cmp	r0, #0
	bne.n	.L_080dfc10
	bl	sub_08014878
	lsls	r0, r0, #4
	lsrs	r0, r0, #16
	adds	r0, #8
	strh	r0, [r5, #0]
.L_080dfc10:
	adds	r2, r7, #0
	adds	r2, #100
	ldrh	r3, [r2, #0]
	movs	r1, #202
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r1, r1, #15
	lsls	r3, r3, #16
	cmp	r3, r1
	bne.n	.L_080dfc2c
	ldr	r1, [pc, #16]
	adds	r0, r7, #0
	bl	sub_080200a8
.L_080dfc2c:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x0300021c
	.2byte 0x0e54
	.2byte 0x080f
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r7, r0, #0
	ldr	r0, [r7, #104]
	ldrh	r5, [r7, #6]
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r9, r0
	adds	r0, r5, #0
	mov	r8, r2
	bl	sub_08002090
	ldr	r6, [pc, #112]
	adds	r1, r0, #0
	mov	r0, r8
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4682
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r1, r0, #0
	mov	r0, r8
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x464a
	ldr	r3, [r2, #8]
	movs	r1, #0
	add	r3, sl
	str	r3, [r7, #8]
	ldr	r3, [r2, #16]
	adds	r2, r7, #0
	adds	r3, r3, r0
	str	r3, [r7, #16]
	ldrh	r3, [r7, #6]
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r3, r3, r0
	strh	r3, [r7, #6]
	adds	r2, #100
	ldrh	r3, [r2, #0]
	movs	r0, #242
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r0, r0, #15
	lsls	r3, r3, #16
	cmp	r3, r0
	bne.n	.L_080dfcc2
	ldr	r3, [pc, #44]
	str	r3, [r7, #108]
	adds	r3, r7, #0
	adds	r3, #102
	strh	r1, [r2, #0]
	strh	r1, [r3, #0]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r7, #72]
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r7, #40]
	bl	sub_08014878
	strh	r0, [r7, #6]
.L_080dfcc2:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x0300021c
	.2byte 0xfb89
	.2byte 0x080d
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r3, [r5, #24]
	movs	r2, #128
	subs	r3, #128
	lsls	r2, r2, #8
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	cmp	r3, r2
	bge.n	.L_080dfcf6
	movs	r3, #0
	movs	r1, #0
	movs	r2, #0
	bl	sub_080200e8
	movs	r3, #0
	str	r3, [r5, #108]
.L_080dfcf6:
	pop	{r5, pc}
	push	{lr}
	bl	.L_080dfd00
	pop	{pc}
.L_080dfd00:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	sub	sp, #40
	ldr	r1, [r3, #20]
	mov	sl, r3
	ldr	r2, [r3, #16]
	str	r1, [sp, #0]
	movs	r3, #0
	mov	fp, r3
	mov	r3, sl
	adds	r3, #52
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	ldr	r1, [r1, #80]
	cmp	r3, #0
	beq.n	.L_080dfd36
	b.n	.L_080dffd6
.L_080dfd36:
	mov	r3, sl
	adds	r3, #32
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080dfd66
	movs	r3, #1
	mov	fp, r3
	ldr	r3, [r1, #40]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	movs	r1, #186
	lsls	r1, r1, #1
	cmp	r3, r1
	bne.n	.L_080dfd5a
	movs	r1, #2
	mov	fp, r1
.L_080dfd5a:
	movs	r1, #118
	adds	r1, #255
	cmp	r3, r1
	bne.n	.L_080dfd66
	movs	r3, #3
	mov	fp, r3
.L_080dfd66:
	ldr	r3, [r2, #8]
	add	r1, sp, #16
	str	r3, [r1, #0]
	add	r5, sp, #4
	ldr	r3, [r2, #12]
	mov	r8, r1
	str	r3, [r1, #4]
	movs	r0, #140
	ldr	r3, [r2, #16]
	mov	r2, sl
	str	r3, [r1, #8]
	ldr	r1, [pc, #612]
	ldr	r3, [r2, #4]
	lsls	r0, r0, #1
	str	r3, [r5, #0]
	ldr	r3, [r2, #8]
	adds	r3, r3, r1
	str	r3, [r5, #4]
	movs	r1, #0
	ldr	r3, [r2, #12]
	movs	r2, #0
	str	r3, [r5, #8]
	movs	r3, #0
	bl	sub_080dc10c
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L_080dfda0
	b.n	.L_080dffd6
.L_080dfda0:
	bl	sub_080dc294
	adds	r0, r7, #0
	movs	r1, #2
	bl	sub_08020090
	mov	r9, r8
	movs	r6, #0
	mov	r8, r5
.L_080dfdb2:
	mov	r2, r8
	mov	r1, r9
	ldr	r3, [r2, #0]
	ldr	r5, [r1, #0]
	movs	r1, #10
	subs	r3, r3, r5
	adds	r0, r6, #0
	muls	r0, r3
	bl	sub_08002054
	adds	r5, r5, r0
	str	r5, [r7, #8]
	mov	r2, r8
	mov	r1, r9
	ldr	r3, [r2, #4]
	ldr	r5, [r1, #4]
	movs	r1, #10
	subs	r3, r3, r5
	adds	r0, r6, #0
	muls	r0, r3
	bl	sub_08002054
	adds	r5, r5, r0
	str	r5, [r7, #12]
	mov	r2, r8
	mov	r1, r9
	ldr	r3, [r2, #8]
	ldr	r5, [r1, #8]
	movs	r1, #10
	subs	r3, r3, r5
	adds	r0, r6, #0
	muls	r0, r3
	bl	sub_08002054
	movs	r3, #134
	lsls	r3, r3, #9
	adds	r3, #204
	adds	r5, r5, r0
	movs	r1, #10
	adds	r0, r6, #0
	muls	r0, r3
	str	r5, [r7, #16]
	bl	sub_08002054
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r0, r0, r2
	str	r0, [r7, #24]
	str	r0, [r7, #28]
	adds	r6, #1
	movs	r0, #1
	bl	sub_08013560
	cmp	r6, #11
	blt.n	.L_080dfdb2
	ldr	r3, [pc, #452]
	movs	r0, #207
	str	r3, [r7, #24]
	movs	r3, #166
	lsls	r3, r3, #9
	adds	r3, #204
	str	r3, [r7, #28]
	bl	sub_081c0010
	mov	r3, fp
	cmp	r3, #0
	beq.n	.L_080dfed6
	mov	r1, fp
	cmp	r1, #1
	bne.n	.L_080dfef8
	movs	r0, #20
	bl	sub_08013560
	ldr	r2, [sp, #0]
	cmp	r2, #0
	beq.n	.L_080dfe4e
	ldr	r3, [pc, #416]
	str	r3, [r2, #108]
.L_080dfe4e:
	movs	r3, #0
	mov	r8, r3
	add	r6, sp, #28
.L_080dfe54:
	ldr	r3, [r7, #8]
	movs	r1, #128
	str	r3, [r6, #0]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	mov	r2, r8
	muls	r2, r3
	ldr	r3, [r7, #12]
	lsls	r1, r1, #11
	adds	r3, r3, r2
	adds	r3, r3, r1
	str	r3, [r6, #4]
	ldr	r3, [r7, #16]
	str	r3, [r6, #8]
	bl	sub_08014878
	movs	r2, #192
	lsls	r5, r0, #2
	lsls	r2, r2, #10
	adds	r5, r5, r0
	adds	r5, r5, r2
	bl	sub_08014878
	adds	r2, r6, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_0801489c
	movs	r0, #154
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	lsls	r0, r0, #1
	bl	sub_080dc10c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080dfeba
	ldr	r3, [pc, #332]
	movs	r2, #0
	str	r3, [r5, #108]
	adds	r3, r5, #0
	adds	r3, #100
	str	r7, [r5, #104]
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	bl	sub_08014878
	strh	r0, [r5, #6]
.L_080dfeba:
	movs	r0, #6
	bl	sub_08013560
	movs	r3, #1
	add	r8, r3
	mov	r1, r8
	cmp	r1, #15
	ble.n	.L_080dfe54
	movs	r0, #20
	bl	sub_08013560
	movs	r0, #120
	bl	sub_08013560
.L_080dfed6:
	movs	r1, #1
	adds	r0, r7, #0
	bl	sub_08020090
	movs	r0, #30
	bl	sub_08013560
	movs	r0, #136
	bl	sub_081c0010
	adds	r0, r7, #0
	bl	sub_08020340
	adds	r0, r7, #0
	bl	sub_080200c8
	b.n	.L_080dffd2
.L_080dfef8:
	mov	r3, fp
	subs	r3, #2
	cmp	r3, #1
	bhi.n	.L_080dffd2
	movs	r0, #5
	bl	sub_08013560
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_08020090
	mov	r2, fp
	cmp	r2, #2
	bne.n	.L_080dffa2
	movs	r1, #192
	movs	r3, #0
	lsls	r1, r1, #10
	movs	r2, #20
	mov	r8, r3
	add	r6, sp, #28
	mov	fp, r1
	mov	r9, r2
.L_080dff24:
	ldr	r3, [r7, #8]
	movs	r1, #128
	str	r3, [r6, #0]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	mov	r2, r8
	muls	r2, r3
	ldr	r3, [r7, #12]
	lsls	r1, r1, #11
	adds	r3, r3, r2
	adds	r3, r3, r1
	str	r3, [r6, #4]
	ldr	r3, [r7, #16]
	str	r3, [r6, #8]
	bl	sub_08014878
	lsls	r5, r0, #2
	adds	r5, r5, r0
	bl	sub_08014878
	add	r5, fp
	adds	r1, r0, #0
	adds	r2, r6, #0
	adds	r0, r5, #0
	bl	sub_0801489c
	movs	r0, #154
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	lsls	r0, r0, #1
	bl	sub_080dc10c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080dff92
	ldr	r3, [pc, #132]
	mov	r1, r9
	str	r3, [r5, #108]
	adds	r3, r5, #0
	adds	r3, #100
	strh	r1, [r3, #0]
	movs	r2, #0
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	mov	r2, fp
	str	r3, [r5, #72]
	str	r2, [r5, #40]
	bl	sub_08014878
	strh	r0, [r5, #6]
.L_080dff92:
	movs	r1, #1
	movs	r3, #2
	add	r8, r1
	negs	r3, r3
	mov	r2, r8
	add	r9, r3
	cmp	r2, #15
	ble.n	.L_080dff24
.L_080dffa2:
	mov	r3, sl
	movs	r1, #0
	ldr	r0, [r3, #16]
	bl	sub_080e1420
	bl	sub_080dc384
	adds	r2, r7, #0
	adds	r2, #100
	movs	r3, #0
	strh	r3, [r2, #0]
	ldr	r3, [pc, #60]
	mov	r2, sl
	str	r3, [r7, #108]
	movs	r1, #26
	ldrsh	r0, [r2, r1]
	bl	sub_080db0b0
	adds	r0, r7, #0
	bl	sub_08020340
	adds	r0, r7, #0
	bl	sub_080200c8
.L_080dffd2:
	bl	sub_080dc384
.L_080dffd6:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xfffc0000
	.4byte 0x0001b333
	.4byte 0x080dfcd5
	.4byte 0x080dfc3d
	.4byte 0x080dfb89
	.2byte 0xfffd
	.2byte 0x080d
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r5, #100
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	ldrh	r2, [r5, #0]
	cmp	r3, #80
	bne.n	.L_080e0018
	movs	r3, #0
	str	r3, [r0, #108]
	movs	r0, #136
	bl	sub_081c0010
	ldrh	r2, [r5, #0]
.L_080e0018:
	adds	r3, r2, #1
	strh	r3, [r5, #0]
	pop	{r5, pc}
	.align 2, 0
