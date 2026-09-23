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
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138086, 0x08138086
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_08149cb4, 0x08149cb4
	.set sub_08149eae, 0x08149eae
	.set sub_08149f9c, 0x08149f9c
	.set sub_0814a2fe, 0x0814a2fe
	.set sub_0814a53c, 0x0814a53c
	.set sub_0814a734, 0x0814a734
	.set sub_0814a814, 0x0814a814
	.set sub_0814cd48, 0x0814cd48
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e20c, 0x0815e20c
	.set sub_081963ec, 0x081963ec
	.set sub_081c0010, 0x081c0010
	.global Func_08149bac
	.thumb_func
Func_08149bac:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #76
	str	r1, [sp, #52]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #92]
	mov	sl, r0
	str	r1, [sp, #48]
	ldr	r2, [r3, #96]
	str	r2, [sp, #44]
	ldr	r3, [r3, #100]
	str	r3, [sp, #32]
	ldr	r3, [sp, #52]
	cmp	r3, #0
	bne.n	.L_08149be6
	movs	r0, #1
	bl	sub_081435e0
	movs	r5, #48
	movs	r4, #60
	str	r4, [sp, #28]
	str	r5, [sp, #24]
	b.n	.L_08149c38
.L_08149be6:
	ldr	r1, [sp, #52]
	cmp	r1, #1
	bne.n	.L_08149c16
	movs	r0, #0
	bl	sub_081435e0
	movs	r5, #238
	ldr	r4, [sp, #48]
	lsls	r5, r5, #7
	movs	r1, #238
	movs	r2, #60
	movs	r3, #64
	adds	r5, #180
	lsls	r1, r1, #7
	str	r2, [sp, #28]
	str	r3, [sp, #24]
	adds	r2, r4, r5
	movs	r3, #24
	adds	r1, #184
	str	r3, [r2, #0]
	adds	r2, r4, r1
	movs	r3, #0
	str	r3, [r2, #0]
	b.n	.L_08149c38
.L_08149c16:
	movs	r0, #0
	bl	sub_081435e0
	mov	r2, sl
	add	r5, sp, #64
	ldr	r0, [r2, #8]
	adds	r1, r5, #0
	bl	sub_0815e20c
	ldr	r3, [r5, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [sp, #28]
	ldr	r3, [r5, #4]
	adds	r3, #48
	str	r3, [sp, #24]
.L_08149c38:
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	mov	r3, sp
	adds	r3, #56
	adds	r1, r3, #0
	movs	r0, #0
	str	r3, [sp, #20]
	bl	sub_08144aac
	ldr	r4, [sp, #48]
	movs	r5, #224
	lsls	r5, r5, #3
	adds	r1, r4, r5
	ldr	r0, [pc, #28]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r0, [pc, #24]
	ldr	r1, [sp, #32]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r1, [sp, #52]
	b.n	.L_08149c80
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x0000013e
	.2byte 0x0134
	.2byte 0x0000
.L_08149c80:
	cmp	r1, #1
	bne.n	.L_08149c9a
	ldr	r0, [pc, #868]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #864]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe00c
.L_08149c9a:
	ldr	r2, [sp, #52]
	cmp	r2, #2
	bne.n	sub_08149cb4
	ldr	r0, [pc, #848]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #836]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2300
	str	r3, [sp, #40]
	mov	r4, sl
	ldr	r3, [r4, #24]
	ldr	r1, [pc, #824]
	lsls	r2, r3, #2
	adds	r2, r2, r3
	lsls	r2, r2, #1
	adds	r2, #2
	ldrh	r3, [r1, r2]
	cmp	r3, #0
	bne.n	.L_08149cce
	b.n	.L_08149e1e
.L_08149cce:
	movs	r5, #0
	str	r5, [sp, #8]
.L_08149cd2:
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #48]
	movs	r1, #0
	mov	r9, r1
	adds	r7, r2, r3
.L_08149cdc:
	mov	r4, r9
	lsls	r6, r4, #1
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
	adds	r0, r5, #0
	str	r3, [r7, #0]
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	mov	r5, r9
	negs	r3, r3
	str	r3, [r7, #4]
	lsrs	r3, r5, #31
	add	r3, r9
	movs	r1, #1
	asrs	r3, r3, #1
	add	r9, r1
	adds	r3, #25
	mov	r2, r9
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r2, #16
	bne.n	.L_08149cdc
	mov	r4, sl
	ldr	r1, [r4, #24]
	movs	r3, #0
	ldr	r0, [pc, #720]
	mov	r9, r3
	lsls	r3, r1, #2
	adds	r3, r3, r1
	lsls	r3, r3, #1
	ldrh	r3, [r0, r3]
	adds	r7, r0, #0
	adds	r2, r1, #0
	cmp	r3, #0
	beq.n	.L_08149dfe
	ldr	r5, [sp, #24]
	lsls	r5, r5, #16
	mov	fp, r5
.L_08149d40:
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #1
	ldrh	r3, [r7, r3]
	ldr	r1, [sp, #40]
	adds	r2, r3, #0
	muls	r2, r1
	add	r2, r9
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r2, [pc, #676]
	lsls	r3, r3, #2
	adds	r5, r3, r2
	bl	sub_08014878
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	ands	r3, r0
	adds	r3, #32
	mov	r8, r3
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r6, r0, #0
	mov	r4, sl
	ands	r6, r3
	ldr	r3, [r4, #4]
	cmp	r3, #1
	bne.n	.L_08149d98
	ldr	r3, [r4, #24]
	ldr	r1, [sp, #40]
	lsls	r2, r3, #2
	adds	r2, r2, r3
	adds	r2, r2, r1
	lsls	r2, r2, #1
	adds	r2, #4
	ldrh	r3, [r7, r2]
	ldr	r2, [sp, #28]
	subs	r3, r2, r3
	adds	r3, #28
	b.n	.L_08149db0
.L_08149d98:
	mov	r4, sl
	ldr	r3, [r4, #24]
	ldr	r1, [sp, #40]
	lsls	r2, r3, #2
	adds	r2, r2, r3
	adds	r2, r2, r1
	lsls	r2, r2, #1
	adds	r2, #4
	ldrh	r3, [r7, r2]
	ldr	r2, [sp, #28]
	adds	r3, r2, r3
	subs	r3, #28
.L_08149db0:
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	mov	r3, fp
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
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #6
	str	r3, [r5, #16]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	adds	r3, #32
	str	r3, [r5, #24]
	mov	r5, sl
	ldr	r1, [r5, #24]
	ldr	r0, [pc, #524]
	lsls	r3, r1, #2
	adds	r3, r3, r1
	lsls	r3, r3, #1
	ldrh	r3, [r0, r3]
	movs	r4, #1
	add	r9, r4
	adds	r7, r0, #0
	adds	r2, r1, #0
	cmp	r9, r3
	bne.n	.L_08149d40
.L_08149dfe:
	ldr	r2, [sp, #8]
	ldr	r4, [sp, #40]
	movs	r3, #224
	lsls	r3, r3, #1
	adds	r2, r2, r3
	adds	r4, #1
	lsls	r3, r1, #2
	str	r2, [sp, #8]
	str	r4, [sp, #40]
	adds	r3, r3, r1
	lsls	r3, r3, #1
	adds	r3, #2
	ldrh	r3, [r0, r3]
	cmp	r4, r3
	beq.n	.L_08149e1e
	b.n	.L_08149cd2
.L_08149e1e:
	ldr	r5, [sp, #48]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r5, r1
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r5, r3
	movs	r1, #200
	movs	r3, #75
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #452]
	bl	sub_080145a8
	movs	r4, #0
	str	r4, [sp, #36]
	mov	r5, sl
	ldr	r1, [r5, #24]
	ldr	r2, [pc, #428]
	adds	r0, r1, #0
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	adds	r3, #2
	ldrh	r3, [r2, r3]
	ldr	r4, [pc, #428]
	cmp	r3, r4
	bne.n	.L_08149e5e
	b.n	.L_0814a222
.L_08149e5e:
	ldr	r5, [sp, #52]
	subs	r5, #1
	str	r5, [sp, #16]
.L_08149e64:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #48]
	cmp	r0, #2
	bne.n	.L_08149e96
	ldr	r3, [sp, #36]
	cmp	r3, #51
	bgt.n	.L_08149e96
	mov	r4, sl
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_08149e8a
	ldrh	r3, [r2, #54]
	movs	r5, #128
	lsls	r5, r5, #1
	adds	r3, r3, r5
	strh	r3, [r2, #54]
	ldr	r1, [r4, #24]
	b.n	.L_08149e96
.L_08149e8a:
	ldrh	r3, [r2, #54]
	ldr	r1, [pc, #376]
	adds	r3, r3, r1
	strh	r3, [r2, #54]
	mov	r2, sl
	ldr	r1, [r2, #24]
.L_08149e96:
	cmp	r1, #3
	bne.n	sub_08149eae
	ldr	r3, [sp, #36]
	cmp	r3, #4
	bne.n	sub_08149eae
	movs	r1, #128
	ldr	r3, [pc, #360]
	ldr	r0, [sp, #44]
	lsls	r1, r1, #7
	ldr	r2, [pc, #356]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9c04
	cmp	r4, #1
	bhi.n	.L_08149ec8
	ldr	r5, [sp, #36]
	cmp	r5, #2
	bne.n	.L_08149ee0
	movs	r0, #145
	bl	sub_081c0010
	movs	r0, #145
	bl	sub_081180e8
	b.n	.L_08149ee0
.L_08149ec8:
	ldr	r1, [sp, #36]
	cmp	r1, #2
	bne.n	.L_08149ed4
	movs	r0, #145
	bl	sub_081c0010
.L_08149ed4:
	ldr	r2, [sp, #36]
	cmp	r2, #24
	bne.n	.L_08149ee0
	movs	r0, #134
	bl	sub_081180e8
.L_08149ee0:
	movs	r3, #0
	str	r3, [sp, #40]
	mov	r4, sl
	ldr	r3, [r4, #24]
	ldr	r5, [pc, #268]
	lsls	r2, r3, #2
	adds	r2, r2, r3
	lsls	r2, r2, #1
	adds	r2, #2
	ldrh	r3, [r5, r2]
	cmp	r3, #0
	bne.n	.L_08149efa
	b.n	.L_0814a1e0
.L_08149efa:
	movs	r1, #0
	str	r1, [sp, #12]
.L_08149efe:
	ldr	r2, [sp, #40]
	ldr	r3, [sp, #36]
	lsls	r2, r2, #3
	mov	fp, r2
	cmp	r3, fp
	bne.n	.L_08149f18
	ldr	r4, [sp, #48]
	movs	r5, #238
	lsls	r5, r5, #7
	adds	r5, #168
	adds	r2, r4, r5
	movs	r3, #12
	str	r3, [r2, #0]
.L_08149f18:
	ldr	r1, [sp, #36]
	cmp	r1, fp
	bge.n	.L_08149f20
	b.n	.L_0814a086
.L_08149f20:
	mov	r3, fp
	adds	r3, #2
	cmp	r1, r3
	bge.n	sub_08149f9c
	mov	r2, sl
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L_08149f66
	ldr	r2, [r2, #24]
	ldr	r4, [sp, #40]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	adds	r3, r3, r4
	ldr	r5, [pc, #188]
	lsls	r3, r3, #1
	adds	r3, #4
	ldrh	r2, [r5, r3]
	movs	r3, #32
	str	r3, [sp, #0]
	movs	r3, #64
	ldr	r1, [sp, #28]
	str	r3, [sp, #4]
	ldr	r3, [sp, #48]
	movs	r5, #224
	lsls	r5, r5, #3
	subs	r2, r1, r2
	adds	r1, r3, r5
	ldr	r3, [sp, #24]
	adds	r2, #12
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #44]
	subs	r3, #32
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe01a
.L_08149f66:
	mov	r1, sl
	ldr	r2, [r1, #24]
	ldr	r4, [pc, #140]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	ldr	r2, [sp, #40]
	ldr	r5, [sp, #28]
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r3, #4
	ldrh	r2, [r4, r3]
	movs	r3, #32
	str	r3, [sp, #0]
	movs	r3, #64
	str	r3, [sp, #4]
	ldr	r3, [sp, #48]
	adds	r2, r5, r2
	movs	r5, #224
	lsls	r5, r5, #3
	adds	r1, r3, r5
	ldr	r3, [sp, #24]
	subs	r2, #44
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #44]
	subs	r3, #32
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9909
	cmp	r1, fp
	blt.n	.L_0814a086
	ldr	r3, [pc, #84]
	ldr	r4, [sp, #12]
	mov	r8, r3
	ldr	r1, [sp, #48]
	lsls	r3, r4, #3
	subs	r3, r3, r4
	movs	r2, #0
	lsls	r3, r3, #2
	mov	r9, r2
	adds	r5, r3, r1
.L_08149fb6:
	movs	r2, #6
	ldrsh	r3, [r5, r2]
	ldr	r4, [sp, #24]
	mov	r1, sl
	adds	r7, r3, r4
	ldr	r3, [r1, #4]
	cmp	r3, #1
	bne.n	.L_0814a014
	mov	r4, sl
	movs	r2, #2
	ldrsh	r1, [r5, r2]
	ldr	r3, [sp, #28]
	ldr	r2, [r4, #24]
	adds	r1, r1, r3
	lsls	r3, r2, #2
	adds	r3, r3, r2
	ldr	r2, [sp, #40]
	mov	r4, r8
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r3, #4
	ldrh	r3, [r4, r3]
	subs	r1, r1, r3
	adds	r6, r1, #0
	adds	r6, #28
	b.n	.L_0814a036
	movs	r0, r0
	.4byte 0x00000148
	.4byte 0x03000730
	.4byte 0x00000188
	.4byte 0x081979de
	.4byte 0x02010000
	.4byte 0x08143001
	.4byte 0x1ffffffb
	.4byte 0xffffff00
	.4byte 0x03000260
	.2byte 0x3f3f
	.2byte 0x3f3f
.L_0814a014:
	mov	r4, sl
	movs	r2, #2
	ldrsh	r1, [r5, r2]
	ldr	r3, [sp, #28]
	ldr	r2, [r4, #24]
	adds	r1, r1, r3
	lsls	r3, r2, #2
	adds	r3, r3, r2
	ldr	r2, [sp, #40]
	mov	r4, r8
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r3, #4
	ldrh	r3, [r4, r3]
	adds	r1, r1, r3
	adds	r6, r1, #0
	subs	r6, #28
.L_0814a036:
	ldr	r0, [r5, #24]
	cmp	r0, #17
	bhi.n	.L_0814a06c
	movs	r1, #3
	bl	sub_08002054
	ldr	r2, [pc, #516]
	ldr	r3, [sp, #48]
	ldrb	r1, [r2, r0]
	movs	r4, #224
	movs	r0, #32
	lsls	r1, r1, #11
	adds	r1, r3, r1
	lsls	r4, r4, #3
	str	r0, [sp, #0]
	adds	r2, r6, #0
	movs	r0, #64
	adds	r3, r7, #0
	adds	r1, r1, r4
	str	r0, [sp, #4]
	subs	r2, #16
	subs	r3, #32
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #44]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69a8
.L_0814a06c:
	cmp	r0, #0
	ble.n	.L_0814a074
	subs	r3, r0, #1
	b.n	.L_0814a078
.L_0814a074:
	movs	r3, #1
	negs	r3, r3
.L_0814a078:
	str	r3, [r5, #24]
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	adds	r5, #28
	cmp	r2, #12
	bne.n	.L_08149fb6
.L_0814a086:
	ldr	r4, [sp, #36]
	mov	r3, fp
	adds	r3, #5
	cmp	r4, r3
	ble.n	.L_0814a17c
	ldr	r5, [sp, #52]
	ldr	r7, [pc, #440]
	cmp	r5, #2
	beq.n	.L_0814a09c
	movs	r7, #128
	lsls	r7, r7, #5
.L_0814a09c:
	movs	r1, #0
	mov	r2, sl
	mov	r9, r1
	ldr	r1, [r2, #24]
	ldr	r0, [pc, #424]
	lsls	r3, r1, #2
	adds	r3, r3, r1
	adds	r4, r0, #0
	lsls	r3, r3, #1
	ldrh	r3, [r4, r3]
	adds	r2, r1, #0
	cmp	r3, #0
	beq.n	.L_0814a17c
.L_0814a0b6:
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #1
	ldrh	r3, [r4, r3]
	ldr	r4, [sp, #40]
	ldr	r5, [pc, #400]
	adds	r2, r3, #0
	muls	r2, r4
	add	r2, r9
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r6, r3, r5
	ldr	r3, [r6, #24]
	cmp	r3, #0
	ble.n	.L_0814a168
	movs	r1, #60
	adds	r2, r7, #0
	adds	r0, r6, #0
	bl	sub_08138086
	ldr	r3, [r6, #24]
	ldr	r2, [r6, #4]
	movs	r1, #216
	subs	r3, #1
	lsls	r1, r1, #15
	str	r3, [r6, #24]
	cmp	r2, r1
	ble.n	.L_0814a104
	ldr	r3, [r6, #16]
	ldr	r0, [pc, #348]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #16]
	mov	r2, sl
	ldr	r1, [r2, #24]
	b.n	.L_0814a168
.L_0814a104:
	ldr	r0, [r6, #0]
	ldr	r4, [pc, #336]
	cmp	r0, r4
	bhi.n	.L_0814a15a
	cmp	r2, #0
	blt.n	.L_0814a162
	asrs	r2, r2, #16
	asrs	r6, r0, #16
	movs	r1, #5
	adds	r0, r3, #0
	mov	r8, r2
	bl	sub_08002054
	adds	r0, #1
	ldr	r2, [pc, #312]
	mov	r5, r9
	movs	r4, #1
	ands	r4, r5
	lsls	r5, r0, #1
	subs	r3, r5, #2
	ldrh	r1, [r2, r3]
	lsrs	r3, r0, #31
	ldr	r2, [sp, #32]
	str	r0, [sp, #0]
	str	r5, [sp, #4]
	adds	r3, r0, r3
	ldr	r5, [sp, #20]
	asrs	r3, r3, #1
	subs	r6, r6, r3
	lsls	r4, r4, #2
	mov	r3, r8
	adds	r1, r2, r1
	subs	r3, r3, r0
	adds	r2, r6, #0
	ldr	r4, [r4, r5]
	ldr	r0, [sp, #44]
	mov	r8, r3
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4652
	ldr	r0, [pc, #248]
	ldr	r1, [r2, #24]
	b.n	.L_0814a168
.L_0814a15a:
	mov	r3, sl
	ldr	r0, [pc, #240]
	ldr	r1, [r3, #24]
	b.n	.L_0814a168
.L_0814a162:
	mov	r4, sl
	ldr	r0, [pc, #232]
	ldr	r1, [r4, #24]
.L_0814a168:
	lsls	r3, r1, #2
	adds	r3, r3, r1
	adds	r4, r0, #0
	lsls	r3, r3, #1
	ldrh	r3, [r4, r3]
	movs	r5, #1
	add	r9, r5
	adds	r2, r1, #0
	cmp	r9, r3
	bne.n	.L_0814a0b6
.L_0814a17c:
	mov	r2, sl
	ldr	r3, [r2, #20]
	movs	r1, #0
	mov	r9, r1
	cmp	r3, #0
	beq.n	.L_0814a1be
	mov	r6, fp
	adds	r6, #6
	movs	r5, #36
.L_0814a18e:
	ldr	r4, [sp, #36]
	cmp	r4, r6
	bne.n	.L_0814a1b4
	mov	r1, sl
	movs	r3, #10
	ldrsh	r0, [r5, r1]
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	mov	r3, r9
	bl	sub_0814cd48
	mov	r3, sl
	ldrsh	r0, [r5, r3]
	movs	r1, #4
	bl	sub_08118088
	mov	r1, sl
	ldr	r3, [r1, #20]
.L_0814a1b4:
	movs	r2, #1
	add	r9, r2
	adds	r5, #2
	cmp	r9, r3
	bne.n	.L_0814a18e
.L_0814a1be:
	ldr	r3, [sp, #12]
	ldr	r4, [sp, #40]
	adds	r3, #16
	adds	r4, #1
	str	r3, [sp, #12]
	str	r4, [sp, #40]
	mov	r5, sl
	ldr	r3, [r5, #24]
	ldr	r1, [pc, #128]
	lsls	r2, r3, #2
	adds	r2, r2, r3
	lsls	r2, r2, #1
	adds	r2, #2
	ldrh	r3, [r1, r2]
	cmp	r4, r3
	beq.n	.L_0814a1e0
	b.n	.L_08149efe
.L_0814a1e0:
	movs	r1, #16
	movs	r0, #16
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r4, #240
	ldr	r3, [sp, #48]
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r5, [sp, #36]
	mov	r1, sl
	adds	r5, #1
	str	r5, [sp, #36]
	ldr	r2, [pc, #68]
	ldr	r0, [r1, #24]
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	adds	r3, #2
	ldrh	r3, [r2, r3]
	adds	r1, r0, #0
	lsls	r3, r3, #3
	adds	r3, #40
	cmp	r5, r3
	beq.n	.L_0814a222
	b.n	.L_08149e64
.L_0814a222:
	ldr	r0, [pc, #60]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #76
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x08197a1a
	.4byte 0xfffff000
	.4byte 0x081979de
	.4byte 0x02010000
	.4byte 0x007effff
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
	sub	sp, #72
	str	r0, [sp, #48]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	str	r0, [sp, #44]
	movs	r0, #1
	ldr	r1, [r3, #96]
	str	r1, [sp, #40]
	ldr	r3, [r3, #100]
	str	r3, [sp, #28]
	bl	sub_081435e0
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r2, [sp, #44]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #44]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r3, #0
	ldr	r0, [pc, #36]
	ldr	r1, [sp, #28]
	movs	r2, #0
	bl	sub_08157cf4
	ldr	r5, [sp, #48]
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L_0814a2e6
	ldr	r0, [pc, #20]
	bl	sub_08013300
	adds	r1, r0, #0
	b.n	.L_0814a2d8
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x00000146
	.4byte 0x00000134
	.2byte 0x0147
	.2byte 0x0000
.L_0814a2d8:
	movs	r0, #160
	ldr	r3, [pc, #760]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe00b
.L_0814a2e6:
	cmp	r3, #2
	bne.n	sub_0814a2fe
	ldr	r0, [pc, #748]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #732]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2113
	movs	r0, #104
	bl	sub_081963ec
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r3, [r5, #104]
	movs	r1, #23
	movs	r0, #188
	str	r3, [sp, #52]
	bl	sub_081963ec
	adds	r5, #188
	ldr	r3, [r5, #0]
	mov	r7, sp
	adds	r7, #52
	str	r7, [sp, #24]
	str	r3, [r7, #4]
	ldr	r3, [pc, #696]
	movs	r0, #0
	movs	r2, #128
	mov	r8, r0
	movs	r1, #0
	lsls	r2, r2, #3
.L_0814a32e:
	movs	r5, #1
	add	r8, r5
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_0814a32e
	ldr	r1, [sp, #48]
	movs	r7, #36
	ldrsh	r0, [r1, r7]
	bl	sub_08118098
	ldr	r7, [pc, #664]
	ldr	r6, [r0, #0]
	ldr	r5, [sp, #44]
	movs	r2, #0
	mov	r8, r2
.L_0814a34e:
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #72
	lsls	r2, r3, #16
	movs	r3, #0
	str	r2, [r5, #0]
	str	r3, [r5, #4]
	ldr	r0, [sp, #48]
	ldr	r3, [r0, #24]
	lsls	r3, r3, #2
	add	r3, r8
	ldrsb	r3, [r7, r3]
	lsls	r3, r3, #16
	str	r3, [r5, #8]
	ldr	r3, [r6, #8]
	cmp	r3, #0
	bge.n	.L_0814a378
	negs	r3, r2
	str	r3, [r5, #0]
.L_0814a378:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r5, #28
	cmp	r2, #4
	bne.n	.L_0814a34e
	ldr	r3, [sp, #44]
	movs	r5, #239
	lsls	r5, r5, #7
	adds	r2, r3, r5
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r7, [sp, #44]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #132
	adds	r2, r7, r0
	movs	r3, #50
	movs	r1, #200
	lsls	r1, r1, #4
	str	r3, [r2, #0]
	ldr	r0, [pc, #576]
	bl	sub_080145a8
	movs	r1, #0
	str	r1, [sp, #36]
.L_0814a3ac:
	ldr	r2, [sp, #48]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #48]
	ldr	r3, [r2, #24]
	cmp	r3, #2
	bne.n	.L_0814a3d2
	ldr	r3, [sp, #36]
	cmp	r3, #63
	bgt.n	.L_0814a3d2
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_0814a3cc
	ldrh	r3, [r5, #54]
	adds	r3, #192
	b.n	.L_0814a3d0
.L_0814a3cc:
	ldrh	r3, [r5, #54]
	subs	r3, #192
.L_0814a3d0:
	strh	r3, [r5, #54]
.L_0814a3d2:
	ldr	r7, [sp, #36]
	cmp	r7, #16
	bne.n	.L_0814a3de
	movs	r0, #134
	bl	sub_081180e8
.L_0814a3de:
	bl	sub_08014de4
	adds	r1, r5, #0
	adds	r0, r5, #0
	adds	r1, #12
	bl	sub_080156e8
	ldr	r0, [sp, #36]
	cmp	r0, #63
	ble.n	.L_0814a3f4
	b.n	.L_0814a686
.L_0814a3f4:
	movs	r1, #0
	str	r1, [sp, #32]
	ldr	r5, [sp, #48]
	ldr	r3, [pc, #492]
	ldr	r2, [r5, #24]
	ldrb	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_0814a406
	b.n	.L_0814a686
.L_0814a406:
	ldr	r3, [sp, #44]
	ldr	r2, [pc, #480]
	movs	r5, #224
	movs	r1, #1
	lsls	r5, r5, #3
	movs	r7, #60
	ands	r0, r1
	adds	r5, r3, r5
	add	r7, sp
	str	r0, [sp, #16]
	str	r2, [sp, #12]
	str	r3, [sp, #8]
	str	r5, [sp, #20]
	mov	r9, r7
	mov	fp, r9
.L_0814a424:
	mov	r1, r9
	ldr	r0, [sp, #8]
	bl	sub_0815e1ec
	mov	r7, r9
	ldr	r3, [r7, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r7, #0]
	ldr	r3, [r7, #4]
	subs	r3, #8
	str	r3, [r7, #4]
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #36]
	ldrb	r3, [r0, #0]
	cmp	r1, r3
	bne.n	.L_0814a452
	movs	r0, #145
	bl	sub_081c0010
	ldr	r2, [sp, #12]
	ldrb	r3, [r2, #0]
.L_0814a452:
	ldr	r5, [sp, #36]
	adds	r3, #4
	cmp	r5, r3
	blt.n	sub_0814a53c
	ldr	r7, [sp, #32]
	lsls	r0, r5, #4
	lsls	r3, r7, #1
	adds	r3, r3, r7
	lsls	r3, r3, #3
	adds	r3, r3, r7
	adds	r0, r0, r3
	movs	r1, #104
	bl	sub_08002064
	ldr	r5, [sp, #32]
	mov	r8, r0
	mov	r3, fp
	movs	r0, #1
	ldr	r2, [r3, #0]
	ldr	r1, [sp, #24]
	ldr	r3, [r3, #4]
	ands	r5, r0
	movs	r6, #34
	movs	r0, #104
	str	r0, [sp, #4]
	str	r6, [sp, #0]
	mov	r7, r8
	lsls	r5, r5, #2
	adds	r5, r5, r1
	subs	r3, r3, r7
	ldr	r1, [sp, #20]
	subs	r2, #17
	subs	r3, #104
	ldr	r4, [r5, #0]
	ldr	r0, [sp, #40]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4659
	ldr	r2, [r1, #0]
	ldr	r3, [r1, #4]
	str	r6, [sp, #0]
	str	r7, [sp, #4]
	subs	r2, #17
	subs	r3, r3, r7
	ldr	r4, [r5, #0]
	ldr	r0, [sp, #40]
	ldr	r1, [sp, #20]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9a04
	cmp	r2, #0
	beq.n	.L_0814a4fc
	ldr	r3, [sp, #44]
	movs	r7, #160
	lsls	r7, r7, #5
	mov	r0, fp
	adds	r7, #208
	ldr	r2, [r0, #0]
	adds	r5, r3, r7
	ldr	r3, [r0, #4]
	movs	r7, #37
	movs	r1, #20
	subs	r2, #20
	subs	r3, #24
	str	r1, [sp, #0]
	str	r7, [sp, #4]
	ldr	r4, [sp, #52]
	adds	r1, r5, #0
	ldr	r0, [sp, #40]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4658
	ldr	r3, [r0, #4]
	ldr	r2, [r0, #0]
	str	r7, [sp, #4]
	ldr	r7, [sp, #24]
	movs	r1, #20
	str	r1, [sp, #0]
	subs	r3, #24
	ldr	r4, [r7, #4]
	ldr	r0, [sp, #40]
	adds	r1, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe01f
.L_0814a4fc:
	ldr	r0, [sp, #44]
	mov	r3, fp
	movs	r1, #184
	ldr	r2, [r3, #0]
	lsls	r1, r1, #5
	ldr	r3, [r3, #4]
	adds	r1, #180
	adds	r5, r0, r1
	movs	r7, #20
	movs	r0, #37
	subs	r2, #20
	subs	r3, #24
	str	r7, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #52]
	adds	r1, r5, #0
	ldr	r0, [sp, #40]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4659
	ldr	r3, [r1, #4]
	ldr	r2, [r1, #0]
	str	r7, [sp, #0]
	movs	r7, #37
	ldr	r0, [sp, #24]
	str	r7, [sp, #4]
	subs	r3, #24
	ldr	r4, [r0, #4]
	adds	r1, r5, #0
	ldr	r0, [sp, #40]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9903
	ldr	r5, [sp, #36]
	ldrb	r3, [r1, #0]
	ldr	r2, [pc, #168]
	cmp	r5, r3
	beq.n	.L_0814a54e
	adds	r3, #16
	cmp	r5, r3
	blt.n	.L_0814a61a
.L_0814a54e:
	movs	r7, #0
	mov	sl, r7
	ldr	r7, [pc, #156]
	movs	r0, #0
	mov	r8, r0
.L_0814a558:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	bne.n	.L_0814a60a
	bl	sub_08014878
	movs	r6, #192
	lsls	r6, r6, #2
	adds	r6, #255
	ands	r6, r0
	bl	sub_08014878
	mov	r2, r9
	ldr	r3, [r2, #0]
	movs	r5, #254
	lsls	r3, r3, #8
	str	r3, [r7, #0]
	lsls	r5, r5, #7
	ldr	r3, [r2, #4]
	ldr	r1, [pc, #116]
	adds	r5, #255
	ands	r5, r0
	movs	r0, #128
	lsls	r0, r0, #5
	lsls	r3, r3, #8
	adds	r5, r5, r1
	adds	r3, r3, r0
	str	r3, [r7, #4]
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r6, #32
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #15
	str	r3, [r7, #8]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #15
	str	r3, [r7, #16]
	ldr	r2, [sp, #12]
	ldr	r5, [sp, #36]
	ldrb	r3, [r2, #0]
	movs	r1, #1
	add	sl, r1
	cmp	r5, r3
	bne.n	.L_0814a5f8
	bl	sub_08014878
	movs	r1, #7
	ands	r0, r1
	adds	r0, #48
	mov	r2, sl
	str	r0, [r7, #24]
	cmp	r2, #200
	bne.n	.L_0814a60a
	b.n	.L_0814a618
	movs	r0, r0
	.4byte 0x03000730
	.4byte 0x00000148
	.4byte 0x02010018
	.4byte 0x08197a23
	.4byte 0x08143001
	.4byte 0x08197a20
	.4byte 0x08197a2f
	.4byte 0x02010000
	.2byte 0xc000
	.2byte 0xffff
.L_0814a5f8:
	.2byte 0xf6ca
	.2byte 0xf93e
	.2byte 0x2307
	ands	r0, r3
	adds	r0, #24
	mov	r5, sl
	str	r0, [r7, #24]
	cmp	r5, #4
	beq.n	.L_0814a618
.L_0814a60a:
	movs	r0, #1
	movs	r1, #128
	add	r8, r0
	lsls	r1, r1, #3
	adds	r7, #28
	cmp	r8, r1
	bne.n	.L_0814a558
.L_0814a618:
	ldr	r2, [pc, #376]
.L_0814a61a:
	ldr	r5, [sp, #32]
	ldr	r7, [sp, #36]
	ldrb	r3, [r2, r5]
	cmp	r7, r3
	bne.n	.L_0814a666
	ldr	r0, [sp, #44]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r3, r0, r1
	movs	r2, #2
	str	r2, [r3, #0]
	ldr	r5, [sp, #48]
	movs	r2, #0
	ldr	r3, [r5, #20]
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_0814a666
	movs	r5, #36
.L_0814a640:
	ldr	r7, [sp, #48]
	movs	r3, #8
	ldrsh	r0, [r5, r7]
	movs	r2, #5
	str	r3, [sp, #0]
	movs	r1, #10
	mov	r3, r8
	bl	sub_0814cd48
	ldrsh	r0, [r5, r7]
	movs	r1, #1
	bl	sub_08118088
	movs	r3, #1
	add	r8, r3
	ldr	r3, [r7, #20]
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L_0814a640
.L_0814a666:
	ldr	r5, [sp, #12]
	ldr	r7, [sp, #8]
	ldr	r0, [sp, #32]
	adds	r5, #1
	adds	r7, #28
	adds	r0, #1
	str	r5, [sp, #12]
	str	r7, [sp, #8]
	str	r0, [sp, #32]
	ldr	r1, [sp, #48]
	ldr	r3, [pc, #284]
	ldr	r2, [r1, #24]
	ldrb	r3, [r3, r2]
	cmp	r0, r3
	beq.n	.L_0814a686
	b.n	.L_0814a424
.L_0814a686:
	ldr	r6, [pc, #276]
	movs	r2, #0
	mov	r8, r2
.L_0814a68c:
	ldr	r5, [r6, #24]
	cmp	r5, #0
	ble.n	sub_0814a734
	subs	r3, r5, #1
	ldr	r2, [r6, #8]
	str	r3, [r6, #24]
	ldr	r3, [r6, #0]
	ldr	r1, [r6, #16]
	adds	r4, r3, r2
	ldr	r3, [r6, #4]
	str	r4, [r6, #0]
	adds	r0, r3, r1
	lsls	r3, r2, #4
	subs	r3, r3, r2
	lsls	r3, r3, #2
	str	r0, [r6, #4]
	cmp	r3, #0
	bge.n	.L_0814a6b2
	adds	r3, #63
.L_0814a6b2:
	asrs	r3, r3, #6
	str	r3, [r6, #8]
	lsls	r3, r1, #4
	subs	r3, r3, r1
	lsls	r3, r3, #2
	cmp	r3, #0
	bge.n	.L_0814a6c2
	adds	r3, #63
.L_0814a6c2:
	asrs	r3, r3, #6
	adds	r2, r3, #0
	subs	r2, #16
	str	r2, [r6, #16]
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L_0814a6d2
	adds	r3, #255
.L_0814a6d2:
	asrs	r3, r3, #8
	mov	ip, r3
	cmp	r3, #120
	ble.n	.L_0814a6e6
	negs	r3, r2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #16]
	b.n	sub_0814a734
.L_0814a6e6:
	cmp	r4, #0
	blt.n	sub_0814a734
	asrs	r7, r4, #8
	cmp	r7, #126
	bgt.n	sub_0814a734
	cmp	r0, #0
	blt.n	sub_0814a734
	adds	r2, r5, #0
	subs	r2, #17
	cmp	r2, #0
	bge.n	.L_0814a6fe
	adds	r2, #7
.L_0814a6fe:
	asrs	r5, r2, #3
	cmp	r5, #0
	bgt.n	.L_0814a706
	movs	r5, #1
.L_0814a706:
	ldr	r2, [pc, #152]
	lsls	r4, r5, #1
	mov	r3, r8
	movs	r0, #1
	ands	r0, r3
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #28]
	str	r5, [sp, #0]
	adds	r1, r2, r1
	lsrs	r2, r5, #31
	adds	r2, r5, r2
	asrs	r2, r2, #1
	subs	r2, r7, r2
	mov	r7, ip
	subs	r3, r7, r5
	str	r4, [sp, #4]
	ldr	r5, [sp, #24]
	lsls	r0, r0, #2
	ldr	r4, [r0, r5]
	ldr	r0, [sp, #40]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2701
	movs	r0, #128
	add	r8, r7
	lsls	r0, r0, #3
	adds	r6, #28
	cmp	r8, r0
	bne.n	.L_0814a68c
	movs	r0, #16
	movs	r1, #16
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r3, #240
	ldr	r1, [sp, #44]
	lsls	r3, r3, #7
	adds	r3, #232
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r5, [sp, #36]
	adds	r5, #1
	str	r5, [sp, #36]
	cmp	r5, #96
	beq.n	.L_0814a76e
	b.n	.L_0814a3ac
.L_0814a76e:
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #40]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #72
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x08197a2f
	.4byte 0x08197a20
	.4byte 0x02010000
	.4byte 0x08197410
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	ldr	r3, [r0, #24]
	cmp	r3, #0
	bne.n	.L_0814a7b8
	movs	r1, #0
	bl	sub_0814a814
	b.n	.L_0814a7be
.L_0814a7b8:
	movs	r1, #1
	bl	sub_0814a814
.L_0814a7be:
	pop	{pc}
	push	{lr}
	movs	r1, #1
	bl	sub_0814a814
	pop	{pc}
	.align 2, 0
