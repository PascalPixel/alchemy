.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08014878, 0x08014878
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016cfc, 0x08016cfc
	.set sub_08020090, 0x08020090
	.set sub_08020098, 0x08020098
	.set sub_080200a8, 0x080200a8
	.set sub_080200c8, 0x080200c8
	.set sub_080200e8, 0x080200e8
	.set sub_08038040, 0x08038040
	.set sub_08038058, 0x08038058
	.set sub_08038120, 0x08038120
	.set sub_08038128, 0x08038128
	.set sub_08038138, 0x08038138
	.set sub_08038340, 0x08038340
	.set sub_080ad028, 0x080ad028
	.set sub_080ad0f0, 0x080ad0f0
	.set sub_080ad1d8, 0x080ad1d8
	.set sub_080ad2e8, 0x080ad2e8
	.set sub_080ad2f8, 0x080ad2f8
	.set sub_080ca18c, 0x080ca18c
	.set sub_080ca5d8, 0x080ca5d8
	.set sub_080cb6c8, 0x080cb6c8
	.set sub_080cccb8, 0x080cccb8
	.set sub_080ccd48, 0x080ccd48
	.set sub_080ccd78, 0x080ccd78
	.set sub_080cd4d4, 0x080cd4d4
	.set sub_080cd5b4, 0x080cd5b4
	.set sub_080cdea8, 0x080cdea8
	.set sub_080cded4, 0x080cded4
	.set sub_080ceba8, 0x080ceba8
	.set sub_080ceeac, 0x080ceeac
	.set sub_080cef68, 0x080cef68
	.set sub_080cef84, 0x080cef84
	.set sub_080cefb4, 0x080cefb4
	.set sub_080cefd0, 0x080cefd0
	.set sub_080cefe0, 0x080cefe0
	.set sub_080ceffc, 0x080ceffc
	.set sub_080cf004, 0x080cf004
	.set sub_080cf17c, 0x080cf17c
	.set sub_080cf340, 0x080cf340
	.set sub_080cf350, 0x080cf350
	.set sub_080d1ed8, 0x080d1ed8
	.set sub_080d2240, 0x080d2240
	.set sub_080d2260, 0x080d2260
	.set sub_080d22a8, 0x080d22a8
	.set sub_080d2350, 0x080d2350
	.set sub_080d2d84, 0x080d2d84
	.set sub_080d35d4, 0x080d35d4
	.set sub_080d4b0c, 0x080d4b0c
	.set sub_080e035c, 0x080e035c
	.set sub_080ea14c, 0x080ea14c
	.set sub_081c0010, 0x081c0010
	.global Func_080cce94
	.thumb_func
Func_080cce94:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r1, r5, #0
	movs	r0, #0
	bl	sub_080ccd78
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #106
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	adds	r6, r0, #0
	cmp	r1, r5
	bne.n	.L_080ccebe
	movs	r0, #7
	bl	sub_080ccd78
	cmp	r0, #0
	bne.n	.L_080ccec0
.L_080ccebe:
	adds	r0, r6, #0
.L_080ccec0:
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #36
	mov	fp, r0
	bl	sub_080cccb8
	movs	r1, #1
	negs	r1, r1
	movs	r3, #128
	str	r1, [sp, #28]
	lsls	r3, r3, #5
	mov	r1, fp
	movs	r2, #0
	ands	r1, r3
	str	r0, [sp, #32]
	str	r2, [sp, #20]
	str	r2, [sp, #16]
	str	r2, [sp, #12]
	str	r1, [sp, #8]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #108]
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #255
	mov	r2, fp
	ands	r2, r3
	mov	fp, r2
	mov	r0, fp
	bl	sub_080d2d84
	mov	r3, fp
	movs	r5, #0
	str	r0, [sp, #24]
	cmp	r3, #63
	bgt.n	.L_080ccf1c
	b.n	.L_080cd3b2
.L_080ccf1c:
	cmp	r3, #79
	beq.n	.L_080ccf5a
	bl	sub_080ceba8
	cmp	r0, #0
	beq.n	.L_080ccf76
	ldr	r2, [pc, #548]
	mov	r1, fp
	lsls	r3, r1, #3
	adds	r3, r0, r3
	adds	r0, r3, r2
	movs	r2, #0
	ldrsh	r1, [r0, r2]
	mov	sl, fp
	mov	r8, r1
	ldr	r1, [pc, #536]
	movs	r5, #1
	adds	r0, r3, r1
	movs	r1, #0
	ldrsh	r2, [r0, r1]
	str	r2, [sp, #4]
	ldr	r2, [pc, #528]
	adds	r0, r3, r2
	movs	r2, #0
	ldrsh	r1, [r0, r2]
	mov	r9, r1
	ldr	r1, [pc, #520]
	adds	r0, r3, r1
	movs	r2, #0
	ldrsh	r7, [r0, r2]
	b.n	.L_080ccf76
.L_080ccf5a:
	ldr	r1, [sp, #28]
	movs	r2, #208
	str	r1, [sp, #4]
	lsls	r2, r2, #4
	movs	r3, #131
	adds	r2, #52
	mov	r8, r3
	adds	r3, r6, r2
	mov	r9, r1
	movs	r1, #0
	ldrsh	r7, [r3, r1]
	movs	r2, #79
	mov	sl, r2
	movs	r5, #1
.L_080ccf76:
	cmp	r5, #0
	bne.n	.L_080ccf7c
	b.n	.L_080cd3b2
.L_080ccf7c:
	mov	r5, r8
	adds	r3, r5, #0
	subs	r3, #128
	movs	r1, #128
	lsls	r3, r3, #16
	lsls	r1, r1, #9
	cmp	r3, r1
	bhi.n	.L_080ccf8e
	movs	r5, #1
.L_080ccf8e:
	mov	r3, r8
	subs	r3, #130
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r2, r2, #10
	cmp	r3, r2
	bhi.n	.L_080ccf9e
	movs	r5, #5
.L_080ccf9e:
	mov	r3, r8
	cmp	r3, #132
	bne.n	.L_080ccfa6
	movs	r5, #26
.L_080ccfa6:
	mov	r1, r8
	cmp	r1, #133
	bne.n	.L_080ccfae
	movs	r5, #32
.L_080ccfae:
	bl	sub_080d22a8
	ldr	r2, [sp, #24]
	movs	r3, #1
	adds	r2, #91
	str	r2, [sp, #0]
	strb	r3, [r2, #0]
	ldr	r3, [sp, #8]
	cmp	r3, #0
	bne.n	.L_080ccfcc
	ldr	r0, [pc, #412]
	movs	r1, #1
	adds	r0, r5, r0
	bl	sub_08038040
.L_080ccfcc:
	ldr	r3, [pc, #404]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #396]
	cmp	r2, r3
	bne.n	.L_080ccffe
	ldr	r2, [sp, #4]
	cmp	r2, #0
	bne.n	.L_080ccffe
	mov	r0, r9
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080ccffe
	adds	r0, r7, #0
	movs	r1, #2
	bl	sub_08038120
	ldr	r0, [pc, #372]
	movs	r1, #1
	bl	sub_08038040
.L_080ccffe:
	mov	r0, r9
	bl	sub_080ccd48
	cmp	r0, #0
	bne.n	.L_080cd00a
	b.n	.L_080cd394
.L_080cd00a:
	mov	r3, r8
	cmp	r3, #128
	bne.n	.L_080cd022
	cmp	r7, #0
	bne.n	.L_080cd01c
	mov	r0, sl
	bl	sub_080cefe0
	b.n	.L_080cd022
.L_080cd01c:
	mov	r0, sl
	bl	sub_080cefb4
.L_080cd022:
	mov	r1, r8
	cmp	r1, #129
	bne.n	.L_080cd02e
	mov	r0, sl
	bl	sub_080cefb4
.L_080cd02e:
	mov	r2, r8
	cmp	r2, #130
	bne.n	.L_080cd0ca
	mov	r0, sl
	bl	sub_080ceffc
	ldr	r3, [pc, #308]
	adds	r5, r0, #0
	str	r3, [r5, #108]
	bl	sub_080cf350
	movs	r0, #83
	bl	sub_081c0010
	adds	r0, r7, #0
	movs	r1, #5
	bl	sub_08038120
	ldr	r6, [pc, #288]
	movs	r1, #3
	adds	r0, r6, #0
	bl	sub_08038040
	movs	r0, #218
	lsls	r0, r0, #3
	movs	r1, #0
	adds	r0, #255
	bl	sub_080cb6c8
	movs	r0, #1
	bl	sub_08038128
	movs	r0, #126
	bl	sub_081c0010
	bl	sub_080ad0f0
	cmp	r0, #1
	bne.n	.L_080cd086
	adds	r0, r6, #6
	movs	r1, #1
	bl	sub_08038040
	b.n	.L_080cd08e
.L_080cd086:
	adds	r0, r6, #1
	movs	r1, #1
	bl	sub_08038040
.L_080cd08e:
	bl	sub_08038138
	movs	r3, #0
	str	r3, [r5, #108]
	movs	r1, #2
	adds	r0, r5, #0
	bl	sub_08020090
	movs	r0, #246
	bl	sub_081c0010
	movs	r0, #30
	bl	sub_080d2240
	ldr	r0, [pc, #204]
	movs	r1, #1
	bl	sub_08038040
	mov	r0, sl
	bl	sub_080cf004
	movs	r3, #1
	negs	r3, r3
	cmp	r9, r3
	bne.n	.L_080cd0c2
	b.n	.L_080cd39e
.L_080cd0c2:
	mov	r0, r9
	bl	sub_08016cfc
	b.n	.L_080cd39e
.L_080cd0ca:
	mov	r1, r8
	cmp	r1, #132
	bne.n	.L_080cd106
	bl	sub_080d22a8
	movs	r0, #0
	bl	sub_080cded4
	mov	r0, sl
	bl	sub_080ea14c
	adds	r0, r7, #0
	bl	sub_08038340
	adds	r0, r7, #0
	bl	sub_080ad2f8
	mov	r0, sl
	bl	sub_080cf004
	movs	r2, #1
	negs	r2, r2
	cmp	r9, r2
	beq.n	.L_080cd100
	mov	r0, r9
	bl	sub_08016cfc
.L_080cd100:
	bl	sub_080d2350
	b.n	.L_080cd39e
.L_080cd106:
	mov	r3, r8
	cmp	r3, #129
	bne.n	.L_080cd1bc
	movs	r3, #192
	lsls	r3, r3, #18
	mov	r0, sl
	ldr	r5, [r3, #108]
	bl	sub_080cef84
	movs	r1, #1
	negs	r1, r1
	cmp	r9, r1
	beq.n	.L_080cd134
	ldr	r3, [pc, #64]
	ldr	r2, [pc, #40]
	movs	r1, #149
	lsls	r1, r1, #2
	adds	r3, r3, r1
	mov	r1, r9
	orrs	r1, r2
	mov	r9, r1
	mov	r2, r9
	strh	r2, [r3, #0]
.L_080cd134:
	cmp	r7, #7
	ble.n	.L_080cd17c
	adds	r1, r7, #0
	subs	r1, #8
	movs	r0, #105
	bl	sub_080ca18c
	movs	r1, #178
	lsls	r1, r1, #1
	adds	r3, r5, r1
	b.n	.L_080cd18a
	movs	r0, r0
	.4byte 0x00001000
	.4byte 0xfffffe00
	.4byte 0xfffffe02
	.4byte 0xfffffe04
	.4byte 0xfffffe06
	.4byte 0x00000dc4
	.4byte 0x02000240
	.4byte 0x00000005
	.4byte 0x000015b5
	.4byte 0x080cf0d1
	.4byte 0x00000e17
	.2byte 0x0e19
	.2byte 0x0000
.L_080cd17c:
	movs	r0, #104
	adds	r1, r7, #0
	bl	sub_080ca18c
	movs	r2, #178
	lsls	r2, r2, #1
	adds	r3, r5, r2
.L_080cd18a:
	strh	r0, [r3, #0]
	ldr	r5, [pc, #780]
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r5, r3
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r0, #104
	adds	r1, r7, #0
	bl	sub_080ca5d8
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #14
	adds	r5, r5, r1
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	bl	sub_081c0010
	ldr	r0, [pc, #748]
	movs	r1, #1
	bl	sub_08038040
	b.n	.L_080cd39e
.L_080cd1bc:
	mov	r3, r8
	cmp	r3, #133
	bne.n	.L_080cd1ca
	ldr	r0, [pc, #736]
	movs	r1, #1
	bl	sub_08038040
.L_080cd1ca:
	mov	r0, sl
	bl	sub_080ceeac
	cmp	r0, #0
	beq.n	.L_080cd1fe
	movs	r1, #1
	ldr	r0, [pc, #720]
	bl	sub_08038040
	movs	r0, #136
	bl	sub_080ad2e8
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	bne.n	.L_080cd1ec
	b.n	.L_080cd39e
.L_080cd1ec:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #173
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_080cd39e
.L_080cd1fe:
	movs	r3, #128
	lsls	r3, r3, #8
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_080cd290
	mov	r2, r8
	cmp	r2, #131
	bne.n	.L_080cd21c
	mov	r0, sl
	bl	sub_080ceffc
	ldr	r3, [pc, #660]
	adds	r6, r0, #0
	str	r3, [r6, #108]
	b.n	.L_080cd238
.L_080cd21c:
	movs	r1, #0
	mov	r0, sl
	bl	sub_080cf17c
	adds	r6, r0, #0
	movs	r0, #30
	bl	sub_08013560
	mov	r3, r8
	cmp	r3, #128
	bne.n	.L_080cd238
	mov	r0, sl
	bl	sub_080cefd0
.L_080cd238:
	adds	r0, r6, #0
	bl	sub_080cf350
	movs	r0, #83
	bl	sub_081c0010
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #255
	ands	r7, r3
	adds	r0, r7, #0
	movs	r1, #5
	bl	sub_08038120
	movs	r1, #3
	ldr	r0, [pc, #600]
	bl	sub_08038040
	adds	r0, r7, #0
	bl	sub_080ad1d8
	movs	r1, #1
	negs	r1, r1
	cmp	r9, r1
	beq.n	.L_080cd270
	mov	r0, r9
	bl	sub_08016cfc
.L_080cd270:
	mov	r2, r8
	cmp	r2, #131
	bne.n	.L_080cd288
	movs	r3, #0
	adds	r0, r6, #0
	movs	r1, #0
	movs	r2, #0
	bl	sub_080200e8
	movs	r3, #0
	str	r3, [r6, #108]
	b.n	.L_080cd39e
.L_080cd288:
	adds	r0, r6, #0
	bl	sub_080200c8
	b.n	.L_080cd39e
.L_080cd290:
	cmp	r7, #0
	bne.n	.L_080cd2a4
	movs	r3, #1
	negs	r3, r3
	cmp	r9, r3
	beq.n	.L_080cd394
	mov	r0, r9
	bl	sub_08016cfc
	b.n	.L_080cd394
.L_080cd2a4:
	mov	r1, r8
	cmp	r1, #131
	bne.n	.L_080cd2b8
	mov	r0, sl
	bl	sub_080ceffc
	ldr	r3, [pc, #504]
	adds	r6, r0, #0
	str	r3, [r6, #108]
	b.n	.L_080cd2c8
.L_080cd2b8:
	mov	r0, sl
	adds	r1, r7, #0
	bl	sub_080cf17c
	adds	r6, r0, #0
	movs	r0, #30
	bl	sub_08013560
.L_080cd2c8:
	adds	r0, r7, #0
	bl	sub_080ad028
	movs	r2, #1
	adds	r5, r0, #0
	negs	r2, r2
	cmp	r5, r2
	bne.n	.L_080cd30e
	adds	r0, r7, #0
	movs	r1, #2
	bl	sub_08038120
	ldr	r5, [pc, #464]
	movs	r1, #1
	adds	r0, r5, #0
	adds	r5, #4
	bl	sub_08038040
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08038040
	mov	r3, r8
	cmp	r3, #131
	beq.n	.L_080cd300
	adds	r0, r6, #0
	bl	sub_080cf340
.L_080cd300:
	mov	r1, r8
	cmp	r1, #128
	bne.n	.L_080cd39e
	mov	r0, sl
	bl	sub_080cef68
	b.n	.L_080cd39e
.L_080cd30e:
	mov	r2, r8
	cmp	r2, #128
	bne.n	.L_080cd31a
	mov	r0, sl
	bl	sub_080cefd0
.L_080cd31a:
	adds	r0, r6, #0
	bl	sub_080cf350
	movs	r0, #83
	bl	sub_081c0010
	movs	r1, #2
	adds	r0, r7, #0
	bl	sub_08038120
	ldr	r3, [pc, #364]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r3, [r3, #0]
	cmp	r5, r3
	bne.n	.L_080cd346
	ldr	r0, [pc, #376]
	movs	r1, #3
	bl	sub_08038040
	b.n	.L_080cd356
.L_080cd346:
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #364]
	movs	r1, #3
	bl	sub_08038040
.L_080cd356:
	movs	r2, #1
	negs	r2, r2
	cmp	r9, r2
	beq.n	.L_080cd364
	mov	r0, r9
	bl	sub_08016cfc
.L_080cd364:
	mov	r3, r8
	cmp	r3, #131
	bne.n	.L_080cd37c
	movs	r3, #0
	adds	r0, r6, #0
	movs	r1, #0
	movs	r2, #0
	bl	sub_080200e8
	movs	r3, #0
	str	r3, [r6, #108]
	b.n	.L_080cd382
.L_080cd37c:
	adds	r0, r6, #0
	bl	sub_080200c8
.L_080cd382:
	mov	r1, r8
	cmp	r1, #133
	bne.n	.L_080cd38e
	mov	r0, sl
	bl	sub_080cf004
.L_080cd38e:
	movs	r2, #0
	str	r2, [sp, #28]
	b.n	.L_080cd39e
.L_080cd394:
	ldr	r0, [pc, #296]
	movs	r1, #1
	adds	r0, r5, r0
	bl	sub_08038040
.L_080cd39e:
	ldr	r1, [sp, #0]
	movs	r3, #0
	strb	r3, [r1, #0]
	bl	sub_080d2350
	ldr	r2, [sp, #28]
	adds	r0, r2, #0
	cmp	r2, #0
	beq.n	.L_080cd3b2
	b.n	.L_080cd568
.L_080cd3b2:
	ldr	r3, [pc, #232]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #106
	adds	r3, r3, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, fp
	bne.n	.L_080cd416
	movs	r3, #1
	movs	r0, #7
	mov	r1, fp
	str	r3, [sp, #16]
	bl	sub_080ccd78
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_080cd426
	mov	r1, fp
	movs	r0, #0
	bl	sub_080ccd78
	movs	r1, #1
	adds	r6, r0, #0
	str	r1, [sp, #12]
	cmp	r6, #0
	bne.n	.L_080cd3ea
	b.n	.L_080cd566
.L_080cd3ea:
	ldr	r3, [r6, #8]
	movs	r2, #128
	lsls	r2, r2, #9
	cmp	r3, r2
	blt.n	.L_080cd420
	mov	r0, fp
	bl	sub_080d1ed8
	adds	r5, r0, #0
	bl	sub_08014878
	ldr	r3, [pc, #192]
	lsls	r0, r0, #1
	lsrs	r0, r0, #16
	lsls	r5, r5, #1
	adds	r5, r5, r0
	adds	r5, r5, r3
	mov	r0, fp
	adds	r1, r5, #0
	bl	sub_080cdea8
	b.n	.L_080cd546
.L_080cd416:
	movs	r0, #0
	mov	r1, fp
	bl	sub_080ccd78
	adds	r6, r0, #0
.L_080cd420:
	cmp	r6, #0
	bne.n	.L_080cd426
	b.n	.L_080cd546
.L_080cd426:
	ldr	r3, [r6, #8]
	cmp	r3, #0
	bne.n	.L_080cd42e
	b.n	.L_080cd546
.L_080cd42e:
	ldr	r1, [sp, #16]
	cmp	r1, #0
	bne.n	.L_080cd484
	ldr	r2, [sp, #24]
	movs	r3, #1
	adds	r2, #91
	strb	r3, [r2, #0]
	movs	r1, #0
	ldr	r0, [sp, #24]
	bl	sub_08020098
	ldr	r2, [sp, #24]
	ldr	r1, [sp, #32]
	ldrh	r2, [r2, #6]
	str	r2, [sp, #20]
	movs	r2, #128
	ldrb	r3, [r1, #22]
	lsls	r2, r2, #17
	lsls	r3, r3, #24
	cmp	r3, r2
	bls.n	.L_080cd460
	movs	r1, #192
	lsls	r1, r1, #18
	cmp	r3, r1
	bne.n	.L_080cd482
.L_080cd460:
	ldr	r5, [pc, #56]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	bl	sub_080d2d84
	ldr	r3, [r0, #16]
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #12]
	bl	sub_080200e8
	ldr	r1, [r5, #0]
	mov	r0, fp
	movs	r2, #0
	bl	sub_080d35d4
.L_080cd482:
	ldr	r3, [r6, #8]
.L_080cd484:
	movs	r1, #128
	lsls	r1, r1, #9
	cmp	r3, r1
	bge.n	.L_080cd4c8
	ldr	r0, [sp, #12]
	bl	sub_08038058
	ldr	r1, [r6, #8]
	mov	r0, fp
	bl	sub_080cdea8
	b.n	sub_080cd4d4
	.4byte 0x02000240
	.4byte 0x00000e1a
	.4byte 0x00000de5
	.4byte 0x00000e1c
	.4byte 0x080cf0d1
	.4byte 0x00000e10
	.4byte 0x00000e0f
	.4byte 0x00000e11
	.4byte 0x00000e12
	.4byte 0x00000def
	.2byte 0x137f
	.2byte 0x0000
.L_080cd4c8:
	bl	sub_080d2260
	ldr	r3, [r6, #8]
	mov	r0, fp
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9a04
	cmp	r2, #0
	bne.n	.L_080cd542
	ldr	r2, [sp, #24]
	movs	r1, #4
	ldrsh	r3, [r2, r1]
	ldr	r2, [r2, #0]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	cmp	r3, #17
	bne.n	.L_080cd532
	ldr	r3, [sp, #32]
	movs	r0, #22
	ldrsb	r0, [r3, r0]
	cmp	r0, #3
	bne.n	.L_080cd51c
	ldr	r3, [pc, #128]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl	sub_080d2d84
	ldr	r2, [sp, #24]
	movs	r3, #1
	str	r0, [r2, #104]
	adds	r1, r2, #0
	adds	r1, #90
	ldrb	r2, [r1, #0]
	orrs	r3, r2
	strb	r3, [r1, #0]
	ldr	r0, [sp, #24]
	ldr	r1, [pc, #100]
	bl	sub_080d4b0c
	b.n	.L_080cd532
.L_080cd51c:
	cmp	r0, #1
	bne.n	.L_080cd532
	ldr	r3, [sp, #24]
	add	r1, sp, #20
	ldrh	r1, [r1, #0]
	adds	r3, #100
	strh	r1, [r3, #0]
	ldr	r0, [sp, #24]
	ldr	r1, [pc, #80]
	bl	sub_080200a8
.L_080cd532:
	ldr	r2, [sp, #24]
	movs	r3, #0
	adds	r2, #91
	strb	r3, [r2, #0]
	ldr	r0, [sp, #24]
	movs	r1, #16
	bl	sub_08020098
.L_080cd542:
	movs	r2, #0
	str	r2, [sp, #28]
.L_080cd546:
	ldr	r3, [sp, #16]
	cmp	r3, #0
	beq.n	.L_080cd566
	ldr	r5, [pc, #40]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #106
	adds	r5, r5, r1
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	bl	sub_080e035c
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r5, #0]
.L_080cd566:
	ldr	r0, [sp, #28]
.L_080cd568:
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x080f3780
	.2byte 0x3300
	.2byte 0x080f
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r1, r5, #0
	movs	r0, #6
	bl	sub_080ccd78
	movs	r3, #1
	negs	r3, r3
	cmp	r0, #0
	beq.n	.L_080cd5b6
	ldr	r1, [r0, #8]
	cmp	r1, #0
	beq.n	.L_080cd5b6
	movs	r2, #128
	lsls	r2, r2, #9
	cmp	r1, r2
	bge.n	.L_080cd5ae
	adds	r0, r3, #0
	bl	sub_080cdea8
	b.n	sub_080cd5b4
.L_080cd5ae:
	adds	r0, r5, #0
	mov	lr, r1
	.2byte 0xf800
	.2byte 0x2300
.L_080cd5b6:
	adds	r0, r3, #0
	pop	{r5, pc}
	.align 2, 0
