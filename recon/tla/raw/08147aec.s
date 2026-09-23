.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014de4, 0x08014de4
	.set sub_080156e8, 0x080156e8
	.set sub_08020030, 0x08020030
	.set sub_08020048, 0x08020048
	.set sub_08118070, 0x08118070
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138086, 0x08138086
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_08147b92, 0x08147b92
	.set sub_08148fae, 0x08148fae
	.set sub_08149830, 0x08149830
	.set sub_081498fc, 0x081498fc
	.set sub_08149946, 0x08149946
	.set sub_08149982, 0x08149982
	.set sub_08149b00, 0x08149b00
	.set sub_08149bac, 0x08149bac
	.set sub_0814cc4c, 0x0814cc4c
	.set sub_0814cd48, 0x0814cd48
	.set sub_08152404, 0x08152404
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e20c, 0x0815e20c
	.set sub_0816442c, 0x0816442c
	.set sub_0816467c, 0x0816467c
	.set sub_08164a4c, 0x08164a4c
	.set sub_081963ec, 0x081963ec
	.set sub_081c0010, 0x081c0010
	.global Func_08147aec
	.thumb_func
Func_08147aec:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #88
	str	r0, [sp, #64]
	str	r1, [sp, #60]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #96]
	str	r0, [sp, #56]
	movs	r0, #0
	ldr	r1, [r3, #48]
	str	r1, [sp, #52]
	ldr	r2, [r3, #92]
	ldr	r3, [r3, #100]
	mov	sl, r2
	str	r3, [sp, #44]
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	subs	r2, #70
	strh	r3, [r2, #0]
	mov	r3, sp
	adds	r3, #68
	adds	r1, r3, #0
	movs	r0, #0
	str	r3, [sp, #40]
	bl	sub_08144aac
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #32]
	add	r1, sl
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r1, #220
	lsls	r1, r1, #6
	ldr	r0, [pc, #20]
	add	r1, sl
	movs	r2, #1
	movs	r3, #0
	b.n	.L_08147b68
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x00000784
	.4byte 0x0000013e
	.2byte 0x0178
	.2byte 0x0000
.L_08147b68:
	bl	sub_08157cf4
	ldr	r0, [pc, #600]
	ldr	r1, [sp, #44]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r6, [sp, #60]
	cmp	r6, #1
	bne.n	sub_08147b92
	ldr	r0, [pc, #588]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #580]
	lsls	r0, r0, #19
	movs	r2, #128
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
	movs	r3, #75
	add	r2, sl
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #548]
	bl	sub_080145a8
	movs	r0, #0
	str	r0, [sp, #24]
	ldr	r2, [sp, #64]
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	bl	sub_08118098
	ldr	r3, [r0, #0]
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #2
	bne.n	.L_08147bce
	movs	r3, #1
	str	r3, [sp, #24]
.L_08147bce:
	ldr	r0, [sp, #60]
	movs	r6, #1
	str	r6, [sp, #28]
	cmp	r0, #0
	beq.n	.L_08147bea
	ldr	r1, [sp, #64]
	movs	r2, #1
	ldr	r3, [r1, #4]
	negs	r2, r2
	str	r2, [sp, #28]
	cmp	r3, #1
	beq.n	.L_08147bea
	movs	r3, #1
	str	r3, [sp, #28]
.L_08147bea:
	ldr	r6, [sp, #60]
	cmp	r6, #1
	bne.n	.L_08147c1a
	ldr	r1, [sp, #64]
	add	r5, sp, #76
	ldr	r0, [r1, #8]
	adds	r1, r5, #0
	bl	sub_0815e20c
	ldr	r3, [r5, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	movs	r3, #66
	str	r3, [r5, #4]
	ldr	r2, [sp, #64]
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L_08147c16
	movs	r3, #76
	b.n	.L_08147c18
.L_08147c16:
	movs	r3, #44
.L_08147c18:
	str	r3, [r5, #0]
.L_08147c1a:
	ldr	r3, [sp, #24]
	cmp	r3, #1
	bne.n	.L_08147c2c
	ldr	r0, [pc, #436]
	movs	r6, #220
	lsls	r6, r6, #16
	str	r6, [sp, #32]
	str	r0, [sp, #36]
	b.n	.L_08147c36
.L_08147c2c:
	ldr	r2, [pc, #428]
	movs	r1, #212
	lsls	r1, r1, #16
	str	r1, [sp, #32]
	str	r2, [sp, #36]
.L_08147c36:
	movs	r2, #1
	mov	r3, sl
	movs	r7, #0
	negs	r2, r2
	adds	r3, #24
.L_08147c40:
	adds	r7, #1
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r7, #64
	bne.n	.L_08147c40
	movs	r5, #168
	lsls	r5, r5, #2
	movs	r7, #0
	movs	r6, #127
	add	r5, sl
.L_08147c54:
	ldr	r3, [sp, #28]
	cmp	r3, #1
	bne.n	.L_08147c64
	bl	sub_08014878
	ands	r0, r6
	adds	r0, #128
	b.n	.L_08147c6c
.L_08147c64:
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #128
.L_08147c6c:
	str	r0, [r5, #0]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	subs	r3, #72
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	negs	r3, r3
	adds	r7, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #16
	bne.n	.L_08147c54
	ldr	r3, [pc, #336]
	movs	r1, #1
	movs	r2, #128
	movs	r7, #0
	negs	r1, r1
	lsls	r2, r2, #2
.L_08147c9a:
	adds	r7, #1
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r7, r2
	bne.n	.L_08147c9a
	ldr	r6, [sp, #60]
	cmp	r6, #0
	bne.n	.L_08147ccc
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #240
	add	r3, sl
	ldr	r0, [r3, #0]
	bl	sub_0814cc4c
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #196
	lsls	r1, r1, #1
	adds	r1, #255
	movs	r0, #8
	movs	r2, #2
	bl	sub_08152404
.L_08147ccc:
	ldr	r1, [sp, #52]
	movs	r0, #0
	adds	r1, #12
	str	r0, [sp, #48]
	str	r1, [sp, #12]
.L_08147cd6:
	ldr	r3, [pc, #268]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08147d6a
	ldr	r2, [sp, #48]
	cmp	r2, #48
	ble.n	.L_08147d6a
	cmp	r2, #159
	bgt.n	.L_08147d6a
	ldr	r3, [sp, #60]
	cmp	r3, #0
	bne.n	.L_08147d32
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #220
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #8
	bl	sub_08020030
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #224
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #9
	bl	sub_08020030
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #232
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #10
	bl	sub_08020030
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #236
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #11
	bl	sub_08020030
.L_08147d32:
	ldr	r6, [sp, #64]
	movs	r7, #0
	ldr	r3, [r6, #20]
	cmp	r3, #0
	beq.n	.L_08147d66
	movs	r5, #36
.L_08147d3e:
	ldr	r1, [sp, #64]
	movs	r3, #0
	ldrsh	r0, [r5, r1]
	str	r3, [sp, #0]
	movs	r1, #10
	movs	r2, #5
	subs	r3, #1
	bl	sub_0814cd48
	ldr	r3, [sp, #64]
	movs	r1, #4
	ldrsh	r0, [r5, r3]
	bl	sub_08118088
	ldr	r0, [sp, #64]
	adds	r7, #1
	ldr	r3, [r0, #20]
	adds	r5, #2
	cmp	r7, r3
	bne.n	.L_08147d3e
.L_08147d66:
	movs	r1, #160
	str	r1, [sp, #48]
.L_08147d6a:
	bl	sub_08014de4
	ldr	r0, [sp, #52]
	ldr	r1, [sp, #12]
	bl	sub_080156e8
	ldr	r2, [sp, #48]
	cmp	r2, #178
	bne.n	.L_08147d82
	movs	r0, #134
	bl	sub_081180e8
.L_08147d82:
	ldr	r3, [sp, #48]
	cmp	r3, #128
	bne.n	.L_08147d94
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, sl
	movs	r3, #50
	str	r3, [r2, #0]
.L_08147d94:
	ldr	r6, [sp, #48]
	cmp	r6, #176
	bne.n	.L_08147df0
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #3
	str	r3, [r2, #0]
	movs	r2, #238
	ldr	r3, [pc, #64]
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, sl
	str	r3, [r2, #0]
	ldr	r0, [pc, #56]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #20]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe02d
	movs	r0, r0
	.4byte 0x00000134
	.4byte 0x00000188
	.4byte 0x03000730
	.4byte 0x08143001
	.4byte 0xffcc0000
	.4byte 0xffc40000
	.4byte 0x02010018
	.4byte 0x03001150
	.4byte 0x02020202
	.2byte 0x0178
	.2byte 0x0000
.L_08147df0:
	ldr	r3, [sp, #48]
	subs	r3, #160
	cmp	r3, #15
	bhi.n	.L_08147e22
	movs	r3, #239
	lsls	r3, r3, #7
	add	r3, sl
	movs	r2, #1
	str	r2, [r3, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	ldr	r3, [pc, #864]
	adds	r2, #132
	add	r2, sl
	str	r3, [r2, #0]
	ldr	r0, [sp, #48]
	cmp	r0, #173
	ble.n	.L_08147e18
	ldr	r3, [pc, #852]
	str	r3, [r2, #0]
.L_08147e18:
	movs	r0, #2
	movs	r1, #2
	movs	r2, #2
	bl	sub_08164a4c
.L_08147e22:
	ldr	r3, [sp, #48]
	subs	r3, #33
	cmp	r3, #142
	bhi.n	.L_08147eaa
	ldr	r3, [sp, #48]
	movs	r1, #0
	movs	r2, #1
	mov	r8, r1
	str	r2, [sp, #20]
	cmp	r3, #103
	ble.n	.L_08147e3c
	movs	r6, #8
	str	r6, [sp, #20]
.L_08147e3c:
	ldr	r1, [pc, #816]
	ldr	r6, [pc, #820]
	movs	r0, #127
	movs	r7, #0
	mov	fp, r0
	mov	r9, r1
.L_08147e48:
	ldr	r3, [r6, #24]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_08147e9e
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	subs	r3, #32
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	movs	r3, #224
	lsls	r3, r3, #15
	str	r3, [r6, #4]
	bl	sub_08014878
	movs	r5, #3
	mov	r1, r9
	mov	r3, fp
	ands	r5, r7
	ands	r0, r3
	ldrb	r3, [r1, r5]
	adds	r0, r0, r3
	lsls	r0, r0, #9
	str	r0, [r6, #12]
	bl	sub_08014878
	mov	r1, r9
	ldrb	r3, [r1, r5]
	mov	r2, fp
	ands	r0, r2
	adds	r0, r0, r3
	negs	r0, r0
	movs	r3, #0
	lsls	r0, r0, #11
	str	r0, [r6, #16]
	str	r3, [r6, #24]
	ldr	r3, [sp, #20]
	movs	r2, #1
	add	r8, r2
	cmp	r8, r3
	beq.n	.L_08147eaa
.L_08147e9e:
	movs	r0, #128
	adds	r7, #1
	lsls	r0, r0, #2
	adds	r6, #28
	cmp	r7, r0
	bne.n	.L_08147e48
.L_08147eaa:
	ldr	r3, [sp, #48]
	subs	r3, #41
	cmp	r3, #86
	bhi.n	.L_08147f7c
	ldr	r2, [sp, #48]
	movs	r3, #1
	movs	r1, #0
	ands	r3, r2
	str	r1, [sp, #16]
	cmp	r3, #0
	beq.n	.L_08147f7c
	movs	r5, #140
	movs	r6, #76
	movs	r3, #7
	lsls	r5, r5, #3
	add	r6, sp
	movs	r7, #0
	mov	fp, r3
	add	r5, sl
	mov	r9, r6
.L_08147ed2:
	ldr	r3, [r5, #24]
	movs	r0, #1
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_08147f74
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	adds	r3, #128
	mov	r8, r3
	bl	sub_08014878
	movs	r3, #248
	lsls	r3, r3, #5
	movs	r1, #156
	ldr	r2, [sp, #60]
	adds	r3, #255
	lsls	r1, r1, #7
	ands	r3, r0
	adds	r1, #32
	adds	r6, r3, r1
	cmp	r2, #0
	bne.n	.L_08147f30
	ldr	r3, [sp, #24]
	cmp	r3, #1
	bne.n	.L_08147f1c
	bl	sub_08014878
	mov	r1, fp
	ands	r0, r1
	adds	r0, #78
	movs	r3, #156
	lsls	r0, r0, #16
	lsls	r3, r3, #15
	str	r0, [r5, #0]
	b.n	.L_08147f48
.L_08147f1c:
	bl	sub_08014878
	mov	r2, fp
	ands	r0, r2
	adds	r0, #78
	movs	r3, #140
	lsls	r0, r0, #16
	lsls	r3, r3, #15
	str	r0, [r5, #0]
	b.n	.L_08147f48
.L_08147f30:
	bl	sub_08014878
	mov	r3, fp
	mov	r1, r9
	ands	r0, r3
	ldr	r3, [r1, #0]
	adds	r0, r0, r3
	subs	r0, #8
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	ldr	r3, [r1, #4]
	lsls	r3, r3, #16
.L_08147f48:
	str	r3, [r5, #4]
	adds	r0, r6, #0
	bl	sub_08002096
	mov	r3, r8
	muls	r3, r0
	asrs	r3, r3, #9
	str	r3, [r5, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	mov	r3, r8
	muls	r3, r0
	asrs	r3, r3, #9
	str	r3, [r5, #16]
	movs	r3, #0
	str	r3, [r5, #24]
	ldr	r2, [sp, #16]
	adds	r2, #1
	str	r2, [sp, #16]
	cmp	r2, #1
	beq.n	.L_08147f7c
.L_08147f74:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #24
	bne.n	.L_08147ed2
.L_08147f7c:
	ldr	r3, [sp, #48]
	cmp	r3, #48
	bne.n	.L_08147f88
	movs	r0, #141
	bl	sub_081c0010
.L_08147f88:
	ldr	r6, [sp, #48]
	cmp	r6, #128
	bne.n	.L_08147f94
	movs	r0, #145
	bl	sub_081c0010
.L_08147f94:
	ldr	r3, [sp, #48]
	subs	r3, #129
	cmp	r3, #46
	bhi.n	.L_08148030
	movs	r1, #76
	movs	r0, #0
	add	r1, sp
	mov	fp, r0
	movs	r7, #0
	mov	r9, r1
	mov	r5, sl
.L_08147faa:
	ldr	r3, [r5, #24]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_08148028
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	adds	r3, #128
	mov	r8, r3
	bl	sub_08014878
	movs	r3, #248
	lsls	r3, r3, #5
	adds	r3, #255
	ands	r3, r0
	ldr	r1, [sp, #60]
	ldr	r0, [pc, #424]
	adds	r6, r3, r0
	cmp	r1, #0
	bne.n	.L_08147ff0
	ldr	r2, [sp, #24]
	cmp	r2, #1
	bne.n	.L_08147fe4
	movs	r3, #144
	lsls	r3, r3, #15
	str	r3, [r5, #0]
	b.n	.L_08147ffc
.L_08147fe4:
	movs	r3, #136
	lsls	r3, r3, #15
	str	r3, [r5, #0]
	movs	r3, #128
	lsls	r3, r3, #15
	b.n	.L_08147ffc
.L_08147ff0:
	mov	r0, r9
	ldr	r3, [r0, #0]
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldr	r3, [r0, #4]
	lsls	r3, r3, #16
.L_08147ffc:
	str	r3, [r5, #4]
	adds	r0, r6, #0
	bl	sub_08002096
	mov	r3, r8
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r5, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	mov	r3, r8
	muls	r3, r0
	movs	r1, #1
	asrs	r3, r3, #6
	add	fp, r1
	str	r3, [r5, #16]
	mov	r2, fp
	movs	r3, #0
	str	r3, [r5, #24]
	cmp	r2, #1
	beq.n	.L_08148030
.L_08148028:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #24
	bne.n	.L_08147faa
.L_08148030:
	ldr	r3, [sp, #48]
	cmp	r3, #175
	bgt.n	.L_08148096
	movs	r7, #0
	mov	r5, sl
.L_0814803a:
	ldr	r1, [r5, #24]
	cmp	r1, #0
	blt.n	.L_0814808e
	asrs	r1, r1, #2
	lsls	r1, r1, #11
	movs	r6, #224
	lsls	r6, r6, #3
	add	r1, sl
	movs	r0, #2
	ldrsh	r2, [r5, r0]
	adds	r1, r1, r6
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	movs	r0, #32
	str	r0, [sp, #0]
	movs	r0, #64
	subs	r2, #16
	subs	r3, #32
	str	r0, [sp, #4]
	ldr	r4, [sp, #68]
	ldr	r0, [sp, #56]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x68eb
	ldr	r0, [sp, #28]
	adds	r2, r0, #0
	muls	r2, r3
	ldr	r3, [r5, #0]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #24
	bne.n	.L_0814808e
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L_0814808e:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #24
	bne.n	.L_0814803a
.L_08148096:
	movs	r5, #140
	lsls	r5, r5, #3
	movs	r7, #0
	movs	r6, #1
	add	r5, sl
.L_081480a0:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_081480ee
	movs	r1, #2
	ldrsh	r2, [r5, r1]
	movs	r1, #2
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	str	r1, [sp, #4]
	str	r6, [sp, #0]
	ldr	r1, [sp, #40]
	subs	r3, #1
	ldr	r4, [r1, #4]
	ldr	r0, [sp, #56]
	ldr	r1, [pc, #188]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x68eb
	ldr	r0, [sp, #28]
	ldr	r1, [pc, #184]
	adds	r2, r0, #0
	muls	r2, r3
	ldr	r3, [r5, #0]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r5, #24]
	adds	r2, r2, r1
	adds	r3, #1
	str	r2, [r5, #16]
	str	r3, [r5, #24]
	cmp	r3, #48
	bne.n	.L_081480ee
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L_081480ee:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #24
	bne.n	.L_081480a0
	ldr	r2, [sp, #48]
	cmp	r2, #175
	bgt.n	.L_081481ba
	ldr	r5, [pc, #116]
	movs	r7, #0
.L_08148100:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_081481ae
	ldr	r2, [pc, #124]
	movs	r3, #3
	ands	r3, r7
	ldrb	r0, [r2, r3]
	ldr	r2, [pc, #120]
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #44]
	movs	r6, #2
	ldrsh	r2, [r5, r6]
	adds	r1, r3, r1
	lsrs	r3, r0, #1
	subs	r2, r2, r3
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #56]
	ldr	r4, [sp, #68]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x68e9
	ldr	r0, [sp, #28]
	ldr	r3, [r5, #0]
	adds	r2, r0, #0
	muls	r2, r1
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [sp, #48]
	cmp	r3, #128
	ble.n	.L_08148194
	movs	r3, #1
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08148160
	ldr	r6, [pc, #48]
	adds	r3, r1, r6
	str	r3, [r5, #12]
	b.n	.L_0814819a
.L_08148160:
	ldr	r0, [pc, #44]
	adds	r3, r1, r0
	str	r3, [r5, #12]
	b.n	.L_0814819a
	.4byte 0x10101010
	.4byte 0x3f3f3f3f
	.4byte 0x08197968
	.4byte 0x02010000
	.4byte 0xffffb1e0
	.4byte 0x0819796c
	.4byte 0xfffffc00
	.4byte 0x0819796e
	.4byte 0x08197410
	.4byte 0xffff8000
	.2byte 0xe000
	.2byte 0xffff
.L_08148194:
	.2byte 0x49a8
	adds	r3, r2, r1
	str	r3, [r5, #16]
.L_0814819a:
	ldr	r3, [r5, #24]
	movs	r2, #128
	adds	r3, #1
	lsls	r2, r2, #1
	str	r3, [r5, #24]
	cmp	r3, r2
	bne.n	.L_081481ae
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L_081481ae:
	movs	r3, #128
	adds	r7, #1
	lsls	r3, r3, #2
	adds	r5, #28
	cmp	r7, r3
	bne.n	.L_08148100
.L_081481ba:
	ldr	r6, [sp, #48]
	cmp	r6, #128
	bne.n	.L_081481cc
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, sl
	movs	r3, #48
	str	r3, [r2, #0]
.L_081481cc:
	ldr	r0, [sp, #60]
	cmp	r0, #0
	bne.n	.L_081481e4
	ldr	r1, [sp, #48]
	cmp	r1, #48
	bne.n	.L_081481e4
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, sl
	movs	r3, #8
	str	r3, [r2, #0]
.L_081481e4:
	ldr	r3, [sp, #48]
	subs	r3, #40
	cmp	r3, #7
	bhi.n	.L_081481fe
	ldr	r2, [sp, #32]
	ldr	r6, [sp, #36]
	ldr	r3, [pc, #584]
	movs	r0, #128
	lsls	r0, r0, #13
	adds	r3, r2, r3
	adds	r0, r6, r0
	str	r3, [sp, #32]
	str	r0, [sp, #36]
.L_081481fe:
	ldr	r1, [sp, #60]
	cmp	r1, #0
	bne.n	.L_081482ac
	ldr	r2, [sp, #48]
	cmp	r2, #128
	bne.n	.L_0814824a
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #220
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #8
	bl	sub_08020030
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #224
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #9
	bl	sub_08020030
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #232
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #10
	bl	sub_08020030
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #236
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #11
	bl	sub_08020030
.L_0814824a:
	ldr	r3, [sp, #48]
	cmp	r3, #176
	bne.n	.L_08148290
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #220
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl	sub_08020030
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #224
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	sub_08020030
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #232
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #3
	bl	sub_08020030
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #236
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #4
	bl	sub_08020030
.L_08148290:
	ldr	r6, [sp, #24]
	cmp	r6, #1
	bne.n	.L_081482a2
	movs	r0, #6
	ldr	r1, [sp, #32]
	ldr	r2, [sp, #36]
	bl	sub_0816442c
	b.n	.L_081482ac
.L_081482a2:
	movs	r0, #3
	ldr	r1, [sp, #32]
	ldr	r2, [sp, #36]
	bl	sub_0816442c
.L_081482ac:
	ldr	r0, [sp, #48]
	cmp	r0, #138
	bne.n	.L_081482e4
	ldr	r1, [sp, #64]
	movs	r7, #0
	ldr	r3, [r1, #20]
	cmp	r3, #0
	beq.n	.L_081482e4
	movs	r5, #36
.L_081482be:
	ldr	r2, [sp, #64]
	movs	r1, #10
	ldrsh	r0, [r5, r2]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r2, #5
	subs	r3, #1
	bl	sub_0814cd48
	ldr	r6, [sp, #64]
	adds	r7, #1
	ldrsh	r0, [r5, r6]
	movs	r1, #4
	bl	sub_08118088
	ldr	r3, [r6, #20]
	adds	r5, #2
	cmp	r7, r3
	bne.n	.L_081482be
.L_081482e4:
	ldr	r2, [sp, #48]
	cmp	r2, #175
	bgt.n	.L_081483ac
	movs	r5, #168
	movs	r3, #0
	movs	r6, #20
	movs	r0, #5
	lsls	r5, r5, #2
	movs	r7, #0
	mov	fp, r3
	mov	r8, r6
	mov	r9, r0
	add	r5, sl
.L_081482fe:
	ldr	r6, [r5, #4]
	cmp	r6, #55
	ble.n	.L_0814834c
	ldr	r3, [r5, #24]
	cmp	r3, #11
	bhi.n	.L_08148340
	lsrs	r4, r3, #31
	ldr	r2, [pc, #304]
	adds	r4, r3, r4
	asrs	r4, r4, #1
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #300]
	movs	r2, #220
	ldrb	r0, [r3, r4]
	lsls	r2, r2, #6
	add	r1, sl
	adds	r1, r1, r2
	ldr	r2, [r5, #0]
	lsrs	r3, r0, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #284]
	ldrb	r3, [r3, r4]
	str	r0, [sp, #0]
	ldr	r0, [pc, #284]
	adds	r3, r6, r3
	ldrb	r0, [r0, r4]
	ldr	r4, [sp, #68]
	str	r0, [sp, #4]
	ldr	r0, [sp, #56]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69ab
.L_08148340:
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #12
	bne.n	.L_081483a4
	mov	r3, fp
	b.n	.L_081483a2
.L_0814834c:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L_081483a0
	ldr	r0, [sp, #28]
	ldr	r2, [r5, #0]
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #1
	subs	r2, r2, r3
	adds	r3, r6, #6
	str	r2, [r5, #0]
	str	r3, [r5, #4]
	ldr	r1, [sp, #48]
	movs	r4, #10
	cmp	r1, #47
	bgt.n	.L_0814837a
	cmp	r3, #55
	ble.n	.L_0814837a
	movs	r0, #136
	str	r4, [sp, #8]
	bl	sub_081c0010
	ldr	r4, [sp, #8]
.L_0814837a:
	ldr	r2, [pc, #212]
	mov	r3, r8
	subs	r3, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #44]
	mov	r3, r9
	adds	r1, r2, r1
	ldr	r2, [r5, #0]
	mov	r6, r8
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	ldr	r0, [sp, #56]
	str	r4, [sp, #0]
	adds	r3, #30
	str	r6, [sp, #4]
	ldr	r4, [sp, #68]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe001
.L_081483a0:
	adds	r3, #1
.L_081483a2:
	str	r3, [r5, #24]
.L_081483a4:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #16
	bne.n	.L_081482fe
.L_081483ac:
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, sl
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	ldr	r0, [sp, #48]
	adds	r0, #1
	str	r0, [sp, #48]
	cmp	r0, #208
	beq.n	.L_081483d6
	b.n	.L_08147cd6
.L_081483d6:
	ldr	r0, [pc, #124]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r1, [sp, #60]
	cmp	r1, #0
	bne.n	.L_08148426
	ldr	r2, [sp, #24]
	cmp	r2, #1
	bne.n	.L_08148400
	movs	r0, #6
	ldr	r1, [sp, #32]
	ldr	r2, [sp, #36]
	bl	sub_0816467c
	b.n	.L_0814840a
.L_08148400:
	movs	r0, #3
	ldr	r1, [sp, #32]
	ldr	r2, [sp, #36]
	bl	sub_0816467c
.L_0814840a:
	ldr	r3, [sp, #60]
	cmp	r3, #0
	bne.n	.L_08148426
	movs	r5, #238
	lsls	r5, r5, #7
	adds	r5, #220
	movs	r7, #0
	add	r5, sl
.L_0814841a:
	ldmia	r5!, {r0}
	adds	r7, #1
	bl	sub_08020048
	cmp	r7, #8
	bne.n	.L_0814841a
.L_08148426:
	bl	sub_08143bb8
	add	sp, #88
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xfffffc00
	.4byte 0xfff80000
	.4byte 0x08197984
	.4byte 0x08197972
	.4byte 0x0819797e
	.4byte 0x08197978
	.4byte 0x08197410
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #60
	str	r0, [sp, #48]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #96]
	movs	r6, #224
	str	r0, [sp, #44]
	movs	r0, #0
	ldr	r1, [r3, #92]
	lsls	r6, r6, #3
	str	r1, [sp, #40]
	ldr	r3, [r3, #100]
	str	r3, [sp, #36]
	bl	sub_081435e0
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	mov	r2, sp
	adds	r2, #52
	adds	r1, r2, #0
	movs	r0, #0
	str	r2, [sp, #32]
	bl	sub_08144aac
	ldr	r3, [sp, #40]
	ldr	r0, [pc, #32]
	adds	r1, r3, r6
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r2, #208
	ldr	r7, [sp, #40]
	lsls	r2, r2, #4
	adds	r2, #228
	adds	r1, r7, r2
	ldr	r0, [pc, #12]
	movs	r2, #1
	movs	r3, #0
	b.n	.L_081484c8
	.4byte 0x00001010
	.4byte 0x0000012f
	.2byte 0x0146
	.2byte 0x0000
.L_081484c8:
	bl	sub_08157cf4
	ldr	r0, [pc, #896]
	ldr	r1, [sp, #36]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r6, #238
	movs	r3, #239
	lsls	r3, r3, #7
	lsls	r6, r6, #7
	adds	r2, r7, r3
	adds	r6, #132
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r1, #200
	adds	r2, r7, r6
	movs	r3, #75
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #864]
	bl	sub_080145a8
	ldr	r3, [pc, #860]
	ldr	r2, [pc, #864]
	movs	r0, #128
	movs	r1, #176
	movs	r7, #0
	lsls	r0, r0, #17
	lsls	r1, r1, #15
	str	r3, [sp, #20]
	ldr	r3, [sp, #40]
	str	r7, [sp, #12]
	str	r0, [sp, #24]
	str	r1, [sp, #28]
	str	r2, [sp, #16]
	movs	r6, #0
	movs	r2, #1
	mov	r8, r6
	negs	r2, r2
	adds	r3, #24
.L_0814851c:
	movs	r7, #1
	add	r8, r7
	mov	r0, r8
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r0, #64
	bne.n	.L_0814851c
	ldr	r2, [sp, #40]
	movs	r3, #168
	movs	r1, #0
	lsls	r3, r3, #2
	mov	r8, r1
	adds	r5, r2, r3
.L_08148536:
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	adds	r3, #56
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r6, #1
	movs	r3, #15
	ands	r3, r0
	add	r8, r6
	negs	r3, r3
	mov	r7, r8
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #16
	bne.n	.L_08148536
	ldr	r3, [pc, #760]
	movs	r0, #0
	movs	r1, #1
	movs	r2, #128
	mov	r8, r0
	negs	r1, r1
	lsls	r2, r2, #3
.L_08148572:
	movs	r6, #1
	add	r8, r6
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_08148572
	ldr	r7, [sp, #40]
	movs	r0, #240
	lsls	r0, r0, #7
	adds	r0, #240
	adds	r3, r7, r0
	ldr	r0, [r3, #0]
	bl	sub_0814cc4c
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #197
	lsls	r1, r1, #1
	adds	r1, #255
	movs	r0, #12
	movs	r2, #2
	bl	sub_08152404
	movs	r1, #0
	mov	r9, r1
.L_081485a6:
	ldr	r3, [pc, #700]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081485c0
	mov	r2, r9
	cmp	r2, #32
	ble.n	.L_081485c0
	cmp	r2, #97
	bgt.n	.L_081485c0
	movs	r3, #98
	mov	r9, r3
.L_081485c0:
	mov	r6, r9
	cmp	r6, #120
	bne.n	.L_081485cc
	movs	r0, #134
	bl	sub_081180e8
.L_081485cc:
	mov	r7, r9
	cmp	r7, #15
	bgt.n	.L_081485d8
	ldr	r0, [sp, #12]
	adds	r0, #2
	str	r0, [sp, #12]
.L_081485d8:
	mov	r1, r9
	cmp	r1, #99
	bgt.n	.L_08148622
	ldr	r3, [sp, #24]
	ldr	r2, [sp, #16]
	ldr	r0, [sp, #16]
	adds	r2, r2, r3
	ldr	r7, [sp, #28]
	ldr	r6, [sp, #20]
	movs	r3, #58
	muls	r3, r0
	adds	r6, r6, r7
	str	r2, [sp, #24]
	str	r6, [sp, #28]
	cmp	r3, #0
	bge.n	.L_081485fa
	adds	r3, #63
.L_081485fa:
	ldr	r1, [sp, #20]
	asrs	r3, r3, #6
	str	r3, [sp, #16]
	lsls	r3, r1, #3
	subs	r3, r3, r1
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L_0814860c
	adds	r3, #63
.L_0814860c:
	asrs	r3, r3, #6
	str	r3, [sp, #20]
	ldr	r2, [sp, #24]
	ldr	r3, [pc, #596]
	cmp	r2, r3
	bgt.n	.L_08148622
	ldr	r6, [sp, #16]
	movs	r7, #128
	lsls	r7, r7, #8
	adds	r7, r6, r7
	str	r7, [sp, #16]
.L_08148622:
	movs	r0, #1
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #28]
	bl	sub_0816442c
	mov	r0, r9
	cmp	r0, #28
	bne.n	.L_081486b6
	ldr	r7, [pc, #568]
	movs	r1, #0
	movs	r2, #63
	mov	r8, r1
	mov	sl, r2
.L_0814863c:
	ldr	r3, [r7, #24]
	movs	r6, #1
	negs	r6, r6
	cmp	r3, r6
	bne.n	.L_081486a8
	bl	sub_08014878
	adds	r6, r0, #0
	mov	r0, sl
	ands	r6, r0
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r3, r6, #0
	muls	r3, r0
	movs	r1, #128
	lsls	r1, r1, #14
	asrs	r3, r3, #3
	adds	r3, r3, r1
	str	r3, [r7, #0]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	movs	r2, #192
	lsls	r2, r2, #15
	asrs	r3, r3, #2
	adds	r3, r3, r2
	str	r3, [r7, #4]
	bl	sub_08014878
	mov	r3, sl
	ands	r0, r3
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r7, #12]
	bl	sub_08014878
	mov	r6, sl
	ands	r0, r6
	negs	r0, r0
	subs	r0, #8
	lsls	r0, r0, #13
	movs	r3, #0
	str	r0, [r7, #16]
	str	r3, [r7, #24]
.L_081486a8:
	movs	r0, #1
	movs	r1, #128
	add	r8, r0
	lsls	r1, r1, #1
	adds	r7, #28
	cmp	r8, r1
	bne.n	.L_0814863c
.L_081486b6:
	mov	r2, r9
	subs	r2, #32
	str	r2, [sp, #8]
	cmp	r2, #47
	bhi.n	.L_08148750
	ldr	r7, [pc, #424]
	movs	r3, #0
	movs	r6, #63
	mov	fp, r3
	mov	r8, r3
	mov	sl, r6
.L_081486cc:
	ldr	r3, [r7, #24]
	movs	r0, #1
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_08148742
	bl	sub_08014878
	mov	r1, sl
	adds	r6, r0, #0
	ands	r6, r1
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r3, r6, #0
	muls	r3, r0
	movs	r2, #128
	lsls	r2, r2, #14
	asrs	r3, r3, #3
	adds	r3, r3, r2
	str	r3, [r7, #0]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	movs	r6, #192
	lsls	r6, r6, #15
	asrs	r3, r3, #2
	adds	r3, r3, r6
	str	r3, [r7, #4]
	bl	sub_08014878
	mov	r1, sl
	ands	r0, r1
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r7, #12]
	bl	sub_08014878
	mov	r2, sl
	movs	r3, #0
	ands	r0, r2
	negs	r0, r0
	str	r3, [r7, #24]
	movs	r3, #1
	subs	r0, #8
	add	fp, r3
	lsls	r0, r0, #13
	mov	r6, fp
	str	r0, [r7, #16]
	cmp	r6, #16
	beq.n	.L_08148750
.L_08148742:
	movs	r0, #1
	movs	r1, #128
	add	r8, r0
	lsls	r1, r1, #3
	adds	r7, #28
	cmp	r8, r1
	bne.n	.L_081486cc
.L_08148750:
	mov	r2, r9
	cmp	r2, #0
	bne.n	.L_0814875c
	movs	r0, #164
	bl	sub_081c0010
.L_0814875c:
	mov	r3, r9
	cmp	r3, #32
	bne.n	.L_08148768
	movs	r0, #145
	bl	sub_081c0010
.L_08148768:
	mov	r6, r9
	cmp	r6, #80
	bne.n	.L_08148774
	movs	r0, #144
	bl	sub_081c0010
.L_08148774:
	ldr	r7, [sp, #8]
	cmp	r7, #47
	bhi.n	.L_081487e4
	ldr	r2, [sp, #40]
	movs	r6, #208
	movs	r0, #0
	lsls	r6, r6, #4
	adds	r6, #228
	mov	r8, r0
	ldr	r0, [pc, #232]
	adds	r2, r2, r6
	mov	r1, r9
	ldr	r6, [pc, #228]
	movs	r7, #34
	lsls	r3, r1, #4
	mov	fp, r7
	mov	sl, r2
	adds	r7, r3, r0
.L_08148798:
	adds	r0, r7, #0
	movs	r1, #104
	bl	sub_08002064
	ldrb	r3, [r6, #1]
	ldrb	r2, [r6, #0]
	adds	r5, r0, #0
	mov	r1, fp
	movs	r0, #104
	subs	r3, r3, r5
	str	r1, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #52]
	subs	r2, #17
	subs	r3, #104
	ldr	r0, [sp, #44]
	mov	r1, sl
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x7832
	ldrb	r3, [r6, #1]
	mov	r1, fp
	subs	r2, #17
	subs	r3, r3, r5
	str	r1, [sp, #0]
	str	r5, [sp, #4]
	ldr	r4, [sp, #52]
	ldr	r0, [sp, #44]
	mov	r1, sl
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2201
	add	r8, r2
	mov	r3, r8
	adds	r6, #2
	adds	r7, #25
	cmp	r3, #3
	bne.n	.L_08148798
.L_081487e4:
	mov	r6, r9
	cmp	r6, #95
	bgt.n	.L_0814882e
	ldr	r0, [sp, #40]
	movs	r1, #224
	lsls	r1, r1, #3
	movs	r7, #0
	adds	r0, r0, r1
	mov	r8, r7
	mov	sl, r0
	movs	r5, #32
	movs	r7, #120
.L_081487fc:
	mov	r2, r8
	lsls	r1, r2, #5
	mov	r2, r9
	cmp	r2, #0
	bge.n	.L_08148808
	adds	r2, #3
.L_08148808:
	movs	r3, #31
	ldr	r6, [sp, #12]
	asrs	r2, r2, #2
	ands	r2, r3
	adds	r2, r1, r2
	subs	r2, #32
	mov	r1, sl
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	ldr	r4, [sp, #52]
	ldr	r0, [sp, #44]
	subs	r3, r7, r6
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2001
	add	r8, r0
	mov	r1, r8
	cmp	r1, #5
	bne.n	.L_081487fc
.L_0814882e:
	ldr	r5, [pc, #60]
	movs	r2, #0
	mov	r8, r2
.L_08148834:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bge.n	.L_0814883c
	b.n	.L_0814894c
.L_0814883c:
	mov	r0, r8
	movs	r1, #3
	bl	sub_08002064
	ldr	r3, [r5, #16]
	adds	r4, r0, #2
	cmp	r3, #0
	ble.n	.L_08148878
	adds	r4, #2
	b.n	.L_08148878
	.4byte 0x00000134
	.4byte 0x08143001
	.4byte 0xfffc0000
	.4byte 0xfff00000
	.4byte 0x02010018
	.4byte 0x03001150
	.4byte 0x0077ffff
	.4byte 0x02010000
	.4byte 0xffffff00
	.2byte 0x7990
	.2byte 0x0819
.L_08148878:
	mov	r6, r9
	cmp	r6, #68
	ble.n	.L_08148884
	cmp	r4, #5
	bgt.n	.L_08148884
	movs	r4, #6
.L_08148884:
	mov	r7, r9
	cmp	r7, #70
	ble.n	.L_08148890
	cmp	r4, #6
	bgt.n	.L_08148890
	movs	r4, #7
.L_08148890:
	mov	r0, r9
	cmp	r0, #72
	ble.n	.L_0814889c
	cmp	r4, #7
	bgt.n	.L_0814889c
	movs	r4, #8
.L_0814889c:
	mov	r1, r9
	cmp	r1, #74
	ble.n	.L_081488a8
	cmp	r4, #8
	bgt.n	.L_081488a8
	movs	r4, #9
.L_081488a8:
	mov	r2, r9
	cmp	r2, #76
	ble.n	.L_081488b0
	movs	r4, #10
.L_081488b0:
	movs	r6, #4
	cmp	r3, #0
	bgt.n	.L_081488b8
	movs	r6, #0
.L_081488b8:
	ldr	r2, [pc, #392]
	lsls	r0, r4, #1
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #36]
	movs	r7, #2
	ldrsh	r2, [r5, r7]
	adds	r1, r3, r1
	lsrs	r3, r4, #31
	adds	r3, r4, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #4]
	str	r4, [sp, #0]
	ldr	r0, [sp, #32]
	subs	r3, r3, r4
	ldr	r4, [r6, r0]
	ldr	r0, [sp, #44]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x682b
	ldr	r2, [r5, #12]
	ldr	r1, [r5, #16]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r3, [r5, #4]
	mov	r2, r9
	adds	r3, r3, r1
	str	r3, [r5, #4]
	cmp	r2, #80
	ble.n	.L_08148900
	ldr	r6, [pc, #332]
	adds	r3, r1, r6
	b.n	.L_0814890e
.L_08148900:
	ldr	r3, [pc, #328]
	movs	r2, #3
	mov	r7, r8
	ands	r2, r7
	lsls	r2, r2, #2
	ldr	r3, [r3, r2]
	adds	r3, r1, r3
.L_0814890e:
	str	r3, [r5, #16]
	ldr	r2, [r5, #12]
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r3, r3, #1
	cmp	r3, #0
	bge.n	.L_0814891e
	adds	r3, #63
.L_0814891e:
	ldr	r2, [r5, #16]
	asrs	r3, r3, #6
	str	r3, [r5, #12]
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r2, r3, #1
	cmp	r2, #0
	bge.n	.L_08148930
	adds	r2, #63
.L_08148930:
	ldr	r3, [r5, #24]
	asrs	r2, r2, #6
	adds	r3, #1
	str	r2, [r5, #16]
	str	r3, [r5, #24]
	cmp	r2, #0
	ble.n	.L_0814894c
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	cmp	r3, #104
	ble.n	.L_0814894c
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L_0814894c:
	movs	r1, #1
	movs	r2, #128
	add	r8, r1
	lsls	r2, r2, #3
	adds	r5, #28
	cmp	r8, r2
	beq.n	.L_0814895c
	b.n	.L_08148834
.L_0814895c:
	mov	r3, r9
	cmp	r3, #79
	bgt.n	.L_081489d6
	ldr	r7, [sp, #48]
	movs	r6, #0
	ldr	r3, [r7, #20]
	mov	r8, r6
	cmp	r3, #0
	beq.n	.L_081489d6
	adds	r7, #36
.L_08148970:
	mov	r0, r9
	cmp	r0, #29
	ble.n	.L_081489cc
	movs	r1, #12
	bl	sub_08002064
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_081489ac
	movs	r1, #0
	ldrsh	r0, [r7, r1]
	bl	sub_08118098
	movs	r3, #1
	ldr	r5, [r0, #0]
	negs	r3, r3
	movs	r2, #0
	ldrsh	r0, [r7, r2]
	movs	r1, #7
	movs	r2, #5
	str	r6, [sp, #0]
	bl	sub_0814cd48
	movs	r3, #144
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	movs	r3, #171
	lsls	r3, r3, #8
	adds	r3, #133
	str	r3, [r5, #72]
.L_081489ac:
	cmp	r6, #6
	bne.n	.L_081489c8
	movs	r3, #0
	ldrsh	r0, [r7, r3]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #0
	subs	r3, #1
	movs	r2, #5
	bl	sub_0814cd48
	ldr	r6, [sp, #48]
	ldr	r3, [r6, #20]
	b.n	.L_081489cc
.L_081489c8:
	ldr	r0, [sp, #48]
	ldr	r3, [r0, #20]
.L_081489cc:
	movs	r1, #1
	add	r8, r1
	adds	r7, #2
	cmp	r8, r3
	bne.n	.L_08148970
.L_081489d6:
	ldr	r3, [sp, #40]
	movs	r6, #240
	lsls	r6, r6, #7
	adds	r6, #232
	adds	r2, r3, r6
	movs	r7, #1
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	add	r9, r7
	bl	sub_08013560
	mov	r0, r9
	cmp	r0, #124
	beq.n	.L_081489f6
	b.n	.L_081485a6
.L_081489f6:
	ldr	r0, [pc, #88]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #28]
	movs	r0, #1
	bl	sub_0816467c
	movs	r3, #238
	ldr	r2, [sp, #40]
	lsls	r3, r3, #7
	movs	r1, #0
	adds	r3, #220
	mov	r8, r1
	adds	r5, r2, r3
.L_08148a20:
	movs	r6, #1
	add	r8, r6
	ldmia	r5!, {r0}
	mov	r7, r8
	bl	sub_08020048
	cmp	r7, #12
	bne.n	.L_08148a20
	bl	sub_08143bb8
	add	sp, #60
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x08197410
	.4byte 0xffff8000
	.4byte 0x08197998
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r5, #192
	lsls	r5, r5, #18
	mov	r8, r0
	ldr	r0, [r5, #96]
	sub	sp, #32
	ldr	r7, [r5, #92]
	str	r0, [sp, #16]
	movs	r0, #1
	bl	sub_081435e0
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r1, r7, r2
	movs	r3, #1
	movs	r2, #1
	ldr	r0, [pc, #80]
	bl	sub_08157cf4
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	ldr	r3, [r5, #104]
	movs	r1, #31
	movs	r0, #188
	str	r3, [sp, #8]
	bl	sub_081963ec
	adds	r5, #188
	ldr	r3, [pc, #48]
	movs	r2, #128
	ldr	r5, [r5, #0]
	lsls	r2, r2, #19
	adds	r2, #82
	mov	r4, r8
	strh	r3, [r2, #0]
	ldr	r0, [r4, #8]
	str	r5, [sp, #12]
	bl	sub_08118098
	mov	r1, r8
	ldr	r6, [r0, #0]
	ldr	r0, [r1, #8]
	bl	sub_08118070
	ldr	r3, [r6, #12]
	movs	r2, #0
	adds	r3, r3, r0
	mov	fp, r3
	movs	r3, #255
	mov	sl, r2
	mov	r9, r3
	adds	r5, r7, #0
	b.n	.L_08148ad8
	movs	r0, r0
	.4byte 0x00000f0f
	.2byte 0x0191
	.2byte 0x0000
.L_08148ad8:
	ldr	r3, [r6, #8]
	mov	r4, fp
	str	r4, [r5, #4]
	str	r3, [r5, #0]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	sub_08014878
	mov	r1, r9
	ands	r0, r1
	subs	r0, #127
	lsls	r0, r0, #16
	asrs	r0, r0, #5
	str	r0, [r5, #12]
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	subs	r3, #16
	lsls	r3, r3, #16
	asrs	r3, r3, #6
	str	r3, [r5, #16]
	bl	sub_08014878
	mov	r2, r9
	ands	r0, r2
	subs	r0, #127
	lsls	r0, r0, #16
	asrs	r0, r0, #5
	movs	r3, #1
	ldr	r4, [pc, #884]
	movs	r1, #1
	str	r0, [r5, #20]
	negs	r3, r3
	mov	r0, sl
	add	sl, r1
	str	r3, [r5, #24]
	mov	r2, sl
	movs	r3, #0
	strb	r3, [r4, r0]
	adds	r5, #28
	cmp	r2, #30
	bne.n	.L_08148ad8
	movs	r3, #0
	mov	r4, r8
	mov	sl, r3
	ldr	r3, [r4, #20]
	cmp	r3, #0
	beq.n	.L_08148b68
	movs	r0, #224
	lsls	r0, r0, #2
	movs	r6, #36
	adds	r5, r7, r0
.L_08148b42:
	mov	r1, r8
	ldrsh	r0, [r6, r1]
	bl	sub_08118098
	ldr	r2, [r0, #0]
	mov	r4, r8
	ldr	r3, [r2, #8]
	adds	r6, #2
	str	r3, [r5, #0]
	movs	r3, #0
	str	r3, [r5, #4]
	ldr	r3, [r2, #16]
	str	r3, [r5, #8]
	movs	r3, #1
	add	sl, r3
	ldr	r3, [r4, #20]
	adds	r5, #28
	cmp	sl, r3
	bne.n	.L_08148b42
.L_08148b68:
	movs	r0, #238
	lsls	r0, r0, #7
	movs	r1, #238
	adds	r0, #172
	lsls	r1, r1, #7
	movs	r2, #0
	adds	r3, r7, r0
	adds	r1, #176
	movs	r5, #200
	str	r2, [r3, #0]
	lsls	r5, r5, #4
	adds	r3, r7, r1
	str	r2, [r3, #0]
	adds	r1, r5, #0
	ldr	r0, [pc, #776]
	bl	sub_080145a8
	movs	r3, #239
	movs	r4, #238
	lsls	r3, r3, #7
	lsls	r4, r4, #7
	adds	r2, r7, r3
	adds	r4, #132
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r7, r4
	movs	r3, #75
	str	r3, [r2, #0]
	adds	r1, r5, #0
	ldr	r0, [pc, #752]
	bl	sub_080145a8
	movs	r0, #164
	bl	sub_081c0010
	mov	r1, r8
	ldr	r3, [r1, #24]
	ldr	r2, [pc, #740]
	lsls	r3, r3, #1
	adds	r3, #1
	ldrb	r3, [r2, r3]
	movs	r0, #0
	mov	fp, r0
	cmp	r3, #0
	bne.n	.L_08148bc4
	b.n	.L_08148e60
.L_08148bc4:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #48]
	mov	r3, fp
	subs	r3, #17
	cmp	r3, #46
	bhi.n	.L_08148be0
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #172
	adds	r2, r7, r3
	movs	r3, #192
	lsls	r3, r3, #1
	b.n	.L_08148bea
.L_08148be0:
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #172
	adds	r2, r7, r4
	movs	r3, #0
.L_08148bea:
	str	r3, [r2, #0]
	mov	r0, r8
	ldr	r3, [r0, #24]
	ldr	r6, [pc, #676]
	lsls	r3, r3, #1
	adds	r3, #1
	ldrb	r3, [r6, r3]
	subs	r3, #16
	cmp	fp, r3
	bne.n	.L_08148c04
	movs	r0, #133
	bl	sub_081180e8
.L_08148c04:
	bl	sub_08014de4
	adds	r1, r5, #0
	adds	r1, #12
	adds	r0, r5, #0
	bl	sub_080156e8
	movs	r1, #0
	mov	r2, r8
	mov	sl, r1
	ldr	r1, [r2, #24]
	lsls	r3, r1, #1
	ldrb	r3, [r6, r3]
	cmp	r3, #0
	bne.n	.L_08148c24
	b.n	.L_08148e18
.L_08148c24:
	movs	r3, #0
	mov	r9, r3
	adds	r6, r7, #0
.L_08148c2a:
	cmp	fp, r9
	ble.n	.L_08148c94
	ldr	r4, [pc, #604]
	mov	r0, sl
	ldrsb	r3, [r4, r0]
	cmp	r3, #0
	bne.n	.L_08148c94
	add	r5, sp, #20
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_0815e1ec
	ldr	r3, [r5, #0]
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	ldr	r3, [r5, #8]
	cmp	r3, #159
	bgt.n	.L_08148c52
	movs	r3, #160
	str	r3, [r5, #8]
.L_08148c52:
	movs	r2, #136
	lsls	r2, r2, #2
	adds	r2, #255
	cmp	r3, r2
	ble.n	.L_08148c5e
	str	r2, [r5, #8]
.L_08148c5e:
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	movs	r1, #12
	movs	r4, #152
	str	r1, [sp, #0]
	lsls	r4, r4, #5
	movs	r1, #24
	subs	r2, #6
	subs	r3, #12
	str	r1, [sp, #4]
	ldr	r0, [sp, #16]
	adds	r1, r7, r4
	ldr	r4, [sp, #8]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x6833
	ldr	r2, [r6, #12]
	adds	r3, r3, r2
	str	r3, [r6, #0]
	ldr	r2, [r6, #16]
	ldr	r3, [r6, #4]
	adds	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r2, [r6, #20]
	ldr	r3, [r6, #8]
	adds	r3, r3, r2
	str	r3, [r6, #8]
.L_08148c94:
	mov	r3, r9
	adds	r3, #48
	cmp	fp, r3
	ble.n	.L_08148d92
	ldr	r0, [pc, #492]
	mov	r1, sl
	ldrsb	r3, [r0, r1]
	cmp	r3, #0
	bne.n	.L_08148d92
	mov	r2, r8
	ldr	r1, [r2, #20]
	mov	r0, sl
	bl	sub_08002064
	lsls	r1, r0, #3
	subs	r1, r1, r0
	lsls	r1, r1, #2
	movs	r3, #224
	lsls	r3, r3, #2
	adds	r1, r7, r1
	adds	r1, r1, r3
	ldr	r3, [r1, #0]
	ldr	r2, [r6, #0]
	subs	r3, r3, r2
	ldr	r2, [r6, #12]
	asrs	r3, r3, #9
	adds	r0, r2, r3
	str	r0, [r6, #12]
	ldr	r2, [r6, #4]
	ldr	r3, [r1, #4]
	subs	r3, r3, r2
	ldr	r2, [r6, #16]
	asrs	r3, r3, #9
	adds	r4, r2, r3
	str	r4, [r6, #16]
	ldr	r2, [r6, #8]
	ldr	r3, [r1, #8]
	subs	r3, r3, r2
	ldr	r2, [r6, #20]
	asrs	r3, r3, #9
	adds	r1, r2, r3
	mov	r3, r9
	adds	r3, #85
	str	r1, [r6, #20]
	cmp	fp, r3
	bge.n	.L_08148d20
	lsls	r3, r0, #4
	subs	r3, r3, r0
	lsls	r2, r3, #2
	cmp	r2, #0
	bge.n	.L_08148cfc
	adds	r2, #63
.L_08148cfc:
	asrs	r3, r2, #6
	str	r3, [r6, #12]
	lsls	r3, r4, #4
	subs	r3, r3, r4
	lsls	r2, r3, #2
	cmp	r2, #0
	bge.n	.L_08148d0c
	adds	r2, #63
.L_08148d0c:
	asrs	r3, r2, #6
	str	r3, [r6, #16]
	lsls	r3, r1, #4
	subs	r3, r3, r1
	lsls	r2, r3, #2
	cmp	r2, #0
	bge.n	.L_08148d1c
	adds	r2, #63
.L_08148d1c:
	asrs	r3, r2, #6
	str	r3, [r6, #20]
.L_08148d20:
	ldr	r3, [r6, #4]
	cmp	r3, #0
	bge.n	.L_08148d92
	ldr	r4, [pc, #356]
	movs	r3, #1
	mov	r0, sl
	strb	r3, [r4, r0]
	movs	r3, #0
	str	r3, [r6, #24]
	add	r5, sp, #20
	ldr	r3, [r5, #0]
	str	r3, [r6, #0]
	bl	sub_08014878
	ldr	r3, [r5, #4]
	movs	r2, #31
	ands	r2, r0
	adds	r3, r3, r2
	subs	r3, #16
	str	r3, [r6, #4]
	mov	r2, r8
	ldr	r1, [r2, #20]
	mov	r0, sl
	bl	sub_08002064
	adds	r3, r0, #0
	lsls	r2, r3, #1
	mov	r4, r8
	adds	r2, #36
	ldrsh	r0, [r4, r2]
	movs	r2, #4
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	bl	sub_0814cd48
	mov	r3, r8
	ldr	r1, [r3, #20]
	mov	r0, sl
	bl	sub_08002064
	lsls	r0, r0, #1
	mov	r4, r8
	adds	r0, #36
	ldrsh	r0, [r4, r0]
	movs	r1, #0
	bl	sub_08118088
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	adds	r3, r7, r2
	movs	r4, #4
	str	r4, [r3, #0]
	movs	r0, #132
	bl	sub_081c0010
.L_08148d92:
	ldr	r3, [r6, #24]
	cmp	r3, #15
	bhi.n	.L_08148dfe
	lsrs	r0, r3, #31
	adds	r0, r3, r0
	movs	r1, #3
	asrs	r0, r0, #1
	bl	sub_08002064
	adds	r1, r0, #0
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #4]
	lsls	r1, r1, #10
	movs	r0, #224
	lsls	r0, r0, #3
	adds	r1, r7, r1
	adds	r1, r1, r0
	movs	r4, #16
	movs	r0, #64
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #8]
	subs	r2, #16
	ldr	r0, [sp, #16]
	subs	r3, #56
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69b0
	movs	r1, #3
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	bl	sub_08002064
	adds	r1, r0, #0
	ldr	r3, [r6, #4]
	lsls	r1, r1, #10
	movs	r0, #224
	lsls	r0, r0, #3
	adds	r1, r7, r1
	adds	r1, r1, r0
	movs	r4, #16
	movs	r0, #64
	ldr	r2, [r6, #0]
	subs	r3, #56
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #12]
	ldr	r0, [sp, #16]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69b3
	adds	r3, #1
	str	r3, [r6, #24]
.L_08148dfe:
	movs	r1, #1
	mov	r2, r8
	add	sl, r1
	ldr	r1, [r2, #24]
	ldr	r3, [pc, #144]
	lsls	r2, r1, #1
	ldrb	r3, [r3, r2]
	movs	r0, #2
	add	r9, r0
	adds	r6, #28
	cmp	sl, r3
	beq.n	.L_08148e18
	b.n	.L_08148c2a
.L_08148e18:
	lsls	r1, r1, #1
	adds	r1, #2
	adds	r0, r1, #0
	bl	sub_08158ce0
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #176
	adds	r2, r7, r3
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_08148e34
	movs	r3, #1
	str	r3, [r2, #0]
.L_08148e34:
	bl	sub_081434f8
	movs	r4, #240
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r7, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	mov	r1, r8
	ldr	r3, [r1, #24]
	ldr	r2, [pc, #72]
	lsls	r3, r3, #1
	adds	r3, #1
	ldrb	r3, [r2, r3]
	movs	r0, #1
	add	fp, r0
	cmp	fp, r3
	beq.n	.L_08148e60
	b.n	.L_08148bc4
.L_08148e60:
	ldr	r0, [pc, #44]
	bl	sub_08014644
	ldr	r0, [pc, #44]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x0814c929
	.4byte 0x08143001
	.2byte 0x79a8
	.2byte 0x0819
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #52
	str	r0, [sp, #40]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #92]
	movs	r7, #0
	str	r0, [sp, #36]
	movs	r0, #1
	ldr	r1, [r5, #96]
	str	r1, [sp, #32]
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r2, [sp, #36]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #44]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	ldr	r3, [r5, #104]
	movs	r0, #188
	movs	r1, #23
	str	r3, [sp, #44]
	bl	sub_081963ec
	adds	r5, #188
	ldr	r3, [r5, #0]
	mov	r0, sp
	adds	r0, #44
	str	r0, [sp, #24]
	str	r3, [r0, #4]
	b.n	.L_08148f08
	.4byte 0x00001010
	.2byte 0x0192
	.2byte 0x0000
.L_08148f08:
	ldr	r5, [sp, #36]
.L_08148f0a:
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	adds	r3, #16
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	negs	r3, r3
	adds	r7, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #16
	bne.n	.L_08148f0a
	ldr	r1, [sp, #36]
	movs	r3, #239
	movs	r0, #238
	lsls	r3, r3, #7
	lsls	r0, r0, #7
	adds	r2, r1, r3
	adds	r0, #132
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r1, r0
	movs	r3, #50
	movs	r1, #200
	lsls	r1, r1, #4
	str	r3, [r2, #0]
	ldr	r0, [pc, #64]
	bl	sub_080145a8
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #141
	bl	sub_081c0010
	movs	r1, #0
	mov	fp, r1
.L_08148f70:
	mov	r2, fp
	lsls	r0, r2, #10
	bl	sub_08002096
	mov	r3, fp
	lsls	r0, r0, #4
	str	r0, [sp, #28]
	cmp	r3, #32
	bne.n	.L_08148f88
	movs	r0, #133
	bl	sub_081180e8
.L_08148f88:
	ldr	r6, [pc, #12]
	movs	r0, #0
	mov	r9, r0
	b.n	.L_08148f9c
	.4byte 0x00001000
	.4byte 0x08143001
	.2byte 0x0260
	.2byte 0x0300
.L_08148f9c:
	movs	r5, #16
.L_08148f9e:
	cmp	fp, r5
	bne.n	sub_08148fae
	movs	r1, #128
	ldr	r0, [sp, #32]
	lsls	r1, r1, #7
	ldr	r2, [pc, #36]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x2101
	add	r9, r1
	mov	r2, r9
	adds	r5, #8
	cmp	r2, #7
	bne.n	.L_08148f9e
	ldr	r0, [sp, #40]
	ldr	r3, [r0, #4]
	cmp	r3, #1
	bne.n	.L_08148fd4
	ldr	r1, [sp, #28]
	movs	r2, #128
	lsls	r2, r2, #14
	adds	r1, r1, r2
	str	r1, [sp, #28]
	b.n	.L_08148fdc
	movs	r0, r0
	.2byte 0x0808
	.2byte 0x0808
.L_08148fd4:
	ldr	r3, [sp, #28]
	ldr	r0, [pc, #72]
	adds	r3, r3, r0
	str	r3, [sp, #28]
.L_08148fdc:
	mov	r1, fp
	cmp	r1, #16
	bgt.n	.L_08148ff0
	ldr	r2, [pc, #52]
	movs	r1, #128
	lsls	r1, r1, #19
	mov	r3, fp
	adds	r1, #82
	orrs	r3, r2
	strh	r3, [r1, #0]
.L_08148ff0:
	mov	r2, fp
	cmp	r2, #63
	ble.n	.L_08149008
	ldr	r2, [pc, #36]
	ldr	r1, [pc, #28]
	movs	r3, #128
	mov	r0, fp
	lsls	r3, r3, #19
	subs	r2, r2, r0
	adds	r3, #82
	orrs	r2, r1
	strh	r2, [r3, #0]
.L_08149008:
	ldr	r2, [sp, #40]
	ldr	r0, [pc, #24]
	ldr	r3, [r2, #24]
	movs	r1, #0
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldrb	r3, [r0, r2]
	b.n	.L_08149028
	.4byte 0x00001000
	.4byte 0x0000004f
	.4byte 0xffe00000
	.2byte 0x79ae
	.2byte 0x0819
.L_08149028:
	mov	r9, r1
	cmp	r3, #0
	bne.n	.L_08149030
	b.n	.L_08149190
.L_08149030:
	mov	r1, fp
	mov	r2, fp
	asrs	r1, r1, #31
	lsls	r2, r2, #11
	movs	r3, #0
	str	r1, [sp, #20]
	str	r2, [sp, #16]
	str	r3, [sp, #12]
.L_08149040:
	ldr	r0, [sp, #16]
	bl	sub_08002096
	ldr	r1, [sp, #40]
	movs	r7, #0
	ldr	r2, [r1, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r2, [pc, #512]
	adds	r3, #1
	ldrb	r3, [r2, r3]
	muls	r3, r0
	ldr	r0, [sp, #28]
	adds	r3, r3, r0
	asrs	r3, r3, #16
	ldr	r0, [sp, #16]
	adds	r3, #40
	mov	sl, r3
	bl	sub_08002090
	ldr	r1, [sp, #20]
	lsls	r0, r0, #1
	asrs	r0, r0, #16
	mov	r8, r0
	lsrs	r0, r1, #31
	add	r0, fp
	movs	r1, #3
	asrs	r0, r0, #1
	bl	sub_08002064
	ldr	r2, [sp, #36]
	lsls	r5, r0, #2
	adds	r5, r5, r0
	movs	r3, #152
	lsls	r6, r5, #9
	lsls	r3, r3, #5
	adds	r6, r2, r6
	adds	r3, #86
	adds	r1, r6, r3
	movs	r0, #40
	movs	r2, #32
	mov	r3, r8
	str	r0, [sp, #0]
	str	r2, [sp, #4]
	ldr	r4, [sp, #44]
	ldr	r0, [sp, #32]
	adds	r3, #16
	mov	r2, sl
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9b09
	movs	r0, #196
	lsls	r5, r5, #8
	lsls	r0, r0, #6
	adds	r5, r3, r5
	adds	r0, #86
	adds	r5, r5, r0
	movs	r1, #40
	movs	r2, #32
	mov	r3, r8
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	ldr	r4, [sp, #44]
	adds	r3, #48
	ldr	r0, [sp, #32]
	adds	r1, r5, #0
	mov	r2, sl
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x23c0
	lsls	r3, r3, #5
	adds	r3, #86
	adds	r6, r6, r3
	movs	r0, #40
	movs	r1, #32
	mov	r3, r8
	adds	r3, #80
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	mov	r2, sl
	adds	r1, r6, #0
	ldr	r4, [sp, #44]
	ldr	r0, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9a03
	ldr	r3, [sp, #36]
	adds	r6, r2, r3
.L_081490f0:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	blt.n	.L_08149140
	lsrs	r2, r7, #31
	adds	r2, r7, r2
	asrs	r2, r2, #1
	lsrs	r4, r3, #31
	adds	r4, r3, r4
	lsls	r3, r2, #1
	adds	r3, r3, r2
	movs	r0, #1
	ldr	r2, [pc, #336]
	asrs	r4, r4, #1
	adds	r5, r7, #0
	adds	r4, r4, r3
	ands	r5, r0
	ldr	r0, [pc, #328]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #36]
	ldrb	r0, [r0, r4]
	movs	r3, #224
	adds	r1, r2, r1
	lsls	r3, r3, #3
	ldr	r2, [r6, #0]
	adds	r1, r1, r3
	ldr	r3, [r6, #4]
	str	r0, [sp, #0]
	ldr	r0, [pc, #308]
	lsls	r5, r5, #2
	ldrb	r0, [r0, r4]
	add	r3, r8
	str	r0, [sp, #4]
	ldr	r0, [sp, #24]
	add	r2, sl
	ldr	r4, [r5, r0]
	ldr	r0, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69b3
.L_08149140:
	adds	r3, #1
	str	r3, [r6, #24]
	cmp	r3, #6
	bne.n	.L_08149162
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	str	r3, [r6, #0]
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	adds	r3, #16
	str	r3, [r6, #4]
	movs	r3, #0
	str	r3, [r6, #24]
.L_08149162:
	adds	r7, #1
	adds	r6, #28
	cmp	r7, #4
	bne.n	.L_081490f0
	ldr	r1, [sp, #16]
	ldr	r3, [sp, #12]
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r1, r1, r2
	adds	r3, #112
	str	r1, [sp, #16]
	str	r3, [sp, #12]
	ldr	r1, [sp, #40]
	movs	r0, #1
	ldr	r3, [r1, #24]
	add	r9, r0
	ldr	r0, [pc, #208]
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldrb	r3, [r0, r2]
	cmp	r9, r3
	beq.n	.L_08149190
	b.n	.L_08149040
.L_08149190:
	ldr	r1, [sp, #40]
	movs	r7, #0
	ldr	r3, [r1, #20]
	cmp	r3, #0
	beq.n	.L_081491ea
	movs	r2, #0
	mov	r8, r2
	movs	r4, #36
.L_081491a0:
	movs	r3, #0
	mov	r5, r8
	mov	r9, r3
	adds	r6, r4, #0
	adds	r5, #16
.L_081491aa:
	cmp	fp, r5
	bne.n	.L_081491ce
	ldr	r1, [sp, #40]
	movs	r3, #4
	ldrsh	r0, [r1, r6]
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	adds	r3, r7, #0
	str	r4, [sp, #8]
	bl	sub_0814cd48
	ldr	r3, [sp, #40]
	ldrsh	r0, [r3, r6]
	movs	r1, #6
	bl	sub_08118088
	ldr	r4, [sp, #8]
.L_081491ce:
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	adds	r5, #8
	cmp	r3, #7
	bne.n	.L_081491aa
	ldr	r1, [sp, #40]
	movs	r0, #3
	ldr	r3, [r1, #20]
	adds	r7, #1
	add	r8, r0
	adds	r4, #2
	cmp	r7, r3
	bne.n	.L_081491a0
.L_081491ea:
	ldr	r2, [sp, #36]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #168
	adds	r3, r2, r0
	movs	r1, #1
	str	r1, [r3, #0]
	ldr	r3, [sp, #40]
	ldr	r1, [pc, #88]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, #2
	ldrb	r0, [r1, r3]
	lsls	r1, r0, #1
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r0, #240
	ldr	r2, [sp, #36]
	lsls	r0, r0, #7
	adds	r0, #232
	adds	r3, r2, r0
	movs	r1, #1
	str	r1, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #1
	add	fp, r2
	mov	r3, fp
	cmp	r3, #80
	beq.n	.L_08149230
	b.n	.L_08148f70
.L_08149230:
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #36]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x081979ae
	.4byte 0x08197486
	.4byte 0x08197492
	.4byte 0x08197498
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	mov	r9, r0
	ldr	r0, [r3, #92]
	sub	sp, #40
	str	r0, [sp, #28]
	movs	r0, #0
	ldr	r3, [r3, #96]
	movs	r7, #239
	str	r3, [sp, #24]
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r1, [sp, #28]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r5, r1, r2
	adds	r1, r5, #0
	ldr	r0, [pc, #44]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	adds	r1, r5, #0
	movs	r2, #1
	ldr	r0, [pc, #32]
	movs	r3, #1
	bl	sub_08157cf4
	mov	r4, sp
	adds	r4, #32
	mov	r3, r9
	ldr	r0, [r3, #4]
	adds	r1, r4, #0
	str	r4, [sp, #16]
	bl	sub_08144aac
	ldr	r5, [sp, #28]
	b.n	.L_081492d8
	.4byte 0x00001010
	.4byte 0x0000014d
	.2byte 0x0193
	.2byte 0x0000
.L_081492d8:
	movs	r0, #238
	lsls	r7, r7, #7
	lsls	r0, r0, #7
	adds	r2, r5, r7
	movs	r3, #2
	adds	r0, #132
	str	r3, [r2, #0]
	movs	r1, #200
	adds	r2, r5, r0
	movs	r3, #50
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #804]
	bl	sub_080145a8
	ldr	r2, [pc, #804]
	movs	r1, #0
	str	r2, [sp, #12]
	mov	r3, r9
	mov	r8, r1
	ldr	r1, [r3, #24]
	lsls	r3, r1, #1
	adds	r3, r3, r1
	ldrb	r3, [r2, r3]
	cmp	r3, #0
	beq.n	.L_08149384
	ldr	r5, [sp, #28]
	movs	r4, #31
	movs	r0, #63
	mov	fp, r4
	movs	r7, #0
	mov	sl, r0
.L_08149318:
	bl	sub_08014878
	ldr	r3, [pc, #768]
	mov	r1, r9
	str	r3, [r5, #4]
	ldr	r3, [r1, #4]
	cmp	r3, #1
	bne.n	.L_0814933e
	bl	sub_08014878
	mov	r2, fp
	ands	r0, r2
	adds	r0, #80
	lsls	r6, r0, #16
	bl	sub_08014878
	mov	r3, sl
	ands	r0, r3
	b.n	.L_08149354
.L_0814933e:
	bl	sub_08014878
	mov	r4, fp
	ands	r0, r4
	adds	r0, #8
	lsls	r6, r0, #16
	bl	sub_08014878
	mov	r1, sl
	ands	r0, r1
	negs	r0, r0
.L_08149354:
	lsls	r0, r0, #12
	str	r0, [r5, #12]
	ldr	r2, [r5, #12]
	str	r7, [r5, #24]
	lsls	r3, r2, #3
	adds	r3, r3, r2
	lsls	r3, r3, #1
	subs	r3, r6, r3
	str	r3, [r5, #0]
	movs	r3, #0
	str	r3, [r5, #16]
	str	r3, [r5, #8]
	mov	r3, r9
	ldr	r1, [r3, #24]
	ldr	r4, [pc, #680]
	lsls	r3, r1, #1
	adds	r3, r3, r1
	ldrb	r3, [r4, r3]
	movs	r2, #1
	add	r8, r2
	adds	r5, #28
	adds	r7, #8
	cmp	r8, r3
	bne.n	.L_08149318
.L_08149384:
	adds	r2, r1, #0
	movs	r5, #0
	str	r5, [sp, #20]
	lsls	r3, r2, #1
	ldr	r7, [sp, #12]
	adds	r3, r3, r2
	adds	r3, #1
	ldrb	r3, [r7, r3]
	cmp	r3, #0
	bne.n	.L_0814939a
	b.n	.L_0814969a
.L_0814939a:
	cmp	r2, #2
	bne.n	.L_081493e2
	ldr	r0, [sp, #20]
	cmp	r0, #103
	bgt.n	.L_081493e2
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #48]
	movs	r2, #192
	cmp	r0, #95
	ble.n	.L_081493be
	ldr	r2, [sp, #20]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	movs	r2, #156
	lsls	r3, r3, #3
	lsls	r2, r2, #4
	subs	r2, r2, r3
.L_081493be:
	mov	r4, r9
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_081493d4
	ldrh	r3, [r1, #54]
	ldr	r5, [pc, #592]
	subs	r3, r3, r2
	strh	r3, [r1, #54]
	str	r5, [sp, #12]
	ldr	r1, [r4, #24]
	b.n	.L_081493e2
.L_081493d4:
	ldrh	r3, [r1, #54]
	ldr	r7, [pc, #580]
	adds	r3, r3, r2
	strh	r3, [r1, #54]
	str	r7, [sp, #12]
	mov	r0, r9
	ldr	r1, [r0, #24]
.L_081493e2:
	lsls	r3, r1, #1
	ldr	r2, [sp, #12]
	adds	r3, r3, r1
	adds	r3, #2
	ldrb	r3, [r2, r3]
	ldr	r4, [sp, #20]
	cmp	r4, r3
	bne.n	.L_081493fc
	movs	r0, #134
	mov	r5, r9
	bl	sub_081180e8
	ldr	r1, [r5, #24]
.L_081493fc:
	lsls	r3, r1, #1
	ldr	r7, [sp, #12]
	adds	r3, r3, r1
	adds	r3, #1
	ldrb	r3, [r7, r3]
	ldr	r0, [sp, #20]
	subs	r3, #8
	cmp	r0, r3
	bne.n	.L_0814942a
	ldr	r1, [sp, #28]
	movs	r3, #239
	lsls	r3, r3, #7
	adds	r2, r1, r3
	movs	r4, #238
	movs	r3, #3
	str	r3, [r2, #0]
	lsls	r4, r4, #7
	ldr	r3, [pc, #516]
	adds	r4, #132
	adds	r2, r1, r4
	str	r3, [r2, #0]
	mov	r5, r9
	ldr	r1, [r5, #24]
.L_0814942a:
	lsls	r3, r1, #1
	ldr	r7, [sp, #12]
	adds	r2, r3, r1
	adds	r3, r2, #1
	ldrb	r3, [r7, r3]
	ldr	r0, [sp, #20]
	subs	r3, #8
	cmp	r0, r3
	ble.n	.L_0814943e
	b.n	.L_08149656
.L_0814943e:
	movs	r3, #0
	mov	fp, r3
	ldrb	r3, [r7, r2]
	cmp	r3, #0
	bne.n	.L_0814944a
	b.n	.L_08149656
.L_0814944a:
	ldr	r4, [sp, #28]
	mov	sl, r4
.L_0814944e:
	mov	r5, sl
	ldr	r3, [r5, #8]
	cmp	r3, #1
	bne.n	.L_08149504
	mov	r0, fp
	lsls	r2, r0, #4
	lsls	r3, r0, #7
	ldr	r1, [pc, #456]
	subs	r3, r3, r2
	movs	r7, #0
	lsls	r3, r3, #2
	mov	r8, r7
	adds	r7, r3, r1
.L_08149468:
	movs	r1, #5
	mov	r0, r8
	bl	sub_08002064
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r1, #96
	ldr	r0, [r7, #24]
	bl	sub_08002054
	movs	r1, #3
	bl	sub_08002064
	movs	r2, #4
	mov	r3, r8
	adds	r6, r7, #0
	adds	r4, r5, r0
	mov	ip, r2
	cmp	r3, #2
	ble.n	.L_08149494
	movs	r5, #0
	mov	ip, r5
.L_08149494:
	ldr	r2, [pc, #404]
	lsls	r3, r4, #2
	ldr	r1, [r2, r3]
	ldr	r0, [sp, #28]
	movs	r2, #240
	adds	r1, r0, r1
	lsls	r2, r2, #4
	adds	r1, r1, r2
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	ldr	r3, [pc, #388]
	ldrb	r5, [r3, r4]
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	movs	r0, #6
	ldrsh	r3, [r6, r0]
	ldr	r0, [pc, #380]
	ldrb	r4, [r0, r4]
	str	r5, [sp, #0]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #16]
	mov	r5, ip
	ldr	r4, [r5, r0]
	ldr	r0, [sp, #24]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	lsls	r2, r2, #6
	adds	r0, r6, #0
	movs	r1, #64
	bl	sub_08138086
	ldr	r3, [r6, #24]
	ldr	r2, [r6, #8]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	cmp	r2, #1
	ble.n	.L_081494f2
	ldr	r1, [sp, #20]
	movs	r3, #1
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_081494f2
	subs	r3, r2, #1
	str	r3, [r6, #8]
.L_081494f2:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r7, #28
	cmp	r3, #16
	bne.n	.L_08149468
	mov	r4, r9
	ldr	r1, [r4, #24]
	b.n	.L_08149640
.L_08149504:
	mov	r5, sl
	ldr	r3, [r5, #24]
	ldr	r7, [sp, #20]
	cmp	r7, r3
	bge.n	.L_08149510
	b.n	.L_08149640
.L_08149510:
	mov	r3, sl
	movs	r1, #2
	ldrsh	r2, [r3, r1]
	movs	r1, #32
	movs	r7, #6
	ldrsh	r4, [r3, r7]
	str	r1, [sp, #0]
	movs	r1, #64
	str	r1, [sp, #4]
	ldr	r3, [sp, #28]
	ldr	r1, [sp, #16]
	movs	r5, #1
	mov	r6, fp
	ands	r6, r5
	movs	r7, #224
	mov	ip, r4
	lsls	r0, r6, #2
	lsls	r7, r7, #3
	ldr	r4, [r0, r1]
	subs	r2, #16
	adds	r1, r3, r7
	ldr	r0, [sp, #24]
	mov	r3, ip
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	mov	r0, sl
	lsls	r2, r2, #9
	movs	r1, #64
	bl	sub_08138086
	mov	r0, sl
	ldr	r3, [r0, #4]
	movs	r2, #224
	lsls	r2, r2, #14
	cmp	r3, r2
	ble.n	.L_08149610
	mov	r3, fp
	str	r2, [r0, #4]
	ldr	r4, [pc, #200]
	lsls	r2, r3, #4
	lsls	r3, r3, #7
	subs	r3, r3, r2
	ldr	r7, [pc, #208]
	movs	r1, #0
	lsls	r3, r3, #2
	str	r5, [r0, #8]
	mov	r8, r1
	adds	r5, r3, r4
	movs	r1, #127
.L_08149574:
	ldrb	r3, [r7, #0]
	mov	r0, sl
	ldr	r2, [r0, #0]
	subs	r3, #40
	lsls	r3, r3, #16
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldrb	r3, [r7, #1]
	str	r1, [sp, #8]
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08014878
	ldr	r1, [sp, #8]
	ands	r0, r1
	subs	r0, #64
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08014878
	ldr	r1, [sp, #8]
	ands	r0, r1
	negs	r0, r0
	lsls	r3, r0, #11
	str	r3, [r5, #16]
	cmp	r6, #0
	beq.n	.L_081495b4
	ldr	r3, [r5, #12]
	lsls	r3, r3, #1
	str	r3, [r5, #12]
	lsls	r3, r0, #12
	str	r3, [r5, #16]
.L_081495b4:
	movs	r3, #32
	movs	r2, #1
	str	r3, [r5, #8]
	add	r8, r2
	movs	r3, #0
	str	r3, [r5, #24]
	mov	r3, r8
	adds	r7, #2
	adds	r5, #28
	cmp	r3, #16
	bne.n	.L_08149574
	ldr	r4, [sp, #28]
	movs	r5, #238
	lsls	r5, r5, #7
	adds	r5, #168
	adds	r3, r4, r5
	movs	r2, #8
	str	r2, [r3, #0]
	movs	r0, #144
	bl	sub_081c0010
	mov	r0, r9
	ldr	r3, [r0, #20]
	movs	r7, #0
	mov	r8, r7
	cmp	r3, #0
	beq.n	.L_0814963c
	movs	r6, #4
	movs	r5, #36
.L_081495ee:
	mov	r1, r9
	ldrsh	r0, [r5, r1]
	mov	r3, r8
	movs	r1, #7
	movs	r2, #5
	str	r6, [sp, #0]
	bl	sub_0814cd48
	movs	r3, #1
	mov	r4, r9
	add	r8, r3
	ldr	r3, [r4, #20]
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L_081495ee
	ldr	r1, [r4, #24]
	b.n	.L_08149640
.L_08149610:
	mov	r5, r9
	ldr	r1, [r5, #24]
	b.n	.L_08149640
	movs	r0, r0
	.4byte 0x08143001
	.4byte 0x081979b7
	.4byte 0xffc00000
	.4byte 0x06060606
	.4byte 0x02010000
	.4byte 0x08197834
	.4byte 0x0819781a
	.4byte 0x08197826
	.2byte 0x77f8
	.2byte 0x0819
.L_0814963c:
	mov	r7, r9
	ldr	r1, [r7, #24]
.L_08149640:
	ldr	r4, [pc, #124]
	lsls	r3, r1, #1
	adds	r3, r3, r1
	ldrb	r3, [r4, r3]
	movs	r2, #1
	movs	r0, #28
	add	fp, r2
	add	sl, r0
	cmp	fp, r3
	beq.n	.L_08149656
	b.n	.L_0814944e
.L_08149656:
	lsls	r0, r1, #1
	lsls	r1, r1, #2
	adds	r1, #8
	adds	r0, #4
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r7, #240
	ldr	r5, [sp, #28]
	lsls	r7, r7, #7
	adds	r7, #232
	adds	r2, r5, r7
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #20]
	ldr	r1, [pc, #64]
	adds	r0, #1
	str	r1, [sp, #12]
	str	r0, [sp, #20]
	mov	r3, r9
	ldr	r2, [r3, #24]
	ldr	r4, [pc, #52]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, #1
	ldrb	r3, [r4, r3]
	adds	r1, r2, #0
	cmp	r0, r3
	beq.n	.L_0814969a
	b.n	.L_0814939a
.L_0814969a:
	ldr	r0, [pc, #40]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x081979b7
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	lsls	r5, r5, #10
	adds	r1, r5, r1
	adds	r0, r1, #0
	sub	sp, #128
	adds	r6, r2, #0
	mov	sl, r3
	bl	sub_08002096
	mov	r8, r0
	mov	r3, r8
	adds	r6, r5, r6
	lsls	r3, r3, #4
	asrs	r3, r3, #15
	adds	r0, r6, #0
	mov	r8, r3
	bl	sub_08002096
	add	r5, sl
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_08002096
	movs	r3, #0
	mov	r2, sp
	lsls	r6, r6, #4
	lsls	r0, r0, #4
	strh	r3, [r2, #0]
	asrs	r6, r6, #15
	asrs	r0, r0, #15
	mov	r7, r8
	mov	r5, sp
	movs	r4, #1
	adds	r7, #16
	adds	r6, #16
	adds	r0, #16
	adds	r5, #2
.L_0814971a:
	adds	r3, r4, #0
	muls	r3, r7
	cmp	r3, #0
	bge.n	.L_08149724
	adds	r3, #63
.L_08149724:
	adds	r2, r4, #0
	muls	r2, r6
	asrs	r1, r3, #6
	cmp	r2, #0
	bge.n	.L_08149730
	adds	r2, #63
.L_08149730:
	adds	r3, r4, #0
	muls	r3, r0
	asrs	r2, r2, #6
	cmp	r3, #0
	bge.n	.L_0814973c
	adds	r3, #63
.L_0814973c:
	asrs	r3, r3, #6
	cmp	r1, #0
	bge.n	.L_08149744
	movs	r1, #0
.L_08149744:
	cmp	r1, #31
	ble.n	.L_0814974a
	movs	r1, #31
.L_0814974a:
	cmp	r2, #0
	bge.n	.L_08149750
	movs	r2, #0
.L_08149750:
	cmp	r2, #31
	ble.n	.L_08149756
	movs	r2, #31
.L_08149756:
	cmp	r3, #0
	bge.n	.L_0814975c
	movs	r3, #0
.L_0814975c:
	cmp	r3, #31
	ble.n	.L_08149762
	movs	r3, #31
.L_08149762:
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r1
	adds	r4, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r4, #64
	bne.n	.L_0814971a
	movs	r0, #160
	lsls	r0, r0, #19
	mov	r1, sp
	ldr	r3, [pc, #20]
	movs	r2, #128
	adds	r0, #2
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xb020
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0730
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r5, #192
	lsls	r5, r5, #18
	mov	r9, r0
	ldr	r0, [r5, #92]
	sub	sp, #48
	str	r0, [sp, #36]
	movs	r0, #1
	ldr	r1, [r5, #96]
	movs	r7, #224
	str	r1, [sp, #32]
	lsls	r7, r7, #3
	ldr	r2, [r5, #100]
	str	r2, [sp, #24]
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r1, #3
	movs	r0, #104
	bl	sub_081963ec
	ldr	r3, [r5, #104]
	movs	r1, #23
	movs	r0, #188
	str	r3, [sp, #40]
	bl	sub_081963ec
	adds	r5, #188
	ldr	r3, [r5, #0]
	mov	r4, sp
	adds	r4, #40
	str	r4, [sp, #12]
	str	r3, [r4, #4]
	ldr	r5, [sp, #36]
	ldr	r0, [pc, #20]
	adds	r1, r5, r7
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r0, [pc, #12]
	movs	r3, #0
	b.n	.L_0814980c
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x00000194
	.2byte 0x0134
	.2byte 0x0000
.L_0814980c:
	ldr	r1, [sp, #24]
	movs	r2, #0
	bl	sub_08157cf4
	mov	r0, r9
	ldr	r3, [r0, #24]
	cmp	r3, #2
	beq.n	sub_08149830
	ldr	r0, [pc, #836]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #832]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4bce
	movs	r1, #0
	movs	r2, #128
	mov	r8, r1
	lsls	r2, r2, #3
.L_0814983a:
	movs	r4, #1
	add	r8, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_0814983a
	ldr	r5, [sp, #36]
	movs	r7, #239
	movs	r0, #238
	lsls	r7, r7, #7
	lsls	r0, r0, #7
	adds	r2, r5, r7
	movs	r3, #2
	adds	r0, #132
	str	r3, [r2, #0]
	movs	r1, #200
	adds	r2, r5, r0
	movs	r3, #50
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #780]
	bl	sub_080145a8
	mov	r1, r9
	ldr	r2, [r1, #24]
	ldr	r3, [pc, #772]
	movs	r7, #42
	ldrb	r3, [r3, r2]
	negs	r7, r7
	str	r3, [sp, #20]
	ldr	r4, [sp, #20]
	ldr	r5, [sp, #20]
	movs	r3, #0
	lsls	r4, r4, #3
	mov	fp, r3
	subs	r3, r4, r5
	str	r4, [sp, #8]
	cmp	r3, r7
	bne.n	.L_0814988a
	b.n	.L_08149b40
.L_0814988a:
	b.n	.L_08149890
.L_0814988c:
	mov	r0, r9
	ldr	r2, [r0, #24]
.L_08149890:
	cmp	r2, #2
	bne.n	.L_081498c8
	mov	r1, fp
	cmp	r1, #63
	bgt.n	.L_081498c8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #48]
	movs	r2, #128
	mov	r3, fp
	lsls	r2, r2, #1
	cmp	r3, #55
	ble.n	.L_081498b4
	mov	r4, fp
	movs	r3, #176
	lsls	r2, r4, #3
	lsls	r3, r3, #2
	subs	r2, r3, r2
.L_081498b4:
	mov	r5, r9
	ldr	r3, [r5, #4]
	cmp	r3, #1
	bne.n	.L_081498c2
	ldrh	r3, [r1, #54]
	subs	r3, r3, r2
	b.n	.L_081498c6
.L_081498c2:
	ldrh	r3, [r1, #54]
	adds	r3, r3, r2
.L_081498c6:
	strh	r3, [r1, #54]
.L_081498c8:
	mov	r7, fp
	cmp	r7, #24
	bne.n	.L_081498d4
	movs	r0, #134
	bl	sub_081180e8
.L_081498d4:
	ldr	r1, [sp, #20]
	movs	r0, #0
	str	r0, [sp, #28]
	cmp	r1, #0
	bne.n	.L_081498e0
	b.n	.L_08149a7e
.L_081498e0:
	ldr	r2, [sp, #28]
	lsls	r5, r2, #3
	cmp	fp, r5
	bne.n	sub_081498fc
	movs	r0, #134
	bl	sub_081c0010
	movs	r1, #128
	ldr	r3, [pc, #644]
	ldr	r0, [sp, #32]
	lsls	r1, r1, #7
	ldr	r2, [pc, #644]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1d2b
	mov	sl, r3
	cmp	fp, r5
	bge.n	.L_08149906
	b.n	.L_08149a34
.L_08149906:
	adds	r3, #5
	cmp	fp, r3
	blt.n	.L_0814990e
	b.n	.L_08149a34
.L_0814990e:
	adds	r3, r5, #1
	adds	r6, r5, #2
	cmp	fp, r3
	blt.n	sub_08149946
	cmp	fp, r6
	bge.n	.L_0814994e
	mov	r7, r9
	ldr	r2, [r7, #4]
	ldr	r0, [sp, #28]
	lsls	r3, r2, #3
	ldr	r1, [pc, #604]
	subs	r3, r3, r2
	adds	r3, r0, r3
	ldrb	r2, [r1, r3]
	movs	r3, #48
	str	r3, [sp, #0]
	movs	r3, #112
	str	r3, [sp, #4]
	ldr	r3, [sp, #36]
	movs	r7, #224
	lsls	r7, r7, #3
	adds	r1, r3, r7
	subs	r2, #24
	ldr	r4, [sp, #40]
	ldr	r0, [sp, #32]
	movs	r3, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1d28
	mov	sl, r0
	cmp	fp, r6
	blt.n	.L_08149a34
.L_0814994e:
	adds	r5, #4
	mov	sl, r5
	cmp	fp, sl
	bge.n	sub_08149982
	mov	r1, r9
	ldr	r2, [r1, #4]
	ldr	r4, [pc, #548]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r2, [sp, #28]
	ldr	r5, [sp, #36]
	adds	r3, r2, r3
	ldrb	r2, [r4, r3]
	movs	r7, #224
	movs	r3, #48
	str	r3, [sp, #0]
	lsls	r7, r7, #5
	movs	r3, #112
	str	r3, [sp, #4]
	subs	r2, #24
	ldr	r4, [sp, #40]
	ldr	r0, [sp, #32]
	adds	r1, r5, r7
	movs	r3, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x45b3
	bne.n	.L_08149a34
	movs	r0, #0
	str	r0, [sp, #16]
	ldr	r7, [pc, #504]
	mov	r8, r0
.L_0814998e:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	bne.n	.L_08149a0e
	bl	sub_08014878
	movs	r6, #192
	lsls	r6, r6, #2
	adds	r6, #255
	ands	r6, r0
	bl	sub_08014878
	mov	r3, r9
	ldr	r2, [r3, #4]
	movs	r5, #254
	lsls	r5, r5, #7
	ldr	r4, [sp, #28]
	adds	r5, #255
	lsls	r3, r2, #3
	ands	r5, r0
	ldr	r0, [pc, #456]
	subs	r3, r3, r2
	adds	r3, r4, r3
	ldrb	r3, [r0, r3]
	ldr	r1, [pc, #456]
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	movs	r3, #208
	adds	r5, r5, r1
	lsls	r3, r3, #15
	str	r3, [r7, #4]
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r6, #32
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #7
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	adds	r3, #32
	str	r3, [r7, #24]
	ldr	r1, [sp, #16]
	mov	r3, r9
	adds	r1, #1
	str	r1, [sp, #16]
	ldr	r1, [pc, #392]
	ldr	r2, [r3, #24]
	ldr	r4, [sp, #16]
	lsls	r3, r2, #2
	ldrh	r3, [r1, r3]
	cmp	r4, r3
	beq.n	.L_08149a22
.L_08149a0e:
	movs	r5, #1
	movs	r0, #128
	add	r8, r5
	lsls	r0, r0, #3
	adds	r7, #28
	cmp	r8, r0
	bne.n	.L_0814998e
	mov	r1, r9
	ldr	r2, [r1, #24]
	ldr	r1, [pc, #360]
.L_08149a22:
	lsls	r2, r2, #2
	ldr	r4, [sp, #36]
	movs	r5, #238
	adds	r2, #2
	lsls	r5, r5, #7
	ldrh	r2, [r1, r2]
	adds	r5, #168
	adds	r3, r4, r5
	str	r2, [r3, #0]
.L_08149a34:
	cmp	fp, sl
	bne.n	.L_08149a70
	mov	r0, r9
	ldr	r3, [r0, #20]
	movs	r7, #0
	mov	r8, r7
	cmp	r3, #0
	beq.n	.L_08149a70
	movs	r5, #36
.L_08149a46:
	mov	r1, r9
	ldrsh	r0, [r5, r1]
	movs	r1, #1
	bl	sub_08118088
	mov	r3, r9
	ldrsh	r0, [r5, r3]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #7
	mov	r3, r8
	movs	r2, #5
	bl	sub_0814cd48
	mov	r0, r9
	ldr	r3, [r0, #20]
	movs	r7, #1
	add	r8, r7
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L_08149a46
.L_08149a70:
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #20]
	adds	r1, #1
	str	r1, [sp, #28]
	cmp	r1, r2
	beq.n	.L_08149a7e
	b.n	.L_081498e0
.L_08149a7e:
	ldr	r6, [pc, #260]
	movs	r3, #0
	mov	r8, r3
.L_08149a84:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	ble.n	sub_08149b00
	subs	r3, #1
	movs	r2, #128
	lsls	r2, r2, #5
	str	r3, [r6, #24]
	adds	r0, r6, #0
	movs	r1, #60
	bl	sub_08138086
	ldr	r2, [r6, #4]
	movs	r4, #208
	lsls	r4, r4, #15
	cmp	r2, r4
	ble.n	.L_08149ab2
	ldr	r3, [r6, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #16]
	b.n	sub_08149b00
.L_08149ab2:
	ldr	r3, [r6, #0]
	asrs	r7, r3, #16
	cmp	r3, #0
	blt.n	sub_08149b00
	cmp	r7, #119
	bgt.n	sub_08149b00
	cmp	r2, #0
	blt.n	sub_08149b00
	ldr	r0, [r6, #24]
	asrs	r2, r2, #16
	mov	ip, r2
	cmp	r0, #0
	bge.n	.L_08149ace
	adds	r0, #7
.L_08149ace:
	asrs	r0, r0, #3
	adds	r0, #1
	mov	r5, r8
	ldr	r2, [pc, #184]
	movs	r4, #1
	ands	r4, r5
	lsls	r5, r0, #1
	subs	r3, r5, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #24]
	str	r0, [sp, #0]
	adds	r1, r2, r1
	lsrs	r2, r0, #31
	adds	r2, r0, r2
	asrs	r2, r2, #1
	subs	r2, r7, r2
	mov	r7, ip
	subs	r3, r7, r0
	str	r5, [sp, #4]
	ldr	r0, [sp, #12]
	lsls	r4, r4, #2
	ldr	r4, [r4, r0]
	ldr	r0, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2101
	movs	r2, #128
	add	r8, r1
	lsls	r2, r2, #3
	adds	r6, #28
	cmp	r8, r2
	bne.n	.L_08149a84
	movs	r0, #8
	movs	r1, #16
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r4, #240
	ldr	r3, [sp, #36]
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r7, [sp, #8]
	ldr	r0, [sp, #20]
	movs	r5, #1
	subs	r3, r7, r0
	add	fp, r5
	adds	r3, #42
	cmp	fp, r3
	beq.n	.L_08149b40
	b.n	.L_0814988c
.L_08149b40:
	ldr	r0, [pc, #44]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x00000122
	.4byte 0x03000730
	.4byte 0x02010018
	.4byte 0x08143001
	.4byte 0x081979da
	.4byte 0x03000260
	.4byte 0x10101010
	.4byte 0x081979cc
	.4byte 0x02010000
	.4byte 0xffffc000
	.4byte 0x081979c0
	.2byte 0x7410
	.2byte 0x0819
	push	{lr}
	movs	r1, #0
	bl	sub_08149bac
	pop	{pc}
	.align 2, 0
