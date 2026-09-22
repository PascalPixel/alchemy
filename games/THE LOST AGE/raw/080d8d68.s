.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_080148e8, 0x080148e8
	.set sub_08014d78, 0x08014d78
	.set sub_08016cfc, 0x08016cfc
	.set sub_08016d18, 0x08016d18
	.set sub_08020090, 0x08020090
	.set sub_080cad84, 0x080cad84
	.set sub_080d8d40, 0x080d8d40
	.set sub_080d933e, 0x080d933e
	.global Overlay_080d8d68
Overlay_080d8d68:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #156
	ldr	r3, [r3, #0]
	adds	r7, r1, #0
	mov	r8, r2
	mov	fp, r0
	movs	r2, #2
	adds	r0, r7, #0
	sub	sp, #12
	mov	sl, r3
	mov	r9, r2
	bl	sub_080cad84
	adds	r5, r0, #0
	mov	r0, r8
	bl	sub_080cad84
	adds	r6, r0, #0
	cmp	r5, #0
	beq.n	.L_080d8dea
	cmp	r6, #0
	beq.n	.L_080d8dea
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_080d8db8
	ldr	r3, [r5, #80]
	ldrb	r3, [r3, #9]
	lsls	r3, r3, #28
	lsrs	r3, r3, #30
	mov	r9, r3
.L_080d8db8:
	mov	r2, fp
	lsls	r3, r2, #5
	add	r3, sl
	adds	r3, #12
	mov	r2, r8
	strh	r2, [r3, #2]
	mov	r2, r9
	strh	r7, [r3, #0]
	strh	r2, [r3, #6]
	movs	r4, #128
	ldr	r0, [r6, #8]
	ldr	r2, [r5, #12]
	ldr	r1, [r5, #8]
	ldr	r3, [r5, #16]
	str	r0, [sp, #0]
	lsls	r4, r4, #12
	ldr	r0, [r6, #12]
	adds	r2, r2, r4
	adds	r0, r0, r4
	str	r0, [sp, #4]
	ldr	r0, [r6, #16]
	str	r0, [sp, #8]
	mov	r0, fp
	bl	.L_080d8e08
.L_080d8dea:
	adds	r0, r5, #0
	movs	r1, #2
	bl	sub_08020090
	adds	r0, r6, #0
	movs	r1, #2
	bl	sub_08020090
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
.L_080d8e08:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #52
	str	r1, [sp, #44]
	str	r2, [sp, #40]
	str	r3, [sp, #36]
	str	r0, [sp, #48]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #156
	ldr	r3, [r3, #0]
	ldr	r2, [sp, #84]
	mov	r8, r3
	movs	r3, #0
	str	r3, [sp, #32]
	ldr	r3, [sp, #44]
	ldr	r1, [sp, #88]
	subs	r2, r2, r3
	str	r2, [sp, #28]
	asrs	r2, r2, #8
	mov	fp, r2
	ldr	r2, [sp, #40]
	ldr	r0, [sp, #92]
	subs	r1, r1, r2
	ldr	r2, [sp, #36]
	adds	r3, r1, #0
	subs	r0, r0, r2
	asrs	r3, r3, #8
	mov	r9, r3
	adds	r3, r0, #0
	str	r1, [sp, #24]
	str	r0, [sp, #20]
	asrs	r3, r3, #8
	ldr	r7, [pc, #328]
	mov	r1, fp
	mov	r0, fp
	mov	sl, r3
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x4649
	adds	r5, r0, #0
	mov	r0, r9
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x4651
	adds	r6, r0, #0
	mov	r0, sl
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x19ad
	adds	r5, r5, r0
	ldr	r3, [pc, #296]
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2106
	bl	sub_08002054
	ldr	r2, [sp, #48]
	adds	r0, #1
	lsls	r3, r2, #5
	add	r8, r3
	mov	r3, r8
	mov	r2, r8
	adds	r3, #12
	adds	r2, #40
	mov	r1, r9
	mov	sl, r0
	mov	r0, fp
	str	r3, [sp, #16]
	str	r2, [sp, #12]
	bl	sub_080148e8
	adds	r5, r0, #0
	lsls	r5, r5, #16
	lsrs	r5, r5, #16
	adds	r0, r5, #0
	bl	sub_08002096
	str	r0, [sp, #8]
	adds	r0, r5, #0
	bl	sub_08002090
	ldr	r2, [sp, #16]
	mov	r3, sl
	str	r0, [sp, #4]
	strh	r3, [r2, #4]
	movs	r3, #0
	mov	r8, r3
	mov	fp, r3
	mov	r9, r3
	str	r3, [sp, #0]
	b.n	.L_080d8f56
.L_080d8ecc:
	mov	r2, r8
	lsls	r0, r2, #16
	mov	r1, sl
	bl	sub_08002054
	bl	sub_08002096
	movs	r1, #128
	ldr	r3, [pc, #192]
	lsls	r1, r1, #9
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4a2e
	ldr	r1, [sp, #4]
	adds	r6, r0, #0
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x4651
	adds	r5, r0, #0
	mov	r0, r9
	bl	sub_08002054
	ldr	r3, [sp, #44]
	ldr	r2, [pc, #164]
	adds	r0, r3, r0
	adds	r0, r0, r5
	str	r0, [r7, #4]
	ldr	r1, [sp, #8]
	adds	r0, r6, #0
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x4651
	adds	r5, r0, #0
	mov	r0, fp
	bl	sub_08002054
	ldr	r3, [sp, #40]
	mov	r1, sl
	adds	r0, r3, r0
	adds	r0, r0, r5
	str	r0, [r7, #8]
	ldr	r0, [sp, #0]
	bl	sub_08002054
	ldr	r2, [sp, #36]
	adds	r0, r2, r0
	str	r0, [r7, #12]
	ldr	r3, [sp, #48]
	adds	r3, #1
	strb	r3, [r7, #19]
	cmp	r8, sl
	bne.n	.L_080d8f38
	movs	r3, #2
	b.n	.L_080d8f3a
.L_080d8f38:
	movs	r3, #1
.L_080d8f3a:
	strb	r3, [r7, #18]
	ldr	r3, [sp, #12]
	str	r7, [r3, #0]
	ldr	r2, [sp, #0]
	ldr	r3, [sp, #20]
	str	r7, [sp, #12]
	adds	r2, r2, r3
	str	r2, [sp, #0]
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #28]
	add	fp, r2
	movs	r2, #1
	add	r9, r3
	add	r8, r2
.L_080d8f56:
	cmp	r8, sl
	bgt.n	.L_080d8f6a
	bl	sub_080d8d40
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L_080d8ecc
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #32]
.L_080d8f6a:
	ldr	r2, [sp, #12]
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r2, [sp, #16]
	movs	r3, #1
	strh	r3, [r2, #10]
	str	r3, [r2, #24]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r2, #12]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r2, #20]
	movs	r3, #8
	ldrsh	r0, [r2, r3]
	cmp	r0, #0
	beq.n	.L_080d8f90
	bl	sub_08016cfc
.L_080d8f90:
	ldr	r0, [sp, #32]
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0300021c
	.2byte 0x02d4
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	r8, r0
	adds	r0, r1, #0
	adds	r5, r2, #0
	sub	sp, #16
	adds	r7, r3, #0
	bl	sub_080cad84
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_080cad84
	adds	r5, r0, #0
	cmp	r6, #0
	beq.n	.L_080d8ff0
	cmp	r5, #0
	beq.n	.L_080d8ff0
	ldr	r0, [r5, #8]
	ldr	r2, [r6, #12]
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #16]
	str	r0, [sp, #0]
	movs	r4, #128
	ldr	r0, [r5, #12]
	lsls	r4, r4, #12
	adds	r0, r0, r4
	str	r0, [sp, #4]
	adds	r2, r2, r4
	ldr	r0, [r5, #16]
	str	r7, [sp, #12]
	str	r0, [sp, #8]
	mov	r0, r8
	bl	.L_080d8ff8
.L_080d8ff0:
	add	sp, #16
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
.L_080d8ff8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #36
	str	r1, [sp, #32]
	str	r2, [sp, #28]
	str	r3, [sp, #24]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #156
	ldr	r6, [r3, #0]
	ldr	r2, [sp, #68]
	ldr	r3, [sp, #32]
	ldr	r1, [sp, #72]
	subs	r2, r2, r3
	asrs	r3, r2, #8
	str	r2, [sp, #20]
	ldr	r2, [sp, #28]
	lsls	r0, r0, #5
	subs	r1, r1, r2
	str	r1, [sp, #16]
	adds	r6, r6, r0
	adds	r6, #12
	ldr	r2, [r6, #28]
	adds	r0, r3, #0
	asrs	r1, r1, #8
	mov	r8, r2
	bl	sub_080148e8
	adds	r5, r0, #0
	lsls	r5, r5, #16
	lsrs	r5, r5, #16
	adds	r0, r5, #0
	bl	sub_08002096
	str	r0, [sp, #12]
	adds	r0, r5, #0
	bl	sub_08002090
	str	r0, [sp, #8]
	movs	r2, #0
	movs	r3, #4
	ldrsh	r7, [r6, r3]
	mov	sl, r2
	cmp	sl, r7
	bgt.n	.L_080d90ee
	ldr	r3, [sp, #76]
	ldr	r2, [sp, #24]
	subs	r3, r3, r2
	str	r3, [sp, #4]
	movs	r3, #0
	str	r3, [sp, #0]
	mov	fp, r3
	mov	r9, r3
.L_080d906c:
	mov	r2, sl
	lsls	r0, r2, #15
	adds	r1, r7, #0
	bl	sub_08002054
	bl	sub_08002096
	ldr	r3, [pc, #132]
	ldr	r1, [sp, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4a1f
	ldr	r1, [sp, #8]
	adds	r6, r0, #0
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x1c39
	adds	r5, r0, #0
	mov	r0, r9
	bl	sub_08002054
	ldr	r3, [sp, #32]
	mov	r2, r8
	adds	r0, r3, r0
	adds	r0, r0, r5
	str	r0, [r2, #4]
	ldr	r1, [sp, #12]
	ldr	r3, [pc, #92]
	adds	r0, r6, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c39
	adds	r5, r0, #0
	mov	r0, fp
	bl	sub_08002054
	ldr	r2, [sp, #28]
	mov	r3, r8
	adds	r0, r2, r0
	adds	r0, r0, r5
	str	r0, [r3, #8]
	ldr	r0, [sp, #0]
	adds	r1, r7, #0
	bl	sub_08002054
	ldr	r2, [sp, #24]
	mov	r3, r8
	adds	r0, r2, r0
	movs	r2, #1
	str	r0, [r3, #12]
	strb	r2, [r3, #18]
	ldr	r3, [r3, #0]
	ldr	r2, [sp, #0]
	mov	r8, r3
	ldr	r3, [sp, #4]
	adds	r2, r2, r3
	str	r2, [sp, #0]
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #20]
	add	fp, r2
	movs	r2, #1
	add	sl, r2
	add	r9, r3
	cmp	sl, r7
	ble.n	.L_080d906c
.L_080d90ee:
	movs	r0, #0
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #156
	ldr	r3, [r3, #0]
	adds	r7, r1, #0
	mov	r8, r2
	mov	fp, r0
	movs	r2, #2
	adds	r0, r7, #0
	sub	sp, #12
	mov	sl, r3
	mov	r9, r2
	bl	sub_080cad84
	adds	r5, r0, #0
	mov	r0, r8
	bl	sub_080cad84
	adds	r6, r0, #0
	cmp	r5, #0
	beq.n	.L_080d9186
	cmp	r6, #0
	beq.n	.L_080d9186
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_080d9154
	ldr	r3, [r5, #80]
	ldrb	r3, [r3, #9]
	lsls	r3, r3, #28
	lsrs	r3, r3, #30
	mov	r9, r3
.L_080d9154:
	mov	r2, fp
	lsls	r3, r2, #5
	add	r3, sl
	adds	r3, #12
	mov	r2, r8
	strh	r2, [r3, #2]
	mov	r2, r9
	strh	r7, [r3, #0]
	strh	r2, [r3, #6]
	movs	r4, #128
	ldr	r0, [r6, #8]
	ldr	r2, [r5, #12]
	ldr	r1, [r5, #8]
	ldr	r3, [r5, #16]
	str	r0, [sp, #0]
	lsls	r4, r4, #12
	ldr	r0, [r6, #12]
	adds	r2, r2, r4
	adds	r0, r0, r4
	str	r0, [sp, #4]
	ldr	r0, [r6, #16]
	str	r0, [sp, #8]
	mov	r0, fp
	bl	.L_080d9194
.L_080d9186:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
.L_080d9194:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	str	r1, [sp, #12]
	str	r2, [sp, #8]
	str	r3, [sp, #4]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #156
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #48]
	mov	sl, r3
	movs	r3, #0
	str	r3, [sp, #0]
	ldr	r3, [sp, #12]
	ldr	r6, [sp, #52]
	subs	r1, r1, r3
	ldr	r3, [sp, #8]
	ldr	r2, [sp, #56]
	subs	r6, r6, r3
	ldr	r3, [sp, #4]
	mov	r9, r0
	subs	r0, r2, r3
	ldr	r2, [pc, #204]
	asrs	r1, r1, #8
	mov	r8, r2
	asrs	r7, r0, #8
	adds	r0, r1, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1236
	adds	r1, r6, #0
	adds	r5, r0, #0
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c39
	adds	r6, r0, #0
	adds	r0, r7, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x19ad
	adds	r5, r5, r0
	ldr	r3, [pc, #168]
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2106
	bl	sub_08002054
	mov	r3, r9
	lsls	r3, r3, #5
	mov	r9, r3
	add	sl, r9
	adds	r0, #1
	movs	r2, #40
	add	r2, sl
	mov	fp, r0
	mov	r3, fp
	mov	r9, r2
	mov	r2, sl
	movs	r6, #128
	strh	r3, [r2, #16]
	movs	r3, #0
	movs	r7, #0
	lsls	r6, r6, #11
	mov	sl, r3
	b.n	.L_080d9270
.L_080d9226:
	adds	r0, r7, #0
	bl	sub_08002090
	adds	r1, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x9a03
	adds	r0, r2, r0
	str	r0, [r5, #4]
	ldr	r3, [sp, #8]
	adds	r0, r7, #0
	str	r3, [r5, #8]
	bl	sub_08002096
	adds	r1, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x9a01
	movs	r3, #2
	adds	r0, r2, r0
	str	r0, [r5, #12]
	mov	r2, r9
	movs	r0, #192
	strb	r3, [r5, #18]
	adds	r1, r6, #0
	str	r5, [r2, #0]
	lsls	r0, r0, #11
	bl	sub_080148e8
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	adds	r7, r7, r0
	movs	r3, #1
	lsls	r0, r0, #2
	mov	r9, r5
	adds	r6, r6, r0
	add	sl, r3
.L_080d9270:
	cmp	sl, fp
	bgt.n	.L_080d9284
	bl	sub_080d8d40
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_080d9226
	movs	r2, #1
	negs	r2, r2
	str	r2, [sp, #0]
.L_080d9284:
	movs	r3, #0
	mov	r2, r9
	str	r3, [r2, #0]
	ldr	r0, [sp, #0]
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0300021c
	.2byte 0x02d4
	.2byte 0x0300
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #156
	ldr	r3, [r3, #0]
	lsls	r0, r0, #5
	adds	r3, r3, r0
	adds	r3, #12
	ldr	r2, [r3, #28]
	movs	r0, #4
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	ble.n	.L_080d92c6
.L_080d92be:
	subs	r3, #1
	ldr	r2, [r2, #0]
	cmp	r3, #0
	bne.n	.L_080d92be
.L_080d92c6:
	ldr	r3, [r2, #4]
	str	r3, [r1, #0]
	ldr	r3, [r2, #8]
	str	r3, [r1, #4]
	ldr	r3, [r2, #12]
	str	r3, [r1, #8]
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #52
	str	r0, [sp, #40]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #156
	ldr	r3, [r3, #0]
	adds	r0, r1, #0
	adds	r6, r2, #0
	mov	r8, r3
	bl	sub_080cad84
	adds	r7, r0, #0
	adds	r0, r6, #0
	bl	sub_080cad84
	mov	sl, r0
	cmp	r7, #0
	bne.n	.L_080d9308
	b.n	.L_080d947c
.L_080d9308:
	cmp	r0, #0
	bne.n	.L_080d930e
	b.n	.L_080d947c
.L_080d930e:
	ldr	r3, [r0, #8]
	mov	r2, sl
	str	r3, [sp, #48]
	ldr	r4, [sp, #40]
	ldr	r0, [r0, #12]
	str	r0, [sp, #36]
	ldr	r3, [r2, #16]
	str	r3, [sp, #44]
	lsls	r3, r4, #5
	add	r3, r8
	adds	r3, #12
	str	r3, [sp, #32]
	mov	r3, sl
	adds	r3, #99
	ldrb	r3, [r3, #0]
	cmp	r3, #99
	bne.n	sub_080d933e
	mov	r5, sl
	add	r1, sp, #48
	add	r2, sp, #44
	ldr	r3, [r5, #104]
	mov	r0, sl
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9b0c
	ldr	r2, [r7, #8]
	ldr	r0, [sp, #36]
	subs	r3, r3, r2
	asrs	r3, r3, #8
	mov	r8, r3
	ldr	r3, [r7, #12]
	ldr	r2, [r7, #16]
	subs	r3, r0, r3
	asrs	r3, r3, #8
	mov	r9, r3
	ldr	r3, [sp, #44]
	ldr	r6, [pc, #308]
	subs	r3, r3, r2
	asrs	r3, r3, #8
	mov	r1, r8
	mov	r0, r8
	mov	fp, r3
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4649
	str	r0, [sp, #28]
	mov	r0, r9
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4659
	str	r0, [sp, #24]
	mov	r0, fp
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x9b06
	ldr	r2, [sp, #28]
	adds	r2, r2, r3
	adds	r0, r2, r0
	str	r2, [sp, #16]
	str	r0, [sp, #20]
	ldr	r3, [pc, #264]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2106
	bl	sub_08002054
	adds	r5, r0, #1
	ldr	r0, [sp, #32]
	movs	r4, #4
	ldrsh	r6, [r0, r4]
	cmp	r5, r6
	ble.n	.L_080d942c
	ldr	r3, [sp, #48]
	ldr	r2, [r7, #8]
	adds	r6, #1
	subs	r3, r3, r2
	adds	r0, r6, #0
	muls	r0, r3
	adds	r1, r5, #0
	mov	r8, r2
	bl	sub_08002054
	ldr	r3, [sp, #36]
	ldr	r4, [r7, #16]
	mov	r9, r3
	ldr	r3, [sp, #44]
	adds	r1, r5, #0
	subs	r3, r3, r4
	add	r8, r0
	adds	r0, r6, #0
	muls	r0, r3
	mov	sl, r4
	bl	sub_08002054
	ldr	r5, [pc, #200]
	add	r0, sl
	mov	fp, r0
	ldr	r0, [r5, #0]
	bl	sub_08002096
	ldr	r2, [r7, #12]
	movs	r3, #128
	lsls	r3, r3, #12
	mov	sl, r3
	lsls	r0, r0, #2
	mov	r6, r9
	ldr	r1, [r7, #8]
	ldr	r3, [r7, #16]
	mov	r4, r8
	str	r0, [sp, #12]
	add	r2, sl
	ldr	r0, [sp, #40]
	add	r6, sl
	mov	r5, fp
	str	r4, [sp, #0]
	str	r6, [sp, #4]
	str	r5, [sp, #8]
	bl	.L_080d9498
	ldr	r0, [sp, #32]
	movs	r3, #0
	str	r3, [r0, #24]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [pc, #136]
	ldr	r0, [r2, #0]
	bl	sub_08002096
	ldr	r2, [r7, #12]
	lsls	r0, r0, #2
	ldr	r1, [r7, #8]
	ldr	r3, [r7, #16]
	mov	r4, r8
	str	r0, [sp, #12]
	add	r2, sl
	ldr	r0, [sp, #40]
	str	r4, [sp, #0]
	str	r6, [sp, #4]
	str	r5, [sp, #8]
	bl	.L_080d9790
	b.n	.L_080d947c
.L_080d942c:
	ldr	r6, [pc, #100]
	movs	r5, #128
	ldr	r0, [r6, #0]
	bl	sub_08002096
	lsls	r5, r5, #12
	mov	ip, r5
	mov	r5, sl
	ldr	r4, [r5, #8]
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	str	r4, [sp, #0]
	lsls	r0, r0, #2
	ldr	r4, [r5, #12]
	add	r2, ip
	add	r4, ip
	str	r4, [sp, #4]
	ldr	r4, [r5, #16]
	str	r0, [sp, #12]
	ldr	r0, [sp, #40]
	str	r4, [sp, #8]
	bl	.L_080d9498
	ldr	r3, [r6, #0]
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r3, r3, r0
	str	r3, [r6, #0]
	movs	r0, #6
	bl	sub_08013560
	adds	r0, r7, #0
	movs	r1, #2
	bl	sub_08020090
	mov	r0, sl
	movs	r1, #2
	bl	sub_08020090
.L_080d947c:
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0300021c
	.4byte 0x030002d4
	.2byte 0x393c
	.2byte 0x080f
.L_080d9498:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #88
	str	r0, [sp, #84]
	str	r1, [sp, #80]
	str	r2, [sp, #76]
	str	r3, [sp, #72]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #156
	ldr	r3, [r3, #0]
	ldr	r2, [sp, #120]
	str	r3, [sp, #68]
	ldr	r3, [sp, #80]
	ldr	r0, [sp, #128]
	subs	r2, r2, r3
	ldr	r3, [sp, #72]
	str	r2, [sp, #64]
	ldr	r1, [sp, #124]
	asrs	r6, r2, #8
	ldr	r2, [sp, #76]
	subs	r0, r0, r3
	subs	r1, r1, r2
	str	r0, [sp, #56]
	ldr	r0, [sp, #84]
	str	r1, [sp, #60]
	asrs	r2, r1, #8
	ldr	r1, [sp, #68]
	lsls	r0, r0, #5
	str	r0, [sp, #32]
	adds	r3, r1, r0
	adds	r0, r3, #0
	adds	r0, #12
	movs	r1, #10
	ldrsh	r3, [r0, r1]
	ldr	r7, [r0, #28]
	cmp	r3, #1
	bne.n	.L_080d94f0
	b.n	.L_080d96fa
.L_080d94f0:
	movs	r3, #1
	strh	r3, [r0, #10]
	movs	r1, #4
	ldrsh	r3, [r0, r1]
	movs	r0, #128
	lsls	r0, r0, #2
	str	r3, [sp, #52]
	str	r2, [sp, #4]
	bl	sub_08014d78
	str	r0, [sp, #28]
	adds	r3, r0, #0
	ldr	r0, [sp, #52]
	ldr	r2, [sp, #4]
	cmp	r0, #0
	blt.n	.L_080d9524
	adds	r0, #1
	mov	r8, r0
.L_080d9514:
	movs	r1, #1
	negs	r1, r1
	add	r8, r1
	mov	r0, r8
	stmia	r3!, {r7}
	ldr	r7, [r7, #0]
	cmp	r0, #0
	bne.n	.L_080d9514
.L_080d9524:
	adds	r1, r2, #0
	adds	r0, r6, #0
	bl	sub_080148e8
	adds	r5, r0, #0
	lsls	r5, r5, #16
	lsrs	r5, r5, #16
	adds	r0, r5, #0
	bl	sub_08002096
	str	r0, [sp, #44]
	adds	r0, r5, #0
	bl	sub_08002090
	ldr	r2, [sp, #52]
	movs	r1, #1
	lsls	r2, r2, #1
	str	r0, [sp, #40]
	str	r1, [sp, #48]
	str	r2, [sp, #24]
	cmp	r1, r2
	ble.n	.L_080d9552
	b.n	.L_080d96cc
.L_080d9552:
	ldr	r3, [sp, #56]
	ldr	r0, [sp, #60]
	ldr	r1, [sp, #64]
	str	r3, [sp, #16]
	str	r0, [sp, #12]
	str	r1, [sp, #8]
.L_080d955e:
	ldr	r2, [sp, #52]
	mov	r8, r2
	cmp	r2, #0
	bge.n	.L_080d9568
	b.n	.L_080d96a0
.L_080d9568:
	ldr	r3, [sp, #84]
	ldr	r0, [pc, #412]
	adds	r3, #1
	str	r3, [sp, #20]
	mov	r9, r0
.L_080d9572:
	ldr	r2, [sp, #28]
	mov	r1, r8
	lsls	r3, r1, #2
	ldr	r7, [r3, r2]
	add	r3, sp, #20
	ldrb	r3, [r3, #0]
	strb	r3, [r7, #19]
	ldr	r0, [sp, #52]
	cmp	r8, r0
	bne.n	.L_080d95f6
	ldr	r1, [sp, #48]
	lsls	r0, r1, #14
	mov	r1, r8
	bl	sub_08002054
	bl	sub_08002096
	movs	r1, #128
	lsls	r1, r1, #12
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x990a
	adds	r6, r0, #0
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x9a02
	adds	r5, r0, #0
	lsrs	r0, r2, #31
	adds	r0, r2, r0
	mov	r1, r8
	asrs	r0, r0, #1
	bl	sub_08002054
	ldr	r3, [sp, #80]
	adds	r0, r3, r0
	adds	r0, r0, r5
	str	r0, [r7, #4]
	ldr	r1, [sp, #44]
	str	r0, [sp, #36]
	adds	r0, r6, #0
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x9903
	adds	r5, r0, #0
	lsrs	r0, r1, #31
	adds	r0, r1, r0
	asrs	r0, r0, #1
	mov	r1, r8
	bl	sub_08002054
	ldr	r2, [sp, #76]
	mov	r1, r8
	adds	r0, r2, r0
	adds	r0, r0, r5
	str	r0, [r7, #8]
	ldr	r3, [sp, #16]
	mov	fp, r0
	lsrs	r0, r3, #31
	adds	r0, r3, r0
	asrs	r0, r0, #1
	bl	sub_08002054
	ldr	r1, [sp, #72]
	adds	r0, r1, r0
	str	r0, [r7, #12]
	b.n	.L_080d9690
.L_080d95f6:
	ldr	r2, [sp, #36]
	ldr	r3, [r7, #4]
	mov	r0, fp
	subs	r3, r2, r3
	asrs	r6, r3, #8
	ldr	r3, [r7, #8]
	ldr	r5, [r7, #12]
	subs	r3, r0, r3
	asrs	r2, r3, #8
	mov	r1, sl
	str	r2, [sp, #4]
	subs	r5, r1, r5
	adds	r0, r6, #0
	adds	r1, r6, #0
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x9a01
	adds	r6, r0, #0
	adds	r1, r2, #0
	adds	r0, r2, #0
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x122d
	adds	r3, r0, #0
	str	r3, [sp, #0]
	adds	r1, r5, #0
	adds	r0, r5, #0
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x9b00
	adds	r6, r6, r3
	adds	r6, r6, r0
	adds	r0, r6, #0
	ldr	r3, [pc, #208]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c06
	cmp	r6, #6
	ble.n	.L_080d9692
	ldr	r2, [sp, #36]
	ldr	r3, [r7, #4]
	adds	r1, r6, #0
	subs	r3, r3, r2
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #1
	bl	sub_08002054
	ldr	r3, [sp, #36]
	adds	r1, r6, #0
	adds	r0, r3, r0
	str	r0, [r7, #4]
	str	r0, [sp, #36]
	ldr	r3, [r7, #8]
	mov	r0, fp
	subs	r3, r3, r0
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #1
	bl	sub_08002054
	ldr	r3, [r7, #12]
	mov	r1, sl
	subs	r3, r3, r1
	add	r0, fp
	str	r0, [r7, #8]
	mov	fp, r0
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #1
	adds	r1, r6, #0
	bl	sub_08002054
	movs	r3, #1
	add	r0, sl
	str	r0, [r7, #12]
	strb	r3, [r7, #18]
.L_080d9690:
	mov	sl, r0
.L_080d9692:
	movs	r2, #1
	negs	r2, r2
	add	r8, r2
	mov	r3, r8
	cmp	r3, #0
	blt.n	.L_080d96a0
	b.n	.L_080d9572
.L_080d96a0:
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #56]
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #60]
	adds	r0, r0, r1
	adds	r2, r2, r3
	str	r0, [sp, #16]
	str	r2, [sp, #12]
	ldr	r0, [sp, #8]
	ldr	r2, [sp, #48]
	ldr	r1, [sp, #64]
	ldr	r3, [sp, #24]
	adds	r0, r0, r1
	adds	r2, #1
	str	r0, [sp, #8]
	str	r2, [sp, #48]
	cmp	r2, r3
	bgt.n	.L_080d96cc
	b.n	.L_080d955e
.L_080d96cc:
	ldr	r0, [sp, #68]
	ldr	r1, [sp, #32]
	adds	r3, r0, r1
	adds	r0, r3, #0
	adds	r0, #12
	movs	r3, #1
	str	r3, [r0, #24]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r0, #12]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r0, #20]
	movs	r2, #8
	ldrsh	r0, [r0, r2]
	cmp	r0, #0
	beq.n	.L_080d96f2
	bl	sub_08016cfc
.L_080d96f2:
	ldr	r0, [sp, #28]
	bl	sub_08013164
	movs	r0, #0
.L_080d96fa:
	add	sp, #88
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0300021c
	.2byte 0x02d4
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #20
	str	r2, [sp, #16]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #156
	mov	sl, r0
	adds	r0, r1, #0
	ldr	r5, [r3, #0]
	bl	sub_080cad84
	adds	r7, r0, #0
	ldr	r0, [sp, #16]
	bl	sub_080cad84
	adds	r6, r0, #0
	cmp	r7, #0
	beq.n	.L_080d9780
	cmp	r6, #0
	beq.n	.L_080d9780
	ldr	r2, [pc, #76]
	str	r7, [r5, #0]
	ldr	r0, [r2, #0]
	str	r6, [r5, #4]
	mov	r8, r2
	bl	sub_08002096
	ldr	r4, [r6, #8]
	movs	r3, #128
	lsls	r3, r3, #12
	ldr	r2, [r7, #12]
	ldr	r1, [r7, #8]
	mov	ip, r3
	ldr	r3, [r7, #16]
	str	r4, [sp, #0]
	lsls	r0, r0, #2
	ldr	r4, [r6, #12]
	add	r2, ip
	add	r4, ip
	str	r4, [sp, #4]
	ldr	r4, [r6, #16]
	str	r0, [sp, #12]
	mov	r0, sl
	str	r4, [sp, #8]
	bl	.L_080d9790
	mov	r2, r8
	ldr	r3, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r2
	mov	r2, r8
	str	r3, [r2, #0]
.L_080d9780:
	add	sp, #20
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x3940
	.2byte 0x080f
.L_080d9790:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #36
	str	r2, [sp, #28]
	str	r3, [sp, #24]
	str	r1, [sp, #32]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #156
	ldr	r3, [r3, #0]
	lsls	r0, r0, #5
	mov	sl, r3
	add	r0, sl
	adds	r5, r0, #0
	movs	r2, #0
	adds	r5, #12
	mov	r8, r2
	movs	r2, #10
	ldrsh	r3, [r5, r2]
	ldr	r7, [r5, #28]
	cmp	r3, #0
	bne.n	.L_080d97c8
	b.n	.L_080d995e
.L_080d97c8:
	mov	r3, r8
	strh	r3, [r5, #10]
	movs	r3, #4
	ldrsh	r2, [r5, r3]
	movs	r0, #128
	lsls	r0, r0, #2
	str	r2, [sp, #20]
	bl	sub_08014d78
	str	r0, [sp, #4]
	adds	r6, r0, #0
	movs	r3, #8
	ldrsh	r0, [r5, r3]
	mov	r2, r8
	str	r2, [r5, #24]
	cmp	r0, #0
	beq.n	.L_080d97ee
	bl	sub_08016d18
.L_080d97ee:
	ldr	r2, [sp, #20]
	cmp	r2, #0
	blt.n	.L_080d9808
	adds	r2, #1
	mov	r8, r2
.L_080d97f8:
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r2, r8
	stmia	r6!, {r7}
	ldr	r7, [r7, #0]
	cmp	r2, #0
	bne.n	.L_080d97f8
.L_080d9808:
	mov	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #3
	bl	sub_08020090
	mov	r2, sl
	ldr	r0, [r2, #4]
	movs	r1, #3
	bl	sub_08020090
	movs	r0, #4
	bl	sub_08013560
	ldr	r2, [sp, #20]
	movs	r3, #0
	str	r3, [sp, #16]
	cmp	r3, r2
	ble.n	.L_080d982e
	b.n	.L_080d9956
.L_080d982e:
	ldr	r3, [sp, #16]
	movs	r2, #0
	lsls	r3, r3, #12
	mov	sl, r3
	ldr	r3, [sp, #20]
	movs	r5, #128
	mov	r8, r2
	lsls	r5, r5, #11
	cmp	r8, r3
	bgt.n	.L_080d9942
	ldr	r2, [pc, #296]
	mov	fp, r2
.L_080d9846:
	mov	r2, r8
	lsls	r3, r2, #2
	ldr	r2, [sp, #4]
	ldr	r7, [r3, r2]
	movs	r3, #0
	strb	r3, [r7, #19]
	ldr	r3, [sp, #16]
	cmp	r8, r3
	bgt.n	.L_080d98a0
	mov	r0, sl
	bl	sub_08002090
	adds	r1, r5, #0
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x9a08
	adds	r0, r2, r0
	str	r0, [r7, #4]
	ldr	r3, [sp, #28]
	str	r0, [sp, #12]
	str	r3, [r7, #8]
	mov	r0, sl
	str	r3, [sp, #8]
	bl	sub_08002096
	adds	r1, r5, #0
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x9a06
	movs	r3, #2
	adds	r0, r2, r0
	str	r0, [r7, #12]
	mov	r9, r0
	movs	r0, #192
	adds	r1, r5, #0
	strb	r3, [r7, #18]
	lsls	r0, r0, #11
	bl	sub_080148e8
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	add	sl, r0
	lsls	r0, r0, #2
	adds	r5, r5, r0
	b.n	.L_080d9938
.L_080d98a0:
	ldr	r3, [sp, #12]
	ldr	r1, [r7, #4]
	ldr	r2, [sp, #8]
	subs	r1, r3, r1
	ldr	r5, [r7, #8]
	ldr	r6, [r7, #12]
	asrs	r1, r1, #8
	mov	r3, r9
	adds	r0, r1, #0
	subs	r5, r2, r5
	subs	r6, r3, r6
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x122d
	adds	r3, r0, #0
	str	r3, [sp, #0]
	adds	r1, r5, #0
	adds	r0, r5, #0
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x1236
	adds	r5, r0, #0
	adds	r1, r6, #0
	adds	r0, r6, #0
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x9b00
	ldr	r2, [pc, #152]
	adds	r3, r3, r5
	adds	r3, r3, r0
	adds	r0, r3, #0
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x1c05
	cmp	r5, #6
	ble.n	.L_080d9942
	ldr	r2, [sp, #12]
	ldr	r3, [r7, #4]
	adds	r1, r5, #0
	subs	r3, r3, r2
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #1
	bl	sub_08002054
	ldr	r3, [sp, #12]
	adds	r1, r5, #0
	adds	r0, r3, r0
	str	r0, [r7, #4]
	str	r0, [sp, #12]
	ldr	r2, [sp, #8]
	ldr	r3, [r7, #8]
	subs	r3, r3, r2
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #1
	bl	sub_08002054
	ldr	r3, [sp, #8]
	mov	r2, r9
	adds	r0, r3, r0
	str	r0, [r7, #8]
	str	r0, [sp, #8]
	ldr	r3, [r7, #12]
	adds	r1, r5, #0
	subs	r3, r3, r2
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #1
	bl	sub_08002054
	movs	r3, #2
	add	r0, r9
	str	r0, [r7, #12]
	strb	r3, [r7, #18]
	mov	r9, r0
.L_080d9938:
	ldr	r2, [sp, #20]
	movs	r3, #1
	add	r8, r3
	cmp	r8, r2
	ble.n	.L_080d9846
.L_080d9942:
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [sp, #16]
	ldr	r2, [sp, #20]
	adds	r3, #1
	str	r3, [sp, #16]
	cmp	r3, r2
	bgt.n	.L_080d9956
	b.n	.L_080d982e
.L_080d9956:
	ldr	r0, [sp, #4]
	bl	sub_08013164
	movs	r0, #0
.L_080d995e:
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0300021c
	.4byte 0x030002d4
