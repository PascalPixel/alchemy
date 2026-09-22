.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014de4, 0x08014de4
	.set sub_080156e8, 0x080156e8
	.set sub_08118070, 0x08118070
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138058, 0x08138058
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cd48, 0x0814cd48
	.set sub_0814ef44, 0x0814ef44
	.set sub_08157cf4, 0x08157cf4
	.set sub_0815b510, 0x0815b510
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_081963ec, 0x081963ec
	.set sub_081c0010, 0x081c0010
	.global Overlay_0814ea58
Overlay_0814ea58:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #72
	str	r1, [sp, #56]
	movs	r3, #192
	lsls	r3, r3, #18
	mov	sl, r0
	ldr	r0, [r3, #92]
	str	r0, [sp, #52]
	movs	r0, #1
	ldr	r1, [r3, #96]
	str	r1, [sp, #48]
	ldr	r2, [r3, #100]
	str	r2, [sp, #32]
	ldr	r3, [r3, #48]
	str	r3, [sp, #28]
	bl	sub_081435e0
	ldr	r3, [sp, #56]
	cmp	r3, #2
	bne.n	.L_0814ea9a
	movs	r2, #128
	lsls	r2, r2, #9
	ldr	r0, [sp, #32]
	movs	r1, #10
	bl	sub_0815b510
	b.n	.L_0814eaa6
.L_0814ea9a:
	ldr	r0, [pc, #372]
	ldr	r1, [sp, #32]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
.L_0814eaa6:
	ldr	r4, [sp, #56]
	cmp	r4, #0
	bne.n	.L_0814eab0
	ldr	r0, [pc, #356]
	b.n	.L_0814eabc
.L_0814eab0:
	ldr	r0, [sp, #56]
	cmp	r0, #1
	bne.n	.L_0814eaba
	ldr	r0, [pc, #352]
	b.n	.L_0814eabc
.L_0814eaba:
	ldr	r0, [pc, #352]
.L_0814eabc:
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #344]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b55
	movs	r1, #0
	movs	r2, #128
	mov	r8, r1
	lsls	r2, r2, #3
	subs	r1, #1
.L_0814eada:
	movs	r4, #1
	add	r8, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_0814eada
	movs	r0, #0
	str	r0, [sp, #36]
	mov	r1, sl
	ldr	r3, [r1, #20]
	cmp	r3, #0
	beq.n	.L_0814eb96
	str	r0, [sp, #8]
	movs	r2, #36
	movs	r7, #255
	mov	fp, r2
.L_0814eafa:
	mov	r3, fp
	mov	r1, sl
	ldrsh	r0, [r3, r1]
	bl	sub_08118098
	mov	r2, fp
	mov	r4, sl
	ldr	r6, [r0, #0]
	ldrsh	r0, [r2, r4]
	bl	sub_08118070
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	ldr	r1, [sp, #8]
	ldr	r2, [pc, #272]
	asrs	r0, r0, #1
	mov	r9, r0
	movs	r0, #0
	mov	r8, r0
	adds	r5, r1, r2
.L_0814eb22:
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	add	r3, r9
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #128
	lsls	r0, r0, #10
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #128
	lsls	r0, r0, #10
	str	r0, [r5, #16]
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #128
	lsls	r3, r0, #10
	str	r3, [r5, #20]
	ldr	r3, [sp, #56]
	cmp	r3, #2
	bne.n	.L_0814eb6a
	ldr	r3, [r5, #12]
	lsls	r3, r3, #1
	str	r3, [r5, #12]
	ldr	r3, [r5, #16]
	lsls	r3, r3, #1
	str	r3, [r5, #16]
	lsls	r3, r0, #11
	str	r3, [r5, #20]
.L_0814eb6a:
	movs	r4, #1
	add	r8, r4
	movs	r3, #0
	mov	r0, r8
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r0, #128
	bne.n	.L_0814eb22
	ldr	r2, [sp, #8]
	ldr	r4, [sp, #36]
	movs	r3, #224
	lsls	r3, r3, #4
	adds	r2, r2, r3
	adds	r4, #1
	str	r2, [sp, #8]
	str	r4, [sp, #36]
	mov	r0, sl
	ldr	r3, [r0, #20]
	movs	r1, #2
	add	fp, r1
	cmp	r4, r3
	bne.n	.L_0814eafa
.L_0814eb96:
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	ldr	r1, [sp, #56]
	str	r3, [sp, #40]
	cmp	r1, #2
	bne.n	.L_0814ec2c
	movs	r7, #160
	lsls	r7, r7, #19
	movs	r2, #1
	movs	r3, #21
	adds	r7, #2
	mov	r8, r2
	mov	r9, r3
.L_0814ebba:
	mov	r6, r8
	adds	r6, #1
	movs	r1, #3
	adds	r0, r6, #0
	bl	sub_08002054
	mov	r4, r9
	adds	r5, r0, #0
	movs	r1, #3
	mov	r0, r8
	subs	r5, r4, r5
	bl	sub_08002054
	mov	r1, r9
	subs	r0, r1, r0
	lsls	r0, r0, #10
	orrs	r0, r5
	strh	r0, [r7, #0]
	mov	r8, r6
	adds	r7, #2
	cmp	r6, #64
	bne.n	.L_0814ebba
	ldr	r3, [pc, #36]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [sp, #52]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r0, [sp, #52]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #132
	adds	r2, r0, r1
	movs	r3, #50
	b.n	.L_0814ec44
	movs	r0, r0
	.4byte 0x00000610
	.4byte 0x00000134
	.4byte 0x0000017d
	.4byte 0x00000184
	.4byte 0x00000165
	.4byte 0x03000730
	.4byte 0x02010018
	.2byte 0x0000
	.2byte 0x0201
.L_0814ec2c:
	ldr	r3, [sp, #52]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #3
	str	r3, [r2, #0]
	ldr	r0, [sp, #52]
	movs	r1, #238
	lsls	r1, r1, #7
	ldr	r3, [pc, #564]
	adds	r1, #132
	adds	r2, r0, r1
.L_0814ec44:
	str	r3, [r2, #0]
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #556]
	bl	sub_080145a8
	movs	r0, #142
	bl	sub_081c0010
	movs	r2, #0
	str	r2, [sp, #44]
	mov	r3, sl
	ldr	r2, [r3, #20]
	movs	r4, #72
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #2
	negs	r4, r4
	cmp	r3, r4
	bne.n	.L_0814ec6e
	b.n	.L_0814ee56
.L_0814ec6e:
	ldr	r0, [sp, #28]
	adds	r0, #12
	str	r0, [sp, #20]
.L_0814ec74:
	mov	r1, sl
	ldr	r0, [r1, #8]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	mov	r2, sl
	mov	fp, r0
	ldr	r0, [r2, #8]
	bl	sub_08118070
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	ldr	r3, [sp, #44]
	asrs	r0, r0, #1
	str	r0, [sp, #24]
	cmp	r3, #64
	bne.n	.L_0814ec9c
	movs	r0, #133
	bl	sub_081180e8
.L_0814ec9c:
	bl	sub_08014de4
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #20]
	bl	sub_080156e8
	ldr	r4, [sp, #44]
	cmp	r4, #40
	bne.n	.L_0814ecc2
	mov	r1, sl
	movs	r3, #0
	movs	r2, #1
	ldr	r0, [r1, #8]
	negs	r2, r2
	str	r3, [sp, #0]
	movs	r1, #7
	subs	r3, #1
	bl	sub_0814cd48
.L_0814ecc2:
	mov	r3, sl
	ldr	r2, [r3, #20]
	ldr	r4, [sp, #44]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, #52
	cmp	r4, r3
	bne.n	.L_0814ecec
	mov	r1, sl
	movs	r3, #0
	movs	r2, #1
	ldr	r0, [r1, #8]
	str	r3, [sp, #0]
	negs	r2, r2
	subs	r3, #1
	movs	r1, #0
	bl	sub_0814cd48
	mov	r3, sl
	ldr	r2, [r3, #20]
.L_0814ecec:
	movs	r4, #0
	str	r4, [sp, #36]
	cmp	r2, #0
	bne.n	.L_0814ecf6
	b.n	.L_0814ee24
.L_0814ecf6:
	movs	r0, #36
	str	r0, [sp, #16]
	str	r4, [sp, #12]
	mov	r9, r4
.L_0814ecfe:
	ldr	r1, [sp, #44]
	cmp	r1, r9
	bne.n	.L_0814ed18
	ldr	r2, [sp, #16]
	mov	r4, sl
	ldrsh	r0, [r2, r4]
	movs	r3, #42
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	ldr	r3, [sp, #36]
	bl	sub_0814cd48
.L_0814ed18:
	ldr	r0, [sp, #44]
	cmp	r0, r9
	ble.n	.L_0814ee00
	ldr	r2, [sp, #12]
	ldr	r3, [pc, #344]
	movs	r1, #0
	mov	r8, r1
	adds	r7, r2, r3
.L_0814ed28:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	blt.n	.L_0814edf4
	ldr	r3, [pc, #336]
	ldr	r4, [sp, #56]
	add	r6, sp, #60
	adds	r0, r7, #0
	adds	r1, r6, #0
	ldrb	r5, [r3, r4]
	bl	sub_0815e1ec
	ldr	r3, [r6, #0]
	asrs	r4, r3, #1
	str	r4, [r6, #0]
	ldr	r0, [sp, #56]
	cmp	r0, #2
	bne.n	.L_0814ed6e
	ldr	r3, [r6, #4]
	lsls	r1, r5, #1
	lsrs	r2, r5, #1
	subs	r2, r4, r2
	str	r1, [sp, #4]
	subs	r3, r3, r5
	ldr	r1, [sp, #32]
	str	r5, [sp, #0]
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #40]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c38
	movs	r1, #61
	movs	r2, #0
	bl	sub_08138058
	b.n	.L_0814ed98
.L_0814ed6e:
	ldr	r2, [pc, #276]
	lsls	r0, r5, #1
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #32]
	ldr	r3, [r6, #4]
	adds	r1, r2, r1
	lsrs	r2, r5, #1
	subs	r2, r4, r2
	str	r0, [sp, #4]
	subs	r3, r3, r5
	str	r5, [sp, #0]
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #40]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c38
	movs	r1, #62
	movs	r2, #0
	bl	sub_08138058
.L_0814ed98:
	mov	r3, r9
	ldr	r0, [sp, #44]
	add	r3, r8
	adds	r3, #10
	cmp	r0, r3
	ble.n	.L_0814edf4
	mov	r1, fp
	ldr	r0, [r1, #8]
	ldr	r3, [r7, #0]
	ldr	r2, [sp, #24]
	ldr	r1, [r1, #12]
	subs	r0, r0, r3
	ldr	r3, [r7, #4]
	adds	r1, r1, r2
	subs	r1, r1, r3
	mov	r3, fp
	ldr	r2, [r3, #16]
	ldr	r3, [r7, #8]
	asrs	r0, r0, #8
	subs	r2, r2, r3
	ldr	r3, [r7, #12]
	asrs	r1, r1, #8
	adds	r3, r3, r0
	str	r3, [r7, #12]
	ldr	r3, [r7, #16]
	movs	r4, #240
	adds	r3, r3, r1
	str	r3, [r7, #16]
	ldr	r3, [r7, #20]
	lsls	r4, r4, #4
	movs	r1, #248
	asrs	r2, r2, #8
	adds	r4, #255
	lsls	r1, r1, #5
	adds	r3, r3, r2
	adds	r0, r0, r4
	adds	r1, #254
	str	r3, [r7, #20]
	cmp	r0, r1
	bhi.n	.L_0814edf4
	adds	r3, r2, r4
	cmp	r3, r1
	bhi.n	.L_0814edf4
	movs	r0, #1
	negs	r0, r0
	str	r0, [r7, #24]
.L_0814edf4:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r7, #28
	cmp	r2, #32
	bne.n	.L_0814ed28
.L_0814ee00:
	ldr	r3, [sp, #16]
	ldr	r0, [sp, #12]
	ldr	r2, [sp, #36]
	movs	r1, #224
	lsls	r1, r1, #4
	adds	r3, #2
	adds	r0, r0, r1
	adds	r2, #1
	str	r3, [sp, #16]
	str	r0, [sp, #12]
	str	r2, [sp, #36]
	movs	r4, #20
	add	r9, r4
	mov	r4, sl
	ldr	r3, [r4, #20]
	cmp	r2, r3
	beq.n	.L_0814ee24
	b.n	.L_0814ecfe
.L_0814ee24:
	bl	sub_081434f8
	movs	r1, #240
	ldr	r0, [sp, #52]
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r2, r0, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #44]
	mov	r3, sl
	adds	r2, #1
	str	r2, [sp, #44]
	ldr	r4, [sp, #44]
	ldr	r2, [r3, #20]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, #72
	cmp	r4, r3
	beq.n	.L_0814ee56
	b.n	.L_0814ec74
.L_0814ee56:
	ldr	r0, [pc, #32]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #72
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x04040404
	.4byte 0x08143001
	.4byte 0x02010000
	.4byte 0x081982f4
	.2byte 0x7410
	.2byte 0x0819
	push	{lr}
	movs	r1, #0
	bl	sub_0814ef44
	pop	{pc}
