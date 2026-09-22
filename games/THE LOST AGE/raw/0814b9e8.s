.syntax unified
	.thumb
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
	.set sub_08014dac, 0x08014dac
	.set sub_08014de4, 0x08014de4
	.set sub_08015024, 0x08015024
	.set sub_080150e4, 0x080150e4
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_08020010, 0x08020010
	.set sub_08020030, 0x08020030
	.set sub_08020048, 0x08020048
	.set sub_08118088, 0x08118088
	.set sub_081180e8, 0x081180e8
	.set sub_0813ba50, 0x0813ba50
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_0814bcb4, 0x0814bcb4
	.set sub_0814c3b0, 0x0814c3b0
	.set sub_0814c424, 0x0814c424
	.set sub_0814c4fa, 0x0814c4fa
	.set sub_0814c71e, 0x0814c71e
	.set sub_0814cc4c, 0x0814cc4c
	.set sub_0814cd48, 0x0814cd48
	.set sub_08152404, 0x08152404
	.set sub_08152470, 0x08152470
	.set sub_0815585c, 0x0815585c
	.set sub_08157cf4, 0x08157cf4
	.set sub_0815e21c, 0x0815e21c
	.set sub_08179e6c, 0x08179e6c
	.set sub_081963ec, 0x081963ec
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Overlay_0814b9e8
Overlay_0814b9e8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #88
	str	r1, [sp, #36]
	str	r0, [sp, #40]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	str	r0, [sp, #32]
	movs	r0, #0
	ldr	r3, [r3, #96]
	str	r3, [sp, #28]
	bl	sub_081435e0
	ldr	r1, [sp, #36]
	cmp	r1, #0
	bne.n	.L_0814ba70
	ldr	r2, [sp, #40]
	movs	r3, #1
	ldr	r1, [r2, #4]
	adds	r0, r2, #0
	eors	r1, r3
	lsls	r1, r1, #4
	movs	r3, #33
	orrs	r1, r3
	mov	r3, sp
	adds	r3, #76
	str	r3, [sp, #16]
	ldr	r2, [sp, #16]
	add	r3, sp, #64
	bl	sub_0815585c
	movs	r2, #184
	ldr	r4, [sp, #32]
	lsls	r2, r2, #6
	adds	r2, #16
	adds	r1, r4, r2
	ldr	r0, [pc, #256]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r4, #216
	ldr	r3, [sp, #32]
	lsls	r4, r4, #7
	adds	r4, #192
	adds	r1, r3, r4
	movs	r2, #1
	movs	r3, #0
	ldr	r0, [pc, #236]
	bl	sub_08157cf4
	ldr	r0, [pc, #236]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #228]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe03c
.L_0814ba70:
	ldr	r0, [sp, #36]
	cmp	r0, #1
	bne.n	.L_0814babc
	ldr	r2, [sp, #40]
	movs	r3, #2
	ldr	r1, [r2, #4]
	eors	r1, r0
	lsls	r1, r1, #4
	orrs	r1, r3
	mov	r3, sp
	adds	r3, #76
	str	r3, [sp, #16]
	adds	r0, r2, #0
	add	r3, sp, #64
	ldr	r2, [sp, #16]
	bl	sub_0815585c
	movs	r2, #216
	ldr	r4, [sp, #32]
	lsls	r2, r2, #7
	adds	r2, #192
	adds	r1, r4, r2
	ldr	r0, [pc, #176]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r4, #184
	ldr	r3, [sp, #32]
	lsls	r4, r4, #6
	adds	r4, #16
	adds	r1, r3, r4
	ldr	r0, [pc, #160]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	b.n	.L_0814baea
.L_0814babc:
	ldr	r2, [sp, #40]
	add	r5, sp, #52
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	adds	r1, r5, #0
	bl	sub_0815e21c
	mov	r3, sp
	adds	r3, #76
	str	r3, [sp, #16]
	ldr	r4, [sp, #16]
	ldr	r3, [r5, #0]
	ldr	r0, [pc, #124]
	str	r3, [r4, #0]
	ldr	r2, [sp, #32]
	movs	r3, #184
	lsls	r3, r3, #6
	adds	r3, #16
	adds	r1, r2, r3
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
.L_0814baea:
	ldr	r4, [sp, #32]
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	lsls	r1, r1, #7
	adds	r2, r4, r0
	movs	r3, #2
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r2, r4, r1
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #80]
	bl	sub_080145a8
	ldr	r3, [pc, #44]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [sp, #16]
	movs	r1, #128
	ldr	r2, [r3, #0]
	movs	r3, #64
	lsls	r1, r1, #19
	subs	r3, r3, r2
	lsls	r3, r3, #8
	adds	r1, #40
	str	r3, [r1, #0]
	ldr	r4, [sp, #40]
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_0814bb5c
	movs	r0, #104
	movs	r1, #7
	bl	sub_081963ec
	b.n	.L_0814bb64
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x000000d0
	.4byte 0x00000154
	.4byte 0x00000130
	.4byte 0x03000730
	.4byte 0x00000150
	.4byte 0x000000d1
	.2byte 0x3001
	.2byte 0x0814
.L_0814bb5c:
	movs	r0, #104
	movs	r1, #3
	bl	sub_081963ec
.L_0814bb64:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r0, #0
	str	r3, [sp, #20]
	mov	r9, r0
.L_0814bb70:
	mov	r1, r9
	cmp	r1, #8
	bne.n	.L_0814bb7c
	movs	r0, #246
	bl	sub_081c0010
.L_0814bb7c:
	mov	r2, r9
	cmp	r2, #0
	bne.n	.L_0814bbf6
	movs	r6, #255
	ldr	r7, [pc, #812]
	ldr	r5, [sp, #32]
	movs	r3, #0
	lsls	r6, r6, #8
	mov	r8, r3
	adds	r6, #255
.L_0814bb90:
	bl	sub_08014878
	ands	r0, r6
	str	r0, [r5, #0]
	bl	sub_08014878
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #8]
	ldr	r4, [sp, #36]
	cmp	r4, #2
	bne.n	.L_0814bbb0
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r5, #4]
	b.n	.L_0814bbb2
.L_0814bbb0:
	str	r7, [r5, #4]
.L_0814bbb2:
	bl	sub_08014878
	movs	r1, #128
	ands	r0, r6
	lsls	r1, r1, #9
	lsls	r0, r0, #1
	adds	r0, r0, r1
	str	r0, [r5, #16]
	bl	sub_08014878
	ands	r0, r6
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r6
	str	r0, [r5, #20]
	bl	sub_08014878
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #255
	movs	r2, #128
	ands	r3, r0
	lsls	r2, r2, #6
	adds	r3, r3, r2
	str	r3, [r5, #24]
	movs	r4, #1
	ldr	r3, [pc, #716]
	add	r8, r4
	mov	r0, r8
	adds	r7, r7, r3
	adds	r5, #28
	cmp	r0, #32
	bne.n	.L_0814bb90
.L_0814bbf6:
	mov	r1, r9
	cmp	r1, #56
	bne.n	.L_0814bc3a
	movs	r0, #212
	bl	sub_081c0010
	movs	r0, #1
	negs	r0, r0
	bl	sub_081180e8
	ldr	r4, [sp, #40]
	movs	r2, #0
	ldr	r3, [r4, #20]
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_0814bc3a
	movs	r5, #36
	movs	r6, #8
.L_0814bc1a:
	ldr	r1, [sp, #40]
	mov	r3, r8
	ldrsh	r0, [r5, r1]
	movs	r2, #1
	movs	r1, #7
	negs	r2, r2
	str	r6, [sp, #0]
	bl	sub_0814cd48
	ldr	r4, [sp, #40]
	movs	r3, #1
	add	r8, r3
	ldr	r3, [r4, #20]
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L_0814bc1a
.L_0814bc3a:
	ldr	r0, [sp, #40]
	ldr	r3, [r0, #28]
	cmp	r3, #1
	bne.n	.L_0814bd32
	mov	r1, r9
	lsls	r5, r1, #11
	adds	r0, r5, #0
	bl	sub_08002096
	negs	r0, r0
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #2
	asrs	r3, r3, #16
	adds	r3, #44
	adds	r0, r5, #0
	mov	r8, r3
	bl	sub_08002090
	ldr	r2, [sp, #16]
	lsls	r0, r0, #2
	ldr	r3, [r2, #4]
	asrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r5, r0, #0
	mov	r3, r9
	subs	r5, #24
	cmp	r3, #32
	ble.n	.L_0814bc7c
	lsls	r3, r3, #2
	subs	r3, r5, r3
	adds	r5, r3, #0
	adds	r5, #128
.L_0814bc7c:
	ldr	r0, [sp, #32]
	movs	r1, #216
	lsls	r1, r1, #7
	adds	r1, #192
	adds	r4, r0, r1
	movs	r6, #40
	adds	r1, r4, #0
	mov	r2, r8
	str	r4, [sp, #8]
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	ldr	r7, [sp, #20]
	ldr	r0, [sp, #28]
	adds	r3, r5, #0
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x464a
	ldr	r4, [sp, #8]
	cmp	r2, #3
	bgt.n	sub_0814bcb4
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	ldr	r0, [sp, #28]
	adds	r1, r4, #0
	mov	r2, r8
	adds	r3, r5, #0
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x464b
	cmp	r3, #79
	bgt.n	.L_0814bd32
	lsrs	r3, r3, #31
	add	r3, r9
	asrs	r3, r3, #1
	lsls	r1, r3, #8
	adds	r1, r1, r3
	ldr	r2, [pc, #500]
	lsls	r3, r1, #16
	adds	r1, r1, r3
	ldr	r4, [sp, #32]
	subs	r2, r2, r1
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r3, [pc, #492]
	adds	r0, r4, r1
	movs	r1, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2200
	mov	r3, r9
	mov	r8, r2
	lsls	r5, r3, #9
.L_0814bce4:
	adds	r0, r5, #0
	bl	sub_08002096
	mov	r3, r9
	muls	r3, r0
	adds	r0, r5, #0
	asrs	r3, r3, #16
	adds	r6, r3, #0
	bl	sub_08002090
	mov	r3, r9
	muls	r3, r0
	adds	r6, #64
	cmp	r3, #0
	bge.n	.L_0814bd04
	adds	r3, #7
.L_0814bd04:
	movs	r2, #1
	ldr	r4, [sp, #32]
	str	r2, [sp, #0]
	movs	r2, #128
	str	r2, [sp, #4]
	movs	r2, #224
	lsls	r2, r2, #3
	asrs	r3, r3, #19
	adds	r1, r4, r2
	subs	r3, #16
	adds	r2, r6, #0
	ldr	r0, [sp, #28]
	ldr	r4, [sp, #20]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2101
	movs	r0, #128
	add	r8, r1
	lsls	r0, r0, #5
	mov	r2, r8
	adds	r5, r5, r0
	cmp	r2, #16
	bne.n	.L_0814bce4
.L_0814bd32:
	bl	sub_08014de4
	ldr	r2, [pc, #396]
	movs	r3, #104
	str	r3, [r2, #16]
	mov	r3, r9
	cmp	r3, #63
	ble.n	.L_0814bd44
	b.n	.L_0814be68
.L_0814bd44:
	movs	r0, #32
	bl	sub_08014dac
	mov	fp, r0
	movs	r0, #1
	bl	sub_081969f8
	mov	r4, r9
	mov	sl, r0
	movs	r3, #0
	cmp	r4, #55
	ble.n	.L_0814bd62
	movs	r3, #56
	subs	r3, r3, r4
	lsls	r3, r3, #3
.L_0814bd62:
	mov	r0, sl
	str	r3, [r0, #20]
	ldr	r2, [pc, #352]
	ldr	r3, [sp, #44]
	movs	r1, #6
	ands	r3, r2
	ldr	r2, [pc, #348]
	orrs	r3, r1
	ands	r3, r2
	movs	r2, #192
	lsls	r2, r2, #3
	orrs	r3, r2
	movs	r4, #184
	ldr	r2, [sp, #32]
	lsls	r4, r4, #6
	adds	r4, #16
	str	r3, [sp, #44]
	adds	r3, r2, r4
	add	r2, sp, #44
	str	r3, [r2, #4]
	ldr	r3, [pc, #324]
	str	r1, [r0, #0]
	mov	r1, fp
	str	r2, [r0, #16]
	str	r3, [r0, #8]
	str	r1, [r0, #12]
	movs	r2, #0
	str	r2, [sp, #12]
	ldr	r6, [sp, #32]
	mov	r8, r2
.L_0814bd9e:
	ldr	r3, [sp, #12]
	ldr	r7, [r6, #24]
	cmp	r9, r3
	blt.n	.L_0814be4a
	bl	sub_08014de4
	movs	r0, #176
	lsls	r0, r0, #4
	adds	r0, #184
	bl	sub_08015024
	ldr	r4, [sp, #36]
	cmp	r4, #2
	bne.n	.L_0814bdc2
	ldr	r3, [r6, #4]
	ldr	r2, [r6, #16]
	subs	r3, r3, r2
	b.n	.L_0814bdc8
.L_0814bdc2:
	ldr	r3, [r6, #4]
	ldr	r2, [r6, #16]
	adds	r3, r3, r2
.L_0814bdc8:
	str	r3, [r6, #4]
	ldr	r3, [r6, #16]
	ldr	r0, [pc, #260]
	cmp	r3, r0
	bge.n	.L_0814bddc
	ldr	r3, [r6, #8]
	movs	r1, #128
	lsls	r1, r1, #9
	adds	r3, r3, r1
	b.n	.L_0814bde4
.L_0814bddc:
	ldr	r3, [r6, #8]
	movs	r2, #192
	lsls	r2, r2, #8
	adds	r3, r3, r2
.L_0814bde4:
	str	r3, [r6, #8]
	ldr	r0, [r6, #0]
	bl	sub_08002096
	movs	r4, #10
	ldrsh	r3, [r6, r4]
	adds	r5, r3, #0
	muls	r5, r0
	ldr	r0, [r6, #0]
	bl	sub_08002090
	movs	r1, #10
	ldrsh	r3, [r6, r1]
	movs	r4, #128
	adds	r2, r3, #0
	muls	r2, r0
	ldr	r3, [r6, #0]
	lsls	r4, r4, #2
	adds	r3, r3, r4
	str	r3, [r6, #0]
	ldr	r1, [r6, #4]
	adds	r0, r5, #0
	bl	sub_08015160
	adds	r1, r7, #0
	adds	r2, r7, #0
	adds	r0, r7, #0
	bl	sub_080151e4
	ldr	r0, [r6, #12]
	bl	sub_08015024
	ldr	r0, [r6, #20]
	bl	sub_080150e4
	ldr	r3, [r6, #12]
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #20]
	ldr	r0, [pc, #160]
	adds	r3, r3, r2
	str	r3, [r6, #20]
	mov	r1, fp
	movs	r2, #4
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_0814be4a:
	ldr	r0, [sp, #12]
	movs	r1, #1
	add	r8, r1
	adds	r0, #3
	mov	r2, r8
	str	r0, [sp, #12]
	adds	r6, #28
	cmp	r2, #16
	bne.n	.L_0814bd9e
	mov	r0, sl
	bl	sub_08013164
	mov	r0, fp
	bl	sub_08013164
.L_0814be68:
	ldr	r2, [pc, #88]
	movs	r3, #120
	str	r3, [r2, #12]
	str	r3, [r2, #16]
	bl	sub_081434f8
	movs	r4, #240
	ldr	r3, [sp, #32]
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	cmp	r1, #64
	beq.n	.L_0814be94
	b.n	.L_0814bb70
.L_0814be94:
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #64]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #88
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xffe00000
	.4byte 0xfffc0000
	.4byte 0x28282828
	.4byte 0x03000260
	.4byte 0x030011e0
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199340
	.4byte 0xfffe0000
	.4byte 0x08199210
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #100
	str	r0, [sp, #56]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #36]
	movs	r5, #0
	str	r0, [sp, #52]
	ldr	r1, [r3, #96]
	str	r1, [sp, #48]
	ldr	r2, [r3, #92]
	ldr	r6, [r3, #100]
	adds	r3, #176
	ldr	r3, [r3, #0]
	mov	sl, r2
	str	r3, [sp, #44]
	ldr	r3, [pc, #452]
	mov	fp, r6
	ldrh	r3, [r3, #4]
	str	r3, [sp, #40]
	bl	sub_0813ba50
	movs	r0, #0
	bl	sub_081435e0
	bl	sub_08179e6c
	movs	r3, #239
	lsls	r3, r3, #7
	add	r3, sl
	movs	r1, #200
	lsls	r1, r1, #4
	str	r5, [r3, #0]
	ldr	r0, [pc, #424]
	bl	sub_080145a8
	movs	r0, #80
	negs	r0, r0
	movs	r1, #0
	mov	r8, r0
	mov	r9, r1
.L_0814bf3e:
	mov	r2, r9
	cmp	r2, #27
	bne.n	.L_0814c014
	ldr	r2, [pc, #404]
	movs	r3, #240
	str	r3, [r2, #16]
	lsls	r3, r3, #7
	adds	r3, #240
	add	r3, sl
	ldr	r0, [r3, #0]
	bl	sub_0814cc4c
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #152
	movs	r3, #8
	add	r2, sl
	negs	r3, r3
	str	r3, [r2, #0]
	ldr	r6, [sp, #44]
	movs	r0, #160
	movs	r3, #1
	lsls	r0, r0, #19
	movs	r1, #128
	str	r3, [r6, #16]
	lsls	r1, r1, #1
	ldr	r3, [pc, #364]
	ldr	r2, [pc, #364]
	adds	r0, #192
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x21e0
	lsls	r1, r1, #3
	ldr	r0, [pc, #356]
	add	r1, sl
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #348]
	movs	r1, #6
	mov	lr, r0
	movs	r5, #0
	mov	ip, r1
	movs	r4, #0
	movs	r0, #0
.L_0814bf9a:
	str	r0, [sp, #8]
	lsls	r2, r4, #1
	movs	r7, #0
	add	r2, fp
.L_0814bfa2:
	mov	r6, lr
	mov	r1, ip
	ldrh	r3, [r6, r1]
	movs	r6, #224
	adds	r3, r3, r7
	lsls	r6, r6, #3
	adds	r3, r3, r6
	mov	r1, sl
	ldrb	r3, [r1, r3]
	cmp	r3, #0
	beq.n	.L_0814bfc2
	ldr	r6, [sp, #8]
	subs	r3, r3, r6
	cmp	r3, #0
	bgt.n	.L_0814bfc2
	movs	r3, #1
.L_0814bfc2:
	adds	r7, #1
	strb	r3, [r2, #0]
	adds	r2, #1
	cmp	r7, #32
	bne.n	.L_0814bfa2
	adds	r5, #1
	adds	r4, #16
	adds	r0, #7
	cmp	r5, #10
	bne.n	.L_0814bf9a
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #276]
	add	r1, sl
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r1, #200
	movs	r2, #200
	movs	r0, #202
	lsls	r1, r1, #7
	lsls	r2, r2, #7
	lsls	r0, r0, #7
	adds	r1, #224
	adds	r2, #192
	movs	r7, #0
	add	r0, sl
	add	r1, sl
	add	r2, sl
.L_0814bffe:
	adds	r3, r7, #0
	adds	r3, #16
	adds	r7, #1
	strb	r3, [r2, #0]
	strb	r3, [r1, #0]
	adds	r2, #1
	strb	r3, [r0, #0]
	adds	r1, #1
	adds	r0, #1
	cmp	r7, #32
	bne.n	.L_0814bffe
.L_0814c014:
	movs	r0, #4
	add	r8, r0
	movs	r5, #128
	movs	r2, #240
	mov	r1, r8
	lsls	r5, r5, #19
	lsls	r2, r2, #7
	lsls	r3, r1, #8
	adds	r5, #40
	adds	r2, #232
	str	r3, [r5, #0]
	add	r2, sl
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	cmp	r3, #28
	beq.n	.L_0814c042
	b.n	.L_0814bf3e
.L_0814c042:
	mov	r6, sp
	adds	r6, #76
	adds	r1, r6, #0
	movs	r0, #0
	str	r6, [sp, #36]
	bl	sub_08144aac
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, sl
	movs	r3, #50
	movs	r0, #0
	str	r3, [r2, #0]
	str	r0, [sp, #16]
	ldr	r0, [pc, #136]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	movs	r2, #128
	ldr	r3, [pc, #128]
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x23f0
	lsls	r3, r3, #7
	adds	r3, #240
	add	r3, sl
	ldr	r0, [r3, #0]
	bl	sub_0814cc4c
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #146
	movs	r2, #1
	movs	r0, #12
	bl	sub_08152404
	movs	r2, #128
	ldr	r1, [sp, #16]
	ldr	r3, [pc, #40]
	lsls	r2, r2, #19
	adds	r2, #32
	str	r1, [r5, #0]
	strh	r3, [r2, #0]
	movs	r2, #0
	mov	r9, r2
.L_0814c0b2:
	ldr	r3, [pc, #72]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0814c0c2
	movs	r3, #1
	str	r3, [sp, #16]
.L_0814c0c2:
	mov	r6, r9
	cmp	r6, #0
	bne.n	.L_0814c100
	movs	r0, #212
	bl	sub_081c0010
	b.n	.L_0814c100
	.4byte 0x00000080
	.4byte 0x03001120
	.4byte 0x08143001
	.4byte 0x030011e0
	.4byte 0x03000260
	.4byte 0x7fff7fff
	.4byte 0x00000134
	.4byte 0x08197410
	.4byte 0x000000c5
	.4byte 0x00000148
	.4byte 0x03000730
	.2byte 0x1150
	.2byte 0x0300
.L_0814c100:
	mov	r0, r9
	cmp	r0, #32
	bne.n	.L_0814c10c
	movs	r0, #191
	bl	sub_081c0010
.L_0814c10c:
	mov	r2, r9
	subs	r2, #78
	cmp	r2, #31
	bhi.n	.L_0814c12a
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	ldr	r2, [pc, #32]
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #32]
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #82
	orrs	r2, r3
	strh	r2, [r1, #0]
.L_0814c12a:
	mov	r1, r9
	cmp	r1, #140
	bne.n	.L_0814c148
	movs	r2, #128
	ldr	r3, [pc, #16]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	b.n	.L_0814c148
	.4byte 0x00000010
	.4byte 0x00001000
	.2byte 0x1010
	.2byte 0x0000
.L_0814c148:
	mov	r2, r9
	cmp	r2, #157
	bne.n	.L_0814c156
	movs	r0, #145
	bl	sub_081180e8
	b.n	.L_0814c164
.L_0814c156:
	mov	r3, r9
	cmp	r3, #161
	beq.n	.L_0814c164
	cmp	r3, #165
	beq.n	.L_0814c164
	cmp	r3, #169
	bne.n	.L_0814c19a
.L_0814c164:
	ldr	r6, [sp, #56]
	movs	r7, #0
	ldr	r3, [r6, #20]
	cmp	r3, #0
	beq.n	.L_0814c19a
	movs	r5, #36
.L_0814c170:
	ldr	r1, [sp, #56]
	ldrsh	r0, [r5, r1]
	movs	r1, #4
	bl	sub_08118088
	ldr	r3, [sp, #56]
	movs	r2, #1
	ldrsh	r0, [r5, r3]
	movs	r3, #4
	str	r3, [sp, #0]
	movs	r1, #7
	adds	r3, r7, #0
	negs	r2, r2
	bl	sub_0814cd48
	ldr	r0, [sp, #56]
	adds	r7, #1
	ldr	r3, [r0, #20]
	adds	r5, #2
	cmp	r7, r3
	bne.n	.L_0814c170
.L_0814c19a:
	mov	r1, r9
	cmp	r1, #0
	bne.n	.L_0814c26e
	ldr	r3, [sp, #44]
	ldr	r1, [pc, #116]
	movs	r2, #1
	movs	r6, #240
	movs	r0, #128
	str	r2, [r3, #16]
	lsls	r6, r6, #16
	lsls	r0, r0, #15
	movs	r2, #0
	movs	r5, #238
	lsls	r5, r5, #7
	str	r6, [sp, #28]
	str	r0, [sp, #32]
	str	r1, [sp, #20]
	str	r2, [sp, #24]
	adds	r5, #220
	movs	r7, #0
	add	r5, sl
.L_0814c1c4:
	adds	r1, r7, #0
	ldmia	r5!, {r0}
	adds	r1, #12
	adds	r7, #1
	bl	sub_08020030
	cmp	r7, #12
	bne.n	.L_0814c1c4
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	movs	r0, #160
	adds	r2, #82
	lsls	r0, r0, #19
	movs	r1, #128
	strh	r3, [r2, #0]
	lsls	r1, r1, #1
	ldr	r3, [pc, #52]
	ldr	r2, [pc, #52]
	adds	r0, #192
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x22ee
	lsls	r2, r2, #7
	adds	r2, #152
	movs	r3, #8
	add	r2, sl
	negs	r3, r3
	str	r3, [r2, #0]
	ldr	r6, [pc, #36]
	ldr	r3, [pc, #36]
	movs	r1, #141
	str	r3, [r6, #12]
	movs	r3, #120
	str	r3, [r6, #16]
	ldr	r0, [sp, #40]
	ldr	r2, [pc, #28]
	lsls	r1, r1, #3
	b.n	.L_0814c230
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0xffec0000
	.4byte 0x03000260
	.4byte 0x7fff7fff
	.4byte 0x030011e0
	.4byte 0xfffffc38
	.2byte 0x1120
	.2byte 0x0300
.L_0814c230:
	adds	r3, r0, r1
	movs	r1, #224
	lsls	r1, r1, #3
	strh	r3, [r2, #4]
	add	r1, sl
	movs	r2, #1
	movs	r3, #1
	ldr	r0, [pc, #136]
	bl	sub_08157cf4
	ldr	r0, [pc, #132]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #128]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x22ef
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, sl
	movs	r3, #50
	str	r3, [r2, #0]
.L_0814c26e:
	movs	r1, #238
	ldr	r6, [pc, #96]
	lsls	r1, r1, #7
	adds	r1, #152
	add	r1, sl
	ldr	r2, [r1, #0]
	ldr	r3, [r6, #12]
	ldr	r0, [pc, #88]
	subs	r3, r3, r2
	str	r3, [r6, #12]
	ldrh	r3, [r0, #4]
	ldr	r2, [r1, #0]
	adds	r1, r0, #0
	adds	r3, r3, r2
	strh	r3, [r1, #4]
	movs	r2, #1
	mov	r3, r9
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0814c348
	mov	r3, r9
	subs	r3, #32
	cmp	r3, #111
	bhi.n	.L_0814c348
	ldr	r6, [sp, #52]
	movs	r0, #206
	lsls	r0, r0, #3
	adds	r3, r6, r0
	ldrh	r0, [r3, #0]
	bl	sub_08013300
	movs	r1, #160
	ldr	r2, [pc, #20]
	lsls	r1, r1, #19
	adds	r1, #192
	movs	r3, #31
	mov	ip, r0
	mov	r8, r1
	movs	r7, #0
	mov	lr, r2
	mov	fp, r3
	b.n	.L_0814c2dc
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x000000c5
	.4byte 0x00000148
	.4byte 0x03000730
	.4byte 0x030011e0
	.2byte 0x1120
	.2byte 0x0300
.L_0814c2dc:
	mov	r6, r8
	ldrh	r2, [r6, #0]
	mov	r5, fp
	lsls	r3, r2, #16
	mov	r1, lr
	lsrs	r0, r3, #26
	lsrs	r4, r3, #21
	ands	r5, r2
	mov	r2, ip
	ands	r0, r1
	ands	r4, r1
	ldrh	r1, [r2, #0]
	mov	r6, lr
	lsls	r2, r1, #16
	lsrs	r3, r2, #26
	ands	r3, r6
	subs	r6, r3, #2
	lsrs	r2, r2, #21
	mov	r3, lr
	ands	r2, r3
	mov	r3, fp
	ands	r3, r1
	subs	r2, #10
	subs	r3, #6
	cmp	r6, #0
	bge.n	.L_0814c312
	movs	r6, #0
.L_0814c312:
	cmp	r2, #0
	bge.n	.L_0814c318
	movs	r2, #0
.L_0814c318:
	cmp	r3, #0
	bge.n	.L_0814c31e
	movs	r3, #0
.L_0814c31e:
	cmp	r0, r6
	ble.n	.L_0814c324
	subs	r0, #1
.L_0814c324:
	cmp	r4, r2
	ble.n	.L_0814c32a
	subs	r4, #1
.L_0814c32a:
	cmp	r5, r3
	ble.n	.L_0814c330
	subs	r5, #1
.L_0814c330:
	lsls	r3, r0, #10
	lsls	r2, r4, #5
	orrs	r3, r2
	mov	r6, r8
	movs	r0, #2
	orrs	r3, r5
	adds	r7, #1
	strh	r3, [r6, #0]
	add	ip, r0
	add	r8, r0
	cmp	r7, #128
	bne.n	.L_0814c2dc
.L_0814c348:
	mov	r3, r9
	subs	r3, #64
	cmp	r3, #35
	bhi.n	.L_0814c362
	ldr	r1, [sp, #20]
	ldr	r3, [sp, #24]
	ldr	r6, [pc, #768]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r2, r1, r2
	adds	r6, r3, r6
	str	r2, [sp, #20]
	str	r6, [sp, #24]
.L_0814c362:
	mov	r0, r9
	cmp	r0, #177
	ble.n	.L_0814c372
	ldr	r1, [sp, #20]
	ldr	r3, [sp, #24]
	ldr	r2, [pc, #748]
	ldr	r6, [pc, #752]
	b.n	.L_0814c394
.L_0814c372:
	mov	r0, r9
	cmp	r0, #139
	ble.n	.L_0814c384
	ldr	r1, [sp, #20]
	ldr	r3, [sp, #24]
	ldr	r2, [pc, #740]
	movs	r6, #128
	lsls	r6, r6, #6
	b.n	.L_0814c394
.L_0814c384:
	mov	r0, r9
	cmp	r0, #99
	ble.n	.L_0814c39c
	ldr	r1, [sp, #20]
	ldr	r3, [sp, #24]
	ldr	r2, [pc, #728]
	movs	r6, #128
	lsls	r6, r6, #4
.L_0814c394:
	adds	r2, r1, r2
	adds	r6, r3, r6
	str	r2, [sp, #20]
	str	r6, [sp, #24]
.L_0814c39c:
	mov	r0, r9
	cmp	r0, #156
	bne.n	sub_0814c3b0
	movs	r1, #240
	ldr	r3, [pc, #708]
	ldr	r0, [sp, #48]
	lsls	r1, r1, #6
	ldr	r2, [pc, #708]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4649
	cmp	r1, #157
	bne.n	.L_0814c402
	movs	r1, #224
	lsls	r1, r1, #3
	movs	r2, #1
	movs	r3, #0
	ldr	r0, [pc, #692]
	add	r1, sl
	bl	sub_08157cf4
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, sl
	movs	r3, #16
	movs	r5, #238
	lsls	r5, r5, #7
	str	r3, [r2, #0]
	adds	r5, #220
	movs	r7, #0
	add	r5, sl
.L_0814c3dc:
	adds	r1, r7, #0
	ldmia	r5!, {r0}
	adds	r1, #24
	adds	r7, #1
	bl	sub_08020030
	cmp	r7, #11
	bne.n	.L_0814c3dc
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, sl
	movs	r3, #50
	str	r3, [r2, #0]
.L_0814c402:
	mov	r3, r9
	subs	r3, #156
	cmp	r3, #7
	bhi.n	sub_0814c424
	movs	r3, #58
	movs	r1, #224
	str	r3, [sp, #0]
	lsls	r1, r1, #3
	movs	r3, #120
	str	r3, [sp, #4]
	ldr	r4, [sp, #76]
	ldr	r0, [sp, #48]
	add	r1, sl
	movs	r2, #12
	movs	r3, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x22ee
	lsls	r2, r2, #7
	adds	r2, #152
	movs	r7, #0
	add	r2, sl
	movs	r1, #114
.L_0814c430:
	cmp	r9, r1
	bne.n	.L_0814c43a
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
.L_0814c43a:
	adds	r7, #1
	adds	r1, #6
	cmp	r7, #8
	bne.n	.L_0814c430
	mov	r2, r9
	cmp	r2, #163
	bne.n	.L_0814c458
	ldr	r6, [pc, #556]
	movs	r0, #255
	ldrh	r3, [r6, #4]
	lsls	r0, r0, #8
	adds	r0, #236
	adds	r3, r3, r0
	adds	r1, r6, #0
	strh	r3, [r1, #4]
.L_0814c458:
	mov	r2, r9
	cmp	r2, #167
	bne.n	.L_0814c46e
	ldr	r6, [pc, #536]
	movs	r0, #255
	ldrh	r3, [r6, #4]
	lsls	r0, r0, #8
	adds	r0, #246
	adds	r3, r3, r0
	adds	r1, r6, #0
	strh	r3, [r1, #4]
.L_0814c46e:
	mov	r2, r9
	cmp	r2, #171
	bne.n	.L_0814c484
	ldr	r6, [pc, #512]
	movs	r0, #255
	ldrh	r3, [r6, #4]
	lsls	r0, r0, #8
	adds	r0, #246
	adds	r3, r3, r0
	adds	r1, r6, #0
	strh	r3, [r1, #4]
.L_0814c484:
	mov	r2, r9
	cmp	r2, #0
	bne.n	.L_0814c4c4
	movs	r7, #0
	movs	r6, #3
	mov	r5, sl
.L_0814c490:
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	subs	r3, #160
	str	r3, [r5, #0]
	str	r6, [r5, #4]
	bl	sub_08014878
	movs	r3, #1
	ands	r0, r3
	str	r0, [r5, #24]
	bl	sub_08014878
	ldr	r3, [r5, #24]
	movs	r2, #3
	ands	r2, r0
	lsls	r3, r3, #3
	adds	r2, r2, r3
	adds	r2, #8
	adds	r7, #1
	str	r2, [r5, #12]
	adds	r6, #6
	adds	r5, #28
	cmp	r7, #20
	bne.n	.L_0814c490
.L_0814c4c4:
	mov	r6, r9
	cmp	r6, #119
	bhi.n	.L_0814c514
	movs	r0, #1
	movs	r7, #0
	ands	r6, r0
	mov	r5, sl
.L_0814c4d2:
	cmp	r6, #0
	beq.n	.L_0814c4dc
	ldr	r1, [sp, #16]
	cmp	r1, #0
	bne.n	sub_0814c4fa
.L_0814c4dc:
	movs	r1, #32
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	str	r1, [sp, #0]
	ldr	r4, [sp, #76]
	ldr	r1, [r5, #24]
	ldr	r0, [sp, #48]
	adds	r1, #1
	str	r1, [sp, #4]
	movs	r1, #200
	lsls	r1, r1, #7
	adds	r1, #192
	add	r1, sl
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x682a
	ldr	r3, [r5, #12]
	adds	r2, r2, r3
	str	r2, [r5, #0]
	cmp	r2, #128
	ble.n	.L_0814c50c
	adds	r3, r2, #0
	subs	r3, #160
	str	r3, [r5, #0]
.L_0814c50c:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #20
	bne.n	.L_0814c4d2
.L_0814c514:
	mov	r2, r9
	cmp	r2, #195
	bls.n	.L_0814c51c
	b.n	.L_0814c828
.L_0814c51c:
	ldr	r3, [pc, #348]
	add	r1, sp, #84
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	movs	r6, #1
	str	r3, [sp, #68]
	str	r4, [sp, #72]
	movs	r3, #0
	str	r3, [r1, #12]
	str	r3, [r1, #4]
	mov	r3, r9
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_0814c53e
	ldr	r0, [sp, #16]
	cmp	r0, #0
	bne.n	.L_0814c5d4
.L_0814c53e:
	mov	r2, r9
	cmp	r2, #156
	ble.n	.L_0814c590
	movs	r5, #238
	movs	r3, #68
	lsls	r5, r5, #7
	ldr	r6, [pc, #308]
	add	r3, sp
	adds	r5, #220
	movs	r7, #0
	mov	r8, r3
	adds	r4, r1, #0
	add	r5, sl
.L_0814c558:
	ldrh	r3, [r6, #0]
	ldr	r0, [sp, #28]
	ldr	r1, [pc, #292]
	lsls	r3, r3, #16
	adds	r3, r3, r0
	adds	r3, r3, r1
	str	r3, [r4, #0]
	ldrh	r3, [r6, #2]
	ldr	r2, [sp, #32]
	lsls	r3, r3, #16
	movs	r0, #128
	adds	r3, r3, r2
	lsls	r0, r0, #13
	adds	r3, r3, r0
	str	r3, [r4, #8]
	adds	r1, r4, #0
	ldmia	r5!, {r0}
	mov	r2, r8
	movs	r3, #0
	str	r4, [sp, #12]
	bl	sub_08020010
	adds	r7, #1
	adds	r6, #4
	ldr	r4, [sp, #12]
	cmp	r7, #11
	bne.n	.L_0814c558
	b.n	.L_0814c5d4
.L_0814c590:
	movs	r6, #238
	movs	r2, #68
	lsls	r6, r6, #7
	ldr	r4, [pc, #240]
	add	r2, sp
	adds	r6, #220
	movs	r7, #0
	mov	r8, r2
	adds	r5, r1, #0
	add	r6, sl
.L_0814c5a4:
	ldrh	r3, [r4, #0]
	ldr	r0, [sp, #28]
	lsls	r3, r3, #16
	adds	r3, r3, r0
	str	r3, [r5, #0]
	ldrh	r3, [r4, #2]
	ldr	r1, [sp, #32]
	ldr	r2, [pc, #216]
	lsls	r3, r3, #16
	adds	r3, r3, r1
	adds	r3, r3, r2
	str	r3, [r5, #8]
	ldmia	r6!, {r0}
	adds	r1, r5, #0
	mov	r2, r8
	movs	r3, #0
	str	r4, [sp, #12]
	bl	sub_08020010
	ldr	r4, [sp, #12]
	adds	r7, #1
	adds	r4, #4
	cmp	r7, #12
	bne.n	.L_0814c5a4
.L_0814c5d4:
	mov	r3, r9
	cmp	r3, #177
	ble.n	.L_0814c602
	ldr	r0, [sp, #28]
	ldr	r6, [sp, #20]
	ldr	r2, [sp, #32]
	adds	r6, r6, r0
	str	r6, [sp, #28]
	ldr	r6, [sp, #20]
	ldr	r1, [sp, #24]
	lsls	r3, r6, #6
	adds	r1, r1, r2
	subs	r3, r3, r6
	str	r1, [sp, #32]
	cmp	r3, #0
	bge.n	.L_0814c5f6
	adds	r3, #63
.L_0814c5f6:
	ldr	r0, [sp, #24]
	asrs	r3, r3, #6
	str	r3, [sp, #20]
	lsls	r3, r0, #6
	subs	r3, r3, r0
	b.n	.L_0814c6be
.L_0814c602:
	mov	r1, r9
	cmp	r1, #149
	ble.n	.L_0814c628
	ldr	r2, [sp, #32]
	ldr	r3, [pc, #132]
	cmp	r2, r3
	ble.n	.L_0814c616
	movs	r6, #0
	str	r6, [sp, #20]
	str	r6, [sp, #24]
.L_0814c616:
	ldr	r1, [sp, #28]
	ldr	r3, [sp, #32]
	ldr	r0, [sp, #20]
	ldr	r2, [sp, #24]
	adds	r0, r0, r1
	adds	r2, r2, r3
	str	r0, [sp, #28]
	str	r2, [sp, #32]
	b.n	.L_0814c6c8
.L_0814c628:
	mov	r6, r9
	cmp	r6, #59
	bgt.n	.L_0814c694
	ldr	r3, [sp, #32]
	ldr	r2, [sp, #24]
	ldr	r6, [sp, #20]
	ldr	r1, [sp, #28]
	adds	r2, r2, r3
	ldr	r0, [sp, #20]
	movs	r3, #58
	muls	r3, r6
	adds	r0, r0, r1
	str	r0, [sp, #28]
	str	r2, [sp, #32]
	cmp	r3, #0
	bge.n	.L_0814c64a
	adds	r3, #63
.L_0814c64a:
	ldr	r0, [sp, #24]
	asrs	r3, r3, #6
	str	r3, [sp, #20]
	lsls	r3, r0, #5
	subs	r3, r3, r0
	lsls	r3, r3, #1
	b.n	.L_0814c6be
	.4byte 0xfffff000
	.4byte 0xfffe0000
	.4byte 0xffff0000
	.4byte 0xffff63c0
	.4byte 0xffffe000
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x000000c6
	.4byte 0x03001120
	.4byte 0x08196e0c
	.4byte 0x08197a38
	.4byte 0xffe00000
	.4byte 0x08197a64
	.4byte 0xfff00000
	.2byte 0xffff
	.2byte 0x0037
.L_0814c694:
	ldr	r6, [sp, #32]
	ldr	r3, [sp, #24]
	ldr	r0, [sp, #20]
	adds	r3, r3, r6
	ldr	r2, [sp, #28]
	ldr	r1, [sp, #20]
	str	r3, [sp, #32]
	movs	r3, #58
	muls	r3, r0
	adds	r1, r1, r2
	str	r1, [sp, #28]
	cmp	r3, #0
	bge.n	.L_0814c6b0
	adds	r3, #63
.L_0814c6b0:
	ldr	r1, [sp, #24]
	asrs	r3, r3, #6
	str	r3, [sp, #20]
	lsls	r3, r1, #4
	subs	r3, r3, r1
	lsls	r3, r3, #2
	adds	r3, r3, r1
.L_0814c6be:
	cmp	r3, #0
	bge.n	.L_0814c6c4
	adds	r3, #63
.L_0814c6c4:
	asrs	r3, r3, #6
	str	r3, [sp, #24]
.L_0814c6c8:
	mov	r3, r9
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0814c6d8
	ldr	r3, [sp, #16]
	cmp	r3, #0
	bne.n	sub_0814c71e
.L_0814c6d8:
	mov	r6, r9
	cmp	r6, #119
	bgt.n	sub_0814c71e
	movs	r1, #3
	mov	r0, r9
	bl	sub_08002054
	movs	r3, #184
	movs	r5, #3
	lsls	r3, r3, #5
	ands	r5, r0
	adds	r3, #112
	adds	r1, r5, #0
	muls	r1, r3
	movs	r0, #224
	ldr	r3, [sp, #28]
	lsls	r0, r0, #3
	add	r1, sl
	adds	r1, r1, r0
	movs	r0, #60
	str	r0, [sp, #0]
	ldr	r6, [sp, #32]
	movs	r0, #100
	lsrs	r2, r3, #31
	str	r0, [sp, #4]
	ldr	r0, [sp, #36]
	adds	r2, r2, r3
	asrs	r2, r2, #17
	asrs	r3, r6, #16
	ldr	r4, [r0, #4]
	subs	r2, #12
	subs	r3, #58
	ldr	r0, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4649
	cmp	r1, #157
	bne.n	.L_0814c730
	ldr	r0, [pc, #460]
	ldr	r1, [pc, #464]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
.L_0814c730:
	mov	r3, r9
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0814c740
	ldr	r3, [sp, #16]
	cmp	r3, #0
	bne.n	.L_0814c828
.L_0814c740:
	mov	r6, r9
	cmp	r6, #156
	bgt.n	.L_0814c748
	b.n	.L_0814c882
.L_0814c748:
	movs	r0, #128
	lsls	r0, r0, #3
	bl	sub_08014dac
	mov	fp, r0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #416]
	ldr	r3, [sp, #60]
	movs	r1, #7
	ands	r3, r2
	ldr	r2, [pc, #412]
	orrs	r3, r1
	ands	r3, r2
	movs	r2, #224
	adds	r6, r0, #0
	lsls	r2, r2, #3
	ldr	r0, [pc, #392]
	orrs	r3, r2
	str	r3, [sp, #60]
	add	r3, sp, #60
	str	r0, [r3, #4]
	str	r3, [r6, #16]
	ldr	r3, [pc, #392]
	str	r1, [r6, #0]
	str	r3, [r6, #8]
	mov	r1, fp
	movs	r3, #0
	str	r1, [r6, #12]
	strb	r3, [r6, #24]
	strb	r3, [r6, #25]
	movs	r7, #0
	mov	r8, r9
.L_0814c78c:
	ldr	r3, [pc, #376]
	ldrb	r3, [r3, r7]
	adds	r1, r3, #0
	adds	r1, #157
	cmp	r9, r1
	ble.n	.L_0814c812
	mov	r3, r9
	subs	r2, r3, r1
	ldr	r3, [pc, #364]
	movs	r0, #128
	ldrb	r3, [r3, r7]
	lsls	r0, r0, #8
	muls	r2, r3
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r2
	lsls	r3, r3, #4
	mov	r2, r9
	adds	r5, r3, r0
	subs	r3, r1, r2
	lsls	r3, r3, #3
	adds	r3, #56
	cmp	r3, #0
	ble.n	.L_0814c7c0
	movs	r3, #0
.L_0814c7c0:
	movs	r0, #64
	negs	r0, r0
	cmp	r3, r0
	ble.n	.L_0814c812
	str	r3, [r6, #20]
	bl	sub_08014de4
	ldr	r3, [pc, #320]
	movs	r2, #0
	ldrsb	r1, [r3, r7]
	ldr	r0, [pc, #316]
	lsls	r1, r1, #16
	bl	sub_08015160
	movs	r1, #3
	lsls	r0, r5, #2
	bl	sub_08002054
	adds	r2, r5, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_080151e4
	movs	r0, #176
	lsls	r0, r0, #4
	adds	r0, #184
	bl	sub_08015024
	mov	r1, r8
	movs	r3, #7
	ands	r3, r1
	lsls	r3, r3, #4
	ldr	r0, [pc, #276]
	strb	r3, [r6, #24]
	mov	r1, fp
	movs	r2, #32
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
.L_0814c812:
	movs	r2, #5
	adds	r7, #1
	add	r8, r2
	cmp	r7, #4
	bne.n	.L_0814c78c
	adds	r0, r6, #0
	bl	sub_08013164
	mov	r0, fp
	bl	sub_08013164
.L_0814c828:
	mov	r3, r9
	cmp	r3, #156
	ble.n	.L_0814c882
	movs	r6, #238
	lsls	r6, r6, #7
	adds	r6, #168
	add	r6, sl
	ldr	r3, [r6, #0]
	cmp	r3, #0
	ble.n	.L_0814c86c
	bl	sub_08014878
	movs	r5, #3
	ands	r5, r0
	bl	sub_08014878
	movs	r3, #15
	ldr	r1, [pc, #208]
	ands	r0, r3
	adds	r2, r0, #0
	adds	r0, #24
	strh	r0, [r1, #6]
	ldr	r0, [pc, #200]
	movs	r3, #120
	subs	r5, #8
	subs	r2, #8
	subs	r5, r3, r5
	subs	r3, r3, r2
	str	r5, [r0, #12]
	str	r3, [r0, #16]
	ldr	r3, [r6, #0]
	subs	r3, #1
	str	r3, [r6, #0]
	b.n	.L_0814c882
.L_0814c86c:
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #164
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #164]
	ldr	r2, [pc, #164]
	strh	r3, [r1, #6]
	movs	r3, #120
	str	r3, [r2, #12]
	str	r3, [r2, #16]
.L_0814c882:
	bl	sub_081434f8
	movs	r2, #1
	mov	r3, r9
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0814c896
	ldr	r3, [sp, #16]
	cmp	r3, #0
	bne.n	.L_0814c8a6
.L_0814c896:
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #232
	add	r3, sl
	str	r2, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
.L_0814c8a6:
	movs	r6, #1
	add	r9, r6
	mov	r0, r9
	cmp	r0, #200
	beq.n	.L_0814c8b4
	bl	.L_0814c0b2
.L_0814c8b4:
	ldr	r0, [pc, #108]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r1, [sp, #44]
	movs	r3, #0
	movs	r5, #238
	lsls	r5, r5, #7
	str	r3, [r1, #16]
	adds	r5, #220
	movs	r7, #0
	add	r5, sl
.L_0814c8d6:
	ldmia	r5!, {r0}
	adds	r7, #1
	bl	sub_08020048
	cmp	r7, #12
	bne.n	.L_0814c8d6
	bl	sub_08143bb8
	add	sp, #100
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x000000c1
	.4byte 0x02014000
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08198ec4
	.4byte 0x08197a94
	.4byte 0x08197a98
	.4byte 0x08197a9c
	.4byte 0xfff00000
	.4byte 0x08198cac
	.4byte 0x03001120
	.4byte 0x030011e0
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #92]
	ldr	r1, [r3, #48]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #176
	adds	r0, r2, r3
	ldr	r3, [r0, #0]
	cmp	r3, #1
	bne.n	.L_0814c956
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #172
	adds	r3, r2, r4
	ldr	r2, [r3, #0]
	ldrh	r3, [r1, #54]
	adds	r3, r3, r2
	movs	r2, #0
	strh	r3, [r1, #54]
	str	r2, [r0, #0]
	b.n	.L_0814c97a
.L_0814c956:
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #172
	adds	r3, r2, r4
	ldr	r2, [r3, #0]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldrh	r3, [r1, #54]
	asrs	r2, r2, #1
	adds	r3, r3, r2
	strh	r3, [r1, #54]
	ldr	r3, [r0, #0]
	cmp	r3, #2
	bne.n	.L_0814c976
	movs	r3, #0
	b.n	.L_0814c978
.L_0814c976:
	movs	r3, #2
.L_0814c978:
	str	r3, [r0, #0]
.L_0814c97a:
	pop	{pc}
	push	{r5, r6, lr}
	adds	r5, r0, #0
	movs	r6, #0
	cmp	r5, #0
	beq.n	.L_0814c990
.L_0814c986:
	adds	r6, #1
	bl	sub_08152470
	cmp	r6, r5
	bne.n	.L_0814c986
.L_0814c990:
	pop	{r5, r6, pc}
