.syntax unified
	.thumb
	.section .text.x02008098,"ax",%progbits
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl 0x0200b1d8
	movs	r1, #1
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl 0x0200b240
	adds	r0, r6, #0
	bl 0x0200b1d8
	movs	r1, #1
	bl 0x0200b198
	ldr	r3, [r5, #8]
	ldr	r2, [pc, #48]
	movs	r0, #1
	adds	r3, r3, r2
	ldr	r2, [r5, #80]
	str	r3, [r5, #8]
	movs	r3, #0
	strh	r3, [r2, #18]
	bl 0x0200b138
	adds	r0, r6, #0
	bl 0x0200b1d8
	movs	r1, #1
	bl 0x0200b198
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	adds	r5, #85
	movs	r3, #3
	strb	r3, [r5, #0]
	movs	r0, #40
	bl 0x0200b138
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xfff4
	.2byte 0xb560
	sub	sp, #8
	bl 0x0200b1b8
	movs	r0, #0
	bl 0x0200b310
	movs	r1, #1
	movs	r0, #8
	bl 0x0200b268
	ldr	r6, [pc, #420]
	adds	r0, r6, #0
	bl 0x0200b288
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #8
	movs	r1, #2
	bl 0x0200b268
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r3, #25
	movs	r2, #42
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r2, #2
	movs	r1, #31
	movs	r0, #38
	bl 0x0200b190
	movs	r0, #8
	bl 0x0200b1d8
	movs	r1, #1
	adds	r5, r0, #0
	movs	r0, #8
	bl 0x0200b240
	movs	r0, #8
	bl 0x0200b1d8
	movs	r1, #1
	bl 0x0200b198
	movs	r0, #1
	bl 0x0200b138
	movs	r0, #8
	bl 0x0200b1d8
	movs	r1, #1
	bl 0x0200b198
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	movs	r3, #3
	adds	r5, #85
	strb	r3, [r5, #0]
	movs	r0, #40
	bl 0x0200b138
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	ldr	r5, [pc, #300]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #8
	bl 0x0200b270
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x0200b2c8
	movs	r0, #20
	bl 0x0200b1b0
	movs	r0, #8
	movs	r1, #0
	movs	r2, #20
	bl 0x0200b298
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #8
	bl 0x0200b2c0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	movs	r2, #40
	bl 0x0200b2b0
	movs	r1, #176
	movs	r0, #8
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200b2b0
	movs	r1, #128
	movs	r0, #8
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200b2b0
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #20
	movs	r0, #8
	bl 0x0200b2c0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #8
	adds	r1, #204
	adds	r2, #102
	bl 0x0200b1e0
	movs	r1, #208
	movs	r2, #170
	lsls	r2, r2, #2
	movs	r0, #8
	lsls	r1, r1, #1
	bl 0x0200b218
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #128
	movs	r2, #20
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r1, #129
	lsls	r1, r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200b2c8
	movs	r0, #20
	bl 0x0200b1b0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	ldr	r1, [r5, #0]
	movs	r2, #0
	movs	r0, #8
	bl 0x0200b270
	movs	r0, #20
	bl 0x0200b1b0
	movs	r2, #0
	ldr	r0, [r5, #0]
	movs	r1, #8
	bl 0x0200b270
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #8
	movs	r1, #2
	bl 0x0200b240
	ldr	r0, [r5, #0]
	bl 0x0200b1d8
	cmp	r0, #0
	beq.n	.L_02000280
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #8
	bl 0x0200b208
.L_02000280:
	movs	r0, #8
	bl 0x0200b220
	movs	r2, #0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b228
	adds	r0, r6, #0
	movs	r1, #1
	adds	r0, #8
	bl 0x0200b1a0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #99
	bl 0x0200b158
	bl 0x0200b1c0
	add	sp, #8
	pop	{r5, r6, pc}
	.4byte 0x000016a2
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	sub	sp, #8
	bl 0x0200b1b8
	movs	r0, #0
	bl 0x0200b310
	movs	r1, #1
	movs	r0, #5
	bl 0x0200b268
	movs	r0, #40
	bl 0x0200b1b0
	movs	r1, #2
	movs	r0, #5
	bl 0x0200b268
	movs	r0, #20
	bl 0x0200b1b0
	ldr	r0, [pc, #432]
	bl 0x0200b288
	movs	r0, #5
	movs	r1, #0
	movs	r2, #40
	bl 0x0200b298
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r3, #17
	movs	r2, #46
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r2, #2
	movs	r1, #31
	movs	r0, #38
	bl 0x0200b190
	movs	r0, #5
	bl 0x0200b1d8
	movs	r1, #1
	adds	r5, r0, #0
	movs	r0, #5
	bl 0x0200b240
	movs	r0, #5
	bl 0x0200b1d8
	movs	r1, #1
	bl 0x0200b198
	movs	r0, #1
	bl 0x0200b138
	movs	r0, #5
	bl 0x0200b1d8
	movs	r1, #1
	bl 0x0200b198
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	movs	r3, #3
	adds	r5, #85
	strb	r3, [r5, #0]
	movs	r0, #40
	bl 0x0200b138
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #20
	bl 0x0200b2b0
	movs	r1, #160
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200b2b0
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200b2b0
	movs	r2, #20
	movs	r0, #5
	movs	r1, #4
	bl 0x0200b258
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	ldr	r5, [pc, #240]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r1, [r5, #0]
	movs	r2, #0
	movs	r0, #5
	bl 0x0200b270
	movs	r0, #20
	bl 0x0200b1b0
	ldr	r0, [r5, #0]
	movs	r1, #5
	movs	r2, #0
	bl 0x0200b270
	movs	r1, #132
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #3
	ldr	r0, [r5, #0]
	bl 0x0200b248
	movs	r0, #40
	bl 0x0200b1b0
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200b2c0
	movs	r1, #0
	movs	r0, #5
	bl 0x0200b290
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200b1d0
	cmp	r0, #0
	bne.n	.L_02000416
	movs	r0, #5
	movs	r1, #3
	bl 0x0200b240
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	b.n	.L_02000430
.L_02000416:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #5
	adds	r3, #2
	strh	r3, [r2, #0]
	movs	r1, #0
	bl 0x0200b2a0
.L_02000430:
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	movs	r0, #5
	ldr	r1, [pc, #92]
	bl 0x0200b1e0
	movs	r0, #5
	movs	r1, #2
	bl 0x0200b240
	ldr	r3, [pc, #76]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b1d8
	cmp	r0, #0
	beq.n	.L_02000466
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x0200b208
.L_02000466:
	movs	r0, #5
	bl 0x0200b220
	movs	r2, #0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b228
	movs	r1, #1
	movs	r0, #5
	bl 0x0200b1c8
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #100
	bl 0x0200b158
	bl 0x0200b1c0
	add	sp, #8
	pop	{r5, pc}
	.4byte 0x000016ab
	.4byte 0x02000240
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, lr}
	sub	sp, #8
	bl 0x0200b1b8
	movs	r0, #0
	bl 0x0200b310
	movs	r1, #2
	movs	r0, #6
	bl 0x0200b268
	ldr	r0, [pc, #384]
	bl 0x0200b288
	movs	r2, #40
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b298
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #6
	movs	r1, #18
	bl 0x0200b240
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r3, #14
	movs	r2, #40
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #31
	movs	r2, #2
	movs	r3, #1
	movs	r0, #38
	bl 0x0200b190
	movs	r0, #6
	bl 0x0200b1d8
	movs	r3, #3
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r2, #0
	movs	r0, #6
	movs	r1, #4
	bl 0x0200b258
	movs	r1, #1
	movs	r0, #6
	bl 0x0200b240
	movs	r0, #6
	bl 0x0200b1d8
	movs	r1, #1
	bl 0x0200b198
	movs	r0, #20
	bl 0x0200b1b0
	ldr	r5, [pc, #280]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #6
	bl 0x0200b278
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200b248
	movs	r1, #6
	movs	r2, #20
	adds	r1, #255
	movs	r0, #6
	bl 0x0200b2c0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #224
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200b2b0
	movs	r1, #160
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200b2b0
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #6
	bl 0x0200b2c0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #6
	bl 0x0200b270
	movs	r0, #20
	bl 0x0200b1b0
	movs	r0, #6
	movs	r1, #3
	bl 0x0200b240
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200b248
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #120]
	adds	r2, #204
	bl 0x0200b1e0
	movs	r2, #170
	movs	r0, #6
	movs	r1, #212
	lsls	r2, r2, #2
	bl 0x0200b218
	movs	r1, #224
	movs	r2, #20
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #6
	movs	r1, #2
	bl 0x0200b240
	ldr	r0, [r5, #0]
	bl 0x0200b1d8
	cmp	r0, #0
	beq.n	.L_0200060a
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #6
	bl 0x0200b208
.L_0200060a:
	movs	r0, #6
	bl 0x0200b220
	movs	r2, #0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b228
	movs	r1, #1
	movs	r0, #6
	bl 0x0200b1c8
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #101
	bl 0x0200b158
	bl 0x0200b1c0
	add	sp, #8
	pop	{r5, pc}
	.4byte 0x000016b5
	.4byte 0x02000240
	.2byte 0x9999
	.2byte 0x0001
	push	{lr}
	bl 0x0200b1b8
	movs	r0, #0
	bl 0x0200b310
	movs	r0, #123
	bl 0x0200b328
	bl 0x0200b300
	bl 0x0200b308
	movs	r0, #3
	bl 0x0200b2e8
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200b150
	cmp	r0, #0
	bne.n	.L_020006a0
	ldr	r5, [pc, #48]
	movs	r3, #133
	lsls	r3, r3, #2
	movs	r1, #204
	movs	r2, #204
	adds	r5, r5, r3
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200b1e0
	movs	r2, #189
	ldr	r0, [r5, #0]
	movs	r1, #152
	lsls	r2, r2, #2
	bl 0x0200b218
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200b158
.L_020006a0:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #99
	sub	sp, #8
	bl 0x0200b150
	cmp	r0, #0
	beq.n	.L_020006bc
	b.n	.L_0200087e
.L_020006bc:
	bl 0x0200b1b8
	movs	r0, #0
	bl 0x0200b310
	bl 0x02008664
	ldr	r6, [pc, #984]
	adds	r0, r6, #0
	bl 0x0200b288
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b228
	movs	r2, #0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b228
	movs	r0, #153
	movs	r1, #152
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #153
	adds	r1, #51
	bl 0x0200b2d0
	movs	r0, #168
	movs	r1, #1
	lsls	r0, r0, #16
	negs	r1, r1
	ldr	r2, [pc, #928]
	movs	r3, #1
	bl 0x0200b2d8
	ldr	r3, [pc, #924]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	movs	r1, #224
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b2b0
	movs	r3, #25
	movs	r2, #42
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r1, #31
	movs	r2, #2
	movs	r0, #38
	bl 0x0200b190
	movs	r0, #8
	bl 0x02008098
	movs	r1, #156
	lsls	r1, r1, #17
	ldr	r2, [pc, #876]
	movs	r0, #8
	bl 0x0200b228
	movs	r0, #1
	bl 0x0200b138
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #8
	ldr	r1, [pc, #860]
	adds	r2, #204
	bl 0x0200b1e0
	movs	r2, #170
	movs	r0, #8
	movs	r1, #212
	lsls	r2, r2, #2
	bl 0x0200b218
	movs	r2, #178
	movs	r0, #8
	movs	r1, #192
	lsls	r2, r2, #2
	bl 0x0200b218
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #8
	bl 0x0200b2b0
	movs	r0, #8
	bl 0x0200b1f0
	ldr	r1, [pc, #816]
	movs	r0, #8
	bl 0x0200b200
	movs	r2, #20
	movs	r0, #8
	movs	r1, #4
	bl 0x0200b258
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #2
	movs	r2, #0
	adds	r1, #255
	movs	r0, #8
	bl 0x0200b2c0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x0200b2c0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #208
	movs	r2, #0
	movs	r0, #8
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #160
	movs	r2, #0
	movs	r0, #8
	lsls	r1, r1, #7
	bl 0x0200b2b0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #8
	movs	r1, #4
	bl 0x0200b240
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r2, #189
	movs	r0, #8
	movs	r1, #172
	lsls	r2, r2, #2
	bl 0x0200b218
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #8
	bl 0x0200b278
	movs	r0, #8
	movs	r1, #3
	bl 0x0200b240
	movs	r0, #192
	lsls	r0, r0, #7
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #8
	movs	r1, #2
	bl 0x0200b240
	ldr	r0, [r5, #0]
	bl 0x0200b1d8
	cmp	r0, #0
	beq.n	.L_0200084a
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #8
	bl 0x0200b208
.L_0200084a:
	movs	r0, #8
	bl 0x0200b220
	movs	r2, #0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b228
	adds	r0, r6, #0
	adds	r0, #8
	movs	r1, #1
	bl 0x0200b1a0
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	movs	r2, #20
	bl 0x0200b2b0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #99
	bl 0x0200b158
	bl 0x0200b1c0
.L_0200087e:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #100
	bl 0x0200b150
	cmp	r0, #0
	beq.n	.L_0200088e
	b.n	.L_02000ac8
.L_0200088e:
	bl 0x0200b1b8
	movs	r0, #0
	bl 0x0200b310
	bl 0x02008664
	movs	r1, #156
	ldr	r2, [pc, #528]
	lsls	r1, r1, #17
	movs	r0, #5
	bl 0x0200b228
	movs	r0, #1
	bl 0x0200b138
	ldr	r0, [pc, #524]
	bl 0x0200b288
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r2, #0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b228
	movs	r0, #153
	movs	r1, #152
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #153
	adds	r1, #51
	bl 0x0200b2d0
	movs	r0, #168
	movs	r1, #1
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	ldr	r2, [pc, #452]
	bl 0x0200b2d8
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	movs	r0, #8
	ldr	r1, [pc, #452]
	bl 0x0200b1e0
	ldr	r1, [pc, #456]
	movs	r0, #8
	bl 0x0200b1e8
	ldr	r6, [pc, #428]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r6, r2
	movs	r1, #224
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b2b0
	movs	r3, #17
	movs	r2, #46
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r1, #31
	movs	r2, #2
	movs	r0, #38
	bl 0x0200b190
	movs	r0, #5
	bl 0x02008098
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #5
	ldr	r1, [pc, #384]
	adds	r2, #204
	bl 0x0200b1e0
	movs	r2, #170
	movs	r0, #5
	movs	r1, #212
	lsls	r2, r2, #2
	bl 0x0200b218
	movs	r2, #178
	movs	r0, #5
	movs	r1, #192
	lsls	r2, r2, #2
	bl 0x0200b218
	movs	r1, #192
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #5
	bl 0x0200b2b0
	movs	r0, #5
	bl 0x0200b1f0
	movs	r1, #4
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200b2c0
	movs	r2, #20
	movs	r0, #5
	movs	r1, #4
	bl 0x0200b258
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x0200b2c8
	movs	r1, #129
	movs	r0, #8
	lsls	r1, r1, #1
	bl 0x0200b2c8
	movs	r1, #8
	movs	r2, #20
	adds	r1, #255
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #6
	movs	r2, #0
	adds	r1, #255
	movs	r0, #8
	bl 0x0200b2c0
	ldr	r0, [r6, #0]
	movs	r1, #3
.L_020009c0:
	bl 0x0200b248
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x0200b248
	movs	r0, #8
	movs	r1, #0
	movs	r2, #40
	bl 0x0200b298
	movs	r1, #6
	movs	r2, #40
	adds	r1, #255
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #224
	movs	r2, #40
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #192
	movs	r0, #8
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #160
	movs	r2, #20
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #8
	movs	r1, #3
	bl 0x0200b240
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x0200b248
	movs	r2, #182
	movs	r0, #5
	movs	r1, #178
	lsls	r2, r2, #2
	bl 0x0200b218
	movs	r1, #192
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #7
	bl 0x0200b2b0
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #208
	movs	r0, #8
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #224
	movs	r2, #20
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	bl 0x0200b2b0
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x0200b248
	ldr	r5, [pc, #84]
	movs	r0, #8
	adds	r1, r5, #0
	bl 0x0200b1e8
	movs	r0, #5
	adds	r1, r5, #0
	bl 0x0200b200
	movs	r0, #5
	movs	r1, #1
	bl 0x0200b1c8
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #7
	movs	r2, #20
	bl 0x0200b2b0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #100
	bl 0x0200b158
	bl 0x0200b1c0
	b.n	.L_02000ac8
	.4byte 0x000016bf
	.4byte 0x02d10000
	.4byte 0x02000240
	.4byte 0x029a0000
	.4byte 0x00019999
	.4byte 0x0200b650
	.4byte 0x000016c8
	.4byte 0x0200b734
	.2byte 0xb790
	.2byte 0x0200
.L_02000ac8:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #101
	bl 0x0200b150
	cmp	r0, #0
	beq.n	.L_02000ad8
	b.n	.L_02000cf4
.L_02000ad8:
	bl 0x0200b1b8
	movs	r0, #0
	bl 0x0200b310
	bl 0x02008664
	movs	r1, #156
	ldr	r2, [pc, #524]
	lsls	r1, r1, #17
	movs	r0, #6
	bl 0x0200b228
	movs	r0, #1
	bl 0x0200b138
	ldr	r0, [pc, #512]
	bl 0x0200b288
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #153
	movs	r1, #152
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #153
	adds	r1, #51
	bl 0x0200b2d0
	movs	r0, #168
	movs	r1, #1
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	ldr	r2, [pc, #476]
	bl 0x0200b2d8
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	movs	r0, #8
	ldr	r1, [pc, #468]
	bl 0x0200b1e0
	ldr	r1, [pc, #464]
	movs	r0, #8
	bl 0x0200b1e8
	ldr	r6, [pc, #460]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r6, r6, r3
	movs	r1, #224
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b2b0
	movs	r3, #14
	movs	r2, #40
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #31
	movs	r2, #2
	movs	r3, #1
	movs	r0, #38
	bl 0x0200b190
	movs	r0, #6
	bl 0x0200b1d8
	movs	r3, #3
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r2, #0
	movs	r0, #6
	movs	r1, #4
	bl 0x0200b258
	movs	r1, #1
	movs	r0, #6
	bl 0x0200b240
	movs	r0, #6
	bl 0x0200b1d8
	movs	r1, #1
	bl 0x0200b198
	movs	r0, #20
	bl 0x0200b1b0
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #360]
	adds	r2, #204
	bl 0x0200b1e0
	movs	r2, #170
	movs	r0, #6
	movs	r1, #212
	lsls	r2, r2, #2
	bl 0x0200b218
	movs	r2, #178
	movs	r0, #6
	movs	r1, #192
	lsls	r2, r2, #2
	bl 0x0200b218
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #6
	bl 0x0200b2b0
	movs	r0, #6
	bl 0x0200b1f0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x0200b2c8
	movs	r1, #8
	movs	r2, #20
	adds	r1, #255
	movs	r0, #6
	bl 0x0200b2c0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r2, #40
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b298
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #10
	movs	r2, #40
	adds	r1, #255
	ldr	r0, [r6, #0]
	bl 0x0200b2c0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #6
	movs	r1, #4
	bl 0x0200b240
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #6
	movs	r1, #3
	bl 0x0200b248
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #8
	movs	r1, #3
	bl 0x0200b240
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x0200b248
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200b2b0
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #6
	movs	r2, #40
	bl 0x0200b2b0
	movs	r1, #192
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200b2b0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #6
	bl 0x0200b2c0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	ldr	r0, [r6, #0]
	movs	r1, #4
	bl 0x0200b248
	movs	r0, #8
	movs	r1, #4
	bl 0x0200b248
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #6
	movs	r1, #3
	bl 0x0200b248
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #8
	movs	r1, #3
	bl 0x0200b240
	movs	r0, #8
	movs	r1, #3
	bl 0x0200b248
	ldr	r5, [pc, #76]
	movs	r0, #8
	adds	r1, r5, #0
	bl 0x0200b1e8
	movs	r0, #6
	adds	r1, r5, #0
	bl 0x0200b200
	movs	r0, #6
	movs	r1, #1
	bl 0x0200b1c8
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #7
	movs	r2, #20
	bl 0x0200b2b0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #101
	bl 0x0200b158
	bl 0x0200b1c0
.L_02000cf4:
	add	sp, #8
	pop	{r5, r6, pc}
	.4byte 0x029a0000
	.4byte 0x000016d1
	.4byte 0x02d10000
	.4byte 0x00019999
	.4byte 0x0200b734
	.4byte 0x02000240
	.2byte 0xb790
	.2byte 0x0200
	push	{r5, lr}
	bl 0x0200b1b8
	movs	r0, #0
	bl 0x0200b310
	movs	r0, #153
	movs	r1, #152
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #153
	adds	r1, #51
.L_02000d2c:
	bl 0x0200b2d0
	movs	r0, #216
	movs	r1, #1
	movs	r2, #180
	movs	r3, #1
	lsls	r0, r0, #15
	negs	r1, r1
	lsls	r2, r2, #18
	bl 0x0200b2d8
	ldr	r5, [pc, #852]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200b1e0
	movs	r2, #178
	lsls	r2, r2, #2
	ldr	r0, [r5, #0]
	movs	r1, #86
	bl 0x0200b218
	ldr	r1, [r5, #0]
	movs	r0, #8
	bl 0x0200b238
	movs	r0, #1
	bl 0x0200b138
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #8
	adds	r1, #204
	adds	r2, #102
	bl 0x0200b1e0
	movs	r2, #178
	movs	r0, #8
	movs	r1, #104
	lsls	r2, r2, #2
	bl 0x0200b218
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #8
	movs	r0, #8
	bl 0x0200b2b0
	ldr	r0, [pc, #752]
	bl 0x0200b288
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #8
	movs	r1, #0
	movs	r2, #40
	bl 0x0200b2b0
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	movs	r2, #20
	bl 0x0200b2b0
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b2b0
	movs	r2, #182
	ldr	r0, [r5, #0]
	movs	r1, #104
	lsls	r2, r2, #2
	bl 0x0200b218
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #20
	bl 0x0200b2b0
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #6
	movs	r2, #20
	bl 0x0200b2b0
	movs	r2, #0
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200b2b0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #192
	movs	r0, #8
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #192
	movs	r2, #20
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #8
	movs	r1, #3
	bl 0x0200b240
	movs	r1, #0
	movs	r0, #8
	bl 0x0200b2a8
	movs	r1, #131
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x0200b2c0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	ldr	r1, [r5, #0]
	movs	r0, #5
	bl 0x0200b238
	ldr	r1, [r5, #0]
	movs	r0, #6
	bl 0x0200b238
	movs	r0, #1
	bl 0x0200b138
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #6
	adds	r1, #204
	adds	r2, #102
	bl 0x0200b1e0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #5
	adds	r1, #204
	adds	r2, #102
	bl 0x0200b1e0
	movs	r2, #178
	movs	r0, #5
	movs	r1, #86
	lsls	r2, r2, #2
	bl 0x0200b210
	movs	r2, #182
	movs	r0, #6
	movs	r1, #86
	lsls	r2, r2, #2
	bl 0x0200b218
	movs	r1, #224
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #6
	bl 0x0200b2b0
	movs	r0, #5
	bl 0x0200b220
	movs	r0, #5
	movs	r1, #1
	bl 0x0200b240
	movs	r1, #128
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #7
	bl 0x0200b2b0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b2b0
	movs	r0, #6
	movs	r1, #0
	movs	r2, #40
	bl 0x0200b2b0
	movs	r1, #224
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #192
	movs	r2, #0
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #8
	movs	r1, #4
.L_02000efe:
	bl 0x0200b240
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #8
	bl 0x0200b290
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b2b0
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200b1d0
	cmp	r0, #0
	bne.n	.L_02000f58
	movs	r0, #8
	movs	r1, #4
	bl 0x0200b248
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02000f7e
.L_02000f58:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #8
	adds	r3, #1
	movs	r1, #3
	strh	r3, [r2, #0]
	bl 0x0200b248
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
.L_02000f7e:
	movs	r2, #0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2b0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #6
	bl 0x0200b2c0
	movs	r1, #192
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #160
	movs	r2, #0
	movs	r0, #8
	lsls	r1, r1, #7
	bl 0x0200b2b0
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	ldr	r3, [pc, #204]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r1, #2
	ldr	r0, [r3, #0]
	adds	r1, #255
	movs	r2, #40
	bl 0x0200b2c0
	movs	r1, #192
	movs	r2, #0
	movs	r0, #8
	lsls	r1, r1, #6
	bl 0x0200b2b0
	movs	r0, #8
	movs	r1, #4
	bl 0x0200b240
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #128
	movs	r2, #20
	movs	r0, #8
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #8
	movs	r1, #3
	bl 0x0200b248
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r2, #20
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2b0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #0
	movs	r0, #5
	bl 0x0200b290
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b1d0
	cmp	r0, #0
	bne.n	.L_020010a0
	movs	r1, #192
	movs	r2, #20
	movs	r0, #8
	lsls	r1, r1, #6
	bl 0x0200b2b0
	movs	r0, #8
	movs	r1, #3
	bl 0x0200b248
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020010da
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x16de
	.2byte 0x0000
.L_020010a0:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r1, #4
	adds	r3, #1
	strh	r3, [r2, #0]
	adds	r1, #255
	movs	r2, #0
	movs	r0, #8
	bl 0x0200b2c0
	movs	r1, #128
	movs	r0, #8
	lsls	r1, r1, #7
	movs	r2, #20
	bl 0x0200b2b0
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	bl 0x0200b320
.L_020010da:
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #8
	bl 0x0200b2c0
	movs	r1, #160
	movs	r2, #0
	movs	r0, #8
	lsls	r1, r1, #7
	bl 0x0200b2b0
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #6
	bl 0x0200b2c0
	movs	r1, #224
	movs	r2, #40
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #128
	movs	r2, #20
	movs	r0, #5
	lsls	r1, r1, #7
	bl 0x0200b2b0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #192
.L_02001148:
	movs	r0, #8
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b2b0
	ldr	r5, [pc, #804]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200b2b0
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #8
	bl 0x0200b290
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200b1d0
	cmp	r0, #0
	bne.n	.L_020011b0
	movs	r1, #4
	adds	r1, #255
	movs	r2, #0
	movs	r0, #6
	bl 0x0200b2c0
	movs	r2, #20
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2b0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020011e0
.L_020011b0:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r1, #132
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #6
	bl 0x0200b2c0
	movs	r0, #6
	movs	r1, #0
	movs	r2, #20
	bl 0x0200b2b0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
.L_020011e0:
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x0200b2c0
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #224
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #6
	movs	r1, #3
	bl 0x0200b248
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #2
	movs	r2, #40
	adds	r1, #255
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	ldr	r5, [pc, #588]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	movs	r1, #6
	bl 0x0200b318
	movs	r0, #8
	movs	r1, #6
	bl 0x0200b318
	movs	r0, #5
	movs	r1, #6
	bl 0x0200b318
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #6
	adds	r1, #102
	adds	r2, #51
	bl 0x0200b1e0
	movs	r2, #187
	movs	r1, #86
	lsls	r2, r2, #2
	movs	r0, #6
	bl 0x0200b218
	movs	r0, #40
	bl 0x0200b1b0
	movs	r1, #10
	movs	r2, #80
	adds	r1, #255
	movs	r0, #6
	bl 0x0200b2c0
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #8
	bl 0x0200b2a0
	ldr	r0, [r5, #0]
	bl 0x0200b1f8
	movs	r0, #8
	bl 0x0200b1f8
	movs	r0, #5
	bl 0x0200b1f8
	movs	r1, #3
	movs	r0, #6
	bl 0x0200b248
	movs	r0, #40
	bl 0x0200b1b0
	movs	r0, #5
	movs	r1, #4
	bl 0x0200b240
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #6
	bl 0x0200b2b0
	movs	r0, #6
	bl 0x0200b1f0
	movs	r0, #6
	movs	r1, #4
	bl 0x0200b240
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #192
	movs	r0, #8
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200b2b0
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #192
	movs	r2, #20
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	bl 0x0200b2b0
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #6
	movs	r1, #3
	bl 0x0200b248
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #8
	movs	r1, #3
	bl 0x0200b240
	movs	r0, #5
	movs	r1, #3
	bl 0x0200b240
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200b248
	movs	r1, #224
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #192
	movs	r2, #0
	movs	r0, #8
	lsls	r1, r1, #6
	bl 0x0200b2b0
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #8
	bl 0x0200b2a8
	movs	r1, #160
	movs	r2, #40
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #128
	movs	r2, #20
	movs	r0, #8
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #6
	movs	r1, #4
	bl 0x0200b240
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #8
	bl 0x0200b2c0
	movs	r1, #6
	movs	r2, #40
	adds	r1, #255
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #192
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #129
	movs	r0, #8
	lsls	r1, r1, #1
	bl 0x0200b2c8
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #20
	movs	r0, #6
	bl 0x0200b2c0
	movs	r1, #224
	movs	r2, #20
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #8
	movs	r1, #3
	bl 0x0200b248
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #0
	movs	r0, #5
	bl 0x0200b290
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200b1d0
	cmp	r0, #0
	bne.n	.L_0200147c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200b240
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_0200149e
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
.L_0200147c:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #5
	adds	r3, #1
	movs	r1, #4
	strh	r3, [r2, #0]
	bl 0x0200b240
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
.L_0200149e:
	movs	r1, #10
	movs	r2, #40
	adds	r1, #255
	movs	r0, #8
	bl 0x0200b2c0
	movs	r1, #4
	movs	r0, #6
	bl 0x0200b248
	movs	r0, #20
	bl 0x0200b1b0
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #6
	movs	r1, #3
	bl 0x0200b248
	movs	r1, #128
	movs	r2, #20
	movs	r0, #8
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r2, #20
	movs	r0, #8
	movs	r1, #4
	bl 0x0200b258
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #102
	movs	r0, #6
	adds	r1, #204
	bl 0x0200b1e0
	ldr	r5, [pc, #40]
	movs	r0, #8
	adds	r1, r5, #0
	bl 0x0200b1e8
	adds	r1, r5, #0
	movs	r0, #5
	bl 0x0200b1e8
	adds	r1, r5, #0
	movs	r0, #6
	bl 0x0200b200
	movs	r0, #237
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200b158
	bl 0x0200b1c0
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x0200b7c4
	.global Func_0200153c
	.thumb_func
Func_0200153c:
	ldr r2, [pc, #16]
	ldr r3, [pc, #12]
	strh r3, [r2]
	ldr r2, [pc, #16]
	movs r3, #0
	str r3, [r2]
	bx lr
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200bc14
	.4byte 0x0200bc10
	.4byte 0x4a0c4b0d
	.4byte 0x4053881b
	.4byte 0x141b041b
	.4byte 0x18c00098
	.4byte 0x01804b0a
	.4byte 0x220018c0
	.4byte 0x21805e83
	.4byte 0x311e04c9
	.4byte 0x2380800b
	.4byte 0x33b004db
	.4byte 0x4a053002
	.4byte 0x3b0cc307
	.4byte 0x0000e008
	.4byte 0x00000001
	.4byte 0x0200bc14
	.4byte 0x0200b990
	.4byte 0xa2600001
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r1, #132
	lsls	r1, r1, #1
	adds	r1, r1, r3
	ldr	r3, [pc, #92]
	mov	r8, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	movs	r6, #0
	lsls	r3, r2, #2
	adds	r3, r3, r2
	ldr	r2, [pc, #80]
	lsls	r3, r3, #6
	adds	r5, r3, r2
.L_020015c6:
	ldr	r7, [pc, #80]
	mov	r1, r8
	ldrb	r0, [r7, #0]
	movs	r2, #6
	ldrsh	r3, [r1, r2]
	subs	r0, r0, r6
	subs	r0, r0, r3
	adds	r0, #160
	lsls	r0, r0, #9
	bl 0x0200b148
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r0, r0, r2
	mov	r2, r8
	movs	r1, #6
	ldrsh	r3, [r2, r1]
	asrs	r0, r0, #15
	adds	r3, r3, r0
	adds	r6, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r6, #160
	bne.n	.L_020015c6
	ldr	r1, [pc, #24]
	ldr	r2, [pc, #16]
	ldrh	r3, [r1, #0]
	eors	r3, r2
	strh	r3, [r1, #0]
	ldr	r3, [r7, #0]
	adds	r3, #1
	str	r3, [r7, #0]
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x00000001
	.4byte 0x0200bc14
	.4byte 0x0200b990
	.2byte 0xbc10
	.2byte 0x0200
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #128
	ldr	r0, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r1, r0, #0
	adds	r2, r0, r3
	movs	r4, #0
.L_02001630:
	ldrh	r3, [r2, #0]
	adds	r4, #1
	strh	r3, [r1, #0]
	adds	r2, #2
	adds	r1, #2
	cmp	r4, #63
	bls.n	.L_02001630
	movs	r3, #0
	strh	r3, [r0, #0]
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200b2f0
	pop	{pc}
	.2byte 0x0000
	.global Func_02001650
	.thumb_func
Func_02001650:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #132
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #7
	str r2, [r3, #24]
	movs r2, #128
	lsls r2, r2, #6
	sub sp, #8
	str r2, [r3, #28]
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r2, #75
	movs r3, #8
	movs r0, #72
	movs r1, #10
	bl 0x0200b188
	movs r0, #9
	movs r1, #2
	bl 0x0200b240
	sub sp, #-8
	pop {pc}
	.2byte 0x0000
	.global Func_0200168c
	.thumb_func
Func_0200168c:
	push {r5, r6, r7, lr}
	adds r4, r0, #0
	adds r6, r2, #0
	adds r5, r1, #0
	lsls r3, r3, #16
	movs r0, #244
	asrs r7, r3, #16
	lsls r0, r0, #1
	adds r3, r6, #0
	adds r1, r4, #0
	adds r2, r5, #0
	bl 0x0200b178
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200168c_0
	movs r0, #151
	ldr r5, [r6, #80]
	bl 0x0200b328
	adds r0, r6, #0
	movs r1, #1
	bl 0x0200b168
	ldr r1, [pc, #20]
	adds r0, r6, #0
	bl 0x0200b170
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	strb r3, [r5, #26]
	strh r7, [r5, #18]
.L_0200168c_0:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200b97c
	push	{r5, r6, lr}
	mov	r6, fp
	mov	r5, sl
	push	{r5, r6}
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6}
	bl 0x0200b1b8
	movs	r0, #0
	bl 0x0200b310
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200b2d8
	movs	r0, #1
	bl 0x0200b138
	movs	r0, #252
	movs	r1, #1
	movs	r2, #216
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #0
	lsls	r0, r0, #16
	bl 0x0200b2d8
	movs	r0, #1
	bl 0x0200b138
	bl 0x0200b180
	ldr	r3, [pc, #1016]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b228
	movs	r3, #128
	movs	r1, #184
	movs	r2, #168
	lsls	r3, r3, #7
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	movs	r0, #5
	mov	sl, r3
	bl 0x0200b230
	movs	r0, #8
	bl 0x0200b1d8
	movs	r2, #128
	lsls	r2, r2, #8
	mov	r9, r2
	mov	r3, r9
	movs	r1, #186
	movs	r2, #164
	strh	r3, [r0, #6]
	lsls	r2, r2, #16
	lsls	r1, r1, #16
	movs	r0, #10
	bl 0x0200b228
	movs	r0, #10
	bl 0x0200b1d8
	movs	r1, #15
	bl 0x0200b280
	movs	r0, #10
	bl 0x0200b1d8
	movs	r1, #0
	bl 0x0200b198
	movs	r0, #1
	bl 0x0200b138
	bl 0x0200b2f8
	movs	r0, #153
	movs	r1, #152
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #153
	adds	r1, #51
	bl 0x0200b2d0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #5
	adds	r1, #204
	adds	r2, #102
	bl 0x0200b1e0
	movs	r0, #5
	movs	r1, #204
	movs	r2, #196
	bl 0x0200b218
	movs	r0, #5
	movs	r1, #208
	movs	r2, #218
	bl 0x0200b218
	movs	r1, #192
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #7
	bl 0x0200b2b0
	movs	r1, #5
	movs	r0, #8
	bl 0x0200b238
	movs	r0, #1
	bl 0x0200b138
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #8
	adds	r1, #204
	adds	r2, #102
	bl 0x0200b1e0
	movs	r0, #8
	movs	r1, #186
	movs	r2, #216
	bl 0x0200b218
	movs	r1, #192
	movs	r0, #8
	lsls	r1, r1, #6
	movs	r2, #60
	bl 0x0200b2b0
	mov	r1, r9
	movs	r0, #8
	movs	r2, #40
	bl 0x0200b2b0
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	movs	r2, #20
	bl 0x0200b2b0
	movs	r1, #192
	movs	r0, #8
	lsls	r1, r1, #6
	movs	r2, #40
	bl 0x0200b2b0
	movs	r2, #0
	movs	r1, #0
	movs	r0, #8
	bl 0x0200b2b0
	ldr	r0, [pc, #756]
	bl 0x0200b288
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r1, #0
	adds	r0, #8
	bl 0x0200b290
	movs	r2, #0
	mov	r1, r9
	movs	r0, #5
	bl 0x0200b2b0
	movs	r1, #0
	movs	r0, #5
	bl 0x0200b1d0
	movs	r0, #5
	bl 0x0200b250
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #5
	movs	r1, #3
	bl 0x0200b248
	movs	r1, #208
	movs	r0, #8
	lsls	r1, r1, #8
	movs	r2, #80
	bl 0x0200b2b0
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b2b0
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #8
	movs	r1, #0
	movs	r2, #40
	bl 0x0200b298
	movs	r1, #131
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x0200b2c0
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #5
	movs	r1, #3
	bl 0x0200b248
	movs	r2, #0
	movs	r0, #8
	movs	r1, #4
	bl 0x0200b258
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #5
	movs	r1, #3
	bl 0x0200b248
	movs	r0, #8
	movs	r1, #3
	bl 0x0200b248
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #5
	adds	r1, #204
	adds	r2, #102
	bl 0x0200b1e0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #102
	movs	r0, #8
	adds	r1, #204
	bl 0x0200b1e0
	ldr	r1, [pc, #556]
	movs	r0, #8
	bl 0x0200b1e8
	movs	r0, #40
	bl 0x0200b1b0
	ldr	r1, [pc, #544]
	movs	r0, #5
	bl 0x0200b200
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #20
	movs	r0, #8
	bl 0x0200b2c0
	mov	r1, r9
	movs	r0, #8
	movs	r2, #40
	bl 0x0200b2b0
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b2b0
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #8
	ldr	r1, [pc, #492]
	adds	r2, #204
	bl 0x0200b1e0
	movs	r1, #129
	movs	r2, #172
	movs	r0, #8
	lsls	r1, r1, #1
	bl 0x0200b218
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #5
	movs	r1, #3
	bl 0x0200b248
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #5
	adds	r1, #102
	adds	r2, #51
	bl 0x0200b1e0
	movs	r0, #5
	movs	r1, #242
	movs	r2, #172
	bl 0x0200b218
	movs	r0, #8
	movs	r1, #248
	movs	r2, #156
	bl 0x0200b218
	movs	r0, #8
	movs	r1, #226
	movs	r2, #164
	bl 0x0200b218
	movs	r0, #8
	movs	r1, #228
	movs	r2, #180
	bl 0x0200b218
	movs	r1, #192
	movs	r2, #40
	movs	r0, #8
	lsls	r1, r1, #6
	bl 0x0200b2b0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #8
	movs	r1, #226
	movs	r2, #164
	bl 0x0200b218
	movs	r0, #8
	movs	r1, #248
	movs	r2, #156
	bl 0x0200b218
	movs	r1, #129
	movs	r0, #8
	lsls	r1, r1, #1
	movs	r2, #172
	bl 0x0200b218
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	movs	r2, #20
	bl 0x0200b2b0
	movs	r1, #6
	movs	r2, #20
	adds	r1, #255
	movs	r0, #8
	bl 0x0200b2c0
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b2b0
	movs	r0, #8
	movs	r1, #6
	movs	r2, #0
	bl 0x0200b258
	movs	r1, #128
	movs	r0, #8
	lsls	r1, r1, #1
	movs	r2, #198
	bl 0x0200b218
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b228
	mov	r1, sl
	movs	r0, #5
	movs	r2, #20
	bl 0x0200b2b0
	movs	r1, #6
	adds	r1, #255
	movs	r2, #40
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #5
	movs	r1, #0
	movs	r2, #20
	bl 0x0200b298
	movs	r0, #5
	movs	r1, #224
	movs	r2, #182
	bl 0x0200b218
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #6
	movs	r2, #40
	bl 0x0200b2b0
	movs	r1, #6
	adds	r1, #255
	movs	r2, #80
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #5
	movs	r1, #242
	movs	r2, #172
	bl 0x0200b218
	mov	r1, sl
	movs	r0, #5
	movs	r2, #40
	bl 0x0200b2b0
	movs	r1, #6
	adds	r1, #255
	movs	r2, #120
	movs	r0, #5
	bl 0x0200b2c0
	movs	r1, #8
	adds	r1, #255
	movs	r2, #40
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #5
	movs	r1, #2
	movs	r2, #10
	bl 0x0200b258
	movs	r0, #5
	movs	r1, #4
	movs	r2, #20
	bl 0x0200b258
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r2, #40
	adds	r0, #5
	movs	r1, #0
	bl 0x0200b298
	movs	r0, #153
	movs	r1, #152
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #153
	adds	r1, #51
	bl 0x0200b2d0
	movs	r0, #131
	movs	r1, #1
	movs	r2, #180
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #16
	bl 0x0200b2d8
	movs	r1, #128
	movs	r2, #198
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	movs	r0, #8
	bl 0x0200b228
	movs	r0, #1
	bl 0x0200b138
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b2b0
	movs	r0, #8
	movs	r1, #6
	movs	r2, #0
	bl 0x0200b258
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #172
	movs	r0, #8
	bl 0x0200b218
	b.n	.L_02001b34
	.4byte 0x02000240
	.4byte 0x00001619
	.4byte 0x0200b330
	.4byte 0x0200b36c
	.2byte 0x9999
	.2byte 0x0001
.L_02001b34:
	movs	r0, #40
	bl 0x0200b1b0
	movs	r1, #6
	movs	r2, #80
	adds	r1, #255
	movs	r0, #8
	bl 0x0200b2c0
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #8
	movs	r1, #4
	bl 0x0200b248
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r2, #20
	adds	r0, #5
	movs	r1, #0
	bl 0x0200b298
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r2, #224
	lsls	r2, r2, #8
	movs	r5, #192
	movs	r6, #172
	mov	r8, r2
	lsls	r5, r5, #14
	lsls	r6, r6, #16
	movs	r0, #238
	adds	r1, r5, #0
	adds	r2, r6, #0
	mov	r3, r8
	lsls	r0, r0, #16
	bl 0x0200968c
	movs	r0, #10
	bl 0x0200b1b0
	movs	r0, #246
	movs	r3, #128
	lsls	r3, r3, #6
	adds	r2, r6, #0
	adds	r1, r5, #0
	lsls	r0, r0, #16
	mov	fp, r3
	bl 0x0200968c
	movs	r0, #30
	bl 0x0200b1b0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	movs	r2, #40
	bl 0x0200b2b0
	movs	r0, #192
	lsls	r0, r0, #7
	adds	r0, #8
	movs	r1, #0
	movs	r2, #20
	bl 0x0200b298
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #5
	adds	r1, #204
	adds	r2, #102
	bl 0x0200b1e0
	movs	r2, #148
	movs	r0, #5
	movs	r1, #248
	bl 0x0200b218
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #176
	movs	r0, #8
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #8
	bl 0x0200b2c0
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	mov	r1, sl
	movs	r0, #5
	movs	r2, #0
	bl 0x0200b2b0
	movs	r2, #20
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b298
	movs	r0, #8
	movs	r1, #4
	bl 0x0200b248
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #8
	adds	r1, #204
	adds	r2, #102
	bl 0x0200b1e0
	movs	r1, #244
	movs	r2, #170
	movs	r0, #8
	bl 0x0200b218
	movs	r0, #10
	bl 0x0200b1b0
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #170
	movs	r0, #8
	bl 0x0200b218
	movs	r0, #10
	bl 0x0200b1b0
	movs	r1, #137
	lsls	r1, r1, #1
	movs	r2, #170
	movs	r0, #8
	bl 0x0200b218
	movs	r0, #10
	bl 0x0200b1b0
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #170
	movs	r0, #8
	bl 0x0200b218
	movs	r0, #10
	bl 0x0200b1b0
	movs	r1, #244
	movs	r2, #170
	movs	r0, #8
	bl 0x0200b218
	movs	r0, #10
	bl 0x0200b1b0
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #170
	movs	r0, #8
	bl 0x0200b218
	movs	r0, #10
	bl 0x0200b1b0
	movs	r1, #137
	lsls	r1, r1, #1
	movs	r2, #170
	movs	r0, #8
	bl 0x0200b218
	movs	r0, #10
	bl 0x0200b1b0
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #170
	movs	r0, #8
	bl 0x0200b218
	movs	r0, #20
	bl 0x0200b1b0
	movs	r1, #131
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x0200b2c0
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #2
	movs	r2, #0
	adds	r1, #255
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #176
	movs	r2, #20
	movs	r0, #8
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #8
	movs	r1, #3
	bl 0x0200b248
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #5
	movs	r1, #3
	bl 0x0200b240
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #144
	lsls	r0, r0, #8
	adds	r0, #10
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b2b0
	movs	r2, #0
	mov	r1, r9
	movs	r0, #8
	bl 0x0200b2b0
	movs	r0, #10
	bl 0x0200b1d8
	movs	r1, #1
	bl 0x0200b198
	movs	r0, #10
	bl 0x0200b1d8
	movs	r1, #0
	bl 0x0200b280
	movs	r0, #10
	bl 0x0200b1b0
	movs	r2, #40
	movs	r0, #10
	movs	r1, #0
	bl 0x0200b2b0
	movs	r0, #10
	movs	r1, #3
	bl 0x0200b240
	movs	r0, #144
	lsls	r0, r0, #8
	adds	r0, #10
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #10
	adds	r1, #204
	adds	r2, #102
	bl 0x0200b1e0
	movs	r0, #10
	movs	r1, #216
	movs	r2, #164
	bl 0x0200b218
	movs	r1, #234
	movs	r2, #160
	movs	r0, #10
	bl 0x0200b218
	movs	r0, #40
	bl 0x0200b1b0
	movs	r1, #192
	movs	r2, #20
	movs	r0, #10
	lsls	r1, r1, #6
	bl 0x0200b2b0
	movs	r0, #10
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #208
	movs	r2, #0
	movs	r0, #10
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #10
	movs	r1, #0
	bl 0x0200b2a0
	mov	r1, r8
	movs	r0, #5
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #208
	movs	r2, #20
	movs	r0, #8
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #5
	movs	r1, #3
	bl 0x0200b248
	movs	r1, #2
	movs	r2, #40
	adds	r1, #255
	movs	r0, #8
	bl 0x0200b2c0
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	mov	r1, fp
	movs	r0, #5
	movs	r2, #0
	bl 0x0200b2b0
	movs	r2, #0
	movs	r0, #10
	movs	r1, #0
	bl 0x0200b2b0
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #10
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #160
	lsls	r0, r0, #8
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #208
	movs	r2, #40
	movs	r0, #10
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #10
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #192
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #7
	bl 0x0200b2b0
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #5
	mov	r1, r8
	movs	r2, #80
	bl 0x0200b2b0
	bl 0x0200b300
	bl 0x0200b308
	movs	r0, #11
	bl 0x0200b2e8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r3}
	mov	fp, r3
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	movs	r0, #5
	sub	sp, #28
	bl 0x0200b1d8
	adds	r6, r0, #0
	bl 0x0200b1b8
	movs	r0, #0
	bl 0x0200b310
	bl 0x0200961c
	bl 0x02009650
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	movs	r3, #0
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	bl 0x0200b2d8
	movs	r1, #222
	movs	r2, #212
	lsls	r2, r2, #16
	movs	r0, #6
	lsls	r1, r1, #17
	bl 0x0200b228
	movs	r1, #19
	movs	r0, #6
	bl 0x0200b240
	movs	r0, #6
	bl 0x0200b1d8
	movs	r1, #0
	bl 0x0200b198
	ldr	r5, [pc, #940]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r1, #216
	movs	r2, #234
	lsls	r2, r2, #16
	lsls	r1, r1, #17
	ldr	r0, [r5, #0]
	bl 0x0200b228
	ldr	r0, [r5, #0]
	bl 0x0200b1d8
	ldr	r3, [pc, #916]
	movs	r1, #41
	str	r3, [r0, #24]
	adds	r6, #100
	ldr	r0, [r5, #0]
	bl 0x0200b240
	ldr	r0, [r5, #0]
	bl 0x0200b1d8
	movs	r1, #0
	bl 0x0200b198
	movs	r1, #196
	movs	r2, #172
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	movs	r0, #10
	bl 0x0200b228
	movs	r0, #1
	bl 0x0200b138
	movs	r0, #192
	movs	r1, #1
	movs	r2, #214
	movs	r3, #0
	negs	r1, r1
	lsls	r2, r2, #16
	lsls	r0, r0, #16
	bl 0x0200b2d8
	bl 0x0200b180
	movs	r0, #1
	bl 0x0200b138
	movs	r1, #208
	movs	r2, #218
	lsls	r2, r2, #16
	movs	r0, #5
	lsls	r1, r1, #16
	bl 0x0200b228
	movs	r1, #19
	movs	r0, #5
	bl 0x0200b240
	movs	r0, #5
	bl 0x0200b1d8
	movs	r1, #0
	bl 0x0200b198
	movs	r1, #176
	movs	r2, #204
	lsls	r2, r2, #16
	movs	r0, #8
	lsls	r1, r1, #16
	bl 0x0200b228
	movs	r1, #5
	movs	r0, #8
	bl 0x0200b240
	movs	r0, #8
	bl 0x0200b1d8
	movs	r1, #0
	bl 0x0200b198
	movs	r0, #1
	bl 0x0200b138
	bl 0x0200b2f8
	bl 0x0200b308
	movs	r0, #120
	bl 0x0200b1b0
	ldr	r0, [pc, #756]
	bl 0x0200b288
	movs	r2, #40
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b298
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #8
	movs	r2, #0
	adds	r1, #255
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #6
	movs	r2, #80
	adds	r1, #255
	movs	r0, #8
	bl 0x0200b2c0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #6
	adds	r1, #255
	movs	r2, #40
	movs	r0, #8
	bl 0x0200b2c0
	movs	r1, #10
	movs	r2, #80
	adds	r1, #255
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #129
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x0200b2c8
	movs	r0, #40
	bl 0x0200b1b0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #6
	adds	r1, #255
	movs	r2, #40
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #8
	movs	r1, #0
	movs	r2, #40
	bl 0x0200b298
	movs	r1, #10
	adds	r1, #255
	movs	r2, #20
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #5
	movs	r1, #0
	movs	r2, #20
	bl 0x0200b298
	movs	r0, #8
	movs	r1, #0
	movs	r2, #40
	bl 0x0200b298
	movs	r1, #6
	movs	r2, #20
	adds	r1, #255
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #10
	adds	r1, #204
	adds	r2, #102
	bl 0x0200b1e0
	movs	r1, #152
	movs	r0, #10
	lsls	r1, r1, #1
	movs	r2, #172
	bl 0x0200b218
	movs	r0, #10
	movs	r1, #224
	movs	r2, #172
	bl 0x0200b218
	movs	r2, #192
	movs	r0, #10
	movs	r1, #208
	bl 0x0200b218
	movs	r0, #10
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #8
	bl 0x0200b2c0
	movs	r1, #2
	movs	r2, #80
	adds	r1, #255
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #10
	movs	r1, #0
	bl 0x0200b2a0
	ldr	r1, [pc, #440]
	movs	r0, #10
	bl 0x0200b1e8
	movs	r0, #40
	bl 0x0200b1b0
	movs	r1, #1
	movs	r0, #8
	bl 0x0200b240
	movs	r0, #8
	bl 0x0200b1d8
	movs	r1, #1
	bl 0x0200b198
	movs	r2, #20
	movs	r0, #8
	movs	r1, #4
	bl 0x0200b258
	movs	r1, #1
	movs	r0, #5
	bl 0x0200b240
	movs	r0, #5
	bl 0x0200b1d8
	movs	r1, #1
	bl 0x0200b198
	movs	r0, #5
	movs	r1, #4
	movs	r2, #20
	bl 0x0200b258
	movs	r1, #192
	movs	r0, #8
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #160
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200b2b0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #8
	adds	r1, #204
	adds	r2, #102
	bl 0x0200b1e0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #102
	movs	r0, #5
	adds	r1, #204
	bl 0x0200b1e0
	ldr	r1, [pc, #312]
	movs	r0, #8
	bl 0x0200b1e8
	movs	r0, #40
	bl 0x0200b1b0
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200b2d0
	movs	r0, #184
	movs	r1, #1
	movs	r2, #210
	lsls	r0, r0, #17
	negs	r1, r1
	movs	r3, #1
	lsls	r2, r2, #16
	bl 0x0200b2d8
	movs	r3, #0
	strh	r3, [r6, #0]
	ldr	r1, [pc, #264]
	movs	r0, #5
	bl 0x0200b1e8
.L_020021f2:
	movs	r0, #1
	bl 0x0200b138
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	beq.n	.L_020021f2
	movs	r0, #10
	bl 0x0200b1b0
	movs	r1, #129
	movs	r0, #8
	lsls	r1, r1, #1
	bl 0x0200b2c8
	movs	r1, #129
	movs	r0, #5
	lsls	r1, r1, #1
	bl 0x0200b2c8
	movs	r3, #12
	movs	r2, #8
	movs	r1, #9
	movs	r0, #2
	movs	r4, #4
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	str	r0, [sp, #12]
	movs	r3, #2
	movs	r5, #1
	movs	r6, #0
	movs	r0, #5
	movs	r1, #7
	movs	r2, #13
	str	r4, [sp, #16]
	str	r5, [sp, #20]
	str	r6, [sp, #24]
	bl 0x0200b2b8
	movs	r1, #192
	movs	r0, #10
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b2b0
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #8
	ldr	r1, [pc, #160]
	adds	r2, #204
	bl 0x0200b1e0
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	movs	r0, #5
	ldr	r1, [pc, #144]
	bl 0x0200b1e0
	ldr	r1, [pc, #144]
	movs	r0, #8
	bl 0x0200b1e8
	ldr	r1, [pc, #140]
	movs	r0, #5
	bl 0x0200b200
	movs	r0, #40
	bl 0x0200b1b0
	movs	r0, #160
	lsls	r0, r0, #7
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r2, #10
	movs	r0, #5
	movs	r1, #4
	bl 0x0200b258
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #176
	movs	r0, #8
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b2b0
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r1, #0
	adds	r0, #8
	movs	r2, #40
	bl 0x0200b298
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #218
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #120
	str	r2, [r3, #0]
	bl 0x0200b300
	bl 0x0200b308
	movs	r0, #12
	bl 0x0200b2e8
	add	sp, #28
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xffff0000
	.4byte 0x00001645
	.4byte 0x0200b3a8
	.4byte 0x0200b400
	.4byte 0x0200b46c
	.4byte 0x00019999
	.2byte 0xb4e4
	.2byte 0x0200
	push	{r2, r3, lr}
	lsls	r0, r0, #8
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #1016]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r6, r2
	ldr	r0, [r6, #0]
	sub	sp, #28
	bl 0x0200b1d8
	adds	r7, r0, #0
	bl 0x0200b1b8
	movs	r0, #0
	bl 0x0200b310
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	bl 0x0200b2d8
	bl 0x02009650
	movs	r3, #192
	movs	r1, #157
	movs	r2, #200
	lsls	r3, r3, #6
	movs	r0, #8
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	movs	r5, #208
	mov	r9, r3
	lsls	r5, r5, #8
	bl 0x0200b230
	movs	r1, #164
	movs	r2, #240
	adds	r3, r5, #0
	movs	r0, #10
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200b230
	movs	r2, #128
	lsls	r2, r2, #7
	mov	fp, r2
	movs	r1, #170
	movs	r2, #200
	movs	r0, #5
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	mov	r3, fp
	bl 0x0200b230
	movs	r1, #171
	movs	r2, #230
	lsls	r2, r2, #16
	movs	r3, #0
	movs	r0, #6
	lsls	r1, r1, #17
	bl 0x0200b230
	movs	r1, #19
	movs	r0, #6
	bl 0x0200b240
	movs	r0, #6
	bl 0x0200b1d8
	movs	r1, #0
	bl 0x0200b198
	movs	r3, #128
	movs	r1, #156
	movs	r2, #230
	lsls	r3, r3, #8
	lsls	r2, r2, #16
	ldr	r0, [r6, #0]
	lsls	r1, r1, #17
	mov	r8, r3
	bl 0x0200b230
	movs	r1, #41
	ldr	r0, [r6, #0]
	bl 0x0200b240
	ldr	r0, [r6, #0]
	bl 0x0200b1d8
	movs	r1, #0
	bl 0x0200b198
	movs	r0, #1
	bl 0x0200b138
	bl 0x0200b2f8
	bl 0x0200b308
	movs	r0, #20
	bl 0x0200b1b0
	ldr	r0, [pc, #812]
	bl 0x0200b288
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #176
	movs	r2, #0
	movs	r0, #10
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #10
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #5
	movs	r1, #4
	bl 0x0200b240
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #8
	movs	r1, #3
	bl 0x0200b248
	movs	r0, #6
	movs	r1, #2
	bl 0x0200b268
	adds	r1, r5, #0
	movs	r0, #10
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #80
	movs	r0, #10
	bl 0x0200b2c0
	movs	r1, #176
	movs	r0, #10
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b2b0
	movs	r2, #10
	str	r2, [sp, #4]
	movs	r3, #6
	movs	r1, #12
	movs	r0, #16
	movs	r2, #0
	movs	r4, #18
	str	r3, [sp, #0]
	str	r1, [sp, #8]
	str	r0, [sp, #12]
	str	r2, [sp, #24]
	movs	r3, #7
	movs	r5, #11
	movs	r0, #8
	movs	r1, #9
	movs	r2, #1
	str	r5, [sp, #20]
	str	r4, [sp, #16]
	bl 0x0200b2b8
	movs	r1, #2
	movs	r2, #40
	adds	r1, #255
	movs	r0, #8
	bl 0x0200b2c0
	movs	r0, #6
	movs	r1, #2
	bl 0x0200b268
	movs	r1, #129
	movs	r0, #5
	lsls	r1, r1, #1
	bl 0x0200b2c8
	movs	r2, #20
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b298
	movs	r1, #1
	movs	r0, #6
	bl 0x0200b268
	movs	r0, #40
	bl 0x0200b1b0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #2
	movs	r2, #40
	adds	r1, #255
	movs	r0, #6
	bl 0x0200b2c0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #18
	movs	r0, #6
	bl 0x0200b240
	movs	r0, #40
	bl 0x0200b1b0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #8
	movs	r1, #3
	bl 0x0200b240
	movs	r0, #8
	movs	r1, #0
	movs	r2, #40
	bl 0x0200b298
	movs	r1, #0
	movs	r2, #40
	movs	r0, #6
	bl 0x0200b298
	movs	r0, #6
	bl 0x0200b1d8
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	movs	r1, #170
	strb	r3, [r0, #0]
	lsls	r1, r1, #1
	movs	r0, #6
	movs	r2, #226
	bl 0x0200b210
	movs	r2, #0
	movs	r0, #6
	movs	r1, #4
	bl 0x0200b258
	movs	r1, #1
	movs	r0, #6
	bl 0x0200b240
	movs	r0, #6
	bl 0x0200b1d8
	movs	r1, #1
	bl 0x0200b198
	movs	r0, #20
	bl 0x0200b1b0
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x0200b2c8
	movs	r0, #20
	bl 0x0200b1b0
	movs	r1, #0
	movs	r0, #5
	bl 0x0200b2a0
	movs	r0, #6
	bl 0x0200b1d8
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r2, #1
	mov	sl, r2
	mov	r2, sl
	orrs	r3, r2
	movs	r1, #160
	strb	r3, [r0, #0]
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #6
	bl 0x0200b2b0
	movs	r0, #6
	bl 0x0200b1f0
	movs	r1, #3
	movs	r0, #6
	bl 0x0200b248
	movs	r0, #40
	bl 0x0200b1b0
	movs	r0, #8
	movs	r1, #6
	bl 0x0200b318
	movs	r0, #5
	movs	r1, #6
	bl 0x0200b318
	movs	r0, #10
	movs	r1, #6
	bl 0x0200b318
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #6
	adds	r1, #102
	adds	r2, #51
	bl 0x0200b1e0
	movs	r1, #179
	lsls	r1, r1, #1
	movs	r2, #222
	movs	r0, #6
	bl 0x0200b218
	movs	r0, #40
	bl 0x0200b1b0
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #6
	movs	r2, #80
	bl 0x0200b2b0
	movs	r1, #224
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200b2b0
	movs	r0, #6
	mov	r1, r8
	movs	r2, #0
	bl 0x0200b2b0
	movs	r2, #20
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b298
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #10
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #224
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200b2b0
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #2
	adds	r1, #255
	movs	r2, #40
	movs	r0, #6
	bl 0x0200b2c0
	movs	r1, #192
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200b2b0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #8
	movs	r1, #4
	bl 0x0200b240
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r2, #0
	movs	r0, #5
	movs	r1, #4
	bl 0x0200b258
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r2, #20
	movs	r0, #6
	mov	r1, r8
	bl 0x0200b2b0
	movs	r0, #10
	movs	r1, #3
	bl 0x0200b248
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #10
	movs	r1, #0
	bl 0x0200b2a0
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #136]
	adds	r2, #204
	bl 0x0200b1e0
	movs	r1, #174
	movs	r0, #6
	lsls	r1, r1, #1
	movs	r2, #222
	bl 0x0200b218
	movs	r1, #192
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200b2b0
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #6
	bl 0x0200b2a0
	movs	r0, #8
	bl 0x0200b1f8
	movs	r0, #5
	bl 0x0200b1f8
	movs	r0, #10
	bl 0x0200b1f8
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #8
	adds	r1, #102
	adds	r2, #51
	bl 0x0200b1e0
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #1
	movs	r2, #200
	bl 0x0200b218
	movs	r0, #8
	movs	r1, #0
	movs	r2, #40
	bl 0x0200b298
	movs	r2, #40
	movs	r0, #6
	mov	r1, r8
	bl 0x0200b2b0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #10
	b.n	.L_02002718
	.4byte 0x02000240
	.4byte 0x00001659
	.2byte 0x9999
	.2byte 0x0001
.L_02002718:
	bl 0x0200b2c0
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #10
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #6
	movs	r1, #4
	bl 0x0200b248
	movs	r1, #192
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200b2b0
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b2b0
	movs	r2, #0
	movs	r0, #5
	mov	r1, r8
	bl 0x0200b2b0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #160
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #10
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #6
	bl 0x0200b2b0
	movs	r0, #6
	bl 0x0200b1f0
	movs	r1, #3
	movs	r0, #6
	bl 0x0200b248
	movs	r0, #20
	bl 0x0200b1b0
	movs	r0, #10
	movs	r1, #4
	bl 0x0200b248
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #10
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #128
	movs	r2, #0
	movs	r0, #8
	lsls	r1, r1, #6
	bl 0x0200b2b0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #6
	bl 0x0200b2b0
	movs	r0, #6
	bl 0x0200b1f0
	movs	r0, #6
	movs	r1, #4
	bl 0x0200b240
	movs	r2, #20
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b298
	movs	r1, #2
	ldr	r0, [r6, #0]
	bl 0x0200b268
	movs	r0, #20
	bl 0x0200b1b0
	movs	r0, #5
	mov	r1, fp
	movs	r2, #0
	bl 0x0200b2b0
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #5
	movs	r1, #0
	movs	r2, #20
	bl 0x0200b298
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	ldr	r0, [r6, #0]
	bl 0x0200b2c0
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #6
	bl 0x0200b2b0
	movs	r0, #6
	bl 0x0200b1f0
	movs	r0, #6
	movs	r1, #3
	bl 0x0200b240
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #6
	movs	r2, #0
	adds	r1, #255
	ldr	r0, [r6, #0]
	bl 0x0200b2c0
	ldr	r0, [r6, #0]
	movs	r1, #2
	bl 0x0200b268
	movs	r1, #129
	movs	r0, #8
	lsls	r1, r1, #1
	bl 0x0200b2c8
	movs	r2, #20
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b298
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x0200b260
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #20
	movs	r0, #10
	bl 0x0200b2c0
	movs	r1, #176
	movs	r2, #20
	movs	r0, #10
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #10
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #42
	ldr	r0, [r6, #0]
	bl 0x0200b240
	movs	r0, #20
	bl 0x0200b1b0
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b2b0
	movs	r2, #20
	movs	r0, #6
	mov	r1, r8
	bl 0x0200b2b0
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #10
	movs	r2, #40
	adds	r1, #255
	movs	r0, #5
	bl 0x0200b2c0
	movs	r1, #0
	movs	r0, #5
	bl 0x0200b2a0
	ldr	r0, [r6, #0]
	bl 0x0200b1d8
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #157
	ands	r5, r3
	strb	r5, [r0, #0]
	lsls	r1, r1, #1
	ldr	r0, [r6, #0]
	movs	r2, #226
	bl 0x0200b210
	movs	r2, #0
	ldr	r0, [r6, #0]
	movs	r1, #4
	bl 0x0200b258
	movs	r1, #1
	ldr	r0, [r6, #0]
	bl 0x0200b240
	ldr	r0, [r6, #0]
	bl 0x0200b1d8
	movs	r1, #1
	bl 0x0200b198
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #24]
	movs	r0, #20
	bl 0x0200b1b0
	movs	r1, #129
	movs	r0, #5
	lsls	r1, r1, #1
	bl 0x0200b2c8
	movs	r1, #0
	movs	r0, #5
	bl 0x0200b2a0
	ldr	r0, [r6, #0]
	bl 0x0200b1d8
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r2, sl
	orrs	r2, r3
	strb	r2, [r0, #0]
	movs	r1, #192
	ldr	r0, [r6, #0]
	lsls	r1, r1, #7
	mov	sl, r2
	movs	r2, #40
	bl 0x0200b2b0
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #6
	movs	r2, #40
	bl 0x0200b2b0
	movs	r1, #160
	movs	r2, #80
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	ldr	r0, [r6, #0]
	bl 0x0200b2c0
	ldr	r0, [r6, #0]
	movs	r1, #0
	movs	r2, #20
	bl 0x0200b2b0
	movs	r1, #176
	movs	r2, #0
	movs	r0, #10
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #10
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b2b0
	movs	r0, #8
	mov	r1, r9
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #192
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #7
	bl 0x0200b2b0
	movs	r0, #10
	movs	r1, #3
	bl 0x0200b248
	movs	r0, #10
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b2b0
	movs	r0, #5
	mov	r1, r8
	movs	r2, #40
	bl 0x0200b2b0
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #160
	movs	r2, #20
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200b2b0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r2, #0
	movs	r0, #8
	mov	r1, r9
	bl 0x0200b2b0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b2b0
	movs	r0, #5
	mov	r1, r8
	movs	r2, #0
	bl 0x0200b2b0
	ldr	r0, [r6, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b2b0
	movs	r0, #6
	mov	r1, r8
	movs	r2, #20
	bl 0x0200b2b0
	movs	r2, #0
	movs	r0, #10
	movs	r1, #0
	bl 0x0200b2b0
	movs	r0, #8
	movs	r1, #3
	bl 0x0200b240
	movs	r0, #5
	movs	r1, #3
	bl 0x0200b240
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x0200b240
	movs	r0, #6
	movs	r1, #3
	bl 0x0200b248
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #10
	ldr	r1, [pc, #316]
	adds	r2, #204
	bl 0x0200b1e0
	movs	r1, #184
	movs	r2, #128
	movs	r0, #10
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200b218
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #6
	bl 0x0200b2c0
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #6
	bl 0x0200b2b0
	movs	r0, #6
	bl 0x0200b1f0
	movs	r0, #160
	lsls	r0, r0, #7
	adds	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #8
	mov	r1, r9
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b2b0
	movs	r1, #200
	movs	r2, #132
	movs	r0, #10
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200b218
	movs	r2, #0
	movs	r0, #10
	movs	r1, #0
	bl 0x0200b2b0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #132
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200b2c0
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #10
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #153
	movs	r1, #152
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #153
	adds	r1, #51
	bl 0x0200b2d0
	movs	r0, #198
	movs	r1, #1
	movs	r2, #234
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #16
	bl 0x0200b2d8
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #5
	adds	r1, #204
	adds	r2, #102
	bl 0x0200b1e0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #6
	adds	r1, #204
	adds	r2, #102
	bl 0x0200b1e0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #8
	adds	r1, #204
	adds	r2, #102
	bl 0x0200b1e0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #102
	ldr	r0, [r6, #0]
	adds	r1, #204
	bl 0x0200b1e0
	ldr	r3, [pc, #40]
	adds	r5, r7, #0
	adds	r5, #100
	strh	r3, [r5, #0]
	ldr	r1, [pc, #40]
	movs	r0, #8
	bl 0x0200b1e8
	ldr	r1, [pc, #36]
	movs	r0, #5
	bl 0x0200b1e8
	ldr	r0, [r6, #0]
	ldr	r1, [pc, #32]
	bl 0x0200b1e8
	ldr	r1, [pc, #28]
	movs	r0, #6
	bl 0x0200b1e8
	.2byte 0xe00c
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0x9999
	.2byte 0x0001
	push	{r2, r3, r4, r5, r7, lr}
	lsls	r0, r0, #8
	push	{r2, r4, r5, lr}
	lsls	r0, r0, #8
	.2byte 0xb600
	lsls	r0, r0, #8
	push	{r3, r4, r5, r6, lr}
	lsls	r0, r0, #8
.L_02002bdc:
	movs	r0, #1
	bl 0x0200b138
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	beq.n	.L_02002bdc
	bl 0x0200b2e0
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x0200b2c0
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #5
	movs	r1, #0
	bl 0x0200b2a0
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #6
	movs	r1, #0
	bl 0x0200b2a0
	movs	r1, #0
	movs	r0, #8
	bl 0x0200b2a0
	movs	r0, #13
	bl 0x0200b2e8
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200ac64,"ax",%progbits
	push	{r5, lr}
	bl 0x0200ac30
	bl 0x0200b1b8
	movs	r0, #0
	bl 0x0200b310
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200b2d8
	movs	r0, #1
	bl 0x0200b138
	movs	r0, #192
	movs	r1, #1
	movs	r2, #182
	negs	r1, r1
	lsls	r2, r2, #18
	movs	r3, #0
	lsls	r0, r0, #17
	bl 0x0200b2d8
	movs	r0, #1
	bl 0x0200b138
	ldr	r3, [pc, #532]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	movs	r1, #41
	ldr	r0, [r5, #0]
	bl 0x0200b240
	movs	r0, #1
	bl 0x0200b138
	ldr	r0, [r5, #0]
	bl 0x0200b1d8
	movs	r1, #0
	bl 0x0200b198
	movs	r0, #1
	bl 0x0200b138
	movs	r1, #192
	movs	r2, #182
	ldr	r0, [r5, #0]
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200b228
	movs	r1, #192
	ldr	r2, [pc, #480]
	lsls	r1, r1, #17
	movs	r0, #11
	bl 0x0200b228
	movs	r0, #1
	bl 0x0200b138
	bl 0x0200b180
	movs	r0, #1
	bl 0x0200b138
	bl 0x0200ac34
	bl 0x0200b2f8
	bl 0x0200b308
	movs	r0, #10
	bl 0x0200b138
	bl 0x0200ac38
	bl 0x0200ac30
	movs	r1, #2
	ldr	r0, [r5, #0]
	bl 0x0200b268
	movs	r0, #1
	bl 0x0200b138
	bl 0x0200ac34
	bl 0x0200ac38
	bl 0x0200ac30
	movs	r1, #42
	ldr	r0, [r5, #0]
	bl 0x0200b240
	movs	r0, #1
	bl 0x0200b138
	bl 0x0200ac34
	bl 0x0200ac38
	bl 0x0200ac30
	movs	r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200b240
	ldr	r0, [r5, #0]
	bl 0x0200b1d8
	movs	r1, #1
	bl 0x0200b198
	movs	r0, #1
	bl 0x0200b138
	movs	r2, #20
	movs	r1, #4
	ldr	r0, [r5, #0]
	bl 0x0200b258
	ldr	r0, [pc, #344]
	bl 0x0200b288
	movs	r1, #0
	movs	r0, #11
	bl 0x0200b290
	movs	r0, #1
	bl 0x0200b138
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200b1d0
	ldr	r3, [r5, #0]
	cmp	r0, #0
	bne.n	.L_02002e3c
	adds	r0, r3, #0
	bl 0x0200b250
	movs	r1, #45
	ldr	r0, [r5, #0]
	bl 0x0200b240
	movs	r0, #1
	bl 0x0200b138
	bl 0x0200ac34
	movs	r0, #50
	bl 0x0200b1b0
	bl 0x0200ac30
	movs	r1, #46
	ldr	r0, [r5, #0]
	bl 0x0200b240
	movs	r0, #1
	bl 0x0200b138
	bl 0x0200ac34
	movs	r0, #50
	bl 0x0200b1b0
	bl 0x0200ac30
	movs	r1, #0
	movs	r0, #11
	bl 0x0200b2a0
	movs	r0, #1
	bl 0x0200b138
	movs	r1, #3
	ldr	r0, [r5, #0]
	bl 0x0200b248
	bl 0x0200ac34
	movs	r0, #20
	bl 0x0200b1b0
	bl 0x0200ac30
	movs	r1, #47
	ldr	r0, [r5, #0]
	bl 0x0200b240
	movs	r0, #1
	bl 0x0200b138
	bl 0x0200ac34
	movs	r0, #50
	bl 0x0200b1b0
	bl 0x0200ac30
	movs	r1, #48
	ldr	r0, [r5, #0]
	bl 0x0200b240
	movs	r0, #1
	bl 0x0200b138
	bl 0x0200ac34
	movs	r0, #50
	bl 0x0200b1b0
	bl 0x0200ac30
	movs	r1, #0
	movs	r0, #11
	bl 0x0200b2a0
	movs	r0, #1
	bl 0x0200b138
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200b248
	b.n	.L_02002e94
.L_02002e3c:
	adds	r0, r3, #0
	bl 0x0200b250
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r1, #204
	adds	r3, #2
	strh	r3, [r2, #0]
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	ldr	r0, [r5, #0]
	adds	r1, #102
	adds	r2, #51
	bl 0x0200b1e0
	movs	r2, #128
	movs	r1, #192
	lsls	r2, r2, #2
	lsls	r1, r1, #1
	adds	r2, #234
	ldr	r0, [r5, #0]
	bl 0x0200b218
	bl 0x0200ac34
	movs	r0, #20
	bl 0x0200b1b0
	bl 0x0200ac30
	movs	r0, #11
	movs	r1, #0
	bl 0x0200b2a0
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200b248
.L_02002e94:
	movs	r1, #0
	movs	r2, #0
	movs	r0, #11
	bl 0x0200b228
	movs	r0, #48
	adds	r0, #255
	bl 0x0200b160
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #68
	bl 0x0200b158
	bl 0x0200b1c0
	bl 0x0200ac34
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x02be0000
	.2byte 0x169e
	.2byte 0x0000
	.global Func_02002ec8
	.thumb_func
Func_02002ec8:
	push	{lr}
	bl 0x0200953c
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #156]
	bl 0x0200b140
	movs	r1, #144
	ldr	r0, [pc, #152]
	lsls	r1, r1, #3
	bl 0x0200b140
	ldr	r3, [pc, #148]
	movs	r0, #241
	lsls	r0, r0, #1
	adds	r3, r3, r0
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	subs	r3, #2
	cmp	r3, #11
	bhi.n	.L_02002f6a
	ldr	r2, [pc, #132]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0200af2c
	.4byte 0x0200af6a
	.4byte 0x0200af3e
	.4byte 0x0200af6a
	.4byte 0x0200af6a
	.4byte 0x0200af6a
	.4byte 0x0200af6a
	.4byte 0x0200af6a
	.4byte 0x0200af6a
	.4byte 0x0200af5a
	.4byte 0x0200af60
	.4byte 0x0200af66
	.4byte 0x30ff200a
	.4byte 0xf90ef000
	.4byte 0xd1022800
	.4byte 0xf0002001
	.4byte 0x490ef935
	.4byte 0x20f24b0f
	.4byte 0x180a0040
	.4byte 0x23f38013
	.4byte 0x18ca005b
	.4byte 0x80132301
	.4byte 0xf83ef000
	.4byte 0xf7fee007
	.4byte 0xe004fbbd
	.4byte 0xffbef7fe
	.4byte 0xf7ffe001
	.2byte 0xf9cd
.L_02002f6a:
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02009559
	.4byte 0x020095a1
	.4byte 0x02000240
	.4byte 0x0200aefc
	.2byte 0x000c
	.2byte 0x0000
	.section .text.x0200af88,"ax",%progbits
	push	{r5, r6, lr}
	adds	r5, r0, #0
	bl 0x0200b1d8
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200b1d8
	movs	r1, #0
	bl 0x0200b198
	movs	r3, #128
	ldr	r2, [pc, #40]
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	movs	r1, #192
	ldr	r3, [r6, #8]
	lsls	r1, r1, #12
	adds	r3, r3, r1
	str	r3, [r6, #8]
	movs	r3, #128
	lsls	r3, r3, #13
	str	r3, [r6, #12]
	adds	r3, r6, #0
	adds	r3, #89
	strb	r2, [r3, #0]
	movs	r3, #192
	ldr	r2, [r6, #80]
	lsls	r3, r3, #8
	strh	r3, [r2, #18]
	b.n	.L_02002fd0
	.2byte 0x0000
	.2byte 0x0000
.L_02002fd0:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #99
	sub	sp, #8
	bl 0x0200b150
	cmp	r0, #0
	bne.n	.L_02003044
	movs	r1, #208
	movs	r2, #170
	movs	r0, #8
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200b228
	movs	r3, #25
	movs	r2, #42
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r2, #2
	movs	r1, #32
	movs	r0, #38
	bl 0x0200b190
	movs	r0, #8
	bl 0x0200b1d8
	adds	r5, r0, #0
	movs	r0, #8
	bl 0x0200b1d8
	movs	r1, #0
	bl 0x0200b198
	movs	r3, #128
	ldr	r2, [pc, #32]
	lsls	r3, r3, #7
	strh	r3, [r5, #6]
	adds	r3, r5, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #12
	str	r3, [r5, #12]
	adds	r5, #89
	strb	r2, [r5, #0]
	movs	r0, #8
	movs	r1, #5
	bl 0x0200b240
	b.n	.L_02003044
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_02003044:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #100
	bl 0x0200b150
	cmp	r0, #0
	bne.n	.L_020030b0
	movs	r1, #144
	movs	r2, #186
	movs	r0, #5
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200b228
	movs	r3, #17
	movs	r2, #46
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r2, #2
	movs	r1, #33
	movs	r0, #38
	bl 0x0200b190
	movs	r0, #5
	bl 0x0200b1d8
	adds	r5, r0, #0
	movs	r0, #5
	bl 0x0200b1d8
	movs	r1, #0
	bl 0x0200b198
	movs	r3, #128
	ldr	r2, [pc, #32]
	lsls	r3, r3, #7
	strh	r3, [r5, #6]
	adds	r3, r5, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #12
	str	r3, [r5, #12]
	adds	r5, #89
	strb	r2, [r5, #0]
	movs	r0, #5
	movs	r1, #18
	bl 0x0200b240
	b.n	.L_020030b0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_020030b0:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #101
	bl 0x0200b150
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_0200310a
	movs	r0, #6
	bl 0x0200b1d8
	movs	r1, #240
	movs	r2, #162
	adds	r5, r0, #0
	lsls	r1, r1, #16
	movs	r0, #6
	lsls	r2, r2, #18
	bl 0x0200b228
	movs	r3, #14
	movs	r2, #40
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r2, #2
	movs	r0, #38
	movs	r1, #34
	bl 0x0200b190
	movs	r1, #19
	movs	r0, #6
	bl 0x0200b240
	movs	r0, #6
	bl 0x0200b1d8
	movs	r1, #0
	bl 0x0200b198
	adds	r3, r5, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #12
	str	r3, [r5, #12]
.L_0200310a:
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #72
	movs	r1, #42
	movs	r2, #75
	movs	r3, #40
	bl 0x0200b188
	movs	r0, #1
	bl 0x0200b138
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #68
	bl 0x0200b150
	cmp	r0, #0
	bne.n	.L_02003134
	bl 0x0200ac64
.L_02003134:
	add	sp, #8
	pop	{r5, r6, pc}
	.section .rodata.x0200b330,"a",%progbits
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00ac0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00a40000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00b60000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f20000
	.4byte 0x00000000
	.4byte 0x00a40000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x00ac0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00ac0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x00ac0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01300000
	.4byte 0x00000000
	.4byte 0x00ac0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00ac0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00ac0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01a40000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00e40000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01900000
	.4byte 0x00000000
	.4byte 0x00d60000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x019c0000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x000000c0
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00000180
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000018
	.4byte 0xc0010000
	.4byte 0x80020000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffe80
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffd00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000c
	.4byte 0xc0020000
	.4byte 0x80030000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x000000c0
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00000180
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000018
	.4byte 0xc0030000
	.4byte 0x80040000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffe80
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffd00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000c
	.4byte 0xc0040000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000002
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02f40000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00920000
	.4byte 0x00000000
	.4byte 0x02e00000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02f40000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global gIdejimaEntrances
gIdejimaEntrances:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gIdejimaExits
gIdejimaExits:
	.4byte 0x00000009
	.4byte 0x00303009
	.4byte 0x00401002
	.4byte 0x00b40002
	.4byte 0x00c03000
	.4byte 0x00d41002
	.4byte 0x000001ff
	.global gIdejimaPlacements
gIdejimaPlacements:
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff015a
	.4byte 0x00000007
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00028000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gIdejimaEvents
gIdejimaEvents:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gIdejimaEventsEntrance1
gIdejimaEventsEntrance1:
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008641
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gIdejimaEventsWake
gIdejimaEventsWake:
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x0863000a
	.4byte 0x020080f1
	.4byte 0x00000003
	.4byte 0x0864000b
	.4byte 0x020082b5
	.4byte 0x00000003
	.4byte 0x0865000c
	.4byte 0x0200849d
	.4byte 0x00008d15
	.4byte 0xffff0408
	.4byte 0x020080f1
	.4byte 0x00008d15
	.4byte 0xffff0405
	.4byte 0x020082b5
	.4byte 0x00000002
	.4byte 0xffff0009
	.4byte 0x020086a9
	.4byte 0x00000002
	.4byte 0x08670008
	.4byte 0x02008d15
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000002e
	.4byte 0x00000026
