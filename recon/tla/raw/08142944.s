.syntax unified
	.thumb
	.set sub_08001c90, 0x08001c90
	.set sub_08001cb0, 0x08001cb0
	.set sub_08001cd0, 0x08001cd0
	.set sub_08001cf0, 0x08001cf0
	.set sub_08002054, 0x08002054
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013164, 0x08013164
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014cc0, 0x08014cc0
	.set sub_08014dac, 0x08014dac
	.set sub_08014de4, 0x08014de4
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_08118150, 0x08118150
	.set sub_08138086, 0x08138086
	.set sub_08142bbe, 0x08142bbe
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cd48, 0x0814cd48
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815b434, 0x0815b434
	.set sub_0815e21c, 0x0815e21c
	.set sub_08164a4c, 0x08164a4c
	.set sub_0818caa8, 0x0818caa8
	.set sub_081963ec, 0x081963ec
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Func_08142944
	.thumb_func
Func_08142944:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #80
	str	r1, [sp, #52]
	movs	r1, #246
	lsls	r1, r1, #7
	str	r0, [sp, #56]
	adds	r1, #124
	movs	r0, #92
	bl	sub_08014cc0
	movs	r1, #128
	str	r0, [sp, #48]
	lsls	r1, r1, #7
	movs	r0, #96
	bl	sub_08014cc0
	movs	r1, #240
	str	r0, [sp, #44]
	ldr	r0, [sp, #48]
	lsls	r1, r1, #7
	adds	r1, #228
	adds	r2, r0, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #0
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r1, #3
	movs	r0, #104
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	ldr	r2, [sp, #48]
	str	r3, [sp, #36]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #28]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r4, [sp, #48]
	movs	r2, #220
	lsls	r2, r2, #6
	adds	r1, r4, r2
	ldr	r0, [pc, #16]
	movs	r2, #0
	movs	r3, #0
	b.n	.L_081429d0
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x0000013e
	.2byte 0x00c9
	.2byte 0x0000
.L_081429d0:
	bl	sub_08157cf4
	ldr	r0, [pc, #388]
	ldr	r1, [pc, #392]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r3, [sp, #48]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r0, [sp, #48]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #132
	adds	r2, r0, r1
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #356]
	bl	sub_080145a8
	movs	r4, #238
	ldr	r3, [sp, #48]
	lsls	r4, r4, #7
	adds	r4, #180
	adds	r2, r3, r4
	movs	r3, #24
	str	r3, [r2, #0]
	ldr	r0, [sp, #48]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #184
	adds	r2, r0, r1
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r3, [sp, #52]
	movs	r2, #100
	str	r2, [sp, #32]
	cmp	r3, #80
	beq.n	.L_08142a2e
	movs	r4, #76
	str	r4, [sp, #32]
.L_08142a2e:
	ldr	r1, [sp, #32]
	movs	r0, #0
	mov	r9, r0
	cmp	r1, #0
	bne.n	.L_08142a3a
	b.n	.L_08142f86
.L_08142a3a:
	movs	r3, #96
	negs	r3, r3
	movs	r2, #68
	str	r3, [sp, #8]
	add	r2, sp
	mov	fp, r2
.L_08142a46:
	ldr	r4, [sp, #56]
	mov	r1, fp
	ldr	r0, [r4, #8]
	bl	sub_0815e21c
	ldr	r0, [sp, #52]
	cmp	r0, #78
	bne.n	.L_08142a66
	mov	r1, fp
	ldr	r3, [r1, #0]
	adds	r3, #40
	str	r3, [r1, #0]
	ldr	r3, [r1, #4]
	subs	r3, #24
	str	r3, [r1, #4]
	b.n	.L_08142a84
.L_08142a66:
	ldr	r2, [sp, #52]
	cmp	r2, #79
	bne.n	.L_08142a7c
	mov	r4, fp
	ldr	r3, [r4, #0]
	subs	r3, #16
	str	r3, [r4, #0]
	ldr	r3, [r4, #4]
	subs	r3, #24
	str	r3, [r4, #4]
	b.n	.L_08142a84
.L_08142a7c:
	mov	r0, fp
	ldr	r3, [r0, #4]
	subs	r3, #48
	str	r3, [r0, #4]
.L_08142a84:
	mov	r1, r9
	cmp	r1, #0
	bne.n	.L_08142ae0
	ldr	r3, [sp, #48]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r0, [sp, #48]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #132
	adds	r2, r0, r1
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r2, [sp, #52]
	cmp	r2, #79
	bne.n	.L_08142ac0
	ldr	r0, [pc, #188]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #180]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe009
.L_08142ac0:
	ldr	r0, [pc, #172]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #160]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x464b
	cmp	r3, #0
	bne.n	.L_08142ae0
	movs	r0, #140
	bl	sub_081c0010
.L_08142ae0:
	mov	r4, r9
	cmp	r4, #48
	bne.n	.L_08142aec
	movs	r0, #144
	bl	sub_081c0010
.L_08142aec:
	mov	r0, r9
	cmp	r0, #56
	bne.n	.L_08142af8
	movs	r0, #148
	bl	sub_081c0010
.L_08142af8:
	mov	r1, r9
	cmp	r1, #32
	bne.n	.L_08142b10
	ldr	r2, [sp, #56]
	movs	r3, #1
	negs	r3, r3
	ldr	r0, [r2, #8]
	movs	r1, #7
	adds	r2, r3, #0
	str	r3, [sp, #0]
	bl	sub_0814cd48
.L_08142b10:
	mov	r3, r9
	cmp	r3, #68
	bne.n	.L_08142b28
	ldr	r4, [sp, #56]
	movs	r3, #1
	negs	r3, r3
	ldr	r0, [r4, #8]
	movs	r1, #0
	adds	r2, r3, #0
	str	r3, [sp, #0]
	bl	sub_0814cd48
.L_08142b28:
	ldr	r0, [sp, #52]
	cmp	r0, #80
	bne.n	.L_08142b78
	mov	r1, r9
	cmp	r1, #61
	ble.n	.L_08142b84
	ldr	r3, [sp, #48]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r0, [sp, #48]
	movs	r1, #238
	lsls	r1, r1, #7
	ldr	r3, [pc, #44]
	adds	r1, #132
	adds	r2, r0, r1
	str	r3, [r2, #0]
	movs	r0, #2
	movs	r1, #2
	movs	r2, #2
	bl	sub_08164a4c
	b.n	.L_08142b84
	movs	r0, r0
	.4byte 0x000000da
	.4byte 0x02012000
	.4byte 0x08143001
	.4byte 0x00000148
	.4byte 0x03000730
	.4byte 0x0000014a
	.2byte 0x1010
	.2byte 0x1010
.L_08142b78:
	mov	r2, r9
	cmp	r2, #62
	bne.n	.L_08142b84
	ldr	r0, [sp, #52]
	bl	sub_08118150
.L_08142b84:
	mov	r3, r9
	cmp	r3, #47
	bgt.n	.L_08142b8c
	b.n	.L_08142cb4
.L_08142b8c:
	cmp	r3, #48
	bne.n	sub_08142bbe
	ldr	r3, [pc, #844]
	movs	r2, #1
	movs	r7, #0
	negs	r2, r2
.L_08142b98:
	adds	r7, #1
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r7, #64
	bne.n	.L_08142b98
	ldr	r4, [sp, #48]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #168
	adds	r3, r4, r0
	movs	r2, #8
	str	r2, [r3, #0]
	movs	r1, #240
	ldr	r3, [pc, #816]
	ldr	r0, [sp, #44]
	lsls	r1, r1, #6
	ldr	r2, [pc, #812]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x464b
	subs	r3, #48
	cmp	r3, #7
	bhi.n	.L_08142c62
	bl	sub_08014878
	ldr	r3, [sp, #68]
	movs	r2, #63
	lsrs	r1, r3, #31
	adds	r3, r3, r1
	asrs	r3, r3, #1
	ands	r2, r0
	adds	r2, r2, r3
	subs	r2, #32
	lsls	r2, r2, #16
	str	r2, [sp, #28]
	bl	sub_08014878
	ldr	r2, [sp, #72]
	movs	r3, #31
	ands	r3, r0
	ldr	r4, [sp, #8]
	adds	r3, r3, r2
	lsls	r3, r3, #16
	str	r3, [sp, #24]
	ldr	r0, [pc, #760]
	lsls	r3, r4, #3
	subs	r3, r3, r4
	movs	r1, #0
	movs	r2, #15
	lsls	r3, r3, #2
	mov	r8, r1
	mov	sl, r2
	adds	r7, r3, r0
.L_08142c02:
	bl	sub_08014878
	movs	r5, #31
	ands	r5, r0
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08014878
	mov	r1, sl
	ands	r0, r1
	ldr	r2, [sp, #28]
	subs	r0, #8
	lsls	r0, r0, #16
	adds	r0, r2, r0
	str	r0, [r7, #0]
	bl	sub_08014878
	mov	r3, sl
	ldr	r4, [sp, #24]
	ands	r0, r3
	subs	r0, #8
	lsls	r0, r0, #16
	adds	r0, r4, r0
	str	r0, [r7, #4]
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #3
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	movs	r0, #1
	asrs	r3, r3, #3
	add	r8, r0
	str	r3, [r7, #16]
	mov	r1, r8
	movs	r3, #0
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r1, #2
	bne.n	.L_08142c02
.L_08142c62:
	ldr	r5, [pc, #648]
	movs	r7, #0
.L_08142c66:
	ldr	r0, [r5, #24]
	cmp	r0, #17
	bhi.n	.L_08142cac
	movs	r1, #3
	bl	sub_08002054
	ldr	r2, [sp, #48]
	adds	r1, r0, #0
	lsls	r1, r1, #11
	movs	r3, #224
	adds	r1, r2, r1
	lsls	r3, r3, #3
	movs	r4, #2
	ldrsh	r2, [r5, r4]
	adds	r1, r1, r3
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	movs	r0, #32
	str	r0, [sp, #0]
	movs	r0, #64
	subs	r3, #48
	str	r0, [sp, #4]
	subs	r2, #16
	ldr	r0, [sp, #44]
	ldr	r4, [sp, #36]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #62
	ldr	r2, [pc, #588]
	bl	sub_08138086
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_08142cac:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #16
	bne.n	.L_08142c66
.L_08142cb4:
	mov	r0, r9
	cmp	r0, #0
	bge.n	.L_08142cbc
	b.n	.L_08142f54
.L_08142cbc:
	ldr	r3, [sp, #68]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [sp, #20]
	ldr	r3, [sp, #72]
	lsls	r3, r3, #16
	str	r3, [sp, #16]
	cmp	r0, #63
	bgt.n	.L_08142cfa
	mov	r3, r9
	adds	r3, #8
	movs	r5, #128
	cmp	r3, #64
	bgt.n	.L_08142cea
	lsls	r5, r3, #1
	movs	r2, #128
	ldr	r0, [pc, #528]
	adds	r1, r5, #0
	lsls	r2, r2, #9
	bl	sub_0815b434
.L_08142cea:
	ldr	r2, [sp, #20]
	ldr	r3, [sp, #16]
	asrs	r1, r2, #16
	ldr	r0, [pc, #512]
	asrs	r2, r3, #16
	adds	r3, r5, #0
	bl	sub_0818caa8
.L_08142cfa:
	mov	r4, r9
	cmp	r4, #0
	bne.n	.L_08142d3e
	ldr	r5, [sp, #48]
	movs	r7, #0
	movs	r6, #0
.L_08142d06:
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r0
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	adds	r3, #128
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #1
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L_08142d32
	ldr	r3, [r5, #4]
	negs	r3, r3
	str	r3, [r5, #4]
.L_08142d32:
	adds	r7, #1
	str	r6, [r5, #24]
	subs	r6, #4
	adds	r5, #28
	cmp	r7, #16
	bne.n	.L_08142d06
.L_08142d3e:
	movs	r5, #128
	lsls	r5, r5, #2
	adds	r0, r5, #0
	bl	sub_08014dac
	str	r0, [sp, #12]
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #420]
	ldr	r3, [sp, #60]
	mov	r8, r0
	ands	r3, r2
	movs	r2, #5
	orrs	r3, r2
	ldr	r2, [pc, #412]
	mov	r1, r8
	movs	r0, #0
	str	r0, [r1, #20]
	ands	r3, r2
	ldr	r2, [sp, #48]
	movs	r4, #220
	orrs	r3, r5
	lsls	r4, r4, #6
	str	r3, [sp, #60]
	add	r6, sp, #60
	adds	r3, r2, r4
	str	r3, [r6, #4]
	movs	r3, #7
	str	r3, [r1, #0]
	ldr	r3, [pc, #388]
	str	r6, [r1, #16]
	str	r3, [r1, #8]
	ldr	r0, [sp, #12]
	movs	r7, #0
	str	r0, [r1, #12]
	ldr	r5, [sp, #48]
	movs	r1, #128
	lsls	r1, r1, #9
	mov	sl, r1
.L_08142d8e:
	ldr	r2, [r5, #4]
	ldr	r1, [r5, #24]
	ldr	r3, [r5, #0]
	lsls	r2, r2, #1
	adds	r1, #2
	adds	r3, r3, r2
	str	r1, [r5, #24]
	str	r3, [r5, #0]
	cmp	r1, #47
	bhi.n	.L_08142e08
	bl	sub_08014de4
	movs	r1, #3
	ands	r1, r7
	movs	r2, #128
	lsls	r0, r1, #17
	lsls	r2, r2, #10
	ldr	r3, [sp, #20]
	adds	r0, r0, r2
	ldr	r2, [sp, #16]
	ldr	r4, [pc, #332]
	adds	r1, #2
	lsls	r1, r1, #16
	adds	r0, r3, r0
	subs	r1, r2, r1
	adds	r0, r0, r4
	adds	r1, r1, r4
	movs	r2, #0
	bl	sub_08015160
	movs	r0, #128
	mov	r1, sl
	mov	r2, sl
	lsls	r0, r0, #8
	bl	sub_080151e4
	ldr	r0, [r5, #0]
	bl	sub_080150e4
	ldr	r2, [r5, #24]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #170
	adds	r0, r2, #0
	muls	r0, r3
	bl	sub_08002096
	movs	r1, #128
	add	r0, sl
	lsls	r1, r1, #10
	mov	r2, sl
	bl	sub_080151e4
	ldr	r0, [pc, #268]
	ldr	r1, [sp, #12]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, r8
	bl	sub_08196a7c
.L_08142e08:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #16
	bne.n	.L_08142d8e
	mov	r3, r8
	str	r6, [r3, #16]
	ldr	r4, [sp, #12]
	movs	r2, #6
	str	r4, [r3, #12]
	add	r3, sp, #60
	strb	r2, [r3, #0]
	ldr	r3, [pc, #236]
	mov	r0, r8
	str	r3, [r6, #4]
	movs	r3, #7
	str	r3, [r0, #0]
	ldr	r3, [pc, #228]
	mov	r1, r9
	strb	r2, [r6, #1]
	str	r3, [r0, #8]
	lsls	r1, r1, #3
	movs	r7, #0
	mov	sl, r1
.L_08142e36:
	ldr	r3, [pc, #220]
	movs	r2, #127
	ldrb	r3, [r3, r7]
	adds	r1, r3, #0
	mov	r3, sl
	ands	r3, r2
	adds	r1, #48
	mov	r2, r8
	strb	r3, [r2, #25]
	cmp	r9, r1
	ble.n	.L_08142f3c
	mov	r4, r9
	subs	r3, r1, r4
	lsls	r3, r3, #3
	adds	r0, r3, #0
	movs	r2, #16
	adds	r0, #56
	negs	r2, r2
	cmp	r0, r2
	ble.n	.L_08142e62
	movs	r0, #16
	negs	r0, r0
.L_08142e62:
	movs	r3, #64
	negs	r3, r3
	cmp	r0, r3
	ble.n	.L_08142f3c
	ldr	r3, [pc, #172]
	mov	r4, r9
	ldrb	r3, [r3, r7]
	subs	r2, r4, r1
	muls	r2, r3
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r2
	movs	r1, #128
	mov	r2, r8
	lsls	r6, r3, #4
	lsls	r1, r1, #7
	str	r0, [r2, #20]
	adds	r5, r6, r1
	bl	sub_08014de4
	mov	r3, fp
	ldr	r0, [r3, #0]
	movs	r1, #128
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #60
	lsls	r0, r0, #16
	lsls	r1, r1, #14
	movs	r2, #0
	bl	sub_08015160
	ldr	r3, [pc, #116]
	movs	r4, #128
	ldrb	r3, [r3, r7]
	lsls	r4, r4, #11
	muls	r3, r5
	lsls	r0, r5, #1
	adds	r1, r3, r4
	cmp	r5, #0
	bge.n	.L_08142ebe
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r2, #3
	adds	r5, r6, r2
.L_08142ebe:
	asrs	r2, r5, #2
	bl	sub_080151e4
	movs	r0, #250
	lsls	r0, r0, #2
	bl	sub_08015024
	movs	r3, #1
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08142f20
	lsls	r0, r7, #3
	add	r0, r9
	lsls	r0, r0, #10
	bl	sub_08015068
	b.n	.L_08142f2c
	.4byte 0x02014018
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x02014000
	.4byte 0xfffff800
	.4byte 0x02010000
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199220
	.4byte 0xffc00000
	.4byte 0x081991b0
	.4byte 0x02012000
	.4byte 0x081990d0
	.4byte 0x081977ec
	.4byte 0x081977f0
	.2byte 0x77f4
	.2byte 0x0819
.L_08142f20:
	lsls	r0, r7, #3
	mov	r3, r9
	subs	r0, r0, r3
	lsls	r0, r0, #10
	bl	sub_08015068
.L_08142f2c:
	ldr	r0, [pc, #188]
	ldr	r1, [sp, #12]
	movs	r2, #32
	bl	sub_08196958
	mov	r0, r8
	bl	sub_08196a7c
.L_08142f3c:
	movs	r4, #32
	adds	r7, #1
	add	sl, r4
	cmp	r7, #4
	beq.n	.L_08142f48
	b.n	.L_08142e36
.L_08142f48:
	mov	r0, r8
	bl	sub_08013164
	ldr	r0, [sp, #12]
	bl	sub_08013164
.L_08142f54:
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r1, #240
	ldr	r0, [sp, #48]
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r2, r0, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #8]
	ldr	r4, [sp, #32]
	movs	r3, #1
	adds	r2, #2
	add	r9, r3
	str	r2, [sp, #8]
	cmp	r9, r4
	beq.n	.L_08142f86
	b.n	.L_08142a46
.L_08142f86:
	ldr	r0, [pc, #104]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [sp, #52]
	cmp	r0, #80
	bne.n	.L_08142fcc
	movs	r0, #195
	lsls	r0, r0, #1
	bl	sub_081c0010
	movs	r4, #238
	ldr	r1, [sp, #48]
	lsls	r4, r4, #7
	adds	r4, #160
	adds	r3, r1, r4
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #68]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #164
	strh	r3, [r2, #4]
	adds	r3, r1, r0
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #60]
	strh	r3, [r2, #6]
	ldr	r2, [pc, #60]
	movs	r3, #120
	str	r3, [r2, #12]
	str	r3, [r2, #16]
	bl	sub_08014644
	b.n	.L_08142fd0
.L_08142fcc:
	bl	sub_08143bb8
.L_08142fd0:
	movs	r0, #96
	bl	sub_0801314c
	movs	r0, #92
	bl	sub_0801314c
	add	sp, #80
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x08199090
	.4byte 0x08143001
	.4byte 0x03001120
	.4byte 0x08143489
	.2byte 0x11e0
	.2byte 0x0300
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #92]
	ldr	r5, [r3, #96]
	ldr	r1, [pc, #244]
	ldrh	r3, [r1, #0]
	adds	r0, r3, #0
	strh	r1, [r1, #0]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #232
	adds	r2, r6, r3
	ldr	r3, [r2, #0]
	cmp	r3, #1
	bne.n	.L_081430ee
	movs	r3, #0
	str	r3, [r2, #0]
	strh	r0, [r1, #0]
	movs	r2, #239
	lsls	r2, r2, #7
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	cmp	r3, #4
	bhi.n	.L_081430de
	ldr	r2, [pc, #208]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x08143050
	.4byte 0x08143060
	.4byte 0x08143086
	.4byte 0x081430b0
	.2byte 0x30c8
	.2byte 0x0814
	movs	r2, #128
	ldr	r3, [pc, #180]
	ldr	r0, [pc, #180]
	adds	r1, r5, #0
	lsls	r2, r2, #7
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe03e
	movs	r2, #128
	ldr	r3, [pc, #164]
	adds	r1, r5, #0
	lsls	r2, r2, #7
	ldr	r0, [pc, #160]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x22ee
	lsls	r2, r2, #7
	adds	r2, #132
	adds	r3, r6, r2
	movs	r1, #128
	ldr	r2, [r3, #0]
	adds	r0, r5, #0
	ldr	r3, [pc, #144]
	lsls	r1, r1, #7
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe02b
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	cmp	r3, #50
	bne.n	.L_081430a2
	movs	r2, #128
	ldr	r1, [pc, #116]
	lsls	r2, r2, #7
	adds	r0, r5, #0
	bl	sub_08001cb0
	b.n	.L_081430de
.L_081430a2:
	movs	r2, #128
	ldr	r1, [pc, #100]
	lsls	r2, r2, #7
	adds	r0, r5, #0
	bl	sub_08001c90
	b.n	.L_081430de
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	adds	r3, r6, r2
	ldr	r1, [r3, #0]
	movs	r3, #128
	ldr	r2, [pc, #76]
	lsls	r3, r3, #7
	adds	r0, r5, #0
	bl	sub_08001cf0
	b.n	.L_081430de
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	adds	r3, r6, r2
	ldr	r1, [r3, #0]
	movs	r3, #128
	ldr	r2, [pc, #52]
	lsls	r3, r3, #7
	adds	r0, r5, #0
	bl	sub_08001cd0
.L_081430de:
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #228
	adds	r3, r6, r2
	ldr	r2, [r3, #0]
	movs	r2, #1
	str	r2, [r3, #0]
	b.n	.L_081430fe
.L_081430ee:
	strh	r0, [r1, #0]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #228
	adds	r2, r6, r3
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
.L_081430fe:
	pop	{r5, r6, pc}
	.4byte 0x04000208
	.4byte 0x0814303c
	.4byte 0x03000730
	.4byte 0x06004000
	.2byte 0x0260
	.2byte 0x0300
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #92]
	ldr	r1, [pc, #72]
	ldrh	r3, [r1, #0]
	adds	r0, r3, #0
	strh	r1, [r1, #0]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #236
	adds	r2, r5, r3
	ldr	r3, [r2, #0]
	cmp	r3, #1
	bne.n	.L_08143164
	movs	r3, #0
	str	r3, [r2, #0]
	strh	r0, [r1, #0]
	movs	r2, #246
	lsls	r2, r2, #7
	adds	r2, #116
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	movs	r3, #207
	lsls	r3, r3, #7
	adds	r5, r5, r3
	movs	r2, #128
	ldr	r3, [pc, #32]
	adds	r1, r5, #0
	lsls	r2, r2, #5
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2180
	ldr	r3, [pc, #24]
	adds	r0, r5, #0
	lsls	r1, r1, #5
	movs	r2, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe000
.L_08143164:
	strh	r0, [r1, #0]
	pop	{r5, pc}
	.4byte 0x04000208
	.4byte 0x03000730
	.2byte 0x0260
	.2byte 0x0300
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #92]
	ldr	r1, [pc, #208]
	ldrh	r3, [r1, #0]
	adds	r0, r3, #0
	strh	r1, [r1, #0]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #232
	adds	r2, r5, r3
	ldr	r3, [r2, #0]
	cmp	r3, #1
	bne.n	.L_0814323c
	movs	r3, #0
	str	r3, [r2, #0]
	strh	r0, [r1, #0]
	movs	r2, #239
	lsls	r2, r2, #7
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	cmp	r3, #1
	beq.n	.L_081431c8
	cmp	r3, #1
	bgt.n	.L_081431ae
	cmp	r3, #0
	beq.n	.L_081431b8
	b.n	.L_0814322c
.L_081431ae:
	cmp	r3, #2
	beq.n	.L_081431ee
	cmp	r3, #3
	beq.n	.L_08143216
	b.n	.L_0814322c
.L_081431b8:
	movs	r2, #240
	ldr	r3, [pc, #152]
	ldr	r0, [pc, #152]
	ldr	r1, [pc, #156]
	lsls	r2, r2, #7
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe031
.L_081431c8:
	movs	r2, #240
	ldr	r3, [pc, #136]
	ldr	r1, [pc, #140]
	lsls	r2, r2, #7
	ldr	r0, [pc, #132]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x22ee
	lsls	r2, r2, #7
	adds	r2, #132
	adds	r3, r5, r2
	movs	r1, #240
	ldr	r2, [r3, #0]
	ldr	r0, [pc, #120]
	ldr	r3, [pc, #120]
	lsls	r1, r1, #7
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe01e
.L_081431ee:
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	cmp	r3, #50
	bne.n	.L_08143208
	ldr	r1, [pc, #88]
	adds	r2, #124
	ldr	r0, [pc, #88]
	bl	sub_08001cb0
	b.n	.L_0814322c
.L_08143208:
	movs	r2, #240
	ldr	r1, [pc, #76]
	lsls	r2, r2, #7
	ldr	r0, [pc, #76]
	bl	sub_08001c90
	b.n	.L_0814322c
.L_08143216:
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	adds	r3, r5, r2
	ldr	r1, [r3, #0]
	movs	r3, #240
	ldr	r2, [pc, #52]
	lsls	r3, r3, #7
	ldr	r0, [pc, #52]
	bl	sub_08001cf0
.L_0814322c:
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #228
	adds	r3, r5, r2
	ldr	r2, [r3, #0]
	movs	r2, #1
	str	r2, [r3, #0]
	b.n	.L_0814324c
.L_0814323c:
	strh	r0, [r1, #0]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #228
	adds	r2, r5, r3
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
.L_0814324c:
	pop	{r5, pc}
	movs	r0, r0
	.4byte 0x04000208
	.4byte 0x03000730
	.4byte 0x06008000
	.4byte 0x02010000
	.2byte 0x0260
	.2byte 0x0300
	push	{r5, r6, lr}
	movs	r0, #192
	lsls	r0, r0, #18
	ldr	r6, [r0, #92]
	ldr	r2, [pc, #212]
	ldrh	r3, [r2, #0]
	adds	r1, r3, #0
	strh	r2, [r2, #0]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #232
	adds	r4, r6, r3
	ldr	r3, [r4, #0]
	cmp	r3, #1
	bne.n	.L_08143330
	ldr	r5, [r0, #96]
	movs	r3, #0
	str	r3, [r4, #0]
	strh	r1, [r2, #0]
	movs	r2, #239
	lsls	r2, r2, #7
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	cmp	r3, #1
	beq.n	.L_081432ba
	cmp	r3, #1
	bgt.n	.L_081432a0
	cmp	r3, #0
	beq.n	.L_081432aa
	b.n	.L_08143320
.L_081432a0:
	cmp	r3, #2
	beq.n	.L_081432e0
	cmp	r3, #3
	beq.n	.L_0814330a
	b.n	.L_08143320
.L_081432aa:
	movs	r2, #240
	ldr	r3, [pc, #152]
	ldr	r0, [pc, #156]
	adds	r1, r5, #0
	lsls	r2, r2, #7
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe032
.L_081432ba:
	movs	r2, #240
	ldr	r3, [pc, #136]
	adds	r1, r5, #0
	lsls	r2, r2, #7
	ldr	r0, [pc, #136]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x22ee
	lsls	r2, r2, #7
	adds	r2, #132
	adds	r3, r6, r2
	movs	r1, #240
	ldr	r2, [r3, #0]
	adds	r0, r5, #0
	ldr	r3, [pc, #120]
	lsls	r1, r1, #6
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe01f
.L_081432e0:
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	cmp	r3, #50
	bne.n	.L_081432fc
	movs	r2, #240
	ldr	r1, [pc, #88]
	lsls	r2, r2, #6
	adds	r0, r5, #0
	bl	sub_08001cb0
	b.n	.L_08143320
.L_081432fc:
	movs	r2, #240
	ldr	r1, [pc, #76]
	lsls	r2, r2, #6
	adds	r0, r5, #0
	bl	sub_08001c90
	b.n	.L_08143320
.L_0814330a:
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	adds	r3, r6, r2
	ldr	r1, [r3, #0]
	movs	r3, #240
	ldr	r2, [pc, #52]
	lsls	r3, r3, #6
	adds	r0, r5, #0
	bl	sub_08001cf0
.L_08143320:
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #228
	adds	r3, r6, r2
	ldr	r2, [r3, #0]
	movs	r2, #1
	str	r2, [r3, #0]
	b.n	.L_08143340
.L_08143330:
	strh	r1, [r2, #0]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #228
	adds	r2, r6, r3
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
.L_08143340:
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x04000208
	.4byte 0x03000730
	.4byte 0x06008000
	.2byte 0x0260
	.2byte 0x0300
	push	{r5, r6, lr}
	ldr	r6, [pc, #76]
	movs	r1, #128
	movs	r2, #1
	ldr	r5, [pc, #72]
	adds	r0, r6, #0
	lsls	r1, r1, #1
	negs	r2, r2
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2380
	lsls	r3, r3, #1
	adds	r6, r6, r3
	adds	r0, r6, #0
	movs	r1, #128
	ldr	r2, [pc, #56]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2480
	ldr	r1, [pc, #52]
	lsls	r4, r4, #10
	adds	r6, #128
	movs	r0, #0
	adds	r4, #2
.L_08143384:
	movs	r3, #0
.L_08143386:
	adds	r3, #1
	stmia	r6!, {r1}
	adds	r1, r1, r4
	cmp	r3, #8
	bne.n	.L_08143386
	ldr	r2, [pc, #24]
	movs	r3, #0
.L_08143394:
	adds	r3, #1
	stmia	r6!, {r2}
	cmp	r3, #8
	bne.n	.L_08143394
	adds	r0, #1
	cmp	r0, #16
	bne.n	.L_08143384
	pop	{r5, r6, pc}
	.4byte 0x0600f800
	.4byte 0x03000260
	.4byte 0x03ff03ff
	.2byte 0x0200
	.2byte 0x0201
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #92]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #188
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #64
	strh	r3, [r1, #0]
	adds	r0, #2
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	adds	r1, #4
	strh	r3, [r1, #0]
	adds	r0, #2
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	subs	r1, #2
	strh	r3, [r1, #0]
	adds	r0, #2
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	adds	r1, #4
	strh	r3, [r1, #0]
	adds	r0, #2
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	adds	r1, #2
	strh	r3, [r1, #0]
	adds	r0, #2
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	adds	r1, #2
	strh	r3, [r1, #0]
	adds	r0, #2
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	subs	r1, #74
	strh	r3, [r1, #0]
	adds	r0, #2
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	adds	r1, #80
	strh	r3, [r1, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #204
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r1, #2
	strh	r3, [r1, #0]
	bx	lr
	.2byte 0x0000
