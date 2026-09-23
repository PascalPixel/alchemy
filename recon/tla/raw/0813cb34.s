.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
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
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138058, 0x08138058
	.set sub_0813d098, 0x0813d098
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cd48, 0x0814cd48
	.set sub_081504c0, 0x081504c0
	.set sub_08157cf4, 0x08157cf4
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_081963ec, 0x081963ec
	.set sub_081c0010, 0x081c0010
	.global Func_0813cb34
	.thumb_func
Func_0813cb34:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	mov	sl, r1
	ldr	r1, [r3, #92]
	sub	sp, #68
	str	r1, [sp, #40]
	mov	r1, sl
	ldr	r2, [r3, #96]
	mov	r9, r0
	str	r2, [sp, #36]
	ldr	r4, [r3, #48]
	str	r4, [sp, #24]
	ldr	r3, [r3, #100]
	str	r3, [sp, #20]
	cmp	r1, #0
	bne.n	.L_0813cb6a
	movs	r0, #0
	bl	sub_081435e0
	b.n	.L_0813cb70
.L_0813cb6a:
	movs	r0, #1
	bl	sub_081435e0
.L_0813cb70:
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r2, [r5, #104]
	movs	r1, #27
	movs	r0, #188
	str	r2, [sp, #28]
	bl	sub_081963ec
	adds	r5, #188
	ldr	r5, [r5, #0]
	movs	r3, #0
	ldr	r0, [pc, #756]
	ldr	r1, [sp, #20]
	movs	r2, #0
	str	r5, [sp, #32]
	bl	sub_08157cf4
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_0813cba4
	ldr	r0, [pc, #740]
	b.n	.L_0813cbb0
.L_0813cba4:
	mov	r4, sl
	cmp	r4, #1
	bne.n	.L_0813cbae
	ldr	r0, [pc, #736]
	b.n	.L_0813cbb0
.L_0813cbae:
	ldr	r0, [pc, #736]
.L_0813cbb0:
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #728]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x990a
	movs	r3, #239
	movs	r4, #238
	lsls	r3, r3, #7
	lsls	r4, r4, #7
	adds	r2, r1, r3
	adds	r4, #132
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r1, r4
	movs	r3, #75
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #696]
	bl	sub_080145a8
	mov	r1, r9
	ldr	r0, [r1, #8]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	ldr	r6, [pc, #684]
	str	r0, [sp, #12]
	movs	r2, #0
	mov	r8, r2
.L_0813cbf6:
	mov	r3, sl
	cmp	r3, #2
	bne.n	.L_0813cc4e
	mov	r4, r9
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_0813cc18
	bl	sub_08014878
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #255
	movs	r1, #128
	ands	r3, r0
	lsls	r1, r1, #8
	adds	r5, r3, r1
	b.n	.L_0813cc26
.L_0813cc18:
	bl	sub_08014878
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r5, r0, #0
	adds	r3, #255
	ands	r5, r3
.L_0813cc26:
	bl	sub_08014878
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	movs	r2, #128
	ands	r3, r0
	lsls	r2, r2, #4
	adds	r7, r3, r2
	bl	sub_08014878
	ldr	r4, [sp, #12]
	lsls	r0, r0, #4
	ldr	r3, [r4, #12]
	movs	r1, #160
	adds	r3, r3, r0
	lsls	r1, r1, #11
	adds	r3, r3, r1
	str	r3, [r6, #4]
	b.n	.L_0813cc70
.L_0813cc4e:
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	movs	r3, #192
	ldr	r2, [sp, #12]
	lsls	r3, r3, #2
	adds	r3, #255
	ands	r3, r0
	adds	r7, r3, #0
	ldr	r3, [r2, #12]
	movs	r4, #160
	lsls	r4, r4, #11
	adds	r3, r3, r4
	str	r3, [r6, #4]
	adds	r7, #32
.L_0813cc70:
	ldr	r1, [sp, #12]
	adds	r0, r5, #0
	ldr	r3, [r1, #8]
	str	r3, [r6, #0]
	ldr	r3, [r1, #16]
	str	r3, [r6, #8]
	bl	sub_08002096
	adds	r3, r7, #0
	muls	r3, r0
	mov	r2, sl
	asrs	r3, r3, #8
	str	r3, [r6, #12]
	cmp	r2, #2
	bne.n	.L_0813ccac
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	adds	r3, #128
	lsls	r3, r3, #10
	str	r3, [r6, #16]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r7, #0
	muls	r3, r0
	negs	r3, r3
	asrs	r3, r3, #9
	b.n	.L_0813ccca
.L_0813ccac:
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	subs	r3, #32
	lsls	r3, r3, #9
	str	r3, [r6, #16]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r7, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #8
.L_0813ccca:
	str	r3, [r6, #20]
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	adds	r3, #48
	str	r3, [r6, #24]
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_0813ccf2
	ldr	r3, [r6, #12]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #12]
	ldr	r3, [r6, #20]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #20]
.L_0813ccf2:
	movs	r4, #1
	movs	r1, #128
	add	r8, r4
	lsls	r1, r1, #1
	adds	r6, #28
	cmp	r8, r1
	beq.n	.L_0813cd02
	b.n	.L_0813cbf6
.L_0813cd02:
	movs	r2, #96
	mov	r3, sl
	str	r2, [sp, #16]
	cmp	r3, #2
	beq.n	.L_0813cd10
	movs	r4, #128
	str	r4, [sp, #16]
.L_0813cd10:
	ldr	r2, [sp, #16]
	movs	r1, #0
	mov	fp, r1
	cmp	r2, #0
	bne.n	.L_0813cd1c
	b.n	.L_0813cf20
.L_0813cd1c:
	ldr	r3, [sp, #24]
	adds	r3, #12
	str	r3, [sp, #8]
.L_0813cd22:
	bl	sub_08014de4
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #8]
	bl	sub_080156e8
	mov	r4, sl
	cmp	r4, #2
	bne.n	.L_0813cd40
	mov	r1, fp
	cmp	r1, #0
	bne.n	.L_0813cd40
	movs	r0, #103
	bl	sub_081c0010
.L_0813cd40:
	ldr	r6, [pc, #344]
	movs	r2, #0
	mov	r8, r2
	add	r7, sp, #56
.L_0813cd48:
	mov	r0, r8
	movs	r1, #32
	bl	sub_08002054
	lsls	r0, r0, #3
	cmp	fp, r0
	blt.n	.L_0813ce30
	ldr	r3, [r6, #24]
	cmp	r3, #0
	blt.n	.L_0813ce30
	mov	r4, r8
	lsls	r0, r4, #2
	adds	r0, r0, r3
	lsls	r0, r0, #10
	bl	sub_08002096
	ldr	r3, [r6, #0]
	lsls	r0, r0, #4
	adds	r3, r3, r0
	str	r3, [r7, #0]
	add	r5, sp, #44
	ldr	r3, [r6, #4]
	adds	r1, r5, #0
	str	r3, [r7, #4]
	adds	r0, r7, #0
	ldr	r3, [r6, #8]
	str	r3, [r7, #8]
	bl	sub_0815e1ec
	ldr	r3, [r5, #0]
	movs	r1, #58
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	ldr	r3, [r5, #8]
	adds	r1, #255
	cmp	r3, r1
	bgt.n	.L_0813cd98
	movs	r3, #157
	lsls	r3, r3, #1
	str	r3, [r5, #8]
.L_0813cd98:
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #122
	cmp	r3, r2
	ble.n	.L_0813cda6
	str	r2, [r5, #8]
	adds	r3, r2, #0
.L_0813cda6:
	ldr	r4, [pc, #248]
	adds	r2, r3, r4
	cmp	r2, #0
	bge.n	.L_0813cdb2
	adds	r2, r3, #0
	subs	r2, #251
.L_0813cdb2:
	asrs	r3, r2, #6
	movs	r0, #6
	subs	r0, r0, r3
	ldr	r2, [pc, #232]
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #20]
	lsrs	r3, r0, #31
	adds	r1, r2, r1
	ldr	r2, [r5, #0]
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #36]
	ldr	r4, [sp, #28]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4651
	cmp	r1, #2
	bne.n	.L_0813cdfe
	ldr	r2, [pc, #192]
	adds	r0, r6, #0
	movs	r1, #62
	bl	sub_08138058
	ldr	r3, [r6, #4]
	ldr	r2, [pc, #184]
	cmp	r3, r2
	bgt.n	.L_0813ce0a
	ldr	r3, [r6, #16]
	negs	r3, r3
	str	r3, [r6, #16]
	b.n	.L_0813ce0a
.L_0813cdfe:
	movs	r2, #128
	adds	r0, r6, #0
	movs	r1, #62
	lsls	r2, r2, #3
	bl	sub_08138058
.L_0813ce0a:
	mov	r3, sl
	cmp	r3, #1
	bne.n	.L_0813ce2a
	ldr	r4, [sp, #12]
	ldr	r3, [r4, #8]
	cmp	r3, #0
	bge.n	.L_0813ce22
	ldr	r3, [r6, #12]
	movs	r1, #128
	lsls	r1, r1, #6
	adds	r3, r3, r1
	b.n	.L_0813ce28
.L_0813ce22:
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #128]
	adds	r3, r3, r2
.L_0813ce28:
	str	r3, [r6, #12]
.L_0813ce2a:
	ldr	r3, [r6, #24]
	subs	r3, #1
	str	r3, [r6, #24]
.L_0813ce30:
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r6, #28
	cmp	r4, #128
	bne.n	.L_0813cd48
	mov	r1, sl
	cmp	r1, #1
	bne.n	.L_0813ceb0
	mov	r4, r9
	ldr	r3, [r4, #20]
	movs	r2, #0
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_0813cefc
	movs	r6, #36
	movs	r5, #48
.L_0813ce52:
	cmp	fp, r5
	bne.n	.L_0813ce74
	movs	r0, #1
	negs	r0, r0
	bl	sub_081180e8
	mov	r1, r9
	movs	r3, #8
	ldrsh	r0, [r6, r1]
	str	r3, [sp, #0]
	movs	r1, #7
	mov	r3, r8
	movs	r2, #5
	bl	sub_0814cd48
	mov	r4, r9
	ldr	r3, [r4, #20]
.L_0813ce74:
	movs	r1, #1
	add	r8, r1
	adds	r6, #2
	adds	r5, #8
	cmp	r8, r3
	bne.n	.L_0813ce52
	b.n	.L_0813cefc
	movs	r0, r0
	.4byte 0x00000134
	.4byte 0x0000013d
	.4byte 0x0000013c
	.4byte 0x00000138
	.4byte 0x03000730
	.4byte 0x08143001
	.4byte 0x02010000
	.4byte 0xfffffec6
	.4byte 0x08197410
	.4byte 0xffffe000
	.2byte 0xffff
	.2byte 0x0004
.L_0813ceb0:
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L_0813cefc
	movs	r3, #0
	mov	r4, r9
	mov	r8, r3
	ldr	r3, [r4, #20]
	cmp	r3, #0
	beq.n	.L_0813cefc
	movs	r7, #1
	negs	r7, r7
	movs	r6, #36
	movs	r5, #48
.L_0813ceca:
	cmp	fp, r5
	bne.n	.L_0813cef0
	movs	r0, #126
	bl	sub_081c0010
	adds	r0, r7, #0
	bl	sub_081180e8
	mov	r1, r9
	movs	r3, #8
	ldrsh	r0, [r6, r1]
	str	r3, [sp, #0]
	movs	r1, #7
	mov	r3, r8
	adds	r2, r7, #0
	bl	sub_0814cd48
	mov	r4, r9
	ldr	r3, [r4, #20]
.L_0813cef0:
	movs	r1, #1
	add	r8, r1
	adds	r6, #2
	adds	r5, #8
	cmp	r8, r3
	bne.n	.L_0813ceca
.L_0813cefc:
	bl	sub_081434f8
	movs	r4, #240
	ldr	r3, [sp, #40]
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #16]
	movs	r1, #1
	add	fp, r1
	cmp	fp, r2
	beq.n	.L_0813cf20
	b.n	.L_0813cd22
.L_0813cf20:
	ldr	r0, [pc, #76]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	mov	r3, sl
	cmp	r3, #2
	bne.n	.L_0813cf5c
	movs	r1, #240
	ldr	r5, [pc, #56]
	lsls	r1, r1, #6
	ldr	r0, [pc, #56]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x21f0
	ldr	r0, [sp, #36]
	lsls	r1, r1, #6
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x480b
	bl	sub_08014644
	mov	r0, r9
	bl	sub_081504c0
	b.n	.L_0813cf60
.L_0813cf5c:
	bl	sub_08143bb8
.L_0813cf60:
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x08143001
	.4byte 0x03000258
	.4byte 0x06004000
	.2byte 0x3489
	.2byte 0x0814
	push	{lr}
	movs	r1, #6
	bl	sub_0813d098
	pop	{pc}
	.align 2, 0
