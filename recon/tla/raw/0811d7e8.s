.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014878, 0x08014878
	.set sub_08014de4, 0x08014de4
	.set sub_080156e8, 0x080156e8
	.set sub_08015768, 0x08015768
	.set sub_08016ca4, 0x08016ca4
	.set sub_08020090, 0x08020090
	.set sub_08038120, 0x08038120
	.set sub_08038130, 0x08038130
	.set sub_080381c8, 0x080381c8
	.set sub_0811a038, 0x0811a038
	.set sub_0811a0b0, 0x0811a0b0
	.set sub_0811a188, 0x0811a188
	.set sub_0811a31c, 0x0811a31c
	.set sub_0811a39c, 0x0811a39c
	.set sub_0811a44c, 0x0811a44c
	.set sub_0811b2c4, 0x0811b2c4
	.set sub_0811b9d4, 0x0811b9d4
	.set sub_0811bc64, 0x0811bc64
	.set sub_0811bc98, 0x0811bc98
	.set sub_0811bdb0, 0x0811bdb0
	.set sub_0811be3c, 0x0811be3c
	.set sub_0811be68, 0x0811be68
	.set sub_0811bec8, 0x0811bec8
	.set sub_0811d720, 0x0811d720
	.set sub_0811d79c, 0x0811d79c
	.set sub_0811e36c, 0x0811e36c
	.set sub_0811ea0c, 0x0811ea0c
	.set sub_0811edb0, 0x0811edb0
	.set sub_0811f088, 0x0811f088
	.set sub_0811f3b8, 0x0811f3b8
	.set sub_0811f4d4, 0x0811f4d4
	.set sub_0811ff08, 0x0811ff08
	.set sub_081201c4, 0x081201c4
	.set sub_08120360, 0x08120360
	.set sub_08122514, 0x08122514
	.set sub_081234f0, 0x081234f0
	.set sub_08123534, 0x08123534
	.set sub_0812381c, 0x0812381c
	.set sub_08124cc4, 0x08124cc4
	.set sub_0812561c, 0x0812561c
	.set sub_0812628c, 0x0812628c
	.set sub_08126804, 0x08126804
	.set sub_08126ae4, 0x08126ae4
	.set sub_08126cfc, 0x08126cfc
	.set sub_08127308, 0x08127308
	.set sub_0812756c, 0x0812756c
	.set sub_08138008, 0x08138008
	.set sub_08138010, 0x08138010
	.set sub_08138018, 0x08138018
	.set sub_081c0010, 0x081c0010
	.global Func_0811d7e8
	.thumb_func
Func_0811d7e8:
.L_0811d7e8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r6, r0, #0
	ldrh	r3, [r6, #0]
	sub	sp, #20
	adds	r5, r1, #0
	movs	r0, #2
	cmp	r3, #7
	bls.n	.L_0811d802
	movs	r0, #1
.L_0811d802:
	mov	sl, sp
	mov	r1, sl
	bl	sub_0811a39c
	ldr	r3, [pc, #140]
	mov	r8, r0
	movs	r1, #104
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x8833
	movs	r0, #0
	cmp	r3, #7
	bhi.n	.L_0811d820
	movs	r0, #1
.L_0811d820:
	bl	sub_0811f4d4
	bl	sub_0811d720
	movs	r3, #2
	str	r3, [r5, #80]
	movs	r3, #14
	str	r3, [r5, #84]
	ldrh	r3, [r6, #6]
	adds	r2, r5, #0
	adds	r2, #74
	strb	r0, [r5, #0]
	strh	r3, [r2, #0]
	mov	r3, r8
	cmp	r3, #0
	ble.n	.L_0811d878
	movs	r6, #1
	adds	r0, r5, #0
	adds	r3, r5, #3
	movs	r7, #0
	mov	r9, r6
	adds	r0, #31
	subs	r2, #57
	mov	ip, r3
	mov	r1, sl
	mov	r4, r8
.L_0811d854:
	ldrh	r6, [r1, #0]
	subs	r4, #1
	mov	lr, r6
	mov	r3, lr
	mov	r6, ip
	strb	r3, [r6, #0]
	mov	r6, r9
	movs	r3, #1
	strb	r7, [r2, #0]
	adds	r1, #2
	strb	r6, [r0, #0]
	add	ip, r3
	strb	r7, [r2, #28]
	strb	r6, [r0, #28]
	adds	r2, #1
	adds	r0, #1
	cmp	r4, #0
	bne.n	.L_0811d854
.L_0811d878:
	mov	r3, r8
	strb	r3, [r5, #1]
	movs	r3, #148
	adds	r3, #255
	str	r3, [r5, #76]
	movs	r3, #128
	lsls	r3, r3, #8
	adds	r3, #26
	str	r3, [r5, #88]
	add	sp, #20
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0258
	.2byte 0x0300
.L_0811d89c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	ldrb	r3, [r5, #0]
	sub	sp, #20
	movs	r0, #1
	cmp	r3, #7
	bls.n	.L_0811d8b2
	movs	r0, #2
.L_0811d8b2:
	mov	r8, sp
	mov	r1, r8
	bl	sub_0811a39c
	mov	sl, r0
	cmp	r0, #0
	ble.n	.L_0811d8fa
	movs	r0, #31
	adds	r0, r0, r5
	adds	r2, r5, #0
	adds	r3, r5, #3
	movs	r7, #0
	movs	r6, #1
	mov	ip, r0
	adds	r2, #17
	mov	lr, r3
	mov	r1, r8
	mov	r4, sl
.L_0811d8d6:
	ldrh	r0, [r1, #0]
	subs	r4, #1
	mov	r8, r0
	mov	r3, r8
	mov	r0, lr
	strb	r3, [r0, #0]
	mov	r0, ip
	movs	r3, #1
	strb	r7, [r2, #0]
	adds	r1, #2
	strb	r6, [r0, #0]
	add	lr, r3
	strb	r7, [r2, #28]
	add	ip, r3
	strb	r6, [r0, #28]
	adds	r2, #1
	cmp	r4, #0
	bne.n	.L_0811d8d6
.L_0811d8fa:
	mov	r3, sl
	strb	r3, [r5, #1]
	movs	r3, #103
	str	r3, [r5, #76]
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #181
	str	r3, [r5, #88]
	add	sp, #20
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
.L_0811d914:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	ldrb	r3, [r6, #0]
	sub	sp, #20
	cmp	r3, #7
	bhi.n	.L_0811d93a
	mov	r7, sp
	adds	r0, r7, #0
	bl	sub_0811a038
	adds	r5, r0, #0
	lsls	r0, r5, #1
	adds	r0, r7, r0
	bl	sub_0811a0b0
	adds	r5, r5, r0
	b.n	.L_0811d946
.L_0811d93a:
	mov	r7, sp
	movs	r0, #2
	adds	r1, r7, #0
	bl	sub_0811a31c
	adds	r5, r0, #0
.L_0811d946:
	cmp	r5, #0
	ble.n	.L_0811d988
	movs	r3, #0
	mov	r8, r3
	movs	r3, #1
	mov	lr, r3
	movs	r3, #31
	adds	r3, r3, r6
	adds	r2, r6, #0
	mov	ip, r3
	adds	r2, #17
	adds	r1, r6, #3
	adds	r0, r7, #0
	adds	r4, r5, #0
.L_0811d962:
	ldrh	r3, [r0, #0]
	mov	r7, r8
	strb	r3, [r1, #0]
	strb	r7, [r2, #0]
	mov	r3, lr
	mov	r7, ip
	strb	r3, [r7, #0]
	mov	r3, r8
	strb	r3, [r2, #28]
	mov	r3, lr
	strb	r3, [r7, #28]
	subs	r4, #1
	movs	r7, #1
	adds	r0, #2
	adds	r1, #1
	add	ip, r7
	adds	r2, #1
	cmp	r4, #0
	bne.n	.L_0811d962
.L_0811d988:
	movs	r2, #149
	lsls	r2, r2, #1
	str	r2, [r6, #76]
	movs	r2, #144
	lsls	r2, r2, #9
	movs	r3, #0
	adds	r2, #182
	strb	r5, [r6, #1]
	str	r2, [r6, #88]
	str	r3, [r6, #96]
	add	sp, #20
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
.L_0811d9a4:
	push	{r5, r6, lr}
	sub	sp, #28
	mov	r5, sp
	movs	r0, #3
	adds	r1, r5, #0
	bl	sub_0811a31c
	cmp	r0, #0
	ble.n	.L_0811d9c8
	adds	r6, r5, #0
	adds	r5, r0, #0
.L_0811d9ba:
	ldrh	r0, [r6, #0]
	subs	r5, #1
	adds	r6, #2
	bl	sub_0811be68
	cmp	r5, #0
	bne.n	.L_0811d9ba
.L_0811d9c8:
	add	sp, #28
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r5, #192
	sub	sp, #12
	lsls	r5, r5, #18
	movs	r2, #0
	adds	r7, r1, #0
	ldr	r1, [r5, #36]
	str	r2, [sp, #4]
	mov	fp, r0
	movs	r3, #0
	ldrsh	r0, [r0, r3]
	mov	sl, r1
	cmp	r0, #255
	bne.n	.L_0811d9f8
	movs	r0, #0
	b.n	.L_0811ddb2
.L_0811d9f8:
	bl	sub_08016ca4
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	cmp	r3, #0
	bne.n	.L_0811da0a
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0811ddb2
.L_0811da0a:
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r3, #105
	add	r3, sl
	movs	r2, #3
	strb	r2, [r3, #0]
	movs	r2, #42
	adds	r2, #255
	adds	r3, r0, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0811da2a
	mov	r0, fp
	movs	r1, #1
	bl	sub_08122514
.L_0811da2a:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #176
	ldr	r6, [r3, #0]
	movs	r3, #60
	str	r3, [r6, #4]
	ldr	r3, [sp, #4]
	movs	r2, #192
	lsls	r2, r2, #3
	str	r3, [r6, #20]
	ldr	r5, [r5, #48]
	adds	r2, #108
	movs	r3, #128
	add	r2, sl
	lsls	r3, r3, #9
	str	r3, [r2, #0]
	bl	sub_08014de4
	adds	r1, r5, #0
	adds	r1, #12
	adds	r0, r5, #0
	bl	sub_080156e8
	movs	r0, #255
	movs	r1, #192
	lsls	r1, r1, #8
	ldr	r3, [pc, #864]
	lsls	r0, r0, #17
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c01
	movs	r0, #255
	lsls	r0, r0, #17
	ldr	r2, [pc, #852]
	bl	sub_08015768
	cmp	r7, #0
	beq.n	.L_0811da82
	movs	r3, #128
	lsls	r3, r3, #6
	str	r3, [r6, #0]
	adds	r0, r7, #0
	bl	sub_08013560
.L_0811da82:
	mov	r2, fp
	movs	r1, #6
	ldrsh	r3, [r2, r1]
	cmp	r3, #10
	bne.n	.L_0811da9e
	movs	r1, #192
	lsls	r1, r1, #3
	adds	r1, #124
	add	r1, sl
	mov	r0, fp
	bl	.L_0811d7e8
	movs	r0, #0
	b.n	.L_0811dabe
.L_0811da9e:
	mov	r1, fp
	ldrh	r3, [r1, #0]
	add	r0, sp, #8
	strh	r3, [r0, #0]
	movs	r3, #255
	strh	r3, [r0, #2]
	movs	r1, #1
	bl	sub_08126cfc
	movs	r1, #192
	lsls	r1, r1, #3
	adds	r1, #124
	add	r1, sl
	mov	r0, fp
	bl	sub_0812381c
.L_0811dabe:
	cmp	r0, #0
	beq.n	.L_0811dac4
	b.n	.L_0811dd56
.L_0811dac4:
	movs	r3, #218
	lsls	r3, r3, #3
	add	r3, sl
	ldr	r3, [r3, #0]
	cmp	r3, #17
	bls.n	.L_0811dad2
	b.n	.L_0811dd70
.L_0811dad2:
	ldr	r2, [pc, #756]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x0811dd70
	.4byte 0x0811db24
	.4byte 0x0811db34
	.4byte 0x0811db84
	.4byte 0x0811dbae
	.4byte 0x0811db54
	.4byte 0x0811db94
	.4byte 0x0811dd3a
	.4byte 0x0811db9e
	.4byte 0x0811db64
	.4byte 0x0811dbc0
	.4byte 0x0811dc9a
	.4byte 0x0811dcea
	.4byte 0x0811dd1a
	.4byte 0x0811db74
	.4byte 0x0811db44
	.4byte 0x0811db9e
	.2byte 0xdb84
	.2byte 0x0811
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r0, #124
	add	r0, sl
	movs	r1, #0
	bl	sub_0811e36c
	b.n	.L_0811dd70
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r0, #124
	add	r0, sl
	movs	r1, #0
	bl	sub_0811ea0c
	b.n	.L_0811dd70
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r0, #124
	add	r0, sl
	movs	r1, #2
	bl	sub_0811ea0c
	b.n	.L_0811dd70
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r0, #124
	add	r0, sl
	movs	r1, #1
	bl	.L_0811df70
	b.n	.L_0811dd70
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r0, #124
	add	r0, sl
	movs	r1, #0
	bl	.L_0811df70
	b.n	.L_0811dd70
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r0, #124
	add	r0, sl
	movs	r1, #3
	bl	.L_0811df70
	b.n	.L_0811dd70
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r0, #124
	add	r0, sl
	movs	r1, #0
	bl	sub_0811f088
	b.n	.L_0811dd70
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r0, #124
	add	r0, sl
	b.n	.L_0811dd32
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r0, #124
	add	r0, sl
	movs	r1, #2
	bl	sub_0811f088
	b.n	.L_0811dd70
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r0, #124
	add	r0, sl
	movs	r1, #0
	mov	r2, fp
	bl	sub_0811edb0
	b.n	.L_0811dd70
	movs	r2, #176
	lsls	r2, r2, #3
	movs	r3, #128
	adds	r2, #255
	lsls	r3, r3, #4
	add	r2, sl
	adds	r3, #78
	ldrb	r4, [r2, #0]
	mov	r9, r2
	add	r3, sl
	movs	r2, #1
	movs	r5, #192
	strh	r2, [r3, #0]
	lsls	r5, r5, #3
	adds	r5, #126
	add	r5, sl
	movs	r7, #192
	ldrb	r3, [r5, #0]
	movs	r6, #220
	lsls	r7, r7, #3
	movs	r1, #0
	lsls	r6, r6, #3
	adds	r7, #124
	add	r7, sl
	mov	r8, r1
	add	r6, sl
	mov	r1, r9
	strb	r3, [r1, #0]
	adds	r0, r7, #0
	movs	r1, #1
	str	r2, [r6, #0]
	str	r4, [sp, #0]
	bl	sub_0811f088
	ldr	r4, [sp, #0]
	mov	r2, r8
	mov	r3, r9
	str	r2, [r6, #0]
	strb	r4, [r3, #0]
	ldrb	r4, [r7, #0]
	ldrb	r3, [r5, #0]
	strb	r3, [r7, #0]
	strb	r4, [r5, #0]
	ldrb	r0, [r7, #0]
	bl	sub_08016ca4
	movs	r1, #165
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldrh	r0, [r3, #0]
	bl	sub_0811d79c
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #212
	add	r3, sl
	str	r0, [r3, #0]
	mov	r1, r9
	ldrb	r3, [r7, #0]
	ldrb	r2, [r1, #0]
	eors	r3, r2
	lsrs	r3, r3, #7
	cmp	r3, #0
	bne.n	.L_0811dc60
	movs	r1, #1
	ldrb	r0, [r7, #0]
	bl	sub_08038120
	mov	r1, r9
	ldrb	r2, [r7, #0]
	ldrb	r3, [r1, #0]
	cmp	r2, r3
	bne.n	.L_0811dc5a
	ldr	r0, [pc, #376]
	bl	sub_080381c8
	b.n	.L_0811dc60
.L_0811dc5a:
	ldr	r0, [pc, #372]
	bl	sub_080381c8
.L_0811dc60:
	movs	r3, #217
	lsls	r3, r3, #3
	add	r3, sl
	movs	r0, #192
	ldr	r3, [r3, #0]
	lsls	r0, r0, #3
	movs	r2, #156
	adds	r0, #124
	lsls	r2, r2, #1
	add	r0, sl
	movs	r1, #32
	cmp	r3, r2
	beq.n	.L_0811dc7c
	movs	r1, #0
.L_0811dc7c:
	bl	sub_0811ea0c
	movs	r1, #192
	movs	r2, #192
	lsls	r1, r1, #3
	lsls	r2, r2, #3
	adds	r1, #124
	adds	r2, #126
	add	r1, sl
	add	r2, sl
	ldrb	r4, [r1, #0]
	ldrb	r3, [r2, #0]
	strb	r3, [r1, #0]
	strb	r4, [r2, #0]
	b.n	.L_0811dd70
	movs	r5, #220
	movs	r0, #192
	lsls	r5, r5, #3
	lsls	r0, r0, #3
	movs	r3, #1
	add	r5, sl
	adds	r0, #124
	str	r3, [r5, #0]
	movs	r1, #1
	add	r0, sl
	bl	sub_0811f088
	ldr	r6, [pc, #288]
	movs	r3, #0
	str	r3, [r5, #0]
	adds	r0, r6, #0
	bl	sub_080381c8
	movs	r3, #176
	lsls	r3, r3, #3
	adds	r3, #255
	add	r3, sl
	ldrb	r3, [r3, #0]
	movs	r0, #0
	cmp	r3, #7
	bls.n	.L_0811dcd0
	movs	r0, #1
.L_0811dcd0:
	bl	sub_08124cc4
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_0811dce0
	adds	r0, r6, #1
	bl	sub_080381c8
.L_0811dce0:
	cmp	r5, #1
	bgt.n	.L_0811dd70
	bl	sub_0811ff08
	b.n	.L_0811dd70
	movs	r5, #192
	lsls	r5, r5, #3
	adds	r5, #124
	add	r5, sl
	movs	r1, #1
	adds	r0, r5, #0
	bl	.L_0811df70
	movs	r0, #1
	movs	r1, #0
	bl	sub_0811a188
	cmp	r0, #0
	beq.n	.L_0811dd70
	movs	r0, #2
	movs	r1, #0
	bl	sub_0811a188
	cmp	r0, #0
	beq.n	.L_0811dd70
	adds	r0, r5, #0
	bl	.L_0811d89c
	b.n	.L_0811dd30
	movs	r5, #192
	lsls	r5, r5, #3
	adds	r5, #124
	add	r5, sl
	movs	r1, #1
	adds	r0, r5, #0
	bl	.L_0811df70
	adds	r0, r5, #0
	bl	.L_0811d914
.L_0811dd30:
	adds	r0, r5, #0
.L_0811dd32:
	movs	r1, #1
	bl	sub_0811f088
	b.n	.L_0811dd70
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r0, #124
	add	r0, sl
	bl	.L_0811de78
	cmp	r0, #0
	beq.n	.L_0811dd4e
	movs	r3, #1
	str	r3, [sp, #4]
.L_0811dd4e:
	ldr	r1, [sp, #4]
	cmp	r1, #0
	beq.n	.L_0811dd70
	b.n	.L_0811dd92
.L_0811dd56:
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L_0811dd68
	bl	sub_0811ff08
	movs	r0, #3
	bl	sub_08013560
.L_0811dd68:
	movs	r0, #0
	movs	r1, #0
	bl	sub_08126cfc
.L_0811dd70:
	bl	sub_0811bc98
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r0, #124
	add	r0, sl
	bl	sub_0812561c
	bl	sub_0811a44c
	bl	sub_0811b2c4
	bl	.L_0811d9a4
	movs	r3, #255
	mov	r1, fp
	strh	r3, [r1, #0]
.L_0811dd92:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	movs	r2, #206
	lsls	r2, r2, #3
	adds	r3, r3, r2
	ldrh	r1, [r3, #0]
	movs	r0, #2
	movs	r2, #0
	bl	sub_0812628c
	movs	r0, #195
	lsls	r0, r0, #1
	bl	sub_081c0010
	ldr	r0, [sp, #4]
.L_0811ddb2:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x03000230
	.4byte 0x7fff0000
	.4byte 0x0811dadc
	.4byte 0x00000c8e
	.4byte 0x00000c8d
	.2byte 0x0ce1
	.2byte 0x0000
.L_0811ddd8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r1, #0
	movs	r1, #0
	str	r1, [r7, #28]
	adds	r6, r0, #0
	ldr	r2, [r6, #88]
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #255
	ands	r3, r2
	str	r3, [r7, #0]
	movs	r3, #192
	lsls	r3, r3, #6
	ands	r2, r3
	lsrs	r2, r2, #12
	str	r2, [r7, #24]
	sub	sp, #4
	ldrb	r3, [r6, #0]
	mov	sl, r1
	str	r3, [r7, #8]
	movs	r3, #1
	ldrsb	r3, [r6, r3]
	cmp	r1, r3
	bge.n	.L_0811de5a
	movs	r2, #36
	adds	r2, r2, r7
	adds	r5, r6, #3
	mov	r8, r2
.L_0811de16:
	ldrb	r0, [r5, #0]
	str	r1, [sp, #0]
	bl	sub_08123534
	ldr	r1, [sp, #0]
	cmp	r0, #0
	beq.n	.L_0811de4c
	ldrb	r0, [r5, #0]
	bl	sub_08016ca4
	movs	r2, #56
	ldrsh	r3, [r0, r2]
	ldr	r1, [sp, #0]
	cmp	r3, #0
	bne.n	.L_0811de40
	ldr	r3, [r6, #88]
	movs	r2, #128
	lsls	r2, r2, #9
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0811de4c
.L_0811de40:
	ldrb	r3, [r5, #0]
	mov	r2, r8
	strh	r3, [r2, #0]
	movs	r3, #2
	add	r8, r3
	adds	r1, #1
.L_0811de4c:
	movs	r3, #1
	ldrsb	r3, [r6, r3]
	movs	r2, #1
	add	sl, r2
	adds	r5, #1
	cmp	sl, r3
	blt.n	.L_0811de16
.L_0811de5a:
	cmp	r1, #0
	bne.n	.L_0811de64
	ldrb	r3, [r6, #3]
	movs	r1, #1
	strh	r3, [r7, #36]
.L_0811de64:
	ldrb	r3, [r6, #3]
	add	sp, #4
	str	r3, [r7, #12]
	movs	r3, #1
	str	r1, [r7, #20]
	str	r3, [r7, #16]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
.L_0811de78:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #176
	ldr	r3, [r3, #0]
	adds	r6, r0, #0
	mov	r8, r3
	movs	r3, #128
	ldrb	r5, [r6, #0]
	lsls	r3, r3, #6
	ldr	r7, [r2, #36]
	mov	r2, r8
	str	r3, [r2, #0]
	movs	r3, #1
	movs	r1, #0
	str	r3, [r2, #16]
	movs	r0, #0
	sub	sp, #32
	mov	sl, r1
	bl	sub_08126cfc
	cmp	r5, #7
	bhi.n	.L_0811df1e
	adds	r3, r7, #0
	adds	r3, #69
	ldrb	r3, [r3, #0]
	mov	r2, sl
	cmp	r3, #2
	beq.n	.L_0811debc
	movs	r2, #1
.L_0811debc:
	cmp	r2, #0
	bne.n	.L_0811decc
	ldr	r0, [pc, #168]
	bl	sub_080381c8
	bl	sub_0811ff08
	b.n	.L_0811df5a
.L_0811decc:
	add	r7, sp, #4
	movs	r0, #1
	adds	r1, r7, #0
	bl	sub_0811a188
	movs	r3, #1
	subs	r6, r0, #1
	negs	r3, r3
	cmp	r6, r3
	beq.n	.L_0811df18
	lsls	r5, r6, #1
.L_0811dee2:
	ldrsh	r0, [r7, r5]
	bl	sub_08016ca4
	movs	r1, #60
	adds	r2, r0, #0
	adds	r1, #255
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0811df0c
	adds	r1, #1
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0811df0c
	ldrsh	r0, [r5, r7]
	bl	sub_0811bec8
	movs	r0, #8
	bl	sub_08013560
.L_0811df0c:
	movs	r3, #1
	subs	r6, #1
	negs	r3, r3
	subs	r5, #2
	cmp	r6, r3
	bne.n	.L_0811dee2
.L_0811df18:
	movs	r1, #1
	mov	sl, r1
	b.n	.L_0811df5a
.L_0811df1e:
	bl	sub_08014878
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	lsrs	r3, r3, #16
	cmp	r3, #6
	bhi.n	.L_0811df50
	ldrb	r0, [r6, #0]
	mov	r2, sp
	movs	r3, #255
	strh	r0, [r2, #0]
	strh	r3, [r2, #2]
	bl	sub_0811bec8
	movs	r0, #8
	bl	sub_08013560
	ldrb	r0, [r6, #0]
	bl	sub_0811f3b8
	ldrb	r0, [r6, #0]
	bl	sub_0811bc64
	b.n	.L_0811df5a
.L_0811df50:
	ldr	r0, [pc, #24]
	bl	sub_080381c8
	bl	sub_0811ff08
.L_0811df5a:
	movs	r3, #0
	mov	r2, r8
	mov	r0, sl
	str	r3, [r2, #16]
	add	sp, #32
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0c9c
	.2byte 0x0000
.L_0811df70:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #136
	mov	sl, r0
	add	r0, sp, #20
	mov	r8, r0
	str	r1, [sp, #16]
	mov	r0, sl
	mov	r1, r8
	bl	.L_0811ddd8
	mov	r1, sl
	ldrb	r1, [r1, #0]
	mov	r2, sl
	str	r1, [sp, #12]
	mov	r0, sl
	ldrb	r2, [r2, #3]
	str	r2, [sp, #8]
	movs	r2, #128
	ldr	r3, [r0, #88]
	lsls	r2, r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0811dfc8
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #176
	ldr	r2, [r3, #0]
	cmp	r1, #7
	bhi.n	.L_0811dfbc
	movs	r3, #128
	lsls	r3, r3, #6
	b.n	.L_0811dfc0
.L_0811dfbc:
	movs	r3, #160
	lsls	r3, r3, #7
.L_0811dfc0:
	str	r3, [r2, #0]
	movs	r3, #60
	str	r3, [r2, #4]
	b.n	.L_0811dfe4
.L_0811dfc8:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #176
	ldr	r1, [r3, #0]
	ldr	r3, [sp, #12]
	ldr	r2, [pc, #104]
	cmp	r3, #7
	bhi.n	.L_0811dfdc
	movs	r2, #128
	lsls	r2, r2, #6
.L_0811dfdc:
	ldr	r3, [r1, #0]
	cmp	r3, r2
	beq.n	.L_0811dfe4
	str	r2, [r1, #0]
.L_0811dfe4:
	movs	r1, #0
	movs	r0, #0
	bl	sub_08126cfc
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	movs	r6, #0
	adds	r3, #65
	ldrb	r0, [r3, #0]
	movs	r3, #2
	negs	r3, r3
	ands	r0, r3
	bl	sub_08038130
	ldr	r0, [sp, #12]
	bl	sub_0811be3c
	ldr	r0, [r0, #0]
	ldr	r3, [pc, #44]
	movs	r2, #128
	mov	fp, r0
	lsls	r2, r2, #19
	mov	r0, sp
	adds	r2, #80
	adds	r0, #108
	strh	r3, [r2, #0]
	str	r0, [sp, #4]
	ldr	r1, [sp, #4]
	movs	r0, #3
	bl	sub_0811a31c
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_0811e094
	ldr	r2, [sp, #16]
	movs	r1, #2
	ldr	r5, [sp, #4]
	ands	r2, r1
	mov	r9, r2
	b.n	.L_0811e040
	movs	r0, r0
	.4byte 0x00003f40
	.2byte 0xe000
	.2byte 0xffff
.L_0811e040:
	.2byte 0x882b
	cmp	r3, #254
	beq.n	.L_0811e08c
	mov	r3, r9
	cmp	r3, #0
	bne.n	.L_0811e056
	ldrh	r0, [r5, #0]
	ldr	r1, [sp, #12]
	cmp	r0, r1
	beq.n	.L_0811e076
	b.n	.L_0811e058
.L_0811e056:
	ldrh	r0, [r5, #0]
.L_0811e058:
	ldr	r3, [sp, #8]
	movs	r2, #0
	cmp	r3, #7
	bhi.n	.L_0811e062
	movs	r2, #1
.L_0811e062:
	movs	r3, #0
	cmp	r0, #7
	bhi.n	.L_0811e06a
	movs	r3, #1
.L_0811e06a:
	cmp	r2, r3
	beq.n	.L_0811e08c
	movs	r1, #1
	bl	sub_08126ae4
	b.n	.L_0811e08c
.L_0811e076:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #105
	adds	r3, r3, r0
	ldrb	r1, [r3, #0]
	mov	r0, fp
	bl	sub_08020090
.L_0811e08c:
	adds	r6, #1
	adds	r5, #2
	cmp	r6, r7
	bne.n	.L_0811e040
.L_0811e094:
	ldr	r1, [sp, #16]
	movs	r3, #2
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_0811e0b4
	movs	r0, #154
	bl	sub_081c0010
	mov	r2, r8
	mov	r3, sl
	ldr	r0, [r2, #8]
	ldr	r1, [r3, #80]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08127308
.L_0811e0b4:
	ldr	r0, [sp, #16]
	movs	r3, #1
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_0811e0c6
	ldr	r0, [sp, #12]
	movs	r1, #1
	bl	sub_08126ae4
.L_0811e0c6:
	movs	r1, #128
	ldr	r2, [pc, #12]
	lsls	r1, r1, #19
	ldr	r5, [pc, #12]
	adds	r1, #82
	movs	r6, #0
	mov	fp, r1
	mov	r9, r2
	b.n	.L_0811e0e0
	.4byte 0x00000010
	.2byte 0x1000
	.2byte 0x0000
.L_0811e0e0:
	mov	r0, r9
	subs	r3, r0, r6
	orrs	r3, r5
	mov	r1, fp
	strh	r3, [r1, #0]
	movs	r0, #1
	adds	r6, #1
	bl	sub_08013560
	cmp	r6, #16
	bne.n	.L_0811e0e0
	mov	r2, sl
	ldr	r3, [r2, #92]
	cmp	r3, #0
	beq.n	.L_0811e126
	cmp	r3, #1
	bne.n	.L_0811e114
	ldrb	r1, [r2, #0]
	movs	r0, #0
	bl	sub_08120360
	ldr	r1, [pc, #72]
	movs	r0, #4
	bl	sub_08120360
	b.n	.L_0811e11c
.L_0811e114:
	ldr	r1, [pc, #64]
	movs	r0, #4
	bl	sub_08120360
.L_0811e11c:
	bl	sub_081201c4
	bl	sub_0812756c
	b.n	.L_0811e29a
.L_0811e126:
	movs	r6, #0
	movs	r2, #0
	cmp	r6, r7
	bcs.n	.L_0811e17e
	ldr	r0, [sp, #16]
	ldr	r5, [sp, #4]
	movs	r3, #1
	ands	r0, r3
	mov	ip, r0
	adds	r1, r5, #0
.L_0811e13a:
	ldrh	r3, [r5, #0]
	ldr	r0, [sp, #12]
	adds	r5, #2
	cmp	r3, r0
	bne.n	.L_0811e15c
	mov	r3, ip
	cmp	r3, #0
	bne.n	.L_0811e178
	add	r0, sp, #12
	ldrh	r0, [r0, #0]
	adds	r2, #1
	strh	r0, [r1, #0]
	b.n	.L_0811e176
	.4byte 0x00000cad
	.2byte 0x0cac
	.2byte 0x0000
.L_0811e15c:
	ldr	r0, [sp, #8]
	movs	r4, #0
	cmp	r0, #7
	bls.n	.L_0811e166
	movs	r4, #1
.L_0811e166:
	movs	r0, #0
	cmp	r3, #7
	bhi.n	.L_0811e16e
	movs	r0, #1
.L_0811e16e:
	cmp	r4, r0
	beq.n	.L_0811e178
	strh	r3, [r1, #0]
	adds	r2, #1
.L_0811e176:
	adds	r1, #2
.L_0811e178:
	adds	r6, #1
	cmp	r6, r7
	bcc.n	.L_0811e13a
.L_0811e17e:
	ldr	r3, [pc, #36]
	ldr	r1, [sp, #4]
	lsls	r2, r2, #1
	strh	r3, [r1, r2]
	ldr	r0, [sp, #4]
	movs	r1, #0
	bl	sub_0811b9d4
	mov	r2, sl
	movs	r3, #1
	ldrsb	r3, [r2, r3]
	movs	r6, #0
	cmp	r6, r3
	bcs.n	.L_0811e1b6
	ldr	r1, [sp, #4]
	mov	ip, r3
	adds	r2, #3
	b.n	.L_0811e1a8
	movs	r0, r0
	.2byte 0x00ff
	.2byte 0x0000
.L_0811e1a8:
	ldrb	r3, [r2, #0]
	adds	r6, #1
	strh	r3, [r1, #0]
	adds	r2, #1
	adds	r1, #2
	cmp	r6, ip
	bcc.n	.L_0811e1a8
.L_0811e1b6:
	ldr	r2, [pc, #20]
	ldr	r0, [sp, #4]
	lsls	r3, r6, #1
	strh	r2, [r0, r3]
	mov	r2, r8
	ldr	r3, [r2, #20]
	movs	r6, #0
	cmp	r3, #0
	beq.n	.L_0811e212
	movs	r5, #0
	b.n	.L_0811e1d0
	.2byte 0x00ff
	.2byte 0x0000
.L_0811e1d0:
	lsls	r3, r6, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	bl	sub_0811be3c
	movs	r1, #0
	ldr	r0, [r0, #0]
	bl	sub_0811bdb0
	ldrb	r3, [r0, #27]
	movs	r1, #0
	subs	r3, #1
	cmp	r3, #0
	beq.n	.L_0811e206
	add	r2, sp, #136
	mov	ip, r3
	adds	r3, r2, r5
	adds	r2, r3, #0
	subs	r2, #62
	adds	r0, #40
.L_0811e1f8:
	ldmia	r0!, {r3}
	adds	r1, #1
	ldrb	r3, [r3, #5]
	strb	r3, [r2, #0]
	adds	r2, #1
	cmp	r1, ip
	bne.n	.L_0811e1f8
.L_0811e206:
	mov	r2, r8
	ldr	r3, [r2, #20]
	adds	r6, #1
	adds	r5, #4
	cmp	r6, r3
	bne.n	.L_0811e1d0
.L_0811e212:
	mov	r3, sl
	ldr	r2, [r3, #88]
	movs	r3, #128
	lsls	r3, r3, #8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0811e22e
	ldr	r0, [sp, #8]
	cmp	r0, #7
	bls.n	.L_0811e236
	movs	r3, #0
	mov	r2, r8
	str	r3, [r2, #4]
	b.n	.L_0811e242
.L_0811e22e:
	mov	r0, sl
	ldrb	r3, [r0, #3]
	cmp	r3, #7
	bhi.n	.L_0811e23e
.L_0811e236:
	movs	r3, #1
	mov	r1, r8
	str	r3, [r1, #4]
	b.n	.L_0811e242
.L_0811e23e:
	mov	r3, r8
	str	r2, [r3, #4]
.L_0811e242:
	mov	r0, sl
	ldr	r3, [r0, #88]
	movs	r2, #128
	lsls	r2, r2, #10
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0811e25a
	mov	r1, r8
	ldr	r3, [r1, #4]
	movs	r2, #1
	eors	r3, r2
	str	r3, [r1, #4]
.L_0811e25a:
	movs	r1, #144
	ldr	r0, [pc, #44]
	lsls	r1, r1, #3
	bl	sub_080145a8
	mov	r2, sl
	ldr	r0, [r2, #88]
	movs	r3, #128
	lsls	r3, r3, #8
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_0811e27a
	mov	r0, r8
	bl	sub_08138010
	b.n	.L_0811e296
.L_0811e27a:
	movs	r3, #128
	lsls	r3, r3, #7
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_0811e290
	mov	r0, r8
	bl	sub_08138008
	b.n	.L_0811e296
	.2byte 0x2d11
	.2byte 0x0812
.L_0811e290:
	mov	r0, r8
	bl	sub_08138018
.L_0811e296:
	bl	sub_081234f0
.L_0811e29a:
	bl	sub_0811a44c
	movs	r0, #3
	ldr	r1, [sp, #4]
	bl	sub_0811a31c
	movs	r2, #128
	ldr	r3, [pc, #24]
	lsls	r2, r2, #19
	adds	r2, #80
	adds	r7, r0, #0
	strh	r3, [r2, #0]
	movs	r6, #0
	cmp	r7, #0
	beq.n	.L_0811e30a
	ldr	r3, [sp, #16]
	ldr	r5, [sp, #4]
	movs	r4, #2
	ands	r4, r3
	b.n	.L_0811e2c8
	movs	r0, r0
	.2byte 0x3f40
	.2byte 0x0000
.L_0811e2c8:
	ldrh	r3, [r5, #0]
	cmp	r3, #254
	beq.n	.L_0811e302
	cmp	r4, #0
	bne.n	.L_0811e2e0
	ldr	r0, [sp, #12]
	cmp	r3, r0
	bne.n	.L_0811e2e0
	mov	r1, r8
	ldr	r3, [r1, #0]
	cmp	r3, #3
	beq.n	.L_0811e302
.L_0811e2e0:
	ldr	r3, [sp, #8]
	ldrh	r0, [r5, #0]
	movs	r2, #0
	cmp	r3, #7
	bhi.n	.L_0811e2ec
	movs	r2, #1
.L_0811e2ec:
	movs	r3, #0
	cmp	r0, #7
	bhi.n	.L_0811e2f4
	movs	r3, #1
.L_0811e2f4:
	cmp	r2, r3
	beq.n	.L_0811e302
	movs	r1, #1
	str	r4, [sp, #0]
	bl	sub_08126ae4
	ldr	r4, [sp, #0]
.L_0811e302:
	adds	r6, #1
	adds	r5, #2
	cmp	r6, r7
	bne.n	.L_0811e2c8
.L_0811e30a:
	movs	r0, #128
	lsls	r0, r0, #19
	ldr	r5, [pc, #36]
	adds	r0, #82
	movs	r6, #0
	mov	r8, r0
.L_0811e316:
	adds	r3, r6, #0
	orrs	r3, r5
	mov	r1, r8
	strh	r3, [r1, #0]
	movs	r0, #1
	adds	r6, #1
	bl	sub_08013560
	cmp	r6, #16
	bne.n	.L_0811e316
	movs	r6, #0
	cmp	r7, #0
	beq.n	.L_0811e348
	ldr	r5, [sp, #4]
	b.n	.L_0811e338
	.2byte 0x1000
	.2byte 0x0000
.L_0811e338:
	ldrh	r0, [r5, #0]
	movs	r1, #0
	adds	r6, #1
	adds	r5, #2
	bl	sub_08126ae4
	cmp	r6, r7
	bne.n	.L_0811e338
.L_0811e348:
	movs	r1, #0
	movs	r2, #0
	movs	r3, #100
	movs	r0, #0
	bl	sub_08126804
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #0
	add	sp, #136
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
