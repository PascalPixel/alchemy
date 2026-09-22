.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014dac, 0x08014dac
	.set sub_08014de4, 0x08014de4
	.set sub_080150e4, 0x080150e4
	.set sub_08015160, 0x08015160
	.set sub_0801521c, 0x0801521c
	.set sub_08118098, 0x08118098
	.set sub_081180d8, 0x081180d8
	.set sub_081180e8, 0x081180e8
	.set sub_08138058, 0x08138058
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cd48, 0x0814cd48
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e21c, 0x0815e21c
	.set sub_0815f000, 0x0815f000
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Overlay_0817b66c
Overlay_0817b66c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #92]
	mov	fp, r0
	movs	r0, #0
	sub	sp, #32
	mov	sl, r3
	bl	sub_081435e0
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #132]
	add	r1, sl
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	movs	r2, #0
	add	r3, sl
	str	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #80
	movs	r1, #200
	strh	r2, [r3, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #92]
	bl	sub_080145a8
	mov	r2, fp
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	bl	sub_08118098
	mov	r1, fp
	add	r5, sp, #20
	movs	r3, #36
	ldrsh	r0, [r1, r3]
	adds	r1, r5, #0
	bl	sub_0815e21c
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r2, [r5, #0]
	movs	r1, #128
	movs	r3, #64
	subs	r3, r3, r2
	lsls	r1, r1, #19
	lsls	r3, r3, #8
	adds	r1, #40
	str	r3, [r1, #0]
	mov	r3, sp
	adds	r3, #12
	str	r3, [sp, #8]
	movs	r2, #0
	mov	r9, r2
.L_0817b700:
	mov	r1, r9
	cmp	r1, #0
	bne.n	.L_0817b77c
	movs	r2, #0
	movs	r6, #128
	movs	r7, #0
	mov	r8, r2
	lsls	r6, r6, #16
	mov	r5, sl
	b.n	.L_0817b720
	.4byte 0x00000100
	.4byte 0x00000139
	.2byte 0x3001
	.2byte 0x0814
.L_0817b720:
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	adds	r3, #32
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	str	r6, [r5, #4]
	bl	sub_08014878
	mov	r3, r8
	str	r3, [r5, #12]
	str	r0, [r5, #8]
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	adds	r3, #32
	negs	r3, r3
	lsls	r3, r3, #10
	str	r3, [r5, #16]
	bl	sub_08014878
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	movs	r1, #128
	ands	r3, r0
	lsls	r1, r1, #3
	adds	r2, r3, r1
	movs	r3, #1
	ands	r3, r7
	str	r2, [r5, #20]
	cmp	r3, #0
	beq.n	.L_0817b76a
	negs	r3, r2
	str	r3, [r5, #20]
.L_0817b76a:
	movs	r3, #128
	mov	r2, r8
	lsls	r3, r3, #12
	adds	r7, #1
	str	r2, [r5, #24]
	adds	r6, r6, r3
	adds	r5, #28
	cmp	r7, #32
	bne.n	.L_0817b720
.L_0817b77c:
	bl	sub_08014de4
	ldr	r2, [pc, #452]
	movs	r3, #104
	str	r3, [r2, #16]
	movs	r7, #0
	movs	r5, #20
.L_0817b78a:
	cmp	r9, r5
	bne.n	.L_0817b7ce
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, sl
	movs	r3, #4
	str	r3, [r2, #0]
	movs	r0, #133
	bl	sub_081c0010
	mov	r2, fp
	movs	r3, #6
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #0
	movs	r2, #5
	bl	sub_0814cd48
	mov	r1, fp
	movs	r3, #36
	ldrsh	r0, [r1, r3]
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [sp, #0]
	movs	r3, #100
	str	r3, [sp, #4]
	movs	r1, #1
	movs	r2, #0
	movs	r3, #0
	bl	sub_0815f000
.L_0817b7ce:
	adds	r7, #1
	adds	r5, #6
	cmp	r7, #8
	bne.n	.L_0817b78a
	mov	r2, r9
	cmp	r2, #90
	bne.n	.L_0817b81c
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	movs	r3, #4
	add	r2, sl
	str	r3, [r2, #0]
	movs	r0, #134
	bl	sub_081180e8
	mov	r1, fp
	movs	r3, #36
	ldrsh	r0, [r1, r3]
	movs	r3, #10
	movs	r2, #5
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #0
	bl	sub_0814cd48
	mov	r3, fp
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [sp, #0]
	movs	r3, #100
	str	r3, [sp, #4]
	movs	r1, #1
	movs	r2, #0
	movs	r3, #0
	bl	sub_0815f000
.L_0817b81c:
	movs	r0, #32
	bl	sub_08014dac
	mov	r8, r0
	movs	r0, #1
	bl	sub_081969f8
	movs	r3, #0
	adds	r6, r0, #0
	str	r3, [r6, #20]
	ldr	r2, [pc, #280]
	ldr	r3, [sp, #12]
	ldr	r1, [sp, #8]
	ands	r3, r2
	movs	r2, #5
	orrs	r3, r2
	ldr	r2, [pc, #272]
	str	r1, [r6, #16]
	ands	r3, r2
	movs	r2, #160
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #12]
	movs	r3, #224
	lsls	r3, r3, #3
	add	r3, sl
	str	r3, [r1, #4]
	movs	r3, #9
	str	r3, [r6, #0]
	ldr	r3, [pc, #252]
	mov	r2, r8
	str	r3, [r6, #8]
	str	r2, [r6, #12]
	movs	r7, #0
	mov	r5, sl
.L_0817b862:
	mov	r3, r9
	cmp	r3, #80
	bne.n	.L_0817b89c
	ldr	r3, [pc, #236]
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #192
	lsls	r3, r3, #2
	ldr	r1, [pc, #228]
	adds	r3, #255
	ands	r3, r0
	lsls	r2, r7, #10
	adds	r2, r2, r3
	adds	r2, r2, r1
	movs	r1, #128
	lsls	r3, r2, #10
	lsls	r1, r1, #15
	adds	r3, r3, r1
	str	r3, [r5, #0]
	movs	r3, #192
	lsls	r3, r3, #12
	str	r2, [r5, #8]
	negs	r2, r2
	lsls	r2, r2, #5
	str	r3, [r5, #16]
	movs	r3, #0
	str	r2, [r5, #12]
	str	r3, [r5, #20]
.L_0817b89c:
	bl	sub_08014de4
	ldr	r3, [pc, #188]
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	adds	r0, r0, r3
	adds	r1, r1, r3
	movs	r2, #0
	bl	sub_08015160
	movs	r0, #128
	lsls	r0, r0, #8
	bl	sub_0801521c
	ldr	r0, [r5, #8]
	bl	sub_080150e4
	movs	r2, #4
	ldr	r0, [pc, #160]
	mov	r1, r8
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
	mov	r2, r9
	cmp	r2, #79
	ble.n	.L_0817b8e0
	adds	r0, r5, #0
	movs	r1, #64
	movs	r2, #0
	bl	sub_08138058
	b.n	.L_0817b8ea
.L_0817b8e0:
	adds	r0, r5, #0
	movs	r1, #64
	ldr	r2, [pc, #128]
	bl	sub_08138058
.L_0817b8ea:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #8
	bne.n	.L_0817b862
	adds	r0, r6, #0
	bl	sub_08013164
	mov	r0, r8
	bl	sub_08013164
	ldr	r2, [pc, #72]
	movs	r3, #120
	str	r3, [r2, #12]
	str	r3, [r2, #16]
	movs	r1, #4
	movs	r0, #4
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	movs	r3, #1
	add	r2, sl
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #1
	add	r9, r3
	mov	r1, r9
	cmp	r1, #96
	beq.n	.L_0817b930
	b.n	.L_0817b700
.L_0817b930:
	ldr	r0, [pc, #56]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x030011e0
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199450
	.4byte 0xffd00000
	.4byte 0xffffee00
	.4byte 0xffc00000
	.4byte 0x08199474
	.4byte 0xfffff000
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r1, #0
	bl	sub_08118098
	movs	r3, #3
	movs	r2, #13
	ands	r5, r3
	negs	r2, r2
	adds	r6, r0, #0
	movs	r7, #0
	lsls	r5, r5, #2
	mov	r8, r2
	b.n	.L_0817b9a4
.L_0817b98e:
	adds	r3, r6, #0
	adds	r3, #42
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0817b9a2
	ldrb	r3, [r0, #9]
	mov	r2, r8
	ands	r3, r2
	orrs	r3, r5
	strb	r3, [r0, #9]
.L_0817b9a2:
	adds	r7, #1
.L_0817b9a4:
	ldr	r0, [r6, #0]
	adds	r1, r7, #0
	bl	sub_081180d8
	cmp	r0, #0
	bne.n	.L_0817b98e
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
