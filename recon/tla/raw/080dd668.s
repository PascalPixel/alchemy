.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_080200a8, 0x080200a8
	.set sub_080200c8, 0x080200c8
	.set sub_080cad84, 0x080cad84
	.set sub_080ce31c, 0x080ce31c
	.set sub_080ce458, 0x080ce458
	.set sub_080ceafc, 0x080ceafc
	.set sub_080cf554, 0x080cf554
	.set sub_080d00f8, 0x080d00f8
	.set sub_080d170c, 0x080d170c
	.set sub_080d172c, 0x080d172c
	.set sub_080d17ac, 0x080d17ac
	.set sub_080db974, 0x080db974
	.set sub_080dc10c, 0x080dc10c
	.set sub_081c0010, 0x081c0010
	.global Func_080dd668
	.thumb_func
Func_080dd668:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	ldr	r5, [pc, #204]
	movs	r0, #154
	bl	sub_081c0010
	movs	r2, #30
	mov	r8, r2
.L_080dd67e:
	ldr	r3, [r7, #12]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	str	r3, [r7, #12]
	ldrh	r3, [r7, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	strh	r3, [r7, #6]
	ldr	r3, [r7, #24]
	movs	r0, #1
	adds	r3, r3, r5
	str	r3, [r7, #24]
	ldr	r3, [r7, #28]
	adds	r3, r3, r5
	str	r3, [r7, #28]
	bl	sub_08013560
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r2, r8
	cmp	r2, #0
	bge.n	.L_080dd67e
	movs	r2, #128
	movs	r3, #7
	lsls	r2, r2, #9
	mov	r8, r3
	mov	sl, r2
.L_080dd6ba:
	movs	r0, #209
	lsls	r0, r0, #1
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	adds	r0, #255
	bl	sub_080dc10c
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_080dd71e
	ldr	r1, [pc, #112]
	bl	sub_080200a8
	bl	sub_08014878
	mov	r3, sl
	adds	r2, r6, #0
	adds	r2, #85
	str	r3, [r6, #52]
	add	r0, sl
	movs	r3, #2
	str	r0, [r6, #48]
	strb	r3, [r2, #0]
	movs	r3, #160
	lsls	r3, r3, #4
	adds	r3, #61
	str	r3, [r6, #72]
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	subs	r5, r5, r0
	str	r5, [r6, #40]
	bl	sub_08014878
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r2, #128
	lsls	r2, r2, #12
	lsls	r5, r5, #3
	adds	r5, r5, r2
	bl	sub_08014878
	adds	r1, r5, #0
	adds	r2, r0, #0
	adds	r0, r6, #0
	bl	sub_080db974
.L_080dd71e:
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r2, r8
	cmp	r2, #0
	bge.n	.L_080dd6ba
	movs	r0, #131
	bl	sub_081c0010
	adds	r0, r7, #0
	bl	sub_080200c8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xfffff800
	.2byte 0x0e78
	.2byte 0x080f
.L_080dd748:
	push	{lr}
	movs	r3, #63
.L_080dd74c:
	subs	r3, #1
	cmp	r3, #0
	bge.n	.L_080dd74c
	pop	{pc}
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	ldr	r3, [pc, #184]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl	sub_080cad84
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #165
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r6, r0, #0
	cmp	r3, #0
	beq.n	.L_080dd794
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #166
	adds	r2, r5, r3
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	ldrh	r1, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080dd794
	subs	r3, r1, #1
	strh	r3, [r2, #0]
.L_080dd794:
	movs	r1, #192
	lsls	r1, r1, #4
	ldr	r0, [r6, #8]
	adds	r1, #168
	adds	r3, r5, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r0, #0
	bge.n	.L_080dd7ae
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r0, r0, r1
.L_080dd7ae:
	movs	r1, #209
	asrs	r0, r0, #16
	lsls	r1, r1, #8
	subs	r0, r3, r0
	adds	r1, #5
	ldr	r3, [pc, #96]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x22c0
	lsls	r2, r2, #4
	adds	r2, #170
	adds	r3, r5, r2
	adds	r1, r0, #0
	ldr	r2, [r6, #16]
	movs	r0, #0
	ldrsh	r4, [r3, r0]
	ldr	r3, [r6, #12]
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L_080dd7de
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r0, r0, r2
.L_080dd7de:
	asrs	r3, r0, #16
	subs	r3, r4, r3
	adds	r0, r3, #0
	muls	r0, r3
	adds	r2, r1, #0
	muls	r2, r1
	adds	r3, r0, #0
	movs	r1, #225
	adds	r2, r2, r3
	lsls	r1, r1, #4
	cmp	r2, r1
	bge.n	.L_080dd806
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #166
	adds	r3, r5, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	bne.n	.L_080dd814
.L_080dd806:
	movs	r1, #179
	movs	r3, #128
	lsls	r1, r1, #1
	lsls	r3, r3, #6
	adds	r2, r5, r1
	adds	r3, #144
	strh	r3, [r2, #0]
.L_080dd814:
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x02000240
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r5, #192
	lsls	r5, r5, #18
	adds	r3, r5, #0
	adds	r3, #224
	ldr	r7, [r3, #0]
	movs	r0, #6
	bl	.L_080dd748
	movs	r0, #8
	ldr	r6, [r5, #108]
	bl	sub_080d00f8
	ldr	r2, [r7, #16]
	ldr	r5, [r5, #124]
	movs	r0, #160
	ldr	r3, [r2, #8]
	lsls	r0, r0, #3
	mov	r8, r5
	adds	r0, #44
	add	r0, r8
	str	r3, [r0, #0]
	movs	r1, #166
	ldr	r3, [r2, #16]
	ldr	r2, [r2, #12]
	lsls	r1, r1, #3
	subs	r3, r3, r2
	add	r1, r8
	mov	sl, r0
	movs	r0, #128
	str	r3, [r1, #0]
	lsls	r0, r0, #9
	mov	r9, r1
	movs	r1, #0
	bl	sub_080d172c
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	adds	r0, #1
	bl	sub_080d170c
	movs	r0, #1
	bl	sub_080d17ac
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #164
	adds	r5, r6, r2
	movs	r3, #0
	strb	r3, [r5, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #8
	bl	sub_080ce31c
	adds	r2, r0, #0
	movs	r0, #160
	lsls	r0, r0, #23
	adds	r0, #5
	movs	r1, #8
	bl	sub_080ce458
	cmp	r0, #0
	beq.n	.L_080dd8b8
	movs	r3, #24
	ldrsh	r1, [r7, r3]
	movs	r3, #26
	ldrsh	r2, [r7, r3]
	bl	sub_080ceafc
.L_080dd8b8:
	movs	r0, #131
	bl	sub_081c0010
	movs	r1, #1
	strb	r1, [r5, #0]
	mov	r0, sl
	ldr	r2, [r0, #0]
	cmp	r2, #0
	bge.n	.L_080dd8d2
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r2, r2, r3
.L_080dd8d2:
	movs	r0, #192
	lsls	r0, r0, #4
	adds	r0, #168
	asrs	r2, r2, #16
	adds	r3, r6, r0
	strh	r2, [r3, #0]
	mov	r3, r9
	ldr	r2, [r3, #0]
	cmp	r2, #0
	bge.n	.L_080dd8ee
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #255
	adds	r2, r2, r0
.L_080dd8ee:
	movs	r0, #192
	lsls	r0, r0, #4
	adds	r0, #170
	adds	r3, r6, r0
	asrs	r2, r2, #16
	strh	r2, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #166
	adds	r2, r6, r3
	movs	r3, #150
	lsls	r3, r3, #2
	subs	r0, #5
	strh	r3, [r2, #0]
	adds	r3, r6, r0
	strb	r1, [r3, #0]
	bl	sub_080cf554
	movs	r6, #160
	lsls	r6, r6, #3
	adds	r6, #42
	movs	r5, #0
	add	r6, r8
.L_080dd91c:
	movs	r0, #1
	bl	sub_08013560
	strh	r5, [r6, #0]
	adds	r5, #1
	cmp	r5, #18
	ble.n	.L_080dd91c
	adds	r3, r7, #0
	adds	r3, #33
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080dd942
	movs	r1, #144
	ldr	r0, [pc, #16]
	lsls	r1, r1, #3
	bl	sub_080145a8
.L_080dd942:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0xd755
	.2byte 0x080d
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r2, r3, #0
	adds	r2, #224
	ldr	r6, [r3, #108]
	ldr	r7, [r2, #0]
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #164
	adds	r5, r6, r2
	ldr	r1, [r3, #124]
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	mov	r8, r1
	cmp	r3, #0
	beq.n	.L_080dda28
	movs	r0, #167
	bl	sub_081c0010
	ldr	r0, [pc, #164]
	bl	sub_08014644
	movs	r1, #192
	lsls	r1, r1, #4
	movs	r2, #0
	adds	r1, #166
	strb	r2, [r5, #0]
	adds	r3, r6, r1
	movs	r5, #128
	strh	r2, [r3, #0]
	movs	r0, #0
	lsls	r5, r5, #9
	bl	.L_080dd748
	movs	r1, #1
	adds	r0, r5, #0
	bl	sub_080d170c
	movs	r0, #1
	bl	sub_080d17ac
	movs	r0, #0
	movs	r1, #0
	bl	sub_080d172c
	movs	r1, #0
	adds	r0, r5, #0
	bl	sub_080d170c
	movs	r0, #30
	bl	sub_080d17ac
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #8
	bl	sub_080ce31c
	adds	r2, r0, #0
	movs	r0, #128
	lsls	r0, r0, #23
	adds	r0, #5
	movs	r1, #8
	bl	sub_080ce458
	cmp	r0, #0
	beq.n	.L_080dd9e8
	movs	r2, #24
	ldrsh	r1, [r7, r2]
	movs	r3, #26
	ldrsh	r2, [r7, r3]
	bl	sub_080ceafc
.L_080dd9e8:
	adds	r3, r7, #0
	adds	r3, #52
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080dda28
	movs	r2, #160
	ldr	r3, [pc, #36]
	lsls	r2, r2, #3
	adds	r2, #62
	add	r2, r8
	strb	r3, [r2, #0]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #60
	add	r3, r8
	movs	r2, #1
	strb	r2, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #61
	add	r3, r8
	strb	r2, [r3, #0]
	movs	r0, #10
	bl	sub_08013560
	b.n	.L_080dda28
	.4byte 0x00000000
	.2byte 0xd755
	.2byte 0x080d
.L_080dda28:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
