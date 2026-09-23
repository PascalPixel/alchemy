.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_080143ac, 0x080143ac
	.set sub_08014bac, 0x08014bac
	.set sub_08016ca4, 0x08016ca4
	.set sub_08039260, 0x08039260
	.set sub_0803939c, 0x0803939c
	.set sub_080393fc, 0x080393fc
	.set sub_0803d3c0, 0x0803d3c0
	.set sub_0803d450, 0x0803d450
	.set sub_0803f9c0, 0x0803f9c0
	.set sub_08041c54, 0x08041c54
	.set sub_08042314, 0x08042314
	.set sub_08044348, 0x08044348
	.set sub_080443d8, 0x080443d8
	.set sub_08044498, 0x08044498
	.set sub_080444e8, 0x080444e8
	.set sub_08108030, 0x08108030
	.set sub_08108038, 0x08108038
	.set sub_08108040, 0x08108040
	.set sub_08108048, 0x08108048
	.set sub_08108088, 0x08108088
	.set sub_081c0010, 0x081c0010
	.global Func_08044558
	.thumb_func
Func_08044558:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #100
	str	r0, [sp, #48]
	mov	r1, sp
	movs	r0, #0
	str	r0, [sp, #40]
	str	r0, [sp, #36]
	str	r0, [sp, #32]
	adds	r1, #85
	ldr	r0, [sp, #48]
	add	r6, sp, #84
	str	r1, [sp, #28]
	bl	sub_08016ca4
	str	r0, [sp, #24]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	movs	r2, #1
	str	r3, [sp, #20]
	str	r2, [sp, #16]
	mov	fp, r2
	bl	sub_08014bac
	ldr	r3, [pc, #348]
	movs	r0, #147
	movs	r1, #128
	lsls	r0, r0, #1
	lsls	r1, r1, #2
	adds	r0, #255
	adds	r1, #38
	adds	r2, r3, r0
	adds	r3, r3, r1
	ldrb	r0, [r2, #0]
	ldrb	r1, [r3, #0]
	bl	sub_0803f9c0
	movs	r5, #2
	movs	r1, #6
	movs	r2, #24
	movs	r3, #9
	movs	r0, #3
	str	r5, [sp, #0]
	bl	sub_08039260
	movs	r1, #3
	mov	sl, r0
	movs	r2, #8
	movs	r3, #3
	movs	r0, #8
	str	r5, [sp, #0]
	bl	sub_08039260
	movs	r2, #3
	str	r0, [sp, #44]
	movs	r3, #1
	ldr	r0, [sp, #48]
	movs	r1, #0
	bl	sub_0803d3c0
	ldr	r1, [pc, #280]
	mov	r0, sl
	bl	sub_08044348
	movs	r3, #7
	str	r3, [sp, #0]
	mov	r0, sl
	movs	r1, #18
	movs	r2, #0
	movs	r3, #18
	bl	sub_08041c54
	add	r2, sp, #16
	ldr	r3, [sp, #20]
	ldrb	r2, [r2, #0]
	mov	r0, sp
	strb	r2, [r3, #3]
	add	r3, sp, #40
	ldrb	r3, [r3, #0]
	adds	r0, #98
	strb	r3, [r6, #0]
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #24]
.L_0804460a:
	ldrb	r3, [r2, #0]
	adds	r2, #1
	strb	r3, [r1, #0]
	adds	r1, #1
	cmp	r3, #0
	beq.n	.L_08044622
	ldr	r3, [sp, #36]
	adds	r3, #1
	str	r3, [sp, #36]
	ldr	r3, [sp, #32]
	adds	r3, #1
	str	r3, [sp, #32]
.L_08044622:
	cmp	r1, r0
	ble.n	.L_0804460a
	ldr	r0, [sp, #28]
	movs	r3, #0
	strb	r3, [r0, #14]
	ldr	r1, [sp, #24]
	ldr	r0, [sp, #44]
	bl	sub_080444e8
	movs	r1, #5
	mov	r8, r1
	bl	sub_080143ac
	adds	r5, r0, #0
	movs	r7, #18
	cmp	r5, #95
	bgt.n	.L_08044680
	ldr	r2, [pc, #180]
	movs	r1, #128
	bl	sub_08108088
	movs	r1, #128
	movs	r3, #0
	lsls	r1, r1, #23
	adds	r0, r5, #0
	mov	r2, sl
	str	r3, [sp, #0]
	bl	sub_08042314
	mov	r2, sp
	adds	r2, #68
	adds	r5, r0, #0
	str	r2, [sp, #12]
	str	r5, [r2, #0]
	mov	r0, sl
	movs	r3, #12
	ldrsh	r1, [r0, r3]
	movs	r3, #14
	ldrsh	r2, [r0, r3]
	lsls	r1, r1, #3
	lsls	r2, r2, #3
	adds	r1, #140
	adds	r2, #52
	ldr	r0, [sp, #12]
	bl	sub_08108048
	b.n	.L_08044686
.L_08044680:
	mov	r0, sp
	adds	r0, #68
	str	r0, [sp, #12]
.L_08044686:
	bl	sub_080143ac
	adds	r5, r0, #0
	cmp	r5, #95
	bgt.n	.L_08044704
	ldr	r2, [pc, #108]
	movs	r1, #128
	bl	sub_08108088
	movs	r1, #128
	movs	r3, #0
	lsls	r1, r1, #23
	adds	r0, r5, #0
	mov	r2, sl
	str	r3, [sp, #0]
	bl	sub_08042314
	movs	r1, #52
	adds	r5, r0, #0
	add	r1, sp
	str	r5, [r1, #0]
	movs	r3, #255
	ldrb	r2, [r5, #25]
	strb	r3, [r5, #15]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r5, #25]
	ldr	r0, [sp, #28]
	mov	r9, r1
	bl	sub_08044498
	adds	r1, r0, #0
	adds	r1, #70
	mov	r0, r9
	movs	r2, #22
	bl	sub_08108048
	b.n	.L_08044708
.L_080446d4:
	mov	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #13
	strb	r3, [r2, #5]
	ldr	r0, [sp, #44]
	bl	sub_080393fc
	ldr	r0, [sp, #44]
	ldr	r1, [sp, #24]
	bl	sub_080444e8
	movs	r0, #10
	bl	sub_08013560
	b.n	.L_08044a0c
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x080aa2d8
	.4byte 0x000001fa
	.2byte 0x0202
	.2byte 0x0000
.L_08044704:
	add	r0, sp, #52
	mov	r9, r0
.L_08044708:
	ldr	r4, [pc, #60]
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #52]
	adds	r1, r4, #0
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #36]
	strh	r3, [r4, #8]
	ldr	r1, [sp, #36]
	ldr	r2, [sp, #28]
	adds	r1, r1, r2
	str	r1, [sp, #8]
.L_0804472a:
	movs	r5, #1
	cmp	r7, #18
	bne.n	.L_08044750
	mov	r3, r8
	cmp	r3, #4
	bne.n	.L_08044738
	movs	r5, #3
.L_08044738:
	mov	r0, r8
	cmp	r0, #5
	bne.n	.L_08044750
	movs	r5, #3
	b.n	.L_08044750
	movs	r0, r0
	.4byte 0x00006318
	.4byte 0x050001c0
	.2byte 0x01e0
	.2byte 0x0500
.L_08044750:
	movs	r1, #1
	movs	r3, #14
	str	r1, [sp, #0]
	str	r3, [sp, #4]
	adds	r1, r7, #0
	mov	r2, r8
	adds	r3, r5, #0
	mov	r0, sl
	bl	sub_080443d8
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #1
	movs	r3, #15
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	mov	r0, sl
	adds	r3, r5, #0
	adds	r1, r7, #0
	mov	r2, r8
	bl	sub_080443d8
	mov	r3, fp
	cmp	r3, #0
	beq.n	.L_080447a6
	movs	r0, #0
	mov	r3, sl
	movs	r2, #12
	ldrsh	r1, [r3, r2]
	mov	fp, r0
	movs	r0, #14
	ldrsh	r2, [r3, r0]
	adds	r1, r1, r7
	add	r2, r8
	lsls	r1, r1, #3
	lsls	r2, r2, #3
	subs	r1, #7
	adds	r2, #15
	ldr	r0, [sp, #12]
	movs	r3, #3
	bl	sub_08108040
.L_080447a6:
	ldr	r1, [sp, #16]
	cmp	r1, #0
	beq.n	.L_080447c4
	movs	r2, #0
	ldr	r0, [sp, #28]
	str	r2, [sp, #16]
	bl	sub_08044498
	adds	r1, r0, #0
	adds	r1, #70
	mov	r0, r9
	movs	r2, #22
	movs	r3, #3
	bl	sub_08108040
.L_080447c4:
	ldr	r0, [sp, #12]
	bl	sub_08108030
	mov	r0, r9
	bl	sub_08108038
	ldr	r3, [pc, #68]
	mov	r1, r9
	ldr	r0, [r3, #0]
	ldr	r5, [r1, #0]
	ldr	r4, [pc, #64]
	movs	r3, #7
	lsrs	r0, r0, #1
	ands	r0, r3
	ldrsb	r3, [r4, r0]
	ldrh	r1, [r5, #6]
	ldr	r2, [pc, #40]
	adds	r1, r1, r3
	ldr	r3, [pc, #40]
	adds	r0, #5
	ands	r1, r3
	ldrh	r3, [r5, #22]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r5, #22]
	movs	r3, #7
	ands	r0, r3
	ldrb	r2, [r5, #8]
	ldrb	r3, [r4, r0]
	adds	r2, r2, r3
	strb	r2, [r5, #20]
	ldr	r5, [pc, #28]
	movs	r3, #64
	ldr	r2, [r5, #12]
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0804485a
	b.n	.L_08044824
	.4byte 0xfffffe00
	.4byte 0x000001ff
	.4byte 0x03001100
	.4byte 0x0805f656
	.2byte 0x1150
	.2byte 0x0300
.L_08044824:
	movs	r0, #111
	bl	sub_081c0010
	movs	r1, #1
	movs	r0, #1
	negs	r1, r1
	mov	fp, r0
	add	r8, r1
	cmp	r7, #18
	beq.n	.L_08044842
	cmp	r8, r1
	bne.n	.L_08044858
	movs	r2, #5
	mov	r8, r2
	b.n	.L_08044858
.L_08044842:
	mov	r0, r8
	movs	r3, #3
	eors	r3, r0
	negs	r2, r3
	orrs	r2, r3
	lsrs	r2, r2, #31
	mov	r8, r2
	mov	r1, r8
	movs	r3, #5
	subs	r1, r3, r1
	mov	r8, r1
.L_08044858:
	ldr	r5, [pc, #500]
.L_0804485a:
	ldr	r3, [pc, #500]
	ldr	r2, [r3, #12]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08044896
	movs	r0, #111
	bl	sub_081c0010
	movs	r0, #1
	mov	fp, r0
	add	r8, r0
	cmp	r7, #18
	beq.n	.L_08044882
	mov	r1, r8
	cmp	r1, #6
	bne.n	.L_08044894
	movs	r2, #0
	mov	r8, r2
	b.n	.L_08044894
.L_08044882:
	mov	r3, r8
	movs	r2, #6
	eors	r2, r3
	negs	r3, r2
	orrs	r3, r2
	lsrs	r3, r3, #31
	mov	r8, r3
	movs	r0, #4
	add	r8, r0
.L_08044894:
	ldr	r5, [pc, #440]
.L_08044896:
	ldr	r1, [pc, #440]
	movs	r3, #32
	ldr	r2, [r1, #12]
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080448d6
	movs	r0, #111
	bl	sub_081c0010
	movs	r3, #1
	movs	r2, #1
	subs	r7, #1
	negs	r3, r3
	mov	fp, r2
	cmp	r7, r3
	bne.n	.L_080448c6
	mov	r3, r8
	subs	r3, #4
	cmp	r3, #1
	bhi.n	.L_080448c2
	movs	r7, #18
	b.n	.L_080448d4
.L_080448c2:
	movs	r7, #16
	b.n	.L_080448d4
.L_080448c6:
	cmp	r7, #5
	beq.n	.L_080448d2
	cmp	r7, #11
	beq.n	.L_080448d2
	cmp	r7, #17
	bne.n	.L_080448d4
.L_080448d2:
	subs	r7, #1
.L_080448d4:
	ldr	r5, [pc, #376]
.L_080448d6:
	ldr	r0, [pc, #376]
	movs	r3, #16
	ldr	r2, [r0, #12]
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08044914
	movs	r0, #111
	bl	sub_081c0010
	adds	r7, #1
	movs	r1, #1
	mov	fp, r1
	cmp	r7, #19
	bne.n	.L_080448f6
	movs	r7, #0
	b.n	.L_08044904
.L_080448f6:
	cmp	r7, #5
	beq.n	.L_08044902
	cmp	r7, #11
	beq.n	.L_08044902
	cmp	r7, #17
	bne.n	.L_08044904
.L_08044902:
	adds	r7, #1
.L_08044904:
	cmp	r7, #18
	bne.n	.L_08044912
	mov	r3, r8
	subs	r3, #4
	cmp	r3, #1
	bls.n	.L_08044912
	movs	r7, #0
.L_08044912:
	ldr	r5, [pc, #316]
.L_08044914:
	ldr	r3, [pc, #312]
	ldr	r2, [r3, #4]
	movs	r3, #8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08044930
	movs	r0, #111
	bl	sub_081c0010
	movs	r0, #1
	movs	r1, #5
	mov	fp, r0
	movs	r7, #18
	mov	r8, r1
.L_08044930:
	ldr	r6, [r5, #12]
	movs	r3, #2
	ands	r6, r3
	cmp	r6, #0
	beq.n	.L_08044972
	movs	r0, #113
	bl	sub_081c0010
.L_08044940:
	ldr	r2, [sp, #32]
	cmp	r2, #0
	beq.n	.L_0804496a
	ldr	r3, [sp, #8]
	subs	r2, #1
	subs	r3, #1
	str	r3, [sp, #8]
	ldr	r0, [sp, #8]
	movs	r3, #0
	str	r2, [sp, #32]
	strb	r3, [r0, #0]
	ldr	r0, [sp, #44]
	bl	sub_080393fc
	ldr	r1, [sp, #28]
	ldr	r0, [sp, #44]
	bl	sub_080444e8
	movs	r1, #1
	str	r1, [sp, #16]
	b.n	.L_0804472a
.L_0804496a:
	movs	r2, #1
	negs	r2, r2
	str	r2, [sp, #40]
	b.n	.L_08044a0c
.L_08044972:
	ldr	r3, [r5, #12]
	movs	r0, #1
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_0804497e
	b.n	.L_0804472a
.L_0804497e:
	movs	r0, #112
	bl	sub_081c0010
	cmp	r7, #18
	bne.n	.L_080449b6
	mov	r1, r8
	cmp	r1, #5
	bne.n	.L_080449ac
	ldr	r2, [sp, #32]
	cmp	r2, #0
	bne.n	.L_08044996
	b.n	.L_080446d4
.L_08044996:
	ldr	r1, [sp, #24]
	ldr	r0, [sp, #28]
	movs	r2, #0
.L_0804499c:
	ldrb	r3, [r0, #0]
	adds	r2, #1
	strb	r3, [r1, #0]
	adds	r0, #1
	adds	r1, #1
	cmp	r2, #14
	ble.n	.L_0804499c
	b.n	.L_08044a0c
.L_080449ac:
	mov	r3, r8
	cmp	r3, #4
	beq.n	.L_080449b4
	b.n	.L_0804472a
.L_080449b4:
	b.n	.L_08044940
.L_080449b6:
	mov	r1, sl
	movs	r0, #14
	ldrsh	r3, [r1, r0]
	movs	r0, #12
	ldrsh	r2, [r1, r0]
	add	r3, r8
	ldr	r1, [sp, #20]
	adds	r2, r2, r7
	adds	r3, #1
	adds	r2, #1
	lsls	r3, r3, #6
	lsls	r2, r2, #1
	adds	r3, r1, r3
	adds	r3, r3, r2
	ldr	r2, [sp, #32]
	ldrb	r3, [r3, #8]
	cmp	r2, #5
	bne.n	.L_080449dc
	b.n	.L_0804472a
.L_080449dc:
	ldr	r0, [sp, #8]
	adds	r2, #1
	strb	r3, [r0, #0]
	adds	r0, #1
	str	r0, [sp, #8]
	strb	r6, [r0, #0]
	str	r2, [sp, #32]
	cmp	r2, #5
	bne.n	.L_080449f8
	movs	r1, #5
	movs	r2, #1
	movs	r7, #18
	mov	r8, r1
	mov	fp, r2
.L_080449f8:
	ldr	r0, [sp, #44]
	bl	sub_080393fc
	ldr	r0, [sp, #44]
	ldr	r1, [sp, #28]
	bl	sub_080444e8
	movs	r3, #1
	str	r3, [sp, #16]
	b.n	.L_0804472a
.L_08044a0c:
	mov	r0, sl
	movs	r1, #2
	bl	sub_0803939c
	movs	r1, #2
	ldr	r0, [sp, #44]
	bl	sub_0803939c
	ldr	r0, [sp, #48]
	bl	sub_0803d450
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [pc, #36]
	ldr	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08044a3e
	adds	r5, r2, #0
.L_08044a32:
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_08044a32
.L_08044a3e:
	ldr	r0, [sp, #40]
	add	sp, #100
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x1150
	.2byte 0x0300
	movs	r0, #0
	bx	lr
