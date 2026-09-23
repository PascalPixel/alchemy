.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_08013eb4, 0x08013eb4
	.set sub_080145a8, 0x080145a8
	.set sub_08014878, 0x08014878
	.set sub_0801489c, 0x0801489c
	.set sub_080148e8, 0x080148e8
	.set sub_080149e0, 0x080149e0
	.set sub_08014d00, 0x08014d00
	.set sub_08014d78, 0x08014d78
	.set sub_08014dac, 0x08014dac
	.set sub_0801587c, 0x0801587c
	.set sub_080158cc, 0x080158cc
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016d18, 0x08016d18
	.set sub_08022b04, 0x08022b04
	.set sub_08023220, 0x08023220
	.set sub_080233a8, 0x080233a8
	.set sub_080233d0, 0x080233d0
	.set sub_08023410, 0x08023410
	.set sub_08023664, 0x08023664
	.set sub_08024738, 0x08024738
	.set sub_08024f20, 0x08024f20
	.set sub_08026e60, 0x08026e60
	.set sub_08026f80, 0x08026f80
	.set sub_0802cc9c, 0x0802cc9c
	.set sub_0802ce4c, 0x0802ce4c
	.set sub_0802d45c, 0x0802d45c
	.set sub_0802d7b0, 0x0802d7b0
	.set sub_0802d87c, 0x0802d87c
	.set sub_0802da24, 0x0802da24
	.set sub_0802dac0, 0x0802dac0
	.set sub_0802db64, 0x0802db64
	.set sub_0802db88, 0x0802db88
	.set sub_0802dbd0, 0x0802dbd0
	.set sub_080ad318, 0x080ad318
	.set sub_080c8088, 0x080c8088
	.set sub_080c8710, 0x080c8710
	.set sub_080c8770, 0x080c8770
	.set sub_080c8828, 0x080c8828
	.set sub_081c0010, 0x081c0010
	.global Func_08027e20
	.thumb_func
Func_08027e20:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #100
	mov	r8, r0
	movs	r0, #0
	str	r0, [sp, #16]
	str	r0, [sp, #12]
	ldr	r3, [pc, #592]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08027e6e
	ldr	r3, [pc, #588]
	movs	r2, #128
	ldr	r3, [r3, #0]
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08027e6e
	adds	r3, r2, #0
.L_08027e50:
	subs	r3, #1
	cmp	r3, #0
	bne.n	.L_08027e50
	movs	r3, #95
.L_08027e58:
	subs	r3, #1
	cmp	r3, #0
	bge.n	.L_08027e58
	movs	r3, #63
.L_08027e60:
	subs	r3, #1
	cmp	r3, #0
	bge.n	.L_08027e60
	movs	r3, #63
.L_08027e68:
	subs	r3, #1
	cmp	r3, #0
	bge.n	.L_08027e68
.L_08027e6e:
	movs	r5, #128
	movs	r3, #128
	mov	r1, r8
	lsls	r5, r5, #9
	lsls	r3, r3, #7
	movs	r0, #128
	str	r5, [r1, #48]
	str	r3, [r1, #52]
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_08027ea0
	ldr	r0, [pc, #516]
	movs	r2, #2
	ldr	r3, [r0, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08027ea2
	movs	r3, #128
	mov	r2, r8
	lsls	r3, r3, #11
	str	r3, [r2, #48]
	str	r5, [r2, #52]
	b.n	.L_08027ea2
.L_08027ea0:
	ldr	r0, [pc, #492]
.L_08027ea2:
	ldr	r3, [r0, #0]
	ldr	r1, [pc, #492]
	movs	r2, #15
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r3, [r1, r3]
	movs	r2, #255
	lsls	r3, r3, #16
	lsls	r2, r2, #8
	lsrs	r1, r3, #16
	adds	r2, #255
	str	r3, [sp, #4]
	cmp	r1, r2
	bne.n	.L_08027eca
	ldr	r0, [sp, #16]
	movs	r3, #4
	orrs	r0, r3
	str	r0, [sp, #16]
	b.n	.L_08028376
.L_08027eca:
	movs	r2, #0
	str	r2, [sp, #16]
	movs	r3, #88
	mov	r0, r8
	add	r3, sp
	mov	fp, r3
	ldr	r3, [r0, #8]
	mov	r2, fp
	str	r3, [r2, #0]
	ldr	r3, [r0, #12]
	str	r3, [r2, #4]
	ldr	r3, [r0, #16]
	movs	r0, #152
	str	r3, [r2, #8]
	lsls	r0, r0, #13
	bl	sub_0801489c
	mov	r3, r8
	adds	r3, #35
	ldrb	r2, [r3, #0]
	movs	r3, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08027faa
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #20]
	ldr	r4, [pc, #404]
	movs	r5, #0
	movs	r0, #128
	mov	r9, r5
	lsls	r0, r0, #12
.L_08027f0a:
	ldr	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L_08027f82
	adds	r3, r7, #0
	adds	r3, #35
	ldrb	r2, [r3, #0]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08027f82
	cmp	r7, r8
	beq.n	.L_08027f82
	mov	r3, r8
	ldr	r2, [r3, #12]
	ldr	r3, [r7, #12]
	ldr	r1, [pc, #368]
	subs	r2, r2, r3
	adds	r3, r2, r1
	cmp	r3, #0
	bge.n	.L_08027f38
	movs	r3, #128
	lsls	r3, r3, #13
	subs	r3, r3, r2
.L_08027f38:
	cmp	r3, r4
	bgt.n	.L_08027f82
	mov	r2, r8
	ldr	r3, [r2, #8]
	ldr	r1, [r7, #8]
	subs	r3, r3, r1
	adds	r3, r3, r0
	asrs	r3, r3, #20
	cmp	r3, #0
	bne.n	.L_08027f60
	ldr	r3, [r2, #16]
	ldr	r2, [r7, #16]
	subs	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r3, r3, r2
	asrs	r3, r3, #20
	cmp	r3, #0
	bne.n	.L_08027f60
	movs	r5, #1
.L_08027f60:
	mov	r2, fp
	ldr	r3, [r2, #0]
	subs	r3, r3, r1
	adds	r3, r3, r0
	asrs	r3, r3, #20
	cmp	r3, #0
	bne.n	.L_08027f82
	ldr	r3, [r2, #8]
	ldr	r2, [r7, #16]
	movs	r1, #128
	subs	r3, r3, r2
	lsls	r1, r1, #12
	adds	r3, r3, r1
	asrs	r3, r3, #20
	cmp	r3, #0
	bne.n	.L_08027f82
	b.n	.L_080281ca
.L_08027f82:
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	adds	r7, #128
	cmp	r3, #63
	ble.n	.L_08027f0a
	cmp	r5, #0
	beq.n	.L_08027faa
	mov	r0, r8
	mov	r1, fp
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08027faa
	ldr	r1, [sp, #4]
	movs	r0, #0
	asrs	r1, r1, #16
	str	r0, [sp, #16]
	str	r1, [sp, #8]
	b.n	.L_08028376
.L_08027faa:
	ldr	r3, [pc, #224]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08027fcc
	ldr	r3, [pc, #220]
	movs	r2, #128
	ldr	r3, [r3, #0]
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08027fcc
	ldr	r3, [sp, #4]
	movs	r2, #0
	asrs	r3, r3, #16
	str	r2, [sp, #16]
	str	r3, [sp, #8]
	b.n	.L_08028376
.L_08027fcc:
	mov	r0, r8
	mov	r1, fp
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_080280a8
	mov	r0, r8
	ldr	r3, [r0, #8]
	add	r5, sp, #76
	str	r3, [r5, #0]
	movs	r7, #152
	ldr	r3, [r0, #12]
	movs	r2, #128
	str	r3, [r5, #4]
	lsls	r2, r2, #5
	ldr	r3, [r0, #16]
	lsls	r7, r7, #13
	str	r3, [r5, #8]
	ldr	r1, [sp, #4]
	adds	r0, r7, #0
	lsrs	r6, r1, #16
	adds	r1, r6, r2
	adds	r2, r5, #0
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_080280a8
	mov	r0, r8
	ldr	r3, [r0, #8]
	ldr	r2, [pc, #144]
	str	r3, [r5, #0]
	adds	r1, r6, r2
	ldr	r3, [r0, #12]
	adds	r2, r5, #0
	str	r3, [r5, #4]
	ldr	r3, [r0, #16]
	adds	r0, r7, #0
	str	r3, [r5, #8]
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_080280a8
	mov	r0, r8
	ldr	r3, [r0, #8]
	movs	r2, #128
	str	r3, [r5, #0]
	lsls	r2, r2, #6
	ldr	r3, [r0, #12]
	adds	r1, r6, r2
	str	r3, [r5, #4]
	adds	r2, r5, #0
	ldr	r3, [r0, #16]
	adds	r0, r7, #0
	str	r3, [r5, #8]
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_080280a8
	mov	r0, r8
	ldr	r3, [r0, #8]
	ldr	r2, [pc, #68]
	str	r3, [r5, #0]
	adds	r1, r6, r2
	ldr	r3, [r0, #12]
	adds	r2, r5, #0
	str	r3, [r5, #4]
	ldr	r3, [r0, #16]
	adds	r0, r7, #0
	str	r3, [r5, #8]
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_080280a8
	ldr	r3, [sp, #4]
	asrs	r3, r3, #16
	lsls	r0, r3, #16
	str	r3, [sp, #8]
	str	r0, [sp, #0]
	b.n	.L_080281ee
	movs	r0, r0
	.4byte 0x03001238
	.4byte 0x03001150
	.4byte 0x0802ec5c
	.4byte 0x0007ffff
	.4byte 0xfff00000
	.4byte 0xfffff000
	.2byte 0xe000
	.2byte 0xffff
.L_080280a8:
	.2byte 0x9a01
	movs	r0, #128
	lsrs	r3, r2, #16
	lsls	r0, r0, #5
	adds	r2, r3, r0
	ldr	r0, [pc, #784]
	add	r1, sp, #20
	strh	r2, [r1, #0]
	adds	r2, r3, r0
	movs	r0, #128
	lsls	r0, r0, #6
	strh	r2, [r1, #2]
	adds	r2, r3, r0
	ldr	r0, [pc, #772]
	strh	r2, [r1, #4]
	adds	r2, r3, r0
	movs	r0, #192
	lsls	r0, r0, #6
	strh	r2, [r1, #6]
	adds	r2, r3, r0
	strh	r2, [r1, #8]
	ldr	r2, [pc, #760]
	mov	sl, r1
	adds	r3, r3, r2
	mov	r0, sl
	strh	r3, [r0, #10]
	movs	r1, #0
	mov	r9, r1
	mov	r7, fp
.L_080280e2:
	mov	r2, r9
	lsls	r3, r2, #1
	mov	r0, sl
	ldrsh	r0, [r0, r3]
	mov	r2, r8
	str	r0, [sp, #8]
	ldr	r3, [r2, #8]
	str	r3, [r7, #0]
	ldr	r3, [r2, #12]
	str	r3, [r7, #4]
	ldr	r3, [r2, #16]
	adds	r2, r7, #0
	str	r3, [r7, #8]
	lsls	r3, r0, #16
	lsrs	r6, r3, #16
	movs	r0, #152
	lsls	r0, r0, #13
	adds	r1, r6, #0
	str	r3, [sp, #0]
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r7, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_080281be
	mov	r0, r8
	ldr	r3, [r0, #8]
	add	r5, sp, #76
	str	r3, [r5, #0]
	movs	r2, #128
	ldr	r3, [r0, #12]
	lsls	r2, r2, #5
	str	r3, [r5, #4]
	adds	r1, r6, r2
	ldr	r3, [r0, #16]
	movs	r0, #152
	lsls	r0, r0, #13
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_080281be
	mov	r0, r8
	ldr	r3, [r0, #8]
	ldr	r2, [pc, #632]
	str	r3, [r5, #0]
	adds	r1, r6, r2
	ldr	r3, [r0, #12]
	adds	r2, r5, #0
	str	r3, [r5, #4]
	ldr	r3, [r0, #16]
	movs	r0, #152
	lsls	r0, r0, #13
	str	r3, [r5, #8]
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_080281be
	mov	r0, r8
	ldr	r3, [r0, #8]
	movs	r2, #128
	str	r3, [r5, #0]
	lsls	r2, r2, #6
	ldr	r3, [r0, #12]
	adds	r1, r6, r2
	str	r3, [r5, #4]
	adds	r2, r5, #0
	ldr	r3, [r0, #16]
	movs	r0, #152
	lsls	r0, r0, #13
	str	r3, [r5, #8]
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_080281be
	mov	r0, r8
	ldr	r3, [r0, #8]
	ldr	r2, [pc, #556]
	str	r3, [r5, #0]
	adds	r1, r6, r2
	ldr	r3, [r0, #12]
	adds	r2, r5, #0
	str	r3, [r5, #4]
	ldr	r3, [r0, #16]
	movs	r0, #152
	lsls	r0, r0, #13
	str	r3, [r5, #8]
	bl	sub_0801489c
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	beq.n	.L_080281ee
.L_080281be:
	movs	r3, #1
	add	r9, r3
	mov	r0, r9
	cmp	r0, #6
	blt.n	.L_080280e2
	b.n	.L_080281d6
.L_080281ca:
	ldr	r2, [sp, #4]
	movs	r1, #0
	asrs	r2, r2, #16
	str	r1, [sp, #16]
	str	r2, [sp, #8]
	b.n	.L_08028376
.L_080281d6:
	mov	r0, r8
	ldr	r3, [r0, #8]
	mov	r1, fp
	str	r3, [r1, #0]
	ldr	r3, [r0, #12]
	str	r3, [r1, #4]
	ldr	r3, [r0, #16]
	str	r3, [r1, #8]
	ldr	r2, [sp, #16]
	movs	r3, #1
	orrs	r2, r3
	str	r2, [sp, #16]
.L_080281ee:
	movs	r3, #64
	mov	r0, r8
	add	r3, sp
	mov	fp, r3
	ldr	r3, [r0, #8]
	mov	r1, fp
	str	r3, [r1, #0]
	ldr	r3, [r0, #12]
	str	r3, [r1, #4]
	ldr	r3, [r0, #16]
	movs	r0, #240
	str	r3, [r1, #8]
	ldr	r2, [sp, #0]
	lsls	r0, r0, #12
	lsrs	r1, r2, #16
	mov	r2, fp
	bl	sub_0801489c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #20]
	movs	r3, #63
	adds	r6, r7, #0
	mov	r9, r3
	adds	r6, #8
.L_08028220:
	mov	r0, r8
	ldrh	r3, [r0, #32]
	subs	r1, r3, #2
	ldr	r3, [r7, #0]
	cmp	r3, #0
	bne.n	.L_0802822e
	b.n	.L_0802834a
.L_0802822e:
	adds	r3, r7, #0
	adds	r3, #89
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0802823e
	b.n	.L_0802834a
.L_0802823e:
	cmp	r7, r8
	bne.n	.L_08028244
	b.n	.L_0802834a
.L_08028244:
	ldrh	r3, [r6, #24]
	adds	r0, r6, #0
	subs	r3, #2
	mov	r2, fp
	bl	sub_08026f80
	cmp	r0, #0
	blt.n	.L_0802834a
	ldr	r3, [r6, #80]
	ldr	r2, [pc, #376]
	movs	r1, #128
	ands	r3, r2
	lsls	r1, r1, #2
	cmp	r3, r1
	bne.n	.L_08028342
	mov	r2, r8
	ldr	r3, [r2, #16]
	ldr	r0, [r6, #8]
	ldr	r1, [r6, #0]
	subs	r0, r0, r3
	ldr	r3, [r2, #8]
	subs	r1, r1, r3
	bl	sub_080148e8
	ldr	r3, [r6, #0]
	add	r5, sp, #76
	str	r3, [r5, #0]
	lsls	r0, r0, #16
	ldr	r3, [r6, #4]
	adds	r2, r5, #0
	str	r3, [r5, #4]
	ldr	r3, [r6, #8]
	str	r3, [r5, #8]
	asrs	r3, r0, #16
	lsrs	r0, r0, #16
	mov	sl, r0
	movs	r0, #128
	lsls	r0, r0, #7
	mov	r1, sl
	str	r3, [sp, #8]
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_08024f20
	cmp	r0, #0
	bne.n	.L_08028342
	ldr	r3, [r6, #0]
	movs	r0, #168
	str	r3, [r5, #0]
	lsls	r0, r0, #13
	ldr	r3, [r6, #4]
	mov	r1, sl
	str	r3, [r5, #4]
	adds	r2, r5, #0
	ldr	r3, [r6, #8]
	str	r3, [r5, #8]
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08028342
	ldr	r3, [r6, #0]
	movs	r1, #128
	str	r3, [r5, #0]
	lsls	r1, r1, #5
	ldr	r3, [r6, #4]
	movs	r0, #168
	str	r3, [r5, #4]
	add	r1, sl
	ldr	r3, [r6, #8]
	lsls	r0, r0, #13
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08028342
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08028342
	ldr	r3, [r6, #0]
	ldr	r1, [pc, #196]
	str	r3, [r5, #0]
	movs	r0, #168
	ldr	r3, [r6, #4]
	add	r1, sl
	str	r3, [r5, #4]
	lsls	r0, r0, #13
	ldr	r3, [r6, #8]
	adds	r2, r5, #0
	str	r3, [r5, #8]
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08028342
	movs	r0, #128
	lsls	r0, r0, #7
	mov	r1, sl
	adds	r2, r6, #0
	bl	sub_0801489c
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r6, #48]
	str	r3, [r6, #52]
	str	r3, [r6, #56]
	ldr	r0, [sp, #12]
	movs	r1, #1
	orrs	r0, r1
	str	r0, [sp, #12]
	b.n	.L_0802834a
.L_08028342:
	ldr	r2, [sp, #16]
	movs	r3, #2
	orrs	r2, r3
	str	r2, [sp, #16]
.L_0802834a:
	movs	r3, #1
	negs	r3, r3
	add	r9, r3
	mov	r0, r9
	adds	r6, #128
	adds	r7, #128
	cmp	r0, #0
	blt.n	.L_0802835c
	b.n	.L_08028220
.L_0802835c:
	ldr	r1, [sp, #16]
	cmp	r1, #0
	bne.n	.L_08028376
	ldr	r2, [sp, #12]
	cmp	r2, #0
	beq.n	.L_08028376
	movs	r3, #128
	lsls	r3, r3, #7
	mov	r0, r8
	str	r3, [r0, #48]
	movs	r3, #128
	lsls	r3, r3, #6
	str	r3, [r0, #52]
.L_08028376:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	cmp	r1, #0
	beq.n	.L_080283ac
	ldr	r3, [sp, #16]
	movs	r2, #3
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08028398
	movs	r0, #194
	lsls	r0, r0, #1
	adds	r2, r1, r0
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_080283a0
.L_08028398:
	movs	r0, #194
	lsls	r0, r0, #1
	adds	r3, r1, r0
	strh	r2, [r3, #0]
.L_080283a0:
	ldr	r3, [pc, #48]
	movs	r0, #195
	ldr	r3, [r3, #0]
	lsls	r0, r0, #1
	adds	r2, r1, r0
	strh	r3, [r2, #0]
.L_080283ac:
	ldr	r1, [sp, #12]
	cmp	r1, #0
	bne.n	.L_080283b8
	ldr	r2, [sp, #16]
	cmp	r2, #0
	beq.n	.L_080283d8
.L_080283b8:
	mov	r0, r8
	movs	r1, #1
	bl	sub_080233d0
	b.n	.L_080283e0
	movs	r0, r0
	.4byte 0xfffff000
	.4byte 0xffffe000
	.4byte 0xffffd000
	.4byte 0xff000200
	.2byte 0x1150
	.2byte 0x0300
.L_080283d8:
	mov	r0, r8
	movs	r1, #2
	bl	sub_080233d0
.L_080283e0:
	ldr	r3, [pc, #316]
	ldr	r1, [pc, #320]
	movs	r0, #140
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldrh	r2, [r3, #0]
	ldr	r3, [r1, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0802840c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #181
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r3, #252
	strh	r3, [r2, #0]
	ldr	r2, [sp, #16]
	movs	r3, #4
	orrs	r2, r3
	str	r2, [sp, #16]
.L_0802840c:
	ldr	r3, [sp, #16]
	cmp	r3, #0
	beq.n	.L_08028466
	movs	r3, #128
	mov	r0, r8
	lsls	r3, r3, #24
	str	r3, [r0, #56]
	str	r3, [r0, #60]
	str	r3, [r0, #64]
	movs	r3, #0
	str	r3, [r0, #36]
	str	r3, [r0, #44]
	ldr	r1, [sp, #16]
	movs	r3, #3
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08028454
	ldrh	r2, [r0, #6]
	ldr	r0, [sp, #4]
	movs	r1, #128
	lsrs	r3, r0, #16
	subs	r3, r3, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	lsls	r1, r1, #5
	cmp	r3, r1
	ble.n	.L_08028446
	movs	r3, #128
	lsls	r3, r3, #3
.L_08028446:
	ldr	r0, [pc, #224]
	cmp	r3, r0
	bge.n	.L_0802844e
	ldr	r3, [pc, #220]
.L_0802844e:
	adds	r3, r2, r3
	mov	r1, r8
	strh	r3, [r1, #6]
.L_08028454:
	mov	r3, r8
	adds	r3, #100
	movs	r2, #0
	strh	r2, [r3, #0]
	mov	r2, r8
	adds	r2, #102
	movs	r3, #2
	strh	r3, [r2, #0]
	b.n	.L_080284ee
.L_08028466:
	add	r3, sp, #88
	ldr	r1, [r3, #0]
	ldr	r2, [r3, #4]
	mov	r0, r8
	ldr	r3, [r3, #8]
	bl	sub_08024738
	mov	r2, r8
	ldr	r1, [r2, #36]
	ldr	r6, [pc, #180]
	adds	r0, r1, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4643
	ldr	r1, [r3, #44]
	adds	r5, r0, #0
	adds	r0, r1, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x182d
	adds	r0, r5, #0
	bl	sub_080149e0
	ldr	r1, [sp, #16]
	mov	r2, r8
	str	r1, [r2, #36]
	str	r1, [r2, #44]
	ldr	r3, [sp, #8]
	adds	r2, #36
	lsls	r1, r3, #16
	lsrs	r1, r1, #16
	bl	sub_0801489c
	mov	r2, r8
	adds	r2, #100
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	ldrh	r1, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080284ba
	subs	r3, r1, #1
	strh	r3, [r2, #0]
.L_080284ba:
	mov	r1, r8
	adds	r1, #90
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldr	r0, [sp, #4]
	mov	r1, r8
	ldrh	r2, [r1, #6]
	lsrs	r3, r0, #16
	subs	r3, r3, r2
	lsls	r3, r3, #16
	movs	r1, #128
	asrs	r3, r3, #16
	lsls	r1, r1, #5
	cmp	r3, r1
	ble.n	.L_080284e0
	movs	r3, #128
	lsls	r3, r3, #3
.L_080284e0:
	ldr	r0, [pc, #68]
	cmp	r3, r0
	bge.n	.L_080284e8
	ldr	r3, [pc, #68]
.L_080284e8:
	adds	r3, r2, r3
	mov	r1, r8
	strh	r3, [r1, #6]
.L_080284ee:
	mov	r3, r8
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_08028500
	mov	r0, r8
	adds	r0, #8
	bl	sub_0802dac0
.L_08028500:
	bl	sub_08026e60
	mov	r2, r8
	ldrh	r3, [r2, #4]
	mov	r0, r8
	adds	r3, #1
	strh	r3, [r0, #4]
	add	sp, #100
	movs	r0, #1
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x03001150
	.4byte 0xfffff000
	.4byte 0xfffffc00
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	adds	r1, r7, #0
	adds	r1, #8
	sub	sp, #96
	movs	r0, #0
	str	r0, [sp, #4]
	adds	r0, r1, #0
	str	r1, [sp, #0]
	bl	sub_0802dac0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	subs	r3, r0, #1
	cmp	r3, #2
	bhi.n	.L_08028570
	movs	r2, #208
	add	r4, sp, #4
	lsls	r2, r2, #4
	ldrb	r4, [r4, #0]
	adds	r2, #58
	adds	r3, r1, r2
	strb	r4, [r3, #0]
.L_08028570:
	subs	r3, r0, #5
	cmp	r3, #1
	bhi.n	.L_08028582
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #58
	adds	r2, r1, r3
	movs	r3, #1
	strb	r3, [r2, #0]
.L_08028582:
	cmp	r0, #4
	bne.n	.L_08028592
	movs	r4, #208
	lsls	r4, r4, #4
	adds	r4, #58
	adds	r2, r1, r4
	movs	r3, #2
	strb	r3, [r2, #0]
.L_08028592:
	subs	r3, r0, #7
	cmp	r3, #5
	bhi.n	.L_080285a4
	movs	r0, #208
	lsls	r0, r0, #4
	adds	r0, #58
	adds	r2, r1, r0
	movs	r3, #3
	strb	r3, [r2, #0]
.L_080285a4:
	movs	r3, #160
	movs	r1, #0
	lsls	r3, r3, #9
	str	r1, [sp, #12]
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #52]
	ldr	r5, [pc, #844]
	movs	r2, #15
	ldr	r3, [r5, #0]
	ldr	r1, [pc, #844]
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r2, [r1, r3]
	movs	r0, #255
	str	r2, [sp, #8]
	lsls	r0, r0, #8
	lsls	r2, r2, #16
	lsrs	r6, r2, #16
	adds	r0, #255
	mov	r9, r2
	cmp	r6, r0
	bne.n	.L_080285e4
	movs	r1, #4
	str	r1, [sp, #12]
	b.n	.L_080287f6
.L_080285dc:
	mov	r2, fp
	asrs	r2, r2, #16
	str	r2, [sp, #8]
	b.n	.L_080287f6
.L_080285e4:
	ldr	r4, [sp, #0]
	add	r3, sp, #84
	mov	r8, r3
	ldr	r3, [r4, #0]
	mov	r0, r8
	str	r3, [r0, #0]
	ldr	r3, [r7, #12]
	movs	r1, #128
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	lsls	r1, r1, #12
	mov	sl, r1
	str	r3, [r0, #8]
	adds	r1, r6, #0
	mov	r0, sl
	mov	r2, r8
	bl	sub_0801489c
	ldr	r3, [pc, #768]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0802861e
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0802861e
	b.n	.L_080287f6
.L_0802861e:
	adds	r0, r7, #0
	mov	r1, r8
	bl	sub_0802db88
	cmp	r0, #0
	bne.n	.L_080286ca
	ldr	r2, [sp, #0]
	add	r5, sp, #72
	ldr	r3, [r2, #0]
	mov	r0, sl
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	adds	r2, r5, #0
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	str	r3, [r5, #8]
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r1, r6, r3
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802db88
	cmp	r0, #0
	bne.n	.L_080286ca
	ldr	r4, [sp, #0]
	ldr	r0, [pc, #696]
	ldr	r3, [r4, #0]
	adds	r1, r6, r0
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	mov	r0, sl
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	adds	r2, r5, #0
	str	r3, [r5, #8]
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802db88
	cmp	r0, #0
	bne.n	.L_080286ca
	ldr	r1, [sp, #0]
	movs	r2, #128
	ldr	r3, [r1, #0]
	lsls	r2, r2, #6
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	adds	r1, r6, r2
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	mov	r0, sl
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802db88
	cmp	r0, #0
	bne.n	.L_080286ca
	ldr	r4, [sp, #0]
	ldr	r0, [pc, #620]
	ldr	r3, [r4, #0]
	adds	r1, r6, r0
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	mov	r0, sl
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	adds	r2, r5, #0
	str	r3, [r5, #8]
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802db88
	cmp	r0, #0
	bne.n	.L_080286ca
	b.n	.L_080287f6
.L_080286ca:
	add	r1, sp, #16
	mov	sl, r1
	mov	r2, r9
	ldr	r1, [pc, #572]
	movs	r4, #128
	lsrs	r3, r2, #16
	lsls	r4, r4, #5
	adds	r2, r3, r4
	mov	r0, sl
	strh	r2, [r0, #0]
	mov	r4, sl
	adds	r2, r3, r1
	strh	r2, [r4, #2]
	movs	r0, #128
	ldr	r4, [pc, #556]
	lsls	r0, r0, #6
	adds	r2, r3, r0
	mov	r1, sl
	strh	r2, [r1, #4]
	mov	r0, sl
	adds	r2, r3, r4
	strh	r2, [r0, #6]
	movs	r1, #192
	ldr	r0, [pc, #540]
	lsls	r1, r1, #6
	adds	r2, r3, r1
	mov	r4, sl
	strh	r2, [r4, #8]
	adds	r3, r3, r0
	mov	r1, sl
	movs	r2, #6
	strh	r3, [r1, #10]
	str	r2, [sp, #4]
	movs	r3, #0
	mov	r9, r3
.L_08028710:
	mov	r4, r9
	lsls	r3, r4, #1
	mov	r0, sl
	ldrsh	r2, [r0, r3]
	ldr	r3, [r7, #8]
	mov	r4, r8
	str	r3, [r4, #0]
	ldr	r3, [r7, #12]
	lsls	r2, r2, #16
	str	r3, [r4, #4]
	ldr	r3, [r7, #16]
	lsrs	r6, r2, #16
	movs	r0, #128
	lsls	r0, r0, #12
	adds	r1, r6, #0
	str	r3, [r4, #8]
	mov	fp, r2
	mov	r2, r8
	bl	sub_0801489c
	adds	r0, r7, #0
	mov	r1, r8
	bl	sub_0802db88
	cmp	r0, #0
	bne.n	.L_080287e4
	ldr	r3, [r7, #8]
	add	r5, sp, #72
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	movs	r0, #128
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	lsls	r0, r0, #5
	adds	r1, r6, r0
	movs	r0, #128
	lsls	r0, r0, #12
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802db88
	cmp	r0, #0
	bne.n	.L_080287e4
	ldr	r3, [r7, #8]
	ldr	r2, [pc, #412]
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	movs	r0, #128
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	adds	r1, r6, r2
	lsls	r0, r0, #12
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802db88
	cmp	r0, #0
	bne.n	.L_080287e4
	ldr	r3, [r7, #8]
	movs	r0, #128
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	lsls	r0, r0, #12
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	adds	r2, r5, #0
	str	r3, [r5, #8]
	movs	r3, #128
	lsls	r3, r3, #6
	adds	r1, r6, r3
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802db88
	cmp	r0, #0
	bne.n	.L_080287e4
	ldr	r3, [r7, #8]
	ldr	r4, [pc, #340]
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	movs	r0, #128
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	adds	r1, r6, r4
	lsls	r0, r0, #12
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802db88
	cmp	r0, #0
	bne.n	.L_080287e4
	b.n	.L_080285dc
.L_080287e4:
	ldr	r1, [sp, #4]
	movs	r0, #1
	add	r9, r0
	cmp	r9, r1
	blt.n	.L_08028710
	ldr	r2, [sp, #12]
	movs	r3, #1
	orrs	r2, r3
	str	r2, [sp, #12]
.L_080287f6:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	cmp	r1, #0
	beq.n	.L_0802882c
	ldr	r3, [sp, #12]
	movs	r2, #3
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08028818
	movs	r4, #194
	lsls	r4, r4, #1
	adds	r2, r1, r4
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_08028820
.L_08028818:
	movs	r0, #194
	lsls	r0, r0, #1
	adds	r3, r1, r0
	strh	r2, [r3, #0]
.L_08028820:
	ldr	r3, [pc, #224]
	movs	r4, #195
	ldr	r3, [r3, #0]
	lsls	r4, r4, #1
	adds	r2, r1, r4
	strh	r3, [r2, #0]
.L_0802882c:
	ldr	r0, [sp, #12]
	cmp	r0, #0
	beq.n	.L_0802883c
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_080233d0
	b.n	.L_08028844
.L_0802883c:
	adds	r0, r7, #0
	movs	r1, #2
	bl	sub_080233d0
.L_08028844:
	ldr	r1, [pc, #212]
	ldr	r0, [pc, #188]
	movs	r2, #140
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r0, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08028870
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r4, #181
	lsls	r4, r4, #1
	adds	r2, r3, r4
	movs	r3, #252
	strh	r3, [r2, #0]
	ldr	r2, [sp, #12]
	movs	r3, #4
	orrs	r2, r3
	str	r2, [sp, #12]
.L_08028870:
	movs	r4, #143
	lsls	r4, r4, #2
	adds	r3, r1, r4
	ldrh	r2, [r3, #0]
	ldr	r3, [r0, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080288aa
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	movs	r0, #208
	lsls	r0, r0, #4
	adds	r0, #57
	adds	r3, r5, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080288aa
	movs	r0, #72
	adds	r0, #255
	bl	sub_08016d18
	movs	r1, #181
	lsls	r1, r1, #1
	adds	r2, r5, r1
	movs	r3, #253
	strh	r3, [r2, #0]
.L_080288aa:
	ldr	r2, [sp, #12]
	cmp	r2, #0
	beq.n	.L_08028920
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r7, #56]
	str	r3, [r7, #60]
	str	r3, [r7, #64]
	movs	r3, #0
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080288ec
	ldr	r4, [sp, #8]
	ldrh	r2, [r7, #6]
	lsls	r3, r4, #16
	lsrs	r3, r3, #16
	subs	r3, r3, r2
	lsls	r3, r3, #16
	asrs	r1, r3, #16
	movs	r3, #128
	lsls	r3, r3, #5
	cmp	r1, r3
	ble.n	.L_080288e0
	adds	r1, r3, #0
.L_080288e0:
	ldr	r3, [pc, #44]
	cmp	r1, r3
	bge.n	.L_080288e8
	adds	r1, r3, #0
.L_080288e8:
	adds	r3, r2, r1
	strh	r3, [r7, #6]
.L_080288ec:
	movs	r0, #100
	adds	r0, r0, r7
	mov	r8, r0
	movs	r3, #0
	mov	r1, r8
	adds	r2, r7, #0
	strh	r3, [r1, #0]
	adds	r2, #102
	movs	r3, #2
	strh	r3, [r2, #0]
	b.n	.L_080289a2
	movs	r0, r0
	.4byte 0x03001150
	.4byte 0x0802ec5c
	.4byte 0x03001238
	.4byte 0xfffff000
	.4byte 0xffffe000
	.4byte 0xffffd000
	.2byte 0x0240
	.2byte 0x0200
.L_08028920:
	add	r3, sp, #84
	ldr	r2, [r3, #4]
	ldr	r1, [r3, #0]
	adds	r0, r7, #0
	ldr	r3, [r3, #8]
	bl	sub_08024738
	ldr	r1, [r7, #36]
	ldr	r6, [pc, #232]
	adds	r0, r1, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x6af9
	adds	r5, r0, #0
	adds	r0, r1, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x182d
	adds	r0, r5, #0
	bl	sub_080149e0
	ldr	r2, [sp, #12]
	str	r2, [r7, #36]
	str	r2, [r7, #44]
	ldr	r4, [sp, #8]
	adds	r2, r7, #0
	lsls	r3, r4, #16
	lsrs	r5, r3, #16
	adds	r2, #36
	adds	r1, r5, #0
	bl	sub_0801489c
	movs	r0, #100
	adds	r0, r0, r7
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	mov	r8, r0
	ldrh	r2, [r0, #0]
	cmp	r3, #0
	beq.n	.L_08028976
	subs	r3, r2, #1
	mov	r2, r8
	strh	r3, [r2, #0]
.L_08028976:
	adds	r1, r7, #0
	adds	r1, #90
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldrh	r2, [r7, #6]
	subs	r3, r5, r2
	lsls	r3, r3, #16
	asrs	r1, r3, #16
	movs	r3, #128
	lsls	r3, r3, #5
	cmp	r1, r3
	ble.n	.L_08028996
	movs	r1, #128
	lsls	r1, r1, #3
.L_08028996:
	ldr	r4, [pc, #136]
	cmp	r1, r4
	bge.n	.L_0802899e
	ldr	r1, [pc, #132]
.L_0802899e:
	adds	r3, r2, r1
	strh	r3, [r7, #6]
.L_080289a2:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #32]
	ldr	r3, [pc, #124]
	ldr	r1, [pc, #128]
	ldr	r3, [r3, #0]
	movs	r2, #15
	lsrs	r3, r3, #4
	ands	r3, r2
	movs	r2, #143
	lsls	r3, r3, #2
	lsls	r2, r2, #1
	ldr	r4, [r1, r3]
	adds	r1, r0, r2
	ldrh	r0, [r1, #0]
	subs	r3, r4, r0
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, #0
	bge.n	.L_080289cc
	adds	r3, #7
.L_080289cc:
	asrs	r2, r3, #3
	movs	r3, #128
	lsls	r3, r3, #2
	cmp	r2, r3
	ble.n	.L_080289d8
	adds	r2, r3, #0
.L_080289d8:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bge.n	.L_080289e0
	adds	r2, r3, #0
.L_080289e0:
	adds	r3, r2, #0
	adds	r3, #15
	cmp	r3, #30
	bhi.n	.L_080289ec
	ldrh	r3, [r1, #0]
	subs	r2, r4, r3
.L_080289ec:
	adds	r3, r0, r2
	strh	r3, [r1, #0]
	adds	r3, r7, #0
	adds	r3, #84
	ldr	r4, [pc, #32]
	ldrb	r3, [r3, #0]
	mov	r9, r4
	mov	sl, r3
	cmp	r3, #1
	bne.n	.L_08028a8c
	ldr	r0, [sp, #0]
	bl	sub_0802dac0
	mov	r1, r8
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	cmp	r3, #0
	bne.n	.L_08028a8c
	ldr	r2, [sp, #12]
	cmp	r2, #0
	bne.n	.L_08028a8c
	b.n	.L_08028a34
	.4byte 0x00000000
	.4byte 0x0300021c
	.4byte 0xfffff000
	.4byte 0xfffffc00
	.4byte 0x03001150
	.4byte 0x0802eca0
	.2byte 0xfe00
	.2byte 0xffff
.L_08028a34:
	ldr	r3, [sp, #0]
	movs	r0, #14
	ldr	r1, [r3, #0]
	adds	r0, #255
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	bl	sub_08023220
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_08028a8c
	ldr	r1, [pc, #92]
	ldr	r6, [r5, #80]
	bl	sub_080233a8
	adds	r3, r5, #0
	adds	r3, #85
	mov	r4, r9
	strb	r4, [r3, #0]
	mov	r0, sl
	subs	r3, #51
	strb	r0, [r3, #0]
	cmp	r6, #0
	beq.n	.L_08028a86
	movs	r1, #2
	adds	r0, r6, #0
	bl	sub_08022b04
	mov	r1, r9
	strb	r1, [r6, #26]
	movs	r2, #13
	ldrb	r1, [r6, #5]
	negs	r2, r2
	adds	r3, r2, #0
	ands	r3, r1
	strb	r3, [r6, #5]
	ldrb	r3, [r6, #9]
	ands	r2, r3
	movs	r3, #8
	orrs	r2, r3
	strb	r2, [r6, #9]
.L_08028a86:
	movs	r3, #10
	mov	r2, r8
	strh	r3, [r2, #0]
.L_08028a8c:
	bl	sub_08026e60
	ldrh	r3, [r7, #4]
	movs	r0, #1
	adds	r3, #1
	strh	r3, [r7, #4]
	add	sp, #96
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0xec94
	.2byte 0x0802
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	movs	r1, #8
	adds	r1, r1, r7
	mov	r8, r1
	sub	sp, #96
	movs	r0, #0
	str	r0, [sp, #4]
	mov	r0, r8
	bl	sub_0802dac0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	subs	r3, r0, #1
	cmp	r3, #2
	bhi.n	.L_08028ae8
	movs	r2, #208
	add	r4, sp, #4
	lsls	r2, r2, #4
	ldrb	r4, [r4, #0]
	adds	r2, #58
	adds	r3, r1, r2
	strb	r4, [r3, #0]
.L_08028ae8:
	subs	r3, r0, #5
	cmp	r3, #1
	bhi.n	.L_08028afa
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #58
	adds	r2, r1, r3
	movs	r3, #1
	strb	r3, [r2, #0]
.L_08028afa:
	cmp	r0, #4
	bne.n	.L_08028b0a
	movs	r4, #208
	lsls	r4, r4, #4
	adds	r4, #58
	adds	r2, r1, r4
	movs	r3, #2
	strb	r3, [r2, #0]
.L_08028b0a:
	subs	r3, r0, #7
	cmp	r3, #5
	bhi.n	.L_08028b1c
	movs	r0, #208
	lsls	r0, r0, #4
	adds	r0, #58
	adds	r2, r1, r0
	movs	r3, #3
	strb	r3, [r2, #0]
.L_08028b1c:
	add	r1, sp, #4
	ldrb	r1, [r1, #0]
	adds	r3, r7, #0
	adds	r3, #85
	movs	r2, #0
	strb	r1, [r3, #0]
	str	r2, [sp, #12]
	ldr	r0, [r7, #12]
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L_08028b34
	adds	r3, #15
.L_08028b34:
	movs	r4, #160
	lsls	r4, r4, #9
	asrs	r3, r3, #4
	adds	r3, r3, r4
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #52]
	ldr	r5, [pc, #840]
	ldr	r1, [pc, #844]
	ldr	r3, [r5, #0]
	movs	r2, #15
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r2, [r1, r3]
	movs	r1, #255
	str	r2, [sp, #8]
	lsls	r1, r1, #8
	lsls	r2, r2, #16
	lsrs	r6, r2, #16
	adds	r1, #255
	mov	fp, r2
	cmp	r6, r1
	bne.n	.L_08028b74
	movs	r2, #4
	str	r2, [sp, #12]
	b.n	.L_08028dd4
.L_08028b6c:
	mov	r3, fp
	asrs	r3, r3, #16
	str	r3, [sp, #8]
	b.n	.L_08028d88
.L_08028b74:
	mov	r4, sp
	adds	r4, #84
	str	r4, [sp, #0]
	mov	r1, r8
	ldr	r3, [r1, #0]
	str	r0, [r4, #4]
	str	r3, [r4, #0]
	ldr	r3, [r7, #16]
	movs	r2, #128
	lsls	r2, r2, #12
	str	r3, [r4, #8]
	mov	sl, r2
	mov	r0, sl
	adds	r1, r6, #0
	ldr	r2, [sp, #0]
	bl	sub_0801489c
	ldr	r3, [pc, #768]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08028bac
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08028bac
	b.n	.L_08028dd4
.L_08028bac:
	adds	r0, r7, #0
	ldr	r1, [sp, #0]
	bl	sub_0802dbd0
	cmp	r0, #0
	bne.n	.L_08028c58
	mov	r4, r8
	ldr	r3, [r4, #0]
	add	r5, sp, #72
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	movs	r0, #128
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	lsls	r0, r0, #5
	adds	r1, r6, r0
	str	r3, [r5, #8]
	mov	r0, sl
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802dbd0
	cmp	r0, #0
	bne.n	.L_08028c58
	mov	r1, r8
	ldr	r3, [r1, #0]
	ldr	r2, [pc, #692]
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	adds	r1, r6, r2
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	mov	r0, sl
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802dbd0
	cmp	r0, #0
	bne.n	.L_08028c58
	mov	r4, r8
	ldr	r3, [r4, #0]
	movs	r0, #128
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	lsls	r0, r0, #6
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	adds	r1, r6, r0
	str	r3, [r5, #8]
	mov	r0, sl
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802dbd0
	cmp	r0, #0
	bne.n	.L_08028c58
	mov	r1, r8
	ldr	r3, [r1, #0]
	ldr	r2, [pc, #616]
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	adds	r1, r6, r2
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	mov	r0, sl
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802dbd0
	cmp	r0, #0
	bne.n	.L_08028c58
	b.n	.L_08028d88
.L_08028c58:
	add	r3, sp, #16
	mov	r4, fp
	mov	r9, r3
	movs	r0, #128
	lsrs	r3, r4, #16
	ldr	r4, [pc, #568]
	lsls	r0, r0, #5
	adds	r2, r3, r0
	mov	r1, r9
	strh	r2, [r1, #0]
	mov	r0, r9
	adds	r2, r3, r4
	strh	r2, [r0, #2]
	movs	r1, #128
	ldr	r0, [pc, #552]
	lsls	r1, r1, #6
	adds	r2, r3, r1
	mov	r4, r9
	strh	r2, [r4, #4]
	mov	r1, r9
	adds	r2, r3, r0
	strh	r2, [r1, #6]
	movs	r4, #192
	ldr	r1, [pc, #540]
	lsls	r4, r4, #6
	adds	r2, r3, r4
	mov	r0, r9
	strh	r2, [r0, #8]
	adds	r3, r3, r1
	mov	r2, r9
	strh	r3, [r2, #10]
	ldr	r0, [sp, #0]
	movs	r3, #6
	str	r3, [sp, #4]
	movs	r4, #0
	mov	sl, r4
	mov	r8, r0
.L_08028ca2:
	mov	r1, sl
	lsls	r3, r1, #1
	mov	r4, r9
	ldrsh	r2, [r4, r3]
	ldr	r3, [r7, #8]
	mov	r1, r8
	str	r3, [r1, #0]
	ldr	r3, [r7, #12]
	lsls	r2, r2, #16
	str	r3, [r1, #4]
	ldr	r3, [r7, #16]
	lsrs	r6, r2, #16
	movs	r0, #128
	str	r3, [r1, #8]
	lsls	r0, r0, #12
	adds	r1, r6, #0
	mov	fp, r2
	mov	r2, r8
	bl	sub_0801489c
	adds	r0, r7, #0
	mov	r1, r8
	bl	sub_0802dbd0
	cmp	r0, #0
	bne.n	.L_08028d76
	ldr	r3, [r7, #8]
	add	r5, sp, #72
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	movs	r2, #128
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	lsls	r2, r2, #5
	movs	r0, #128
	adds	r1, r6, r2
	lsls	r0, r0, #12
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802dbd0
	cmp	r0, #0
	bne.n	.L_08028d76
	ldr	r3, [r7, #8]
	movs	r0, #128
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	lsls	r0, r0, #12
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	adds	r2, r5, #0
	str	r3, [r5, #8]
	ldr	r3, [pc, #392]
	adds	r1, r6, r3
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802dbd0
	cmp	r0, #0
	bne.n	.L_08028d76
	ldr	r3, [r7, #8]
	movs	r4, #128
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	lsls	r4, r4, #6
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	movs	r0, #128
	adds	r1, r6, r4
	lsls	r0, r0, #12
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802dbd0
	cmp	r0, #0
	bne.n	.L_08028d76
	ldr	r3, [r7, #8]
	ldr	r0, [pc, #332]
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	adds	r1, r6, r0
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	movs	r0, #128
	lsls	r0, r0, #12
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802dbd0
	cmp	r0, #0
	bne.n	.L_08028d76
	b.n	.L_08028b6c
.L_08028d76:
	ldr	r2, [sp, #4]
	movs	r1, #1
	add	sl, r1
	cmp	sl, r2
	blt.n	.L_08028ca2
	ldr	r4, [sp, #12]
	movs	r3, #1
	orrs	r4, r3
	str	r4, [sp, #12]
.L_08028d88:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #20]
	movs	r0, #63
	adds	r6, r5, #0
	mov	sl, r0
	adds	r6, #89
.L_08028d96:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_08028dc4
	ldrb	r2, [r6, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08028dc4
	cmp	r5, r7
	beq.n	.L_08028dc4
	adds	r0, r5, #0
	adds	r0, #8
	ldrh	r1, [r5, #32]
	ldrh	r3, [r7, #32]
	ldr	r2, [sp, #0]
	bl	sub_08026f80
	cmp	r0, #0
	blt.n	.L_08028dc4
	ldr	r1, [sp, #12]
	movs	r3, #2
	orrs	r1, r3
	str	r1, [sp, #12]
.L_08028dc4:
	movs	r2, #1
	negs	r2, r2
	add	sl, r2
	mov	r3, sl
	adds	r6, #128
	adds	r5, #128
	cmp	r3, #0
	bge.n	.L_08028d96
.L_08028dd4:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	cmp	r1, #0
	beq.n	.L_08028e0a
	ldr	r4, [sp, #12]
	movs	r2, #3
	ands	r2, r4
	cmp	r2, #0
	beq.n	.L_08028df6
	movs	r0, #194
	lsls	r0, r0, #1
	adds	r2, r1, r0
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_08028dfe
.L_08028df6:
	movs	r4, #194
	lsls	r4, r4, #1
	adds	r3, r1, r4
	strh	r2, [r3, #0]
.L_08028dfe:
	ldr	r3, [pc, #144]
	movs	r0, #195
	ldr	r3, [r3, #0]
	lsls	r0, r0, #1
	adds	r2, r1, r0
	strh	r3, [r2, #0]
.L_08028e0a:
	ldr	r3, [r7, #12]
	movs	r1, #128
	lsls	r1, r1, #11
	cmp	r3, r1
	blt.n	.L_08028e1e
	adds	r0, r7, #0
	movs	r1, #6
	bl	sub_080233d0
	b.n	.L_08028e36
.L_08028e1e:
	ldr	r2, [sp, #12]
	cmp	r2, #0
	beq.n	.L_08028e2e
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_080233d0
	b.n	.L_08028e36
.L_08028e2e:
	adds	r0, r7, #0
	movs	r1, #2
	bl	sub_080233d0
.L_08028e36:
	ldr	r3, [sp, #12]
	cmp	r3, #0
	beq.n	.L_08028ea8
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r7, #56]
	str	r3, [r7, #60]
	str	r3, [r7, #64]
	movs	r3, #0
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r4, [sp, #12]
	movs	r3, #3
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_08028e7a
	ldr	r0, [sp, #8]
	ldrh	r2, [r7, #6]
	lsls	r3, r0, #16
	lsrs	r3, r3, #16
	subs	r3, r3, r2
	lsls	r3, r3, #16
	asrs	r1, r3, #16
	movs	r3, #128
	lsls	r3, r3, #5
	cmp	r1, r3
	ble.n	.L_08028e6e
	adds	r1, r3, #0
.L_08028e6e:
	ldr	r3, [pc, #44]
	cmp	r1, r3
	bge.n	.L_08028e76
	adds	r1, r3, #0
.L_08028e76:
	adds	r3, r2, r1
	strh	r3, [r7, #6]
.L_08028e7a:
	movs	r1, #100
	adds	r1, r1, r7
	mov	sl, r1
	movs	r3, #0
	mov	r2, sl
	strh	r3, [r2, #0]
	adds	r2, r7, #0
	adds	r2, #102
	movs	r3, #2
	strh	r3, [r2, #0]
	b.n	.L_08028f2a
	.4byte 0x03001150
	.4byte 0x0802ec5c
	.4byte 0x03001238
	.4byte 0xfffff000
	.4byte 0xffffe000
	.2byte 0xd000
	.2byte 0xffff
.L_08028ea8:
	.2byte 0xab15
	ldr	r2, [r3, #4]
	ldr	r1, [r3, #0]
	adds	r0, r7, #0
	ldr	r3, [r3, #8]
	bl	sub_08024738
	ldr	r1, [r7, #36]
	ldr	r6, [pc, #256]
	adds	r0, r1, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x6af9
	adds	r5, r0, #0
	adds	r0, r1, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x182d
	adds	r0, r5, #0
	bl	sub_080149e0
	ldr	r3, [sp, #12]
	adds	r2, r7, #0
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r4, [sp, #8]
	adds	r2, #36
	lsls	r3, r4, #16
	lsrs	r5, r3, #16
	adds	r1, r5, #0
	bl	sub_0801489c
	movs	r0, #100
	adds	r0, r0, r7
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	mov	sl, r0
	ldrh	r2, [r0, #0]
	cmp	r3, #0
	beq.n	.L_08028efe
	subs	r3, r2, #1
	mov	r2, sl
	strh	r3, [r2, #0]
.L_08028efe:
	adds	r1, r7, #0
	adds	r1, #90
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldrh	r2, [r7, #6]
	subs	r3, r5, r2
	lsls	r3, r3, #16
	asrs	r1, r3, #16
	movs	r3, #128
	lsls	r3, r3, #5
	cmp	r1, r3
	ble.n	.L_08028f1e
	movs	r1, #128
	lsls	r1, r1, #3
.L_08028f1e:
	ldr	r4, [pc, #160]
	cmp	r1, r4
	bge.n	.L_08028f26
	ldr	r1, [pc, #156]
.L_08028f26:
	adds	r3, r2, r1
	strh	r3, [r7, #6]
.L_08028f2a:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #32]
	ldr	r3, [pc, #148]
	ldr	r1, [pc, #152]
	ldr	r3, [r3, #0]
	movs	r2, #15
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #2
	ldr	r0, [r1, r3]
	ldr	r1, [r7, #12]
	cmp	r1, #0
	bge.n	.L_08028f48
	adds	r1, #3
.L_08028f48:
	movs	r2, #128
	lsls	r2, r2, #9
	asrs	r1, r1, #2
	adds	r1, r1, r2
	ldr	r3, [pc, #104]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x238f
	lsls	r3, r3, #1
	adds	r1, r5, r3
	ldrh	r4, [r1, #0]
	subs	r3, r0, r4
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, #0
	bge.n	.L_08028f6a
	adds	r3, #7
.L_08028f6a:
	asrs	r2, r3, #3
	cmp	r2, #32
	ble.n	.L_08028f72
	movs	r2, #32
.L_08028f72:
	movs	r3, #32
	negs	r3, r3
	cmp	r2, r3
	bge.n	.L_08028f7c
	adds	r2, r3, #0
.L_08028f7c:
	adds	r3, r2, #1
	cmp	r3, #2
	bhi.n	.L_08028f86
	ldrh	r3, [r1, #0]
	subs	r2, r0, r3
.L_08028f86:
	adds	r3, r4, r2
	strh	r3, [r1, #0]
	adds	r3, r7, #0
	adds	r3, #84
	ldr	r4, [pc, #40]
	ldrb	r3, [r3, #0]
	mov	r9, r4
	mov	r8, r3
	cmp	r3, #1
	bne.n	.L_0802903a
	ldr	r3, [r7, #12]
	movs	r0, #128
	lsls	r0, r0, #11
	cmp	r3, r0
	bge.n	.L_0802902c
	mov	r2, sl
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	bne.n	.L_0802903a
	ldr	r3, [sp, #12]
	cmp	r3, #0
	bne.n	.L_0802903a
	b.n	.L_08028fd0
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x0300021c
	.4byte 0xfffff000
	.4byte 0xfffffc00
	.4byte 0x03001150
	.2byte 0xeca0
	.2byte 0x0802
.L_08028fd0:
	movs	r0, #14
	adds	r0, #255
	ldr	r1, [r7, #8]
	ldr	r3, [r7, #16]
	movs	r2, #0
	bl	sub_08023220
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0802903a
	ldr	r1, [pc, #248]
	ldr	r6, [r5, #80]
	bl	sub_080233a8
	adds	r3, r5, #0
	adds	r3, #85
	mov	r4, r9
	strb	r4, [r3, #0]
	mov	r0, r8
	subs	r3, #51
	strb	r0, [r3, #0]
	cmp	r6, #0
	beq.n	.L_08029024
	movs	r1, #1
	adds	r0, r6, #0
	bl	sub_08022b04
	mov	r1, r9
	strb	r1, [r6, #26]
	movs	r2, #13
	ldrb	r1, [r6, #5]
	negs	r2, r2
	adds	r3, r2, #0
	ands	r3, r1
	movs	r1, #4
	orrs	r3, r1
	strb	r3, [r6, #5]
	ldrb	r3, [r6, #9]
	ands	r2, r3
	movs	r3, #8
	orrs	r2, r3
	strb	r2, [r6, #9]
.L_08029024:
	movs	r3, #10
	mov	r2, sl
	strh	r3, [r2, #0]
	b.n	.L_0802903a
.L_0802902c:
	ldr	r3, [pc, #180]
	adds	r0, r7, #0
	ldr	r1, [r3, #0]
	mov	r3, r8
	ands	r1, r3
	bl	sub_08023664
.L_0802903a:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	movs	r1, #128
	ldr	r3, [pc, #164]
	movs	r4, #140
	lsls	r1, r1, #2
	adds	r1, #50
	ldr	r0, [pc, #160]
	lsls	r4, r4, #2
	adds	r2, r3, r4
	adds	r3, r3, r1
	ldrh	r1, [r2, #0]
	ldrh	r2, [r3, #0]
	ldr	r3, [r0, #0]
	orrs	r2, r1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08029076
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #59
	adds	r2, r5, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r3, [r7, #12]
	movs	r4, #128
	lsls	r4, r4, #6
	adds	r3, r3, r4
	b.n	.L_0802907c
.L_08029076:
	ldr	r3, [r7, #12]
	ldr	r0, [pc, #116]
	adds	r3, r3, r0
.L_0802907c:
	str	r3, [r7, #12]
	bl	sub_080ad318
	cmp	r0, #0
	bne.n	.L_0802908e
	ldr	r3, [r7, #12]
	ldr	r1, [pc, #100]
	adds	r3, r3, r1
	str	r3, [r7, #12]
.L_0802908e:
	ldr	r2, [r7, #12]
	cmp	r2, #0
	bgt.n	.L_080290b2
	movs	r4, #208
	lsls	r4, r4, #4
	adds	r4, #59
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080290b2
	movs	r0, #181
	lsls	r0, r0, #1
	adds	r2, r5, r0
	movs	r3, #254
	strh	r3, [r2, #0]
	ldr	r2, [r7, #12]
.L_080290b2:
	movs	r1, #192
	lsls	r1, r1, #12
	cmp	r2, r1
	blt.n	.L_080290c2
	movs	r3, #192
	lsls	r3, r3, #12
	str	r3, [r7, #12]
	adds	r2, r3, #0
.L_080290c2:
	cmp	r2, #0
	bgt.n	.L_080290ca
	movs	r3, #0
	str	r3, [r7, #12]
.L_080290ca:
	ldrh	r3, [r7, #4]
	movs	r0, #1
	adds	r3, #1
	strh	r3, [r7, #4]
	add	sp, #96
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0802ec94
	.4byte 0x0300122c
	.4byte 0x02000240
	.4byte 0x03001150
	.2byte 0xc000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #128
	adds	r7, r0, #0
	lsls	r3, r3, #7
	str	r3, [r7, #52]
	ldr	r3, [pc, #472]
	movs	r4, #128
	ldrb	r3, [r3, #0]
	movs	r0, #12
	lsls	r4, r4, #8
	sub	sp, #12
	mov	fp, r0
	str	r4, [r7, #48]
	cmp	r3, #0
	beq.n	.L_08029124
	ldr	r2, [pc, #456]
	ldr	r3, [r2, #0]
	b.n	.L_08029126
.L_08029124:
	ldr	r2, [pc, #448]
.L_08029126:
	ldr	r3, [r2, #0]
	ldr	r1, [pc, #448]
	movs	r2, #15
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r3, [r1, r3]
	movs	r2, #255
	lsls	r1, r3, #16
	lsls	r2, r2, #8
	movs	r3, #4
	lsrs	r0, r1, #16
	adds	r2, #255
	mov	r9, r3
	cmp	r0, r2
	bne.n	.L_08029148
	b.n	.L_08029268
.L_08029148:
	movs	r2, #240
	lsls	r2, r2, #8
	movs	r3, #14
	ands	r2, r0
	mov	fp, r3
	cmp	r2, #0
	beq.n	.L_08029162
	movs	r0, #15
	mov	fp, r0
	cmp	r2, r4
	beq.n	.L_08029162
	movs	r2, #10
	mov	fp, r2
.L_08029162:
	mov	r6, sp
	movs	r0, #128
	movs	r3, #0
	adds	r2, r6, #0
	lsls	r0, r0, #12
	lsrs	r1, r1, #16
	mov	r9, r3
	str	r3, [r6, #0]
	str	r3, [r6, #4]
	str	r3, [r6, #8]
	bl	sub_0801489c
	ldr	r2, [r7, #8]
	ldr	r3, [r6, #0]
	adds	r3, r3, r2
	ldr	r2, [r6, #8]
	str	r3, [r6, #0]
	cmp	r2, #0
	bge.n	.L_0802918e
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r7, #6]
.L_0802918e:
	cmp	r2, #0
	ble.n	.L_08029198
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r7, #6]
.L_08029198:
	ldr	r3, [r7, #12]
	ldr	r2, [r6, #8]
	adds	r0, r7, #0
	subs	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r3, [r7, #16]
	str	r3, [r6, #8]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #32]
	bl	sub_080c8710
	movs	r2, #34
	adds	r2, r2, r7
	mov	r8, r2
	ldrb	r2, [r2, #0]
	lsls	r0, r0, #2
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #156
	lsls	r3, r3, #3
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r1, [r5, r3]
	ldr	r3, [r6, #0]
	adds	r0, r0, r1
	mov	sl, r0
	cmp	r3, #0
	bge.n	.L_080291d6
	ldr	r0, [pc, #284]
	adds	r3, r3, r0
.L_080291d6:
	asrs	r2, r3, #20
	ldr	r3, [r6, #8]
	cmp	r3, #0
	bge.n	.L_080291e2
	ldr	r0, [pc, #272]
	adds	r3, r3, r0
.L_080291e2:
	asrs	r3, r3, #20
	lsls	r3, r3, #7
	adds	r3, r2, r3
	lsls	r3, r3, #2
	adds	r0, r6, #0
	adds	r5, r1, r3
	bl	sub_0802da24
	cmp	r0, #0
	bne.n	.L_0802920e
	mov	r3, sl
	ldrb	r2, [r3, #3]
	movs	r1, #64
	adds	r3, r1, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0802920e
	ldrb	r2, [r5, #3]
	adds	r3, r1, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08029218
.L_0802920e:
	movs	r0, #4
	movs	r2, #12
	mov	r9, r0
	mov	fp, r2
	b.n	.L_08029268
.L_08029218:
	ldr	r2, [pc, #204]
	ldr	r3, [r2, #0]
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08029240
	mov	r3, r8
	ldrb	r0, [r3, #0]
	ldr	r2, [r6, #8]
	ldr	r3, [pc, #200]
	ldr	r1, [r6, #0]
	adds	r2, r2, r3
	bl	sub_0802d45c
	ldr	r3, [r7, #12]
	subs	r3, r0, r3
	movs	r0, #128
	lsls	r0, r0, #13
	cmp	r3, r0
	bge.n	.L_08029268
	b.n	.L_08029260
.L_08029240:
	ldr	r3, [r2, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08029268
	mov	r2, r8
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #8]
	bl	sub_0802d45c
	ldr	r3, [r7, #12]
	subs	r3, r0, r3
	ldr	r0, [pc, #156]
	cmp	r3, r0
	ble.n	.L_08029268
.L_08029260:
	movs	r2, #1
	movs	r3, #12
	mov	r9, r2
	mov	fp, r3
.L_08029268:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	cmp	r1, #0
	beq.n	.L_0802929e
	movs	r2, #3
	mov	r0, r9
	ands	r2, r0
	cmp	r2, #0
	beq.n	.L_0802928a
	movs	r3, #194
	lsls	r3, r3, #1
	adds	r2, r1, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_08029292
.L_0802928a:
	movs	r0, #194
	lsls	r0, r0, #1
	adds	r3, r1, r0
	strh	r2, [r3, #0]
.L_08029292:
	ldr	r3, [pc, #84]
	movs	r0, #195
	ldr	r3, [r3, #0]
	lsls	r0, r0, #1
	adds	r2, r1, r0
	strh	r3, [r2, #0]
.L_0802929e:
	adds	r0, r7, #0
	mov	r1, fp
	bl	sub_080233d0
	mov	r2, r9
	cmp	r2, #0
	beq.n	.L_080292c0
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r7, #56]
	str	r3, [r7, #60]
	str	r3, [r7, #64]
	movs	r3, #0
	str	r3, [r7, #36]
	str	r3, [r7, #40]
	str	r3, [r7, #44]
	b.n	.L_080292ce
.L_080292c0:
	mov	r3, sp
	ldr	r1, [r3, #0]
	ldr	r2, [r3, #4]
	adds	r0, r7, #0
	ldr	r3, [r3, #8]
	bl	sub_08024738
.L_080292ce:
	ldrh	r3, [r7, #4]
	movs	r0, #1
	adds	r3, #1
	strh	r3, [r7, #4]
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x03001238
	.4byte 0x03001150
	.4byte 0x0802ec5c
	.4byte 0x000fffff
	.4byte 0xfff00000
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	sl, r0
	ldr	r0, [r0, #80]
	sub	sp, #36
	str	r0, [sp, #16]
	movs	r6, #192
	lsls	r6, r6, #18
	adds	r6, #156
	ldr	r1, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #9
	movs	r3, #0
	str	r1, [sp, #12]
	str	r2, [sp, #0]
	str	r3, [sp, #20]
	add	r1, sp, #20
	movs	r3, #128
	lsls	r3, r3, #8
	mov	r0, sl
	ldrb	r1, [r1, #0]
	str	r3, [r0, #48]
	str	r3, [r0, #52]
	mov	r3, sl
	adds	r3, #85
	strb	r1, [r3, #0]
	ldr	r3, [pc, #552]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #548]
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	movs	r2, #255
	ldrh	r1, [r1, r3]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r1, r2
	bne.n	.L_0802935e
	ldr	r3, [r0, #104]
	movs	r0, #0
	mov	r9, r3
	str	r0, [sp, #4]
	b.n	.L_0802940e
.L_0802935e:
	ldr	r3, [sp, #20]
	add	r5, sp, #24
	movs	r2, #16
	movs	r0, #128
	str	r2, [sp, #4]
	lsls	r0, r0, #12
	str	r3, [r5, #0]
	str	r3, [r5, #4]
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	mov	r0, sl
	ldr	r2, [r0, #8]
	ldr	r3, [r5, #0]
	ldr	r6, [r6, #0]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #8]
	ldr	r3, [r0, #12]
	movs	r1, #134
	subs	r3, r3, r2
	str	r3, [r5, #4]
	lsls	r1, r1, #1
	ldr	r3, [r0, #16]
	movs	r2, #0
	str	r3, [r5, #8]
	movs	r3, #128
	lsls	r3, r3, #21
	str	r6, [sp, #12]
	adds	r7, r6, r1
	mov	r9, r2
	str	r3, [sp, #8]
	cmp	r6, #0
	beq.n	.L_0802949e
	movs	r1, #127
	mov	fp, r1
.L_080293a8:
	movs	r3, #18
	ldrsb	r3, [r7, r3]
	cmp	r3, #1
	bne.n	.L_08029400
	ldr	r1, [sp, #24]
	ldr	r3, [r7, #4]
	ldr	r5, [sp, #28]
	subs	r1, r1, r3
	ldr	r3, [r7, #8]
	ldr	r6, [sp, #32]
	subs	r5, r5, r3
	ldr	r3, [r7, #12]
	asrs	r1, r1, #8
	ldr	r2, [pc, #424]
	adds	r0, r1, #0
	subs	r6, r6, r3
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x122d
	adds	r1, r5, #0
	ldr	r3, [pc, #408]
	mov	r8, r0
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1236
	adds	r5, r0, #0
	adds	r1, r6, #0
	ldr	r2, [pc, #392]
	adds	r0, r6, #0
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x44a8
	mov	r1, r8
	movs	r2, #128
	adds	r3, r1, r0
	lsls	r2, r2, #3
	cmp	r3, r2
	bgt.n	.L_08029400
	ldr	r0, [sp, #8]
	cmp	r0, r3
	ble.n	.L_08029400
	str	r3, [sp, #8]
	mov	r9, r7
.L_08029400:
	movs	r1, #1
	negs	r1, r1
	add	fp, r1
	mov	r2, fp
	adds	r7, #32
	cmp	r2, #0
	bge.n	.L_080293a8
.L_0802940e:
	mov	r3, r9
	cmp	r3, #0
	bne.n	.L_0802941e
	ldr	r0, [sp, #20]
	movs	r3, #1
	orrs	r0, r3
	str	r0, [sp, #20]
	b.n	.L_080294da
.L_0802941e:
	mov	r1, r9
	movs	r3, #19
	ldrsb	r3, [r1, r3]
	ldr	r2, [sp, #12]
	lsls	r3, r3, #5
	adds	r3, r2, r3
	adds	r6, r3, #0
	subs	r6, #20
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	bl	sub_080c8088
	adds	r5, r0, #0
	movs	r1, #2
	ldrsh	r0, [r6, r1]
	bl	sub_080c8088
	ldr	r2, [r5, #8]
	ldr	r3, [r0, #8]
	cmp	r2, r3
	ble.n	.L_0802944e
	ldr	r2, [sp, #0]
	negs	r2, r2
	str	r2, [sp, #0]
.L_0802944e:
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	bl	sub_080c8088
	adds	r5, r0, #0
	movs	r1, #2
	ldrsh	r0, [r6, r1]
	bl	sub_080c8088
	ldr	r2, [r5, #12]
	ldr	r3, [r0, #12]
	cmp	r2, r3
	ble.n	.L_08029474
	ldr	r2, [sp, #0]
	movs	r1, #192
	negs	r2, r2
	str	r2, [sp, #0]
	lsls	r1, r1, #8
	b.n	.L_08029478
.L_08029474:
	movs	r1, #128
	lsls	r1, r1, #7
.L_08029478:
	ldr	r3, [sp, #0]
	mov	r0, sl
	str	r3, [r0, #24]
	mov	r3, r9
	ldr	r2, [r3, #4]
	ldr	r3, [r0, #8]
	cmp	r2, r3
	bne.n	.L_080294a4
	mov	r0, r9
	ldr	r2, [r0, #8]
	mov	r0, sl
	ldr	r3, [r0, #12]
	cmp	r2, r3
	bne.n	.L_080294a4
	mov	r3, r9
	ldr	r2, [r3, #12]
	ldr	r3, [r0, #16]
	cmp	r2, r3
	bne.n	.L_080294a4
.L_0802949e:
	movs	r0, #1
	str	r0, [sp, #20]
	b.n	.L_080294da
.L_080294a4:
	movs	r3, #1
	str	r3, [r6, #24]
	ldr	r3, [r6, #12]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #10
	str	r3, [r6, #12]
	cmp	r3, r2
	ble.n	.L_080294bc
	str	r2, [r6, #12]
.L_080294bc:
	mov	r3, r9
	mov	r0, sl
	str	r3, [r0, #104]
	ldr	r3, [r3, #4]
	add	r2, sp, #24
	str	r3, [r2, #0]
	mov	r0, r9
	ldr	r3, [r0, #8]
	str	r3, [r2, #4]
	ldr	r3, [r0, #12]
	str	r3, [r2, #8]
	ldrh	r3, [r0, #16]
	adds	r3, r3, r1
	ldr	r1, [sp, #16]
	strh	r3, [r1, #18]
.L_080294da:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	cmp	r1, #0
	beq.n	.L_08029510
	ldr	r3, [sp, #20]
	movs	r2, #3
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080294fc
	movs	r0, #194
	lsls	r0, r0, #1
	adds	r2, r1, r0
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_08029504
.L_080294fc:
	movs	r0, #194
	lsls	r0, r0, #1
	adds	r3, r1, r0
	strh	r2, [r3, #0]
.L_08029504:
	ldr	r3, [pc, #92]
	movs	r0, #195
	ldr	r3, [r3, #0]
	lsls	r0, r0, #1
	adds	r2, r1, r0
	strh	r3, [r2, #0]
.L_08029510:
	mov	r0, sl
	movs	r1, #29
	bl	sub_080233d0
	mov	r0, sl
	ldr	r1, [sp, #4]
	bl	sub_08023410
	ldr	r1, [sp, #20]
	cmp	r1, #0
	beq.n	.L_0802953c
	movs	r3, #128
	mov	r2, sl
	lsls	r3, r3, #24
	str	r3, [r2, #56]
	str	r3, [r2, #60]
	str	r3, [r2, #64]
	movs	r3, #0
	str	r3, [r2, #36]
	str	r3, [r2, #40]
	str	r3, [r2, #44]
	b.n	.L_0802954a
.L_0802953c:
	add	r3, sp, #24
	ldr	r1, [r3, #0]
	ldr	r2, [r3, #4]
	mov	r0, sl
	ldr	r3, [r3, #8]
	bl	sub_08024738
.L_0802954a:
	mov	r0, sl
	ldrh	r3, [r0, #4]
	mov	r1, sl
	adds	r3, #1
	movs	r0, #1
	strh	r3, [r1, #4]
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x03001150
	.4byte 0x0802ec5c
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r4, r0, #0
	ldr	r0, [r4, #80]
	movs	r3, #128
	sub	sp, #40
	movs	r2, #0
	lsls	r3, r3, #8
	movs	r1, #1
	str	r1, [sp, #16]
	str	r2, [sp, #24]
	str	r0, [sp, #20]
	str	r3, [r4, #48]
	str	r3, [r4, #52]
	adds	r3, r4, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	ldr	r3, [pc, #608]
	ldr	r1, [pc, #612]
	ldr	r3, [r3, #0]
	movs	r2, #15
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r3, [r1, r3]
	lsls	r3, r3, #16
	str	r3, [sp, #4]
	lsrs	r1, r3, #16
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	cmp	r1, r3
	bne.n	.L_080295d8
	movs	r0, #4
	str	r0, [sp, #24]
	movs	r2, #28
	ldr	r3, [r4, #8]
	add	r2, sp
	ldr	r1, [r4, #104]
	str	r3, [r2, #0]
	mov	sl, r1
	ldr	r3, [r1, #8]
	mov	r9, r2
	str	r3, [r2, #4]
	ldr	r3, [r4, #16]
	str	r3, [r2, #8]
	b.n	.L_080296fa
.L_080295d8:
	add	r3, sp, #28
	mov	r9, r3
	ldr	r3, [r4, #8]
	mov	r0, r9
	str	r3, [r0, #0]
	mov	r2, r9
	ldr	r3, [r4, #12]
	str	r4, [sp, #0]
	str	r3, [r0, #4]
	ldr	r3, [r4, #16]
	str	r3, [r0, #8]
	movs	r0, #128
	lsls	r0, r0, #12
	bl	sub_0801489c
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #160
	ldr	r3, [r3, #0]
	movs	r2, #128
	adds	r7, r3, #0
	movs	r1, #0
	lsls	r2, r2, #21
	str	r3, [sp, #12]
	adds	r7, #236
	mov	sl, r1
	str	r2, [sp, #8]
	ldr	r4, [sp, #0]
	cmp	r3, #0
	bne.n	.L_0802961a
	movs	r3, #1
	str	r3, [sp, #24]
	b.n	.L_080296fa
.L_0802961a:
	movs	r0, #127
	mov	fp, r0
.L_0802961e:
	movs	r3, #16
	ldrsb	r3, [r7, r3]
	cmp	r3, #1
	bne.n	.L_0802967a
	ldr	r1, [sp, #28]
	ldr	r3, [r7, #4]
	ldr	r5, [sp, #32]
	subs	r1, r1, r3
	ldr	r3, [r7, #8]
	ldr	r6, [sp, #36]
	subs	r5, r5, r3
	ldr	r3, [r7, #12]
	asrs	r1, r1, #8
	str	r4, [sp, #0]
	ldr	r2, [pc, #460]
	adds	r0, r1, #0
	subs	r6, r6, r3
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x122d
	adds	r1, r5, #0
	ldr	r3, [pc, #444]
	mov	r8, r0
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1236
	adds	r5, r0, #0
	adds	r1, r6, #0
	ldr	r2, [pc, #428]
	adds	r0, r6, #0
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x44a8
	mov	r1, r8
	movs	r2, #128
	adds	r3, r1, r0
	lsls	r2, r2, #3
	ldr	r4, [sp, #0]
	cmp	r3, r2
	bgt.n	.L_0802967a
	ldr	r0, [sp, #8]
	cmp	r0, r3
	ble.n	.L_0802967a
	str	r3, [sp, #8]
	mov	sl, r7
.L_0802967a:
	movs	r1, #1
	negs	r1, r1
	add	fp, r1
	mov	r2, fp
	adds	r7, #32
	cmp	r2, #0
	bge.n	.L_0802961e
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_08029698
	ldr	r0, [sp, #24]
	movs	r3, #1
	orrs	r0, r3
	str	r0, [sp, #24]
	b.n	.L_080296fa
.L_08029698:
	mov	r1, sl
	ldr	r2, [r1, #4]
	ldr	r3, [r4, #8]
	cmp	r2, r3
	bne.n	.L_080296b8
	ldr	r2, [r1, #8]
	ldr	r3, [r4, #12]
	cmp	r2, r3
	bne.n	.L_080296b8
	ldr	r2, [r1, #12]
	ldr	r3, [r4, #16]
	cmp	r2, r3
	bne.n	.L_080296b8
	movs	r2, #1
	str	r2, [sp, #24]
	b.n	.L_080296fa
.L_080296b8:
	mov	r3, sl
	movs	r2, #17
	ldrsb	r2, [r3, r2]
	cmp	r2, #0
	beq.n	.L_080296ec
	ldr	r0, [sp, #12]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r0, r3
	adds	r2, r3, #0
	subs	r2, #16
	movs	r3, #1
	str	r3, [r2, #20]
	ldr	r3, [r2, #8]
	movs	r1, #128
	lsls	r1, r1, #8
	adds	r3, r3, r1
	movs	r1, #128
	lsls	r1, r1, #10
	str	r3, [r2, #8]
	cmp	r3, r1
	ble.n	.L_080296e8
	str	r1, [r2, #8]
.L_080296e8:
	mov	r2, sl
	str	r2, [r4, #104]
.L_080296ec:
	mov	r0, sl
	ldr	r3, [r0, #4]
	str	r3, [sp, #28]
	ldr	r3, [r0, #8]
	str	r3, [sp, #32]
	ldr	r3, [r0, #12]
	str	r3, [sp, #36]
.L_080296fa:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	cmp	r1, #0
	beq.n	.L_08029730
	ldr	r3, [sp, #24]
	movs	r2, #3
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0802971c
	movs	r0, #194
	lsls	r0, r0, #1
	adds	r2, r1, r0
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_08029724
.L_0802971c:
	movs	r0, #194
	lsls	r0, r0, #1
	adds	r3, r1, r0
	strh	r2, [r3, #0]
.L_08029724:
	ldr	r3, [pc, #216]
	movs	r0, #195
	ldr	r3, [r3, #0]
	lsls	r0, r0, #1
	adds	r2, r1, r0
	strh	r3, [r2, #0]
.L_08029730:
	ldr	r1, [sp, #24]
	cmp	r1, #0
	bne.n	.L_0802973a
	movs	r2, #2
	str	r2, [sp, #16]
.L_0802973a:
	adds	r0, r4, #0
	ldr	r1, [sp, #16]
	str	r4, [sp, #0]
	bl	sub_080233d0
	ldr	r4, [sp, #0]
	movs	r1, #8
	adds	r0, r4, #0
	bl	sub_08023410
	ldr	r0, [sp, #20]
	movs	r3, #4
	strb	r3, [r0, #23]
	ldr	r1, [sp, #24]
	movs	r2, #0
	ldr	r4, [sp, #0]
	cmp	r1, #0
	beq.n	.L_080297a0
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r4, #56]
	str	r3, [r4, #60]
	str	r3, [r4, #64]
	movs	r3, #3
	ands	r3, r1
	str	r2, [r4, #36]
	str	r2, [r4, #40]
	str	r2, [r4, #44]
	cmp	r3, #0
	beq.n	.L_08029798
	ldr	r2, [sp, #4]
	ldrh	r1, [r4, #6]
	lsrs	r3, r2, #16
	subs	r3, r3, r1
	lsls	r3, r3, #16
	movs	r2, #128
	asrs	r3, r3, #16
	lsls	r2, r2, #5
	cmp	r3, r2
	ble.n	.L_0802978c
	adds	r3, r2, #0
.L_0802978c:
	ldr	r2, [pc, #124]
	cmp	r3, r2
	bge.n	.L_08029794
	adds	r3, r2, #0
.L_08029794:
	adds	r3, r1, r3
	strh	r3, [r4, #6]
.L_08029798:
	mov	r0, r9
	ldr	r3, [r0, #4]
	str	r3, [r4, #12]
	b.n	.L_080297ea
.L_080297a0:
	mov	r1, r9
	ldr	r2, [r1, #4]
	mov	r0, r9
	str	r2, [r4, #12]
	str	r4, [sp, #0]
	ldr	r3, [r0, #8]
	ldr	r1, [r1, #0]
	adds	r0, r4, #0
	bl	sub_08024738
	ldr	r4, [sp, #0]
	mov	r1, r9
	ldr	r2, [r4, #8]
	ldr	r3, [r1, #0]
	cmp	r2, r3
	bne.n	.L_080297ea
	ldr	r2, [r4, #16]
	ldr	r3, [r1, #8]
	cmp	r2, r3
	bne.n	.L_080297ea
	ldr	r2, [sp, #4]
	ldrh	r1, [r4, #6]
	lsrs	r3, r2, #16
	subs	r3, r3, r1
	lsls	r3, r3, #16
	movs	r2, #128
	asrs	r3, r3, #16
	lsls	r2, r2, #5
	cmp	r3, r2
	ble.n	.L_080297de
	adds	r3, r2, #0
.L_080297de:
	ldr	r2, [pc, #44]
	cmp	r3, r2
	bge.n	.L_080297e6
	adds	r3, r2, #0
.L_080297e6:
	adds	r3, r1, r3
	strh	r3, [r4, #6]
.L_080297ea:
	ldrh	r3, [r4, #4]
	movs	r0, #1
	adds	r3, #1
	strh	r3, [r4, #4]
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x03001150
	.4byte 0x0802ec5c
	.4byte 0x0300021c
	.2byte 0xf000
	.2byte 0xffff
	push	{r5, r6, lr}
	adds	r6, r0, #0
	ldr	r2, [r6, #80]
	movs	r1, #128
	ldrh	r3, [r2, #18]
	lsls	r1, r1, #3
	adds	r3, r3, r1
	strh	r3, [r2, #18]
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	ldr	r3, [r6, #8]
	lsrs	r0, r0, #1
	lsrs	r5, r5, #1
	subs	r5, r5, r0
	adds	r3, r3, r5
	str	r3, [r6, #8]
	pop	{r5, r6, pc}
.L_08029838:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	adds	r6, r0, #0
	movs	r0, #0
	cmp	r5, #0
	beq.n	.L_080298b6
	ldr	r1, [pc, #108]
	ldr	r4, [pc, #112]
	movs	r2, #141
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r4, #4]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0802987e
	movs	r2, #140
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r4, #4]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0802987e
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #54
	adds	r3, r1, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r4, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08029880
.L_0802987e:
	movs	r0, #1
.L_08029880:
	cmp	r0, #0
	beq.n	.L_080298b4
	adds	r0, r6, #0
	bl	sub_080c8828
	cmp	r0, #0
	beq.n	.L_08029896
	movs	r0, #113
	bl	sub_081c0010
	b.n	.L_080298b4
.L_08029896:
	movs	r0, #163
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080298b4
	movs	r3, #179
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #128
	lsls	r3, r3, #6
	adds	r3, #139
	strh	r3, [r2, #0]
	movs	r0, #1
	b.n	.L_080298b6
.L_080298b4:
	movs	r0, #0
.L_080298b6:
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #92
	adds	r7, r0, #0
	movs	r0, #0
	str	r0, [sp, #28]
	ldr	r3, [r7, #12]
	ldr	r1, [r7, #16]
	adds	r4, r7, #0
	adds	r4, #34
	ldr	r6, [r7, #8]
	ldr	r5, [pc, #872]
	str	r3, [sp, #20]
	str	r4, [sp, #12]
	mov	r8, r1
	ands	r6, r5
	mov	r2, r8
	ands	r2, r5
	ldrb	r0, [r4, #0]
	adds	r1, r6, #0
	mov	r8, r2
	bl	sub_0802d45c
	ldr	r1, [sp, #12]
	str	r0, [sp, #24]
	add	r5, r8
	ldrb	r0, [r1, #0]
	adds	r2, r5, #0
	adds	r1, r6, #0
	bl	sub_0802d45c
	adds	r2, r7, #0
	adds	r2, #100
	str	r0, [sp, #16]
	str	r2, [sp, #8]
	movs	r4, #0
	ldrsh	r3, [r2, r4]
	cmp	r3, #0
	beq.n	.L_08029930
	ldr	r3, [r7, #56]
	movs	r6, #128
	lsls	r6, r6, #24
	cmp	r3, r6
	beq.n	.L_08029924
	b.n	.L_0802a100
.L_08029924:
	add	r0, sp, #28
	ldrh	r0, [r0, #0]
	adds	r1, r2, #0
	strh	r0, [r1, #0]
	ldr	r1, [sp, #24]
	str	r1, [r7, #20]
.L_08029930:
	adds	r2, r7, #0
	adds	r2, #102
	str	r2, [sp, #4]
	movs	r4, #0
	ldrsh	r3, [r2, r4]
	cmp	r3, #0
	bne.n	.L_08029940
	b.n	.L_08029c14
.L_08029940:
	ldr	r6, [sp, #16]
	ldr	r0, [sp, #20]
	cmp	r6, r0
	bge.n	.L_08029984
	ldr	r5, [pc, #772]
	ldr	r1, [sp, #12]
	add	r5, r8
	ldrb	r2, [r1, #0]
	ldr	r0, [r7, #8]
	ldr	r3, [r7, #20]
	adds	r1, r5, #0
	bl	sub_0802d7b0
	cmp	r0, #2
	bne.n	.L_08029968
	ldr	r3, [r7, #12]
	ldr	r2, [pc, #752]
	adds	r3, r3, r2
	str	r3, [r7, #12]
	b.n	.L_08029984
.L_08029968:
	adds	r0, r7, #0
	adds	r3, r5, #0
	ldr	r1, [r7, #8]
	ldr	r2, [sp, #16]
	bl	sub_08024738
	ldr	r4, [sp, #8]
	movs	r3, #1
	strh	r3, [r4, #0]
	add	r6, sp, #28
	ldrh	r6, [r6, #0]
	ldr	r0, [sp, #4]
	strh	r6, [r0, #0]
	b.n	.L_0802a100
.L_08029984:
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #20]
	cmp	r0, r1
	ble.n	.L_0802999e
	ldr	r2, [sp, #4]
	movs	r3, #0
	strh	r3, [r2, #0]
	adds	r2, r7, #0
	adds	r2, #85
	movs	r3, #3
	str	r0, [r7, #20]
	strb	r3, [r2, #0]
	b.n	.L_0802a100
.L_0802999e:
	movs	r3, #0
	str	r3, [sp, #32]
	ldr	r3, [pc, #692]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080299ae
	ldr	r3, [pc, #688]
	ldr	r3, [r3, #0]
.L_080299ae:
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #52]
	ldr	r3, [pc, #672]
	ldr	r1, [pc, #672]
	ldr	r3, [r3, #0]
	movs	r2, #15
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r3, [r1, r3]
	movs	r0, #255
	lsls	r3, r3, #16
	mov	r9, r3
	mov	r6, r9
	lsrs	r6, r6, #16
	lsls	r0, r0, #8
	mov	r8, r6
	adds	r0, #255
	cmp	r8, r0
	bne.n	.L_080299e4
	movs	r1, #4
	str	r1, [sp, #32]
	b.n	.L_08029b12
.L_080299e4:
	ldr	r3, [r7, #8]
	add	r6, sp, #80
	str	r3, [r6, #0]
	ldr	r2, [sp, #32]
	movs	r3, #128
	lsls	r3, r3, #12
	mov	sl, r3
	str	r2, [r6, #4]
	str	r2, [r6, #8]
	mov	r0, sl
	mov	r1, r8
	adds	r2, r6, #0
	bl	sub_0801489c
	ldr	r3, [r7, #12]
	ldr	r2, [r6, #8]
	adds	r0, r7, #0
	subs	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r3, [r7, #16]
	movs	r1, #3
	str	r3, [r6, #8]
	bl	sub_080233d0
	ldr	r3, [r7, #8]
	add	r5, sp, #68
	str	r3, [r5, #0]
	ldr	r4, [sp, #32]
	mov	r0, sl
	mov	r1, r8
	adds	r2, r5, #0
	str	r4, [r5, #4]
	str	r4, [r5, #8]
	bl	sub_0801489c
	ldr	r3, [r7, #12]
	ldr	r2, [r5, #8]
	adds	r0, r7, #0
	subs	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	adds	r1, r5, #0
	str	r3, [r5, #8]
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08029afe
	ldr	r3, [r7, #8]
	movs	r1, #128
	str	r3, [r5, #0]
	ldr	r0, [sp, #32]
	lsls	r1, r1, #5
	str	r0, [r5, #4]
	str	r0, [r5, #8]
	add	r1, r8
	mov	r0, sl
	adds	r2, r5, #0
	bl	sub_0801489c
	ldr	r3, [r7, #12]
	ldr	r2, [r5, #8]
	adds	r0, r7, #0
	subs	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	adds	r1, r5, #0
	str	r3, [r5, #8]
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08029afe
	ldr	r3, [r7, #8]
	mov	r0, sl
	str	r3, [r5, #0]
	ldr	r1, [sp, #32]
	adds	r2, r5, #0
	str	r1, [r5, #4]
	str	r1, [r5, #8]
	ldr	r1, [pc, #480]
	add	r1, r8
	bl	sub_0801489c
	ldr	r3, [r7, #12]
	ldr	r2, [r5, #8]
	adds	r0, r7, #0
	subs	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	adds	r1, r5, #0
	str	r3, [r5, #8]
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08029afe
	ldr	r3, [r7, #8]
	movs	r1, #128
	str	r3, [r5, #0]
	ldr	r2, [sp, #32]
	lsls	r1, r1, #6
	str	r2, [r5, #4]
	str	r2, [r5, #8]
	add	r1, r8
	mov	r0, sl
	adds	r2, r5, #0
	bl	sub_0801489c
	ldr	r3, [r7, #12]
	ldr	r2, [r5, #8]
	adds	r0, r7, #0
	subs	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	adds	r1, r5, #0
	str	r3, [r5, #8]
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08029afe
	ldr	r3, [r7, #8]
	ldr	r1, [pc, #404]
	str	r3, [r5, #0]
	ldr	r3, [sp, #32]
	add	r1, r8
	str	r3, [r5, #4]
	str	r3, [r5, #8]
	mov	r0, sl
	adds	r2, r5, #0
	bl	sub_0801489c
	ldr	r3, [r7, #12]
	ldr	r2, [r5, #8]
	adds	r0, r7, #0
	subs	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	adds	r1, r5, #0
	str	r3, [r5, #8]
	bl	sub_0802d87c
	cmp	r0, #0
	beq.n	.L_08029b12
.L_08029afe:
	ldr	r3, [r7, #8]
	str	r3, [r6, #0]
	ldr	r3, [r7, #12]
	str	r3, [r6, #4]
	ldr	r3, [r7, #16]
	str	r3, [r6, #8]
	ldr	r4, [sp, #32]
	movs	r3, #1
	orrs	r4, r3
	str	r4, [sp, #32]
.L_08029b12:
	ldr	r6, [sp, #32]
	cmp	r6, #0
	beq.n	.L_08029b28
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r7, #56]
	str	r3, [r7, #60]
	str	r3, [r7, #64]
	movs	r3, #0
	str	r3, [r7, #36]
	b.n	.L_08029b74
.L_08029b28:
	mov	r0, r9
	asrs	r0, r0, #16
	str	r0, [sp, #28]
	add	r3, sp, #80
	ldr	r2, [r3, #4]
	ldr	r1, [r3, #0]
	adds	r0, r7, #0
	ldr	r3, [r3, #8]
	bl	sub_08024738
	ldr	r1, [r7, #36]
	ldr	r6, [pc, #300]
	adds	r0, r1, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x6af9
	adds	r5, r0, #0
	adds	r0, r1, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x182d
	adds	r0, r5, #0
	bl	sub_080149e0
	ldr	r1, [sp, #32]
	str	r1, [r7, #36]
	str	r1, [r7, #44]
	ldr	r2, [sp, #28]
	lsls	r1, r2, #16
	adds	r2, r7, #0
	lsrs	r1, r1, #16
	adds	r2, #36
	bl	sub_0801489c
	ldr	r3, [r7, #44]
	negs	r3, r3
	str	r3, [r7, #40]
	ldr	r3, [sp, #32]
.L_08029b74:
	str	r3, [r7, #44]
	ldr	r3, [pc, #248]
	ldr	r3, [r3, #0]
	mov	r8, r3
	mov	r4, r8
	movs	r3, #15
	ands	r4, r3
	mov	r8, r4
	cmp	r4, #0
	bne.n	.L_08029c0c
	bl	sub_08014878
	ldr	r1, [r7, #8]
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #1
	movs	r6, #192
	subs	r1, r1, r3
	lsls	r6, r6, #10
	add	r4, sp, #36
	adds	r1, r1, r6
	str	r1, [r4, #0]
	ldr	r2, [r7, #12]
	movs	r0, #30
	str	r2, [r4, #4]
	ldr	r3, [r7, #16]
	adds	r0, #255
	str	r3, [r4, #8]
	bl	sub_08023220
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_08029c0c
	ldr	r3, [r7, #20]
	ldr	r1, [pc, #184]
	str	r3, [r5, #20]
	ldr	r6, [r5, #80]
	bl	sub_080233a8
	ldr	r3, [pc, #180]
	adds	r2, r5, #0
	str	r3, [r5, #108]
	adds	r2, #85
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r0, [sp, #32]
	cmp	r0, #0
	bne.n	.L_08029bdc
	movs	r0, #149
	lsls	r0, r0, #2
	bl	sub_081c0010
.L_08029bdc:
	movs	r3, #160
	lsls	r3, r3, #4
	adds	r3, #61
	str	r3, [r5, #72]
	movs	r3, #152
	lsls	r3, r3, #7
	adds	r3, #204
	mov	r1, r8
	str	r1, [r5, #40]
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	cmp	r6, #0
	beq.n	.L_08029c0c
	adds	r0, r6, #0
	movs	r1, #2
	bl	sub_08022b04
	mov	r2, r8
	strb	r2, [r6, #26]
	ldrb	r2, [r6, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r6, #9]
.L_08029c0c:
	adds	r0, r7, #0
	bl	.L_08029838
	b.n	.L_0802a100
.L_08029c14:
	movs	r3, #0
	str	r3, [sp, #32]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #52]
	ldr	r5, [pc, #52]
	ldr	r1, [pc, #56]
	ldr	r3, [r5, #0]
	movs	r2, #15
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r3, [r1, r3]
	movs	r6, #255
	lsls	r3, r3, #16
	lsls	r6, r6, #8
	lsrs	r1, r3, #16
	adds	r6, #255
	str	r3, [sp, #0]
	cmp	r1, r6
	bne.n	.L_08029c7c
	movs	r0, #4
	str	r0, [sp, #32]
	b.n	.L_08029f68
	movs	r0, r0
	.4byte 0xfff00000
	.4byte 0xfff80000
	.4byte 0xffff0000
	.4byte 0x03001238
	.4byte 0x03001150
	.4byte 0x0802ec5c
	.4byte 0xfffff000
	.4byte 0xffffe000
	.4byte 0x0300021c
	.4byte 0x0300122c
	.4byte 0x0802ed28
	.2byte 0x9811
	.2byte 0x0802
.L_08029c7c:
	ldr	r3, [r7, #8]
	add	r2, sp, #80
	str	r3, [r2, #0]
	ldr	r3, [r7, #12]
	movs	r0, #128
	str	r3, [r2, #4]
	ldr	r3, [r7, #16]
	lsls	r0, r0, #12
	str	r3, [r2, #8]
	mov	fp, r2
	bl	sub_0801489c
	ldr	r3, [pc, #860]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08029cb0
	ldr	r3, [r5, #0]
	ldr	r4, [sp, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	asrs	r4, r4, #16
	ands	r3, r2
	str	r4, [sp, #28]
	cmp	r3, #0
	beq.n	.L_08029cb0
	b.n	.L_08029f68
.L_08029cb0:
	adds	r0, r7, #0
	mov	r1, fp
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08029d64
	ldr	r3, [r7, #8]
	add	r5, sp, #68
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	movs	r6, #128
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	lsls	r6, r6, #12
	str	r3, [r5, #8]
	ldr	r0, [sp, #0]
	movs	r2, #128
	mov	r8, r6
	lsls	r2, r2, #5
	lsrs	r6, r0, #16
	adds	r1, r6, r2
	mov	r0, r8
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08029d64
	ldr	r3, [r7, #8]
	mov	r0, r8
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	adds	r2, r5, #0
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	str	r3, [r5, #8]
	ldr	r3, [pc, #760]
	adds	r1, r6, r3
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08029d64
	ldr	r3, [r7, #8]
	movs	r4, #128
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	lsls	r4, r4, #6
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	adds	r1, r6, r4
	mov	r0, r8
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08029d64
	ldr	r3, [r7, #8]
	ldr	r0, [pc, #704]
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	adds	r1, r6, r0
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	mov	r0, r8
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r1, r5, #0
	adds	r0, r7, #0
	bl	sub_0802d87c
	ldr	r1, [sp, #0]
	asrs	r1, r1, #16
	str	r1, [sp, #28]
	cmp	r0, #0
	bne.n	.L_08029d64
	b.n	.L_08029f68
.L_08029d64:
	ldr	r4, [sp, #0]
	add	r2, sp, #48
	ldr	r1, [pc, #652]
	movs	r6, #128
	lsrs	r3, r4, #16
	mov	sl, r2
	lsls	r6, r6, #5
	adds	r2, r3, r6
	mov	r0, sl
	strh	r2, [r0, #0]
	movs	r6, #128
	adds	r2, r3, r1
	ldr	r1, [pc, #636]
	mov	r4, sl
	lsls	r6, r6, #6
	strh	r2, [r4, #2]
	adds	r2, r3, r6
	strh	r2, [r0, #4]
	movs	r6, #192
	adds	r2, r3, r1
	ldr	r1, [pc, #624]
	lsls	r6, r6, #6
	strh	r2, [r4, #6]
	adds	r2, r3, r6
	strh	r2, [r0, #8]
	adds	r3, r3, r1
	mov	r2, sl
	strh	r3, [r2, #10]
	movs	r3, #0
	mov	r9, r3
	mov	r8, fp
.L_08029da2:
	mov	r4, r9
	lsls	r3, r4, #1
	mov	r6, sl
	ldrsh	r6, [r6, r3]
	mov	r1, r8
	str	r6, [sp, #28]
	ldr	r3, [r7, #8]
	movs	r0, #128
	str	r3, [r1, #0]
	ldr	r3, [r7, #12]
	lsls	r0, r0, #12
	str	r3, [r1, #4]
	ldr	r3, [r7, #16]
	mov	r2, r8
	str	r3, [r1, #8]
	lsls	r3, r6, #16
	lsrs	r6, r3, #16
	adds	r1, r6, #0
	bl	sub_0801489c
	adds	r0, r7, #0
	mov	r1, r8
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08029e74
	ldr	r3, [r7, #8]
	add	r5, sp, #68
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	movs	r2, #128
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	lsls	r2, r2, #5
	movs	r0, #128
	adds	r1, r6, r2
	lsls	r0, r0, #12
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08029e74
	ldr	r3, [r7, #8]
	movs	r0, #128
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	lsls	r0, r0, #12
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	adds	r2, r5, #0
	str	r3, [r5, #8]
	ldr	r3, [pc, #484]
	adds	r1, r6, r3
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08029e74
	ldr	r3, [r7, #8]
	movs	r4, #128
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	lsls	r4, r4, #6
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	movs	r0, #128
	adds	r1, r6, r4
	lsls	r0, r0, #12
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	bne.n	.L_08029e74
	ldr	r3, [r7, #8]
	ldr	r0, [pc, #424]
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	adds	r1, r6, r0
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	movs	r0, #128
	lsls	r0, r0, #12
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0802d87c
	cmp	r0, #0
	beq.n	.L_08029f68
.L_08029e74:
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	cmp	r2, #6
	blt.n	.L_08029da2
	ldr	r0, [r7, #8]
	mov	r3, fp
	str	r0, [r3, #0]
	ldr	r3, [r7, #12]
	mov	r4, fp
	str	r3, [r4, #4]
	ldr	r1, [r7, #16]
	movs	r5, #1
	str	r1, [r4, #8]
	ldr	r6, [sp, #32]
	ldr	r4, [pc, #368]
	orrs	r6, r5
	str	r6, [sp, #32]
	movs	r2, #64
	ldr	r3, [r4, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08029ef4
	ldr	r2, [sp, #20]
	movs	r6, #128
	lsls	r6, r6, #12
	adds	r3, r2, r6
	ldr	r2, [sp, #16]
	cmp	r2, r3
	ble.n	.L_08029ef4
	ldr	r3, [pc, #340]
	movs	r2, #2
	ands	r0, r3
	ands	r1, r3
	ldr	r3, [pc, #336]
	adds	r6, r0, r6
	adds	r3, r3, r1
	mov	r8, r3
	adds	r0, r6, #0
	mov	r1, r8
	bl	sub_080c8770
	cmp	r0, #0
	beq.n	.L_08029ef2
	adds	r3, r7, #0
	movs	r2, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	ldr	r3, [r7, #12]
	movs	r4, #128
	lsls	r4, r4, #9
	adds	r3, r3, r4
	str	r3, [r7, #12]
	ldr	r6, [sp, #4]
	adds	r0, r7, #0
	strh	r5, [r6, #0]
	movs	r1, #3
	str	r2, [r7, #36]
	str	r2, [r7, #40]
	str	r2, [r7, #44]
	bl	sub_080233d0
	b.n	.L_0802a100
.L_08029ef2:
	ldr	r4, [pc, #272]
.L_08029ef4:
	ldr	r3, [r4, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08029f68
	ldr	r2, [pc, #264]
	ldr	r3, [r7, #8]
	movs	r0, #128
	ands	r3, r2
	lsls	r0, r0, #12
	adds	r6, r3, r0
	ldr	r3, [r7, #16]
	movs	r1, #192
	ands	r3, r2
	lsls	r1, r1, #13
	ldr	r2, [sp, #12]
	adds	r1, r1, r3
	mov	r8, r1
	ldrb	r0, [r2, #0]
	adds	r1, r6, #0
	mov	r2, r8
	bl	sub_0802d45c
	ldr	r3, [sp, #20]
	cmp	r0, r3
	bge.n	.L_08029f68
	adds	r0, r6, #0
	mov	r1, r8
	movs	r2, #2
	bl	sub_080c8770
	cmp	r0, #0
	beq.n	.L_08029f68
	adds	r3, r7, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	ldr	r4, [sp, #24]
	ldr	r6, [pc, #204]
	ldr	r1, [r7, #8]
	adds	r0, r7, #0
	mov	r3, r8
	adds	r2, r4, r6
	bl	sub_08024738
	ldr	r0, [sp, #8]
	movs	r3, #1
	strh	r3, [r0, #0]
	ldr	r1, [sp, #4]
	adds	r0, r7, #0
	strh	r3, [r1, #0]
	str	r5, [r7, #36]
	str	r5, [r7, #40]
	str	r5, [r7, #44]
	movs	r1, #3
	bl	sub_080233d0
	b.n	.L_0802a100
.L_08029f68:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	cmp	r1, #0
	beq.n	.L_08029f9e
	ldr	r3, [sp, #32]
	movs	r2, #3
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08029f8a
	movs	r4, #194
	lsls	r4, r4, #1
	adds	r2, r1, r4
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_08029f92
.L_08029f8a:
	movs	r6, #194
	lsls	r6, r6, #1
	adds	r3, r1, r6
	strh	r2, [r3, #0]
.L_08029f92:
	ldr	r3, [pc, #112]
	movs	r0, #195
	ldr	r3, [r3, #0]
	lsls	r0, r0, #1
	adds	r2, r1, r0
	strh	r3, [r2, #0]
.L_08029f9e:
	movs	r1, #1
	adds	r0, r7, #0
	bl	sub_080233d0
	ldr	r1, [sp, #32]
	cmp	r1, #0
	beq.n	.L_0802a014
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r7, #56]
	str	r3, [r7, #60]
	str	r3, [r7, #64]
	movs	r3, #0
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	movs	r3, #3
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08029fe6
	ldr	r2, [sp, #0]
	ldrh	r1, [r7, #6]
	lsrs	r3, r2, #16
	subs	r3, r3, r1
	lsls	r3, r3, #16
	movs	r2, #128
	asrs	r3, r3, #16
	lsls	r2, r2, #5
	cmp	r3, r2
	ble.n	.L_08029fda
	adds	r3, r2, #0
.L_08029fda:
	ldr	r2, [pc, #28]
	cmp	r3, r2
	bge.n	.L_08029fe2
	adds	r3, r2, #0
.L_08029fe2:
	adds	r3, r1, r3
	strh	r3, [r7, #6]
.L_08029fe6:
	movs	r3, #98
	adds	r3, r3, r7
	mov	r8, r3
	mov	r4, r8
	movs	r3, #0
	b.n	.L_0802a064
	movs	r0, r0
	.4byte 0x03001238
	.4byte 0xfffff000
	.4byte 0xffffe000
	.4byte 0xffffd000
	.4byte 0x03001150
	.4byte 0xfff00000
	.4byte 0xfff80000
	.2byte 0x0000
	.2byte 0xfffe
.L_0802a014:
	.2byte 0xab14
	ldr	r2, [r3, #4]
	ldr	r1, [r3, #0]
	adds	r0, r7, #0
	ldr	r3, [r3, #8]
	bl	sub_08024738
	ldr	r1, [r7, #36]
	ldr	r6, [pc, #240]
	adds	r0, r1, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x6af9
	adds	r5, r0, #0
	adds	r0, r1, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x182d
	adds	r0, r5, #0
	bl	sub_080149e0
	ldr	r6, [sp, #32]
	str	r6, [r7, #36]
	str	r6, [r7, #44]
	ldr	r2, [sp, #28]
	lsls	r1, r2, #16
	adds	r2, r7, #0
	adds	r2, #36
	lsrs	r1, r1, #16
	bl	sub_0801489c
	movs	r3, #98
	adds	r3, r3, r7
	ldrb	r2, [r3, #0]
	mov	r8, r3
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_0802a066
	adds	r3, #255
	mov	r4, r8
.L_0802a064:
	strb	r3, [r4, #0]
.L_0802a066:
	mov	r6, r8
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	bne.n	.L_0802a0fa
	ldr	r0, [sp, #32]
	cmp	r0, #0
	bne.n	.L_0802a0fa
	ldr	r3, [r7, #16]
	ldr	r4, [pc, #164]
	movs	r0, #178
	lsls	r0, r0, #1
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	adds	r3, r3, r4
	bl	sub_08023220
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0802a0fa
	ldr	r3, [r7, #20]
	ldr	r1, [pc, #144]
	str	r3, [r5, #20]
	ldr	r6, [r5, #80]
	bl	sub_080233a8
	adds	r2, r5, #0
	adds	r2, #35
	movs	r3, #8
	add	r0, sp, #32
	strb	r3, [r2, #0]
	ldrb	r0, [r0, #0]
	movs	r3, #100
	adds	r2, #64
	strb	r3, [r2, #0]
	adds	r3, r5, #0
	adds	r3, #85
	strb	r0, [r3, #0]
	movs	r0, #149
	lsls	r0, r0, #2
	bl	sub_081c0010
	cmp	r6, #0
	beq.n	.L_0802a0f4
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r1, #1
	adds	r0, r6, #0
	ldr	r5, [r3, #108]
	bl	sub_08022b04
	ldrb	r3, [r6, #9]
	movs	r0, #13
	negs	r0, r0
	adds	r2, r0, #0
	ands	r2, r3
	movs	r3, #8
	orrs	r2, r3
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #74
	add	r1, sp, #32
	ldrb	r1, [r1, #0]
	adds	r5, r5, r3
	ldrb	r3, [r5, #0]
	strb	r1, [r6, #26]
	movs	r1, #3
	ands	r3, r1
	lsls	r3, r3, #2
	ands	r2, r0
	orrs	r2, r3
	strb	r2, [r6, #9]
.L_0802a0f4:
	movs	r3, #5
	mov	r4, r8
	strb	r3, [r4, #0]
.L_0802a0fa:
	adds	r0, r7, #0
	bl	.L_08029838
.L_0802a100:
	ldrh	r3, [r7, #4]
	movs	r0, #1
	adds	r3, #1
	strh	r3, [r7, #4]
	add	sp, #92
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0300021c
	.4byte 0xfffe0000
	.2byte 0xece0
	.2byte 0x0802
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	adds	r1, r7, #0
	sub	sp, #100
	adds	r1, #102
	movs	r0, #0
	str	r0, [sp, #16]
	str	r1, [sp, #4]
	add	r2, sp, #16
	ldrh	r2, [r2, #0]
	adds	r3, r1, #0
	strh	r2, [r3, #0]
	adds	r2, r7, #0
	movs	r3, #1
	adds	r2, #100
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #52]
	ldr	r5, [pc, #768]
	movs	r6, #128
	ldr	r3, [r5, #12]
	lsls	r6, r6, #2
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_0802a170
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r7, #48]
.L_0802a170:
	ldr	r3, [r5, #0]
	ldr	r1, [pc, #752]
	lsrs	r3, r3, #4
	movs	r2, #15
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r4, [r1, r3]
	movs	r1, #255
	lsls	r3, r4, #16
	lsrs	r3, r3, #16
	lsls	r1, r1, #8
	mov	r8, r3
	adds	r1, #255
	str	r4, [sp, #12]
	cmp	r8, r1
	bne.n	.L_0802a19e
	movs	r2, #4
	str	r2, [sp, #16]
	b.n	.L_0802a310
.L_0802a196:
	mov	r3, r9
	asrs	r3, r3, #16
	str	r3, [sp, #12]
	b.n	.L_0802a310
.L_0802a19e:
	mov	r4, sp
	adds	r4, #88
	str	r4, [sp, #0]
	ldr	r3, [r7, #8]
	movs	r0, #128
	str	r3, [r4, #0]
	ldr	r3, [r7, #12]
	lsls	r0, r0, #12
	str	r3, [r4, #4]
	ldr	r3, [r7, #16]
	mov	r1, r8
	str	r3, [r4, #8]
	ldr	r2, [sp, #0]
	bl	sub_0801489c
	ldr	r3, [pc, #680]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0802a1ce
	ldr	r3, [r5, #0]
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_0802a1ce
	b.n	.L_0802a310
.L_0802a1ce:
	ldr	r3, [r7, #8]
	add	r0, sp, #76
	str	r3, [r0, #0]
	ldr	r3, [r7, #12]
	movs	r1, #168
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	lsls	r1, r1, #5
	mov	fp, r0
	str	r3, [r0, #8]
	adds	r1, #85
	movs	r0, #128
	add	r1, r8
	lsls	r0, r0, #12
	mov	r2, fp
	bl	sub_0801489c
	ldr	r3, [r7, #8]
	add	r1, sp, #64
	str	r3, [r1, #0]
	ldr	r3, [r7, #12]
	mov	r9, r1
	str	r3, [r1, #4]
	ldr	r3, [r7, #16]
	movs	r0, #128
	str	r3, [r1, #8]
	ldr	r1, [pc, #616]
	mov	r2, r9
	add	r1, r8
	lsls	r0, r0, #12
	bl	sub_0801489c
	adds	r0, r7, #0
	ldr	r1, [sp, #0]
	bl	sub_0802db64
	mov	r1, fp
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	sub_0802db64
	mov	r1, r9
	adds	r6, r0, #0
	adds	r0, r7, #0
	bl	sub_0802db64
	orrs	r5, r6
	orrs	r5, r0
	cmp	r5, #0
	beq.n	.L_0802a310
	movs	r2, #20
	ldr	r0, [sp, #16]
	add	r2, sp
	movs	r3, #128
	mov	sl, r2
	lsls	r3, r3, #5
	add	r3, r8
	mov	r4, sl
	strh	r3, [r4, r0]
	ldr	r3, [pc, #552]
	mov	r1, sl
	add	r3, r8
	strh	r3, [r1, #2]
	movs	r3, #128
	lsls	r3, r3, #6
	add	r3, r8
	strh	r3, [r2, #4]
	ldr	r3, [pc, #540]
	mov	r0, sl
	add	r3, r8
	strh	r3, [r4, #6]
	movs	r3, #192
	lsls	r3, r3, #6
	add	r3, r8
	strh	r3, [r0, #8]
	ldr	r3, [pc, #528]
	movs	r2, #0
	add	r3, r8
	strh	r3, [r1, #10]
	str	r2, [sp, #8]
	mov	r8, r9
.L_0802a270:
	ldr	r4, [sp, #8]
	mov	r0, sl
	lsls	r3, r4, #1
	ldrsh	r2, [r0, r3]
	ldr	r3, [r7, #8]
	lsls	r2, r2, #16
	str	r3, [sp, #88]
	ldr	r3, [r7, #12]
	lsrs	r5, r2, #16
	str	r3, [sp, #92]
	ldr	r3, [r7, #16]
	movs	r0, #128
	str	r3, [sp, #96]
	add	r3, sp, #88
	adds	r1, r5, #0
	lsls	r0, r0, #12
	mov	r9, r2
	adds	r2, r3, #0
	bl	sub_0801489c
	ldr	r3, [r7, #8]
	mov	r4, fp
	str	r3, [r4, #0]
	ldr	r3, [r7, #12]
	movs	r0, #168
	str	r3, [r4, #4]
	ldr	r3, [r7, #16]
	lsls	r0, r0, #5
	adds	r0, #85
	adds	r1, r5, r0
	movs	r0, #128
	str	r3, [r4, #8]
	lsls	r0, r0, #12
	mov	r2, fp
	bl	sub_0801489c
	ldr	r3, [r7, #8]
	mov	r1, r8
	str	r3, [r1, #0]
	ldr	r3, [r7, #12]
	ldr	r2, [pc, #424]
	str	r3, [r1, #4]
	ldr	r3, [r7, #16]
	adds	r5, r5, r2
	movs	r0, #128
	mov	r2, r8
	str	r3, [r1, #8]
	lsls	r0, r0, #12
	adds	r1, r5, #0
	bl	sub_0801489c
	add	r3, sp, #88
	adds	r1, r3, #0
	adds	r0, r7, #0
	bl	sub_0802db64
	mov	r1, fp
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	sub_0802db64
	mov	r1, r8
	adds	r6, r0, #0
	adds	r0, r7, #0
	bl	sub_0802db64
	orrs	r5, r6
	orrs	r5, r0
	cmp	r5, #0
	bne.n	.L_0802a2fe
	b.n	.L_0802a196
.L_0802a2fe:
	ldr	r4, [sp, #8]
	adds	r4, #1
	str	r4, [sp, #8]
	cmp	r4, #6
	blt.n	.L_0802a270
	ldr	r0, [sp, #16]
	movs	r3, #1
	orrs	r0, r3
	str	r0, [sp, #16]
.L_0802a310:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	cmp	r1, #0
	beq.n	.L_0802a346
	ldr	r3, [sp, #16]
	movs	r2, #3
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0802a332
	movs	r4, #194
	lsls	r4, r4, #1
	adds	r2, r1, r4
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_0802a33a
.L_0802a332:
	movs	r0, #194
	lsls	r0, r0, #1
	adds	r3, r1, r0
	strh	r2, [r3, #0]
.L_0802a33a:
	ldr	r3, [pc, #292]
	movs	r4, #195
	ldr	r3, [r3, #0]
	lsls	r4, r4, #1
	adds	r2, r1, r4
	strh	r3, [r2, #0]
.L_0802a346:
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_080233d0
	ldr	r0, [sp, #16]
	cmp	r0, #0
	beq.n	.L_0802a3a2
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r7, #56]
	str	r3, [r7, #60]
	str	r3, [r7, #64]
	movs	r3, #0
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	movs	r3, #3
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_0802a390
	ldr	r1, [sp, #12]
	ldrh	r2, [r7, #6]
	lsls	r3, r1, #16
	lsrs	r3, r3, #16
	subs	r3, r3, r2
	lsls	r3, r3, #16
	asrs	r1, r3, #16
	movs	r3, #128
	lsls	r3, r3, #5
	cmp	r1, r3
	ble.n	.L_0802a384
	adds	r1, r3, #0
.L_0802a384:
	ldr	r3, [pc, #232]
	cmp	r1, r3
	bge.n	.L_0802a38c
	adds	r1, r3, #0
.L_0802a38c:
	adds	r3, r2, r1
	strh	r3, [r7, #6]
.L_0802a390:
	movs	r2, #98
	adds	r2, r2, r7
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r4, [sp, #4]
	movs	r3, #2
	mov	r8, r2
	strh	r3, [r4, #0]
	b.n	.L_0802a3f4
.L_0802a3a2:
	add	r3, sp, #88
	ldr	r2, [r3, #4]
	ldr	r1, [r3, #0]
	adds	r0, r7, #0
	ldr	r3, [r3, #8]
	bl	sub_08024738
	ldr	r1, [r7, #36]
	ldr	r6, [pc, #200]
	adds	r0, r1, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x6af9
	adds	r5, r0, #0
	adds	r0, r1, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x182d
	adds	r0, r5, #0
	bl	sub_080149e0
	ldr	r1, [sp, #16]
	str	r1, [r7, #36]
	str	r1, [r7, #44]
	ldr	r2, [sp, #12]
	lsls	r1, r2, #16
	adds	r2, r7, #0
	adds	r2, #36
	lsrs	r1, r1, #16
	bl	sub_0801489c
	movs	r3, #98
	adds	r3, r3, r7
	ldrb	r2, [r3, #0]
	mov	r8, r3
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_0802a3f4
	adds	r3, #255
	mov	r4, r8
	strb	r3, [r4, #0]
.L_0802a3f4:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #32]
	ldr	r3, [pc, #100]
	ldr	r1, [pc, #128]
	ldr	r3, [r3, #0]
	movs	r2, #15
	lsrs	r3, r3, #4
	ands	r3, r2
	movs	r2, #143
	lsls	r3, r3, #2
	lsls	r2, r2, #1
	ldr	r4, [r1, r3]
	adds	r1, r0, r2
	ldrh	r0, [r1, #0]
	subs	r3, r4, r0
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, #0
	bge.n	.L_0802a41e
	adds	r3, #7
.L_0802a41e:
	asrs	r2, r3, #3
	movs	r3, #128
	lsls	r3, r3, #2
	cmp	r2, r3
	ble.n	.L_0802a42a
	adds	r2, r3, #0
.L_0802a42a:
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bge.n	.L_0802a432
	adds	r2, r3, #0
.L_0802a432:
	adds	r3, r2, #0
	adds	r3, #15
	cmp	r3, #30
	bhi.n	.L_0802a43e
	ldrh	r3, [r1, #0]
	subs	r2, r4, r3
.L_0802a43e:
	adds	r3, r0, r2
	strh	r3, [r1, #0]
	adds	r3, r7, #0
	adds	r3, #84
	ldrb	r6, [r3, #0]
	cmp	r6, #1
	bne.n	.L_0802a48e
	adds	r0, r7, #0
	adds	r0, #8
	ldr	r5, [r7, #80]
	bl	sub_0802dac0
	cmp	r0, #9
	bne.n	.L_0802a488
	ldr	r3, [r5, #44]
	strb	r6, [r3, #6]
	b.n	.L_0802a48e
	.4byte 0x03001150
	.4byte 0x0802ec5c
	.4byte 0x03001238
	.4byte 0xffffeaab
	.4byte 0xfffff000
	.4byte 0xffffe000
	.4byte 0xffffd000
	.4byte 0x0300021c
	.4byte 0x0802eca0
	.2byte 0xfe00
	.2byte 0xffff
.L_0802a488:
	ldr	r2, [r5, #44]
	movs	r3, #9
	strb	r3, [r2, #6]
.L_0802a48e:
	mov	r4, r8
	ldrb	r3, [r4, #0]
	cmp	r3, #0
	bne.n	.L_0802a506
	ldr	r0, [sp, #16]
	cmp	r0, #0
	bne.n	.L_0802a506
	ldr	r3, [r7, #16]
	ldr	r4, [pc, #132]
	movs	r0, #178
	lsls	r0, r0, #1
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	adds	r3, r3, r4
	bl	sub_08023220
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0802a506
	ldr	r3, [r7, #20]
	ldr	r1, [pc, #112]
	str	r3, [r5, #20]
	ldr	r6, [r5, #80]
	bl	sub_080233a8
	adds	r2, r5, #0
	adds	r2, #35
	movs	r3, #8
	add	r0, sp, #16
	strb	r3, [r2, #0]
	ldrb	r0, [r0, #0]
	movs	r3, #100
	adds	r2, #64
	strb	r3, [r2, #0]
	adds	r3, r5, #0
	adds	r3, #85
	strb	r0, [r3, #0]
	cmp	r6, #0
	beq.n	.L_0802a500
	movs	r1, #1
	adds	r0, r6, #0
	bl	sub_08022b04
	add	r1, sp, #16
	ldrb	r1, [r1, #0]
	ldrb	r2, [r6, #9]
	strb	r1, [r6, #26]
	movs	r1, #13
	negs	r1, r1
	adds	r3, r1, #0
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	ands	r3, r1
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r6, #9]
.L_0802a500:
	movs	r3, #5
	mov	r2, r8
	strb	r3, [r2, #0]
.L_0802a506:
	adds	r0, r7, #0
	bl	.L_08029838
	ldrh	r3, [r7, #4]
	movs	r0, #1
	adds	r3, #1
	strh	r3, [r7, #4]
	add	sp, #100
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xfffe0000
	.2byte 0xece0
	.2byte 0x0802
	push	{lr}
	movs	r3, #1
	subs	r1, #1
	negs	r3, r3
	ldr	r2, [pc, #20]
	cmp	r1, r3
	beq.n	.L_0802a54a
	mov	ip, r3
.L_0802a53c:
	ldrb	r3, [r0, #0]
	subs	r1, #1
	ldrb	r3, [r2, r3]
	strb	r3, [r0, #0]
	adds	r0, #1
	cmp	r1, ip
	bne.n	.L_0802a53c
.L_0802a54a:
	pop	{pc}
	.2byte 0x08a8
	.2byte 0x0802
.L_0802a550:
	push	{r5, r6, r7, lr}
	subs	r3, r0, #1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r4, r3, #1
	movs	r3, #1
	ands	r3, r0
	ldr	r6, [pc, #120]
	ldr	r5, [pc, #120]
	cmp	r3, #0
	beq.n	.L_0802a5d6
	ldr	r3, [pc, #120]
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	beq.n	.L_0802a594
	cmp	r3, #1
	bgt.n	.L_0802a578
	cmp	r3, #0
	beq.n	.L_0802a57e
	b.n	.L_0802a5d6
.L_0802a578:
	cmp	r3, #2
	beq.n	.L_0802a5bc
	b.n	.L_0802a5d6
.L_0802a57e:
	movs	r1, #0
	cmp	r1, r4
	bge.n	.L_0802a5d6
.L_0802a584:
	ldrh	r3, [r6, #0]
	adds	r1, #1
	strh	r3, [r5, #0]
	adds	r6, #2
	adds	r5, #2
	cmp	r1, r4
	blt.n	.L_0802a584
	b.n	.L_0802a5d6
.L_0802a594:
	ldr	r6, [pc, #64]
	movs	r1, #0
	movs	r7, #0
	adds	r0, r4, r6
	cmp	r1, r4
	bge.n	.L_0802a5d6
.L_0802a5a0:
	ldrb	r3, [r6, #0]
	ldrb	r2, [r0, #0]
	lsls	r3, r3, #8
	orrs	r3, r2
	eors	r3, r7
	adds	r1, #1
	strh	r3, [r5, #0]
	adds	r0, #1
	adds	r6, #1
	adds	r5, #2
	adds	r7, r3, #0
	cmp	r1, r4
	blt.n	.L_0802a5a0
	b.n	.L_0802a5d6
.L_0802a5bc:
	movs	r2, #0
	cmp	r4, #0
	ble.n	.L_0802a5d6
	adds	r1, r4, #0
.L_0802a5c4:
	ldrh	r3, [r6, #0]
	subs	r1, #1
	eors	r3, r2
	strh	r3, [r5, #0]
	adds	r6, #2
	adds	r5, #2
	adds	r2, r3, #0
	cmp	r1, #0
	bne.n	.L_0802a5c4
.L_0802a5d6:
	pop	{r5, r6, r7, pc}
	.4byte 0x02010002
	.4byte 0x02020000
	.2byte 0x0001
	.2byte 0x0201
.L_0802a5e4:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r5, #128
	lsls	r5, r5, #8
	adds	r0, r5, #0
	bl	sub_08014dac
	ldr	r3, [pc, #68]
	ldr	r1, [pc, #72]
	adds	r2, r5, #0
	mov	r8, r0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4d10
	adds	r0, r5, #0
	bl	sub_08014d78
	movs	r2, #132
	movs	r3, #128
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #48]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #40]
	ldr	r1, [pc, #28]
	mov	r2, r8
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c30
	bl	sub_08013164
	mov	r0, r8
	bl	sub_08013164
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.4byte 0x03000730
	.4byte 0x02010000
	.4byte 0x000000a0
	.4byte 0x08021704
	.2byte 0x8000
	.2byte 0x0201
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	ldr	r3, [pc, #84]
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r0, #128
	lsls	r5, r5, #2
	lsls	r0, r0, #2
	mov	sl, r1
	adds	r5, r5, r3
	bl	sub_08014d78
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r6, r0, #0
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	ldrh	r0, [r5, #2]
	ldr	r3, [pc, #56]
	mov	r8, r1
	adds	r0, r0, r3
	bl	sub_08013300
	adds	r1, r6, #0
	bl	sub_0801587c
	mov	r3, r8
	strh	r3, [r6, #0]
	movs	r2, #132
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r6, #0
	mov	r1, sl
	adds	r2, #112
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r6, #0
	bl	sub_08013164
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x0802f380
	.2byte 0x026c
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	movs	r3, #193
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	adds	r5, r0, #0
	strh	r3, [r1, #0]
	movs	r0, #0
	sub	sp, #8
	bl	sub_08013eb4
	ldr	r2, [pc, #532]
	lsls	r3, r5, #1
	adds	r3, r3, r5
	movs	r5, #216
	lsls	r5, r5, #1
	lsls	r3, r3, #2
	adds	r3, r3, r2
	adds	r1, r5, #0
	movs	r0, #32
	str	r3, [sp, #0]
	bl	sub_08014d00
	adds	r1, r5, #0
	ldr	r3, [pc, #512]
	mov	r8, r0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9a00
	ldr	r3, [pc, #504]
	ldrh	r0, [r2, #0]
	adds	r0, r0, r3
	bl	sub_08013300
	adds	r7, r0, #0
	ldr	r3, [r7, #36]
	ldr	r1, [pc, #496]
	adds	r0, r7, r3
	bl	sub_0801587c
	bl	.L_0802a550
	movs	r3, #1
	add	r0, sp, #4
	negs	r3, r3
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r1, [pc, #472]
	ldr	r2, [pc, #476]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [r7, #40]
	adds	r0, r7, r3
	bl	sub_0801587c
	ldr	r3, [r7, #44]
	ldr	r1, [pc, #464]
	adds	r0, r7, r3
	bl	sub_0801587c
	bl	.L_0802a5e4
	ldr	r3, [r7, #48]
	ldr	r1, [pc, #452]
	adds	r0, r7, r3
	bl	sub_0801587c
	ldr	r0, [r7, #52]
	cmp	r0, #0
	beq.n	.L_0802a76a
	ldr	r5, [pc, #444]
	adds	r0, r7, r0
	adds	r1, r5, #0
	bl	sub_0801587c
	adds	r0, r5, #0
	bl	sub_0802cc9c
.L_0802a76a:
	ldr	r0, [r7, #56]
	cmp	r0, #0
	beq.n	.L_0802a780
	ldr	r5, [pc, #424]
	adds	r0, r7, r0
	adds	r1, r5, #0
	bl	sub_0801587c
	adds	r0, r5, #0
	bl	sub_0802ce4c
.L_0802a780:
	ldr	r3, [r7, #60]
	ldr	r1, [pc, #412]
	adds	r0, r7, r3
	bl	sub_0801587c
	ldrb	r3, [r7, #0]
	mov	r2, r8
	adds	r2, #236
	lsls	r3, r3, #19
	str	r3, [r2, #0]
	adds	r2, #4
	ldrb	r3, [r7, #1]
	mov	r1, r8
	lsls	r3, r3, #19
	str	r3, [r2, #0]
	adds	r1, #244
	ldrb	r3, [r7, #2]
	mov	r0, r8
	lsls	r3, r3, #19
	str	r3, [r1, #0]
	adds	r0, #248
	ldrb	r3, [r7, #3]
	lsls	r3, r3, #19
	str	r3, [r0, #0]
	movs	r3, #228
	add	r3, r8
	mov	fp, r3
	mov	r2, fp
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r2, #232
	add	r2, r8
	str	r3, [r2, #0]
	mov	r9, r2
	movs	r3, #130
	ldrb	r2, [r7, #4]
	lsls	r3, r3, #1
	add	r3, r8
	strb	r2, [r3, #0]
	ldrb	r3, [r7, #5]
	movs	r2, #6
	adds	r2, #255
	add	r2, r8
	strb	r3, [r2, #0]
	ldrb	r3, [r7, #6]
	movs	r2, #131
	lsls	r2, r2, #1
	add	r2, r8
	strb	r3, [r2, #0]
	movs	r2, #2
	adds	r2, #255
	movs	r3, #2
	add	r2, r8
	strb	r3, [r2, #0]
	ldr	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L_0802a7f8
	movs	r3, #128
	lsls	r3, r3, #20
	str	r3, [r1, #0]
.L_0802a7f8:
	ldr	r3, [r0, #0]
	cmp	r3, #0
	bne.n	.L_0802a804
	movs	r3, #128
	lsls	r3, r3, #20
	str	r3, [r0, #0]
.L_0802a804:
	movs	r5, #132
	lsls	r5, r5, #1
	adds	r6, r7, #0
	movs	r3, #2
	add	r5, r8
	adds	r6, #12
	mov	sl, r3
.L_0802a812:
	ldrb	r4, [r6, #0]
	ldrb	r0, [r6, #1]
	lsls	r3, r4, #19
	str	r3, [r5, #8]
	lsls	r3, r0, #19
	str	r3, [r5, #12]
	movs	r3, #3
	ldrsb	r3, [r6, r3]
	ldrb	r2, [r6, #6]
	lsls	r3, r3, #12
	str	r3, [r5, #20]
	movs	r3, #4
	ldrsb	r3, [r6, r3]
	lsrs	r0, r0, #1
	lsls	r3, r3, #12
	str	r3, [r5, #24]
	movs	r3, #5
	ldrsb	r3, [r6, r3]
	lsrs	r4, r4, #1
	lsls	r3, r3, #12
	str	r3, [r5, #28]
	movs	r3, #127
	ands	r3, r2
	ldrb	r2, [r6, #7]
	strh	r3, [r5, #40]
	movs	r3, #127
	ands	r3, r2
	movs	r2, #0
	str	r2, [r5, #32]
	str	r2, [r5, #36]
	lsls	r0, r0, #7
	ldr	r2, [pc, #188]
	strh	r0, [r5, #46]
	movs	r1, #2
	ldrsb	r1, [r6, r1]
	adds	r0, r0, r4
	lsls	r3, r3, #7
	strh	r3, [r5, #42]
	lsls	r3, r0, #2
	adds	r3, r3, r2
	lsls	r1, r1, #12
	str	r3, [r5, #48]
	ldr	r3, [pc, #172]
	str	r1, [r5, #16]
	strh	r4, [r5, #44]
	adds	r0, r0, r3
	mov	r2, fp
	str	r0, [r5, #52]
	ldr	r3, [pc, #176]
	ldr	r0, [r2, #0]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x68ab
	mov	r2, r9
	adds	r0, r0, r3
	str	r0, [r5, #0]
	ldr	r1, [r5, #20]
	ldr	r0, [r2, #0]
	ldr	r3, [pc, #156]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x68eb
	movs	r2, #1
	negs	r2, r2
	add	sl, r2
	adds	r0, r0, r3
	mov	r3, sl
	str	r0, [r5, #4]
	adds	r6, #8
	adds	r5, #56
	cmp	r3, #0
	bge.n	.L_0802a812
	movs	r3, #128
	lsls	r3, r3, #5
	mov	r2, r8
	strh	r3, [r2, #20]
	movs	r1, #130
	lsls	r1, r1, #1
	add	r1, r8
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_0802a8bc
	movs	r3, #192
	lsls	r3, r3, #5
	strh	r3, [r2, #20]
.L_0802a8bc:
	movs	r0, #6
	adds	r0, #255
	add	r0, r8
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L_0802a8d4
	mov	r2, r8
	ldrh	r3, [r2, #20]
	ldr	r2, [pc, #32]
	orrs	r3, r2
	mov	r2, r8
	strh	r3, [r2, #20]
.L_0802a8d4:
	movs	r3, #131
	lsls	r3, r3, #1
	add	r3, r8
	mov	ip, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0802a928
	mov	r2, r8
	ldrh	r3, [r2, #20]
	ldr	r2, [pc, #12]
	orrs	r3, r2
	mov	r2, r8
	strh	r3, [r2, #20]
	b.n	.L_0802a928
	.4byte 0x00000400
	.4byte 0x00000200
	.4byte 0x0802f380
	.4byte 0x03000258
	.4byte 0x0000026c
	.4byte 0x02010001
	.4byte 0x0202c000
	.4byte 0x85000800
	.4byte 0x02010000
	.4byte 0x02024000
	.4byte 0x0202d000
	.4byte 0x0202de00
	.4byte 0x0202e000
	.2byte 0x021c
	.2byte 0x0300
.L_0802a928:
	ldrb	r3, [r7, #7]
	ldrb	r2, [r1, #0]
	lsls	r3, r3, #2
	orrs	r2, r3
	movs	r3, #160
	lsls	r3, r3, #3
	orrs	r2, r3
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #14
	strh	r2, [r3, #0]
	ldrb	r2, [r0, #0]
	ldrb	r3, [r7, #8]
	lsls	r3, r3, #2
	orrs	r2, r3
	movs	r3, #192
	lsls	r3, r3, #3
	orrs	r2, r3
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #12
	strh	r2, [r3, #0]
	mov	r3, ip
	ldrb	r2, [r3, #0]
	ldrb	r3, [r7, #9]
	lsls	r3, r3, #2
	orrs	r2, r3
	movs	r3, #224
	lsls	r3, r3, #3
	orrs	r2, r3
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #10
	strh	r2, [r3, #0]
	movs	r5, #184
	lsls	r5, r5, #1
	adds	r0, r5, #0
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_0802a982
	adds	r0, r5, #0
	bl	sub_08016d18
	b.n	.L_0802aa24
.L_0802a982:
	movs	r2, #128
	lsls	r2, r2, #7
	mov	sl, r2
	mov	r0, sl
	bl	sub_08014d78
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_0802aa24
	movs	r3, #160
	lsls	r3, r3, #19
	movs	r2, #0
	ldrsh	r5, [r3, r2]
	mov	r8, r3
	ldr	r3, [sp, #0]
	ldr	r6, [pc, #180]
	ldrh	r0, [r3, #2]
	adds	r0, r0, r6
	bl	sub_08013300
	adds	r1, r7, #0
	bl	sub_0801587c
	movs	r2, #224
	adds	r1, r7, #0
	strh	r5, [r7, #0]
	lsls	r2, r2, #1
	ldr	r5, [pc, #160]
	mov	r0, r8
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9a00
	ldrh	r0, [r2, #4]
	adds	r0, r0, r6
	bl	sub_08013300
	adds	r1, r7, #0
	bl	sub_080158cc
	mov	r2, sl
	adds	r1, r7, #0
	ldr	r0, [pc, #136]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9b00
	ldrh	r0, [r3, #6]
	adds	r0, r0, r6
	bl	sub_08013300
	adds	r1, r7, #0
	bl	sub_080158cc
	adds	r1, r7, #0
	mov	r2, sl
	ldr	r0, [pc, #116]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9a00
	ldrh	r0, [r2, #8]
	adds	r0, r0, r6
	bl	sub_08013300
	adds	r1, r7, #0
	bl	sub_080158cc
	adds	r1, r7, #0
	mov	r2, sl
	ldr	r0, [pc, #92]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9b00
	ldrh	r0, [r3, #10]
	adds	r0, r0, r6
	bl	sub_08013300
	ldr	r1, [pc, #80]
	bl	sub_080158cc
	adds	r0, r7, #0
	bl	sub_08013164
.L_0802aa24:
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r2, #0
	adds	r3, #76
	strh	r2, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	movs	r2, #160
	lsls	r2, r2, #1
	subs	r3, #80
	strh	r2, [r3, #0]
	ldr	r0, [pc, #52]
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r1, #133
	bl	sub_080145a8
	movs	r0, #2
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0000026c
	.4byte 0x03000730
	.4byte 0x06004000
	.4byte 0x06008000
	.4byte 0x0600c000
	.4byte 0x02028000
	.2byte 0xad85
	.2byte 0x0802
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r0, #0
	ldr	r1, [r3, #0]
	sub	sp, #4
	mov	lr, r0
	mov	r8, r0
	mov	ip, r0
	str	r1, [sp, #0]
	cmp	r1, #0
	bne.n	.L_0802aa96
	b.n	.L_0802ab98
.L_0802aa96:
	ldmia	r1!, {r2}
	adds	r3, #252
	mov	lr, r2
	adds	r2, r1, #0
	str	r2, [sp, #0]
	ldmia	r1!, {r5}
	mov	r8, r5
	adds	r5, r1, #0
	str	r5, [sp, #0]
	ldmia	r1!, {r7}
	mov	ip, r7
	adds	r7, r1, #0
	str	r7, [sp, #0]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0802ab98
	adds	r2, r3, #0
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	movs	r1, #1
	negs	r1, r1
	ldrh	r4, [r2, #0]
	cmp	r3, r1
	beq.n	.L_0802ab88
	mov	sl, r1
.L_0802aac8:
	ldr	r5, [pc, #232]
	lsls	r3, r4, #16
	adds	r1, r3, r5
	movs	r7, #2
	ldrsh	r3, [r2, r7]
	ldr	r0, [pc, #228]
	lsls	r3, r3, #16
	add	r3, r8
	adds	r7, r3, r0
	movs	r5, #4
	ldrsh	r3, [r2, r5]
	movs	r0, #240
	lsls	r3, r3, #16
	lsls	r0, r0, #15
	adds	r4, r3, r0
	movs	r5, #6
	ldrsh	r3, [r2, r5]
	movs	r5, #192
	lsls	r3, r3, #16
	add	r3, r8
	lsls	r5, r5, #15
	adds	r0, r3, r5
	cmp	lr, r1
	blt.n	.L_0802ab7c
	cmp	ip, r7
	blt.n	.L_0802ab7c
	cmp	lr, r4
	bgt.n	.L_0802ab7c
	cmp	ip, r0
	bgt.n	.L_0802ab7c
	adds	r5, r1, #0
	mov	r1, lr
	subs	r6, r5, r1
	adds	r2, r6, #0
	cmp	r6, #0
	bge.n	.L_0802ab12
	subs	r2, r1, r5
.L_0802ab12:
	mov	r1, lr
	subs	r3, r4, r1
	cmp	r3, #0
	blt.n	.L_0802ab20
	cmp	r2, r3
	bgt.n	.L_0802ab28
	b.n	.L_0802ab2e
.L_0802ab20:
	mov	r1, lr
	subs	r3, r1, r4
	cmp	r2, r3
	ble.n	.L_0802ab2e
.L_0802ab28:
	adds	r5, r4, #0
	mov	r2, lr
	subs	r6, r5, r2
.L_0802ab2e:
	adds	r1, r7, #0
	mov	r3, ip
	subs	r4, r1, r3
	adds	r2, r4, #0
	cmp	r4, #0
	bge.n	.L_0802ab3c
	subs	r2, r3, r1
.L_0802ab3c:
	mov	r7, ip
	subs	r3, r0, r7
	cmp	r3, #0
	blt.n	.L_0802ab4a
	cmp	r2, r3
	bgt.n	.L_0802ab52
	b.n	.L_0802ab58
.L_0802ab4a:
	mov	r7, ip
	subs	r3, r7, r0
	cmp	r2, r3
	ble.n	.L_0802ab58
.L_0802ab52:
	adds	r1, r0, #0
	mov	r0, ip
	subs	r4, r1, r0
.L_0802ab58:
	adds	r2, r6, #0
	cmp	r2, #0
	bge.n	.L_0802ab62
	mov	r3, lr
	subs	r2, r3, r5
.L_0802ab62:
	cmp	r4, #0
	blt.n	.L_0802ab6c
	cmp	r2, r4
	ble.n	.L_0802ab74
	b.n	.L_0802ab78
.L_0802ab6c:
	mov	r7, ip
	subs	r3, r7, r1
	cmp	r2, r3
	bgt.n	.L_0802ab78
.L_0802ab74:
	mov	lr, r5
	b.n	.L_0802ab88
.L_0802ab78:
	mov	ip, r1
	b.n	.L_0802ab88
.L_0802ab7c:
	adds	r2, #8
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	ldrh	r4, [r2, #0]
	cmp	r3, sl
	bne.n	.L_0802aac8
.L_0802ab88:
	ldr	r3, [sp, #0]
	mov	r1, lr
	subs	r3, #12
	str	r1, [r3, #0]
	ldr	r3, [sp, #0]
	mov	r2, ip
	subs	r3, #4
	str	r2, [r3, #0]
.L_0802ab98:
	mov	r3, ip
	mov	r5, r8
	subs	r1, r3, r5
	mov	r0, lr
	bl	.L_0802af9c
	bl	.L_0802ad84
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xff880000
	.2byte 0x0000
	.2byte 0xffc0
.L_0802abbc:
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #184]
	adds	r6, r1, #0
	mov	ip, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r3, r3, #3
	adds	r1, r1, r3
	movs	r3, #132
	lsls	r3, r3, #1
	adds	r1, r1, r3
	ldr	r3, [pc, #160]
	lsls	r0, r0, #11
	adds	r3, r3, r0
	mov	sl, r3
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	movs	r0, #127
	asrs	r3, r3, #1
	ands	r3, r0
	mov	lr, r0
	movs	r0, #30
	ands	r2, r0
	lsls	r2, r2, #5
	ldrh	r4, [r1, #42]
	mov	r8, r2
	movs	r2, #254
	lsls	r2, r2, #6
	ldrh	r5, [r1, #46]
	lsls	r7, r3, #7
	cmp	r4, r2
	beq.n	.L_0802ac16
	ldr	r2, [pc, #116]
	lsls	r3, r5, #2
	subs	r7, r7, r5
	adds	r2, r2, r3
	ands	r7, r4
	mov	ip, r2
.L_0802ac16:
	lsrs	r3, r6, #31
	ldrh	r5, [r1, #40]
	adds	r3, r6, r3
	asrs	r4, r3, #1
	mov	r3, lr
	ands	r4, r3
	ldrh	r1, [r1, #44]
	ands	r0, r6
	ands	r4, r5
	cmp	r5, #127
	beq.n	.L_0802ac34
	subs	r4, r4, r1
	lsls	r3, r1, #2
	ands	r4, r5
	add	ip, r3
.L_0802ac34:
	movs	r2, #30
	movs	r6, #0
	mov	lr, r2
.L_0802ac3a:
	adds	r3, r7, r4
	lsls	r3, r3, #2
	mov	r2, ip
	ldr	r1, [r3, r2]
	ldr	r2, [pc, #68]
	lsls	r1, r1, #21
	lsrs	r1, r1, #18
	adds	r3, r1, r2
	mov	r2, r8
	adds	r2, r2, r0
	lsls	r2, r2, #1
	ldr	r3, [r3, #0]
	mov	r9, r2
	add	r9, sl
	mov	r2, r9
	str	r3, [r2, #0]
	ldr	r2, [pc, #48]
	adds	r4, #1
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	mov	r2, r9
	str	r3, [r2, #64]
	adds	r0, #2
	mov	r3, lr
	adds	r6, #1
	ands	r4, r5
	ands	r0, r3
	cmp	r6, #15
	bls.n	.L_0802ac3a
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x06002800
	.4byte 0x02020000
	.2byte 0x0004
	.2byte 0x0202
.L_0802ac90:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #212]
	adds	r7, r1, #0
	mov	r8, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r3, r3, #3
	adds	r1, r1, r3
	ldr	r3, [pc, #196]
	lsls	r0, r0, #11
	adds	r3, r3, r0
	movs	r6, #132
	lsls	r6, r6, #1
	mov	r9, r3
	lsrs	r3, r2, #31
	adds	r1, r1, r6
	adds	r3, r2, r3
	movs	r0, #127
	ldrh	r6, [r1, #42]
	asrs	r3, r3, #1
	ands	r3, r0
	mov	sl, r0
	lsls	r4, r3, #7
	movs	r0, #254
	movs	r3, #30
	mov	lr, r6
	ands	r2, r3
	lsls	r0, r0, #6
	ldrh	r6, [r1, #46]
	mov	ip, r3
	lsls	r5, r2, #5
	cmp	lr, r0
	beq.n	.L_0802acf2
	subs	r4, r4, r6
	lsls	r3, r6, #2
	ldr	r6, [pc, #136]
	mov	r2, lr
	adds	r6, r6, r3
	ands	r4, r2
	mov	r8, r6
.L_0802acf2:
	lsrs	r3, r7, #31
	adds	r3, r7, r3
	ldrh	r2, [r1, #40]
	asrs	r0, r3, #1
	mov	fp, r7
	mov	r3, sl
	ands	r0, r3
	mov	r6, fp
	mov	r3, ip
	ands	r6, r3
	ldrh	r1, [r1, #44]
	mov	fp, r6
	ands	r0, r2
	cmp	r2, #127
	beq.n	.L_0802ad18
	subs	r0, r0, r1
	lsls	r3, r1, #2
	ands	r0, r2
	add	r8, r3
.L_0802ad18:
	movs	r6, #1
	mov	ip, r6
	mov	r2, ip
	movs	r3, #240
	ands	r2, r7
	lsls	r3, r3, #2
	mov	ip, r2
	movs	r7, #0
	mov	sl, r3
.L_0802ad2a:
	adds	r3, r4, r0
	lsls	r3, r3, #2
	mov	r6, r8
	ldr	r1, [r3, r6]
	ldr	r3, [pc, #72]
	lsls	r1, r1, #21
	lsrs	r1, r1, #19
	add	r1, ip
	lsls	r1, r1, #1
	mov	r6, fp
	adds	r2, r1, r3
	adds	r3, r5, r6
	ldrh	r2, [r2, #0]
	add	r3, ip
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r6, [pc, #52]
	strh	r2, [r3, #0]
	adds	r2, r1, r6
	ldrh	r2, [r2, #0]
	adds	r3, #64
	strh	r2, [r3, #0]
	adds	r4, #128
	mov	r2, lr
	adds	r5, #64
	mov	r3, sl
	adds	r7, #1
	ands	r4, r2
	ands	r5, r3
	cmp	r7, #10
	bls.n	.L_0802ad2a
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02010000
	.4byte 0x06002800
	.4byte 0x02020000
	.2byte 0x0004
	.2byte 0x0202
.L_0802ad84:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r6, #132
	ldr	r1, [r3, #0]
	mov	r8, r3
	lsls	r6, r6, #1
	sub	sp, #4
	add	r6, r8
	cmp	r1, #0
	bne.n	.L_0802adaa
	b.n	.L_0802af70
.L_0802adaa:
	ldmia	r1!, {r3}
	ldr	r2, [pc, #464]
	ldr	r5, [pc, #468]
	adds	r2, r2, r3
	mov	sl, r2
	ldmia	r1!, {r2}
	ldr	r3, [r1, #0]
	mov	r1, r8
	subs	r3, r3, r2
	adds	r7, r3, r5
	mov	r3, r8
	adds	r3, #236
	ldr	r1, [r1, #4]
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #448]
	adds	r0, r3, r1
	mov	r3, r8
	adds	r3, #244
	ldr	r3, [r3, #0]
	mov	ip, r1
	subs	r3, r3, r1
	adds	r1, r3, r2
	mov	r3, r8
	mov	r5, r8
	adds	r3, #240
	ldr	r4, [r5, #8]
	ldr	r3, [r3, #0]
	ldr	r5, [pc, #424]
	adds	r2, r3, r4
	mov	r3, r8
	adds	r3, #248
	ldr	r3, [r3, #0]
	subs	r3, r3, r4
	adds	r3, r3, r5
	cmp	r0, r1
	ble.n	.L_0802adf4
	adds	r1, r0, #0
.L_0802adf4:
	cmp	r2, r3
	ble.n	.L_0802adfa
	adds	r3, r2, #0
.L_0802adfa:
	cmp	sl, r0
	bge.n	.L_0802ae00
	mov	sl, r0
.L_0802ae00:
	cmp	sl, r1
	ble.n	.L_0802ae06
	mov	sl, r1
.L_0802ae06:
	cmp	r7, r2
	bge.n	.L_0802ae0c
	adds	r7, r2, #0
.L_0802ae0c:
	cmp	r7, r3
	ble.n	.L_0802ae12
	adds	r7, r3, #0
.L_0802ae12:
	mov	r1, ip
	cmp	r1, #0
	beq.n	.L_0802ae42
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	ldr	r3, [pc, #364]
	subs	r5, r5, r0
	mov	r2, r8
	adds	r1, r5, #0
	ldr	r0, [r2, #4]
	mov	r9, r3
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x4645
	add	sl, r0
	ldr	r1, [r5, #12]
	ldr	r0, [r5, #4]
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x68ac
	str	r0, [r5, #4]
.L_0802ae42:
	cmp	r4, #0
	beq.n	.L_0802ae70
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	ldr	r2, [pc, #316]
	subs	r5, r5, r0
	mov	r1, r8
	ldr	r0, [r1, #8]
	mov	r9, r2
	adds	r1, r5, #0
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x4643
	adds	r7, r7, r0
	ldr	r1, [r3, #12]
	ldr	r0, [r3, #8]
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x4645
	str	r0, [r5, #8]
.L_0802ae70:
	movs	r1, #232
	add	r1, r8
	ldr	r2, [r1, #0]
	movs	r5, #128
	subs	r3, r2, r7
	lsls	r5, r5, #13
	mov	r9, r1
	cmp	r3, r5
	ble.n	.L_0802ae88
	ldr	r1, [pc, #272]
	adds	r7, r2, r1
	subs	r3, r2, r7
.L_0802ae88:
	ldr	r5, [pc, #264]
	cmp	r3, r5
	bge.n	.L_0802ae94
	movs	r1, #128
	lsls	r1, r1, #13
	adds	r7, r2, r1
.L_0802ae94:
	movs	r2, #228
	add	r2, r8
	mov	r3, sl
	mov	r5, r9
	str	r3, [r2, #0]
	str	r7, [r5, #0]
	movs	r1, #0
	mov	fp, r2
	mov	r8, r1
.L_0802aea6:
	mov	r2, fp
	ldr	r0, [r2, #0]
	ldr	r1, [r6, #16]
	ldr	r3, [pc, #224]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x464d
	ldr	r2, [pc, #216]
	mov	sl, r0
	ldr	r1, [r6, #20]
	ldr	r0, [r5, #0]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x69b2
	adds	r7, r0, #0
	cmp	r2, #0
	beq.n	.L_0802aed0
	ldr	r3, [r6, #32]
	adds	r3, r3, r2
	str	r3, [r6, #32]
	add	sl, r3
.L_0802aed0:
	ldr	r2, [r6, #28]
	cmp	r2, #0
	beq.n	.L_0802aede
	ldr	r3, [r6, #36]
	adds	r3, r3, r2
	str	r3, [r6, #36]
	adds	r7, r7, r3
.L_0802aede:
	ldr	r3, [r6, #8]
	ldr	r1, [r6, #0]
	add	sl, r3
	ldr	r3, [r6, #12]
	mov	r2, sl
	adds	r7, r7, r3
	mov	r3, sl
	lsrs	r4, r3, #19
	adds	r3, r1, #0
	eors	r3, r2
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r2
	lsrs	r5, r7, #19
	cmp	r3, #0
	beq.n	.L_0802af1c
	cmp	r1, sl
	bge.n	.L_0802af0e
	adds	r1, r4, #0
	adds	r1, #30
	mov	r0, r8
	adds	r2, r5, #0
	str	r4, [sp, #0]
	b.n	.L_0802af16
.L_0802af0e:
	adds	r1, r4, #0
	mov	r0, r8
	adds	r2, r5, #0
	str	r4, [sp, #0]
.L_0802af16:
	bl	.L_0802ac90
	ldr	r4, [sp, #0]
.L_0802af1c:
	ldr	r1, [r6, #4]
	movs	r2, #128
	adds	r3, r1, #0
	eors	r3, r7
	lsls	r2, r2, #13
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0802af48
	cmp	r1, r7
	bge.n	.L_0802af3e
	adds	r2, r5, #0
	adds	r2, #20
	mov	r0, r8
	adds	r1, r4, #0
	bl	.L_0802abbc
	b.n	.L_0802af48
.L_0802af3e:
	mov	r0, r8
	adds	r1, r4, #0
	adds	r2, r5, #0
	bl	.L_0802abbc
.L_0802af48:
	mov	r3, sl
	mov	r5, r8
	str	r3, [r6, #0]
	movs	r3, #3
	subs	r3, r3, r5
	ldr	r5, [pc, #68]
	mov	r1, sl
	asrs	r2, r1, #16
	lsls	r3, r3, #2
	movs	r1, #1
	strh	r2, [r5, r3]
	add	r8, r1
	asrs	r2, r7, #16
	adds	r3, r3, r5
	strh	r2, [r3, #2]
	mov	r2, r8
	str	r7, [r6, #4]
	adds	r6, #56
	cmp	r2, #2
	bls.n	.L_0802aea6
.L_0802af70:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xff880000
	.4byte 0xffa00000
	.4byte 0xff100000
	.4byte 0xff600000
	.4byte 0x0300021c
	.4byte 0xfff00000
	.2byte 0x1120
	.2byte 0x0300
.L_0802af9c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	mov	r8, r0
	adds	r0, r1, #0
	movs	r1, #132
	sub	sp, #40
	lsls	r1, r1, #1
	str	r3, [sp, #24]
	adds	r7, r3, r1
	ldr	r3, [pc, #440]
	ldr	r2, [pc, #440]
	adds	r0, r0, r3
	ldr	r3, [sp, #24]
	add	r8, r2
	adds	r3, #236
	ldr	r3, [r3, #0]
	cmp	r8, r3
	bge.n	.L_0802afd2
	mov	r8, r3
.L_0802afd2:
	ldr	r3, [sp, #24]
	ldr	r4, [pc, #424]
	adds	r3, #244
	ldr	r3, [r3, #0]
	adds	r3, r3, r4
	cmp	r8, r3
	ble.n	.L_0802afe2
	mov	r8, r3
.L_0802afe2:
	ldr	r3, [sp, #24]
	adds	r3, #240
	ldr	r3, [r3, #0]
	cmp	r0, r3
	bge.n	.L_0802afee
	adds	r0, r3, #0
.L_0802afee:
	ldr	r3, [sp, #24]
	ldr	r1, [pc, #400]
	adds	r3, #248
	ldr	r3, [r3, #0]
	adds	r3, r3, r1
	cmp	r0, r3
	ble.n	.L_0802affe
	adds	r0, r3, #0
.L_0802affe:
	ldr	r2, [sp, #24]
	mov	r3, r8
	adds	r2, #228
	str	r2, [sp, #16]
	str	r3, [r2, #0]
	ldr	r4, [sp, #24]
	movs	r1, #0
	adds	r4, #232
	str	r4, [sp, #12]
	str	r0, [r4, #0]
	str	r1, [sp, #36]
.L_0802b014:
	ldr	r2, [sp, #36]
	movs	r4, #130
	ldr	r1, [sp, #24]
	lsls	r4, r4, #1
	adds	r3, r2, r4
	ldrb	r3, [r1, r3]
	cmp	r3, #0
	bne.n	.L_0802b026
	b.n	.L_0802b15e
.L_0802b026:
	ldr	r2, [sp, #16]
	ldr	r1, [r7, #16]
	ldr	r0, [r2, #0]
	ldr	r3, [pc, #344]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9c03
	ldr	r2, [pc, #336]
	mov	r8, r0
	ldr	r1, [r7, #20]
	ldr	r0, [r4, #0]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x69ba
	cmp	r2, #0
	beq.n	.L_0802b04e
	ldr	r3, [r7, #32]
	adds	r3, r3, r2
	str	r3, [r7, #32]
	add	r8, r3
.L_0802b04e:
	ldr	r2, [r7, #28]
	cmp	r2, #0
	beq.n	.L_0802b05c
	ldr	r3, [r7, #36]
	adds	r3, r3, r2
	str	r3, [r7, #36]
	adds	r0, r0, r3
.L_0802b05c:
	ldr	r3, [r7, #8]
	add	r8, r3
	ldr	r3, [r7, #12]
	mov	r1, r8
	adds	r0, r0, r3
	cmp	r1, #0
	bge.n	.L_0802b06e
	ldr	r1, [pc, #288]
	add	r1, r8
.L_0802b06e:
	asrs	r3, r1, #19
	mov	r8, r3
	adds	r2, r0, #0
	cmp	r0, #0
	bge.n	.L_0802b07c
	ldr	r4, [pc, #272]
	adds	r2, r0, r4
.L_0802b07c:
	ldr	r4, [sp, #36]
	asrs	r0, r2, #19
	lsls	r3, r4, #11
	ldr	r4, [pc, #268]
	adds	r4, r3, r4
	str	r4, [sp, #32]
	ldrh	r3, [r7, #42]
	ldrh	r4, [r7, #40]
	mov	sl, r3
	ldrh	r3, [r7, #44]
	mov	lr, r4
	ldr	r4, [pc, #256]
	ldrh	r5, [r7, #46]
	str	r3, [sp, #20]
	mov	r3, lr
	str	r4, [sp, #28]
	cmp	r3, #127
	beq.n	.L_0802b0aa
	ldr	r4, [sp, #20]
	lsls	r3, r4, #2
	ldr	r4, [pc, #236]
	adds	r4, r3, r4
	str	r4, [sp, #28]
.L_0802b0aa:
	movs	r3, #254
	lsls	r3, r3, #6
	mov	ip, r3
	cmp	sl, ip
	beq.n	.L_0802b0bc
	ldr	r4, [sp, #28]
	lsls	r3, r5, #2
	adds	r4, r4, r3
	str	r4, [sp, #28]
.L_0802b0bc:
	lsrs	r3, r2, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	movs	r2, #127
	ands	r3, r2
	lsls	r4, r3, #7
	movs	r3, #30
	ands	r3, r0
	lsls	r6, r3, #5
	cmp	sl, ip
	beq.n	.L_0802b0d8
	subs	r4, r4, r5
	mov	r2, sl
	ands	r4, r2
.L_0802b0d8:
	movs	r3, #0
	mov	r9, r3
	lsrs	r3, r1, #31
	add	r3, r8
	asrs	r3, r3, #1
	str	r3, [sp, #8]
.L_0802b0e4:
	ldr	r1, [sp, #8]
	movs	r0, #127
	movs	r5, #30
	mov	r2, r8
	mov	r3, lr
	ands	r0, r1
	ands	r5, r2
	cmp	r3, #127
	beq.n	.L_0802b0fc
	ldr	r1, [sp, #20]
	subs	r0, r0, r1
	ands	r0, r3
.L_0802b0fc:
	movs	r2, #0
	movs	r3, #30
	mov	ip, r2
	mov	fp, r3
.L_0802b104:
	ldr	r2, [sp, #28]
	adds	r3, r4, r0
	lsls	r3, r3, #2
	ldr	r1, [r3, r2]
	ldr	r2, [pc, #136]
	lsls	r1, r1, #21
	lsrs	r1, r1, #18
	str	r1, [sp, #4]
	adds	r3, r1, r2
	ldr	r1, [sp, #32]
	adds	r2, r6, r5
	lsls	r2, r2, #1
	adds	r2, r2, r1
	str	r2, [sp, #0]
	ldr	r1, [pc, #120]
	ldr	r3, [r3, #0]
	adds	r0, #1
	str	r3, [r2, #0]
	ldr	r2, [sp, #4]
	adds	r5, #2
	adds	r3, r2, r1
	ldr	r3, [r3, #0]
	ldr	r2, [sp, #0]
	mov	r1, fp
	str	r3, [r2, #64]
	movs	r2, #1
	mov	r3, lr
	add	ip, r2
	ands	r0, r3
	mov	r3, ip
	ands	r5, r1
	cmp	r3, #15
	bls.n	.L_0802b104
	movs	r3, #240
	add	r9, r2
	adds	r4, #128
	mov	r1, sl
	adds	r6, #64
	lsls	r3, r3, #2
	mov	r2, r9
	ands	r4, r1
	ands	r6, r3
	cmp	r2, #10
	bls.n	.L_0802b0e4
	adds	r7, #56
.L_0802b15e:
	ldr	r3, [sp, #36]
	adds	r3, #1
	str	r3, [sp, #36]
	cmp	r3, #2
	bhi.n	.L_0802b16a
	b.n	.L_0802b014
.L_0802b16a:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffa00000
	.4byte 0xff880000
	.4byte 0xff100000
	.4byte 0xff600000
	.4byte 0x0300021c
	.4byte 0x0007ffff
	.4byte 0x06002800
	.4byte 0x02010000
	.4byte 0x02020000
	.2byte 0x0004
	.2byte 0x0202
.L_0802b1a0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r4, r3, #0
	mov	fp, r2
	lsls	r1, r1, #7
	ldr	r2, [pc, #260]
	lsls	r3, r4, #7
	adds	r1, r1, r0
	add	r3, fp
	lsls	r1, r1, #2
	lsls	r3, r3, #2
	sub	sp, #36
	adds	r3, r3, r2
	adds	r1, r1, r2
	str	r3, [sp, #4]
	str	r1, [sp, #8]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r2, r3, r0
	add	r0, sp, #12
	mov	r9, r0
	movs	r6, #2
.L_0802b1dc:
	ldr	r3, [r2, #0]
	subs	r6, #1
	asrs	r3, r3, #20
	str	r3, [r0, #0]
	ldr	r3, [r2, #4]
	adds	r2, #56
	asrs	r3, r3, #20
	str	r3, [r0, #4]
	adds	r0, #8
	cmp	r6, #0
	bge.n	.L_0802b1dc
	ldr	r3, [sp, #72]
	adds	r7, r4, #0
	adds	r3, r7, r3
	cmp	r7, r3
	bcs.n	.L_0802b2ae
	ldr	r1, [sp, #68]
	str	r3, [sp, #0]
	movs	r3, #128
	subs	r3, r3, r1
	lsls	r3, r3, #2
	mov	r8, r3
.L_0802b208:
	ldr	r2, [sp, #68]
	mov	r1, fp
	adds	r3, r1, r2
	cmp	r1, r3
	bcs.n	.L_0802b29a
	mov	ip, r7
	mov	r4, ip
	mov	lr, r3
	movs	r3, #15
	ands	r4, r3
	mov	sl, r3
	mov	ip, r4
.L_0802b220:
	ldr	r2, [sp, #8]
	ldr	r4, [sp, #4]
	ldmia	r2!, {r5}
	movs	r3, #240
	adds	r0, r2, #0
	str	r0, [sp, #8]
	lsls	r3, r3, #4
	adds	r3, #255
	ands	r5, r3
	ldr	r2, [pc, #140]
	ldr	r3, [r4, #0]
	movs	r6, #0
	ands	r3, r2
	orrs	r3, r5
	stmia	r4!, {r3}
	adds	r2, r1, #0
	adds	r0, r4, #0
	mov	r3, sl
	mov	r4, ip
	ands	r2, r3
	lsls	r3, r4, #5
	adds	r3, r3, r2
	str	r0, [sp, #4]
	lsls	r4, r3, #2
	mov	r0, r9
.L_0802b252:
	ldr	r3, [r0, #0]
	cmp	r3, r1
	bgt.n	.L_0802b286
	adds	r3, #16
	cmp	r3, r1
	ble.n	.L_0802b286
	ldr	r3, [r0, #4]
	cmp	r3, r7
	bgt.n	.L_0802b286
	adds	r3, #12
	cmp	r3, r7
	ble.n	.L_0802b286
	lsls	r3, r5, #3
	ldr	r2, [pc, #84]
	ldr	r5, [pc, #88]
	adds	r0, r4, r2
	adds	r2, r3, r5
	ldr	r2, [r2, #0]
	str	r2, [r0, #0]
	ldr	r0, [pc, #80]
	adds	r2, r3, r0
	ldr	r3, [pc, #80]
	adds	r0, r4, r3
	ldr	r3, [r2, #0]
	str	r3, [r0, #0]
	b.n	.L_0802b294
.L_0802b286:
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r6, #1
	adds	r4, r4, r2
	adds	r0, #8
	cmp	r6, #2
	ble.n	.L_0802b252
.L_0802b294:
	adds	r1, #1
	cmp	r1, lr
	bcc.n	.L_0802b220
.L_0802b29a:
	ldr	r3, [sp, #8]
	ldr	r4, [sp, #4]
	ldr	r5, [sp, #0]
	add	r3, r8
	add	r4, r8
	adds	r7, #1
	str	r3, [sp, #8]
	str	r4, [sp, #4]
	cmp	r7, r5
	bcc.n	.L_0802b208
.L_0802b2ae:
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02010000
	.4byte 0xfffff000
	.4byte 0x06002800
	.4byte 0x02020000
	.4byte 0x02020004
	.2byte 0x2840
	.2byte 0x0600
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r7, r0, #0
	ldrh	r0, [r7, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	mov	ip, r0
	adds	r3, #255
	sub	sp, #8
	mov	sl, r1
	mov	r8, r2
	cmp	ip, r3
	beq.n	.L_0802b336
	mov	r9, r3
	adds	r6, r7, #2
.L_0802b2f8:
	movs	r2, #0
	ldrsh	r1, [r6, r2]
	movs	r4, #2
	ldrsh	r3, [r6, r4]
	movs	r4, #4
	ldrsh	r2, [r6, r4]
	movs	r4, #6
	ldrsh	r5, [r6, r4]
	lsls	r3, r3, #16
	lsls	r2, r2, #16
	lsrs	r3, r3, #16
	lsrs	r2, r2, #16
	lsls	r1, r1, #16
	lsls	r5, r5, #16
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	lsrs	r1, r1, #16
	mov	r2, sl
	mov	r3, r8
	lsrs	r5, r5, #16
	bl	.L_0802b1a0
	adds	r7, #10
	adds	r0, r5, #0
	bl	sub_08013560
	ldrh	r0, [r7, #0]
	adds	r6, #10
	mov	ip, r0
	cmp	ip, r9
	bne.n	.L_0802b2f8
.L_0802b336:
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
