.syntax unified
	.thumb
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_08013e70, 0x08013e70
	.set sub_08013eb4, 0x08013eb4
	.set sub_08013f3c, 0x08013f3c
	.set sub_08013fdc, 0x08013fdc
	.set sub_080144c0, 0x080144c0
	.set sub_08014b70, 0x08014b70
	.set sub_08014bac, 0x08014bac
	.set sub_0801587c, 0x0801587c
	.set sub_081c0010, 0x081c0010
	.global Overlay_081a7518
Overlay_081a7518:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	r8, r0
	movs	r0, #110
	bl	sub_081c0010
	ldr	r2, [pc, #88]
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r5, [pc, #84]
	bl	sub_080144c0
	movs	r0, #1
	bl	sub_08013e70
	bl	sub_08014b70
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	subs	r2, #12
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	movs	r6, #0
	strh	r6, [r3, #10]
	adds	r0, r5, #0
	bl	sub_08013300
	movs	r7, #128
	movs	r3, #128
	movs	r2, #132
	lsls	r7, r7, #1
	lsls	r3, r3, #19
	movs	r1, #160
	lsls	r2, r2, #24
	adds	r7, #255
	adds	r4, r0, #0
	adds	r3, #212
	lsls	r1, r1, #19
	adds	r2, #112
	b.n	.L_081a758c
	.4byte 0x00000681
	.4byte 0x00001440
	.4byte 0x0300120c
	.4byte 0x00000020
	.2byte 0x1120
	.2byte 0x0300
.L_081a758c:
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [pc, #164]
	movs	r3, #224
	lsls	r3, r3, #1
	adds	r4, r4, r3
	adds	r1, r5, #0
	adds	r0, r4, #0
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r5, #0
	ldr	r1, [pc, #144]
	ldr	r2, [pc, #148]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r1, [pc, #144]
	movs	r3, #128
	lsls	r3, r3, #1
	movs	r5, #0
.L_081a75b8:
	movs	r0, #0
.L_081a75ba:
	adds	r2, r3, #0
	movs	r4, #128
	lsls	r3, r2, #16
	lsls	r4, r4, #9
	adds	r3, r3, r4
	adds	r0, #1
	strh	r2, [r1, #0]
	asrs	r3, r3, #16
	adds	r1, #2
	cmp	r0, #29
	bls.n	.L_081a75ba
	strh	r7, [r1, #0]
	adds	r5, #1
	adds	r1, #2
	strh	r7, [r1, #0]
	adds	r1, #2
	cmp	r5, #19
	bls.n	.L_081a75b8
	ldr	r3, [pc, #104]
	movs	r5, #0
	movs	r2, #0
.L_081a75e4:
	adds	r5, #1
	strh	r2, [r3, #2]
	strh	r2, [r3, #0]
	adds	r3, #4
	cmp	r5, #3
	bls.n	.L_081a75e4
	movs	r3, #128
	movs	r1, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r1, r1, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #72]
	adds	r1, #16
	adds	r2, #4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bl	sub_08014bac
	bl	sub_08014b70
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L_081a766e
	movs	r0, #1
	bl	sub_08013eb4
	bl	sub_08013fdc
	ldr	r3, [pc, #32]
	movs	r2, #9
	ldr	r3, [r3, #4]
	movs	r5, #0
	b.n	.L_081a7662
	movs	r0, r0
	.4byte 0x00001540
	.4byte 0x02010000
	.4byte 0x06004000
	.4byte 0x84002580
	.4byte 0x06003000
	.4byte 0x03001120
	.2byte 0x1150
	.2byte 0x0300
.L_081a7650:
	movs	r0, #1
	adds	r5, #1
	bl	sub_08013560
	cmp	r5, #119
	bhi.n	.L_081a76ea
	ldr	r3, [pc, #148]
	movs	r2, #9
	ldr	r3, [r3, #4]
.L_081a7662:
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081a7650
	movs	r6, #1
	negs	r6, r6
	b.n	.L_081a76ea
.L_081a766e:
	ldr	r3, [pc, #132]
	movs	r2, #9
	ldr	r3, [r3, #4]
	movs	r5, #0
	b.n	.L_081a768a
.L_081a7678:
	movs	r0, #1
	adds	r5, #1
	bl	sub_08013560
	cmp	r5, #59
	bhi.n	.L_081a7694
	ldr	r3, [pc, #108]
	movs	r2, #9
	ldr	r3, [r3, #4]
.L_081a768a:
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081a7678
	movs	r6, #1
	negs	r6, r6
.L_081a7694:
	cmp	r6, #0
	beq.n	.L_081a76a0
	movs	r0, #8
	bl	sub_08013eb4
	b.n	.L_081a76a6
.L_081a76a0:
	movs	r0, #60
	bl	sub_08013eb4
.L_081a76a6:
	bl	sub_08013fdc
	cmp	r6, #0
	bne.n	.L_081a76d8
	ldr	r3, [pc, #68]
	movs	r2, #9
	ldr	r3, [r3, #4]
	movs	r5, #0
	b.n	.L_081a76ca
.L_081a76b8:
	movs	r0, #1
	adds	r5, #1
	bl	sub_08013560
	cmp	r5, #179
	bhi.n	.L_081a76d4
	ldr	r3, [pc, #44]
	movs	r2, #9
	ldr	r3, [r3, #4]
.L_081a76ca:
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081a76b8
	movs	r6, #1
	negs	r6, r6
.L_081a76d4:
	cmp	r6, #0
	beq.n	.L_081a76e0
.L_081a76d8:
	movs	r0, #8
	bl	sub_08013e70
	b.n	.L_081a76e6
.L_081a76e0:
	movs	r0, #60
	bl	sub_08013e70
.L_081a76e6:
	bl	sub_08013fdc
.L_081a76ea:
	adds	r0, r6, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	ldr	r2, [pc, #96]
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r6, [pc, #92]
	bl	sub_080144c0
	movs	r0, #1
	bl	sub_08013e70
	bl	sub_08014b70
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	subs	r2, #12
	strh	r3, [r2, #0]
	ldr	r3, [pc, #60]
	movs	r5, #0
	strh	r5, [r3, #10]
	ldr	r5, [pc, #56]
	adds	r0, r6, #0
	mov	r8, r3
	bl	sub_08013300
	adds	r1, r5, #0
	bl	sub_0801587c
	movs	r3, #128
	movs	r2, #132
	adds	r6, r5, #0
	lsls	r3, r3, #19
	movs	r1, #160
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r6, #0
	lsls	r1, r1, #19
	adds	r2, #112
	b.n	.L_081a7770
	movs	r0, r0
	.4byte 0x00000685
	.4byte 0x00001440
	.4byte 0x0300120c
	.4byte 0x00000021
	.4byte 0x03001120
	.2byte 0x0000
	.2byte 0x0201
.L_081a7770:
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #224
	lsls	r3, r3, #1
	adds	r6, r6, r3
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r6, #0
	ldr	r1, [pc, #132]
	ldr	r2, [pc, #132]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r6, r6, r3
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r6, #0
	ldr	r1, [pc, #116]
	ldr	r2, [pc, #120]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #128
	lsls	r3, r3, #7
	adds	r6, r6, r3
	movs	r5, #0
	movs	r2, #0
	mov	r3, r8
.L_081a77ac:
	adds	r5, #1
	strh	r2, [r3, #2]
	strh	r2, [r3, #0]
	adds	r3, #4
	cmp	r5, #3
	bls.n	.L_081a77ac
	movs	r3, #128
	movs	r1, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r1, r1, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #80]
	adds	r1, #16
	adds	r2, #4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bl	sub_08014bac
	bl	sub_08014b70
	movs	r0, #1
	bl	sub_08013f3c
	bl	sub_08013fdc
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	movs	r2, #132
	ldr	r0, [r3, #0]
	movs	r3, #3
	lsrs	r0, r0, #3
	ands	r0, r3
	movs	r3, #128
	lsls	r0, r0, #10
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	movs	r5, #0
	b.n	.L_081a7840
	movs	r0, r0
	.4byte 0x00001540
	.4byte 0x06003000
	.4byte 0x84000200
	.4byte 0x06004000
	.4byte 0x84001000
	.4byte 0x03001120
	.2byte 0x122c
	.2byte 0x0300
.L_081a7820:
	movs	r0, #1
	adds	r5, #1
	bl	sub_08013560
	cmp	r5, #119
	bhi.n	.L_081a7858
	ldr	r3, [pc, #48]
	movs	r2, #132
	ldr	r0, [r3, #0]
	movs	r3, #3
	lsrs	r0, r0, #3
	ands	r0, r3
	movs	r3, #128
	lsls	r0, r0, #10
	lsls	r3, r3, #19
	lsls	r2, r2, #24
.L_081a7840:
	adds	r3, #212
	adds	r0, r0, r6
	ldr	r1, [pc, #28]
	adds	r2, #208
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #24]
	movs	r2, #9
	ldr	r3, [r3, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081a7820
.L_081a7858:
	movs	r0, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.4byte 0x0300122c
	.4byte 0x06004100
	.4byte 0x03001150
