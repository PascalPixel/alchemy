.syntax unified
	.thumb
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xa12c
	.2byte 0x0200
	.global Func_02000040
	.thumb_func
Func_02000040:
	movs	r0, #0
	bx	lr
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xa15c
	.2byte 0x0200
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xa1ec
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r1, #0
	bl 0x02009fcc
	movs	r0, #0
	bl 0x0200a0cc
	movs	r0, #158
	bl 0x0200a124
	ldrh	r1, [r5, #4]
	ldrh	r2, [r5, #6]
	ldr	r0, [r5, #0]
	bl 0x02009fb4
	ldr	r5, [pc, #76]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x02009fe4
	movs	r3, #2
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r2, r2, #7
	lsls	r1, r1, #8
	bl 0x02009fec
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x0200a02c
	movs	r2, #8
	movs	r1, #2
	negs	r2, r2
	ldr	r0, [r5, #0]
	bl 0x0200a014
	movs	r0, #10
	bl 0x02009fc4
	adds	r0, r6, #0
	bl 0x0200a0b4
	bl 0x0200a0bc
	bl 0x0200a0c4
	bl 0x02009fd4
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	adds	r1, r0, #0
	ldr	r0, [pc, #20]
	cmp	r1, #3
	bne.n	.L_020000d4
	adds	r0, #8
.L_020000d4:
	cmp	r1, #16
	bne.n	.L_020000da
	ldr	r0, [pc, #12]
.L_020000da:
	movs	r2, #0
	bl 0x02008054
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200a368
	.2byte 0xa378
	.2byte 0x0200
	.global Func_020000ec
	.thumb_func
Func_020000ec:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xa380
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #72]
	movs	r1, #6
	ldr	r0, [r3, #0]
	bl 0x02009f3c
	cmp	r0, #0
	bne.n	.L_02000126
	ldr	r3, [pc, #60]
	movs	r0, #14
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	asrs	r1, r3, #16
.L_0200010e:
	ldr	r4, [pc, #56]
	lsls	r3, r0, #1
	adds	r2, r3, r4
	subs	r4, #2
	adds	r3, r3, r4
	ldrh	r3, [r3, #0]
	subs	r0, #1
	strh	r3, [r2, #0]
	cmp	r0, #1
	bne.n	.L_0200010e
	ldr	r3, [pc, #40]
	strh	r1, [r3, #0]
.L_02000126:
	ldr	r3, [pc, #24]
	movs	r1, #12
	ldr	r0, [r3, #0]
	ldr	r5, [pc, #32]
	lsrs	r0, r0, #2
	bl 0x02009f3c
	lsls	r0, r0, #1
	ldrsh	r1, [r5, r0]
	ldr	r3, [pc, #24]
	strh	r1, [r3, #0]
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x0300122c
	.4byte 0x0500019c
	.4byte 0x05000180
	.4byte 0x05000182
	.4byte 0x0200a608
	.2byte 0x019e
	.2byte 0x0500
	.global Func_02000158
	.thumb_func
Func_02000158:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	movs	r2, #129
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	adds	r3, r3, r1
	adds	r2, #255
	movs	r1, #200
	str	r2, [r3, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #88]
	bl 0x02009f4c
	movs	r1, #3
	movs	r0, #8
	bl 0x0200a084
	ldr	r2, [pc, #76]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #6
	bne.n	.L_020001a6
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r2, r1
	ldr	r0, [r3, #0]
	bl 0x02009fe4
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
.L_020001a6:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #106
	bl 0x02009f8c
	cmp	r0, #0
	beq.n	.L_020001c8
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a024
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a024
.L_020001c8:
	movs	r0, #0
	pop	{pc}
	.4byte 0x020080f5
	.2byte 0x0240
	.2byte 0x0200
	.global Func_020001d4
	.thumb_func
Func_020001d4:
	movs	r0, #0
	bx	lr
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #13
	bl 0x02009fe4
	movs	r1, #5
	adds	r6, r0, #0
	movs	r0, #13
	bl 0x0200a02c
	movs	r0, #28
	bl 0x02009fc4
	movs	r0, #112
	bl 0x0200a124
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #12]
	movs	r3, #128
	lsls	r3, r3, #13
	movs	r7, #160
	movs	r0, #128
	mov	fp, r3
	lsls	r7, r7, #12
	lsls	r0, r0, #2
	adds	r1, r1, r7
	add	r2, fp
	ldr	r3, [r6, #16]
	adds	r0, #162
	bl 0x0200a0fc
	movs	r2, #128
	adds	r5, r0, #0
	lsls	r2, r2, #10
	str	r2, [r5, #48]
	str	r2, [r5, #52]
	mov	r8, r2
	movs	r2, #0
	mov	r9, r2
	adds	r3, r5, #0
	mov	r2, r9
	adds	r3, #85
	strb	r2, [r3, #0]
	movs	r3, #192
	ldr	r1, [r5, #8]
	lsls	r3, r3, #11
	adds	r1, r1, r3
	ldr	r2, [r5, #12]
	movs	r3, #128
	lsls	r3, r3, #12
	mov	sl, r3
	add	r2, sl
	ldr	r3, [r5, #16]
	bl 0x02009fa4
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #12]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r1, r1, r7
	add	r2, fp
	ldr	r3, [r6, #16]
	adds	r0, #162
	bl 0x0200a0fc
	adds	r6, r0, #0
	mov	r2, r8
	adds	r3, r6, #0
	str	r2, [r6, #48]
	str	r2, [r6, #52]
	adds	r3, #85
	mov	r2, r9
	strb	r2, [r3, #0]
	ldr	r3, [pc, #56]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	add	r1, sl
	adds	r2, r2, r3
	ldr	r3, [r5, #16]
	bl 0x02009fa4
	adds	r0, r5, #0
	bl 0x02009fac
	adds	r0, r5, #0
	bl 0x02009f9c
	adds	r0, r6, #0
	bl 0x02009fac
	adds	r0, r6, #0
	bl 0x02009f9c
	movs	r0, #30
	bl 0x02009fc4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xfffc
	.2byte 0xb560
	ldr	r3, [pc, #200]
	sub	sp, #12
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #1
	bne.n	.L_020002e8
	ldr	r4, [pc, #188]
	ldr	r1, [pc, #192]
	ldr	r0, [r4, #0]
	movs	r2, #15
	asrs	r3, r0, #3
	ands	r3, r2
	ldrb	r1, [r1, r3]
	lsls	r2, r1, #8
	movs	r3, #16
	subs	r3, r3, r1
	orrs	r2, r3
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	strh	r2, [r3, #0]
	adds	r0, #1
	str	r0, [r4, #0]
	b.n	.L_02000338
.L_020002e8:
	cmp	r3, #2
	bne.n	.L_02000338
	ldr	r6, [pc, #148]
	movs	r1, #28
	ldr	r0, [r6, #0]
	asrs	r0, r0, #2
	bl 0x02009f34
	ldr	r2, [pc, #144]
	adds	r5, r0, #0
	lsls	r3, r5, #1
	ldrh	r2, [r2, r3]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	strh	r2, [r3, #0]
	cmp	r5, #0
	bne.n	.L_0200031c
	ldr	r3, [pc, #128]
	movs	r2, #1
	str	r2, [r3, #0]
	ldr	r3, [pc, #128]
	movs	r0, #153
	str	r5, [r3, #0]
	bl 0x0200a124
.L_0200031c:
	cmp	r5, #14
	bne.n	.L_02000332
	ldr	r2, [pc, #108]
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r2, [pc, #108]
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #153
	bl 0x0200a124
.L_02000332:
	ldr	r3, [r6, #0]
	adds	r3, #1
	str	r3, [r6, #0]
.L_02000338:
	movs	r3, #208
	mov	r5, sp
	lsls	r3, r3, #18
	str	r3, [r5, #0]
	movs	r3, #0
	str	r3, [r5, #4]
	movs	r3, #224
	lsls	r3, r3, #17
	str	r3, [r5, #8]
	adds	r0, r5, #0
	bl 0x0200a0d4
	ldr	r3, [pc, #60]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02000366
	ldr	r3, [r5, #0]
	ldr	r0, [pc, #60]
	str	r3, [r0, #12]
	ldr	r3, [r5, #8]
	str	r3, [r0, #16]
	bl 0x0200a10c
.L_02000366:
	ldr	r3, [pc, #44]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0200037c
	ldr	r3, [r5, #0]
	ldr	r0, [pc, #40]
	str	r3, [r0, #12]
	ldr	r3, [r5, #8]
	str	r3, [r0, #16]
	bl 0x0200a10c
.L_0200037c:
	add	sp, #12
	pop	{r5, r6, pc}
	.4byte 0x0200b08c
	.4byte 0x0200b098
	.4byte 0x0200afd7
	.4byte 0x0200afe8
	.4byte 0x0200b090
	.4byte 0x0200b024
	.4byte 0x0200b060
	.2byte 0xb030
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #128
	lsls	r0, r0, #5
	sub	sp, #4
	bl 0x02009f5c
	mov	r9, r0
	movs	r2, #253
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #6
	adds	r3, #80
	strh	r2, [r3, #0]
	adds	r3, #2
	movs	r1, #128
	lsls	r1, r1, #5
	strh	r1, [r3, #0]
	ldr	r0, [pc, #228]
	movs	r7, #32
	mov	r1, r9
	bl 0x02009f6c
	ldr	r5, [pc, #220]
	bl 0x02009f84
	movs	r1, #128
	mov	r2, r9
	str	r0, [r5, #0]
	lsls	r1, r1, #5
	ldr	r5, [pc, #212]
	bl 0x02009f7c
	ldr	r6, [pc, #208]
	movs	r3, #192
	str	r0, [r5, #0]
	movs	r1, #0
	str	r0, [sp, #0]
	movs	r2, #0
	adds	r0, r6, #0
	lsls	r3, r3, #24
	bl 0x0200a104
	movs	r3, #1
	strh	r3, [r6, #30]
	movs	r5, #13
	ldrb	r3, [r6, #9]
	negs	r5, r5
	adds	r2, r5, #0
	ands	r2, r3
	movs	r3, #8
	mov	r8, r3
	mov	r1, r8
	orrs	r2, r1
	ldrb	r1, [r6, #5]
	adds	r3, r5, #0
	ands	r3, r1
	movs	r1, #4
	mov	sl, r1
	mov	r1, sl
	orrs	r3, r1
	orrs	r3, r7
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r2, r3
	strb	r2, [r6, #9]
	mov	r1, r9
	ldr	r0, [pc, #144]
	mov	fp, r3
	bl 0x02009f6c
	ldr	r6, [pc, #140]
	bl 0x02009f84
	movs	r1, #128
	mov	r2, r9
	str	r0, [r6, #0]
	lsls	r1, r1, #5
	ldr	r6, [pc, #132]
	bl 0x02009f7c
	str	r0, [r6, #0]
	ldr	r6, [pc, #128]
	movs	r3, #192
	str	r0, [sp, #0]
	movs	r1, #0
	adds	r0, r6, #0
	movs	r2, #0
	lsls	r3, r3, #24
	bl 0x0200a104
	ldrb	r2, [r6, #9]
	movs	r3, #2
	strh	r3, [r6, #30]
	adds	r3, r5, #0
	mov	r1, r8
	ands	r3, r2
	ldrb	r2, [r6, #5]
	orrs	r3, r1
	mov	r1, fp
	ands	r3, r1
	strb	r3, [r6, #9]
	ands	r5, r2
	ldr	r3, [pc, #88]
	mov	r2, sl
	orrs	r5, r2
	orrs	r5, r7
	movs	r2, #0
	strb	r5, [r6, #5]
	str	r2, [r3, #0]
	ldr	r3, [pc, #80]
	movs	r1, #0
	str	r2, [r3, #0]
	ldr	r3, [pc, #76]
	ldr	r0, [pc, #80]
	str	r2, [r3, #0]
	ldr	r3, [pc, #80]
	strb	r1, [r3, #0]
	movs	r1, #144
	lsls	r1, r1, #3
	bl 0x02009f4c
	mov	r0, r9
	bl 0x02009f64
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200a620
	.4byte 0x0200b020
	.4byte 0x0200b088
	.4byte 0x0200b060
	.4byte 0x0200aaf8
	.4byte 0x0200b094
	.4byte 0x0200b058
	.4byte 0x0200b030
	.4byte 0x0200b090
	.4byte 0x0200b024
	.4byte 0x0200b098
	.4byte 0x020082b5
	.2byte 0xb08c
	.2byte 0x0200
	push	{lr}
	ldr	r0, [pc, #24]
	bl 0x02009f54
	ldr	r3, [pc, #20]
	ldr	r0, [r3, #0]
	bl 0x02009f74
	ldr	r3, [pc, #16]
	ldr	r0, [r3, #0]
	bl 0x02009f74
	pop	{pc}
	.2byte 0x0000
	.4byte 0x020082b5
	.4byte 0x0200b020
	.2byte 0xb094
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #5
	adds	r3, #82
	strh	r2, [r3, #0]
	ldr	r2, [pc, #44]
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r5, #0
.L_02000526:
	movs	r7, #16
	subs	r3, r7, r5
	movs	r6, #128
	lsls	r3, r3, #8
	lsls	r6, r6, #19
	orrs	r3, r5
	adds	r6, #82
	strh	r3, [r6, #0]
	movs	r0, #4
	adds	r5, #1
	bl 0x02009f44
	cmp	r5, #15
	ble.n	.L_02000526
	strh	r7, [r6, #0]
	ldr	r2, [pc, #4]
	movs	r3, #1
	strb	r3, [r2, #0]
	pop	{r5, r6, r7, pc}
	.2byte 0xb08c
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r2, #16
	adds	r3, #82
	strh	r2, [r3, #0]
	ldr	r2, [pc, #40]
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r5, #0
.L_02000564:
	movs	r3, #16
	movs	r6, #128
	lsls	r2, r5, #8
	subs	r3, r3, r5
	lsls	r6, r6, #19
	orrs	r2, r3
	adds	r6, #82
	strh	r2, [r6, #0]
	movs	r0, #3
	adds	r5, #1
	bl 0x02009f44
	cmp	r5, #15
	ble.n	.L_02000564
	movs	r3, #128
	lsls	r3, r3, #5
	strh	r3, [r6, #0]
	pop	{r5, r6, pc}
	.2byte 0xb08c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	bl 0x02009fcc
	movs	r0, #0
	bl 0x0200a0cc
	ldr	r0, [pc, #140]
	bl 0x0200a05c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #16
	ldr	r1, [pc, #132]
	adds	r2, #153
	bl 0x02009fec
	movs	r1, #24
	movs	r2, #0
	movs	r0, #16
	negs	r1, r1
	bl 0x0200a0ec
	movs	r1, #160
	movs	r0, #16
	lsls	r1, r1, #7
	bl 0x0200a07c
	ldr	r3, [pc, #108]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r1, [r5, #0]
	movs	r2, #0
	movs	r0, #16
	bl 0x0200a054
	movs	r1, #0
	movs	r0, #16
	bl 0x0200a064
	movs	r0, #4
	movs	r1, #0
	bl 0x02009fdc
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_0200063c
	movs	r0, #30
	bl 0x02009fc4
	movs	r0, #16
	movs	r1, #4
	bl 0x0200a034
	movs	r0, #16
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #16
	bl 0x0200a0ec
	movs	r0, #16
	movs	r1, #24
	movs	r2, #0
	bl 0x0200a0ec
	movs	r1, #160
	movs	r0, #16
	lsls	r1, r1, #7
	bl 0x0200a07c
	bl 0x02009fd4
	bl 0x02009f20
	cmp	r0, #4
	movs	r0, r0
	adds	r3, #51
	movs	r1, r0
	lsls	r0, r0, #9
	lsls	r0, r0, #8
.L_0200063c:
	movs	r1, #3
	movs	r0, #16
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #16
	movs	r1, #24
	movs	r2, #0
	bl 0x0200a0ec
	movs	r1, #160
	movs	r2, #0
	movs	r0, #16
	lsls	r1, r1, #7
	bl 0x0200a074
	ldr	r1, [r5, #0]
	movs	r0, #16
	bl 0x0200a0f4
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #106
	bl 0x02009f94
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #107
	bl 0x02009f94
	movs	r0, #7
	bl 0x02009fbc
	movs	r0, #64
	bl 0x02009fe4
	movs	r1, #0
	str	r7, [r0, #28]
	movs	r2, #0
	movs	r0, #14
	bl 0x0200a074
	movs	r1, #179
	movs	r2, #178
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #13
	adds	r1, #51
	adds	r2, #153
	bl 0x02009fec
	movs	r1, #179
	movs	r2, #178
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #14
	adds	r1, #51
	adds	r2, #153
	bl 0x02009fec
	movs	r1, #179
	movs	r2, #178
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #7
	adds	r1, #51
	adds	r2, #153
	bl 0x02009fec
	movs	r1, #218
	movs	r2, #132
	movs	r0, #4
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	bl 0x0200a00c
	movs	r0, #218
	movs	r1, #1
	movs	r2, #204
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #18
	bl 0x0200a0a4
	bl 0x0200a0ac
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #160
	movs	r0, #16
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #128
	movs	r2, #45
	lsls	r1, r1, #1
	movs	r0, #13
	bl 0x0200a08c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #13
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #3
	movs	r0, #13
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #128
	adds	r3, #1
	lsls	r0, r0, #6
	strh	r3, [r2, #0]
	adds	r0, #13
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #192
	movs	r2, #0
	movs	r0, #14
	lsls	r1, r1, #6
	bl 0x0200a074
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #15
	bl 0x0200a07c
	movs	r0, #30
	bl 0x02009fc4
	movs	r1, #0
	movs	r0, #14
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #3
	movs	r0, #14
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r2, #5
	movs	r0, #14
	movs	r1, #0
	bl 0x0200a06c
	movs	r0, #13
	movs	r1, #4
	bl 0x0200a034
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #2
	movs	r0, #15
	bl 0x0200a04c
	movs	r0, #5
	bl 0x02009fc4
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #8
	bl 0x0200a07c
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #15
	bl 0x0200a08c
	movs	r0, #15
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r1, #224
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a074
	movs	r2, #0
	movs	r0, #13
	movs	r1, #0
	bl 0x0200a074
	movs	r1, #0
	movs	r0, #14
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #13
	bl 0x0200a08c
	movs	r2, #5
	movs	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r0, #15
	movs	r1, #2
	bl 0x0200a04c
	movs	r2, #5
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #3
	movs	r0, #13
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r2, #5
	movs	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #13
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #3
	movs	r0, #13
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r0, #13
	movs	r1, #1
	bl 0x0200a09c
	movs	r1, #216
	movs	r2, #222
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	movs	r0, #13
	bl 0x0200a004
	movs	r0, #20
	bl 0x02009fc4
	movs	r1, #212
	movs	r2, #222
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	movs	r0, #14
	bl 0x0200a004
	movs	r0, #20
	bl 0x02009fc4
	movs	r1, #208
	movs	r2, #222
	lsls	r2, r2, #1
	lsls	r1, r1, #2
	movs	r0, #7
	bl 0x0200a004
	movs	r0, #13
	bl 0x0200a01c
	movs	r0, #13
	movs	r1, #1
	bl 0x0200a02c
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #13
	bl 0x0200a074
	movs	r0, #14
	bl 0x0200a01c
	movs	r0, #14
	movs	r1, #1
	bl 0x0200a02c
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #14
	bl 0x0200a074
	movs	r0, #7
	bl 0x0200a01c
	movs	r0, #7
	movs	r1, #1
	bl 0x0200a02c
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #7
	bl 0x0200a07c
	movs	r0, #218
	movs	r1, #1
	movs	r2, #248
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r0, r0, #18
	bl 0x0200a0a4
	bl 0x0200a0ac
	movs	r0, #5
	bl 0x02009fc4
	movs	r3, #176
	movs	r0, #17
	movs	r1, #16
	movs	r2, #0
	lsls	r3, r3, #8
	bl 0x0200a0dc
	movs	r1, #16
	movs	r3, #192
	movs	r0, #5
	negs	r1, r1
	movs	r2, #0
	lsls	r3, r3, #8
	bl 0x0200a0dc
	movs	r1, #32
	movs	r3, #192
	lsls	r3, r3, #8
	movs	r0, #6
	negs	r1, r1
	movs	r2, #0
	bl 0x0200a0dc
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x0200a074
	movs	r0, #6
	bl 0x0200a01c
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #3
	movs	r0, #13
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #0
	movs	r0, #7
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #3
	movs	r0, #7
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #7
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #7
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r1, #160
	movs	r2, #0
	movs	r0, #14
	lsls	r1, r1, #7
	bl 0x0200a074
	movs	r1, #160
	movs	r0, #13
	lsls	r1, r1, #7
	bl 0x0200a07c
	movs	r2, #8
	negs	r2, r2
	movs	r1, #0
	movs	r0, #6
	bl 0x0200a0ec
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #3
	movs	r0, #6
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #7
	bl 0x0200a07c
	movs	r0, #7
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r1, #192
	movs	r2, #0
	movs	r0, #14
	lsls	r1, r1, #6
	bl 0x0200a074
	movs	r1, #160
	movs	r0, #13
	lsls	r1, r1, #7
	bl 0x0200a07c
	movs	r2, #8
	negs	r2, r2
	movs	r1, #0
	movs	r0, #5
	bl 0x0200a0ec
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #3
	movs	r0, #5
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #6
	bl 0x0200a07c
	movs	r0, #7
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r1, #192
	movs	r2, #0
	movs	r0, #14
	lsls	r1, r1, #6
	bl 0x0200a074
	movs	r1, #192
	movs	r0, #13
	lsls	r1, r1, #6
	bl 0x0200a07c
	movs	r2, #8
	negs	r2, r2
	movs	r1, #0
	movs	r0, #4
	bl 0x0200a0ec
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #3
	movs	r0, #4
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #6
	bl 0x0200a07c
	movs	r0, #7
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r1, #192
	movs	r2, #0
	movs	r0, #14
	lsls	r1, r1, #6
	bl 0x0200a074
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #13
	bl 0x0200a07c
	movs	r0, #17
	bl 0x02009fe4
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	mov	sl, r3
	ands	r3, r2
	movs	r2, #8
	strb	r3, [r0, #0]
	movs	r1, #0
	negs	r2, r2
	movs	r0, #17
	bl 0x0200a0ec
	movs	r0, #1
	bl 0x02009fc4
	movs	r0, #17
	bl 0x02009fe4
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r2, #1
	mov	r9, r2
	mov	r2, r9
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #3
	movs	r0, #17
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #0
	movs	r0, #14
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r2, #5
	movs	r0, #14
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #13
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #3
	movs	r0, #13
	bl 0x0200a034
	movs	r0, #20
	bl 0x02009fc4
	movs	r1, #192
	movs	r2, #0
	movs	r0, #14
	lsls	r1, r1, #6
	bl 0x0200a074
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #13
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #13
	bl 0x0200a08c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r0, #17
	movs	r1, #3
	bl 0x0200a044
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #17
	bl 0x0200a094
	movs	r0, #37
	bl 0x02009fc4
	movs	r2, #5
	movs	r0, #17
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #3
	movs	r0, #13
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a06c
	movs	r2, #5
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r0, #13
	bl 0x0200a07c
	movs	r0, #15
	bl 0x02009fc4
	movs	r2, #5
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #13
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #13
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r1, #6
	adds	r1, #255
	movs	r2, #30
	movs	r0, #13
	bl 0x0200a08c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #2
	movs	r0, #13
	bl 0x0200a04c
	movs	r0, #5
	bl 0x02009fc4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a06c
	movs	r2, #5
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #2
	movs	r0, #7
	bl 0x0200a04c
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r0, #7
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #7
	movs	r1, #4
	bl 0x0200a034
	movs	r2, #5
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r0, #13
	bl 0x0200a07c
	movs	r0, #5
	bl 0x02009fc4
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r0, #13
	movs	r1, #4
	bl 0x0200a034
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r0, #17
	movs	r1, #2
	bl 0x0200a04c
	movs	r0, #17
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #192
	movs	r2, #0
	movs	r0, #14
	lsls	r1, r1, #6
	bl 0x0200a074
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #13
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #3
	movs	r0, #13
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #13
	bl 0x020081d8
	movs	r1, #132
	lsls	r1, r1, #6
	adds	r1, #8
	movs	r0, #32
	.2byte 0xf001
	.2byte 0xfa20
	movs	r0, #32
	bl 0x02009fc4
	movs	r0, #20
	bl 0x02009fc4
	bl 0x020083a0
	ldr	r5, [pc, #760]
	ldr	r6, [pc, #764]
	movs	r3, #1
	str	r3, [r6, #0]
	movs	r0, #153
	str	r7, [r5, #0]
	mov	r8, r3
	.2byte 0xf001
	.2byte 0xfa17
	bl 0x02008510
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #4
	bl 0x0200a08c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #17
	bl 0x0200a08c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #6
	bl 0x0200a08c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #55
	movs	r0, #5
	bl 0x0200a08c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #13
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r0, #15
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a06c
	movs	r1, #6
	adds	r1, #255
	movs	r2, #50
	movs	r0, #17
	bl 0x0200a08c
	movs	r0, #13
	bl 0x020081d8
	bl 0x02008550
	movs	r0, #30
	bl 0x02009fc4
	mov	r2, r8
	movs	r0, #153
	str	r7, [r6, #0]
	str	r2, [r5, #0]
	.2byte 0xf001
	.2byte 0xf9dd
	bl 0x02008510
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #13
	movs	r1, #0
	movs	r2, #10
	.2byte 0xf001
	.2byte 0xf978
	movs	r1, #0
	movs	r2, #5
	movs	r0, #14
	.2byte 0xf001
	.2byte 0xf973
	movs	r0, #17
	bl 0x02009fe4
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r2, sl
	ands	r2, r3
	strb	r2, [r0, #0]
	mov	sl, r2
	movs	r2, #16
	movs	r1, #0
	negs	r2, r2
	movs	r0, #17
	.2byte 0xf001
	.2byte 0xf9a4
	movs	r0, #1
	bl 0x02009fc4
	movs	r0, #17
	bl 0x02009fe4
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r2, r9
	orrs	r2, r3
	strb	r2, [r0, #0]
	movs	r0, #25
	mov	r9, r2
	bl 0x02009fc4
	movs	r0, #17
	movs	r1, #3
	.2byte 0xf001
	.2byte 0xf93d
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #17
	.2byte 0xf001
	.2byte 0xf960
	movs	r0, #40
	bl 0x02009fc4
	movs	r0, #17
	movs	r1, #0
	movs	r2, #5
	.2byte 0xf001
	.2byte 0xf944
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #13
	.2byte 0xf001
	.2byte 0xf94e
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #13
	movs	r1, #0
	.2byte 0xf001
	.2byte 0xf937
	movs	r1, #0
	movs	r0, #6
	.2byte 0xf001
	.2byte 0xf93b
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #6
	.2byte 0xf001
	.2byte 0xf93a
	movs	r0, #6
	movs	r1, #0
	movs	r2, #5
	.2byte 0xf001
	.2byte 0xf925
	movs	r2, #16
	movs	r0, #17
	movs	r1, #0
	bl 0x0200a0ec
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #17
	.2byte 0xf001
	.2byte 0xf923
	movs	r0, #10
	.2byte 0xf001
	.2byte 0xf8c4
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #17
	.2byte 0xf001
	.2byte 0xf922
	movs	r0, #17
	movs	r1, #0
	movs	r2, #5
	.2byte 0xf001
	.2byte 0xf90d
	movs	r1, #128
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #5
	.2byte 0xf001
	.2byte 0xf917
	movs	r1, #0
	movs	r0, #5
	.2byte 0xf001
	.2byte 0xf90b
	movs	r0, #5
	.2byte 0xf001
	.2byte 0xf8ac
	movs	r2, #5
	movs	r0, #5
	movs	r1, #0
	.2byte 0xf001
	.2byte 0xf8fb
	movs	r0, #14
	movs	r1, #2
	.2byte 0xf001
	.2byte 0xf8e7
	movs	r0, #14
	movs	r1, #0
	movs	r2, #5
	.2byte 0xf001
	.2byte 0xf8f2
	movs	r1, #176
	movs	r0, #17
	lsls	r1, r1, #8
	movs	r2, #0
	.2byte 0xf001
	.2byte 0xf8f0
	movs	r1, #192
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200a074
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #6
	.2byte 0xf001
	.2byte 0xf8e9
	movs	r0, #20
	.2byte 0xf001
	.2byte 0xf88a
	movs	r1, #3
	movs	r0, #13
	.2byte 0xf001
	.2byte 0xf8be
	movs	r0, #10
	.2byte 0xf001
	.2byte 0xf883
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	movs	r2, #5
	adds	r0, #13
	bl 0x0200a06c
	movs	r0, #13
	bl 0x020081d8
	bl 0x02008550
	movs	r0, #10
	.2byte 0xf001
	.2byte 0xf874
	ldr	r3, [pc, #264]
	ldr	r2, [pc, #268]
	str	r7, [r3, #0]
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r0, #150
	.2byte 0xf001
	.2byte 0xf86c
	movs	r0, #17
	movs	r1, #3
	bl 0x0200a044
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #17
	bl 0x0200a094
	movs	r0, #40
	.2byte 0xf001
	.2byte 0xf860
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a06c
	movs	r0, #15
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #15
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009fec
	movs	r1, #222
	movs	r2, #222
.L_02000f2a:
	lsls	r2, r2, #1
	movs	r0, #15
	lsls	r1, r1, #2
	bl 0x0200a00c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #15
	bl 0x0200a07c
	movs	r0, #5
	.2byte 0xf001
	.2byte 0xf840
	movs	r0, #218
	movs	r1, #1
	movs	r2, #236
	movs	r3, #1
	lsls	r2, r2, #17
	negs	r1, r1
	lsls	r0, r0, #18
	bl 0x0200a0a4
	bl 0x0200a0ac
	movs	r0, #10
	.2byte 0xf001
	.2byte 0xf832
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #15
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r2, #5
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #0
	movs	r0, #13
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #13
	movs	r1, #4
	bl 0x0200a034
	movs	r2, #5
	movs	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #2
	movs	r0, #15
	bl 0x0200a04c
	movs	r0, #5
	bl 0x02009fc4
	movs	r0, #15
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r1, #10
	movs	r2, #30
	adds	r1, #255
	movs	r0, #13
	bl 0x0200a08c
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #13
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #13
	bl 0x020081d8
	movs	r0, #153
	bl 0x0200a124
	movs	r5, #0
	b.n	.L_02000ff2
	.2byte 0x0000
	.4byte 0x0200b024
	.4byte 0x0200b090
	.4byte 0x0200b098
	.2byte 0xb08c
	.2byte 0x0200
.L_02000ff0:
	adds	r5, #1
.L_02000ff2:
	cmp	r5, #119
	bgt.n	.L_0200100c
	movs	r0, #1
	bl 0x02009f44
	ldr	r3, [pc, #948]
	movs	r1, #14
	ldr	r0, [r3, #0]
	asrs	r0, r0, #2
	bl 0x02009f34
	cmp	r0, #0
	bne.n	.L_02000ff0
.L_0200100c:
	bl 0x020084e8
	movs	r0, #32
	bl 0x0200a11c
	movs	r0, #40
	bl 0x02009fc4
	movs	r1, #0
	movs	r0, #13
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r2, #5
	movs	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #2
	movs	r0, #13
	bl 0x0200a04c
	movs	r0, #5
	bl 0x02009fc4
	movs	r2, #5
	movs	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #3
	movs	r0, #15
	bl 0x0200a034
	movs	r0, #15
	bl 0x02009fc4
	movs	r1, #128
	movs	r2, #0
	movs	r0, #14
	lsls	r1, r1, #8
	bl 0x0200a074
	movs	r1, #254
	lsls	r1, r1, #7
	adds	r1, #255
	movs	r0, #13
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #13
	movs	r1, #0
.L_02001082:
	bl 0x0200a06c
	movs	r1, #0
	movs	r0, #7
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #3
	movs	r0, #7
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #7
	movs	r1, #0
	movs	r2, #5
.L_020010a8:
	bl 0x0200a06c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #13
	bl 0x0200a08c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r0, #7
	movs	r1, #4
	bl 0x0200a034
	movs	r0, #7
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #50
	movs	r0, #13
	bl 0x0200a08c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #7
	bl 0x0200a08c
	movs	r0, #7
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #14
	bl 0x0200a08c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #50
	movs	r0, #13
	bl 0x0200a08c
	movs	r2, #0
	movs	r0, #14
	movs	r1, #0
	bl 0x0200a074
	movs	r1, #0
	movs	r0, #13
	bl 0x0200a07c
	movs	r0, #20
	bl 0x02009fc4
	movs	r0, #15
	movs	r1, #2
	bl 0x0200a04c
	movs	r2, #5
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #2
	movs	r0, #17
	bl 0x0200a04c
	movs	r0, #5
	bl 0x02009fc4
	movs	r0, #17
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #160
	movs	r0, #15
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #192
	movs	r0, #14
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #192
	movs	r0, #13
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a074
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a074
	movs	r2, #0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a074
	movs	r1, #0
	movs	r0, #4
	bl 0x0200a07c
	movs	r0, #25
	bl 0x02009fc4
	movs	r1, #128
	lsls	r1, r1, #8
.L_020011aa:
	movs	r0, #17
	bl 0x0200a07c
	movs	r0, #20
	bl 0x02009fc4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #17
	bl 0x0200a08c
	movs	r2, #5
	movs	r0, #17
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r0, #17
	bl 0x0200a07c
	movs	r0, #15
	bl 0x02009fc4
	movs	r1, #2
	movs	r0, #17
	bl 0x0200a04c
	movs	r0, #10
	bl 0x02009fc4
	movs	r2, #5
	movs	r0, #17
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #2
	movs	r0, #14
	bl 0x0200a04c
	movs	r0, #5
	bl 0x02009fc4
	movs	r0, #14
	movs	r1, #0
	movs	r2, #5
.L_02001208:
	bl 0x0200a06c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #13
	bl 0x0200a08c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #3
	movs	r0, #17
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r2, #5
	movs	r0, #17
	movs	r1, #0
	bl 0x0200a06c
	movs	r0, #13
	movs	r1, #4
	bl 0x0200a034
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #13
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #17
	bl 0x0200a08c
	movs	r2, #5
	movs	r0, #17
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #2
	movs	r0, #7
	bl 0x0200a04c
	movs	r0, #5
	bl 0x02009fc4
	movs	r0, #7
	movs	r1, #0
.L_0200127c:
	movs	r2, #5
	bl 0x0200a06c
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #192
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200a074
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x0200a07c
	movs	r0, #20
.L_020012a6:
	bl 0x02009fc4
	movs	r0, #14
	movs	r1, #4
	bl 0x0200a034
	movs	r0, #14
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r1, #129
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #15
	bl 0x0200a08c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #15
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r2, #5
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a06c
	movs	r0, #13
	movs	r1, #4
	bl 0x0200a034
	movs	r1, #0
	movs	r0, #13
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #13
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r1, #2
	movs	r2, #50
	adds	r1, #255
	movs	r0, #15
	bl 0x0200a08c
	movs	r0, #15
	movs	r1, #4
	bl 0x0200a034
	movs	r2, #5
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #3
	movs	r0, #13
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #13
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r1, #128
	movs	r2, #45
	lsls	r1, r1, #1
	movs	r0, #15
	bl 0x0200a08c
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #13
	bl 0x0200a07c
	movs	r0, #5
	bl 0x02009fc4
	movs	r2, #5
	movs	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #15
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #15
	bl 0x0200a08c
	movs	r1, #0
	movs	r0, #15
	bl 0x0200a064
	movs	r0, #4
	movs	r1, #0
	bl 0x02009fdc
	cmp	r0, #0
	bne.n	.L_020013b8
	movs	r0, #15
	bl 0x02009fc4
	movs	r2, #5
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a06c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020013da
	.2byte 0x0000
	.2byte 0xb098
	.2byte 0x0200
.L_020013b8:
	movs	r0, #35
	bl 0x02009fc4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #15
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
.L_020013da:
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #15
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #15
	bl 0x0200a08c
	movs	r2, #5
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #0
	movs	r0, #13
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #3
	movs	r0, #13
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #13
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #13
	bl 0x0200a08c
	movs	r2, #5
	movs	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r0, #15
	movs	r1, #4
	bl 0x0200a034
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #15
	bl 0x02009fec
	movs	r0, #15
	bl 0x02009fe4
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #32
	movs	r2, #0
	movs	r0, #15
	bl 0x0200a0ec
	movs	r0, #1
	bl 0x02009fc4
	movs	r0, #15
	bl 0x02009fe4
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #0
	movs	r0, #7
	movs	r2, #0
	bl 0x0200a074
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a074
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #208
	movs	r0, #17
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #224
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #224
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200a074
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x0200a07c
	movs	r0, #5
	bl 0x02009fc4
	movs	r1, #2
	movs	r0, #15
	bl 0x0200a04c
	movs	r0, #10
	bl 0x02009fc4
	movs	r2, #5
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a06c
	movs	r0, #15
	movs	r1, #4
	bl 0x0200a034
	movs	r2, #5
.L_020014f6:
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #2
	movs	r0, #7
	bl 0x0200a04c
	movs	r0, #10
	bl 0x02009fc4
	movs	r2, #5
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #3
	movs	r0, #15
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #15
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #4
	bl 0x0200a08c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #5
	bl 0x0200a08c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #6
	bl 0x0200a08c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #17
	bl 0x0200a08c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #60
	movs	r0, #7
	bl 0x0200a08c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #15
	bl 0x0200a08c
	movs	r2, #5
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #2
	movs	r0, #15
	bl 0x0200a04c
	movs	r0, #5
	bl 0x02009fc4
	movs	r0, #15
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #15
	ldr	r1, [pc, #988]
	adds	r2, #153
	bl 0x02009fec
	movs	r1, #50
	movs	r2, #24
	movs	r0, #15
	negs	r1, r1
	bl 0x0200a0ec
	movs	r0, #13
	movs	r1, #15
	bl 0x0200a0f4
	movs	r0, #14
	movs	r1, #15
	bl 0x0200a0f4
	movs	r0, #7
	movs	r1, #15
	bl 0x0200a0f4
	movs	r0, #4
	movs	r1, #15
	bl 0x0200a0f4
	movs	r0, #5
	movs	r1, #15
	bl 0x0200a0f4
	movs	r0, #6
	movs	r1, #15
	bl 0x0200a0f4
	movs	r0, #17
	movs	r1, #15
	bl 0x0200a0f4
	movs	r0, #15
	movs	r1, #1
	bl 0x0200a09c
	movs	r1, #8
	movs	r0, #15
	negs	r1, r1
	movs	r2, #32
	bl 0x0200a0ec
	movs	r1, #160
	movs	r2, #0
	movs	r0, #16
	lsls	r1, r1, #7
	bl 0x0200a074
	movs	r0, #4
	movs	r1, #3
	bl 0x0200a044
	movs	r1, #129
	movs	r0, #4
	lsls	r1, r1, #1
	bl 0x0200a094
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a044
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x0200a094
	movs	r0, #37
	bl 0x02009fc4
	movs	r0, #4
	ldr	r1, [pc, #840]
	ldr	r2, [pc, #840]
	bl 0x02009fec
	movs	r0, #5
	ldr	r1, [pc, #828]
	ldr	r2, [pc, #832]
	bl 0x02009fec
	movs	r1, #8
	movs	r0, #5
	negs	r1, r1
	movs	r2, #16
	bl 0x0200a0e4
	movs	r1, #8
	movs	r2, #16
	movs	r0, #4
	bl 0x0200a0ec
	movs	r0, #5
	bl 0x0200a01c
	movs	r1, #128
	movs	r2, #0
	movs	r0, #4
	lsls	r1, r1, #8
	bl 0x0200a074
	movs	r1, #0
	movs	r0, #5
	bl 0x0200a07c
	movs	r0, #15
	bl 0x02009fc4
	movs	r0, #4
	movs	r1, #15
	bl 0x0200a0f4
	movs	r0, #5
	movs	r1, #15
	bl 0x0200a0f4
	movs	r2, #64
	movs	r1, #0
	movs	r0, #15
	bl 0x0200a0ec
	movs	r0, #15
	bl 0x02009fc4
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r0, #15
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #8
	adds	r1, #255
	movs	r2, #40
	movs	r0, #15
	bl 0x0200a08c
	movs	r2, #5
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #15
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #16
	bl 0x0200a07c
	movs	r0, #15
	bl 0x02009fc4
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #16
	bl 0x0200a07c
	movs	r0, #15
	bl 0x02009fc4
	movs	r0, #16
	movs	r1, #5
	bl 0x0200a02c
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	movs	r3, #0
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	bl 0x0200a0a4
	movs	r1, #0
	movs	r2, #96
	movs	r0, #15
	bl 0x0200a0ec
	movs	r0, #20
	bl 0x02009fc4
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a024
	movs	r1, #128
	movs	r2, #128
	movs	r0, #4
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009fec
	movs	r1, #128
	movs	r2, #128
	movs	r0, #5
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009fec
	movs	r2, #16
	movs	r0, #5
	movs	r1, #8
	negs	r2, r2
	bl 0x0200a0e4
	movs	r1, #8
	movs	r2, #16
	negs	r1, r1
	negs	r2, r2
	movs	r0, #4
	bl 0x0200a0ec
	movs	r0, #5
	bl 0x0200a01c
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #192
	movs	r0, #14
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #192
	movs	r0, #13
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a074
	movs	r0, #218
	movs	r1, #1
	movs	r2, #240
	movs	r3, #1
	lsls	r2, r2, #17
	negs	r1, r1
	lsls	r0, r0, #18
	bl 0x0200a0a4
	bl 0x0200a0ac
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #16
	bl 0x02009ff4
	movs	r0, #16
	movs	r1, #1
	bl 0x0200a02c
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r0, #17
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r2, #5
	movs	r0, #17
	movs	r1, #0
	bl 0x0200a06c
	movs	r0, #13
	movs	r1, #4
	bl 0x0200a034
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r0, #13
	movs	r1, #4
	bl 0x0200a034
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #13
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #128
	movs	r0, #14
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a074
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #128
	movs	r2, #0
	movs	r0, #17
	lsls	r1, r1, #8
	bl 0x0200a074
	movs	r1, #0
	movs	r0, #4
	bl 0x0200a07c
	movs	r0, #40
	bl 0x02009fc4
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #192
	movs	r0, #14
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #160
	movs	r0, #13
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #176
	movs	r2, #0
	movs	r0, #17
	lsls	r1, r1, #8
	bl 0x0200a074
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x0200a07c
	movs	r0, #20
	bl 0x02009fc4
	movs	r0, #13
	movs	r1, #2
	bl 0x0200a04c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r0, #6
	movs	r1, #2
	bl 0x0200a04c
	movs	r0, #6
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #13
	bl 0x0200a08c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #13
	bl 0x0200a07c
	movs	r0, #20
	bl 0x02009fc4
	movs	r1, #0
	movs	r0, #14
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #3
	movs	r0, #14
	bl 0x0200a034
	movs	r0, #20
	bl 0x02009fc4
	movs	r1, #192
	movs	r2, #0
	movs	r0, #13
	lsls	r1, r1, #6
	bl 0x0200a074
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #14
	bl 0x0200a07c
	movs	r0, #5
	bl 0x02009fc4
	movs	r0, #14
	movs	r1, #24
	movs	r2, #26
	bl 0x0200a0ec
	movs	r2, #8
	movs	r1, #0
	movs	r0, #14
	bl 0x0200a0ec
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #2
	movs	r0, #14
	bl 0x0200a04c
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #14
	movs	r1, #4
	movs	r2, #20
	bl 0x0200a03c
	movs	r5, #15
	b.n	.L_02001988
	.2byte 0x0000
	.4byte 0x00013333
	.4byte 0x00023333
	.2byte 0x1999
	.2byte 0x0001
.L_02001988:
	movs	r0, #64
	bl 0x02009fe4
	ldr	r3, [r0, #28]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r2
	str	r3, [r0, #28]
	subs	r5, #1
	movs	r0, #1
	bl 0x02009f44
	cmp	r5, #0
	bge.n	.L_02001988
	movs	r0, #64
	bl 0x02009fe4
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r0, #28]
	movs	r0, #5
	bl 0x02009fc4
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200a08c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #17
	bl 0x0200a08c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #6
	bl 0x0200a08c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #60
	movs	r0, #4
	bl 0x0200a08c
	movs	r1, #24
	movs	r2, #26
	movs	r0, #14
	negs	r1, r1
	negs	r2, r2
	bl 0x0200a0ec
	movs	r2, #8
	negs	r2, r2
	movs	r0, #14
	movs	r1, #0
	bl 0x0200a0ec
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #14
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #2
	movs	r0, #14
	bl 0x0200a04c
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #14
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #128
	movs	r2, #0
	movs	r0, #17
	lsls	r1, r1, #8
	bl 0x0200a074
	movs	r1, #0
	movs	r0, #4
	bl 0x0200a07c
	movs	r0, #30
	bl 0x02009fc4
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #176
	movs	r2, #0
	movs	r0, #17
	lsls	r1, r1, #8
	bl 0x0200a074
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x0200a07c
	movs	r0, #15
	bl 0x02009fc4
	movs	r1, #2
	movs	r0, #13
	bl 0x0200a04c
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #0
	movs	r0, #7
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #7
	bl 0x0200a08c
	movs	r2, #5
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #128
	movs	r0, #13
	lsls	r1, r1, #8
	bl 0x0200a07c
	movs	r0, #13
	movs	r1, #4
	bl 0x0200a034
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #13
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r1, #128
	movs	r2, #0
	movs	r0, #7
	lsls	r1, r1, #6
	bl 0x0200a074
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #13
	bl 0x0200a07c
	movs	r0, #5
	bl 0x02009fc4
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #3
	movs	r0, #17
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #17
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r2, #0
	movs	r0, #14
	movs	r1, #0
	bl 0x0200a074
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #13
	bl 0x0200a07c
	movs	r0, #30
	bl 0x02009fc4
	movs	r0, #14
	movs	r1, #3
	bl 0x0200a02c
	movs	r1, #3
	movs	r0, #13
	bl 0x0200a034
	movs	r0, #20
	bl 0x02009fc4
	movs	r1, #192
	movs	r2, #0
	movs	r0, #14
	lsls	r1, r1, #6
	bl 0x0200a074
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #13
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #3
	movs	r0, #14
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #14
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #13
	bl 0x0200a08c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #3
	movs	r0, #13
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #2
	movs	r0, #14
	bl 0x0200a04c
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #14
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #128
	movs	r2, #0
	movs	r0, #17
	lsls	r1, r1, #8
	bl 0x0200a074
	movs	r1, #0
	movs	r0, #4
	bl 0x0200a07c
	movs	r0, #30
	bl 0x02009fc4
	movs	r0, #4
	movs	r1, #3
	bl 0x0200a02c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a02c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200a02c
	movs	r1, #3
	movs	r0, #17
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #176
	movs	r2, #0
	movs	r0, #17
	lsls	r1, r1, #8
	bl 0x0200a074
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x0200a07c
	movs	r0, #20
	bl 0x02009fc4
	movs	r1, #2
	movs	r0, #13
	bl 0x0200a04c
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #10
	adds	r0, #13
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r0, #13
	bl 0x0200a07c
	movs	r0, #5
	bl 0x02009fc4
	movs	r1, #216
	movs	r2, #196
	lsls	r2, r2, #1
	movs	r0, #13
	lsls	r1, r1, #2
	bl 0x0200a00c
	movs	r1, #192
	movs	r0, #13
	lsls	r1, r1, #6
	bl 0x0200a07c
	movs	r1, #2
	movs	r0, #14
	bl 0x0200a04c
	movs	r0, #5
	bl 0x02009fc4
	movs	r2, #5
	movs	r0, #14
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #14
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #3
	movs	r0, #14
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r2, #5
	movs	r0, #14
	movs	r1, #0
	bl 0x0200a06c
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #14
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #2
	movs	r0, #14
	bl 0x0200a04c
	movs	r0, #10
	bl 0x02009fc4
	movs	r0, #14
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a06c
	movs	r1, #128
	movs	r0, #17
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a074
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #128
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200a074
	movs	r1, #0
	movs	r0, #4
	bl 0x0200a07c
	movs	r0, #20
	bl 0x02009fc4
	movs	r0, #4
	movs	r1, #3
	bl 0x0200a02c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a02c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200a02c
	movs	r1, #3
	movs	r0, #17
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a074
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #17
	ldr	r1, [pc, #412]
	bl 0x02009fec
	movs	r0, #17
	movs	r1, #2
	bl 0x0200a02c
	ldr	r3, [pc, #400]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	bl 0x02009fe4
	cmp	r0, #0
	beq.n	.L_02001dbc
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #17
	bl 0x02009ffc
.L_02001dbc:
	movs	r0, #17
	bl 0x0200a01c
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a024
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #5
	ldr	r1, [pc, #344]
	adds	r2, #153
	bl 0x02009fec
	movs	r0, #5
	movs	r1, #2
	bl 0x0200a02c
	ldr	r0, [r5, #0]
	bl 0x02009fe4
	cmp	r0, #0
	beq.n	.L_02001dfa
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x02009ffc
.L_02001dfa:
	movs	r0, #5
	bl 0x0200a01c
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a024
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #280]
	adds	r2, #153
	bl 0x02009fec
	movs	r0, #6
	movs	r1, #2
	bl 0x0200a02c
	ldr	r0, [r5, #0]
	bl 0x02009fe4
	cmp	r0, #0
	beq.n	.L_02001e38
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #6
	bl 0x02009ffc
.L_02001e38:
	movs	r0, #6
	bl 0x0200a01c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #6
	bl 0x0200a024
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #32
	movs	r2, #48
	movs	r0, #4
	negs	r1, r1
	negs	r2, r2
	bl 0x0200a0ec
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a074
	movs	r1, #160
	movs	r2, #0
	movs	r0, #14
	lsls	r1, r1, #7
	bl 0x0200a074
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x0200a07c
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #3
	movs	r0, #4
	bl 0x0200a034
	movs	r0, #20
	bl 0x02009fc4
	movs	r1, #3
	movs	r0, #7
	bl 0x0200a034
	movs	r0, #10
	bl 0x02009fc4
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #7
	ldr	r1, [pc, #132]
	adds	r2, #153
	bl 0x02009fec
	movs	r0, #7
	movs	r1, #2
	bl 0x0200a02c
	ldr	r0, [r5, #0]
	bl 0x02009fe4
	cmp	r0, #0
	beq.n	.L_02001ece
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #7
	bl 0x02009ffc
.L_02001ece:
	movs	r0, #7
	bl 0x0200a01c
	movs	r2, #0
	movs	r1, #0
	movs	r0, #7
	bl 0x0200a024
	movs	r0, #10
	bl 0x02009fc4
	movs	r1, #3
	movs	r0, #14
	bl 0x0200a034
	movs	r0, #15
	bl 0x02009fc4
	movs	r1, #210
	movs	r2, #204
	lsls	r2, r2, #1
	movs	r0, #14
	lsls	r1, r1, #2
	bl 0x0200a00c
	movs	r1, #192
	movs	r0, #14
	lsls	r1, r1, #6
	bl 0x0200a07c
	movs	r1, #160
	movs	r0, #16
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a074
	movs	r0, #10
	bl 0x02009fc4
	bl 0x02009fd4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x00013333
	.4byte 0x02000240
	.section .rodata.x0200a12c,"a",%progbits
	.4byte 0xffff0000
	.4byte 0x00000120
	.4byte 0xc00001c6
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000cc
	.4byte 0x1010b0ca
	.4byte 0xffffffff
	.4byte 0x102060cc
	.4byte 0xffffffff
	.4byte 0x103070cc
	.4byte 0xffffffff
	.4byte 0x104080cc
	.4byte 0xffffffff
	.4byte 0x105090cc
	.4byte 0xffffffff
	.4byte 0x106020cc
	.4byte 0xffffffff
	.4byte 0x107030cc
	.4byte 0xffffffff
	.4byte 0x108040cc
	.4byte 0xffffffff
	.4byte 0x109050cc
	.4byte 0xffffffff
	.4byte 0x10a0a0ca
	.4byte 0xffffffff
	.4byte 0x10b0d0cc
	.4byte 0xffffffff
	.4byte 0x10c0e0cc
	.4byte 0xffffffff
	.4byte 0x10d0b0cc
	.4byte 0xffffffff
	.4byte 0x10e0c0cc
	.4byte 0xffffffff
	.4byte 0x10f100cc
	.4byte 0xffffffff
	.4byte 0x1100f0cc
	.4byte 0xffffffff
	.4byte 0x111110ca
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0x03900000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00015000
	.4byte 0xffff009c
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00013000
	.4byte 0xffff00a0
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00030000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00010000
	.4byte 0xffff00a4
	.4byte 0x00000001
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0001d000
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x03600000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00013000
	.4byte 0xffff002b
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00013000
	.4byte 0xffff0022
	.4byte 0x00000001
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00018000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x02300000
	.4byte 0x00015000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002c000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002c000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x0002e000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00340001
	.4byte 0x00020001
	.4byte 0x00020006
	.4byte 0x00010034
	.4byte 0x00060002
	.4byte 0x0002ffff
	.4byte 0x00020036
	.4byte 0x00060002
	.4byte 0x00360004
	.4byte 0x00020002
	.4byte 0xffff0006
	.4byte 0x0200a33c
	.4byte 0x00170005
	.4byte 0x0200a33c
	.4byte 0x0017001e
	.4byte 0x0200a33c
	.4byte 0x00040038
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x020080c9
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x020080c9
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x0000c602
	.4byte 0xffff0010
	.4byte 0x020080c9
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000002
	.4byte 0x096a0014
	.4byte 0x0200858d
	.4byte 0x00000000
	.4byte 0x196a0008
	.4byte 0x0000286e
	.4byte 0x00000000
	.4byte 0x196a0009
	.4byte 0x0000286f
	.4byte 0x00000000
	.4byte 0x196a000a
	.4byte 0x00002870
	.4byte 0x00000000
	.4byte 0x196a000b
	.4byte 0x00002871
	.4byte 0x00000000
	.4byte 0x196a000c
	.4byte 0x00002872
	.4byte 0x00008d15
	.4byte 0x196a0008
	.4byte 0x00002874
	.4byte 0x00008d15
	.4byte 0x196a0009
	.4byte 0x00002875
	.4byte 0x00008d15
	.4byte 0x196a000a
	.4byte 0x00002876
	.4byte 0x00008d15
	.4byte 0x196a000b
	.4byte 0x00002877
	.4byte 0x00008d15
	.4byte 0x196a000c
	.4byte 0x00002878
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000027f6
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000027f7
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000027f8
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000027f9
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000027fa
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000027fc
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000027fd
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000027fe
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000027ff
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002800
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000287a
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0000287b
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x0000287c
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000287d
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000287e
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x0000287f
	.4byte 0x000000f3
	.4byte 0xffff00c8
	.4byte 0x00403064
	.4byte 0x000000f3
	.4byte 0xffff00c9
	.4byte 0x00403065
	.4byte 0x000000f3
	.4byte 0xffff00ca
	.4byte 0x00403066
	.4byte 0x000000f3
	.4byte 0xffff00cb
	.4byte 0x00403067
	.4byte 0x000000f3
	.4byte 0xffff00cc
	.4byte 0x00403068
	.4byte 0x000000f3
	.4byte 0xffff00cd
	.4byte 0x00403069
	.4byte 0x000000f3
	.4byte 0xffff00ce
	.4byte 0x0040306a
	.4byte 0x000000f3
	.4byte 0xffff00cf
	.4byte 0x0040306b
	.4byte 0x000001c3
	.4byte 0xffff00d0
	.4byte 0x0040306d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x76c07b00
	.4byte 0x6e407280
	.4byte 0x65c06a00
	.4byte 0x65c06180
	.4byte 0x6e406a00
	.4byte 0x76c07280
	.4byte 0x687c0300
	.4byte 0x534f1241
	.4byte 0xb9b78bbb
	.4byte 0x85cb2760
	.4byte 0x3359ae11
	.4byte 0xf46b8462
	.4byte 0x05e6d358
	.4byte 0x170a174f
	.4byte 0xa817e020
	.4byte 0x8626b17d
	.4byte 0x23369a1e
	.4byte 0x618b21ec
	.2byte 0x2e22
.L_02002652:
	.2byte 0xf460
	.2byte 0x3c19
	.2byte 0x7a6d
	.2byte 0x93d0
	.2byte 0x61b7
	.2byte 0xe3cf
	.2byte 0xda7d
	.4byte 0x9a0500c4
	.4byte 0x91e4f014
	.4byte 0x0a313d01
	.4byte 0x276721e5
	.4byte 0x47a0a429
	.4byte 0x3c913c8f
	.4byte 0x882e2d8f
	.4byte 0xc7df42e4
	.4byte 0xbf4090e1
	.4byte 0x04190087
	.4byte 0xc300f26d
	.4byte 0x8609081a
	.4byte 0x33350a7b
	.4byte 0x63c4500d
	.4byte 0x03dfa703
	.4byte 0x4ba1622a
	.4byte 0x51eeda92
	.4byte 0x1ba45c24
	.4byte 0x7e501f87
	.4byte 0x65005f8f
	.4byte 0x0fc7843e
	.4byte 0xe7dedaca
	.2byte 0x69c2
	.2byte 0xb54d
	.2byte 0x2493
	.2byte 0x8841
	.2byte 0xa72e
	.2byte 0x24e5
	.2byte 0x1a1c
	.2byte 0x9284
	.2byte 0xe7c3
	.2byte 0xa2c0
	.4byte 0xc071f037
	.4byte 0x69c5e7eb
	.4byte 0xc2e0373f
	.4byte 0x59c2223d
	.4byte 0xddf8566c
	.4byte 0xd33e3c0a
	.4byte 0x3c19f1ac
	.4byte 0x59c312e8
	.4byte 0x38f22a81
	.4byte 0x511e79cc
	.4byte 0xfc42b9cd
	.4byte 0xc7186e30
	.4byte 0xa1e46258
	.4byte 0x96f23f41
	.4byte 0x6003c9f7
	.4byte 0x985339a4
	.4byte 0xf7ab0e53
	.4byte 0x9226eaf1
	.4byte 0x704841e7
	.4byte 0x02c2804c
	.4byte 0x992b09f8
	.4byte 0x4b01b09a
	.4byte 0x2b547b36
	.4byte 0x1ee84d50
	.4byte 0xa63d9903
	.4byte 0x9ccd48b2
	.4byte 0x91684ccd
	.4byte 0x4022d03c
	.4byte 0x103a2613
	.4byte 0x4580f7e6
	.4byte 0x9b21911a
	.4byte 0x304d9780
	.4byte 0x82107df0
	.4byte 0x1c3e471a
	.4byte 0x335e95e1
	.4byte 0xde6cb747
	.4byte 0xfc84fbce
	.4byte 0xd80b1210
	.4byte 0x0e07c343
	.4byte 0x09c71efc
	.4byte 0x888f0ff1
	.4byte 0xf810587d
	.4byte 0x805451f9
	.4byte 0xf1321171
	.2byte 0xcd3c
	.2byte 0x4284
.L_02002780:
	.2byte 0x79f7
	.2byte 0x1a1f
	.2byte 0xe3cf
	.2byte 0x7c69
	.4byte 0x41f3ea0c
	.4byte 0xf9821205
	.4byte 0x338605c9
	.4byte 0x6e6c0d1b
	.4byte 0xe108e856
	.4byte 0xbedd48f7
	.4byte 0xd017458f
	.4byte 0x0bc2339f
	.4byte 0xe3cb8e3f
	.4byte 0x0d7136a6
	.4byte 0x49087a5a
	.4byte 0x69c3b1e7
	.4byte 0x38f928af
	.4byte 0xf1a73f1e
	.4byte 0xe1ea0720
	.4byte 0xe20640dc
	.4byte 0xbce108ff
	.4byte 0xcf9029af
	.4byte 0xefe359e0
	.4byte 0xf15c1e34
	.4byte 0xc53837c3
	.4byte 0xebd8f0a7
	.4byte 0xec0b15b1
	.4byte 0xe1de1162
	.4byte 0xc34ebe04
	.4byte 0x1e6c9ee4
	.4byte 0x4c7df182
	.4byte 0x02087404
	.4byte 0x61c9241c
	.4byte 0x9c78080c
	.4byte 0x5fca1127
	.4byte 0x0830f4cc
	.4byte 0x78cfaf46
	.4byte 0xe8e64de1
	.4byte 0x021f0821
	.4byte 0xd808f8c5
	.4byte 0xa4790ece
	.4byte 0x98978dff
	.4byte 0x25080806
	.4byte 0xb059aa0f
	.4byte 0x11123c07
	.4byte 0xd0102494
	.4byte 0x92484700
	.4byte 0x30fbc97c
	.4byte 0x26688e38
	.4byte 0x80467059
	.4byte 0x1e5200fb
	.4byte 0x82c0f227
	.4byte 0xc40603df
	.4byte 0x0709b670
	.4byte 0x3ffa5f98
	.4byte 0xdf3f0292
	.4byte 0x34e02b91
	.4byte 0x000200d6
	.4byte 0x4f01978f
	.4byte 0x819f1f7b
	.4byte 0xed0b8a0b
	.4byte 0xab1ee355
	.4byte 0xe7664900
	.4byte 0xf71cf208
	.4byte 0x0f7666d4
	.4byte 0xb4ed2830
	.4byte 0x1eeb6647
	.4byte 0xe89e3c8b
	.4byte 0xae69c804
	.4byte 0x67606c9f
	.4byte 0x6411df1e
	.4byte 0x511e2cf8
	.4byte 0xbc4d079b
	.4byte 0xf1e2040f
	.4byte 0x80083d1e
	.4byte 0x9bd98d79
	.4byte 0xc0ba11f7
	.4byte 0x0e20e622
	.4byte 0xe4003c75
	.4byte 0x4172cf70
	.4byte 0x1a271802
	.4byte 0x7844e901
	.4byte 0x2103f1ee
	.4byte 0xb25dc7bf
	.4byte 0x1f04f513
	.4byte 0xdd93643e
	.4byte 0x9221c7b9
	.4byte 0x08080f5e
	.4byte 0xd78fbcfa
	.4byte 0x43079051
	.4byte 0xedcc7b1f
	.4byte 0x80b80f24
	.4byte 0xce9cd1d4
	.4byte 0x9d76c8fb
	.4byte 0x8d2048ab
	.4byte 0xef2ba004
	.4byte 0x2a303323
	.4byte 0x8f71af30
	.4byte 0x01d18589
	.4byte 0x91dc2174
	.4byte 0xa28040f3
	.4byte 0x01b31a28
	.4byte 0x839f0363
	.4byte 0x1e02a1d3
	.4byte 0x3c7bf410
	.4byte 0x49c8e08c
	.4byte 0x4355e3e8
	.4byte 0x209a4e3d
	.4byte 0xd8bc7b20
	.4byte 0x08f8f7ed
	.4byte 0x73cfd7dc
	.4byte 0xc5e978d6
	.4byte 0xeebc79d7
	.4byte 0x91f90003
	.4byte 0x780b9fa0
	.4byte 0x6bdfd718
	.4byte 0x3ceb9c3c
	.4byte 0x389e11c7
	.4byte 0xf8f0a7c7
	.4byte 0x7c4026bd
	.4byte 0x739f8b69
	.4byte 0x25c139f1
	.4byte 0xf522b087
	.4byte 0xee183740
	.4byte 0x8f708f90
	.4byte 0x20a18eac
	.4byte 0x1910f91e
	.4byte 0x10783d10
	.4byte 0xc1c847bf
	.4byte 0x10e19750
	.4byte 0x80600dd9
	.4byte 0x15d21a53
	.4byte 0xc63c8186
	.4byte 0xa6d63dc2
	.4byte 0xa0662935
	.4byte 0x47c06046
	.4byte 0x883978f0
	.4byte 0x280f2f79
	.4byte 0x37f0d492
	.4byte 0x4c74ee44
	.4byte 0x97889223
	.4byte 0x976c1ce1
	.4byte 0x2f112ee1
	.4byte 0x4863e8f7
	.4byte 0x08ccc66b
	.4byte 0x51cb5c00
	.4byte 0x0e68359f
	.4byte 0x87300f61
	.4byte 0xf70f603c
	.4byte 0x0dc3ebb0
	.4byte 0x3399e478
	.4byte 0x5e0a0204
	.4byte 0x0605e4c3
	.4byte 0x030822ce
	.4byte 0x08871f38
	.4byte 0x92906610
	.4byte 0x2f906b10
	.4byte 0x06624f30
	.4byte 0x48f0a7c0
	.4byte 0x269a3900
	.4byte 0xc0626289
	.4byte 0xfc25c127
	.4byte 0x6fd785c4
	.4byte 0x8523c738
	.4byte 0x4f741a06
	.4byte 0x1cbae717
	.4byte 0x8e36f3f5
	.4byte 0xf0d39a00
	.4byte 0x587bce46
	.4byte 0xb07833e4
	.4byte 0xe9c75e1d
	.4byte 0x037d06cb
	.4byte 0xf030071f
	.4byte 0xc7860685
	.4byte 0x0ecc54a4
	.4byte 0xd01e6110
	.4byte 0xf02e0a21
	.4byte 0x64e02c38
	.4byte 0x9cc1fe01
	.4byte 0xb8fb4786
	.4byte 0x28f9c887
	.4byte 0xf31e53d3
	.4byte 0x79e42024
	.4byte 0x1efc6544
	.4byte 0x61442d8f
	.4byte 0x188d39f9
	.4byte 0x3184f486
	.4byte 0x81ece41d
	.4byte 0xae0f70ab
	.4byte 0xe53ca3c0
	.4byte 0x81316c0b
	.4byte 0x489b46a1
	.4byte 0x02a38114
	.4byte 0xace82b5e
	.4byte 0x13341f86
	.4byte 0xc430720e
	.4byte 0x0b41711e
	.4byte 0xc2f0099c
	.4byte 0xd0019f1b
	.4byte 0x8e1a73f3
	.4byte 0x92407a1e
	.4byte 0xc0940088
	.4byte 0x1811c8fb
	.4byte 0x78038477
	.4byte 0x61478058
	.4byte 0x8b7c0cf9
	.4byte 0x0b2351f1
	.4byte 0x14fce0c6
	.4byte 0x93c1cfab
	.4byte 0x13e92b39
	.4byte 0x41af3c78
	.4byte 0x861a732d
	.4byte 0x86b8fc2e
	.4byte 0xf55803df
	.4byte 0x64ce443e
	.4byte 0x8e0cc040
	.4byte 0xd1a8fd7a
	.4byte 0x08d4af0d
	.4byte 0x7c0ff3e0
	.4byte 0x3fcf81fe
	.4byte 0x3e07f9f0
	.4byte 0x1fe7c0ff
	.4byte 0x1f03fcf8
	.4byte 0x0003e05b
	.4byte 0x967c0300
	.4byte 0x6e2becc1
	.4byte 0xb073b739
	.4byte 0xda769f35
	.4byte 0x60de6573
	.4byte 0x06e73c05
	.4byte 0x011e10fb
	.4byte 0x8b12f1c0
	.4byte 0x08f30497
	.4byte 0x5798e9ce
	.4byte 0x6663f0b8
	.4byte 0x91c88212
	.4byte 0x0cf7e045
	.4byte 0x556c8966
	.4byte 0x3ad1d4e7
	.4byte 0x50583efb
	.4byte 0xf021c0a4
	.4byte 0x3f9f0421
	.4byte 0x59db0c67
	.4byte 0x3cd4ea4d
	.4byte 0x81a30bab
	.4byte 0x8fd9756f
	.4byte 0x0f0d00a6
	.4byte 0xb9c6a357
	.4byte 0x2c366878
	.4byte 0x01a44870
	.4byte 0x02fc1a41
	.4byte 0x7b3f9d7d
	.4byte 0x61abd6de
	.4byte 0x56440682
	.4byte 0xac568eaf
	.4byte 0x87a266e1
	.4byte 0xf00f3cc7
	.4byte 0xa5797bcc
	.4byte 0x20494636
	.4byte 0x9d854917
	.4byte 0xa76cf0f3
	.4byte 0x0411f028
	.4byte 0xce671cea
	.4byte 0xefcb2329
	.4byte 0x78c831f1
	.4byte 0x0050b0ce
	.4byte 0xf93f3b08
	.4byte 0x1b79163d
	.4byte 0x3673a21e
	.4byte 0xe44f0cc9
	.4byte 0xd39c0d43
	.4byte 0x10f9ce6a
	.4byte 0xc157bf1e
	.4byte 0xaebc8f27
	.4byte 0xa5713840
	.4byte 0xc7bf5873
	.4byte 0x6bf91730
	.4byte 0x7db68cc6
	.4byte 0x9128d69e
	.4byte 0x3c99e08f
	.4byte 0xd3585a36
	.4byte 0x9985209d
	.4byte 0xa83457c0
	.4byte 0xb4f239f4
	.4byte 0x70d10444
	.4byte 0x27bf6a58
	.4byte 0x69b4cb1a
	.4byte 0x8e75aad1
	.4byte 0x68b4c446
	.4byte 0x27bf6463
	.4byte 0x6144fea3
	.4byte 0x45830327
	.4byte 0xd31fa98c
	.4byte 0x1ab81298
	.4byte 0x30b49346
	.4byte 0xe0083141
	.4byte 0x6ab320fb
	.4byte 0xd11368d1
	.4byte 0x215902a0
	.4byte 0x39d3dfa3
	.4byte 0x61b0a2d1
	.4byte 0x9c34c391
	.4byte 0x704ec8b0
	.4byte 0x59f490d1
	.4byte 0x8ba98cb1
	.4byte 0xf81705c1
	.4byte 0x3e320a3d
	.4byte 0x3108b8e4
	.4byte 0x764b5dae
	.4byte 0xf036116c
	.4byte 0x3642d33e
	.4byte 0x0f800dc3
	.4byte 0x840099ec
	.4byte 0x91e13c42
	.4byte 0x44f0581a
	.4byte 0x3843c200
	.4byte 0xc2c2e0c8
	.4byte 0x08fabec5
	.4byte 0x6921f01e
	.4byte 0x225173d8
	.4byte 0x87a2eeb5
	.4byte 0x929d837f
	.4byte 0xc8f4ee14
	.4byte 0x38ba078f
	.4byte 0x30c39c16
	.4byte 0x2860e1b2
	.4byte 0x1f7879e7
	.4byte 0x33d64cf0
	.4byte 0xc1fac698
	.4byte 0x5982de7e
	.4byte 0xc7c78d3e
	.4byte 0x7e58f810
	.4byte 0x38e0539f
	.4byte 0x72f14d02
	.4byte 0x80e75e1d
	.4byte 0x87cf1ec7
	.4byte 0xeb5c7908
	.4byte 0xece9e79c
	.4byte 0x5ece8065
	.4byte 0x904a00d0
	.4byte 0x1f91d493
	.4byte 0x1619e0d3
	.4byte 0x5c045b4c
	.4byte 0x673a7cc8
	.4byte 0x7c46a45c
	.4byte 0x91fe23c7
	.4byte 0x3ee3f0e4
	.4byte 0xab6a9e12
	.4byte 0xc53efb3b
	.4byte 0x0d111890
	.4byte 0x34e7e002
	.4byte 0x761b988a
	.4byte 0x4e1add06
	.4byte 0x35a66283
	.4byte 0x1aa7b7a4
	.4byte 0xad3c43c2
	.4byte 0x28a44862
	.4byte 0x0d44b1e3
	.4byte 0x8036018f
	.4byte 0x1b607b93
	.4byte 0x401201a9
	.4byte 0x1e923f23
	.4byte 0x6b49f518
	.4byte 0x5a1e12e0
	.4byte 0xf5852fe7
	.4byte 0x1f037f38
	.4byte 0xf40d81d4
	.4byte 0x0dc795f2
	.4byte 0x7ace82b1
	.4byte 0x1a1e0180
	.4byte 0x1aaa0436
	.4byte 0xc780e39f
	.4byte 0x68178008
	.4byte 0x207f8d3c
	.4byte 0x902f7430
	.4byte 0x879b01e8
	.4byte 0x20223c89
	.4byte 0x2c9ec2ae
	.2byte 0x30f2
.L_02002d62:
	.2byte 0x9780
	.2byte 0xe03b
	.2byte 0xcf1a
	.4byte 0xf7b673b3
	.4byte 0xb82a0739
	.4byte 0xe0ae1e68
	.4byte 0x8f1efd09
	.4byte 0x3e3fe00b
	.4byte 0xf3f5610f
	.4byte 0xe3c69f42
	.4byte 0x02d8fbf7
	.4byte 0x3ef1a73f
	.4byte 0xf23c7991
	.4byte 0x3cc3c030
	.4byte 0x7b978fbe
	.4byte 0xfcc7bcfc
	.4byte 0x46c041c7
	.4byte 0x99590da4
	.4byte 0xdfb3f1b6
	.4byte 0xc584eb03
	.4byte 0xb26232d8
	.4byte 0x46c43a62
	.4byte 0xf23b274a
	.4byte 0x1978e63c
	.4byte 0xf2696a9d
	.4byte 0x0c3086ea
	.4byte 0x10e111f8
	.4byte 0xec06c64c
	.4byte 0xd423cce9
	.4byte 0x03c8d513
	.4byte 0xb231fd9f
	.4byte 0x98f48901
	.2byte 0xa4dc
.L_02002dde:
	.2byte 0xa67e
	.2byte 0xe8dd
	.2byte 0x2e51
	.2byte 0xae68
.L_02002de6:
	.2byte 0x7de2
	.2byte 0x8380
	.2byte 0x61c7
	.2byte 0x1691
	.2byte 0x48d4
.L_02002df0:
	.2byte 0xe4c6
	.2byte 0xa81e
	.4byte 0x3c556aa6
	.4byte 0x5a34d8e1
	.2byte 0x4828
.L_02002dfe:
	.2byte 0x881e
	.2byte 0xfa47
	.2byte 0x483d
	.2byte 0x5540
	.2byte 0xa8cc
	.2byte 0x9caa
	.2byte 0x24c1
.L_02002e0c:
	.2byte 0x3538
	.2byte 0xec3e
	.2byte 0x8c7d
	.2byte 0x8d1b
	.2byte 0xfc1b
	.2byte 0xf91e
.L_02002e18:
	.2byte 0xd3a3
	.2byte 0x0f07
	.2byte 0x18e3
	.2byte 0xf90d
	.2byte 0xae59
	.2byte 0x4217
	.2byte 0x07a0
	.2byte 0x4290
	.2byte 0x99a5
	.2byte 0x22b1
	.2byte 0x538f
.L_02002e2e:
	.2byte 0x1950
	.2byte 0x0a87
	.2byte 0xdc30
	.2byte 0xd923
	.2byte 0x5d5a
	.2byte 0x3acf
	.2byte 0xa159
	.2byte 0x47bc
	.2byte 0x460f
	.2byte 0xee60
	.2byte 0xd623
	.2byte 0xfe70
	.2byte 0x1e23
	.2byte 0x38f0
	.2byte 0x3a10
	.2byte 0xcfe0
.L_02002e4e:
	.2byte 0x3eac
	.2byte 0x40f0
	.2byte 0xcf88
	.2byte 0xf4e3
	.2byte 0x7c60
	.2byte 0x833e
	.2byte 0xc007
	.2byte 0xf221
	.2byte 0x53b6
	.2byte 0xa7cf
	.2byte 0xc256
	.2byte 0x9e3d
.L_02002e66:
	.2byte 0xc507
.L_02002e68:
	.2byte 0xa7e7
	.2byte 0x3c42
	.2byte 0xcc4a
	.2byte 0x35e3
	.2byte 0x027c
.L_02002e72:
	.2byte 0xc62e
	.2byte 0xf1c7
	.2byte 0x3cf8
	.2byte 0x9402
.L_02002e7a:
	.2byte 0xf805
	.2byte 0xac22
.L_02002e7e:
	.2byte 0x8d39
.L_02002e80:
	.2byte 0x89d4
	.2byte 0x981e
	.2byte 0xf3bf
	.2byte 0x523c
	.2byte 0xf85e
	.2byte 0xc30d
.L_02002e8c:
	.2byte 0xc67d
.L_02002e8e:
	.2byte 0x50bd
	.2byte 0x0e13
	.2byte 0xe9ed
	.2byte 0xf311
.L_02002e96:
	.2byte 0x7130
	.2byte 0x67e3
	.2byte 0x9c07
	.2byte 0x11bd
.L_02002e9e:
	.2byte 0x1004
	.2byte 0xb92c
.L_02002ea2:
	.2byte 0xfe66
	.2byte 0x58eb
	.2byte 0x8ab6
	.2byte 0x0515
	.2byte 0xbc7d
	.2byte 0xb48f
.L_02002eae:
	.2byte 0x0287
.L_02002eb0:
	.2byte 0x89a4
	.2byte 0x06e0
	.2byte 0x224d
	.2byte 0x98f7
	.2byte 0x00e1
	.2byte 0x5494
.L_02002ebc:
	.2byte 0x7903
	.2byte 0xc851
	.2byte 0x8087
	.2byte 0x5486
	.2byte 0xc320
.L_02002ec6:
	.2byte 0x1ff0
.L_02002ec8:
	.2byte 0xb180
	.2byte 0x91d5
	.2byte 0x3e91
	.2byte 0x0018
	.2byte 0x9f71
.L_02002ed2:
	.2byte 0x7eb0
	.2byte 0x998f
	.2byte 0x7122
	.2byte 0xa711
	.2byte 0x20d4
	.2byte 0x8827
	.2byte 0xbf00
.L_02002ee0:
	.2byte 0xb187
	.2byte 0x051a
	.2byte 0x483e
	.2byte 0x0c3c
.L_02002ee8:
	.2byte 0x8016
.L_02002eea:
	.2byte 0xf0df
	.2byte 0x3062
	.2byte 0x07a9
	.2byte 0xb08b
.L_02002ef2:
	.2byte 0xde7d
	.2byte 0xf447
	.2byte 0xb3a1
	.2byte 0xef17
	.2byte 0x4818
	.2byte 0x0c30
	.2byte 0x1ee1
	.2byte 0xefcf
	.2byte 0x8758
	.2byte 0x3dc3
.L_02002f06:
	.2byte 0x7a3e
	.2byte 0xe1de
	.2byte 0x0788
	.4byte 0x0d71c34e
	.4byte 0xc52011d4
	.4byte 0x8f220f89
	.2byte 0x7406
	.2byte 0x53be
.L_02002f1c:
	.2byte 0xd611
	.2byte 0xc7c3
	.2byte 0xcac1
.L_02002f22:
	.2byte 0x1dfc
.L_02002f24:
	.2byte 0x0794
.L_02002f26:
	.2byte 0x394a
	.2byte 0x2967
	.2byte 0xe7f7
	.4byte 0x083097e1
	.4byte 0x0de41669
	.4byte 0x9fac26f4
	.4byte 0x2238f0c8
	.4byte 0x0e4f8239
	.2byte 0x390a
.L_02002f42:
	.2byte 0x6533
	.2byte 0x4034
	.2byte 0x4c1e
	.2byte 0xe840
	.2byte 0x5c81
	.2byte 0xba0f
	.2byte 0x80c2
	.2byte 0xdf85
	.2byte 0xa203
	.2byte 0xfe30
	.2byte 0x2443
	.2byte 0x2581
	.2byte 0xc1e1
	.2byte 0x06a6
	.2byte 0xa15c
	.2byte 0x36c7
	.2byte 0xfc0b
	.2byte 0x869c
	.2byte 0x8997
	.2byte 0xe23a
	.2byte 0x0d69
	.4byte 0x3e43c8dc
	.4byte 0x10180a4d
	.4byte 0xb03c8b00
	.4byte 0xb03dabe0
	.4byte 0x119bc7ce
	.4byte 0x1336da07
	.4byte 0xd8e1a738
	.4byte 0x5e3b63a9
	.4byte 0x02bce3bf
	.4byte 0x3c0f40a4
	.4byte 0x28413a40
	.4byte 0xfbd7abe3
	.4byte 0xcabe3df8
	.4byte 0x20869cc4
	.4byte 0x8a5c4387
	.4byte 0xdc7ff1ac
	.4byte 0xac1e8d6b
	.4byte 0xcc43c797
	.4byte 0x4203aa73
	.4byte 0x5da0f3e1
	.4byte 0x8020a33e
	.4byte 0xf9f03fcf
	.4byte 0xc0ff3e07
	.4byte 0xfcf81fe7
	.4byte 0xe07f9f03
	.4byte 0x6c7c0ff3
	.4byte 0x00000f81
	.4byte 0x01000000
	.4byte 0x03020201
	.4byte 0x03030303
	.4byte 0x00010203
	.4byte 0x0c040e02
	.4byte 0x08080a06
	.4byte 0x040c060a
	.4byte 0x00100010
	.4byte 0x060a040c
	.4byte 0x0a060808
	.4byte 0x0e020c04
	.4byte 0x0c040e02
	.4byte 0x08080a06
	.4byte 0x040c060a
	.4byte 0x00100010
	.4byte 0x060a040c
	.4byte 0x0a060808
	.4byte 0x0e020c04
