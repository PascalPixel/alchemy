.syntax unified
	.thumb
	.set sub_08002064, 0x08002064
	.set sub_0801314c, 0x0801314c
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014d00, 0x08014d00
	.set sub_08016cfc, 0x08016cfc
	.set sub_08039260, 0x08039260
	.set sub_0803939c, 0x0803939c
	.set sub_080411f0, 0x080411f0
	.set sub_08041c54, 0x08041c54
	.set sub_08042188, 0x08042188
	.set sub_08042314, 0x08042314
	.set sub_08044460, 0x08044460
	.set sub_080ad028, 0x080ad028
	.set sub_080c8268, 0x080c8268
	.set sub_08108030, 0x08108030
	.set sub_08108040, 0x08108040
	.set sub_08108048, 0x08108048
	.set sub_081c0010, 0x081c0010
	.global Func_08040ed4
	.thumb_func
Func_08040ed4:
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl	sub_08016cfc
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	bl	sub_08016cfc
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #170
	bl	sub_08016cfc
	ldr	r3, [pc, #20]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #66
	adds	r3, r3, r2
	movs	r2, #141
	strh	r2, [r3, #0]
	ldr	r0, [pc, #12]
	movs	r1, #1
	bl	sub_080c8268
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x0062
	.2byte 0x0000
	push	{lr}
	movs	r0, #184
	adds	r0, #255
	bl	sub_080ad028
	movs	r0, #220
	lsls	r0, r0, #1
	bl	sub_080ad028
	movs	r0, #186
	adds	r0, #255
	bl	sub_080ad028
	movs	r0, #136
	lsls	r0, r0, #4
	adds	r0, #255
	bl	sub_08016cfc
	ldr	r3, [pc, #20]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #66
	adds	r3, r3, r2
	movs	r2, #141
	strh	r2, [r3, #0]
	ldr	r0, [pc, #12]
	movs	r1, #9
	bl	sub_080c8268
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x00b6
	.2byte 0x0000
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #254
	bl	sub_08016cfc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #76
	bl	sub_08016cfc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #225
	bl	sub_08016cfc
	movs	r0, #133
	lsls	r0, r0, #4
	adds	r0, #255
	bl	sub_08016cfc
	ldr	r0, [pc, #8]
	movs	r1, #30
	bl	sub_080c8268
	pop	{pc}
	.2byte 0x00f4
	.2byte 0x0000
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #36
	bl	sub_08016cfc
	movs	r0, #190
	lsls	r0, r0, #1
	bl	sub_08016cfc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #197
	bl	sub_08016cfc
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #61
	bl	sub_08016cfc
	movs	r0, #156
	lsls	r0, r0, #4
	bl	sub_08016cfc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #193
	bl	sub_08016cfc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #194
	bl	sub_08016cfc
	ldr	r0, [pc, #8]
	movs	r1, #1
	bl	sub_080c8268
	pop	{pc}
	movs	r0, r0
	.2byte 0x010e
	.2byte 0x0000
	push	{lr}
	ldr	r0, [pc, #8]
	movs	r1, #1
	bl	sub_080c8268
	pop	{pc}
	.2byte 0x0129
	.2byte 0x0000
	push	{lr}
	ldr	r0, [pc, #8]
	movs	r1, #20
	bl	sub_080c8268
	pop	{pc}
	.2byte 0x011e
	.2byte 0x0000
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #208
	ldr	r0, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #164
	adds	r0, r0, r3
	bl	sub_08108030
	pop	{pc}
.L_0804101c:
	push	{lr}
	movs	r1, #197
	lsls	r1, r1, #3
	movs	r0, #208
	sub	sp, #4
	bl	sub_08014d00
	movs	r3, #0
	adds	r1, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r2, [pc, #16]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #12]
	bl	sub_080145a8
	add	sp, #4
	pop	{pc}
	.4byte 0x8500018a
	.2byte 0x1005
	.2byte 0x0804
.L_08041054:
	push	{lr}
	ldr	r0, [pc, #12]
	bl	sub_08014644
	movs	r0, #208
	bl	sub_0801314c
	pop	{pc}
	.2byte 0x1005
	.2byte 0x0804
.L_08041068:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #208
	ldr	r3, [r3, #0]
	sub	sp, #4
	mov	r9, r3
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r3, #19
	movs	r1, #0
	movs	r2, #30
	bl	sub_08039260
	movs	r3, #4
	ldr	r7, [pc, #96]
	mov	sl, r3
	movs	r3, #0
	adds	r6, r0, #0
	mov	r8, r3
	movs	r5, #2
	b.n	.L_080410b2
.L_0804109e:
	adds	r2, r5, #0
	movs	r3, #29
	adds	r0, r6, #0
	movs	r1, #0
	str	r5, [sp, #0]
	bl	sub_08041c54
	movs	r3, #1
	adds	r5, #2
	add	r8, r3
.L_080410b2:
	mov	r3, sl
	ldr	r0, [r7, #0]
	adds	r1, r6, #0
	movs	r2, #8
	bl	sub_08042188
	movs	r3, #16
	add	sl, r3
	mov	r3, r8
	adds	r7, #8
	cmp	r3, #8
	bne.n	.L_0804109e
	bl	sub_08044460
	movs	r1, #128
	movs	r3, #0
	lsls	r1, r1, #23
	adds	r2, r6, #0
	str	r3, [sp, #0]
	bl	sub_08042314
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #164
	add	r3, r9
	str	r0, [r3, #0]
	add	sp, #4
	adds	r0, r6, #0
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0xa128
	.2byte 0x080a
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	bl	.L_0804101c
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #208
	ldr	r3, [r3, #0]
	movs	r0, #1
	mov	sl, r3
	bl	sub_08013560
	bl	.L_08041068
	movs	r7, #1
	negs	r7, r7
	mov	r8, r0
.L_08041120:
	cmp	r7, #0
	beq.n	.L_08041170
	adds	r0, r5, #0
	movs	r1, #9
	adds	r0, #9
	bl	sub_08002064
	mov	r1, r8
	adds	r5, r0, #0
	movs	r0, #12
	ldrsh	r3, [r1, r0]
	mov	r0, r8
	lsls	r3, r3, #3
	subs	r1, r3, #4
	movs	r2, #14
	ldrsh	r3, [r0, r2]
	lsls	r2, r5, #4
	lsls	r3, r3, #3
	adds	r3, r3, r2
	adds	r2, r3, #0
	movs	r3, #1
	negs	r3, r3
	adds	r2, #12
	cmp	r7, r3
	bne.n	.L_08041160
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r0, #164
	add	r0, sl
	bl	sub_08108048
	b.n	.L_0804116e
.L_08041160:
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r0, #164
	add	r0, sl
	movs	r3, #3
	bl	sub_08108040
.L_0804116e:
	movs	r7, #0
.L_08041170:
	movs	r0, #1
	bl	sub_08013560
	ldr	r6, [pc, #132]
	movs	r2, #2
	ldr	r3, [r6, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804118e
	movs	r0, #113
	movs	r5, #1
	bl	sub_081c0010
	negs	r5, r5
	b.n	.L_080411ca
.L_0804118e:
	ldr	r3, [r6, #4]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080411a0
	movs	r0, #112
	bl	sub_081c0010
	b.n	.L_080411ca
.L_080411a0:
	ldr	r3, [r6, #12]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080411b4
	movs	r0, #111
	subs	r5, #1
	movs	r7, #1
	bl	sub_081c0010
.L_080411b4:
	ldr	r3, [r6, #12]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08041120
	movs	r0, #111
	adds	r5, #1
	movs	r7, #1
	bl	sub_081c0010
	b.n	.L_08041120
.L_080411ca:
	mov	r0, r8
	movs	r1, #2
	bl	sub_0803939c
	bl	.L_08041054
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #1
	negs	r0, r0
	cmp	r5, r0
	beq.n	sub_080411f0
	ldr	r3, [pc, #24]
	lsls	r2, r5, #3
	adds	r2, #4
	ldr	r0, [r3, r2]
	mov	lr, r0
	.2byte 0xf800
	.2byte 0x1c28
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03001150
	.4byte 0x080aa128
