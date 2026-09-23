.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_080051d8, 0x080051d8
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080b5088, 0x080b5088
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080df90c, 0x080df90c
	.set sub_080df9d0, 0x080df9d0
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e38b8, 0x080e38b8
	.set sub_080e3944, 0x080e3944
	.set sub_080e3980, 0x080e3980
	.set sub_080ed408, 0x080ed408
	.global Func_080dfa48
	.thumb_func
Func_080dfa48:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #824]
	adds	r3, r2, #0
	adds	r6, r1, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #72
	str	r3, [sp, #32]
	adds	r3, r2, #0
	subs	r3, #108
	ldr	r3, [r3, #0]
	str	r3, [sp, #20]
	ldr	r5, [pc, #804]
	mov	r9, r1
	ldr	r2, [r2, #8]
	add	r5, r9
	str	r2, [sp, #16]
	str	r0, [r5, #0]
	movs	r0, #0
	bl	sub_080cd594
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080dfaa6
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	sub_080ed408
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #11
	str	r5, [sp, #0]
	bl	sub_080ed408
	b.n	.L_080dfac4
.L_080dfaa6:
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	movs	r0, #46
	str	r5, [sp, #0]
	bl	sub_080ed408
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #15
	str	r5, [sp, #0]
	bl	sub_080ed408
.L_080dfac4:
	ldr	r3, [pc, #720]
	adds	r2, r3, #0
	adds	r2, #184
	adds	r3, #188
	ldr	r2, [r2, #0]
	ldr	r3, [r3, #0]
	str	r2, [sp, #24]
	str	r3, [sp, #28]
	ldr	r1, [sp, #16]
	ldr	r0, [pc, #708]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	ldr	r0, [pc, #700]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	movs	r3, #144
	lsls	r3, r3, #1
	mov	r0, r9
	ldr	r1, [pc, #688]
	movs	r2, #40
	bl	sub_080df9d0
	ldr	r0, [pc, #684]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	cmp	r6, #1
	beq.n	.L_080dfb1e
	cmp	r6, #1
	bgt.n	.L_080dfb14
	cmp	r6, #0
	beq.n	.L_080dfb1a
	b.n	.L_080dfb26
.L_080dfb14:
	cmp	r6, #2
	beq.n	.L_080dfb22
	b.n	.L_080dfb26
.L_080dfb1a:
	ldr	r0, [pc, #656]
	b.n	.L_080dfb28
.L_080dfb1e:
	ldr	r0, [pc, #656]
	b.n	.L_080dfb28
.L_080dfb22:
	ldr	r0, [pc, #656]
	b.n	.L_080dfb28
.L_080dfb26:
	ldr	r0, [pc, #656]
.L_080dfb28:
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #648]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	sub_080072f0
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #632]
	ldr	r5, [pc, #588]
	add	r2, r9
	movs	r3, #75
	movs	r1, #144
	str	r3, [r2, #0]
	add	r5, r9
	lsls	r1, r1, #3
	ldr	r0, [pc, #620]
	bl	sub_080041d8
	ldr	r3, [r5, #0]
	movs	r2, #36
	ldrsh	r1, [r3, r2]
	ldr	r0, [r3, #8]
	movs	r2, #10
	bl	sub_080df90c
	ldr	r3, [r5, #0]
	movs	r4, #36
	ldrsh	r0, [r3, r4]
	bl	sub_080b5098
	movs	r5, #225
	ldr	r6, [r0, #0]
	lsls	r5, r5, #7
	movs	r0, #0
	mov	r8, r0
	movs	r7, #255
	add	r5, r9
.L_080dfb80:
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	movs	r1, #160
	ldr	r3, [r6, #12]
	lsls	r1, r1, #12
	adds	r3, r3, r1
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	sub_08004458
	ldr	r3, [pc, #560]
	ands	r3, r0
	lsls	r3, r3, #11
	str	r3, [r5, #12]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #64
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #128
	ldr	r3, [r5, #0]
	lsls	r0, r0, #11
	str	r0, [r5, #20]
	cmp	r3, #0
	ble.n	.L_080dfbc2
	ldr	r3, [r5, #12]
	negs	r3, r3
	str	r3, [r5, #12]
.L_080dfbc2:
	mov	r2, r8
	lsrs	r3, r2, #31
	add	r3, r8
	asrs	r3, r3, #1
	adds	r3, #16
	str	r3, [r5, #24]
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r5, #28
	cmp	r4, #64
	bne.n	.L_080dfb80
	ldr	r3, [pc, #440]
	add	r3, r9
	ldr	r3, [r3, #0]
	mov	r2, sp
	adds	r2, #60
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	adds	r1, r2, #0
	str	r2, [sp, #12]
	bl	sub_080e3980
	ldr	r4, [pc, #416]
	movs	r0, #48
	add	r4, r9
	movs	r3, #0
	add	r0, sp
	str	r4, [sp, #8]
	mov	fp, r3
	mov	sl, r0
.L_080dfc00:
	mov	r1, fp
	cmp	r1, #14
	bgt.n	.L_080dfc58
	ldr	r2, [sp, #8]
	ldr	r3, [r2, #0]
	mov	r1, sl
	ldr	r0, [r3, #8]
	bl	sub_080e3980
	mov	r3, sl
	ldr	r2, [r3, #0]
	mov	r4, sl
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r4, #4]
	movs	r0, #40
	movs	r1, #32
	asrs	r2, r2, #1
	subs	r2, #16
	subs	r3, #48
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	ldr	r4, [sp, #24]
	mov	r1, r9
	ldr	r0, [sp, #32]
	bl	sub_080072f4
	mov	r0, sl
	ldr	r2, [r0, #0]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r0, #4]
	movs	r1, #40
	movs	r4, #32
	asrs	r2, r2, #1
	str	r1, [sp, #0]
	str	r4, [sp, #4]
	subs	r2, #16
	subs	r3, #16
	ldr	r0, [sp, #32]
	mov	r1, r9
	ldr	r4, [sp, #28]
	bl	sub_080072f4
.L_080dfc58:
	mov	r0, fp
	cmp	r0, #10
	bne.n	.L_080dfc90
	ldr	r1, [sp, #8]
	ldr	r3, [r1, #0]
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r1, #7
	movs	r3, #0
	bl	sub_080d6888
	ldr	r4, [sp, #8]
	ldr	r3, [r4, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r1, #4
	bl	sub_080b5088
	movs	r0, #134
	bl	sub_080b50e8
	ldr	r3, [pc, #320]
	movs	r2, #8
	add	r3, r9
	str	r2, [r3, #0]
.L_080dfc90:
	mov	r5, fp
	subs	r5, #8
	cmp	r5, #11
	bhi.n	.L_080dfcca
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	ldr	r4, [sp, #12]
	lsls	r1, r3, #4
	ldr	r2, [r4, #0]
	subs	r1, r1, r3
	ldr	r3, [pc, #252]
	lsls	r1, r1, #6
	mov	r0, sl
	adds	r1, r1, r3
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r0, #4]
	movs	r0, #20
	asrs	r2, r2, #1
	str	r0, [sp, #0]
	movs	r0, #48
	str	r0, [sp, #4]
	subs	r2, #16
	subs	r3, #40
	ldr	r0, [sp, #32]
	ldr	r4, [sp, #24]
	bl	sub_080072f4
.L_080dfcca:
	cmp	r5, #55
	bhi.n	.L_080dfd40
	bl	sub_080049ac
	ldr	r0, [sp, #20]
	adds	r1, r0, #0
	adds	r1, #12
	bl	sub_080051d8
	movs	r6, #225
	movs	r0, #0
	lsls	r6, r6, #7
	mov	r8, r0
	add	r7, sp, #36
	add	r6, r9
.L_080dfce8:
	ldr	r5, [r6, #24]
	cmp	r5, #0
	ble.n	.L_080dfd34
	adds	r1, r7, #0
	adds	r0, r6, #0
	bl	sub_080e3944
	asrs	r5, r5, #4
	ldr	r2, [r7, #0]
	adds	r5, #2
	lsls	r0, r5, #1
	ldr	r4, [pc, #208]
	asrs	r2, r2, #1
	str	r2, [r7, #0]
	subs	r3, r0, #2
	ldrh	r1, [r4, r3]
	ldr	r3, [sp, #16]
	adds	r1, r3, r1
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	ldr	r4, [sp, #24]
	subs	r3, r3, r5
	str	r0, [sp, #4]
	str	r5, [sp, #0]
	ldr	r0, [sp, #32]
	bl	sub_080072f4
	adds	r0, r6, #0
	movs	r1, #60
	ldr	r2, [pc, #168]
	bl	sub_080e38b8
	ldr	r3, [r6, #24]
	subs	r3, #1
	str	r3, [r6, #24]
.L_080dfd34:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r6, #28
	cmp	r1, #64
	bne.n	.L_080dfce8
.L_080dfd40:
	movs	r0, #8
	movs	r1, #8
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r2, [pc, #136]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r2, #1
	add	fp, r2
	mov	r3, fp
	cmp	r3, #60
	beq.n	.L_080dfd66
	b.n	.L_080dfc00
.L_080dfd66:
	ldr	r0, [pc, #92]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #72
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x03001e50
	.4byte 0x00000073
	.4byte 0x00000099
	.4byte 0x02010000
	.4byte 0x000000bd
	.4byte 0x000000c2
	.4byte 0x000000b9
	.4byte 0x000000bb
	.4byte 0x000000c0
	.4byte 0x03001388
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x000001ff
	.4byte 0x000077a8
	.4byte 0x080ede48
	.4byte 0xfffffe00
	.4byte 0x00007824
