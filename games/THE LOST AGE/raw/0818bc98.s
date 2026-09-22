.syntax unified
	.thumb
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
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_08118088, 0x08118088
	.set sub_081180e8, 0x081180e8
	.set sub_08138086, 0x08138086
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cd48, 0x0814cd48
	.set sub_08157530, 0x08157530
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815b510, 0x0815b510
	.set sub_0815e21c, 0x0815e21c
	.set sub_0815f000, 0x0815f000
	.set sub_0818bfd8, 0x0818bfd8
	.set sub_0818c01c, 0x0818c01c
	.set sub_0818c180, 0x0818c180
	.set sub_0818c720, 0x0818c720
	.set sub_0818c7b8, 0x0818c7b8
	.set sub_081963ec, 0x081963ec
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Overlay_0818bc98
Overlay_0818bc98:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #108
	str	r1, [sp, #64]
	movs	r3, #192
	lsls	r3, r3, #18
	mov	fp, r0
	ldr	r0, [r3, #92]
	ldr	r2, [sp, #64]
	str	r0, [sp, #60]
	ldr	r1, [r3, #96]
	str	r1, [sp, #56]
	ldr	r3, [r3, #100]
	str	r3, [sp, #48]
	cmp	r2, #0
	beq.n	.L_0818bcca
	movs	r0, #0
	bl	sub_081435e0
	b.n	.L_0818bcd0
.L_0818bcca:
	movs	r0, #1
	bl	sub_081435e0
.L_0818bcd0:
	movs	r3, #0
	ldr	r0, [pc, #268]
	ldr	r1, [sp, #48]
	movs	r2, #0
	bl	sub_08157cf4
	ldr	r3, [sp, #64]
	cmp	r3, #2
	bne.n	.L_0818bcf8
	ldr	r0, [pc, #256]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #248]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe009
.L_0818bcf8:
	ldr	r0, [pc, #240]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #228]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4838
	ldr	r1, [pc, #228]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #220]
	ldr	r1, [pc, #224]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r5, [sp, #64]
	cmp	r5, #0
	bne.n	.L_0818bd92
	ldr	r6, [sp, #60]
	movs	r7, #0
	mov	sl, r6
.L_0818bd30:
	movs	r5, #224
	lsls	r5, r5, #3
	lsls	r0, r7, #2
	add	r5, sl
	movs	r6, #0
	mov	r8, r0
.L_0818bd3c:
	mov	r1, fp
	ldr	r3, [r1, #24]
	cmp	r3, #0
	bne.n	.L_0818bd56
	lsls	r0, r6, #10
	bl	sub_08002096
	lsls	r0, r0, #7
	asrs	r0, r0, #16
	mov	r2, r8
	subs	r0, r0, r2
	subs	r0, #64
	b.n	.L_0818bd66
.L_0818bd56:
	lsls	r0, r6, #10
	bl	sub_08002096
	lsls	r3, r0, #6
	subs	r3, r3, r0
	asrs	r3, r3, #16
	mov	r1, r8
	subs	r0, r3, r1
.L_0818bd66:
	cmp	r0, #63
	ble.n	.L_0818bd6c
	movs	r0, #63
.L_0818bd6c:
	cmp	r0, #0
	bge.n	.L_0818bd72
	movs	r0, #0
.L_0818bd72:
	movs	r3, #0
.L_0818bd74:
	adds	r3, #1
	strb	r0, [r5, #0]
	adds	r5, #1
	cmp	r3, #32
	bne.n	.L_0818bd74
	adds	r6, #1
	cmp	r6, #32
	bne.n	.L_0818bd3c
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r7, #1
	add	sl, r2
	cmp	r7, #16
	bne.n	.L_0818bd30
	b.n	.L_0818bdbe
.L_0818bd92:
	ldr	r3, [sp, #64]
	cmp	r3, #3
	bne.n	.L_0818bdac
	ldr	r5, [sp, #60]
	movs	r6, #224
	lsls	r6, r6, #3
	ldr	r0, [pc, #96]
	adds	r1, r5, r6
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	b.n	.L_0818bdbe
.L_0818bdac:
	ldr	r2, [sp, #60]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #76]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
.L_0818bdbe:
	movs	r3, #128
	ldr	r2, [pc, #24]
	lsls	r3, r3, #19
	adds	r3, #82
	mov	r5, fp
	strh	r2, [r3, #0]
	ldr	r3, [r5, #4]
	cmp	r3, #0
	bne.n	.L_0818be08
	movs	r0, #104
	movs	r1, #23
	bl	sub_081963ec
	b.n	.L_0818be10
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x00000134
	.4byte 0x00000130
	.4byte 0x03000730
	.4byte 0x00000148
	.4byte 0x000000c2
	.4byte 0x02010000
	.4byte 0x000000c1
	.4byte 0x02014000
	.4byte 0x000000c9
	.2byte 0x00f6
	.2byte 0x0000
.L_0818be08:
	movs	r0, #104
	movs	r1, #19
	bl	sub_081963ec
.L_0818be10:
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r3, [r5, #104]
	movs	r1, #3
	movs	r0, #188
	str	r3, [sp, #76]
	bl	sub_081963ec
	adds	r5, #188
	ldr	r3, [r5, #0]
	mov	r6, sp
	adds	r6, #76
	str	r6, [sp, #20]
	str	r3, [r6, #4]
	ldr	r0, [sp, #60]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r0, r1
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r0, r3
	movs	r1, #200
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r0, [pc, #700]
	lsls	r1, r1, #4
	bl	sub_080145a8
	ldr	r5, [sp, #64]
	cmp	r5, #2
	bne.n	.L_0818be64
	mov	r6, fp
	movs	r2, #36
	ldrsh	r1, [r6, r2]
	ldr	r0, [r6, #8]
	movs	r2, #10
	movs	r3, #80
	bl	sub_08157530
.L_0818be64:
	mov	r5, sp
	mov	r3, fp
	adds	r5, #96
	ldr	r0, [r3, #8]
	adds	r1, r5, #0
	str	r5, [sp, #44]
	bl	sub_0815e21c
	ldr	r6, [sp, #64]
	cmp	r6, #3
	bne.n	.L_0818be9a
	mov	r0, fp
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne.n	.L_0818be8a
	ldr	r3, [r5, #0]
	subs	r3, #32
	str	r3, [r5, #0]
	b.n	.L_0818be92
.L_0818be8a:
	ldr	r1, [sp, #44]
	ldr	r3, [r1, #0]
	adds	r3, #32
	str	r3, [r1, #0]
.L_0818be92:
	ldr	r2, [sp, #44]
	ldr	r3, [r2, #4]
	subs	r3, #8
	str	r3, [r2, #4]
.L_0818be9a:
	mov	r6, sp
	adds	r6, #84
	mov	r5, fp
	movs	r3, #36
	ldrsh	r0, [r5, r3]
	adds	r1, r6, #0
	str	r6, [sp, #40]
	bl	sub_0815e21c
	movs	r0, #0
	str	r0, [sp, #52]
	ldr	r3, [pc, #596]
	ldr	r1, [sp, #64]
	ldrb	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_0818bebe
	bl	.L_0818c754
.L_0818bebe:
	ldr	r3, [sp, #60]
	ldr	r2, [sp, #44]
	movs	r5, #142
	lsls	r5, r5, #7
	adds	r5, r3, r5
	str	r2, [sp, #36]
	str	r5, [sp, #24]
.L_0818becc:
	ldr	r6, [sp, #52]
	cmp	r6, #0
	bne.n	.L_0818bf40
	ldr	r0, [sp, #60]
	movs	r1, #224
	lsls	r1, r1, #2
	movs	r7, #0
	adds	r5, r0, r1
.L_0818bedc:
	negs	r3, r7
	str	r3, [r5, #24]
	bl	sub_08014878
	str	r0, [r5, #0]
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	adds	r3, #32
	adds	r7, #1
	str	r3, [r5, #4]
	adds	r5, #28
	cmp	r7, #32
	bne.n	.L_0818bedc
	ldr	r2, [sp, #36]
	ldr	r5, [sp, #60]
	ldr	r3, [r2, #0]
	movs	r0, #0
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldr	r6, [sp, #36]
	ldr	r3, [r6, #4]
	str	r0, [r5, #12]
	subs	r3, #32
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	str	r0, [r5, #16]
	ldr	r1, [sp, #40]
	ldr	r2, [r1, #0]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r6, #0]
	asrs	r2, r2, #1
	lsrs	r1, r3, #31
	adds	r3, r3, r1
	asrs	r3, r3, #1
	subs	r2, r2, r3
	lsls	r2, r2, #10
	str	r2, [r5, #8]
	ldr	r2, [sp, #40]
	ldr	r3, [r2, #4]
	ldr	r2, [r6, #4]
	str	r0, [r5, #24]
	subs	r3, r3, r2
	lsls	r3, r3, #10
	str	r3, [r5, #20]
.L_0818bf40:
	ldr	r5, [sp, #44]
	mov	r6, fp
	ldr	r3, [r5, #4]
	subs	r3, #32
	str	r3, [sp, #28]
	ldr	r3, [r6, #4]
	cmp	r3, #0
	bne.n	.L_0818bf5c
	ldr	r3, [r5, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	subs	r3, #8
	b.n	.L_0818bf68
.L_0818bf5c:
	ldr	r0, [sp, #44]
	ldr	r3, [r0, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r3, #8
.L_0818bf68:
	str	r3, [sp, #32]
	ldr	r1, [sp, #64]
	cmp	r1, #0
	bne.n	sub_0818bfd8
	ldr	r1, [sp, #52]
	subs	r1, #16
	cmp	r1, #63
	bhi.n	sub_0818bfd8
	ldr	r2, [sp, #52]
	adds	r5, r1, #0
	cmp	r2, #63
	ble.n	.L_0818bf92
	mov	r3, fp
	ldr	r2, [pc, #392]
	ldr	r1, [r3, #24]
	ldr	r5, [sp, #52]
	ldrb	r3, [r2, r1]
	subs	r3, r3, r5
	adds	r5, r3, #0
	adds	r5, #64
	b.n	.L_0818bf98
.L_0818bf92:
	mov	r6, fp
	ldr	r1, [r6, #24]
	ldr	r2, [pc, #372]
.L_0818bf98:
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	lsls	r5, r3, #1
	ldrb	r3, [r2, r1]
	cmp	r5, r3
	ble.n	.L_0818bfa8
	adds	r5, r3, #0
.L_0818bfa8:
	cmp	r5, #0
	ble.n	sub_0818bfd8
	movs	r2, #128
	adds	r1, r5, #0
	ldr	r0, [sp, #24]
	lsls	r2, r2, #9
	bl	sub_0815b510
	ldr	r1, [sp, #28]
	lsrs	r2, r5, #31
	subs	r3, r1, r5
	lsls	r1, r5, #1
	adds	r2, r5, r2
	ldr	r0, [sp, #32]
	str	r5, [sp, #0]
	str	r1, [sp, #4]
	ldr	r5, [sp, #20]
	asrs	r2, r2, #1
	subs	r2, r0, r2
	ldr	r4, [r5, #4]
	ldr	r0, [sp, #56]
	ldr	r1, [sp, #24]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9e10
	cmp	r6, #3
	bne.n	sub_0818c01c
	ldr	r0, [sp, #52]
	cmp	r0, #47
	bhi.n	sub_0818c01c
	movs	r6, #40
	cmp	r0, #39
	ble.n	.L_0818bff0
	lsls	r2, r0, #1
	movs	r3, #104
	subs	r6, r3, r2
.L_0818bff0:
	asrs	r5, r6, #1
	lsls	r6, r5, #1
	movs	r2, #128
	ldr	r0, [sp, #24]
	adds	r1, r6, #0
	lsls	r2, r2, #9
	bl	sub_0815b510
	ldr	r1, [sp, #32]
	asrs	r2, r6, #1
	lsls	r5, r5, #2
	ldr	r0, [sp, #28]
	subs	r2, r1, r2
	str	r6, [sp, #0]
	ldr	r1, [sp, #20]
	str	r5, [sp, #4]
	subs	r3, r0, r6
	ldr	r4, [r1, #4]
	ldr	r0, [sp, #56]
	ldr	r1, [sp, #24]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4b3c
	ldr	r2, [sp, #64]
	ldr	r5, [sp, #60]
	ldrb	r3, [r3, r2]
	ldr	r1, [sp, #52]
	movs	r0, #224
	lsls	r0, r0, #2
	movs	r7, #0
	adds	r6, r5, r0
	cmp	r1, r3
	blt.n	.L_0818c118
	cmp	r1, r3
	bne.n	.L_0818c08a
	ldr	r2, [sp, #32]
	ldr	r3, [sp, #28]
	lsls	r2, r2, #16
	lsls	r3, r3, #16
	mov	sl, r2
	mov	r8, r3
	movs	r6, #63
	adds	r5, r5, r0
.L_0818c046:
	mov	r0, sl
	mov	r1, r8
	str	r0, [r5, #0]
	str	r1, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r5, #16]
	mov	r2, fp
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_0818c06c
	bl	sub_08014878
	ands	r0, r6
	negs	r0, r0
	b.n	.L_0818c072
.L_0818c06c:
	bl	sub_08014878
	ands	r0, r6
.L_0818c072:
	lsls	r0, r0, #13
	str	r0, [r5, #12]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #16
	adds	r7, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #32
	bne.n	.L_0818c046
.L_0818c08a:
	ldr	r3, [sp, #60]
	movs	r5, #224
	lsls	r5, r5, #2
	adds	r3, r3, r5
	movs	r7, #0
	mov	r8, r3
.L_0818c096:
	mov	r6, r8
	ldr	r3, [r6, #24]
	cmp	r3, #0
	blt.n	.L_0818c0f8
	mov	r0, fp
	ldr	r3, [r0, #24]
	movs	r2, #3
	ands	r2, r7
	adds	r5, r2, #1
	cmp	r3, #1
	bne.n	.L_0818c0b0
	adds	r5, #1
	b.n	.L_0818c0b6
.L_0818c0b0:
	cmp	r3, #2
	bne.n	.L_0818c0b6
	adds	r5, r2, #3
.L_0818c0b6:
	ldr	r2, [pc, #92]
	lsls	r4, r5, #1
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #48]
	movs	r0, #1
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	mov	lr, r2
	movs	r3, #6
	ldrsh	r2, [r6, r3]
	str	r5, [sp, #0]
	subs	r3, r2, r5
	str	r4, [sp, #4]
	ldr	r5, [sp, #20]
	ands	r0, r7
	lsls	r0, r0, #2
	ldr	r4, [r0, r5]
	mov	r2, lr
	ldr	r0, [sp, #56]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c30
	movs	r1, #60
	movs	r2, #0
	bl	sub_08138086
	ldr	r3, [r6, #24]
	subs	r3, #1
	str	r3, [r6, #24]
.L_0818c0f8:
	movs	r6, #28
	adds	r7, #1
	add	r8, r6
	cmp	r7, #32
	bne.n	.L_0818c096
	b.n	.L_0818c188
	.4byte 0x08143001
	.4byte 0x08199d97
	.4byte 0x08199d9f
	.4byte 0x08199d9b
	.2byte 0x7410
	.2byte 0x0819
.L_0818c118:
	ldr	r3, [r6, #24]
	adds	r3, #1
	str	r3, [r6, #24]
	cmp	r3, #0
	blt.n	sub_0818c180
	ldr	r0, [r6, #0]
	bl	sub_08002096
	ldr	r3, [r6, #4]
	muls	r3, r0
	ldr	r0, [sp, #32]
	asrs	r3, r3, #16
	adds	r0, r0, r3
	mov	r8, r0
	ldr	r0, [r6, #0]
	bl	sub_08002090
	ldr	r2, [r6, #4]
	ldr	r1, [sp, #28]
	adds	r3, r2, #0
	muls	r3, r0
	asrs	r3, r3, #16
	adds	r1, r1, r3
	movs	r3, #3
	ands	r3, r7
	mov	ip, r1
	adds	r5, r3, #3
	cmp	r2, #1
	ble.n	sub_0818c180
	subs	r3, r2, #2
	ldr	r2, [pc, #836]
	lsls	r4, r5, #1
	str	r3, [r6, #4]
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #48]
	mov	r3, r8
	adds	r1, r2, r1
	lsrs	r2, r5, #1
	subs	r2, r3, r2
	mov	r3, ip
	str	r5, [sp, #0]
	movs	r0, #1
	subs	r3, r3, r5
	str	r4, [sp, #4]
	ldr	r5, [sp, #20]
	ands	r0, r7
	lsls	r0, r0, #2
	ldr	r4, [r0, r5]
	ldr	r0, [sp, #56]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x3701
	adds	r6, #28
	cmp	r7, #32
	bne.n	.L_0818c118
.L_0818c188:
	ldr	r6, [sp, #64]
	cmp	r6, #0
	bne.n	.L_0818c20a
	ldr	r0, [sp, #52]
	cmp	r0, #55
	ble.n	.L_0818c262
	ldr	r1, [sp, #36]
	ldr	r3, [r1, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r5, r3, #1
	adds	r3, r0, #0
	subs	r3, #72
	ldr	r2, [r1, #4]
	cmp	r3, #0
	bge.n	.L_0818c1aa
	movs	r3, #0
.L_0818c1aa:
	cmp	r3, #15
	ble.n	.L_0818c1b0
	movs	r3, #15
.L_0818c1b0:
	subs	r2, #48
	mov	r8, r2
	ldr	r2, [sp, #60]
	lsls	r3, r3, #10
	adds	r6, r2, r3
	movs	r3, #32
	movs	r7, #0
	mov	sl, r3
.L_0818c1c0:
	mov	r0, fp
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne.n	.L_0818c1e6
	movs	r3, #224
	mov	r1, sl
	adds	r2, r5, #0
	lsls	r3, r3, #3
	str	r1, [sp, #0]
	str	r1, [sp, #4]
	subs	r2, #40
	adds	r1, r6, r3
	ldr	r4, [sp, #76]
	ldr	r0, [sp, #56]
	mov	r3, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x3d20
	b.n	.L_0818c202
.L_0818c1e6:
	movs	r3, #224
	mov	r0, sl
	adds	r2, r5, #0
	lsls	r3, r3, #3
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	adds	r1, r6, r3
	adds	r2, #8
	ldr	r4, [sp, #76]
	ldr	r0, [sp, #56]
	mov	r3, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x3520
.L_0818c202:
	adds	r7, #1
	cmp	r7, #4
	bne.n	.L_0818c1c0
	b.n	.L_0818c262
.L_0818c20a:
	ldr	r5, [sp, #64]
	cmp	r5, #1
	bne.n	.L_0818c262
	ldr	r3, [pc, #652]
	ldr	r6, [sp, #52]
	ldrb	r3, [r3, r5]
	cmp	r6, r3
	bge.n	.L_0818c21c
	b.n	.L_0818c5d4
.L_0818c21c:
	ldr	r1, [sp, #60]
	ldr	r6, [sp, #60]
	movs	r0, #2
	ldrsh	r2, [r1, r0]
	movs	r5, #6
	ldrsh	r3, [r1, r5]
	movs	r1, #32
	movs	r5, #224
	str	r1, [sp, #0]
	lsls	r5, r5, #3
	movs	r1, #48
	subs	r3, #24
	str	r1, [sp, #4]
	subs	r2, #16
	ldr	r4, [sp, #76]
	ldr	r0, [sp, #56]
	adds	r1, r6, r5
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2200
	ldr	r0, [sp, #60]
	movs	r1, #56
	bl	sub_08138086
	ldr	r3, [r6, #12]
	ldr	r2, [r6, #8]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r2, [r6, #20]
	ldr	r3, [r6, #16]
	adds	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #24]
	adds	r3, #1
	str	r3, [r6, #24]
.L_0818c262:
	ldr	r5, [pc, #572]
	ldr	r6, [sp, #64]
	ldr	r0, [sp, #52]
	ldrb	r3, [r5, r6]
	cmp	r0, r3
	bge.n	.L_0818c270
	b.n	.L_0818c5d4
.L_0818c270:
	movs	r0, #128
	lsls	r0, r0, #3
	bl	sub_08014dac
	str	r0, [sp, #16]
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #544]
	ldr	r3, [sp, #68]
	mov	r1, sp
	ands	r3, r2
	movs	r2, #7
	orrs	r3, r2
	ldr	r2, [pc, #536]
	adds	r1, #68
	ands	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #68]
	movs	r3, #6
	str	r3, [r0, #0]
	ldr	r3, [pc, #524]
	str	r1, [sp, #12]
	str	r3, [r0, #8]
	str	r1, [r0, #16]
	ldr	r2, [sp, #16]
	ldr	r3, [pc, #516]
	mov	r8, r0
	str	r2, [r0, #12]
	str	r3, [r1, #4]
	cmp	r6, #1
	bne.n	.L_0818c2b6
	b.n	.L_0818c3d4
.L_0818c2b6:
	ldrb	r2, [r5, r6]
	ldr	r5, [sp, #52]
	movs	r3, #156
	lsls	r3, r3, #6
	subs	r2, r5, r2
	adds	r3, #208
	muls	r3, r2
	movs	r6, #128
	lsls	r6, r6, #8
	lsls	r2, r2, #3
	adds	r5, r3, r6
	movs	r3, #128
	subs	r0, r3, r2
	mov	r2, fp
	ldr	r1, [r2, #24]
	cmp	r1, #0
	bne.n	.L_0818c2dc
	asrs	r5, r5, #1
	b.n	.L_0818c2ea
.L_0818c2dc:
	cmp	r1, #1
	bne.n	.L_0818c2ea
	lsls	r3, r5, #1
	adds	r3, r3, r5
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r5, r3, #1
.L_0818c2ea:
	cmp	r0, #0
	ble.n	.L_0818c2f0
	movs	r0, #0
.L_0818c2f0:
	movs	r3, #64
	negs	r3, r3
	cmp	r0, r3
	ble.n	.L_0818c3de
	ldr	r6, [sp, #64]
	movs	r3, #3
	eors	r3, r6
	negs	r2, r3
	orrs	r2, r3
	lsrs	r2, r2, #31
	mov	r9, r2
	mov	r1, r9
	movs	r3, #2
	subs	r1, r3, r1
	mov	r2, r8
	mov	r9, r1
	str	r0, [r2, #20]
	movs	r7, #0
	cmp	r1, #0
	beq.n	.L_0818c3da
	ldr	r0, [sp, #32]
	ldr	r3, [sp, #28]
	subs	r0, #64
	str	r0, [sp, #8]
	subs	r3, #64
	asrs	r6, r5, #31
	mov	sl, r3
.L_0818c326:
	bl	sub_08014de4
	ldr	r1, [sp, #64]
	cmp	r1, #3
	bne.n	.L_0818c360
	ldr	r2, [sp, #8]
	mov	r3, sl
	lsls	r1, r3, #16
	lsls	r0, r2, #16
	movs	r2, #0
	bl	sub_08015160
	movs	r2, #128
	lsls	r2, r2, #9
	movs	r1, #128
	adds	r0, r2, #0
	lsls	r1, r1, #10
	bl	sub_080151e4
	movs	r1, #128
	lsls	r1, r1, #6
	lsls	r0, r7, #14
	adds	r0, r0, r1
	bl	sub_080150e4
	ldr	r0, [pc, #344]
	bl	sub_08015024
	b.n	.L_0818c39c
.L_0818c360:
	mov	r2, fp
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_0818c37a
	ldr	r0, [sp, #32]
	mov	r3, sl
	subs	r0, #72
	lsls	r0, r0, #16
	lsls	r1, r3, #16
	movs	r2, #0
	bl	sub_08015160
	b.n	.L_0818c38a
.L_0818c37a:
	ldr	r0, [sp, #32]
	mov	r2, sl
	subs	r0, #56
	lsls	r1, r2, #16
	lsls	r0, r0, #16
	movs	r2, #0
	bl	sub_08015160
.L_0818c38a:
	movs	r0, #200
	lsls	r0, r0, #6
	adds	r0, #200
	bl	sub_08015068
	movs	r0, #128
	lsls	r0, r0, #7
	bl	sub_08015024
.L_0818c39c:
	ldr	r3, [sp, #64]
	cmp	r3, #2
	bne.n	.L_0818c3aa
	lsrs	r3, r6, #31
	adds	r3, r5, r3
	asrs	r5, r3, #1
	asrs	r6, r3, #31
.L_0818c3aa:
	lsrs	r2, r6, #31
	adds	r2, r5, r2
	asrs	r2, r2, #1
	adds	r0, r2, #0
	adds	r1, r5, #0
	bl	sub_080151e4
	ldr	r0, [pc, #252]
	ldr	r1, [sp, #16]
	movs	r2, #4
	bl	sub_08196958
	adds	r7, #1
	mov	r0, r8
	bl	sub_08196a7c
	cmp	r7, r9
	bne.n	.L_0818c326
	mov	r5, fp
	ldr	r1, [r5, #24]
	b.n	.L_0818c3de
.L_0818c3d4:
	mov	r6, fp
	ldr	r1, [r6, #24]
	b.n	.L_0818c3de
.L_0818c3da:
	mov	r0, fp
	ldr	r1, [r0, #24]
.L_0818c3de:
	cmp	r1, #2
	beq.n	.L_0818c3e4
	b.n	.L_0818c528
.L_0818c3e4:
	ldr	r1, [sp, #12]
	ldr	r3, [pc, #212]
	mov	r2, r8
	str	r3, [r1, #4]
	movs	r3, #7
	str	r3, [r2, #0]
	ldr	r3, [pc, #204]
	movs	r5, #8
	str	r3, [r2, #8]
	ldr	r3, [sp, #28]
	ldr	r0, [sp, #64]
	subs	r3, #64
	movs	r7, #0
	mov	sl, r3
	mov	r9, r5
	lsls	r6, r0, #2
.L_0818c404:
	ldr	r3, [pc, #188]
	ldr	r2, [sp, #52]
	ldrb	r1, [r3, r6]
	cmp	r2, r1
	bgt.n	.L_0818c410
	b.n	.L_0818c51a
.L_0818c410:
	ldr	r3, [pc, #180]
	subs	r2, r2, r1
	ldrb	r3, [r3, r6]
	movs	r0, #131
	muls	r2, r3
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #220
	muls	r3, r2
	ldr	r2, [sp, #52]
	lsls	r0, r0, #7
	adds	r5, r3, r0
	subs	r3, r1, r2
	lsls	r3, r3, #3
	adds	r3, #56
	cmp	r3, #0
	ble.n	.L_0818c434
	movs	r3, #0
.L_0818c434:
	movs	r0, #64
	negs	r0, r0
	cmp	r3, r0
	ble.n	.L_0818c51a
	mov	r1, r8
	str	r3, [r1, #20]
	bl	sub_08014de4
	ldr	r2, [sp, #64]
	cmp	r2, #2
	bne.n	.L_0818c47c
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	mov	r0, fp
	asrs	r5, r3, #1
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne.n	.L_0818c46c
	ldr	r2, [pc, #112]
	mov	r1, r9
	ldrsb	r0, [r1, r2]
	ldr	r3, [sp, #32]
	mov	r2, sl
	adds	r0, r3, r0
	subs	r0, #48
	lsls	r1, r2, #16
	lsls	r0, r0, #16
	b.n	.L_0818c494
.L_0818c46c:
	ldr	r1, [pc, #92]
	mov	r3, r9
	ldrsb	r0, [r3, r1]
	ldr	r2, [sp, #32]
	mov	r3, sl
	subs	r0, r2, r0
	subs	r0, #80
	b.n	.L_0818c490
.L_0818c47c:
	mov	r0, fp
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne.n	.L_0818c4d0
	ldr	r1, [pc, #68]
	ldr	r2, [sp, #32]
	ldrsb	r0, [r6, r1]
	mov	r3, sl
	adds	r0, r2, r0
	subs	r0, #64
.L_0818c490:
	lsls	r0, r0, #16
	lsls	r1, r3, #16
.L_0818c494:
	movs	r2, #0
	bl	sub_08015160
	b.n	.L_0818c4e6
	.4byte 0x08197410
	.4byte 0x08199d9b
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199364
	.4byte 0x02010000
	.4byte 0xfffff000
	.4byte 0x08199210
	.4byte 0x02014000
	.4byte 0x08198ec4
	.4byte 0x08199da3
	.4byte 0x08199daf
	.2byte 0x9dbb
	.2byte 0x0819
.L_0818c4d0:
	ldr	r1, [pc, #676]
	ldr	r2, [sp, #32]
	ldrsb	r0, [r6, r1]
	mov	r3, sl
	subs	r0, r2, r0
	subs	r0, #64
	lsls	r0, r0, #16
	lsls	r1, r3, #16
	movs	r2, #0
	bl	sub_08015160
.L_0818c4e6:
	adds	r1, r5, #0
	adds	r2, r5, #0
	adds	r0, r5, #0
	bl	sub_080151e4
	movs	r0, #216
	lsls	r0, r0, #6
	adds	r0, #176
	bl	sub_08015068
	movs	r0, #128
	lsls	r0, r0, #7
	bl	sub_08015024
	ldr	r5, [sp, #52]
	lsls	r0, r5, #10
	bl	sub_08015068
	ldr	r0, [pc, #624]
	ldr	r1, [sp, #16]
	movs	r2, #32
	bl	sub_08196958
	mov	r0, r8
	bl	sub_08196a7c
.L_0818c51a:
	movs	r0, #1
	adds	r7, #1
	add	r9, r0
	adds	r6, #1
	cmp	r7, #4
	beq.n	.L_0818c528
	b.n	.L_0818c404
.L_0818c528:
	ldr	r1, [sp, #64]
	cmp	r1, #3
	bne.n	.L_0818c5c8
	ldr	r2, [sp, #52]
	movs	r6, #192
	lsls	r6, r6, #6
	movs	r3, #0
	adds	r6, #96
	cmp	r2, #31
	ble.n	.L_0818c542
	movs	r3, #32
	subs	r3, r3, r2
	lsls	r3, r3, #2
.L_0818c542:
	mov	r5, r8
	str	r3, [r5, #20]
	ldr	r0, [sp, #12]
	movs	r3, #5
	strb	r3, [r0, #0]
	add	r2, sp, #68
	movs	r3, #2
	strb	r3, [r2, #1]
	movs	r3, #7
	str	r3, [r5, #0]
	ldr	r3, [pc, #552]
	str	r3, [r5, #8]
	ldr	r1, [sp, #60]
	movs	r5, #224
	lsls	r5, r5, #3
	adds	r3, r1, r5
	str	r3, [r2, #4]
	bl	sub_08014de4
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #28]
	subs	r0, #64
	subs	r1, #64
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	movs	r2, #0
	movs	r5, #128
	bl	sub_08015160
	lsls	r5, r5, #9
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r1, r5, #0
	adds	r2, r5, #0
	bl	sub_080151e4
	ldr	r0, [sp, #52]
	cmp	r0, #7
	ble.n	.L_0818c59a
	movs	r1, #176
	lsls	r1, r1, #6
	lsls	r3, r0, #7
	adds	r1, #96
	adds	r6, r3, r1
.L_0818c59a:
	mov	r2, fp
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L_0818c5a4
	negs	r6, r6
.L_0818c5a4:
	adds	r0, r6, #0
	bl	sub_080150e4
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #10
	adds	r2, r5, #0
	bl	sub_080151e4
	ldr	r0, [pc, #456]
	ldr	r1, [sp, #16]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, r8
	bl	sub_08196a7c
.L_0818c5c8:
	mov	r0, r8
	bl	sub_08013164
	ldr	r0, [sp, #16]
	bl	sub_08013164
.L_0818c5d4:
	ldr	r3, [sp, #64]
	cmp	r3, #0
	bne.n	.L_0818c5e8
	ldr	r5, [sp, #52]
	cmp	r5, #0
	bne.n	.L_0818c5fa
	movs	r0, #140
	bl	sub_081c0010
	b.n	.L_0818c5fa
.L_0818c5e8:
	ldr	r6, [sp, #64]
	cmp	r6, #2
	bne.n	.L_0818c5fa
	ldr	r0, [sp, #52]
	cmp	r0, #2
	bne.n	.L_0818c5fa
	movs	r0, #212
	bl	sub_081c0010
.L_0818c5fa:
	ldr	r1, [sp, #64]
	cmp	r1, #3
	bne.n	.L_0818c6be
	ldr	r2, [sp, #52]
	cmp	r2, #0
	bne.n	.L_0818c61a
	ldr	r3, [sp, #60]
	movs	r5, #238
	lsls	r5, r5, #7
	adds	r5, #168
	adds	r2, r3, r5
	movs	r3, #8
	str	r3, [r2, #0]
	movs	r0, #221
	bl	sub_081c0010
.L_0818c61a:
	ldr	r6, [sp, #52]
	cmp	r6, #16
	bne.n	.L_0818c62c
	movs	r0, #134
	bl	sub_081c0010
	movs	r0, #134
	bl	sub_081180e8
.L_0818c62c:
	mov	r1, fp
	ldr	r0, [r1, #20]
	movs	r7, #0
	cmp	r0, #0
	beq.n	sub_0818c720
	movs	r5, #36
.L_0818c638:
	ldr	r2, [pc, #332]
	ldr	r6, [sp, #64]
	ldr	r1, [sp, #52]
	ldrb	r2, [r2, r6]
	lsls	r3, r7, #2
	adds	r3, r3, r2
	cmp	r1, r3
	blt.n	.L_0818c6b4
	ldr	r3, [pc, #320]
	ldrb	r3, [r3, r6]
	subs	r3, #8
	cmp	r1, r3
	bge.n	.L_0818c6b4
	ldr	r3, [sp, #52]
	movs	r1, #3
	ands	r3, r1
	cmp	r3, r7
	bne.n	.L_0818c6b4
	ldr	r3, [sp, #60]
	movs	r6, #238
	lsls	r6, r6, #7
	adds	r6, #168
	adds	r2, r3, r6
	movs	r3, #8
	str	r3, [r2, #0]
	ldr	r0, [sp, #64]
	cmp	r0, #6
	bne.n	.L_0818c682
	mov	r2, fp
	ldrsh	r0, [r5, r2]
	str	r1, [sp, #0]
	movs	r2, #5
	movs	r1, #14
	adds	r3, r7, #0
	bl	sub_0814cd48
	b.n	.L_0818c692
.L_0818c682:
	mov	r6, fp
	ldrsh	r0, [r5, r6]
	adds	r3, r7, #0
	str	r1, [sp, #0]
	movs	r2, #5
	movs	r1, #7
	bl	sub_0814cd48
.L_0818c692:
	mov	r3, fp
	ldrsh	r0, [r5, r3]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [sp, #0]
	ldr	r3, [sp, #52]
	movs	r2, #128
	adds	r3, #100
	str	r3, [sp, #4]
	movs	r3, #128
	movs	r1, #1
	lsls	r2, r2, #9
	lsls	r3, r3, #11
	bl	sub_0815f000
	mov	r1, fp
	ldr	r0, [r1, #20]
.L_0818c6b4:
	adds	r7, #1
	adds	r5, #2
	cmp	r7, r0
	bne.n	.L_0818c638
	b.n	sub_0818c720
.L_0818c6be:
	ldr	r3, [pc, #208]
	ldr	r2, [sp, #64]
	ldr	r5, [sp, #52]
	ldrb	r3, [r3, r2]
	cmp	r5, r3
	bne.n	sub_0818c720
	mov	r6, fp
	ldr	r3, [r6, #20]
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L_0818c6fe
	movs	r5, #36
.L_0818c6d6:
	mov	r1, fp
	ldrsh	r0, [r5, r1]
	movs	r1, #4
	bl	sub_08118088
	mov	r3, fp
	ldrsh	r0, [r5, r3]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r1, #7
	adds	r3, r7, #0
	movs	r2, #5
	bl	sub_0814cd48
	mov	r0, fp
	ldr	r3, [r0, #20]
	adds	r7, #1
	adds	r5, #2
	cmp	r7, r3
	bne.n	.L_0818c6d6
.L_0818c6fe:
	ldr	r1, [sp, #60]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	adds	r3, r1, r2
	movs	r2, #8
	str	r2, [r3, #0]
	movs	r0, #144
	bl	sub_081180e8
	movs	r1, #128
	ldr	r3, [pc, #124]
	ldr	r0, [sp, #56]
	lsls	r1, r1, #7
	ldr	r2, [pc, #124]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2008
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r5, #240
	ldr	r3, [sp, #60]
	lsls	r5, r5, #7
	adds	r5, #232
	adds	r2, r3, r5
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r6, [sp, #52]
	ldr	r3, [pc, #72]
	adds	r6, #1
	str	r6, [sp, #52]
	ldr	r0, [sp, #64]
	ldrb	r3, [r3, r0]
	cmp	r6, r3
	beq.n	.L_0818c754
	bl	.L_0818becc
.L_0818c754:
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #56]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #108
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x08199dbb
	.4byte 0x08198cac
	.4byte 0x08199220
	.4byte 0x081991c0
	.4byte 0x08199d9b
	.4byte 0x08199d97
	.4byte 0x08199dc7
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r1, #0
	bl	sub_0818c7b8
	pop	{pc}
