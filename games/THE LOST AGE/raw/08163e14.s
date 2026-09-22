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
	.set sub_0801587c, 0x0801587c
	.set sub_08020010, 0x08020010
	.set sub_08020048, 0x08020048
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138086, 0x08138086
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_0814cc4c, 0x0814cc4c
	.set sub_0814cd48, 0x0814cd48
	.set sub_08152404, 0x08152404
	.set sub_08157cf4, 0x08157cf4
	.set sub_081648a4, 0x081648a4
	.set sub_08164900, 0x08164900
	.set sub_08164964, 0x08164964
	.set sub_081649c2, 0x081649c2
	.set sub_081963ec, 0x081963ec
	.set sub_081c0010, 0x081c0010
	.global Overlay_08163e14
Overlay_08163e14:
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
	str	r0, [sp, #44]
	movs	r0, #0
	ldr	r1, [r3, #92]
	str	r1, [sp, #40]
	ldr	r3, [r3, #100]
	str	r3, [sp, #36]
	bl	sub_081435e0
	ldr	r3, [pc, #56]
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
	ldr	r0, [pc, #36]
	bl	sub_08013300
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	movs	r1, #160
	lsls	r2, r2, #24
	adds	r4, r0, #0
	adds	r3, #212
	lsls	r1, r1, #19
	adds	r2, #32
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [sp, #40]
	movs	r6, #224
	b.n	.L_08163e80
	movs	r0, r0
	.4byte 0x00001010
	.2byte 0x012f
	.2byte 0x0000
.L_08163e80:
	adds	r4, #128
	lsls	r6, r6, #3
	adds	r1, r3, r6
	adds	r0, r4, #0
	bl	sub_0801587c
	ldr	r0, [pc, #896]
	bl	sub_08013300
	ldr	r7, [sp, #40]
	adds	r4, r0, #0
	movs	r0, #208
	lsls	r0, r0, #4
	adds	r4, #128
	adds	r0, #228
	adds	r1, r7, r0
	adds	r0, r4, #0
	bl	sub_0801587c
	ldr	r0, [pc, #876]
	bl	sub_08013300
	ldr	r1, [sp, #36]
	bl	sub_0801587c
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r7, r1
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r7, r3
	movs	r1, #200
	movs	r3, #75
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #840]
	bl	sub_080145a8
	ldr	r1, [pc, #840]
	ldr	r2, [pc, #840]
	movs	r7, #128
	movs	r0, #176
	movs	r6, #0
	lsls	r7, r7, #17
	lsls	r0, r0, #15
	ldr	r3, [sp, #40]
	str	r6, [sp, #12]
	str	r7, [sp, #24]
	str	r0, [sp, #28]
	str	r1, [sp, #16]
	str	r2, [sp, #20]
	movs	r2, #1
	mov	r8, r6
	negs	r2, r2
	adds	r3, #24
.L_08163ef4:
	movs	r6, #1
	add	r8, r6
	mov	r7, r8
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r7, #64
	bne.n	.L_08163ef4
	ldr	r1, [sp, #40]
	movs	r2, #168
	movs	r0, #0
	lsls	r2, r2, #2
	mov	r8, r0
	adds	r5, r1, r2
.L_08163f0e:
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
	movs	r3, #15
	ands	r3, r0
	negs	r3, r3
	str	r3, [r5, #24]
	movs	r3, #1
	add	r8, r3
	mov	r6, r8
	adds	r5, #28
	cmp	r6, #16
	bne.n	.L_08163f0e
	ldr	r3, [pc, #740]
	movs	r7, #0
	movs	r1, #1
	movs	r2, #128
	mov	r8, r7
	negs	r1, r1
	lsls	r2, r2, #3
.L_08163f4a:
	movs	r0, #1
	add	r8, r0
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_08163f4a
	ldr	r1, [sp, #40]
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #240
	adds	r3, r1, r2
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
	movs	r3, #0
	mov	r9, r3
.L_08163f7e:
	ldr	r3, [pc, #680]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08163f98
	mov	r6, r9
	cmp	r6, #32
	ble.n	.L_08163f98
	cmp	r6, #97
	bgt.n	.L_08163f98
	movs	r7, #98
	mov	r9, r7
.L_08163f98:
	mov	r0, r9
	cmp	r0, #120
	bne.n	.L_08163fa4
	movs	r0, #134
	bl	sub_081180e8
.L_08163fa4:
	mov	r1, r9
	cmp	r1, #15
	bgt.n	.L_08163fb0
	ldr	r2, [sp, #12]
	adds	r2, #2
	str	r2, [sp, #12]
.L_08163fb0:
	mov	r3, r9
	cmp	r3, #99
	bgt.n	.L_08163ffa
	ldr	r2, [sp, #16]
	ldr	r7, [sp, #24]
	ldr	r1, [sp, #28]
	ldr	r6, [sp, #16]
	ldr	r0, [sp, #20]
	movs	r3, #58
	muls	r3, r2
	adds	r6, r6, r7
	adds	r0, r0, r1
	str	r6, [sp, #24]
	str	r0, [sp, #28]
	cmp	r3, #0
	bge.n	.L_08163fd2
	adds	r3, #63
.L_08163fd2:
	ldr	r6, [sp, #20]
	asrs	r3, r3, #6
	str	r3, [sp, #16]
	lsls	r3, r6, #3
	subs	r3, r3, r6
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L_08163fe4
	adds	r3, #63
.L_08163fe4:
	ldr	r7, [sp, #24]
	ldr	r0, [pc, #580]
	asrs	r3, r3, #6
	str	r3, [sp, #20]
	cmp	r7, r0
	bgt.n	.L_08163ffa
	ldr	r1, [sp, #16]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r2, r1, r2
	str	r2, [sp, #16]
.L_08163ffa:
	movs	r0, #1
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #28]
	bl	.L_0816442c
	mov	r3, r9
	cmp	r3, #28
	bne.n	.L_0816408e
	movs	r7, #63
	mov	sl, r7
	ldr	r7, [pc, #544]
	movs	r6, #0
	mov	r8, r6
.L_08164014:
	ldr	r3, [r7, #24]
	movs	r0, #1
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_08164080
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
	asrs	r3, r3, #2
	lsls	r6, r6, #15
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
	ands	r0, r2
	negs	r0, r0
	subs	r0, #8
	lsls	r0, r0, #13
	movs	r3, #0
	str	r0, [r7, #16]
	str	r3, [r7, #24]
.L_08164080:
	movs	r3, #1
	movs	r6, #128
	add	r8, r3
	lsls	r6, r6, #1
	adds	r7, #28
	cmp	r8, r6
	bne.n	.L_08164014
.L_0816408e:
	mov	r7, r9
	subs	r7, #32
	str	r7, [sp, #8]
	cmp	r7, #47
	bhi.n	.L_08164128
	ldr	r7, [pc, #404]
	movs	r0, #0
	movs	r1, #63
	mov	fp, r0
	mov	r8, r0
	mov	sl, r1
.L_081640a4:
	ldr	r3, [r7, #24]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_0816411a
	bl	sub_08014878
	mov	r3, sl
	adds	r6, r0, #0
	ands	r6, r3
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
	movs	r0, #128
	lsls	r0, r0, #14
	asrs	r3, r3, #3
	adds	r3, r3, r0
	str	r3, [r7, #0]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	movs	r1, #192
	lsls	r1, r1, #15
	asrs	r3, r3, #2
	adds	r3, r3, r1
	str	r3, [r7, #4]
	bl	sub_08014878
	mov	r2, sl
	ands	r0, r2
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r7, #12]
	bl	sub_08014878
	mov	r3, sl
	ands	r0, r3
	negs	r0, r0
	subs	r0, #8
	movs	r6, #1
	lsls	r0, r0, #13
	add	fp, r6
	str	r0, [r7, #16]
	movs	r3, #0
	mov	r0, fp
	str	r3, [r7, #24]
	cmp	r0, #16
	beq.n	.L_08164128
.L_0816411a:
	movs	r1, #1
	movs	r2, #128
	add	r8, r1
	lsls	r2, r2, #3
	adds	r7, #28
	cmp	r8, r2
	bne.n	.L_081640a4
.L_08164128:
	mov	r3, r9
	cmp	r3, #0
	bne.n	.L_08164134
	movs	r0, #164
	bl	sub_081c0010
.L_08164134:
	mov	r6, r9
	cmp	r6, #32
	bne.n	.L_08164140
	movs	r0, #145
	bl	sub_081c0010
.L_08164140:
	mov	r7, r9
	cmp	r7, #80
	bne.n	.L_0816414c
	movs	r0, #144
	bl	sub_081c0010
.L_0816414c:
	ldr	r0, [sp, #8]
	cmp	r0, #47
	bhi.n	.L_081641bc
	ldr	r6, [sp, #40]
	movs	r7, #208
	lsls	r7, r7, #4
	movs	r1, #0
	adds	r7, #228
	adds	r6, r6, r7
	mov	r8, r1
	ldr	r1, [pc, #208]
	mov	r2, r9
	mov	sl, r6
	ldr	r6, [pc, #208]
	lsls	r3, r2, #4
	movs	r0, #34
	mov	fp, r0
	adds	r7, r3, r1
.L_08164170:
	adds	r0, r7, #0
	movs	r1, #104
	bl	sub_08002064
	ldrb	r3, [r6, #1]
	ldrb	r2, [r6, #0]
	adds	r5, r0, #0
	movs	r1, #104
	mov	r0, fp
	subs	r3, r3, r5
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	ldr	r4, [sp, #52]
	subs	r2, #17
	subs	r3, #104
	ldr	r0, [sp, #44]
	mov	r1, sl
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x7832
	ldrb	r3, [r6, #1]
	mov	r0, fp
	subs	r2, #17
	str	r0, [sp, #0]
	mov	r1, sl
	subs	r3, r3, r5
	str	r5, [sp, #4]
	ldr	r4, [sp, #52]
	ldr	r0, [sp, #44]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2101
	add	r8, r1
	mov	r2, r8
	adds	r6, #2
	adds	r7, #25
	cmp	r2, #3
	bne.n	.L_08164170
.L_081641bc:
	mov	r3, r9
	cmp	r3, #95
	bgt.n	.L_08164206
	ldr	r7, [sp, #40]
	movs	r0, #224
	lsls	r0, r0, #3
	adds	r7, r7, r0
	movs	r6, #0
	mov	sl, r7
	mov	r8, r6
	movs	r5, #32
	movs	r7, #120
.L_081641d4:
	mov	r2, r8
	lsls	r1, r2, #5
	mov	r2, r9
	cmp	r2, #0
	bge.n	.L_081641e0
	adds	r2, #3
.L_081641e0:
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
	bne.n	.L_081641d4
.L_08164206:
	ldr	r5, [pc, #40]
	movs	r2, #0
	mov	r8, r2
	b.n	.L_0816423c
	movs	r0, r0
	.4byte 0x00000146
	.4byte 0x00000134
	.4byte 0x08143001
	.4byte 0xfff00000
	.4byte 0xfffc0000
	.4byte 0x02010018
	.4byte 0x03001150
	.4byte 0x0077ffff
	.4byte 0x02010000
	.4byte 0xffffff00
	.2byte 0x896a
	.2byte 0x0819
.L_0816423c:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_08164328
	mov	r0, r8
	movs	r1, #3
	bl	sub_08002064
	ldr	r3, [r5, #16]
	adds	r4, r0, #2
	cmp	r3, #0
	ble.n	.L_08164254
	adds	r4, #2
.L_08164254:
	mov	r6, r9
	cmp	r6, #68
	ble.n	.L_08164260
	cmp	r4, #5
	bgt.n	.L_08164260
	movs	r4, #6
.L_08164260:
	mov	r7, r9
	cmp	r7, #70
	ble.n	.L_0816426c
	cmp	r4, #6
	bgt.n	.L_0816426c
	movs	r4, #7
.L_0816426c:
	mov	r0, r9
	cmp	r0, #72
	ble.n	.L_08164278
	cmp	r4, #7
	bgt.n	.L_08164278
	movs	r4, #8
.L_08164278:
	mov	r1, r9
	cmp	r1, #74
	ble.n	.L_08164284
	cmp	r4, #8
	bgt.n	.L_08164284
	movs	r4, #9
.L_08164284:
	mov	r2, r9
	cmp	r2, #76
	ble.n	.L_0816428c
	movs	r4, #10
.L_0816428c:
	movs	r6, #4
	cmp	r3, #0
	bgt.n	.L_08164294
	movs	r6, #0
.L_08164294:
	ldr	r2, [pc, #388]
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
	ble.n	.L_081642dc
	ldr	r6, [pc, #328]
	adds	r3, r1, r6
	b.n	.L_081642ea
.L_081642dc:
	ldr	r3, [pc, #324]
	movs	r2, #3
	mov	r7, r8
	ands	r2, r7
	lsls	r2, r2, #2
	ldr	r3, [r3, r2]
	adds	r3, r1, r3
.L_081642ea:
	str	r3, [r5, #16]
	ldr	r2, [r5, #12]
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r3, r3, #1
	cmp	r3, #0
	bge.n	.L_081642fa
	adds	r3, #63
.L_081642fa:
	ldr	r2, [r5, #16]
	asrs	r3, r3, #6
	str	r3, [r5, #12]
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r2, r3, #1
	cmp	r2, #0
	bge.n	.L_0816430c
	adds	r2, #63
.L_0816430c:
	ldr	r3, [r5, #24]
	asrs	r2, r2, #6
	adds	r3, #1
	str	r2, [r5, #16]
	str	r3, [r5, #24]
	cmp	r2, #0
	ble.n	.L_08164328
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	cmp	r3, #104
	ble.n	.L_08164328
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L_08164328:
	movs	r1, #1
	movs	r2, #128
	add	r8, r1
	lsls	r2, r2, #3
	adds	r5, #28
	cmp	r8, r2
	bne.n	.L_0816423c
	mov	r3, r9
	cmp	r3, #79
	bgt.n	.L_081643b0
	ldr	r7, [sp, #48]
	movs	r6, #0
	ldr	r3, [r7, #20]
	mov	r8, r6
	cmp	r3, #0
	beq.n	.L_081643b0
	adds	r7, #36
.L_0816434a:
	mov	r0, r9
	cmp	r0, #29
	ble.n	.L_081643a6
	movs	r1, #12
	bl	sub_08002064
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_08164386
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
.L_08164386:
	cmp	r6, #6
	bne.n	.L_081643a2
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
	b.n	.L_081643a6
.L_081643a2:
	ldr	r0, [sp, #48]
	ldr	r3, [r0, #20]
.L_081643a6:
	movs	r1, #1
	add	r8, r1
	adds	r7, #2
	cmp	r8, r3
	bne.n	.L_0816434a
.L_081643b0:
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
	beq.n	.L_081643d0
	b.n	.L_08163f7e
.L_081643d0:
	ldr	r0, [pc, #84]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #28]
	movs	r0, #1
	bl	.L_0816467c
	movs	r3, #238
	ldr	r2, [sp, #40]
	lsls	r3, r3, #7
	movs	r1, #0
	adds	r3, #220
	mov	r8, r1
	adds	r5, r2, r3
.L_081643fa:
	movs	r6, #1
	add	r8, r6
	ldmia	r5!, {r0}
	mov	r7, r8
	bl	sub_08020048
	cmp	r7, #12
	bne.n	.L_081643fa
	bl	sub_08143bb8
	add	sp, #60
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x08197410
	.4byte 0xffff8000
	.4byte 0x08198974
	.2byte 0x3001
	.2byte 0x0814
.L_0816442c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	mov	sl, r1
	ldr	r1, [r3, #92]
	ldr	r3, [pc, #512]
	sub	sp, #40
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	add	r6, sp, #24
	str	r3, [sp, #16]
	str	r4, [sp, #20]
	ldr	r3, [pc, #500]
	mov	r8, r2
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	str	r3, [sp, #8]
	str	r4, [sp, #12]
	movs	r3, #0
	str	r3, [r6, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r6, #4]
	cmp	r0, #6
	bls.n	.L_08164468
	b.n	.L_08164632
.L_08164468:
	ldr	r2, [pc, #476]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0816448c
	.4byte 0x0816450e
	.4byte 0x0816454e
	.4byte 0x081645c4
	.4byte 0x081645fc
	.4byte 0x081644d0
	.2byte 0x458c
	.2byte 0x0816
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #220
	adds	r2, r1, r2
	str	r2, [sp, #4]
	movs	r5, #0
	add	r7, sp, #16
.L_0816449a:
	adds	r0, r5, #0
	movs	r1, #3
	bl	sub_08002064
	lsls	r0, r0, #21
	add	r0, sl
	str	r0, [r6, #0]
	movs	r1, #3
	adds	r0, r5, #0
	bl	sub_08002054
	lsls	r0, r0, #21
	add	r0, r8
	str	r0, [r6, #8]
	ldr	r2, [sp, #4]
	adds	r1, r6, #0
	ldmia	r2!, {r0}
	adds	r5, #1
	adds	r3, r2, #0
	str	r3, [sp, #4]
	adds	r2, r7, #0
	movs	r3, #0
	bl	sub_08020010
	cmp	r5, #9
	bne.n	.L_0816449a
	b.n	.L_08164632
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #220
	movs	r5, #0
	add	r7, sp, #16
	adds	r4, r1, r3
.L_081644dc:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_081644e4
	adds	r3, r5, #3
.L_081644e4:
	asrs	r3, r3, #2
	lsls	r2, r3, #2
	subs	r2, r5, r2
	ldmia	r4!, {r0}
	lsls	r2, r2, #21
	lsls	r3, r3, #21
	add	r2, sl
	add	r3, r8
	str	r2, [r6, #0]
	str	r3, [r6, #8]
	adds	r1, r6, #0
	adds	r2, r7, #0
	movs	r3, #0
	str	r4, [sp, #0]
	bl	sub_08020010
	adds	r5, #1
	ldr	r4, [sp, #0]
	cmp	r5, #12
	bne.n	.L_081644dc
	b.n	.L_08164632
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #220
	movs	r5, #0
	add	r7, sp, #16
	adds	r4, r1, r2
.L_0816451a:
	ldr	r3, [pc, #304]
	ldr	r2, [pc, #304]
	ldrb	r3, [r3, r5]
	ldmia	r4!, {r0}
	lsls	r3, r3, #16
	add	r3, sl
	adds	r3, r3, r2
	str	r3, [r6, #0]
	ldr	r3, [pc, #296]
	ldr	r2, [pc, #296]
	ldrb	r3, [r3, r5]
	adds	r1, r6, #0
	lsls	r3, r3, #16
	add	r3, r8
	adds	r3, r3, r2
	str	r3, [r6, #8]
	adds	r2, r7, #0
	movs	r3, #0
	str	r4, [sp, #0]
	bl	sub_08020010
	adds	r5, #1
	ldr	r4, [sp, #0]
	cmp	r5, #12
	bne.n	.L_0816451a
	b.n	.L_08164632
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #220
	movs	r5, #0
	add	r7, sp, #16
	adds	r4, r1, r3
.L_0816455a:
	ldr	r3, [pc, #256]
	movs	r2, #128
	ldrb	r3, [r3, r5]
	lsls	r2, r2, #13
	lsls	r3, r3, #16
	add	r3, sl
	adds	r3, r3, r2
	str	r3, [r6, #0]
	ldr	r3, [pc, #244]
	ldmia	r4!, {r0}
	ldrb	r3, [r3, r5]
	adds	r1, r6, #0
	lsls	r3, r3, #16
	add	r3, r8
	str	r3, [r6, #8]
	adds	r2, r7, #0
	movs	r3, #0
	str	r4, [sp, #0]
	bl	sub_08020010
	adds	r5, #1
	ldr	r4, [sp, #0]
	cmp	r5, #8
	bne.n	.L_0816455a
	b.n	.L_08164632
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #220
	movs	r5, #0
	add	r7, sp, #16
	adds	r4, r1, r3
.L_08164598:
	ldr	r3, [pc, #200]
	ldmia	r4!, {r0}
	ldrb	r3, [r3, r5]
	adds	r1, r6, #0
	lsls	r3, r3, #16
	add	r3, sl
	str	r3, [r6, #0]
	ldr	r3, [pc, #192]
	adds	r2, r7, #0
	ldrb	r3, [r3, r5]
	str	r4, [sp, #0]
	lsls	r3, r3, #16
	add	r3, r8
	str	r3, [r6, #8]
	movs	r3, #0
	bl	sub_08020010
	adds	r5, #1
	ldr	r4, [sp, #0]
	cmp	r5, #8
	bne.n	.L_08164598
	b.n	.L_08164632
	movs	r3, #238
	movs	r2, #8
	lsls	r3, r3, #7
	add	r2, sp
	adds	r3, #220
	movs	r5, #0
	mov	r9, r2
	adds	r7, r1, r3
.L_081645d4:
	ldr	r3, [pc, #148]
	ldmia	r7!, {r0}
	ldrb	r3, [r3, r5]
	adds	r1, r6, #0
	lsls	r3, r3, #16
	add	r3, sl
	str	r3, [r6, #0]
	ldr	r3, [pc, #140]
	mov	r2, r9
	ldrb	r3, [r3, r5]
	adds	r5, #1
	lsls	r3, r3, #16
	add	r3, r8
	str	r3, [r6, #8]
	movs	r3, #0
	bl	sub_08020010
	cmp	r5, #8
	bne.n	.L_081645d4
	b.n	.L_08164632
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #220
	movs	r5, #0
	add	r7, sp, #16
	adds	r4, r1, r2
.L_08164608:
	ldr	r3, [pc, #104]
	ldmia	r4!, {r0}
	ldrsb	r3, [r3, r5]
	adds	r1, r6, #0
	lsls	r3, r3, #16
	add	r3, sl
	str	r3, [r6, #0]
	ldr	r3, [pc, #96]
	adds	r2, r7, #0
	ldrb	r3, [r3, r5]
	str	r4, [sp, #0]
	lsls	r3, r3, #16
	add	r3, r8
	str	r3, [r6, #8]
	movs	r3, #0
	bl	sub_08020010
	adds	r5, #1
	ldr	r4, [sp, #0]
	cmp	r5, #11
	bne.n	.L_08164608
.L_08164632:
	add	sp, #40
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x08196e5c
	.4byte 0x08196e64
	.4byte 0x08164470
	.4byte 0x08198984
	.4byte 0xfff00000
	.4byte 0x08198990
	.4byte 0xffe00000
	.4byte 0x0819899c
	.4byte 0x081989a4
	.4byte 0x081989ac
	.4byte 0x081989b4
	.4byte 0x081989bc
	.4byte 0x081989c4
	.4byte 0x081989cc
	.2byte 0x89d7
	.2byte 0x0819
.L_0816467c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #36
	str	r0, [sp, #32]
	str	r2, [sp, #28]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #92]
	mov	fp, r1
	ldr	r1, [r5, #96]
	mov	sl, r0
	str	r1, [sp, #24]
	mov	r3, fp
	movs	r0, #160
	adds	r4, r3, #0
	lsls	r0, r0, #14
	adds	r4, r4, r0
	mov	fp, r4
	ldr	r2, [r5, #100]
	str	r3, [sp, #8]
	lsrs	r3, r4, #31
	add	r3, fp
	asrs	r3, r3, #1
	str	r2, [sp, #12]
	mov	fp, r3
	movs	r2, #128
	ldr	r3, [pc, #60]
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	adds	r2, #8
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #40
	strh	r3, [r2, #0]
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	ldr	r1, [r5, #104]
	movs	r0, #188
	str	r1, [sp, #16]
	movs	r1, #3
	bl	sub_081963ec
	adds	r5, #188
	ldr	r5, [r5, #0]
	ldr	r1, [sp, #12]
	ldr	r0, [pc, #20]
	movs	r2, #0
	movs	r3, #0
	str	r5, [sp, #20]
	bl	sub_08157cf4
	b.n	.L_08164704
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x00003f46
	.2byte 0x0134
	.2byte 0x0000
.L_08164704:
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #792]
	add	r1, sl
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r1, #192
	lsls	r1, r1, #7
	adds	r1, #216
	ldr	r0, [pc, #780]
	add	r1, sl
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
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
	movs	r1, #200
	str	r3, [r2, #0]
	ldr	r0, [pc, #748]
	lsls	r1, r1, #4
	bl	sub_080145a8
	movs	r2, #0
	mov	r8, r2
	mov	r7, sl
.L_0816474c:
	bl	sub_08014878
	movs	r6, #255
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r6, r0
	adds	r6, r6, r3
	bl	sub_08014878
	mov	r4, fp
	str	r4, [r7, #0]
	movs	r3, #255
	adds	r5, r0, #0
	lsls	r3, r3, #8
	ldr	r0, [sp, #28]
	adds	r3, #255
	ands	r5, r3
	str	r0, [r7, #4]
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #6
	negs	r3, r3
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #15
	movs	r1, #1
	ands	r3, r0
	add	r8, r1
	adds	r3, #16
	mov	r2, r8
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r2, #64
	bne.n	.L_0816474c
	movs	r5, #192
	lsls	r5, r5, #3
	movs	r3, #0
	adds	r5, #172
	mov	r8, r3
	movs	r6, #0
	add	r5, sl
.L_081647b4:
	mov	r4, fp
	str	r4, [r5, #0]
	ldr	r0, [sp, #28]
	str	r0, [r5, #4]
	adds	r0, r6, #0
	bl	sub_08002096
	lsls	r0, r0, #5
	asrs	r0, r0, #6
	str	r0, [r5, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	movs	r1, #170
	lsls	r0, r0, #5
	movs	r2, #1
	asrs	r0, r0, #5
	lsls	r1, r1, #7
	add	r8, r2
	negs	r0, r0
	adds	r1, #85
	mov	r3, r8
	str	r0, [r5, #16]
	adds	r6, r6, r1
	adds	r5, #28
	cmp	r3, #3
	bne.n	.L_081647b4
	ldr	r7, [pc, #580]
	movs	r4, #0
	mov	r8, r4
.L_081647f0:
	bl	sub_08014878
	movs	r6, #255
	ands	r6, r0
	bl	sub_08014878
	adds	r5, r0, #0
	mov	r0, fp
	str	r0, [r7, #0]
	movs	r3, #255
	ldr	r1, [sp, #28]
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r5, r3
	str	r1, [r7, #4]
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r6, #32
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #5
	negs	r3, r3
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	movs	r2, #1
	adds	r3, #20
	add	r8, r2
	str	r3, [r7, #24]
	mov	r3, r8
	adds	r7, #28
	cmp	r3, #64
	bne.n	.L_081647f0
	movs	r7, #0
.L_08164848:
	cmp	r7, #4
	bne.n	.L_08164852
	movs	r0, #154
	bl	sub_081c0010
.L_08164852:
	cmp	r7, #32
	bne.n	.L_0816485c
	movs	r0, #212
	bl	sub_081c0010
.L_0816485c:
	cmp	r7, #47
	bgt.n	sub_081648a4
	adds	r0, r7, #0
	subs	r0, #8
	movs	r1, #5
	bl	sub_08002054
	adds	r4, r0, #0
	cmp	r4, #0
	bge.n	.L_08164872
	movs	r4, #0
.L_08164872:
	ldr	r2, [pc, #448]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	mov	r3, fp
	asrs	r2, r3, #16
	ldr	r3, [pc, #440]
	movs	r0, #224
	ldrb	r5, [r3, r4]
	lsls	r0, r0, #3
	add	r1, sl
	adds	r1, r1, r0
	ldr	r0, [sp, #28]
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	asrs	r3, r0, #16
	ldr	r0, [pc, #424]
	ldrb	r4, [r0, r4]
	str	r5, [sp, #0]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #24]
	ldr	r4, [sp, #16]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2000
	mov	r8, r0
	mov	r6, sl
.L_081648aa:
	mov	r1, r8
	lsrs	r3, r1, #31
	add	r3, r8
	asrs	r3, r3, #1
	cmp	r7, r3
	ble.n	sub_08164900
	ldr	r3, [r6, #24]
	cmp	r3, #0
	ble.n	sub_08164900
	subs	r3, #1
	str	r3, [r6, #24]
	adds	r0, r6, #0
	movs	r1, #60
	movs	r2, #0
	bl	sub_08138086
	ldr	r4, [r6, #24]
	cmp	r4, #0
	bge.n	.L_081648d2
	adds	r4, #15
.L_081648d2:
	asrs	r4, r4, #4
	adds	r4, #3
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	movs	r0, #6
	ldrsh	r3, [r6, r0]
	ldr	r0, [pc, #352]
	lsls	r5, r4, #1
	subs	r1, r5, #2
	ldrh	r1, [r0, r1]
	ldr	r0, [sp, #12]
	subs	r3, r3, r4
	adds	r1, r0, r1
	lsrs	r0, r4, #31
	adds	r0, r4, r0
	asrs	r0, r0, #1
	subs	r2, r2, r0
	str	r4, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #24]
	ldr	r4, [sp, #20]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2001
	add	r8, r0
	mov	r1, r8
	adds	r6, #28
	cmp	r1, #30
	bne.n	.L_081648aa
	ldr	r3, [pc, #304]
	ldr	r6, [pc, #288]
	movs	r2, #0
	mov	r8, r2
	mov	r9, r3
.L_08164916:
	cmp	r7, #35
	ble.n	sub_08164964
	ldr	r3, [r6, #24]
	cmp	r3, #0
	ble.n	sub_08164964
	subs	r3, #1
	str	r3, [r6, #24]
	adds	r0, r6, #0
	movs	r1, #60
	movs	r2, #0
	bl	sub_08138086
	ldr	r4, [r6, #24]
	cmp	r4, #0
	bge.n	.L_08164936
	adds	r4, #15
.L_08164936:
	asrs	r4, r4, #4
	adds	r4, #1
	lsls	r5, r4, #1
	movs	r0, #2
	ldrsh	r2, [r6, r0]
	movs	r1, #6
	ldrsh	r3, [r6, r1]
	mov	r0, r9
	subs	r1, r5, #2
	ldrh	r1, [r0, r1]
	ldr	r0, [sp, #12]
	subs	r3, r3, r4
	adds	r1, r0, r1
	lsrs	r0, r4, #31
	adds	r0, r4, r0
	asrs	r0, r0, #1
	subs	r2, r2, r0
	str	r4, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #24]
	ldr	r4, [sp, #20]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2001
	add	r8, r0
	mov	r1, r8
	adds	r6, #28
	cmp	r1, #60
	bne.n	.L_08164916
	movs	r5, #192
	lsls	r5, r5, #3
	movs	r2, #0
	adds	r6, r7, #0
	adds	r5, #172
	mov	r8, r2
	subs	r6, #36
	add	r5, sl
.L_08164980:
	cmp	r6, #27
	bhi.n	sub_081649c2
	movs	r2, #0
	adds	r0, r5, #0
	movs	r1, #64
	bl	sub_08138086
	movs	r1, #7
	adds	r0, r6, #0
	bl	sub_08002054
	lsls	r1, r0, #3
	adds	r1, r1, r0
	movs	r0, #192
	lsls	r1, r1, #5
	lsls	r0, r0, #7
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	adds	r0, #216
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	add	r1, sl
	adds	r1, r1, r0
	movs	r0, #12
	str	r0, [sp, #0]
	movs	r0, #24
	str	r0, [sp, #4]
	subs	r2, #6
	subs	r3, #12
	ldr	r0, [sp, #24]
	ldr	r4, [sp, #16]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2001
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #3
	bne.n	.L_08164980
	cmp	r7, #35
	bgt.n	.L_081649dc
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #28]
	bl	.L_0816442c
.L_081649dc:
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, sl
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	adds	r7, #1
	bl	sub_08013560
	cmp	r7, #72
	beq.n	.L_081649f6
	b.n	.L_08164848
.L_081649f6:
	ldr	r0, [pc, #52]
	bl	sub_08014644
	movs	r1, #128
	ldr	r3, [pc, #68]
	lsls	r1, r1, #7
	ldr	r0, [pc, #68]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00000120
	.4byte 0x00000121
	.4byte 0x08143001
	.4byte 0x02010000
	.4byte 0x081989f2
	.4byte 0x081989e2
	.4byte 0x081989ea
	.4byte 0x08197410
	.4byte 0x03000258
	.2byte 0x4000
	.2byte 0x0600
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r5, #160
	ldr	r7, [pc, #48]
	lsls	r5, r5, #19
	mov	r8, r0
	mov	lr, r1
	mov	ip, r2
	adds	r5, #2
	movs	r6, #0
.L_08164a62:
	ldrh	r2, [r5, #0]
	movs	r0, #31
	lsls	r3, r2, #16
	lsrs	r1, r3, #26
	ands	r1, r7
	lsrs	r4, r3, #21
	ands	r4, r7
	ands	r0, r2
	add	r1, r8
	add	r4, lr
	add	r0, ip
	cmp	r1, #31
	ble.n	.L_08164a7e
	movs	r1, #31
.L_08164a7e:
	cmp	r4, #31
	ble.n	.L_08164a8c
	movs	r4, #31
	b.n	.L_08164a8c
	movs	r0, r0
	.2byte 0x001f
	.2byte 0x0000
.L_08164a8c:
	cmp	r0, #31
	ble.n	.L_08164a92
	movs	r0, #31
.L_08164a92:
	cmp	r1, #0
	bge.n	.L_08164a98
	movs	r1, #0
.L_08164a98:
	cmp	r4, #0
	bge.n	.L_08164a9e
	movs	r4, #0
.L_08164a9e:
	cmp	r0, #0
	bge.n	.L_08164aa4
	movs	r0, #0
.L_08164aa4:
	lsls	r3, r1, #10
	lsls	r2, r4, #5
	orrs	r3, r2
	orrs	r3, r0
	adds	r6, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r6, #63
	bne.n	.L_08164a62
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r5, #160
	ldr	r7, [pc, #48]
	lsls	r5, r5, #19
	mov	r8, r0
	mov	lr, r1
	mov	ip, r2
	adds	r5, #192
	movs	r6, #0
.L_08164ad2:
	ldrh	r2, [r5, #0]
	movs	r0, #31
	lsls	r3, r2, #16
	lsrs	r1, r3, #26
	lsrs	r4, r3, #21
	ands	r0, r2
	ands	r1, r7
	ands	r4, r7
	add	r0, r8
	add	r4, lr
	add	r1, ip
	cmp	r0, #31
	ble.n	.L_08164aee
	movs	r0, #31
.L_08164aee:
	cmp	r4, #31
	ble.n	.L_08164afc
	movs	r4, #31
	b.n	.L_08164afc
	movs	r0, r0
	.2byte 0x001f
	.2byte 0x0000
.L_08164afc:
	cmp	r1, #31
	ble.n	.L_08164b02
	movs	r1, #31
.L_08164b02:
	cmp	r0, #0
	bge.n	.L_08164b08
	movs	r0, #0
.L_08164b08:
	cmp	r4, #0
	bge.n	.L_08164b0e
	movs	r4, #0
.L_08164b0e:
	cmp	r1, #0
	bge.n	.L_08164b14
	movs	r1, #0
.L_08164b14:
	lsls	r3, r1, #10
	lsls	r2, r4, #5
	orrs	r3, r2
	orrs	r3, r0
	adds	r6, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r6, #128
	bne.n	.L_08164ad2
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	mov	sl, r0
	mov	lr, r2
	ldr	r3, [r3, #36]
	movs	r0, #160
	ldr	r2, [pc, #48]
	movs	r7, #160
	lsls	r0, r0, #3
	lsls	r7, r7, #19
	adds	r0, #108
	mov	r8, r1
	adds	r7, #192
	adds	r5, r3, r0
	movs	r6, #0
	mov	ip, r2
.L_08164b54:
	ldrh	r2, [r5, #0]
	mov	r0, ip
	lsls	r3, r2, #16
	lsrs	r1, r3, #26
	lsrs	r4, r3, #21
	ands	r1, r0
	ands	r4, r0
	movs	r0, #31
	ands	r0, r2
	add	r0, sl
	add	r4, r8
	add	r1, lr
	cmp	r0, #31
	ble.n	.L_08164b78
	movs	r0, #31
	b.n	.L_08164b78
	.2byte 0x001f
	.2byte 0x0000
.L_08164b78:
	cmp	r4, #31
	ble.n	.L_08164b7e
	movs	r4, #31
.L_08164b7e:
	cmp	r1, #31
	ble.n	.L_08164b84
	movs	r1, #31
.L_08164b84:
	cmp	r0, #0
	bge.n	.L_08164b8a
	movs	r0, #0
.L_08164b8a:
	cmp	r4, #0
	bge.n	.L_08164b90
	movs	r4, #0
.L_08164b90:
	cmp	r1, #0
	bge.n	.L_08164b96
	movs	r1, #0
.L_08164b96:
	lsls	r3, r1, #10
	lsls	r2, r4, #5
	orrs	r3, r2
	orrs	r3, r0
	adds	r6, #1
	strh	r3, [r7, #0]
	adds	r5, #2
	adds	r7, #2
	cmp	r6, #128
	bne.n	.L_08164b54
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
