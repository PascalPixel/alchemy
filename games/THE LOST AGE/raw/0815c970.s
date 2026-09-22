.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
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
	.set sub_0801521c, 0x0801521c
	.set sub_08020010, 0x08020010
	.set sub_08020048, 0x08020048
	.set sub_08020060, 0x08020060
	.set sub_08118040, 0x08118040
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180d8, 0x081180d8
	.set sub_081180e8, 0x081180e8
	.set sub_081180f0, 0x081180f0
	.set sub_08118120, 0x08118120
	.set sub_08118148, 0x08118148
	.set sub_08138058, 0x08138058
	.set sub_08138086, 0x08138086
	.set sub_0813ba50, 0x0813ba50
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cc4c, 0x0814cc4c
	.set sub_0814cd48, 0x0814cd48
	.set sub_081504c0, 0x081504c0
	.set sub_08152404, 0x08152404
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815b3b0, 0x0815b3b0
	.set sub_0815c9f4, 0x0815c9f4
	.set sub_0815cae4, 0x0815cae4
	.set sub_0815e20c, 0x0815e20c
	.set sub_0815e22c, 0x0815e22c
	.set sub_0815f000, 0x0815f000
	.set sub_0816ae40, 0x0816ae40
	.set sub_0818de3c, 0x0818de3c
	.set sub_081963ec, 0x081963ec
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Overlay_0815c970
Overlay_0815c970:
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
	ldr	r0, [r5, #92]
	ldr	r2, [r5, #96]
	sub	sp, #28
	mov	sl, r0
	movs	r0, #0
	str	r2, [sp, #12]
	mov	fp, r1
	bl	sub_081435e0
	movs	r3, #128
	ldr	r2, [pc, #20]
	lsls	r3, r3, #19
	adds	r3, #32
	mov	r0, r8
	strh	r2, [r3, #0]
	ldr	r3, [r0, #4]
	cmp	r3, #1
	bne.n	.L_0815c9b4
	movs	r0, #104
	movs	r1, #35
	b.n	.L_0815c9b8
	movs	r0, r0
	.2byte 0x0100
	.2byte 0x0000
.L_0815c9b4:
	movs	r0, #104
	movs	r1, #39
.L_0815c9b8:
	bl	sub_081963ec
	ldr	r6, [r5, #104]
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #480]
	add	r1, sl
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r2, #1
	ldr	r0, [pc, #468]
	ldr	r1, [pc, #472]
	movs	r3, #0
	bl	sub_08157cf4
	mov	r2, fp
	cmp	r2, #0
	bne.n	sub_0815c9f4
	ldr	r0, [pc, #460]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #456]
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
	ldr	r0, [pc, #424]
	bl	sub_080145a8
	mov	r2, r8
	movs	r3, #36
	ldrsh	r0, [r2, r3]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	mov	r2, r8
	add	r5, sp, #16
	mov	r9, r0
	adds	r1, r5, #0
	movs	r3, #36
	ldrsh	r0, [r2, r3]
	bl	sub_0815e20c
	mov	r0, r8
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne.n	.L_0815ca40
	ldr	r2, [r5, #0]
	movs	r1, #128
	movs	r3, #16
	b.n	.L_0815ca46
.L_0815ca40:
	ldr	r2, [r5, #0]
	movs	r1, #128
	movs	r3, #112
.L_0815ca46:
	lsls	r1, r1, #19
	subs	r3, r3, r2
	adds	r1, #40
	lsls	r3, r3, #8
	str	r3, [r1, #0]
	movs	r2, #74
	mov	r3, fp
	str	r2, [sp, #8]
	cmp	r3, #1
	beq.n	.L_0815ca5e
	movs	r0, #48
	str	r0, [sp, #8]
.L_0815ca5e:
	ldr	r2, [sp, #8]
	movs	r5, #0
	cmp	r2, #0
	bne.n	.L_0815ca68
	b.n	.L_0815cb86
.L_0815ca68:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_0815ca70
	adds	r3, r5, #3
.L_0815ca70:
	asrs	r4, r3, #2
	cmp	r4, #5
	bgt.n	sub_0815cae4
	cmp	r4, #3
	bgt.n	.L_0815cab2
	ldr	r0, [pc, #320]
	lsls	r3, r4, #1
	ldrh	r1, [r0, r3]
	movs	r2, #224
	lsls	r2, r2, #3
	mov	r3, r8
	add	r1, sl
	adds	r1, r1, r2
	ldr	r2, [r3, #4]
	ldr	r0, [pc, #304]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r3, r4, r3
	ldrb	r2, [r0, r3]
	ldr	r0, [pc, #296]
	ldr	r3, [pc, #300]
	ldrb	r0, [r0, r4]
	ldrsb	r3, [r3, r4]
	str	r0, [sp, #0]
	ldr	r0, [pc, #296]
	adds	r3, #32
	ldrb	r0, [r0, r4]
	str	r0, [sp, #4]
	ldr	r0, [sp, #12]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0xe018
.L_0815cab2:
	ldr	r2, [pc, #264]
	lsls	r3, r4, #1
	mov	r0, r8
	ldrh	r1, [r2, r3]
	ldr	r2, [r0, #4]
	ldr	r3, [pc, #236]
	ldr	r0, [pc, #256]
	adds	r1, r1, r3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r3, r4, r3
	ldrb	r2, [r0, r3]
	ldr	r0, [pc, #244]
	ldr	r3, [pc, #248]
	ldrb	r0, [r0, r4]
	ldrsb	r3, [r3, r4]
	str	r0, [sp, #0]
	ldr	r0, [pc, #244]
	adds	r3, #32
	ldrb	r0, [r0, r4]
	str	r0, [sp, #4]
	ldr	r0, [sp, #12]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x2d08
	bne.n	.L_0815cb28
	mov	r2, fp
	cmp	r2, #0
	bne.n	.L_0815cb02
	movs	r0, #133
	bl	sub_081180e8
	mov	r2, r8
	movs	r3, #36
	ldrsh	r0, [r2, r3]
	movs	r1, #1
	bl	sub_08118088
	b.n	.L_0815cb1c
.L_0815cb02:
	movs	r0, #134
	bl	sub_081c0010
	mov	r2, r8
	movs	r3, #36
	ldrsh	r0, [r2, r3]
	movs	r3, #4
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
.L_0815cb1c:
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, sl
	movs	r3, #8
	str	r3, [r2, #0]
.L_0815cb28:
	mov	r3, fp
	cmp	r3, #1
	bne.n	.L_0815cb5e
	cmp	r5, #13
	bne.n	.L_0815cb48
	movs	r3, #192
	mov	r0, r9
	lsls	r3, r3, #12
	str	r3, [r0, #40]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #81
	str	r3, [r0, #72]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r0, #68]
.L_0815cb48:
	cmp	r5, #65
	bne.n	.L_0815cb5e
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, sl
	movs	r3, #4
	str	r3, [r2, #0]
	movs	r0, #134
	bl	sub_081180e8
.L_0815cb5e:
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, sl
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #8]
	adds	r5, #1
	cmp	r5, r2
	beq.n	.L_0815cb86
	b.n	.L_0815ca68
.L_0815cb86:
	ldr	r0, [pc, #48]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x00000132
	.4byte 0x00000133
	.4byte 0x02010000
	.4byte 0x00000163
	.4byte 0x03000730
	.4byte 0x08143001
	.4byte 0x081986d4
	.4byte 0x081986e0
	.4byte 0x081986c8
	.4byte 0x081986ec
	.2byte 0x86ce
	.2byte 0x0819
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r6, #192
	lsls	r6, r6, #18
	ldr	r2, [r6, #96]
	ldr	r3, [r6, #92]
	mov	r9, r0
	movs	r0, #2
	sub	sp, #20
	mov	sl, r3
	mov	fp, r2
	bl	sub_081435e0
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	add	r5, sp, #8
	adds	r2, #50
	strh	r3, [r2, #0]
	adds	r0, r5, #0
	bl	sub_0815e22c
	ldr	r2, [r5, #0]
	movs	r1, #128
	movs	r3, #64
	subs	r3, r3, r2
	lsls	r1, r1, #19
	adds	r1, #40
	lsls	r3, r3, #8
	str	r3, [r1, #0]
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #20]
	add	r1, sl
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r2, #239
	b.n	.L_0815cc3c
	.4byte 0x00000100
	.4byte 0x00001000
	.2byte 0x011b
	.2byte 0x0000
.L_0815cc3c:
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	movs	r3, #0
	add	r2, sl
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #64]
	bl	sub_080145a8
	movs	r0, #143
	bl	sub_081c0010
	movs	r3, #32
	movs	r7, #0
	mov	r8, r3
.L_0815cc66:
	cmp	r7, #8
	bgt.n	.L_0815cc78
	ldr	r2, [pc, #36]
	lsls	r3, r7, #1
	orrs	r3, r2
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
.L_0815cc78:
	cmp	r7, #53
	ble.n	.L_0815cc9c
	ldr	r2, [pc, #20]
	lsls	r3, r7, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #12]
	orrs	r2, r3
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	strh	r2, [r3, #0]
	b.n	.L_0815cc9c
	.4byte 0x00001000
	.4byte 0x0000007c
	.2byte 0x3001
	.2byte 0x0814
.L_0815cc9c:
	movs	r1, #35
	movs	r0, #104
	bl	sub_081963ec
	mov	r2, r8
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #192
	movs	r1, #224
	lsls	r3, r3, #18
	lsls	r1, r1, #3
	ldr	r4, [r3, #104]
	movs	r2, #33
	movs	r3, #41
	add	r1, sl
	mov	r0, fp
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2068
	bl	sub_0801314c
	movs	r1, #39
	movs	r0, #104
	bl	sub_081963ec
	mov	r2, r8
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #192
	movs	r1, #224
	lsls	r3, r3, #18
	lsls	r1, r1, #3
	ldr	r4, [r3, #104]
	movs	r2, #64
	movs	r3, #41
	add	r1, sl
	mov	r0, fp
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2068
	bl	sub_0801314c
	movs	r1, #43
	movs	r0, #104
	bl	sub_081963ec
	mov	r2, r8
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #192
	movs	r1, #224
	lsls	r3, r3, #18
	lsls	r1, r1, #3
	ldr	r4, [r3, #104]
	movs	r2, #33
	movs	r3, #72
	add	r1, sl
	mov	r0, fp
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2068
	bl	sub_0801314c
	movs	r1, #47
	movs	r0, #104
	bl	sub_081963ec
	mov	r2, r8
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #192
	movs	r1, #224
	lsls	r3, r3, #18
	lsls	r1, r1, #3
	ldr	r4, [r3, #104]
	mov	r0, fp
	add	r1, sl
	movs	r2, #64
	movs	r3, #72
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2068
	bl	sub_0801314c
	cmp	r7, #32
	bne.n	.L_0815cd4e
	movs	r0, #143
	bl	sub_081180e8
.L_0815cd4e:
	mov	r2, r9
	ldr	r3, [r2, #20]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_0815cd7e
	movs	r6, #36
.L_0815cd5a:
	cmp	r7, #10
	bne.n	.L_0815cd76
	mov	r3, r9
	ldrsh	r0, [r6, r3]
	movs	r3, #8
	movs	r2, #1
	str	r3, [sp, #0]
	negs	r2, r2
	adds	r3, r5, #0
	movs	r1, #7
	bl	sub_0814cd48
	mov	r2, r9
	ldr	r3, [r2, #20]
.L_0815cd76:
	adds	r5, #1
	adds	r6, #2
	cmp	r5, r3
	bne.n	.L_0815cd5a
.L_0815cd7e:
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, sl
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	adds	r7, #1
	bl	sub_08013560
	cmp	r7, #63
	beq.n	.L_0815cd9c
	b.n	.L_0815cc66
.L_0815cd9c:
	ldr	r0, [pc, #56]
	bl	sub_08014644
	movs	r1, #240
	ldr	r5, [pc, #52]
	lsls	r1, r1, #6
	ldr	r0, [pc, #52]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x21f0
	lsls	r1, r1, #6
	mov	r0, fp
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2300
	mov	r2, r9
	str	r3, [r2, #28]
	ldr	r0, [pc, #36]
	bl	sub_08014644
	mov	r0, r9
	bl	sub_081504c0
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x08143001
	.4byte 0x03000258
	.4byte 0x06004000
	.2byte 0x3489
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #120
	str	r0, [sp, #84]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #96]
	str	r0, [sp, #80]
	ldr	r1, [r5, #92]
	str	r1, [sp, #76]
	ldr	r6, [r5, #100]
	bl	sub_0813ba50
	movs	r0, #3
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	movs	r1, #3
	movs	r0, #104
	bl	sub_081963ec
	ldr	r2, [r5, #104]
	adds	r1, r6, #0
	str	r2, [sp, #68]
	ldr	r0, [pc, #40]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r6, #240
	ldr	r4, [sp, #76]
	lsls	r6, r6, #7
	adds	r6, #240
	adds	r3, r4, r6
	ldr	r0, [r3, #0]
	bl	sub_0814cc4c
	ldr	r7, [sp, #76]
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	lsls	r1, r1, #7
	b.n	.L_0815ce58
	.4byte 0x00003f44
	.2byte 0x0134
	.2byte 0x0000
.L_0815ce58:
	adds	r2, r7, r0
	movs	r3, #1
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r2, r7, r1
	movs	r1, #192
	movs	r3, #0
	lsls	r1, r1, #4
	str	r3, [r2, #0]
	adds	r1, #254
	ldr	r0, [pc, #396]
	bl	sub_080145a8
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #392]
	bl	sub_080145a8
	movs	r6, #152
	ldr	r5, [r5, #36]
	movs	r2, #224
	movs	r3, #143
	movs	r4, #240
	lsls	r2, r2, #3
	lsls	r3, r3, #4
	lsls	r4, r4, #4
	lsls	r6, r6, #5
	adds	r6, r7, r6
	adds	r2, r7, r2
	adds	r3, r7, r3
	adds	r4, r7, r4
	ldr	r1, [pc, #364]
	str	r6, [sp, #20]
	str	r5, [sp, #60]
	str	r2, [sp, #32]
	str	r3, [sp, #28]
	str	r4, [sp, #24]
	ldr	r6, [pc, #356]
	movs	r7, #0
	movs	r0, #7
	mov	r8, r7
	mov	r9, r0
	mov	sl, r1
.L_0815ceae:
	mov	r5, r8
	mov	r2, r9
	movs	r7, #0
	ands	r5, r2
.L_0815ceb6:
	mov	r1, r8
	cmp	r1, #0
	bge.n	.L_0815cebe
	adds	r1, #7
.L_0815cebe:
	asrs	r1, r1, #3
	lsls	r1, r1, #8
	ldr	r3, [pc, #328]
	adds	r1, r1, r5
	lsls	r1, r1, #3
	adds	r1, r1, r7
	adds	r0, r6, #0
	adds	r1, r1, r3
	movs	r2, #8
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x24e0
	lsls	r4, r4, #3
	adds	r7, #64
	adds	r4, #255
	adds	r6, #8
	cmp	r7, r4
	ble.n	.L_0815ceb6
	movs	r7, #1
	add	r8, r7
	mov	r0, r8
	cmp	r0, #119
	ble.n	.L_0815ceae
	ldr	r1, [sp, #76]
	movs	r2, #200
	lsls	r2, r2, #6
	adds	r2, #184
	adds	r5, r1, r2
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #1
	ldr	r0, [pc, #272]
	bl	sub_08157cf4
	movs	r6, #208
	ldr	r4, [sp, #76]
	lsls	r6, r6, #6
	movs	r3, #0
	movs	r1, #144
	adds	r6, #248
	mov	r8, r3
	lsls	r1, r1, #1
	adds	r2, r4, r6
.L_0815cf14:
	ldrb	r3, [r5, #0]
	movs	r7, #1
	add	r8, r7
	strb	r3, [r2, #0]
	adds	r5, #2
	adds	r2, #1
	cmp	r8, r1
	bne.n	.L_0815cf14
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #163
	lsls	r1, r1, #2
	movs	r0, #12
	movs	r2, #1
	bl	sub_08152404
	movs	r2, #238
	ldr	r1, [sp, #76]
	lsls	r2, r2, #7
	movs	r3, #13
	ldr	r7, [pc, #196]
	movs	r0, #0
	adds	r2, #220
	negs	r3, r3
	mov	r8, r0
	adds	r5, r1, r2
	adds	r6, r3, #0
.L_0815cf4c:
	movs	r1, #32
	ldr	r2, [pc, #196]
	movs	r3, #0
	movs	r0, #32
	bl	sub_0815b3b0
	mov	r4, r8
	str	r0, [r5, #48]
	movs	r3, #3
	movs	r1, #238
	ands	r3, r4
	ldr	r2, [sp, #76]
	lsls	r1, r1, #7
	adds	r1, #240
	lsls	r3, r3, #2
	adds	r3, r3, r1
	ldr	r1, [r2, r3]
	movs	r2, #24
	mov	lr, r7
	.2byte 0xf800
	.2byte 0xcd02
	movs	r2, #4
	ldrb	r3, [r1, #9]
	ands	r3, r6
	orrs	r3, r2
	strb	r3, [r1, #9]
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	cmp	r4, #16
	bne.n	.L_0815cf4c
	movs	r6, #0
	str	r6, [sp, #64]
.L_0815cf8e:
	ldr	r3, [pc, #136]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0815d024
	ldr	r7, [sp, #64]
	cmp	r7, #15
	ble.n	.L_0815d024
	cmp	r7, #185
	bgt.n	.L_0815d024
	movs	r1, #128
	ldr	r3, [pc, #116]
	ldr	r0, [sp, #80]
	lsls	r1, r1, #7
	ldr	r2, [pc, #112]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x21f0
	ldr	r0, [sp, #76]
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r2, r0, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #186
	str	r2, [sp, #64]
	ldr	r3, [pc, #44]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r4, [sp, #76]
	movs	r6, #238
	lsls	r6, r6, #7
	movs	r3, #0
	movs	r1, #128
	adds	r6, #220
	mov	r8, r3
	lsls	r1, r1, #7
	adds	r2, r4, r6
.L_0815cfe6:
	ldmia	r2!, {r3}
	movs	r7, #1
	add	r8, r7
	mov	r0, r8
	strh	r1, [r3, #18]
	cmp	r0, #28
	bne.n	.L_0815cfe6
	b.n	.L_0815d024
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x08164bb5
	.4byte 0x08143001
	.4byte 0x03000730
	.4byte 0x02010000
	.4byte 0x06008000
	.4byte 0x00000190
	.4byte 0x80002000
	.4byte 0x03001150
	.4byte 0x03000260
	.2byte 0x3f3f
	.2byte 0x3f3f
.L_0815d024:
	ldr	r1, [sp, #64]
	cmp	r1, #8
	bgt.n	.L_0815d058
	lsls	r3, r1, #1
	adds	r3, r3, r1
	movs	r2, #24
	subs	r2, r2, r3
	lsls	r3, r2, #4
	adds	r3, r3, r2
	lsls	r3, r3, #4
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r3, r3, #2
	lsls	r2, r2, #9
	subs	r2, r2, r3
	movs	r4, #160
	ldr	r3, [sp, #60]
	lsls	r4, r4, #3
	movs	r1, #160
	adds	r4, #108
	lsls	r1, r1, #19
	adds	r0, r3, r4
	adds	r1, #192
	movs	r3, #128
	bl	sub_081180f0
.L_0815d058:
	ldr	r6, [sp, #64]
	cmp	r6, #186
	bne.n	.L_0815d074
	movs	r0, #132
	ldr	r2, [pc, #80]
	movs	r7, #192
	lsls	r0, r0, #15
	lsls	r7, r7, #14
	adds	r0, #3
	movs	r1, #0
	str	r7, [sp, #52]
	str	r0, [sp, #56]
	str	r1, [sp, #36]
	str	r2, [sp, #40]
.L_0815d074:
	ldr	r3, [sp, #64]
	cmp	r3, #0
	beq.n	.L_0815d07c
	b.n	.L_0815d228
.L_0815d07c:
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r4, [sp, #76]
	movs	r6, #239
	lsls	r6, r6, #7
	adds	r2, r4, r6
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r7, #238
	lsls	r7, r7, #7
	ldr	r0, [sp, #64]
	adds	r7, #132
	adds	r3, r4, r7
	str	r0, [r3, #0]
	movs	r3, #238
	movs	r1, #0
	lsls	r3, r3, #7
	mov	r8, r1
	adds	r3, #220
	movs	r1, #192
	lsls	r1, r1, #8
	adds	r2, r4, r3
	b.n	.L_0815d0b8
	.4byte 0x00000010
	.2byte 0x0000
	.2byte 0xfffa
.L_0815d0b8:
	.2byte 0xca08
	movs	r4, #1
	add	r8, r4
	mov	r6, r8
	strh	r1, [r3, #18]
	cmp	r6, #28
	bne.n	.L_0815d0b8
	movs	r1, #0
	ldr	r0, [pc, #384]
	str	r1, [sp, #36]
	ldr	r1, [pc, #384]
	movs	r7, #240
	movs	r2, #192
	lsls	r7, r7, #15
	lsls	r2, r2, #12
	str	r7, [sp, #52]
	str	r0, [sp, #56]
	str	r2, [sp, #40]
	ldr	r4, [pc, #372]
	ldrh	r3, [r4, #0]
	adds	r0, r3, #0
	movs	r6, #130
	ldr	r7, [pc, #364]
	lsls	r6, r6, #2
	strh	r6, [r7, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_0815d112
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #238
	adds	r3, r3, r1
	lsls	r2, r2, #7
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_0815d112:
	ldr	r1, [pc, #320]
	strh	r0, [r1, #0]
	movs	r7, #0
	ldr	r4, [sp, #28]
	movs	r2, #0
	ldr	r3, [pc, #312]
	str	r2, [sp, #16]
	ldr	r5, [sp, #28]
	mov	lr, r2
	adds	r4, #12
	mov	fp, r2
	mov	r9, r3
	mov	sl, r2
.L_0815d12c:
	mov	r1, sl
	ldr	r2, [pc, #296]
	adds	r3, r1, r7
	movs	r6, #0
	mov	r0, r9
	lsls	r3, r3, #1
	adds	r3, r3, r2
	mov	r8, r6
	ldrb	r6, [r0, #1]
	mov	r0, sl
	adds	r1, r3, #2
	lsls	r3, r0, #2
	ldr	r0, [sp, #32]
	adds	r2, r3, r0
	mov	r3, r9
	ldrb	r0, [r3, #0]
.L_0815d14c:
	ldrb	r3, [r1, #0]
	subs	r3, r3, r0
	strb	r3, [r2, #0]
	ldrb	r3, [r1, #1]
	adds	r1, #2
	subs	r3, r3, r6
	strb	r3, [r2, #1]
	mov	r3, lr
	strb	r3, [r2, #2]
	movs	r3, #1
	add	r8, r3
	mov	r3, r8
	adds	r2, #4
	cmp	r3, #4
	bne.n	.L_0815d14c
	movs	r3, #1
	strb	r3, [r5, #1]
	mov	r6, lr
	movs	r3, #2
	strb	r3, [r5, #2]
	strb	r6, [r5, #0]
	strb	r3, [r4, #1]
	movs	r3, #3
	strb	r3, [r4, #2]
	strb	r6, [r4, #0]
	strb	r6, [r4, #12]
	strb	r6, [r4, #13]
	strb	r6, [r4, #14]
	mov	r2, fp
	ldr	r6, [sp, #28]
	movs	r0, #0
	ldr	r1, [pc, #204]
	adds	r3, r2, r7
	mov	r8, r0
	lsls	r3, r3, #2
	ldr	r0, [sp, #16]
	adds	r2, r3, r6
	mov	ip, r1
	adds	r1, r2, #4
.L_0815d19a:
	ldrb	r3, [r2, #0]
	mov	r6, ip
	adds	r3, r0, r3
	lsls	r3, r3, #1
	adds	r3, #2
	ldrb	r3, [r6, r3]
	strb	r3, [r1, #0]
	ldrb	r3, [r2, #0]
	adds	r3, r0, r3
	lsls	r3, r3, #1
	adds	r3, #3
	ldrb	r3, [r6, r3]
	strb	r3, [r1, #1]
	ldrb	r3, [r2, #12]
	adds	r3, r0, r3
	lsls	r3, r3, #1
	adds	r3, #2
	ldrb	r3, [r6, r3]
	strb	r3, [r1, #12]
	ldrb	r3, [r2, #12]
	adds	r2, #1
	adds	r3, r0, r3
	lsls	r3, r3, #1
	adds	r3, #3
	ldrb	r3, [r6, r3]
	strb	r3, [r1, #13]
	movs	r3, #1
	add	r8, r3
	mov	r6, r8
	adds	r1, #2
	cmp	r6, #3
	bne.n	.L_0815d19a
	ldr	r0, [sp, #16]
	movs	r1, #8
	adds	r0, #5
	movs	r2, #10
	movs	r3, #4
	adds	r7, #1
	adds	r4, #36
	str	r0, [sp, #16]
	add	fp, r1
	adds	r5, #36
	add	r9, r2
	add	sl, r3
	cmp	r7, #31
	bne.n	.L_0815d12c
	ldr	r6, [sp, #84]
	movs	r4, #0
	ldr	r3, [r6, #20]
	mov	r8, r4
	cmp	r3, #0
	beq.n	.L_0815d21e
	movs	r6, #0
	movs	r5, #36
.L_0815d206:
	ldr	r7, [sp, #84]
	ldrsh	r0, [r5, r7]
	bl	sub_08118098
	ldr	r3, [r0, #0]
	movs	r2, #1
	str	r6, [r3, #12]
	ldr	r3, [r7, #20]
	add	r8, r2
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L_0815d206
.L_0815d21e:
	movs	r2, #128
	ldr	r3, [pc, #32]
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
.L_0815d228:
	ldr	r3, [sp, #64]
	subs	r3, #10
	mov	fp, r3
	cmp	r3, #16
	bhi.n	.L_0815d25c
	ldr	r1, [pc, #20]
	movs	r3, #128
	mov	r4, fp
	lsls	r3, r3, #19
	lsls	r2, r4, #8
	adds	r3, #82
	orrs	r2, r1
	strh	r2, [r3, #0]
	b.n	.L_0815d25c
	.4byte 0x00000787
	.4byte 0x00000010
	.4byte 0xff880000
	.4byte 0x020038e0
	.4byte 0x04000208
	.2byte 0x86f2
	.2byte 0x0819
.L_0815d25c:
	ldr	r3, [sp, #64]
	subs	r3, #16
	cmp	r3, #63
	bhi.n	.L_0815d334
	ldr	r6, [sp, #64]
	cmp	r6, #16
	bne.n	.L_0815d2c8
	ldr	r0, [sp, #76]
	movs	r1, #217
	movs	r7, #0
	lsls	r1, r1, #2
	mov	r8, r7
	movs	r6, #31
	adds	r5, r0, r1
.L_0815d278:
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	adds	r3, #32
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #120
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #16
	lsls	r0, r0, #13
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #63
	mov	r2, r8
	lsls	r0, r0, #12
	str	r0, [r5, #16]
	lsls	r0, r2, #9
	bl	sub_08002090
	movs	r3, #1
	lsls	r0, r0, #5
	asrs	r0, r0, #16
	add	r8, r3
	subs	r0, #32
	mov	r4, r8
	str	r0, [r5, #24]
	adds	r5, #28
	cmp	r4, #27
	bne.n	.L_0815d278
.L_0815d2c8:
	ldr	r7, [sp, #76]
	ldr	r1, [sp, #64]
	movs	r0, #217
	movs	r6, #0
	lsls	r0, r0, #2
	mov	r8, r6
	adds	r5, r7, r0
	lsls	r6, r1, #10
.L_0815d2d8:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_0815d324
	movs	r3, #208
	lsls	r3, r3, #5
	adds	r3, #22
	mov	r0, r8
	muls	r0, r3
	adds	r0, r6, r0
	bl	sub_08002096
	ldr	r2, [r5, #0]
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	ldr	r7, [sp, #76]
	lsls	r0, r0, #4
	movs	r4, #208
	adds	r2, r2, r0
	movs	r1, #12
	lsls	r4, r4, #6
	str	r1, [sp, #0]
	adds	r4, #248
	movs	r1, #24
	asrs	r2, r2, #16
	subs	r3, #12
	str	r1, [sp, #4]
	subs	r2, #6
	adds	r1, r7, r4
	ldr	r0, [sp, #80]
	ldr	r7, [sp, #68]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #63
	ldr	r2, [pc, #88]
	bl	sub_08138086
	ldr	r3, [r5, #24]
.L_0815d324:
	movs	r0, #1
	add	r8, r0
	adds	r3, #1
	mov	r1, r8
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r1, #27
	bne.n	.L_0815d2d8
.L_0815d334:
	ldr	r2, [sp, #64]
	cmp	r2, #95
	ble.n	.L_0815d342
	adds	r3, r2, #0
	subs	r3, #160
	cmp	r3, #31
	bhi.n	.L_0815d424
.L_0815d342:
	ldr	r3, [sp, #76]
	movs	r6, #236
	lsls	r6, r6, #7
	adds	r6, #64
	adds	r4, r3, r6
	ldr	r3, [sp, #64]
	cmp	r3, #0
	bge.n	.L_0815d354
	adds	r3, #7
.L_0815d354:
	ldr	r7, [sp, #64]
	asrs	r3, r3, #3
	mov	sl, r3
	cmp	r7, #159
	ble.n	.L_0815d37c
	adds	r3, r7, #0
	subs	r3, #160
	cmp	r3, #0
	bge.n	.L_0815d368
	adds	r3, #3
.L_0815d368:
	asrs	r3, r3, #2
	adds	r3, #12
	mov	sl, r3
	cmp	r3, #20
	ble.n	.L_0815d386
	movs	r0, #20
	mov	sl, r0
	b.n	.L_0815d386
	.2byte 0xc000
	.2byte 0xffff
.L_0815d37c:
	.2byte 0x4651
	cmp	r1, #12
	ble.n	.L_0815d386
	movs	r2, #12
	mov	sl, r2
.L_0815d386:
	movs	r3, #0
	mov	r8, r3
.L_0815d38a:
	movs	r6, #1
	add	r8, r6
	mov	r7, r8
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r7, #15
	bne.n	.L_0815d38a
.L_0815d398:
	mov	r6, r8
	subs	r6, #15
	movs	r1, #15
	adds	r0, r6, #0
	str	r4, [sp, #8]
	bl	sub_08002054
	mov	r1, sl
	negs	r0, r0
	subs	r0, r0, r1
	adds	r7, r0, #0
	adds	r7, #40
	adds	r3, r6, #0
	ldr	r4, [sp, #8]
	cmp	r6, #0
	bge.n	.L_0815d3ba
	mov	r3, r8
.L_0815d3ba:
	asrs	r3, r3, #4
	mov	r2, sl
	negs	r3, r3
	subs	r3, r3, r2
	adds	r0, r6, #0
	movs	r1, #17
	adds	r5, r3, #0
	str	r4, [sp, #8]
	bl	sub_08002054
	mov	r3, sl
	negs	r0, r0
	subs	r0, r0, r3
	adds	r5, #32
	adds	r0, #16
	ldr	r4, [sp, #8]
	cmp	r7, #0
	bge.n	.L_0815d3e0
	movs	r7, #0
.L_0815d3e0:
	cmp	r7, #31
	ble.n	.L_0815d3e6
	movs	r7, #31
.L_0815d3e6:
	cmp	r5, #0
	bge.n	.L_0815d3ec
	movs	r5, #0
.L_0815d3ec:
	cmp	r5, #31
	ble.n	.L_0815d3f2
	movs	r5, #31
.L_0815d3f2:
	cmp	r0, #0
	bge.n	.L_0815d3f8
	movs	r0, #0
.L_0815d3f8:
	cmp	r0, #31
	ble.n	.L_0815d3fe
	movs	r0, #31
.L_0815d3fe:
	lsls	r3, r7, #10
	lsls	r2, r5, #5
	movs	r6, #1
	orrs	r3, r2
	add	r8, r6
	orrs	r3, r0
	mov	r7, r8
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r7, #135
	bne.n	.L_0815d398
	ldr	r3, [pc, #48]
.L_0815d416:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r1, #160
	bne.n	.L_0815d416
.L_0815d424:
	ldr	r2, [sp, #64]
	cmp	r2, #0
	blt.n	.L_0815d43a
	ldr	r4, [sp, #52]
	ldr	r7, [sp, #56]
	ldr	r3, [sp, #36]
	ldr	r6, [sp, #40]
	adds	r3, r3, r4
	adds	r6, r6, r7
	str	r3, [sp, #52]
	str	r6, [sp, #56]
.L_0815d43a:
	ldr	r0, [sp, #64]
	cmp	r0, #168
	bne.n	.L_0815d4bc
	ldr	r4, [pc, #8]
	movs	r3, #0
	b.n	.L_0815d450
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0x0000
	.2byte 0xfffa
.L_0815d450:
	.2byte 0x21c0
	ldr	r6, [sp, #76]
	movs	r7, #238
	lsls	r1, r1, #14
	movs	r2, #168
	lsls	r7, r7, #7
	str	r3, [sp, #36]
	str	r4, [sp, #40]
	lsls	r2, r2, #16
	str	r1, [sp, #52]
	adds	r7, #220
	movs	r1, #128
	str	r2, [sp, #56]
	mov	r8, r3
	lsls	r1, r1, #7
	adds	r2, r6, r7
.L_0815d470:
	ldmia	r2!, {r3}
	movs	r0, #1
	add	r8, r0
	strh	r1, [r3, #18]
	mov	r3, r8
	cmp	r3, #28
	bne.n	.L_0815d470
	ldr	r6, [sp, #84]
	movs	r4, #0
	ldr	r3, [r6, #20]
	mov	r8, r4
	cmp	r3, #0
	beq.n	.L_0815d4bc
	movs	r6, #36
.L_0815d48c:
	ldr	r7, [sp, #84]
	ldrsh	r0, [r6, r7]
	bl	sub_08118098
	movs	r3, #128
	lsls	r3, r3, #11
	ldr	r5, [r0, #0]
	ldrsh	r0, [r6, r7]
	str	r3, [sp, #0]
	movs	r3, #100
	str	r3, [sp, #4]
	movs	r1, #1
	movs	r3, #0
	movs	r2, #0
	bl	sub_0815f000
	movs	r3, #0
	str	r3, [r5, #72]
	movs	r3, #1
	add	r8, r3
	ldr	r3, [r7, #20]
	adds	r6, #2
	cmp	r8, r3
	bne.n	.L_0815d48c
.L_0815d4bc:
	ldr	r4, [sp, #64]
	cmp	r4, #186
	bne.n	.L_0815d4c6
	ldr	r6, [pc, #340]
	str	r6, [sp, #40]
.L_0815d4c6:
	ldr	r3, [sp, #64]
	subs	r3, #54
	cmp	r3, #113
	bhi.n	.L_0815d4d0
	b.n	.L_0815d756
.L_0815d4d0:
	ldr	r7, [sp, #64]
	cmp	r7, #213
	ble.n	.L_0815d4d8
	b.n	.L_0815d750
.L_0815d4d8:
	add	r5, sp, #104
	movs	r3, #0
	str	r3, [r5, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	cmp	r7, #167
	ble.n	.L_0815d4f0
	ldr	r0, [sp, #48]
	ldr	r1, [pc, #304]
	adds	r1, r0, r1
	str	r1, [sp, #48]
.L_0815d4f0:
	ldr	r2, [sp, #64]
	cmp	r2, #53
	ble.n	.L_0815d4f8
	b.n	.L_0815d638
.L_0815d4f8:
	ldr	r4, [sp, #76]
	movs	r7, #238
	lsls	r7, r7, #7
	movs	r3, #0
	adds	r7, #220
	mov	r8, r3
	adds	r6, r4, r7
.L_0815d506:
	ldr	r3, [pc, #280]
	mov	r0, r8
	ldrb	r2, [r3, r0]
	ldr	r1, [sp, #52]
	movs	r3, #40
	subs	r3, r3, r2
	lsls	r3, r3, #16
	adds	r3, r3, r1
	str	r3, [r5, #0]
	ldr	r3, [pc, #264]
	ldrb	r3, [r3, r0]
	lsls	r2, r3, #5
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_0815d526
	adds	r2, #31
.L_0815d526:
	asrs	r2, r2, #5
	movs	r3, #128
	ldr	r4, [sp, #56]
	subs	r3, r3, r2
	lsls	r3, r3, #16
	adds	r2, r3, r4
	mov	r7, r8
	str	r2, [r5, #8]
	cmp	r7, #8
	ble.n	.L_0815d540
	ldr	r0, [pc, #236]
	adds	r3, r2, r0
	str	r3, [r5, #8]
.L_0815d540:
	ldr	r3, [r5, #8]
	ldr	r2, [pc, #232]
	cmp	r3, r2
	bge.n	.L_0815d54c
	str	r2, [r5, #8]
	adds	r3, r2, #0
.L_0815d54c:
	ldr	r1, [pc, #224]
	cmp	r3, r1
	ble.n	.L_0815d558
	movs	r3, #160
	lsls	r3, r3, #16
	str	r3, [r5, #8]
.L_0815d558:
	ldr	r2, [pc, #216]
	movs	r3, #0
	ldmia	r6!, {r0}
	adds	r1, r5, #0
	bl	sub_08020010
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #12
	bne.n	.L_0815d506
	ldr	r7, [sp, #76]
	movs	r0, #240
	lsls	r0, r0, #7
	movs	r4, #0
	adds	r0, #12
	mov	r8, r4
	adds	r6, r7, r0
.L_0815d57c:
	ldr	r3, [pc, #160]
	mov	r1, r8
	movs	r4, #3
	ands	r4, r1
	adds	r1, r4, #5
	ldrb	r2, [r3, r1]
	movs	r3, #39
	subs	r3, r3, r2
	ldr	r2, [sp, #52]
	lsls	r3, r3, #16
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r3, [pc, #140]
	ldrb	r2, [r3, r1]
	lsls	r3, r2, #4
	subs	r3, r3, r2
	lsls	r3, r3, #1
	cmp	r3, #0
	bge.n	.L_0815d5a4
	adds	r3, #31
.L_0815d5a4:
	asrs	r3, r3, #5
	mov	r2, r8
	negs	r1, r3
	cmp	r2, #0
	bge.n	.L_0815d5b0
	adds	r2, #3
.L_0815d5b0:
	asrs	r2, r2, #2
	lsls	r3, r2, #4
	subs	r3, r3, r2
	lsls	r3, r3, #2
	ldr	r7, [sp, #56]
	subs	r3, r1, r3
	lsls	r3, r3, #16
	movs	r0, #132
	ldr	r2, [pc, #104]
	adds	r3, r3, r7
	lsls	r0, r0, #15
	adds	r3, r3, r0
	str	r3, [r5, #8]
	cmp	r3, r2
	bge.n	.L_0815d5d0
	str	r2, [r5, #8]
.L_0815d5d0:
	ldr	r3, [r5, #8]
	ldr	r1, [pc, #92]
	cmp	r3, r1
	ble.n	.L_0815d5de
	movs	r3, #160
	lsls	r3, r3, #16
	str	r3, [r5, #8]
.L_0815d5de:
	movs	r2, #238
	lsls	r3, r4, #2
	lsls	r2, r2, #7
	ldr	r4, [sp, #76]
	adds	r2, #240
	adds	r3, r3, r2
	ldr	r3, [r4, r3]
	ldmia	r6!, {r0}
	ldrh	r2, [r3, #8]
	ldr	r1, [pc, #32]
	ldrh	r3, [r0, #8]
	lsls	r2, r2, #22
	lsrs	r2, r2, #22
	ands	r3, r1
	orrs	r3, r2
	movs	r7, #1
	strh	r3, [r0, #8]
	adds	r1, r5, #0
	ldr	r2, [pc, #48]
	movs	r3, #0
	add	r8, r7
	bl	sub_08020010
	mov	r0, r8
	cmp	r0, #16
	bne.n	.L_0815d57c
	b.n	.L_0815d850
	.4byte 0xfffffc00
	.4byte 0xfff00000
	.4byte 0xfffa0000
	.4byte 0x08198830
	.4byte 0x0819883c
	.4byte 0xff100000
	.4byte 0xffff0000
	.4byte 0x009fffff
	.2byte 0x8828
	.2byte 0x0819
.L_0815d638:
	ldr	r2, [sp, #76]
	movs	r3, #238
	lsls	r3, r3, #7
	movs	r1, #0
	adds	r3, #220
	mov	r8, r1
	adds	r7, r5, #0
	adds	r4, r2, r3
.L_0815d648:
	ldr	r6, [pc, #240]
	mov	r0, r8
	ldrb	r3, [r6, r0]
	ldr	r1, [sp, #52]
	lsls	r3, r3, #16
	adds	r3, r3, r1
	str	r3, [r7, #0]
	ldr	r3, [pc, #232]
	ldr	r2, [sp, #56]
	ldrb	r3, [r3, r0]
	mov	sl, r6
	lsls	r3, r3, #16
	adds	r3, r3, r2
	str	r3, [r7, #8]
	cmp	r0, #8
	ble.n	.L_0815d670
	movs	r6, #128
	lsls	r6, r6, #17
	adds	r3, r3, r6
	str	r3, [r5, #8]
.L_0815d670:
	ldr	r3, [r5, #8]
	ldr	r6, [pc, #208]
	cmp	r3, r6
	bge.n	.L_0815d67c
	str	r6, [r5, #8]
	adds	r3, r6, #0
.L_0815d67c:
	ldr	r0, [pc, #200]
	cmp	r3, r0
	ble.n	.L_0815d688
	movs	r3, #160
	lsls	r3, r3, #16
	str	r3, [r5, #8]
.L_0815d688:
	ldmia	r4!, {r0}
	adds	r1, r5, #0
	ldr	r2, [pc, #188]
	movs	r3, #0
	str	r4, [sp, #8]
	bl	sub_08020010
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	ldr	r4, [sp, #8]
	cmp	r2, #12
	bne.n	.L_0815d648
	ldr	r0, [sp, #76]
	movs	r1, #240
	lsls	r1, r1, #7
	movs	r3, #0
	adds	r1, #12
	adds	r7, r6, #0
	mov	r8, r3
	mov	r4, sl
	adds	r6, r0, r1
.L_0815d6b4:
	mov	r2, r8
	movs	r1, #3
	ands	r1, r2
	adds	r3, r1, #5
	ldrb	r2, [r4, r3]
	ldr	r0, [sp, #52]
	lsls	r2, r2, #16
	adds	r2, r2, r0
	adds	r2, r2, r7
	str	r2, [r5, #0]
	ldr	r2, [pc, #116]
	ldrb	r2, [r2, r3]
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_0815d6d4
	adds	r3, #3
.L_0815d6d4:
	asrs	r3, r3, #2
	lsls	r3, r3, #6
	adds	r3, r2, r3
	ldr	r2, [sp, #56]
	lsls	r3, r3, #16
	movs	r0, #128
	adds	r3, r3, r2
	lsls	r0, r0, #15
	adds	r3, r3, r0
	str	r3, [r5, #8]
	cmp	r3, r7
	bge.n	.L_0815d6ee
	str	r7, [r5, #8]
.L_0815d6ee:
	ldr	r3, [r5, #8]
	ldr	r2, [pc, #84]
	cmp	r3, r2
	ble.n	.L_0815d6fc
	movs	r3, #160
	lsls	r3, r3, #16
	str	r3, [r5, #8]
.L_0815d6fc:
	lsls	r3, r1, #2
	movs	r1, #238
	ldr	r2, [sp, #76]
	lsls	r1, r1, #7
	adds	r1, #240
	adds	r3, r3, r1
	ldr	r3, [r2, r3]
	ldmia	r6!, {r0}
	ldrh	r2, [r3, #8]
	ldr	r1, [pc, #40]
	ldrh	r3, [r0, #8]
	lsls	r2, r2, #22
	lsrs	r2, r2, #22
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #8]
	adds	r1, r5, #0
	movs	r3, #0
	ldr	r2, [pc, #40]
	str	r4, [sp, #8]
	bl	sub_08020010
	movs	r3, #1
	add	r8, r3
	mov	r0, r8
	ldr	r4, [sp, #8]
	cmp	r0, #16
	bne.n	.L_0815d6b4
	b.n	.L_0815d850
	movs	r0, r0
	.4byte 0xfffffc00
	.4byte 0x08198830
	.4byte 0x0819883c
	.4byte 0xffff0000
	.4byte 0x009fffff
	.2byte 0x8828
	.2byte 0x0819
.L_0815d750:
	cmp	r3, #113
	bls.n	.L_0815d756
	b.n	.L_0815d850
.L_0815d756:
	ldr	r1, [sp, #64]
	cmp	r1, #54
	bne.n	.L_0815d78e
	ldr	r3, [sp, #76]
	movs	r4, #238
	lsls	r4, r4, #7
	movs	r2, #0
	adds	r4, #220
	mov	r8, r2
	movs	r1, #0
	adds	r2, r3, r4
.L_0815d76c:
	ldmia	r2!, {r3}
	movs	r6, #1
	add	r8, r6
	mov	r7, r8
	strh	r1, [r3, #18]
	cmp	r7, #28
	bne.n	.L_0815d76c
	ldr	r2, [pc, #520]
	movs	r0, #128
	movs	r1, #144
	lsls	r0, r0, #17
	lsls	r1, r1, #15
	movs	r3, #0
	str	r0, [sp, #52]
	str	r1, [sp, #56]
	str	r2, [sp, #36]
	str	r3, [sp, #40]
.L_0815d78e:
	ldr	r4, [sp, #64]
	cmp	r4, #138
	bne.n	.L_0815d79a
	movs	r6, #128
	lsls	r6, r6, #12
	str	r6, [sp, #36]
.L_0815d79a:
	add	r6, sp, #104
	movs	r3, #0
	ldr	r7, [sp, #36]
	str	r3, [r6, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r6, #4]
	adds	r0, r7, #0
	lsls	r3, r7, #3
	subs	r3, r3, r0
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L_0815d7b6
	adds	r3, #63
.L_0815d7b6:
	ldr	r1, [sp, #40]
	asrs	r3, r3, #6
	str	r3, [sp, #36]
	lsls	r3, r1, #3
	subs	r3, r3, r1
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L_0815d7c8
	adds	r3, #63
.L_0815d7c8:
	ldr	r2, [sp, #64]
	asrs	r3, r3, #6
	str	r3, [sp, #40]
	cmp	r2, #137
	bgt.n	.L_0815d7f6
	lsls	r5, r2, #9
	adds	r0, r5, #0
	bl	sub_08002096
	ldr	r3, [sp, #52]
	lsls	r0, r0, #4
	adds	r0, r0, r3
	str	r0, [sp, #44]
	mov	sl, r0
	adds	r0, r5, #0
	bl	sub_08002090
	ldr	r4, [sp, #56]
	lsls	r0, r0, #3
	adds	r0, r0, r4
	str	r0, [sp, #48]
	adds	r7, r0, #0
	b.n	.L_0815d814
.L_0815d7f6:
	ldr	r7, [sp, #64]
	lsls	r5, r7, #9
	adds	r0, r5, #0
	bl	sub_08002096
	ldr	r1, [sp, #52]
	lsls	r0, r0, #4
	adds	r1, r1, r0
	adds	r0, r5, #0
	mov	sl, r1
	bl	sub_08002090
	ldr	r2, [sp, #56]
	lsls	r0, r0, #3
	adds	r7, r0, r2
.L_0815d814:
	ldr	r4, [sp, #76]
	movs	r0, #238
	lsls	r0, r0, #7
	movs	r3, #0
	adds	r0, #220
	adds	r5, r6, #0
	mov	r8, r3
	adds	r6, r4, r0
.L_0815d824:
	ldr	r3, [pc, #352]
	mov	r1, r8
	ldrsb	r3, [r3, r1]
	ldr	r2, [pc, #352]
	lsls	r3, r3, #16
	add	r3, sl
	str	r3, [r5, #0]
	ldr	r3, [pc, #348]
	ldmia	r6!, {r0}
	ldrsb	r3, [r3, r1]
	adds	r1, r5, #0
	lsls	r3, r3, #16
	adds	r3, r3, r7
	str	r3, [r5, #8]
	movs	r3, #0
	bl	sub_08020010
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #8
	bne.n	.L_0815d824
.L_0815d850:
	ldr	r3, [sp, #64]
	subs	r3, #138
	cmp	r3, #29
	bhi.n	.L_0815d86c
	ldr	r4, [sp, #64]
	ldr	r3, [pc, #312]
	cmp	r4, #151
	ble.n	.L_0815d866
	ldr	r6, [pc, #308]
	lsls	r3, r4, #13
	adds	r3, r3, r6
.L_0815d866:
	ldr	r7, [sp, #44]
	adds	r3, r7, r3
	str	r3, [sp, #44]
.L_0815d86c:
	ldr	r0, [sp, #64]
	cmp	r0, #78
	bne.n	.L_0815d878
	movs	r0, #140
	bl	sub_081c0010
.L_0815d878:
	ldr	r1, [sp, #64]
	cmp	r1, #138
	bne.n	.L_0815d884
	movs	r0, #104
	bl	sub_081c0010
.L_0815d884:
	ldr	r2, [sp, #64]
	cmp	r2, #152
	bne.n	.L_0815d890
	movs	r0, #163
	bl	sub_081c0010
.L_0815d890:
	ldr	r3, [sp, #64]
	cmp	r3, #7
	bne.n	.L_0815d89c
	movs	r0, #212
	bl	sub_081c0010
.L_0815d89c:
	ldr	r4, [sp, #64]
	cmp	r4, #10
	bne.n	.L_0815d942
	movs	r0, #144
	bl	sub_081c0010
	movs	r6, #0
	mov	r8, r6
	ldr	r5, [sp, #76]
	ldr	r6, [pc, #236]
.L_0815d8b0:
	ldrb	r2, [r6, #0]
	ldrb	r1, [r6, #1]
	lsrs	r2, r2, #1
	lsls	r3, r2, #16
	str	r3, [r5, #0]
	subs	r2, #60
	lsls	r3, r1, #16
	subs	r1, #60
	lsls	r2, r2, #12
	lsls	r1, r1, #10
	str	r3, [r5, #4]
	movs	r3, #0
	str	r3, [r5, #8]
	str	r2, [r5, #12]
	str	r1, [r5, #16]
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	movs	r7, #128
	lsls	r3, r3, #2
	lsls	r7, r7, #2
	adds	r3, r3, r7
	str	r3, [r5, #20]
	bl	sub_08014878
	movs	r3, #1
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L_0815d8f2
	ldr	r3, [r5, #20]
	negs	r3, r3
	str	r3, [r5, #20]
.L_0815d8f2:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r6, #10
	adds	r5, #28
	cmp	r1, #31
	bne.n	.L_0815d8b0
	ldr	r1, [pc, #156]
	ldr	r2, [pc, #160]
	ldrh	r3, [r2, #0]
	adds	r0, r3, #0
	movs	r3, #130
	ldr	r4, [pc, #152]
	lsls	r3, r3, #2
	strh	r3, [r4, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_0815d938
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #234
	adds	r3, r3, r1
	lsls	r2, r2, #7
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_0815d938:
	ldr	r6, [pc, #104]
	strh	r0, [r6, #0]
	ldr	r7, [sp, #64]
	cmp	r7, #10
	beq.n	.L_0815d948
.L_0815d942:
	ldr	r0, [sp, #64]
	cmp	r0, #186
	bne.n	.L_0815da26
.L_0815d948:
	ldr	r2, [sp, #84]
	movs	r1, #0
	ldr	r3, [r2, #20]
	mov	r8, r1
	cmp	r3, #0
	beq.n	.L_0815da0a
	adds	r2, #36
	mov	sl, r2
.L_0815d958:
	mov	r4, sl
	movs	r3, #0
	ldrsh	r0, [r4, r3]
	bl	sub_08118098
	adds	r5, r0, #0
	ldr	r6, [r5, #0]
	ldr	r7, [sp, #64]
	mov	r9, r6
	cmp	r7, #186
	bne.n	.L_0815d978
	mov	r2, sl
	movs	r1, #0
	ldrsh	r0, [r2, r1]
	bl	sub_08118120
.L_0815d978:
	ldr	r3, [sp, #64]
	cmp	r3, #10
	bne.n	.L_0815d9fa
	movs	r7, #0
	b.n	.L_0815d9b2
	movs	r0, r0
	.4byte 0xfffa0000
	.4byte 0x08198850
	.4byte 0x08198848
	.4byte 0x08198858
	.4byte 0xfffe0000
	.4byte 0xffeb0000
	.4byte 0x081986f2
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
.L_0815d9a8:
	ldr	r1, [r5, #36]
	adds	r0, r6, #0
	bl	sub_08020060
	adds	r7, #1
.L_0815d9b2:
	ldr	r0, [r5, #0]
	adds	r1, r7, #0
	bl	sub_081180d8
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_0815d9a8
	mov	r7, sl
	movs	r3, #8
	movs	r4, #0
	ldrsh	r0, [r7, r4]
	movs	r1, #9
	str	r3, [sp, #0]
	movs	r2, #5
	mov	r3, r8
	bl	sub_0814cd48
	bl	sub_08014878
	movs	r3, #0
	ldrsh	r2, [r7, r3]
	movs	r1, #128
	lsls	r1, r1, #9
	lsls	r0, r0, #1
	adds	r0, r0, r1
	movs	r3, #100
	str	r0, [sp, #0]
	str	r3, [sp, #4]
	adds	r0, r2, #0
	movs	r1, #1
	movs	r2, #0
	movs	r3, #0
	bl	sub_0815f000
	mov	r4, r9
	str	r6, [r4, #72]
.L_0815d9fa:
	ldr	r0, [sp, #84]
	movs	r7, #1
	ldr	r3, [r0, #20]
	movs	r6, #2
	add	r8, r7
	add	sl, r6
	cmp	r8, r3
	bne.n	.L_0815d958
.L_0815da0a:
	ldr	r1, [sp, #64]
	cmp	r1, #186
	bne.n	.L_0815da26
	ldr	r4, [sp, #84]
	ldr	r2, [pc, #40]
	lsls	r3, r3, #1
	adds	r3, #36
	adds	r0, r4, #0
	strh	r2, [r4, r3]
	adds	r0, #36
	movs	r1, #0
	movs	r2, #0
	bl	sub_08118148
.L_0815da26:
	ldr	r6, [sp, #64]
	cmp	r6, #9
	ble.n	.L_0815da6a
	ldr	r0, [sp, #84]
	movs	r7, #0
	ldr	r3, [r0, #20]
	mov	r8, r7
	cmp	r3, #0
	beq.n	.L_0815da6a
	movs	r5, #36
	b.n	.L_0815da40
	.2byte 0x00ff
	.2byte 0x0000
.L_0815da40:
	ldr	r1, [sp, #84]
	ldrsh	r0, [r5, r1]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	ldr	r2, [r0, #40]
	lsls	r3, r2, #4
	subs	r3, r3, r2
	lsls	r3, r3, #2
	cmp	r3, #0
	bge.n	.L_0815da58
	adds	r3, #63
.L_0815da58:
	asrs	r3, r3, #6
	str	r3, [r0, #40]
	ldr	r4, [sp, #84]
	movs	r3, #1
	add	r8, r3
	ldr	r3, [r4, #20]
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L_0815da40
.L_0815da6a:
	mov	r6, fp
	cmp	r6, #43
	bls.n	.L_0815da72
	b.n	.L_0815db98
.L_0815da72:
	movs	r0, #32
	bl	sub_08014dac
	mov	sl, r0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #160]
	ldr	r3, [sp, #96]
	movs	r1, #8
	ands	r3, r2
	ldr	r2, [pc, #156]
	orrs	r3, r1
	ands	r3, r2
	movs	r2, #224
	ldr	r7, [pc, #152]
	lsls	r2, r2, #3
	orrs	r3, r2
	adds	r6, r0, #0
	str	r3, [sp, #96]
	add	r3, sp, #96
	str	r7, [r3, #4]
	str	r3, [r6, #16]
	ldr	r3, [pc, #140]
	mov	r0, sl
	ldrh	r3, [r3, #4]
	str	r1, [r6, #0]
	str	r0, [r6, #12]
	strb	r3, [r6, #24]
	ldr	r7, [sp, #28]
	ldr	r5, [sp, #76]
	movs	r1, #0
	mov	r8, r1
.L_0815dab4:
	str	r7, [r6, #8]
	bl	sub_08014de4
	ldr	r3, [pc, #120]
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	adds	r0, r0, r3
	adds	r1, r1, r3
	movs	r2, #0
	bl	sub_08015160
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r1, #128
	adds	r0, r2, #0
	lsls	r1, r1, #9
	bl	sub_080151e4
	movs	r3, #3
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #1
	beq.n	.L_0815db04
	cmp	r3, #1
	bgt.n	.L_0815daec
	cmp	r3, #0
	beq.n	.L_0815daf6
	b.n	.L_0815db46
.L_0815daec:
	cmp	r3, #2
	beq.n	.L_0815db14
	cmp	r3, #3
	beq.n	.L_0815db38
	b.n	.L_0815db46
.L_0815daf6:
	ldr	r0, [r5, #8]
	bl	sub_08015024
	ldr	r0, [r5, #8]
	bl	sub_080150e4
	b.n	.L_0815db46
.L_0815db04:
	ldr	r0, [r5, #8]
	bl	sub_08015024
	ldr	r0, [r5, #8]
	negs	r0, r0
	bl	sub_080150e4
	b.n	.L_0815db46
.L_0815db14:
	ldr	r0, [r5, #8]
	bl	sub_08015024
	ldr	r0, [r5, #8]
	bl	sub_08015068
	b.n	.L_0815db46
	movs	r0, r0
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x02010000
	.4byte 0x03001120
	.2byte 0x0000
	.2byte 0xffc0
.L_0815db38:
	.2byte 0x68a8
	bl	sub_080150e4
	ldr	r0, [r5, #8]
	negs	r0, r0
	bl	sub_08015068
.L_0815db46:
	ldr	r4, [sp, #32]
	mov	r3, r8
	lsls	r0, r3, #4
	adds	r0, r4, r0
	mov	r1, sl
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r5, #0
	movs	r1, #62
	movs	r2, #0
	bl	sub_08138058
	movs	r3, #8
	mov	r0, r8
	subs	r3, r3, r0
	ldr	r1, [sp, #64]
	lsls	r3, r3, #1
	adds	r3, #16
	cmp	r1, r3
	blt.n	.L_0815db78
	ldr	r3, [r5, #16]
	ldr	r2, [pc, #416]
	adds	r3, r3, r2
	str	r3, [r5, #16]
.L_0815db78:
	adds	r0, r6, #0
	bl	sub_08196a7c
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r7, #36
	adds	r5, #28
	cmp	r4, #31
	bne.n	.L_0815dab4
	adds	r0, r6, #0
	bl	sub_08013164
	mov	r0, sl
	bl	sub_08013164
.L_0815db98:
	ldr	r6, [sp, #64]
	cmp	r6, #78
	beq.n	.L_0815dba0
	b.n	.L_0815dcb6
.L_0815dba0:
	movs	r3, #8
	ldr	r0, [sp, #20]
	movs	r1, #16
	movs	r2, #16
	str	r3, [sp, #0]
	movs	r7, #0
	bl	sub_0818de3c
	movs	r0, #0
	mov	sl, r7
	mov	r9, r0
	mov	fp, r0
.L_0815dbb8:
	mov	r0, r9
	bl	sub_08002090
	negs	r0, r0
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #4
	asrs	r3, r3, #16
	mov	r0, r9
	mov	r8, r3
	bl	sub_08002096
	lsls	r3, r0, #1
	ldr	r6, [sp, #24]
	adds	r3, r3, r0
	lsls	r3, r3, #4
	asrs	r2, r3, #16
	movs	r7, #0
	add	r6, fp
.L_0815dbde:
	lsls	r5, r7, #12
	adds	r0, r5, #0
	str	r2, [sp, #12]
	bl	sub_08002096
	ldr	r2, [sp, #12]
	mov	r1, r8
	adds	r3, r2, #0
	muls	r3, r0
	asrs	r3, r3, #16
	strb	r3, [r6, #0]
	strb	r1, [r6, #1]
	adds	r0, r5, #0
	bl	sub_08002090
	ldr	r2, [sp, #12]
	adds	r7, #1
	adds	r3, r2, #0
	muls	r3, r0
	asrs	r3, r3, #16
	strb	r3, [r6, #2]
	adds	r6, #4
	cmp	r7, #16
	bne.n	.L_0815dbde
	movs	r2, #128
	movs	r4, #1
	lsls	r2, r2, #4
	add	sl, r4
	adds	r2, #136
	movs	r3, #64
	mov	r6, sl
	add	r9, r2
	add	fp, r3
	cmp	r6, #16
	bne.n	.L_0815dbb8
	ldr	r0, [pc, #240]
	ldr	r1, [pc, #244]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r7, #0
	mov	r8, r7
	movs	r5, #0
.L_0815dc36:
	ldr	r2, [sp, #76]
	movs	r0, #0
	mov	r1, r8
	lsls	r3, r5, #4
	mov	sl, r0
	lsls	r4, r1, #11
	adds	r0, r3, r2
.L_0815dc44:
	movs	r3, #232
	ldr	r6, [pc, #212]
	lsls	r3, r3, #6
	adds	r3, #8
	movs	r7, #0
	adds	r1, r0, r3
	adds	r2, r4, r6
.L_0815dc52:
	ldrb	r3, [r2, #0]
	adds	r7, #1
	strb	r3, [r1, #0]
	adds	r2, #1
	subs	r1, #16
	cmp	r7, #64
	bne.n	.L_0815dc52
	movs	r7, #1
	add	sl, r7
	mov	r1, sl
	adds	r4, #128
	adds	r0, #1
	cmp	r1, #16
	bne.n	.L_0815dc44
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r5, #64
	cmp	r3, #6
	bne.n	.L_0815dc36
	ldr	r5, [pc, #160]
	ldr	r0, [pc, #160]
	adds	r1, r5, #0
	movs	r3, #0
	bl	sub_08157cf4
	adds	r0, r5, #0
	ldr	r1, [pc, #152]
	movs	r2, #64
	movs	r3, #64
	bl	sub_0816ae40
	movs	r2, #0
	movs	r3, #0
	ldr	r0, [pc, #144]
	adds	r1, r5, #0
	bl	sub_08157cf4
	ldr	r4, [sp, #76]
	movs	r6, #239
	movs	r7, #238
	lsls	r6, r6, #7
	lsls	r7, r7, #7
	adds	r2, r4, r6
	movs	r3, #2
	adds	r7, #132
	str	r3, [r2, #0]
	adds	r2, r4, r7
	movs	r3, #50
	str	r3, [r2, #0]
.L_0815dcb6:
	ldr	r0, [sp, #64]
	cmp	r0, #152
	bne.n	.L_0815dcc8
	ldr	r0, [pc, #108]
	ldr	r1, [pc, #92]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
.L_0815dcc8:
	ldr	r1, [sp, #64]
	subs	r1, #80
	mov	sl, r1
	cmp	r1, #107
	bls.n	.L_0815dcd4
	b.n	.L_0815df02
.L_0815dcd4:
	movs	r3, #128
	ldr	r2, [pc, #56]
	lsls	r3, r3, #19
	adds	r3, #12
	strh	r2, [r3, #0]
	ldr	r3, [sp, #64]
	subs	r3, #168
	cmp	r3, #17
	bhi.n	.L_0815dd86
	movs	r0, #188
	movs	r1, #7
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r3, [r3, #0]
	ldr	r4, [sp, #56]
	movs	r6, #16
	str	r3, [sp, #72]
	mov	fp, r6
	asrs	r3, r4, #16
	ldr	r6, [sp, #64]
	movs	r2, #0
	adds	r7, r3, #0
	movs	r0, #64
	mov	r8, r2
	subs	r7, #32
	mov	r9, r0
	b.n	.L_0815dd30
	.4byte 0x00000784
	.4byte 0xffff8000
	.4byte 0x0000016a
	.4byte 0x02014000
	.4byte 0x000000ec
	.4byte 0x02010000
	.4byte 0x000000c2
	.2byte 0x00b4
	.2byte 0x0000
.L_0815dd30:
	adds	r0, r6, #0
	movs	r1, #6
	bl	sub_08002064
	ldr	r1, [sp, #76]
	adds	r5, r0, #0
	movs	r2, #216
	lsls	r5, r5, #10
	lsls	r2, r2, #6
	adds	r5, r1, r5
	adds	r2, #24
	adds	r5, r5, r2
	mov	r3, fp
	mov	r4, r9
	str	r3, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #80]
	ldr	r4, [sp, #68]
	adds	r1, r5, #0
	movs	r2, #30
	adds	r3, r7, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4658
	mov	r1, r9
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	ldr	r0, [sp, #80]
	adds	r1, r5, #0
	movs	r2, #46
	adds	r3, r7, #0
	ldr	r4, [sp, #72]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2001
	add	r8, r0
	mov	r1, r8
	adds	r6, #3
	cmp	r1, #2
	bne.n	.L_0815dd30
	movs	r0, #188
	bl	sub_0801314c
.L_0815dd86:
	ldr	r2, [sp, #64]
	cmp	r2, #185
	ble.n	.L_0815dd8e
	b.n	.L_0815df08
.L_0815dd8e:
	movs	r0, #1
	bl	sub_081969f8
	mov	r4, sl
	lsls	r3, r4, #1
	add	r3, sl
	movs	r7, #128
	lsls	r5, r3, #8
	lsls	r7, r7, #9
	adds	r6, r0, #0
	movs	r1, #0
	cmp	r5, r7
	ble.n	.L_0815ddac
	movs	r5, #128
	lsls	r5, r5, #9
.L_0815ddac:
	ldr	r3, [sp, #88]
	ldr	r2, [pc, #884]
	movs	r0, #7
	ands	r3, r2
	ldr	r2, [pc, #880]
	orrs	r3, r0
	ands	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	orrs	r3, r2
	ldr	r2, [pc, #872]
	add	r7, sp, #88
	str	r3, [sp, #88]
	str	r0, [r6, #0]
	str	r2, [r7, #4]
	str	r7, [r6, #16]
	ldr	r3, [sp, #20]
	mov	r8, r0
	str	r3, [r6, #8]
	ldr	r4, [sp, #32]
	str	r1, [r6, #20]
	str	r4, [r6, #12]
	strb	r1, [r6, #24]
	strb	r1, [r6, #25]
	bl	sub_08014de4
	ldr	r1, [sp, #44]
	ldr	r3, [sp, #48]
	ldr	r4, [pc, #840]
	lsrs	r0, r1, #31
	ldr	r2, [pc, #840]
	adds	r0, r0, r1
	asrs	r0, r0, #1
	adds	r1, r3, r4
	adds	r0, r0, r2
	movs	r2, #0
	bl	sub_08015160
	lsls	r1, r5, #1
	adds	r0, r5, #0
	adds	r2, r5, #0
	bl	sub_080151e4
	movs	r1, #0
	movs	r2, #0
	ldr	r0, [pc, #816]
	bl	sub_08015160
	ldr	r0, [sp, #64]
	lsls	r5, r0, #10
	adds	r0, r5, #0
	bl	sub_08015024
	adds	r0, r5, #0
	bl	sub_08015068
	movs	r2, #128
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #32]
	lsls	r2, r2, #1
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
	ldr	r1, [sp, #64]
	cmp	r1, #137
	ble.n	.L_0815de98
	lsls	r3, r1, #2
	movs	r2, #160
	adds	r3, r3, r1
	lsls	r2, r2, #3
	lsls	r3, r3, #1
	adds	r2, #164
	subs	r1, r2, r3
	ldr	r2, [sp, #64]
	ldr	r4, [pc, #756]
	lsls	r3, r2, #13
	adds	r5, r3, r4
	cmp	r1, #0
	ble.n	.L_0815de50
	movs	r1, #0
.L_0815de50:
	movs	r0, #64
	negs	r0, r0
	cmp	r1, r0
	ble.n	.L_0815de98
	ldr	r3, [pc, #740]
	mov	r2, r8
	str	r3, [r7, #4]
	ldr	r3, [pc, #740]
	strb	r2, [r7, #0]
	str	r3, [r6, #8]
	strb	r2, [r7, #1]
	str	r1, [r6, #20]
	bl	sub_08014de4
	movs	r0, #176
	movs	r1, #0
	movs	r2, #0
	lsls	r0, r0, #14
	bl	sub_08015160
	movs	r0, #208
	lsls	r0, r0, #6
	adds	r0, #72
	bl	sub_08015068
	adds	r0, r5, #0
	bl	sub_0801521c
	ldr	r0, [pc, #700]
	ldr	r1, [sp, #32]
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
.L_0815de98:
	ldr	r3, [sp, #64]
	cmp	r3, #151
	ble.n	.L_0815defc
	ldr	r4, [sp, #76]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #168
	adds	r2, r4, r0
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [sp, #64]
	ldr	r4, [pc, #668]
	lsls	r3, r2, #13
	adds	r5, r3, r4
	add	r2, sp, #88
	movs	r3, #6
	strb	r3, [r7, #0]
	strb	r3, [r2, #1]
	ldr	r3, [pc, #640]
	movs	r1, #0
	str	r3, [r2, #4]
	ldr	r3, [pc, #652]
	str	r1, [r6, #20]
	str	r3, [r6, #8]
	bl	sub_08014de4
	ldr	r0, [pc, #608]
	ldr	r7, [sp, #48]
	movs	r2, #0
	adds	r1, r7, r0
	ldr	r0, [pc, #636]
	bl	sub_08015160
	asrs	r2, r5, #1
	adds	r0, r2, #0
	adds	r1, r5, #0
	bl	sub_080151e4
	ldr	r1, [sp, #64]
	lsls	r0, r1, #11
	bl	sub_080150e4
	ldr	r0, [pc, #600]
	ldr	r1, [sp, #32]
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
.L_0815defc:
	adds	r0, r6, #0
	bl	sub_08013164
.L_0815df02:
	ldr	r2, [sp, #64]
	cmp	r2, #185
	ble.n	.L_0815dfe4
.L_0815df08:
	ldr	r3, [sp, #64]
	cmp	r3, #186
	bne.n	.L_0815df6e
	ldr	r7, [pc, #540]
	movs	r4, #0
	mov	r8, r4
.L_0815df14:
	mov	r0, r8
	lsls	r6, r0, #8
	bl	sub_08014878
	movs	r3, #127
	adds	r5, r0, #0
	adds	r0, r6, #0
	ands	r5, r3
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	ldr	r2, [r7, #12]
	adds	r1, r5, #0
	muls	r1, r0
	lsls	r3, r2, #1
	adds	r3, r3, r2
	movs	r2, #192
	lsls	r2, r2, #14
	lsls	r3, r3, #1
	asrs	r1, r1, #4
	adds	r3, r3, r2
	str	r3, [r7, #0]
	lsls	r3, r1, #2
	adds	r3, r3, r1
	str	r3, [r7, #4]
	str	r1, [r7, #16]
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	adds	r3, #32
	str	r3, [r7, #24]
	movs	r4, #128
	movs	r3, #1
	add	r8, r3
	lsls	r4, r4, #1
	adds	r7, #28
	cmp	r8, r4
	bne.n	.L_0815df14
.L_0815df6e:
	ldr	r5, [pc, #444]
	movs	r6, #0
	mov	r8, r6
.L_0815df74:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_0815dfd2
	movs	r7, #2
	ldrsh	r2, [r5, r7]
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	ldr	r4, [sp, #76]
	movs	r6, #208
	movs	r1, #12
	lsls	r6, r6, #6
	str	r1, [sp, #0]
	adds	r6, #248
	movs	r1, #24
	subs	r3, #12
	str	r1, [sp, #4]
	subs	r2, #6
	adds	r1, r4, r6
	ldr	r0, [sp, #80]
	ldr	r7, [sp, #68]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x2280
	adds	r0, r5, #0
	movs	r1, #63
	lsls	r2, r2, #9
	bl	sub_08138086
	ldr	r3, [r5, #4]
	movs	r0, #192
	lsls	r0, r0, #15
	cmp	r3, r0
	ble.n	.L_0815dfd0
	ldr	r3, [r5, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r5, #16]
	ldr	r3, [r5, #12]
	lsls	r3, r3, #2
	str	r3, [r5, #12]
	ldr	r3, [r5, #24]
	subs	r3, #8
	str	r3, [r5, #24]
	b.n	.L_0815dfd2
.L_0815dfd0:
	ldr	r3, [r5, #24]
.L_0815dfd2:
	movs	r1, #1
	movs	r2, #128
	subs	r3, #1
	add	r8, r1
	lsls	r2, r2, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r8, r2
	bne.n	.L_0815df74
.L_0815dfe4:
	ldr	r3, [sp, #64]
	cmp	r3, #186
	bne.n	.L_0815e092
	ldr	r4, [sp, #60]
	movs	r6, #206
	lsls	r6, r6, #3
	adds	r3, r4, r6
	ldrh	r1, [r3, #0]
	movs	r0, #1
	movs	r2, #24
	bl	sub_08118040
	movs	r1, #128
	ldr	r3, [pc, #344]
	ldr	r0, [sp, #80]
	lsls	r1, r1, #7
	ldr	r2, [pc, #340]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4955
	ldr	r7, [pc, #340]
	ldrh	r3, [r7, #0]
	adds	r0, r3, #0
	movs	r2, #130
	ldr	r3, [pc, #332]
	lsls	r2, r2, #2
	strh	r2, [r3, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_0815e042
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #238
	adds	r3, r3, r1
	lsls	r2, r2, #7
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_0815e042:
	ldr	r4, [pc, #288]
	strh	r0, [r4, #0]
	movs	r6, #0
	ldr	r7, [sp, #84]
	mov	r8, r6
	ldr	r3, [r7, #20]
	cmp	r3, #0
	beq.n	.L_0815e07e
	movs	r5, #36
.L_0815e054:
	ldr	r1, [sp, #84]
	movs	r3, #16
	ldrsh	r0, [r5, r1]
	str	r3, [sp, #0]
	movs	r1, #7
	mov	r3, r8
	movs	r2, #5
	bl	sub_0814cd48
	ldr	r3, [sp, #84]
	movs	r1, #4
	ldrsh	r0, [r5, r3]
	bl	sub_08118088
	ldr	r7, [sp, #84]
	movs	r6, #1
	ldr	r3, [r7, #20]
	add	r8, r6
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L_0815e054
.L_0815e07e:
	movs	r0, #145
	bl	sub_081c0010
	movs	r1, #238
	ldr	r0, [sp, #76]
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r2, r0, r1
	movs	r3, #16
	str	r3, [r2, #0]
.L_0815e092:
	ldr	r2, [sp, #64]
	cmp	r2, #214
	bne.n	.L_0815e09e
	movs	r0, #134
	bl	sub_081180e8
.L_0815e09e:
	ldr	r3, [sp, #64]
	cmp	r3, #185
	ble.n	.L_0815e0ae
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	b.n	.L_0815e0b6
.L_0815e0ae:
	movs	r0, #2
	movs	r1, #2
	bl	sub_08158ce0
.L_0815e0b6:
	ldr	r4, [sp, #64]
	cmp	r4, #185
	ble.n	.L_0815e0c0
	bl	sub_081434f8
.L_0815e0c0:
	ldr	r6, [sp, #76]
	movs	r7, #240
	lsls	r7, r7, #7
	adds	r7, #232
	adds	r2, r6, r7
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	ldr	r0, [sp, #64]
	adds	r0, #1
	str	r0, [sp, #64]
	cmp	r0, #228
	beq.n	.L_0815e0e2
	bl	.L_0815cf8e
.L_0815e0e2:
	ldr	r0, [pc, #132]
	bl	sub_08014644
	ldr	r0, [pc, #128]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	movs	r2, #238
	lsls	r2, r2, #7
	movs	r1, #0
	adds	r2, #220
	mov	r8, r1
	adds	r5, r6, r2
.L_0815e100:
	ldmia	r5!, {r0}
	bl	sub_08020048
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	cmp	r4, #28
	bne.n	.L_0815e100
	bl	sub_08143bb8
	add	sp, #120
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x02010000
	.4byte 0xffb00000
	.4byte 0xffbc0000
	.4byte 0xffd80000
	.4byte 0xffef0000
	.4byte 0x02014000
	.4byte 0x08199364
	.4byte 0x081991e0
	.4byte 0xffed0000
	.4byte 0x08199340
	.4byte 0xfff00000
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x020038e0
	.4byte 0x04000208
	.4byte 0x08143001
	.4byte 0x08164bb5
