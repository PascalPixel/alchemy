.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x020085b9, 0x02008039, 0x02008045, 0x0200804d, 0x02008541, 0x02008041, 0x02008735
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8c2c
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8c5c
	.2byte 0x0200
	push	{lr}
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x02008a74
	cmp	r0, #0
	beq.n	.L_02000060
	ldr	r0, [pc, #24]
	b.n	.L_02000074
.L_02000060:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x02008a74
	cmp	r0, #0
	beq.n	.L_02000072
	ldr	r0, [pc, #12]
	b.n	.L_02000074
.L_02000072:
	ldr	r0, [pc, #12]
.L_02000074:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200916c
	.4byte 0x02008f14
	.2byte 0x8cbc
	.2byte 0x0200
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #178
	bl 0x02008a7c
	bl 0x02008a94
	movs	r0, #0
	bl 0x02008b8c
	ldr	r0, [pc, #264]
	bl 0x02008b24
	movs	r0, #31
	movs	r1, #1
	bl 0x02008b84
	movs	r1, #226
	movs	r2, #80
	movs	r0, #4
	lsls	r1, r1, #1
	adds	r2, #255
	bl 0x02008adc
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02008b44
	bl 0x02008b7c
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #31
	bl 0x02008b44
	movs	r0, #20
	bl 0x02008a8c
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r2, #10
	adds	r0, #31
	movs	r1, #0
	bl 0x02008b34
	movs	r0, #4
	movs	r1, #3
	bl 0x02008b0c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #4
	bl 0x02008b64
	movs	r0, #50
	bl 0x02008a8c
	movs	r1, #3
	movs	r0, #31
	bl 0x02008afc
	movs	r0, #10
	bl 0x02008a8c
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r2, #10
	adds	r0, #31
	movs	r1, #0
	bl 0x02008b34
	movs	r1, #3
	movs	r0, #4
	bl 0x02008afc
	movs	r0, #20
	bl 0x02008a8c
	movs	r0, #31
	movs	r1, #4
	bl 0x02008afc
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r2, #10
	adds	r0, #31
	movs	r1, #0
	bl 0x02008b34
	movs	r1, #2
	movs	r0, #31
	bl 0x02008b14
	movs	r0, #10
	bl 0x02008a8c
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r2, #10
	adds	r0, #31
	movs	r1, #0
	bl 0x02008b34
	movs	r1, #3
	movs	r0, #31
	bl 0x02008afc
	movs	r0, #10
	bl 0x02008a8c
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #31
	movs	r1, #0
	movs	r2, #10
	bl 0x02008b34
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #31
	bl 0x02008b44
	movs	r0, #20
	bl 0x02008a8c
	ldr	r3, [pc, #28]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x02008b6c
	bl 0x02008b7c
	bl 0x02008a9c
	pop	{pc}
	.2byte 0x0000
	.4byte 0x00001b29
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x02008aac
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #24]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_020001e8
	movs	r0, #9
	adds	r1, r5, #0
	bl 0x02008bc4
	b.n	.L_020001f6
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_020001e8:
	ldr	r0, [pc, #12]
	bl 0x02008b24
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008b3c
.L_020001f6:
	pop	{r5, pc}
	.2byte 0x1b18
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x02008aac
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #24]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_02000238
	movs	r0, #2
	adds	r1, r5, #0
	bl 0x02008bd4
	b.n	.L_02000246
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_02000238:
	ldr	r0, [pc, #12]
	bl 0x02008b24
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008b3c
.L_02000246:
	pop	{r5, pc}
	.2byte 0x1b1a
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x02008aac
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #20]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_02000284
	adds	r0, r5, #0
	bl 0x02008bcc
	b.n	.L_02000292
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_02000284:
	ldr	r0, [pc, #12]
	bl 0x02008b24
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008b3c
.L_02000292:
	pop	{r5, pc}
	.2byte 0x1b40
	.2byte 0x0000
	push	{r5, lr}
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	adds	r5, r0, #0
	bl 0x02008b5c
	ldr	r0, [pc, #16]
	bl 0x02008b24
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008b3c
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1b08
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #177
	bl 0x02008a7c
	bl 0x02008a94
	movs	r0, #0
	bl 0x02008b8c
	ldr	r5, [pc, #360]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r2, #164
	ldr	r0, [r5, #0]
	movs	r1, #152
	lsls	r2, r2, #1
	bl 0x02008adc
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02008b44
	ldr	r0, [pc, #332]
	bl 0x02008b24
	movs	r1, #0
	movs	r2, #0
	movs	r0, #14
	bl 0x02008b44
	movs	r0, #20
	bl 0x02008a8c
	movs	r0, #14
	movs	r1, #0
	movs	r2, #10
	bl 0x02008b34
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #15
	bl 0x02008b44
	movs	r0, #20
	bl 0x02008a8c
	movs	r1, #3
	movs	r0, #15
	bl 0x02008afc
	movs	r0, #10
	bl 0x02008a8c
	movs	r0, #15
	movs	r1, #0
	movs	r2, #10
	bl 0x02008b34
	movs	r1, #128
	movs	r2, #128
	movs	r0, #14
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x02008ab4
	movs	r1, #32
	negs	r1, r1
	movs	r2, #0
	movs	r0, #14
	bl 0x02008b94
	movs	r0, #10
	bl 0x02008a8c
	movs	r2, #10
	movs	r0, #14
	movs	r1, #0
	bl 0x02008b34
	movs	r1, #2
	movs	r0, #13
	bl 0x02008b14
	movs	r0, #10
	bl 0x02008a8c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #13
	bl 0x02008b44
	movs	r0, #20
	bl 0x02008a8c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #13
	bl 0x02008b5c
	movs	r2, #10
	movs	r0, #13
	movs	r1, #0
	bl 0x02008b34
	movs	r0, #14
	movs	r1, #3
	bl 0x02008b0c
	movs	r1, #129
	movs	r0, #14
	lsls	r1, r1, #1
	bl 0x02008b64
	movs	r0, #15
	movs	r1, #3
	bl 0x02008b0c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #15
	bl 0x02008b64
	movs	r0, #50
	bl 0x02008a8c
	movs	r1, #2
	movs	r0, #13
	bl 0x02008b14
	movs	r0, #20
	bl 0x02008a8c
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #13
	bl 0x02008b44
	movs	r0, #10
	bl 0x02008a8c
	movs	r1, #0
	movs	r2, #10
	movs	r0, #13
	bl 0x02008b34
	movs	r0, #10
	bl 0x02008a8c
	movs	r2, #156
	lsls	r2, r2, #1
	movs	r1, #152
	movs	r0, #14
	bl 0x02008adc
	movs	r0, #10
	bl 0x02008a8c
	movs	r0, #15
	movs	r1, #4
	bl 0x02008af4
	movs	r1, #4
	movs	r0, #14
	bl 0x02008afc
	movs	r0, #10
	bl 0x02008a8c
	movs	r1, #192
	movs	r0, #14
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02008b44
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02008b44
	bl 0x02008a9c
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x1b09
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #44]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	adds	r5, r0, #0
	bl 0x02008b1c
	ldr	r0, [pc, #28]
	bl 0x02008b24
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008b3c
	movs	r1, #192
	movs	r0, #14
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02008b44
	pop	{r5, pc}
	.4byte 0x02000240
	.2byte 0x1b0e
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r1, #0
	movs	r2, #0
	ldr	r1, [r3, #0]
	adds	r0, r5, #0
	bl 0x02008b1c
	ldr	r0, [pc, #32]
	bl 0x02008b24
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008b3c
	movs	r1, #224
	adds	r0, r5, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02008b44
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x1b3e
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #184
	bl 0x02008a7c
	ldr	r0, [pc, #40]
	bl 0x02008b24
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008b3c
	movs	r0, #10
	bl 0x02008a8c
	movs	r1, #132
	adds	r0, r5, #0
	lsls	r1, r1, #1
	movs	r2, #30
	bl 0x02008b5c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008b3c
	pop	{r5, pc}
	.2byte 0x1c33
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r5, [pc, #68]
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x02008b24
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x02008b2c
	bl 0x02008ba4
	movs	r1, #0
	bl 0x02008aa4
	cmp	r0, #0
	bne.n	.L_02000524
	movs	r0, #10
	bl 0x02008a8c
	adds	r0, r5, #1
	bl 0x02008b24
	b.n	.L_02000530
.L_02000524:
	movs	r0, #20
	bl 0x02008a8c
	adds	r0, r5, #2
	bl 0x02008b24
.L_02000530:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02008b3c
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x1c67
	.2byte 0x0000
	push	{lr}
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x02008a74
	cmp	r0, #0
	beq.n	.L_02000554
	ldr	r0, [pc, #24]
	b.n	.L_02000568
.L_02000554:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x02008a74
	cmp	r0, #0
	beq.n	.L_02000566
	ldr	r0, [pc, #12]
	b.n	.L_02000568
.L_02000566:
	ldr	r0, [pc, #12]
.L_02000568:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x020099dc
	.4byte 0x02009730
	.2byte 0x93dc
	.2byte 0x0200
	push	{lr}
	ldr	r4, [pc, #24]
	ldr	r2, [r0, #12]
	movs	r3, #192
	lsls	r3, r3, #12
	ldr	r1, [r0, #8]
	adds	r2, r2, r3
	ldr	r3, [r0, #16]
	adds	r0, r4, #0
	bl 0x02008bbc
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x9c90
	.2byte 0x0200
	push	{lr}
	ldr	r4, [pc, #24]
	ldr	r2, [r0, #12]
	movs	r3, #128
	lsls	r3, r3, #12
	ldr	r1, [r0, #8]
	adds	r2, r2, r3
	ldr	r3, [r0, #16]
	adds	r0, r4, #0
	bl 0x02008bbc
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x9c60
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	movs	r0, #16
	bl 0x02008aac
	adds	r0, #89
	ldrb	r2, [r0, #0]
	movs	r3, #4
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #16
	bl 0x02008aac
	adds	r0, #89
	ldrb	r2, [r0, #0]
	movs	r3, #16
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #3
	movs	r0, #9
	bl 0x02008b54
	movs	r0, #31
	movs	r1, #3
	bl 0x02008b54
	movs	r0, #14
	movs	r1, #3
	bl 0x02008b54
	movs	r1, #3
	movs	r0, #15
	bl 0x02008b54
	movs	r0, #0
	bl 0x02008bac
	movs	r3, #128
	ldr	r0, [pc, #268]
	movs	r1, #8
	movs	r2, #15
	lsls	r3, r3, #23
	bl 0x02008bb4
	ldr	r5, [pc, #260]
	movs	r3, #128
	movs	r1, #8
	movs	r2, #15
	adds	r0, r5, #0
	lsls	r3, r3, #23
	bl 0x02008bb4
	movs	r3, #192
	lsls	r3, r3, #9
	str	r3, [r5, #24]
	movs	r0, #14
	bl 0x02008aac
	ldr	r3, [pc, #236]
	str	r3, [r0, #108]
	movs	r0, #15
	bl 0x02008aac
	ldr	r3, [pc, #228]
	str	r3, [r0, #108]
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x02008a74
	cmp	r0, #0
	beq.n	.L_020006d4
	ldr	r3, [pc, #216]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x02008aac
	ldr	r3, [r0, #8]
	movs	r2, #128
	lsls	r2, r2, #18
	cmp	r3, r2
	blt.n	.L_02000678
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02008a7c
	b.n	.L_02000680
.L_02000678:
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02008a84
.L_02000680:
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02008a74
	cmp	r0, #0
	beq.n	.L_020006c6
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #35
	bl 0x02008a74
	cmp	r0, #0
	beq.n	.L_020006c6
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #3
	bl 0x02008a74
	cmp	r0, #0
	bne.n	.L_020006c6
	movs	r1, #188
	movs	r2, #158
	movs	r0, #31
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x02008aec
	movs	r1, #196
	movs	r2, #172
	movs	r0, #32
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x02008aec
	b.n	.L_0200071c
.L_020006c6:
	movs	r0, #31
	movs	r1, #0
	movs	r2, #0
	bl 0x02008aec
	movs	r0, #32
	b.n	.L_020006f2
.L_020006d4:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x02008a74
	cmp	r0, #0
	beq.n	.L_020006fc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #113
	bl 0x02008a74
	cmp	r0, #0
	beq.n	.L_0200071c
	movs	r0, #31
.L_020006f2:
	movs	r1, #0
	movs	r2, #0
	bl 0x02008aec
	b.n	.L_0200071c
.L_020006fc:
	movs	r0, #24
	movs	r1, #2
	bl 0x02008b54
	movs	r0, #19
	movs	r1, #3
	bl 0x02008b54
	movs	r0, #28
	movs	r1, #2
	bl 0x02008b54
	movs	r0, #25
	movs	r1, #3
	bl 0x02008b54
.L_0200071c:
	movs	r0, #0
	pop	{r5, pc}
	.4byte 0x02009c90
	.4byte 0x02009c60
	.4byte 0x02008579
	.4byte 0x02008599
	.2byte 0x0240
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	push	{r5, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #3
	bl 0x02008a74
	cmp	r0, #0
	beq.n	.L_0200074a
	b.n	.L_02000a6a
.L_0200074a:
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02008a74
	cmp	r0, #0
	bne.n	.L_02000758
	b.n	.L_02000a6a
.L_02000758:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #3
	bl 0x02008a7c
	bl 0x02008a94
	movs	r0, #0
	bl 0x02008b8c
	ldr	r0, [pc, #764]
	bl 0x02008b24
	movs	r1, #252
	movs	r2, #164
	lsls	r2, r2, #1
	movs	r0, #4
	lsls	r1, r1, #1
	bl 0x02008adc
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #8
	bl 0x02008b4c
	movs	r0, #204
	movs	r1, #1
	movs	r2, #164
	movs	r3, #1
	lsls	r2, r2, #17
	negs	r1, r1
	lsls	r0, r0, #17
	bl 0x02008b74
	bl 0x02008b7c
	movs	r0, #10
	bl 0x02008a8c
	movs	r1, #2
	movs	r0, #31
	bl 0x02008b14
	movs	r0, #5
	bl 0x02008a8c
	movs	r0, #31
	movs	r1, #0
	movs	r2, #5
	bl 0x02008b34
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #32
	bl 0x02008b5c
	movs	r0, #32
	movs	r1, #0
	movs	r2, #5
	bl 0x02008b34
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #31
	bl 0x02008b5c
	movs	r0, #31
	movs	r1, #0
	movs	r2, #5
	bl 0x02008b34
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #32
	bl 0x02008b5c
	movs	r2, #5
	movs	r0, #32
	movs	r1, #0
	bl 0x02008b34
	movs	r1, #3
	movs	r0, #31
	bl 0x02008afc
	movs	r0, #10
	bl 0x02008a8c
	movs	r0, #31
	movs	r1, #0
	movs	r2, #5
	bl 0x02008b34
	movs	r0, #32
	movs	r1, #6
	movs	r2, #15
	bl 0x02008b04
	movs	r0, #32
	movs	r1, #6
	movs	r2, #23
	bl 0x02008b04
	movs	r2, #5
	movs	r0, #32
	movs	r1, #0
	bl 0x02008b34
	movs	r1, #3
	movs	r0, #31
	bl 0x02008afc
	movs	r0, #10
	bl 0x02008a8c
	movs	r0, #31
	movs	r1, #0
	movs	r2, #5
	bl 0x02008b34
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #32
	bl 0x02008b5c
	movs	r2, #5
	movs	r0, #32
	movs	r1, #0
	bl 0x02008b34
	movs	r1, #2
	movs	r0, #31
	bl 0x02008b14
	movs	r0, #5
	bl 0x02008a8c
	movs	r2, #5
	movs	r0, #31
	movs	r1, #0
	bl 0x02008b34
	movs	r1, #3
	movs	r0, #32
	bl 0x02008afc
	movs	r0, #10
	bl 0x02008a8c
	movs	r2, #5
	movs	r0, #32
	movs	r1, #0
	bl 0x02008b34
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r0, #31
	bl 0x02008b4c
	movs	r0, #15
	bl 0x02008a8c
	movs	r2, #5
	movs	r0, #31
	movs	r1, #0
	bl 0x02008b34
	movs	r1, #3
	movs	r0, #32
	bl 0x02008afc
	movs	r0, #10
	bl 0x02008a8c
	movs	r2, #5
	movs	r0, #32
	movs	r1, #0
	bl 0x02008b34
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #31
	bl 0x02008b4c
	movs	r0, #15
	bl 0x02008a8c
	movs	r1, #3
	movs	r0, #31
	bl 0x02008afc
	movs	r0, #20
	bl 0x02008a8c
	movs	r1, #3
	movs	r0, #32
	bl 0x02008afc
	movs	r0, #20
	bl 0x02008a8c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #31
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02008ab4
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r0, #32
	lsls	r1, r1, #9
	bl 0x02008ab4
	movs	r0, #31
	movs	r1, #2
	bl 0x02008b54
	movs	r0, #32
	movs	r1, #2
	bl 0x02008b54
	movs	r0, #32
	movs	r1, #1
	bl 0x02008b6c
	movs	r1, #197
	movs	r2, #171
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #31
	bl 0x02008ad4
	movs	r0, #10
	bl 0x02008a8c
	movs	r1, #225
	movs	r2, #173
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #32
	bl 0x02008ad4
	movs	r0, #31
	bl 0x02008ae4
	movs	r1, #225
	movs	r2, #173
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #31
	bl 0x02008ad4
	movs	r0, #32
	bl 0x02008ae4
	movs	r1, #235
	movs	r2, #164
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #32
	bl 0x02008ad4
	movs	r0, #31
	bl 0x02008ae4
	movs	r1, #235
	movs	r2, #164
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #31
	bl 0x02008ad4
	movs	r0, #32
	bl 0x02008ae4
	movs	r1, #243
	movs	r2, #164
	lsls	r2, r2, #1
	lsls	r1, r1, #1
	movs	r0, #32
	bl 0x02008ad4
	movs	r0, #32
	bl 0x02008ae4
	movs	r0, #32
	movs	r1, #1
	bl 0x02008af4
	movs	r1, #0
	movs	r0, #32
	bl 0x02008b4c
	movs	r0, #31
	bl 0x02008ae4
	movs	r0, #31
	movs	r1, #1
	bl 0x02008af4
	movs	r0, #31
	movs	r1, #0
	bl 0x02008b4c
	movs	r2, #16
	negs	r2, r2
	movs	r0, #4
	movs	r1, #0
	bl 0x02008b94
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #4
	bl 0x02008b4c
	movs	r0, #10
	bl 0x02008a8c
	ldr	r5, [pc, #140]
	movs	r0, #32
	adds	r1, r5, #0
	bl 0x02008abc
	movs	r0, #10
	bl 0x02008a8c
	adds	r1, r5, #0
	movs	r0, #31
	bl 0x02008abc
	movs	r0, #10
	bl 0x02008a8c
	movs	r1, #32
	movs	r0, #4
	bl 0x02008b9c
	movs	r0, #60
	bl 0x02008a8c
	movs	r0, #31
	movs	r1, #3
	bl 0x02008b54
	movs	r1, #3
	movs	r0, #32
	bl 0x02008b54
	movs	r0, #32
	bl 0x02008ac4
	movs	r0, #4
	bl 0x02008acc
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	movs	r3, #0
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	bl 0x02008b74
	movs	r1, #0
	movs	r2, #0
	movs	r0, #32
	bl 0x02008aec
	movs	r0, #123
	bl 0x02008bdc
	movs	r0, #31
	bl 0x02008ac4
	movs	r1, #0
	movs	r2, #0
	movs	r0, #31
	bl 0x02008aec
	movs	r0, #123
	bl 0x02008bdc
	movs	r0, #10
	bl 0x02008a8c
	bl 0x02008a9c
.L_02000a6a:
	pop	{r5, pc}
	.4byte 0x00002f99
	.4byte 0x02008be4
	.irp EntryTarget, 0x080003c9, 0x080003d1, 0x080003d9, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80a9, 0x080c80b1, 0x080c80d1, 0x080c80d9, 0x080c80f1, 0x080c80f9, 0x080c8119, 0x080c8129, 0x080c8139, 0x080c8141, 0x080c8149, 0x080c8161, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81a1, 0x080c81d1, 0x080c81d9, 0x080c8201, 0x080c8211, 0x080c8219, 0x080c8229, 0x080c8239, 0x080c8241, 0x080c8249, 0x080c84e1, 0x080c85f9, 0x080c8601, 0x080c8779, 0x080c88a1, 0x080c88a9, 0x080c88b1, 0x08108009, 0x08108011, 0x08108019, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.section .rodata,"a",%progbits
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000011
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
	.4byte 0x00000060
	.4byte 0x1010105f
	.4byte 0xffffffff
	.4byte 0x1020205f
	.4byte 0xffffffff
	.4byte 0x1050505f
	.4byte 0xffffffff
	.4byte 0x1060605f
	.4byte 0xffffffff
	.4byte 0x1070705f
	.4byte 0xffffffff
	.4byte 0x11414060
	.4byte 0xffffffff
	.4byte 0x11515060
	.4byte 0xffffffff
	.4byte 0x11616060
	.4byte 0xffffffff
	.4byte 0x11717060
	.4byte 0xffffffff
	.4byte 0x11818060
	.4byte 0xffffffff
	.4byte 0x11919060
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00014000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00014000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00008000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x0001e000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0002b000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00022000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x00014000
	.4byte 0xffff00d1
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0000a000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00006000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02fa0000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x0001c000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x03600000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0001c000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00014000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00012000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x0000a000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x0000e000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x02200000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00004000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00005000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00008000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0001e000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00014000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00014000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00008000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x0000e000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0001b000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00012000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x00014000
	.4byte 0xffff00d1
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0000a000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x03600000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02fa0000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x0001c000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00010000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0001c000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00010000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00012000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0000a000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0000e000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x02000000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00005000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x0000a000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0001e000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00014000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00014000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00008000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x0000e000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0000b000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00002000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00004000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x00014000
	.4byte 0xffff00d1
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0000a000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x03600000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02fa0000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x0001c000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00010000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0001c000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00012000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0000a000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0000e000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x02000000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00005000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x0000a000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x013c0000
	.4byte 0x00003000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x0000b000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004401
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00004401
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00004401
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00004401
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00004401
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0xffff0015
	.4byte 0x00000015
	.4byte 0x00000001
	.4byte 0xffff0016
	.4byte 0x00000016
	.4byte 0x00000001
	.4byte 0xffff0017
	.4byte 0x00000017
	.4byte 0x00000001
	.4byte 0xffff0018
	.4byte 0x00000018
	.4byte 0x00000001
	.4byte 0xffff0019
	.4byte 0x00000019
	.4byte 0x0000c400
	.4byte 0xffff0008
	.4byte 0x020081ad
	.4byte 0x0000c400
	.4byte 0xffff0010
	.4byte 0x020081fd
	.4byte 0x0000c400
	.4byte 0xffff0009
	.4byte 0x0200824d
	.4byte 0x00000173
	.4byte 0xffff00d6
	.4byte 0x00403051
	.4byte 0x00000173
	.4byte 0xffff00d7
	.4byte 0x00403052
	.4byte 0x00000173
	.4byte 0xffff00d8
	.4byte 0x00403053
	.4byte 0x00000173
	.4byte 0xffff00d9
	.4byte 0x00403054
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x00403055
	.4byte 0x00000002
	.4byte 0x08b2001e
	.4byte 0x02008085
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001b03
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001b04
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001b05
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001b06
	.4byte 0x00000000
	.4byte 0x08b1000e
	.4byte 0x020082bd
	.4byte 0x00008d15
	.4byte 0x08b1040e
	.4byte 0x020082bd
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001b07
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008299
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008445
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001b0f
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001b10
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001b11
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001b12
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001b13
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001b18
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001b19
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001b1a
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001b1b
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001b1c
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001b1d
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001b1e
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001b1f
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00001b20
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00001b21
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00001b22
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00001b23
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x00001b24
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x00001b25
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x00001b26
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte 0x00001b27
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x00001b28
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte 0x00001b2e
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001b2f
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001b30
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001b31
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001b32
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001b33
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001b34
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001b35
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001b36
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001b37
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x00001b38
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00001b39
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x00001b3a
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x00001b3b
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte 0x00001b3c
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte 0x00001b3d
	.4byte 0x00008d15
	.4byte 0xffff041f
	.4byte 0x0200847d
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001b40
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001b41
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004401
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00004401
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00004401
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00004401
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00004401
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0xffff0015
	.4byte 0x00000015
	.4byte 0x00000001
	.4byte 0xffff0016
	.4byte 0x00000016
	.4byte 0x00000001
	.4byte 0xffff0017
	.4byte 0x00000017
	.4byte 0x00000001
	.4byte 0xffff0018
	.4byte 0x00000018
	.4byte 0x00000001
	.4byte 0xffff0019
	.4byte 0x00000019
	.4byte 0x0000c400
	.4byte 0xffff0008
	.4byte 0x020081ad
	.4byte 0x0000c400
	.4byte 0xffff0010
	.4byte 0x020081fd
	.4byte 0x0000c400
	.4byte 0xffff0009
	.4byte 0x0200824d
	.4byte 0x00000173
	.4byte 0xffff00d6
	.4byte 0x00403051
	.4byte 0x00000173
	.4byte 0xffff00d7
	.4byte 0x00403052
	.4byte 0x00000173
	.4byte 0xffff00d8
	.4byte 0x00403053
	.4byte 0x00000173
	.4byte 0xffff00d9
	.4byte 0x00403054
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x00403055
	.4byte 0x00000000
	.4byte 0x1971001e
	.4byte 0x00001c73
	.4byte 0x00008d15
	.4byte 0x1971001e
	.4byte 0x00001c74
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001c2f
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001c30
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001c31
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001c32
	.4byte 0x00000000
	.4byte 0x08b8000c
	.4byte 0x020084b9
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001c34
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001c35
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001c36
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001c37
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001c38
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001c39
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001c3a
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001c3b
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001c5d
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001c5e
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001c5f
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001c60
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001c61
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001c62
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001c63
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00001c64
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte 0x00001c65
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x00001c66
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte 0x020084f5
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001c6a
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001c6b
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001c6c
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001c6d
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001c6e
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001c6f
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte 0x00001c70
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte 0x00001c71
	.4byte 0x00008d15
	.4byte 0xffff001f
	.4byte 0x00001c72
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001c75
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001c76
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004401
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00004401
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00004401
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00004401
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00004401
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0xffff0015
	.4byte 0x00000015
	.4byte 0x00000001
	.4byte 0xffff0016
	.4byte 0x00000016
	.4byte 0x00000001
	.4byte 0xffff0017
	.4byte 0x00000017
	.4byte 0x00000001
	.4byte 0xffff0018
	.4byte 0x00000018
	.4byte 0x00000001
	.4byte 0xffff0019
	.4byte 0x00000019
	.4byte 0x0000c400
	.4byte 0xffff0008
	.4byte 0x020081ad
	.4byte 0x0000c400
	.4byte 0xffff0010
	.4byte 0x020081fd
	.4byte 0x0000c400
	.4byte 0xffff0009
	.4byte 0x0200824d
	.4byte 0x00000173
	.4byte 0xffff00d6
	.4byte 0x00403051
	.4byte 0x00000173
	.4byte 0xffff00d7
	.4byte 0x00403052
	.4byte 0x00000173
	.4byte 0xffff00d8
	.4byte 0x00403053
	.4byte 0x00000173
	.4byte 0xffff00d9
	.4byte 0x00403054
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x00403055
	.4byte 0x00000002
	.4byte 0x1823001f
	.4byte 0x02008739
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002588
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002589
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000258a
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000258b
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000258c
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000258d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0000258e
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0000258f
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002590
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002591
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002592
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002593
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000025a6
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000025a7
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x000025a8
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000025a9
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x000025aa
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x000025ab
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x000025ac
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x000025ad
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte 0x000025ae
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x000025af
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000025b0
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000025b1
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000025b2
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x000025b3
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x000025b4
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x000025b5
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte 0x000025b6
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte 0x000025b7
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000025b8
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000025b9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
