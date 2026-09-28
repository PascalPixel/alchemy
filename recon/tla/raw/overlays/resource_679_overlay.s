.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008fb9, 0x0200834d, 0x02008359, 0x02008361, 0x02008d91, 0x02008355, 0x02009051
	overlay_veneer \EntryTarget
	.endr
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_0200007c
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200007c
	ldr	r1, [r5, #80]
	movs	r2, #13
	ldrb	r0, [r1, #9]
	movs	r3, #3
	negs	r2, r2
	ands	r4, r3
	adds	r3, r2, #0
	lsls	r4, r4, #2
	ands	r3, r0
	orrs	r3, r4
	strb	r3, [r1, #9]
	adds	r1, #37
	ldrb	r3, [r1, #0]
	ands	r2, r3
	orrs	r2, r4
	strb	r2, [r1, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
.L_0200007c:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r4, r0, #0
	adds	r5, r1, #0
	adds	r6, r2, #0
	adds	r0, r3, #0
	adds	r2, r5, #0
	adds	r1, r4, #0
	adds	r3, r6, #0
	bl 0x02009c7c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020000ca
	movs	r1, #0
	bl 0x02008038
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r2, #4
	movs	r3, #8
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02009ca4
	adds	r0, r5, #0
	movs	r1, #14
	bl 0x02009d3c
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x02009cac
	adds	r0, r5, #0
	b.n	.L_020000cc
.L_020000ca:
	movs	r0, #0
.L_020000cc:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r4, r0, #0
	adds	r5, r1, #0
.L_020000d6:
	adds	r6, r2, #0
	adds	r0, r3, #0
	adds	r2, r5, #0
	adds	r1, r4, #0
	adds	r3, r6, #0
	bl 0x02009c7c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0200011e
	movs	r1, #1
	bl 0x02008038
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r2, #4
	movs	r3, #8
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02009ca4
	adds	r0, r5, #0
	movs	r1, #15
	bl 0x02009d3c
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #34
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r0, r5, #0
	b.n	.L_02000120
.L_0200011e:
	movs	r0, #0
.L_02000120:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x6c426883
	.4byte 0x189b6d01
	.4byte 0x6c826083
	.4byte 0x189b68c3
	.4byte 0x6cc260c3
	.4byte 0x189b6903
	.4byte 0x6b026103
	.4byte 0x189b6983
	.4byte 0x6b426183
	.4byte 0x189b69c3
	.4byte 0x306461c3
	.4byte 0x88028a4b
	.4byte 0x824b189b
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	str	r3, [sp, #0]
	ldr	r3, [pc, #444]
	mov	r8, r2
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r1, #0
	ldr	r1, [sp, #44]
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	mov	sl, r1
	ldr	r7, [sp, #48]
	bl 0x02009cdc
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, sl
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_020001a4
	cmp	r7, #0
	beq.n	.L_020001a4
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_020001ac
.L_020001a4:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_020001ac:
	mov	r3, r8
	bl 0x02009c7c
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_020001ba
	b.n	.L_0200031e
.L_020001ba:
	ldr	r3, [r6, #80]
	mov	r1, sl
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	mov	r8, r3
	bl 0x02009c6c
	ldr	r2, [pc, #352]
	mov	r3, sl
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	fp, r3
	bl 0x02009c74
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02009ca4
	ldr	r3, [pc, #324]
	mov	r1, r9
	str	r3, [r6, #108]
	ldr	r3, [sp, #0]
	adds	r0, r6, #0
	str	r3, [r6, #68]
	ldr	r3, [sp, #36]
	str	r3, [r6, #72]
	ldr	r3, [sp, #40]
	str	r3, [r6, #76]
	ldr	r3, [r1, #80]
	ldrb	r1, [r3, #9]
	lsls	r1, r1, #28
	lsrs	r1, r1, #30
	bl 0x02008038
	movs	r2, #100
	adds	r2, r2, r6
	mov	r9, r2
	mov	r3, r9
	str	r5, [r6, #48]
	str	r5, [r6, #52]
	strh	r5, [r3, #0]
	ldr	r3, [pc, #280]
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200031e
	cmp	r7, #0
	beq.n	.L_0200031e
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200023c
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x02009d3c
.L_0200023c:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, sl
	ands	r3, r2
.L_02000244:
	cmp	r3, #0
	beq.n	.L_02000274
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #3
	ldrb	r2, [r7, #0]
	adds	r0, r6, #0
	ands	r2, r3
	mov	r3, r8
	ldrb	r1, [r3, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	lsls	r2, r2, #2
	mov	r1, r8
	orrs	r3, r2
	strb	r3, [r1, #9]
	ldr	r1, [r7, #0]
	bl 0x02008038
.L_02000274:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, sl
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_02000288
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_02000288:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020002ce
	ldr	r3, [pc, #152]
	mov	r1, fp
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_020002b6
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x02009bcc
.L_020002aa:
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_020002c8
.L_020002b6:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x02009bcc
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_020002c8:
	bl 0x02009bcc
	str	r0, [r6, #52]
.L_020002ce:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020002ea
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x02009c6c
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x02009c74
.L_020002ea:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020002fc
	ldrh	r3, [r7, #32]
	mov	r1, r8
	strh	r3, [r1, #18]
.L_020002fc:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200030e
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_0200030e:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200031e
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_0200031e:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200a038
	.4byte 0x02008125
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb500
	movs	r0, #10
	movs	r1, #3
	movs	r2, #11
	bl 0x02009dd4
	pop	{pc}
	.2byte 0x0000
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xa11c
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xa14c
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #44]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #36]
	cmp	r2, r3
	bne.n	.L_02000378
	ldr	r0, [pc, #32]
	b.n	.L_0200038e
.L_02000378:
	ldr	r3, [pc, #32]
	cmp	r2, r3
	bne.n	.L_02000382
	ldr	r0, [pc, #32]
	b.n	.L_0200038e
.L_02000382:
	ldr	r3, [pc, #32]
	cmp	r2, r3
	bne.n	.L_0200038c
	ldr	r0, [pc, #28]
	b.n	.L_0200038e
.L_0200038c:
	ldr	r0, [pc, #28]
.L_0200038e:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000009a
	.4byte 0x0200a218
	.4byte 0x0000009c
	.4byte 0x0200a320
	.4byte 0x0000009b
	.4byte 0x0200a458
	.2byte 0xa200
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #20]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x02009cdc
	movs	r3, #2
	adds	r0, #34
	strb	r3, [r0, #0]
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #20]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x02009cdc
	movs	r3, #0
	adds	r0, #34
	strb	r3, [r0, #0]
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r5, #0
	ldrsh	r3, [r3, r5]
	cmp	r3, #0
	beq.n	.L_02000404
	b.n	.L_0200051a
.L_02000404:
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #164
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	r8, r3
	cmp	r3, #0
	beq.n	.L_0200041a
	b.n	.L_0200051a
.L_0200041a:
	ldr	r7, [pc, #260]
	movs	r2, #1
	ldr	r3, [r7, #0]
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_02000460
	ldr	r6, [pc, #252]
	movs	r1, #160
	lsls	r1, r1, #19
	ldr	r5, [pc, #248]
	adds	r0, r6, #0
	adds	r1, #96
	movs	r2, #32
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x1c31
	movs	r2, #32
	ldr	r0, [pc, #236]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4e3b
	movs	r1, #160
	lsls	r1, r1, #19
	adds	r1, #128
	movs	r2, #32
	adds	r0, r6, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4838
	adds	r1, r6, #0
	movs	r2, #32
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4643
	str	r3, [r7, #0]
.L_02000460:
	ldr	r3, [r7, #0]
	cmp	r3, #0
	bge.n	.L_02000468
	adds	r3, #7
.L_02000468:
	asrs	r2, r3, #3
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_02000472
	adds	r3, r2, #3
.L_02000472:
	asrs	r1, r3, #2
	lsls	r3, r1, #2
	subs	r1, r2, r3
	ldr	r5, [pc, #184]
	ldr	r2, [pc, #180]
	ldr	r7, [pc, #172]
	ldr	r6, [pc, #164]
	movs	r4, #0
	mov	ip, r5
	mov	r8, r2
	movs	r0, #24
.L_02000488:
	adds	r2, r1, r4
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_02000492
	adds	r3, r2, #3
.L_02000492:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r2, r3
	ldrh	r2, [r6, r0]
	lsls	r3, r3, #1
	adds	r3, #24
	strh	r2, [r7, r3]
	mov	r5, r8
	ldrh	r5, [r5, r0]
	mov	r2, ip
	adds	r4, #1
	strh	r5, [r2, r3]
	adds	r0, #2
	cmp	r4, #3
	ble.n	.L_02000488
	ldr	r0, [pc, #132]
	ldr	r1, [pc, #136]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_020004e0
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r0
	adds	r3, #4
	adds	r2, #1
	stmia	r3!, {r7}
	strh	r2, [r0, #0]
	movs	r2, #160
	lsls	r2, r2, #19
	adds	r2, #96
	stmia	r3!, {r2}
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #8
	str	r2, [r3, #0]
.L_020004e0:
	strh	r4, [r1, #0]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_02000510
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r0
	adds	r3, #4
	strh	r2, [r0, #0]
	mov	r2, ip
	stmia	r3!, {r2}
	movs	r2, #160
	lsls	r2, r2, #19
	adds	r2, #128
	stmia	r3!, {r2}
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #8
	str	r2, [r3, #0]
.L_02000510:
	strh	r4, [r1, #0]
	ldr	r2, [pc, #12]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
.L_0200051a:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0200a4c4
	.4byte 0x0200a958
	.4byte 0x03000730
	.4byte 0x0200a978
	.4byte 0x0200a998
	.4byte 0x0200a9b8
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r5, #0
	ldrsh	r3, [r3, r5]
	cmp	r3, #0
	beq.n	.L_0200055c
	b.n	.L_0200066a
.L_0200055c:
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #164
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	r8, r3
	cmp	r3, #0
	beq.n	.L_02000572
	b.n	.L_0200066a
.L_02000572:
	ldr	r7, [pc, #252]
	movs	r2, #1
	ldr	r3, [r7, #0]
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_020005b4
	ldr	r6, [pc, #244]
	movs	r1, #160
	lsls	r1, r1, #19
	ldr	r5, [pc, #240]
	adds	r0, r6, #0
	adds	r1, #96
	movs	r2, #32
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x1c31
	movs	r2, #32
	ldr	r0, [pc, #228]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4e39
	ldr	r1, [pc, #228]
	movs	r2, #32
	adds	r0, r6, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4838
	adds	r1, r6, #0
	movs	r2, #32
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4643
	str	r3, [r7, #0]
.L_020005b4:
	ldr	r3, [r7, #0]
	cmp	r3, #0
	bge.n	.L_020005bc
	adds	r3, #7
.L_020005bc:
	asrs	r2, r3, #3
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_020005c6
	adds	r3, r2, #3
.L_020005c6:
	asrs	r1, r3, #2
	lsls	r3, r1, #2
	subs	r1, r2, r3
	ldr	r5, [pc, #184]
	ldr	r2, [pc, #176]
	ldr	r7, [pc, #168]
	ldr	r6, [pc, #160]
	movs	r4, #0
	mov	ip, r5
	mov	r8, r2
	movs	r0, #24
.L_020005dc:
	adds	r2, r1, r4
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_020005e6
	adds	r3, r2, #3
.L_020005e6:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r2, r3
	ldrh	r2, [r6, r0]
	lsls	r3, r3, #1
	adds	r3, #24
	strh	r2, [r7, r3]
	mov	r5, r8
	ldrh	r5, [r5, r0]
	mov	r2, ip
	adds	r4, #1
	strh	r5, [r2, r3]
	adds	r0, #2
	cmp	r4, #3
	ble.n	.L_020005dc
	ldr	r0, [pc, #132]
	ldr	r1, [pc, #136]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_02000634
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r0
	adds	r3, #4
	adds	r2, #1
	stmia	r3!, {r7}
	strh	r2, [r0, #0]
	movs	r2, #160
	lsls	r2, r2, #19
	adds	r2, #96
	stmia	r3!, {r2}
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #8
	str	r2, [r3, #0]
.L_02000634:
	strh	r4, [r1, #0]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_02000660
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r0
	adds	r3, #4
	strh	r2, [r0, #0]
	mov	r2, ip
	stmia	r3!, {r2}
	ldr	r2, [pc, #44]
	stmia	r3!, {r2}
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #8
	str	r2, [r3, #0]
.L_02000660:
	strh	r4, [r1, #0]
	ldr	r2, [pc, #12]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
.L_0200066a:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0200a4c8
	.4byte 0x0200a9d8
	.4byte 0x03000730
	.4byte 0x0200a9f8
	.4byte 0x0200aa18
	.4byte 0x050001a0
	.4byte 0x0200aa38
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	push	{lr}
	ldr	r0, [pc, #12]
	bl 0x02009bf4
	movs	r0, #3
	bl 0x02009bdc
	pop	{pc}
	.2byte 0x8541
	.2byte 0x0200
	push	{lr}
	ldr	r0, [pc, #12]
	bl 0x02009bfc
	movs	r0, #3
	bl 0x02009bdc
	pop	{pc}
	.2byte 0x8541
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #144]
	movs	r2, #1
	ldr	r3, [r5, #0]
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_020006e2
	movs	r1, #160
	lsls	r1, r1, #19
	ldr	r3, [pc, #132]
	ldr	r0, [pc, #132]
	adds	r1, #128
	movs	r2, #32
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2300
	str	r3, [r5, #0]
.L_020006e2:
	ldr	r0, [r5, #0]
	movs	r1, #15
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	bl 0x02009bd4
	movs	r5, #0
	adds	r7, r0, #0
	movs	r6, #2
.L_020006f6:
	ldr	r3, [pc, #104]
	adds	r0, r7, r5
	movs	r1, #15
	mov	r8, r3
	bl 0x02009bd4
	ldr	r3, [pc, #88]
	lsls	r0, r0, #1
	ldrh	r3, [r3, r6]
	adds	r0, #2
	mov	r2, r8
	adds	r5, #1
	strh	r3, [r2, r0]
	adds	r6, #2
	cmp	r5, #14
	ble.n	.L_020006f6
	ldr	r1, [pc, #76]
	ldr	r0, [pc, #76]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_02000744
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r1
	adds	r3, #4
	strh	r2, [r1, #0]
	mov	r2, r8
	stmia	r3!, {r2}
	ldr	r2, [pc, #48]
	stmia	r3!, {r2}
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #8
	str	r2, [r3, #0]
.L_02000744:
	strh	r4, [r0, #0]
	ldr	r2, [pc, #12]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0200a4cc
	.4byte 0x03000730
	.4byte 0x0200aa58
	.4byte 0x0200aa78
	.4byte 0x020038e0
	.4byte 0x04000208
	.2byte 0x03c0
	.2byte 0x0500
	push	{r5, lr}
	movs	r1, #0
	adds	r5, r0, #0
	bl 0x02009cb4
	movs	r3, #0
	str	r3, [r5, #108]
	pop	{r5, pc}
	push	{lr}
	ldr	r3, [pc, #28]
	movs	r2, #2
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000796
	movs	r1, #7
	bl 0x02009cb4
	b.n	.L_0200079c
.L_02000796:
	movs	r1, #0
	bl 0x02009cb4
.L_0200079c:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, lr}
	adds	r0, r1, #0
	bl 0x02009cdc
	adds	r5, r0, #0
	bl 0x02009cc4
	movs	r0, #0
	bl 0x02009dcc
	ldr	r3, [pc, #420]
	movs	r0, #78
	str	r3, [r5, #108]
	bl 0x02009e2c
	movs	r0, #30
	bl 0x02009bdc
	movs	r0, #21
	bl 0x02009e2c
	bl 0x02009ad8
	movs	r0, #17
	bl 0x02009ba0
	movs	r0, #170
	bl 0x02009dec
	movs	r5, #3
.L_020007e0:
	bl 0x02009c04
	adds	r3, r0, #0
	movs	r0, #254
	lsls	r0, r0, #7
	adds	r0, #255
	ands	r0, r3
	movs	r1, #0
	bl 0x02009da4
	movs	r0, #15
	bl 0x02009dac
	movs	r0, #10
	bl 0x02009bdc
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x02009da4
	movs	r0, #10
	bl 0x02009dac
	subs	r5, #1
	movs	r0, #10
	bl 0x02009bdc
	cmp	r5, #0
	bge.n	.L_020007e0
	bl 0x02009de4
	ldr	r0, [pc, #320]
	ldr	r1, [pc, #324]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_02000852
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r0
	strh	r2, [r0, #0]
	movs	r2, #240
	adds	r3, #4
	lsls	r2, r2, #4
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_02000852:
	strh	r4, [r1, #0]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_02000882
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r0
	strh	r2, [r0, #0]
	movs	r2, #128
	adds	r3, #4
	lsls	r2, r2, #5
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_02000882:
	strh	r4, [r1, #0]
	movs	r0, #1
	bl 0x02009bdc
	ldr	r0, [pc, #224]
	bl 0x02009bec
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #216]
	bl 0x02009be4
	bl 0x020097bc
	movs	r0, #33
	movs	r1, #0
	movs	r2, #0
	bl 0x020097f8
	movs	r5, #1
.L_020008aa:
	ldr	r1, [pc, #184]
	ldr	r0, [pc, #184]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r3, [r1, #0]
	cmp	r3, #31
	bgt.n	.L_020008de
	lsls	r2, r3, #1
	adds	r2, r2, r3
	adds	r3, #1
	lsls	r2, r2, #2
	strh	r3, [r1, #0]
	movs	r3, #128
	adds	r2, r2, r1
	lsls	r3, r3, #5
	adds	r2, #4
	orrs	r3, r5
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L_020008de:
	strh	r4, [r0, #0]
	movs	r0, #4
	adds	r5, #1
	bl 0x02009bdc
	cmp	r5, #6
	ble.n	.L_020008aa
	movs	r0, #134
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009c4c
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #94
	bl 0x02009c4c
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x02009d7c
	movs	r0, #180
	movs	r1, #1
	movs	r2, #164
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x02009d84
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #20
	movs	r0, #16
	bl 0x02009d6c
	bl 0x02009d8c
	movs	r0, #220
	movs	r1, #1
	movs	r2, #188
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	bl 0x02009d84
	bl 0x02009d8c
	movs	r0, #236
	movs	r1, #1
	movs	r2, #204
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	bl 0x02009d84
	movs	r0, #77
	bl 0x02009d94
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02008781
	.4byte 0x020038e0
	.4byte 0x04000208
	.4byte 0x02008541
	.2byte 0x86bd
	.2byte 0x0200
	push	{lr}
	bl 0x02009cc4
	movs	r0, #0
	bl 0x02009dcc
	movs	r0, #17
	bl 0x02009cdc
	ldr	r3, [pc, #180]
	str	r3, [r0, #108]
	ldr	r0, [pc, #180]
	bl 0x02009d44
	ldr	r3, [pc, #176]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #16
	movs	r2, #0
	bl 0x02009d34
	bl 0x02009db4
	bl 0x02009dc4
	movs	r2, #0
	movs	r0, #16
	movs	r1, #0
	bl 0x02009d5c
	movs	r0, #16
	movs	r1, #3
	bl 0x02009d14
	movs	r1, #128
	movs	r2, #0
	movs	r0, #16
	lsls	r1, r1, #8
	bl 0x02009d5c
	movs	r0, #16
	movs	r1, #3
	bl 0x02009d14
	movs	r1, #224
	movs	r2, #0
	movs	r0, #16
	lsls	r1, r1, #8
	bl 0x02009d5c
	movs	r0, #16
	movs	r1, #3
	bl 0x02009d14
	movs	r1, #129
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #16
	bl 0x02009d6c
	movs	r0, #16
	movs	r1, #0
	bl 0x02009d54
	movs	r2, #25
	movs	r0, #16
	movs	r1, #2
	bl 0x02009d1c
	movs	r0, #16
	movs	r1, #0
	bl 0x02009d54
	movs	r0, #16
	movs	r1, #1
	bl 0x02009d24
	movs	r0, #16
	movs	r1, #0
	bl 0x02009d54
	movs	r0, #16
	movs	r1, #3
	bl 0x02009d14
	movs	r0, #16
	movs	r1, #0
	bl 0x02009d54
	bl 0x02009ccc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #94
	bl 0x02009c54
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02008771
	.4byte 0x0000227b
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	sub	sp, #12
	movs	r3, #68
	movs	r2, #4
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #68
	movs	r1, #38
	movs	r2, #39
	movs	r3, #24
	bl 0x02009e0c
	add	sp, #12
	pop	{pc}
	push	{r5, r6, lr}
	ldr	r3, [pc, #136]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	sub	sp, #12
	bl 0x02009cdc
	movs	r3, #68
	movs	r2, #4
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	adds	r5, r0, #0
	movs	r1, #38
	movs	r0, #68
	movs	r2, #38
	movs	r3, #21
	bl 0x02009e0c
	movs	r0, #240
	lsls	r0, r0, #4
	adds	r0, #155
	bl 0x02009c44
	cmp	r0, #0
	bne.n	.L_02000aee
	movs	r1, #188
	movs	r2, #188
	movs	r0, #65
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x02009d04
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #23
	bne.n	.L_02000aee
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #23
	bne.n	.L_02000aee
	bl 0x02009cc4
	movs	r0, #0
	bl 0x02009dcc
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x02009d74
	ldr	r0, [r6, #0]
	movs	r1, #16
	movs	r2, #0
	bl 0x02009cf4
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r0, [r6, #0]
	bl 0x02009cfc
	bl 0x02009ccc
.L_02000aee:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [pc, #136]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	sub	sp, #12
	bl 0x02009cdc
	movs	r3, #68
	movs	r2, #4
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	adds	r5, r0, #0
	movs	r1, #38
	movs	r0, #68
	movs	r2, #38
	movs	r3, #21
	bl 0x02009e0c
	movs	r0, #240
	lsls	r0, r0, #4
	adds	r0, #154
	bl 0x02009c44
	cmp	r0, #0
	bne.n	.L_02000b7e
	movs	r1, #150
	movs	r2, #204
	movs	r0, #64
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x02009d04
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #37
	bne.n	.L_02000b7e
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #25
	bne.n	.L_02000b7e
	bl 0x02009cc4
	movs	r0, #0
	bl 0x02009dcc
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x02009d74
	ldr	r0, [r6, #0]
	movs	r1, #16
	movs	r2, #0
	bl 0x02009cf4
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r0, [r6, #0]
	bl 0x02009cfc
	bl 0x02009ccc
.L_02000b7e:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	sub	sp, #12
	movs	r3, #67
	movs	r2, #8
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #67
	movs	r1, #38
	movs	r2, #11
	movs	r3, #6
	bl 0x02009e0c
	add	sp, #12
	pop	{pc}
	push	{r5, r6, lr}
	ldr	r3, [pc, #136]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	sub	sp, #12
	bl 0x02009cdc
	movs	r3, #67
	movs	r2, #8
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	adds	r5, r0, #0
	movs	r1, #38
	movs	r0, #67
	movs	r2, #11
	movs	r3, #6
	bl 0x02009e0c
	movs	r0, #240
	lsls	r0, r0, #4
	adds	r0, #158
	bl 0x02009c44
	cmp	r0, #0
	bne.n	.L_02000c2e
	movs	r1, #200
	movs	r2, #152
	movs	r0, #64
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x02009d04
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #12
	bne.n	.L_02000c2e
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #9
	bne.n	.L_02000c2e
	bl 0x02009cc4
	movs	r0, #0
	bl 0x02009dcc
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x02009d74
	ldr	r0, [r6, #0]
	movs	r1, #16
	movs	r2, #0
	bl 0x02009cf4
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r0, [r6, #0]
	bl 0x02009cfc
	bl 0x02009ccc
.L_02000c2e:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #108]
	bl 0x02009cc4
	movs	r0, #0
	bl 0x02009dcc
	ldr	r5, [pc, #100]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x02009ce4
	movs	r1, #2
	ldr	r0, [r5, #0]
	bl 0x02009d64
	ldr	r0, [r5, #0]
	bl 0x02009cdc
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #123
	bl 0x02009e2c
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x02009d0c
	movs	r2, #6
	movs	r1, #2
	negs	r2, r2
	ldr	r0, [r5, #0]
	bl 0x02009cec
	movs	r0, #10
	bl 0x02009cbc
	movs	r3, #170
	lsls	r3, r3, #1
	adds	r6, r6, r3
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	bl 0x02009d94
	bl 0x02009dbc
	bl 0x02009dc4
	bl 0x02009ccc
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [pc, #84]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	bl 0x02009cdc
	ldrh	r5, [r0, #6]
	movs	r3, #128
	lsls	r3, r3, #6
	adds	r5, r5, r3
	ldr	r3, [pc, #56]
	ands	r5, r3
	lsls	r5, r5, #16
	asrs	r5, r5, #16
	bl 0x02009cc4
	lsls	r5, r5, #16
	movs	r0, #0
	bl 0x02009dcc
	cmp	r5, #0
	beq.n	.L_02000d18
	movs	r2, #0
	ldr	r1, [r6, #0]
	movs	r0, #9
	bl 0x02009d34
	ldr	r0, [pc, #32]
	bl 0x02009d44
	movs	r0, #9
	movs	r1, #0
	bl 0x02009d54
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #6
	movs	r2, #0
	b.n	.L_02000d14
	.2byte 0x0000
	.4byte 0xffffc000
	.4byte 0x02000240
	.2byte 0x2280
	.2byte 0x0000
.L_02000d14:
	bl 0x02009d5c
.L_02000d18:
	bl 0x02009ccc
	movs	r0, #0
	pop	{r5, r6, pc}
	push	{r5, lr}
	ldr	r3, [pc, #64]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x02009cdc
	ldrh	r5, [r0, #6]
	movs	r3, #128
	lsls	r3, r3, #6
	adds	r5, r5, r3
	ldr	r3, [pc, #36]
	ands	r5, r3
	lsls	r5, r5, #16
	asrs	r5, r5, #16
	bl 0x02009cc4
	lsls	r5, r5, #16
	movs	r0, #0
	bl 0x02009dcc
	cmp	r5, #0
	beq.n	.L_02000d6c
	ldr	r0, [pc, #20]
	bl 0x02009d44
	movs	r0, #9
	movs	r1, #0
	bl 0x02009d54
	b.n	.L_02000d6c
	.4byte 0xffffc000
	.4byte 0x02000240
	.2byte 0x2282
	.2byte 0x0000
.L_02000d6c:
	bl 0x02009ccc
	movs	r0, #0
	pop	{r5, pc}
	push	{lr}
	ldr	r3, [pc, #20]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	movs	r2, #5
	bl 0x02009d2c
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #44]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #36]
	cmp	r2, r3
	bne.n	.L_02000da8
	ldr	r0, [pc, #32]
	b.n	.L_02000dbe
.L_02000da8:
	ldr	r3, [pc, #32]
	cmp	r2, r3
	bne.n	.L_02000db2
	ldr	r0, [pc, #32]
	b.n	.L_02000dbe
.L_02000db2:
	ldr	r3, [pc, #32]
	cmp	r2, r3
	bne.n	.L_02000dbc
	ldr	r0, [pc, #28]
	b.n	.L_02000dbe
.L_02000dbc:
	ldr	r0, [pc, #28]
.L_02000dbe:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000009a
	.4byte 0x0200a4d0
	.4byte 0x0000009c
	.4byte 0x0200a698
	.4byte 0x0000009b
	.4byte 0x0200a80c
	.2byte 0xa4b8
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x02009cdc
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
	bne.n	.L_02000e1c
	movs	r0, #8
	adds	r1, r5, #0
	bl 0x02009e24
	b.n	.L_02000e38
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_02000e1c:
	bl 0x02009cc4
	movs	r0, #0
	bl 0x02009dcc
	ldr	r0, [pc, #20]
	bl 0x02009d44
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02009d54
	bl 0x02009ccc
.L_02000e38:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x2291
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl 0x02009cc4
	movs	r0, #0
	bl 0x02009dcc
	ldr	r5, [pc, #68]
	adds	r0, r5, #0
	bl 0x02009d44
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x02009d4c
	bl 0x02009e14
	movs	r1, #0
	bl 0x02009cd4
	cmp	r0, #0
	bne.n	.L_02000e7a
	movs	r0, #10
	bl 0x02009cbc
	adds	r0, r5, #1
	bl 0x02009d44
	b.n	.L_02000e86
.L_02000e7a:
	movs	r0, #20
	bl 0x02009cbc
	adds	r0, r5, #2
	bl 0x02009d44
.L_02000e86:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02009d54
	bl 0x02009ccc
	pop	{r5, r6, pc}
	.2byte 0x2292
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x02009cdc
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
	bne.n	.L_02000ed4
	movs	r0, #23
	adds	r1, r5, #0
	bl 0x02009e1c
	b.n	.L_02000ef0
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_02000ed4:
	bl 0x02009cc4
	movs	r0, #0
	bl 0x02009dcc
	ldr	r0, [pc, #20]
	bl 0x02009d44
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02009d54
	bl 0x02009ccc
.L_02000ef0:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x228f
	.2byte 0x0000
	push	{lr}
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #6
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #32]
	ldr	r3, [r3, #0]
	cmp	r2, r3
	bge.n	.L_02000f2c
	ldr	r3, [pc, #28]
	ldrh	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #20
	strh	r2, [r3, #0]
	movs	r2, #128
	ldr	r3, [pc, #4]
	lsls	r2, r2, #19
	adds	r2, #80
	b.n	.L_02000f46
	.4byte 0x00000000
	.4byte 0x0200aa98
	.2byte 0xaa9c
	.2byte 0x0200
.L_02000f2c:
	ldr	r3, [pc, #36]
	ldrh	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #20
	strh	r2, [r3, #0]
	movs	r2, #128
	ldr	r3, [pc, #16]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #12]
	subs	r2, #2
.L_02000f46:
	strh	r3, [r2, #0]
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0000100c
	.4byte 0x00003f42
	.4byte 0x0200aa9e
	.4byte 0x049b23c0
	.4byte 0x21bc6a1a
	.4byte 0x18520049
	.4byte 0x5ed12306
	.4byte 0x23b54807
	.4byte 0x1a5b00db
	.4byte 0x4b066003
	.4byte 0x5e522102
	.4byte 0x801a4905
	.4byte 0x681b4b05
	.4byte 0x1ad2089b
	.4byte 0x4770800a
	.4byte 0x0200aa98
	.4byte 0x0200aa9c
	.4byte 0x0200aa9e
	.2byte 0x122c
	.2byte 0x0300
	push	{lr}
	ldr	r2, [pc, #20]
	movs	r0, #1
	movs	r1, #0
	bl 0x02009c0c
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #8]
	bl 0x02009be4
	pop	{pc}
	.4byte 0x02008ef9
	.2byte 0x8f59
	.2byte 0x0200
	push	{lr}
	movs	r0, #192
	lsls	r0, r0, #18
	ldr	r1, [r0, #32]
	movs	r3, #13
	ldrb	r2, [r1, #23]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #23]
	ldr	r3, [r0, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r0, r3, r2
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #2
	ldr	r1, [pc, #92]
	str	r3, [r0, #0]
	movs	r4, #240
	lsls	r4, r4, #1
	adds	r3, r1, r4
	movs	r4, #0
	ldrsh	r2, [r3, r4]
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_02000ff6
	bl 0x02009054
	b.n	.L_02001038
.L_02000ff6:
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_0200101a
	movs	r1, #144
	ldr	r0, [pc, #72]
	lsls	r1, r1, #3
	bl 0x02009be4
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #94
	bl 0x02009c44
	cmp	r0, #0
	beq.n	.L_02001038
	bl 0x02008974
	b.n	.L_02001038
.L_0200101a:
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_02001038
	movs	r3, #128
	lsls	r3, r3, #1
	str	r3, [r0, #0]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #6
	bne.n	.L_02001038
	bl 0x020090b4
.L_02001038:
	movs	r0, #0
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000009b
	.4byte 0x0000009a
	.4byte 0x02008541
	.2byte 0x009c
	.2byte 0x0000
	movs	r0, #0
	bx	lr
	push	{r5, lr}
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #80]
	bl 0x02009be4
	bl 0x02009dfc
	movs	r1, #128
	movs	r2, #8
	movs	r3, #9
	lsls	r1, r1, #2
	movs	r0, #0
	bl 0x02009e04
	movs	r0, #170
	bl 0x02009dec
	bl 0x02008f98
	ldr	r3, [pc, #48]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r5, r3, r2
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #3
	bne.n	.L_02001090
	bl 0x02009500
.L_02001090:
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #4
	bne.n	.L_0200109c
	bl 0x02009500
.L_0200109c:
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #5
	bne.n	.L_020010a8
	bl 0x02009500
.L_020010a8:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x020083e9
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r0, #48
	adds	r2, #93
	str	r2, [r3, #0]
	adds	r0, #255
	bl 0x02009c54
	pop	{pc}
	push	{lr}
	bl 0x02009d9c
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	movs	r0, #128
	lsls	r0, r0, #11
	adds	r2, r2, r0
	adds	r3, r3, r0
	ldr	r1, [r6, #8]
	movs	r0, #14
	bl 0x02009c7c
	ldr	r2, [r6, #80]
	adds	r5, r0, #0
	mov	r8, r2
	cmp	r5, #0
	beq.n	.L_02001134
	ldr	r3, [r6, #20]
	ldr	r7, [r5, #80]
	str	r3, [r5, #20]
	ldr	r1, [pc, #52]
	bl 0x02009c74
	adds	r3, r5, #0
	adds	r3, #85
	movs	r5, #0
	strb	r5, [r3, #0]
	cmp	r7, #0
	beq.n	.L_02001134
	movs	r1, #1
	adds	r0, r7, #0
	bl 0x02009c5c
	strb	r5, [r7, #26]
	mov	r2, r8
	ldrb	r3, [r2, #9]
	ldrb	r1, [r7, #9]
	movs	r2, #12
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r7, #9]
.L_02001134:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x9ee8
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #88]
	ldr	r7, [r3, #0]
	movs	r3, #15
	ands	r7, r3
	cmp	r7, #0
	bne.n	.L_02001198
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #16]
	movs	r0, #14
	adds	r0, #255
	bl 0x02009c7c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001198
	ldr	r1, [pc, #60]
	ldr	r6, [r5, #80]
	bl 0x02009c74
	adds	r3, r5, #0
	adds	r3, #85
	strb	r7, [r3, #0]
	ldr	r3, [pc, #48]
	adds	r2, r5, #0
	str	r3, [r5, #12]
	adds	r2, #34
	movs	r3, #1
	strb	r3, [r2, #0]
	cmp	r6, #0
	beq.n	.L_02001198
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x02009c5c
	ldrb	r3, [r6, #9]
	movs	r2, #13
	negs	r2, r2
	ands	r2, r3
	movs	r3, #8
	orrs	r2, r3
	strb	r7, [r6, #26]
	strb	r2, [r6, #9]
.L_02001198:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0300122c
	.4byte 0x02009ef4
	.2byte 0x8000
	.2byte 0xfff8
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r6, [pc, #320]
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r0, #133
	ldr	r2, [r3, #108]
	lsls	r0, r0, #2
	adds	r3, r6, r0
	movs	r1, #230
	ldr	r3, [r3, #0]
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldr	r2, [r2, #0]
	mov	r8, r3
	mov	r0, r8
	mov	sl, r2
	sub	sp, #12
	bl 0x02009cdc
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r6, r2
	adds	r5, r0, #0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #276]
	cmp	r2, r3
	bne.n	.L_020011ec
	ldr	r3, [r5, #20]
	cmp	r3, #0
	beq.n	.L_020012e8
.L_020011ec:
	ldr	r3, [r5, #8]
	mov	r7, sp
	str	r3, [r7, #0]
	movs	r1, #128
	ldr	r3, [r5, #12]
	lsls	r1, r1, #10
	str	r3, [r7, #4]
	adds	r0, r5, #0
	ldr	r3, [r5, #16]
	adds	r3, r3, r1
	str	r3, [r7, #8]
	adds	r1, r7, #0
	bl 0x02009c9c
	ldr	r3, [pc, #240]
	movs	r2, #4
	ldr	r3, [r3, #0]
	adds	r6, r0, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0200121c
	adds	r0, r5, #0
	bl 0x020090d8
.L_0200121c:
	cmp	r6, #0
	bge.n	.L_0200126e
	movs	r1, #129
	mov	r0, r8
	lsls	r1, r1, #1
	bl 0x02009d74
	ldr	r3, [r5, #16]
	movs	r0, #128
	lsls	r0, r0, #12
	adds	r3, r3, r0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	adds	r0, r5, #0
	bl 0x02009c84
	adds	r0, r5, #0
	movs	r1, #49
	bl 0x02009c6c
	adds	r0, r5, #0
	bl 0x02009c8c
.L_0200124a:
	movs	r0, #1
	bl 0x02009bdc
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #20]
	cmp	r2, r3
	bne.n	.L_0200124a
	adds	r0, r5, #0
	bl 0x020090d8
	adds	r0, r5, #0
	movs	r1, #49
	bl 0x02009c6c
	movs	r0, #3
	bl 0x02009bdc
	b.n	.L_020012e8
.L_0200126e:
	ldr	r3, [r5, #8]
	movs	r1, #128
	str	r3, [r7, #0]
	lsls	r1, r1, #12
	ldr	r3, [r5, #12]
	adds	r0, r5, #0
	str	r3, [r7, #4]
	ldr	r3, [r5, #16]
	adds	r3, r3, r1
	str	r3, [r7, #8]
	adds	r1, r7, #0
	bl 0x02009c9c
	adds	r6, r0, #0
	cmp	r6, #0
	bgt.n	.L_020012e8
	ldr	r3, [r5, #8]
	ldr	r2, [pc, #108]
	adds	r0, r5, #0
	adds	r3, r3, r2
	str	r3, [r7, #0]
	adds	r1, r7, #0
	ldr	r3, [r5, #12]
	str	r3, [r7, #4]
	ldr	r3, [r5, #16]
	adds	r3, r3, r2
	str	r3, [r7, #8]
	bl 0x02009c9c
	adds	r6, r0, #0
	cmp	r6, #0
	bgt.n	.L_020012e8
	ldr	r3, [r5, #8]
	ldr	r2, [pc, #80]
	ldr	r0, [pc, #76]
	adds	r3, r3, r2
	str	r3, [r7, #0]
	adds	r1, r7, #0
	ldr	r3, [r5, #12]
	str	r3, [r7, #4]
	ldr	r3, [r5, #16]
	adds	r3, r3, r0
	str	r3, [r7, #8]
	adds	r0, r5, #0
	bl 0x02009c9c
	adds	r6, r0, #0
	cmp	r6, #0
	bgt.n	.L_020012e8
	mov	r1, sl
	ldr	r3, [r1, #16]
	movs	r2, #128
	lsls	r2, r2, #10
	adds	r3, r3, r2
	str	r3, [r1, #16]
	ldr	r3, [r5, #16]
	adds	r3, r3, r2
	str	r3, [r5, #16]
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r5, #6]
.L_020012e8:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000009e
	.4byte 0x0300122c
	.4byte 0x0005b333
	.2byte 0x4ccd
	.2byte 0xfffa
	.2byte 0xb520
	bl 0x02009cdc
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001334
	movs	r1, #126
	adds	r1, #255
	ldr	r0, [r5, #80]
	bl 0x02009c64
	movs	r3, #0
	strb	r3, [r0, #5]
	strb	r3, [r0, #6]
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x02009c6c
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x02009c6c
.L_02001334:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	ldr	r5, [pc, #148]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	adds	r7, r0, #0
	ldr	r0, [r5, #0]
	bl 0x02009cdc
	adds	r6, r0, #0
	bl 0x02009cc4
	movs	r0, #0
	bl 0x02009dcc
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x02009d64
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r0, #215
	bl 0x02009e2c
	adds	r0, r6, #0
	movs	r1, #18
	bl 0x02009c6c
	movs	r0, #153
	lsls	r0, r0, #2
	bl 0x02009e2c
	movs	r5, #0
.L_0200138a:
	cmp	r5, #30
	bne.n	.L_02001392
	bl 0x02009dbc
.L_02001392:
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #60]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldrh	r3, [r6, #6]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r2
	strh	r3, [r6, #6]
	movs	r3, #7
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L_020013b6
	movs	r0, #15
	bl 0x02009cdc
	bl 0x020090d8
.L_020013b6:
	movs	r0, #1
	adds	r5, #1
	bl 0x02009bdc
	cmp	r5, #59
	ble.n	.L_0200138a
	bl 0x02009ccc
	adds	r0, r7, #0
	bl 0x02009d94
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xc000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #128]
	sub	sp, #56
	ldr	r2, [r3, #0]
	mov	r8, r3
	movs	r3, #1
	ands	r3, r2
	adds	r7, r0, #0
	cmp	r3, #0
	beq.n	.L_02001454
	movs	r3, #7
	add	r6, sp, #16
	str	r3, [r6, #4]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02001402
	movs	r3, #5
	str	r3, [r6, #4]
.L_02001402:
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	movs	r5, #0
	str	r3, [r6, #8]
	str	r3, [r6, #12]
	str	r5, [r6, #0]
	bl 0x02009c04
	lsls	r0, r0, #3
	lsrs	r0, r0, #16
	lsls	r4, r0, #1
	adds	r4, r4, r0
	lsls	r3, r4, #4
	adds	r4, r4, r3
	lsls	r3, r4, #8
	adds	r4, r4, r3
	mov	r3, r8
	ldr	r2, [r3, #0]
	movs	r3, #15
	ldr	r0, [r7, #8]
	ands	r2, r3
	movs	r3, #8
	subs	r3, r3, r2
	ldr	r1, [r7, #12]
	lsls	r3, r3, #16
	adds	r0, r0, r3
	movs	r3, #208
	lsls	r3, r3, #13
	adds	r1, r1, r3
	movs	r3, #176
	lsls	r3, r3, #12
	ldr	r2, [r7, #16]
	negs	r4, r4
	str	r3, [sp, #8]
	movs	r3, #0
	str	r4, [sp, #0]
	str	r5, [sp, #4]
	str	r6, [sp, #12]
	bl 0x0200815c
.L_02001454:
	movs	r0, #0
	add	sp, #56
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	ldr	r5, [pc, #136]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	mov	sl, r0
	ldr	r0, [r5, #0]
	bl 0x02009cdc
	adds	r6, r0, #0
	bl 0x02009cc4
	movs	r0, #0
	bl 0x02009dcc
	movs	r0, #228
	bl 0x02009e2c
	ldr	r3, [pc, #108]
	movs	r2, #0
	str	r3, [r6, #108]
	mov	r8, r2
	adds	r3, r6, #0
	mov	r2, r8
	adds	r3, #85
	strb	r2, [r3, #0]
	movs	r3, #204
	lsls	r3, r3, #6
	adds	r3, #51
	str	r3, [r6, #48]
	movs	r1, #2
	ldr	r0, [r5, #0]
	bl 0x02009d0c
	movs	r2, #8
	negs	r2, r2
	movs	r1, #0
	ldr	r0, [r5, #0]
	bl 0x02009cf4
	ldr	r0, [r5, #0]
	bl 0x02009cfc
	ldr	r0, [r5, #0]
	bl 0x02009cdc
	movs	r1, #9
	bl 0x02009d3c
	ldr	r0, [r5, #0]
	bl 0x02009cdc
	movs	r1, #0
	bl 0x02009ca4
	mov	r3, r8
	str	r3, [r6, #108]
	bl 0x02009dbc
	bl 0x02009dc4
	mov	r0, sl
	bl 0x02009d94
	bl 0x02009ccc
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x93d9
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #228]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	bl 0x02009cdc
	adds	r6, r0, #0
	movs	r0, #10
	adds	r0, #255
	bl 0x02009c44
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L_020015e6
	bl 0x02009cc4
	movs	r0, #0
	bl 0x02009dcc
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	negs	r0, r0
	negs	r1, r1
	movs	r3, #0
	bl 0x02009d84
	movs	r3, #85
	adds	r3, r3, r6
	strb	r7, [r3, #0]
	mov	r8, r3
	movs	r2, #10
	ldrsh	r1, [r6, r2]
	movs	r3, #18
	ldrsh	r2, [r6, r3]
	ldr	r3, [pc, #156]
	lsls	r2, r2, #16
	adds	r2, r2, r3
	lsls	r1, r1, #16
	ldr	r0, [r5, #0]
	bl 0x02009d04
	ldr	r0, [r5, #0]
	bl 0x02009cdc
	movs	r1, #9
	bl 0x02009d3c
	ldr	r0, [r5, #0]
	bl 0x02009cdc
	movs	r1, #0
	bl 0x02009ca4
	bl 0x02009db4
	movs	r0, #228
	bl 0x02009e2c
	ldr	r3, [pc, #112]
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	str	r3, [r6, #108]
	ldr	r0, [r5, #0]
	adds	r1, #102
	adds	r2, #51
	bl 0x02009ce4
	movs	r2, #8
	movs	r1, #0
	ldr	r0, [r5, #0]
	bl 0x02009ddc
	ldr	r0, [r5, #0]
	bl 0x02009cdc
	movs	r1, #0
	bl 0x02009d3c
	ldr	r0, [r5, #0]
.L_020015ae:
	bl 0x02009cdc
	movs	r1, #1
	bl 0x02009ca4
	ldr	r1, [r6, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
	movs	r2, #10
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x02009ddc
	movs	r3, #3
	mov	r2, r8
	strb	r3, [r2, #0]
	str	r7, [r6, #108]
	bl 0x02009df4
	bl 0x02009dc4
	bl 0x02009ccc
.L_020015e6:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0xfff00000
	.2byte 0x93d9
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	ldr	r3, [pc, #356]
	adds	r2, r1, #0
	adds	r2, #228
	ldr	r0, [r2, #0]
	ldr	r2, [r2, #4]
	ands	r0, r3
	ands	r2, r3
	ldr	r3, [r1, #0]
	sub	sp, #4
	ldr	r3, [r3, #4]
	ldr	r5, [pc, #340]
	str	r3, [sp, #0]
	mov	r8, r2
	ldrh	r3, [r5, #0]
	ldr	r2, [pc, #336]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r2, [r3, #2]
	ldrh	r4, [r3, #2]
	lsrs	r2, r2, #5
	mov	fp, r2
	ldr	r2, [pc, #328]
	movs	r3, #128
	adds	r1, r4, r2
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	mov	r9, r0
	adds	r3, #212
	ldr	r0, [pc, #316]
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #312]
	ldr	r0, [pc, #312]
	adds	r1, r4, r2
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #304]
	ldr	r0, [pc, #304]
	adds	r1, r4, r2
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #296]
	ldr	r0, [pc, #296]
	adds	r1, r4, r2
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #288]
	ldr	r0, [pc, #288]
	adds	r1, r4, r2
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #280]
	ldr	r0, [pc, #280]
	adds	r1, r4, r2
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #272]
	ldr	r0, [pc, #256]
	adds	r1, r4, r2
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #260]
	ldr	r0, [pc, #248]
	adds	r1, r4, r2
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #127
	adds	r5, #4
	mov	sl, r3
.L_020016c4:
	ldrh	r0, [r5, #18]
	cmp	r0, #0
	beq.n	.L_02001758
	ldr	r3, [r5, #0]
	mov	r1, r9
	subs	r4, r3, r1
	ldr	r3, [r5, #8]
	ldr	r2, [r5, #4]
	mov	r1, r8
	subs	r3, r3, r1
	subs	r1, r3, r2
	ldr	r3, [r5, #12]
	cmp	r2, r3
	bne.n	.L_020016ea
	movs	r2, #236
	lsls	r2, r2, #8
	mov	ip, r2
	cmp	r0, #2
	bne.n	.L_020016f0
.L_020016ea:
	movs	r3, #232
	lsls	r3, r3, #8
	mov	ip, r3
.L_020016f0:
	movs	r0, #0
	asrs	r3, r4, #16
	asrs	r2, r1, #16
	mov	lr, r0
	movs	r0, #167
	adds	r4, r3, #0
	adds	r1, r2, #0
	adds	r3, #7
	lsls	r0, r0, #1
	subs	r4, #8
	subs	r1, #8
	cmp	r3, r0
	bhi.n	.L_02001758
	adds	r3, r2, #0
	movs	r2, #143
	adds	r3, #39
	lsls	r2, r2, #1
	cmp	r3, r2
	bhi.n	.L_02001758
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r4, r3
	movs	r3, #255
	ands	r1, r3
	ldrh	r3, [r5, #16]
	adds	r7, r5, #0
	adds	r7, #20
	adds	r2, r7, #0
	lsls	r6, r3, #2
	movs	r0, #0
	cmp	r3, #2
	bne.n	.L_02001736
	movs	r0, #128
	lsls	r0, r0, #21
.L_02001736:
	mov	r3, lr
	str	r3, [r2, #0]
	lsls	r3, r4, #16
	orrs	r1, r3
	ldr	r3, [pc, #120]
	orrs	r1, r0
	orrs	r1, r3
	mov	r0, fp
	adds	r3, r0, r6
	str	r1, [r5, #24]
	mov	r1, ip
	orrs	r1, r3
	str	r1, [r5, #28]
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x02009c3c
.L_02001758:
	movs	r2, #1
	negs	r2, r2
	add	sl, r2
	mov	r3, sl
	adds	r5, #32
	cmp	r3, #0
	bge.n	.L_020016c4
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffff0000
	.4byte 0x0200aaa0
	.4byte 0x020036e0
	.4byte 0x06010000
	.4byte 0x0600f000
	.4byte 0x06010040
	.4byte 0x0600f400
	.4byte 0x06010080
	.4byte 0x0600f040
	.4byte 0x060100c0
	.4byte 0x0600f440
	.4byte 0x06010100
	.4byte 0x0600f080
	.4byte 0x06010140
	.4byte 0x0600f480
	.4byte 0x06010180
	.4byte 0x060101c0
	.2byte 0x0400
	.2byte 0x4000
	push	{r5, lr}
	ldr	r5, [pc, #44]
	movs	r1, #128
	lsls	r1, r1, #5
	ldr	r3, [pc, #40]
	adds	r1, #4
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xf000
	.2byte 0xfa31
	.2byte 0x8028
	movs	r1, #128
	ldrh	r0, [r5, #0]
	lsls	r1, r1, #2
	movs	r2, #0
	bl 0x02009c2c
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #12]
	bl 0x02009be4
	pop	{r5, pc}
	.4byte 0x0200aaa0
	.4byte 0x03000258
	.2byte 0x95f9
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	str	r1, [sp, #24]
	str	r2, [sp, #20]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r1, #132
	mov	ip, r3
	movs	r3, #160
	lsls	r3, r3, #1
	add	r3, ip
	ldr	r3, [r3, #48]
	lsls	r1, r1, #1
	mov	r8, r3
	mov	r3, ip
	add	r1, ip
	adds	r3, #236
	ldr	r2, [r1, #8]
	mov	sl, r0
	ldr	r0, [r3, #0]
	adds	r3, #4
	adds	r2, r2, r0
	str	r2, [sp, #16]
	ldr	r2, [r3, #0]
	ldr	r1, [r1, #12]
	adds	r3, #4
	adds	r1, r1, r2
	str	r1, [sp, #12]
	ldr	r1, [pc, #248]
	ldr	r3, [r3, #0]
	mov	r9, r1
	subs	r3, r3, r0
	asrs	r3, r3, #20
	str	r3, [sp, #8]
	mov	r3, ip
	adds	r3, #248
	ldr	r3, [r3, #0]
	asrs	r0, r0, #20
	subs	r3, r3, r2
	asrs	r3, r3, #20
	str	r3, [sp, #4]
	asrs	r2, r2, #20
	ldrh	r3, [r1, #2]
	lsls	r2, r2, #7
	adds	r2, r2, r0
	str	r2, [sp, #0]
	ldr	r2, [sp, #24]
	ldr	r1, [pc, #212]
	lsls	r3, r3, #5
	add	r3, r9
	adds	r5, r3, #4
	lsls	r3, r2, #8
	str	r3, [r1, #0]
	ldr	r1, [sp, #0]
	movs	r2, #0
	lsls	r3, r1, #2
	add	r8, r3
	ldr	r3, [sp, #4]
	mov	lr, r2
	cmp	lr, r3
	bge.n	.L_02001928
.L_02001880:
	mov	r1, lr
	lsls	r1, r1, #16
	lsrs	r3, r1, #7
	mov	r2, r8
	adds	r6, r2, r3
	ldr	r3, [sp, #8]
	movs	r7, #0
	mov	fp, r1
	cmp	r7, r3
	bge.n	.L_02001912
.L_02001894:
	ldrb	r4, [r6, #2]
	cmp	r4, #0
	beq.n	.L_020018fe
	cmp	r4, sl
	bcc.n	.L_020018fe
	mov	r3, sl
	adds	r3, #4
	cmp	r4, r3
	bcs.n	.L_020018fe
	ldr	r1, [sp, #16]
	lsls	r0, r7, #16
	lsrs	r0, r0, #16
	lsls	r3, r0, #20
	movs	r2, #128
	adds	r3, r3, r1
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [sp, #12]
	mov	r3, fp
	lsrs	r1, r3, #16
	lsls	r3, r1, #20
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r5, #8]
	ldr	r2, [sp, #20]
	lsls	r1, r1, #7
	lsls	r3, r2, #19
	str	r3, [r5, #12]
	ldr	r2, [sp, #24]
	adds	r0, r0, r1
	lsls	r3, r2, #19
	mov	r2, sl
	str	r3, [r5, #4]
	subs	r3, r4, r2
	strh	r3, [r5, #16]
	movs	r3, #1
	strh	r3, [r5, #18]
	mov	r2, r9
	ldrh	r3, [r2, #2]
	adds	r5, #32
	adds	r3, #1
	strh	r3, [r2, #2]
	movs	r3, #158
	lsls	r3, r3, #1
	add	r3, ip
	ldr	r2, [r3, #0]
	ldr	r3, [sp, #0]
	adds	r2, r2, r3
	movs	r3, #120
	strb	r3, [r2, r0]
.L_020018fe:
	movs	r1, #128
	lsls	r3, r7, #16
	lsls	r1, r1, #9
	ldr	r2, [sp, #8]
	adds	r3, r3, r1
	asrs	r7, r3, #16
	lsrs	r3, r3, #16
	adds	r6, #4
	cmp	r3, r2
	blt.n	.L_02001894
.L_02001912:
	mov	r1, lr
	movs	r2, #128
	lsls	r3, r1, #16
	lsls	r2, r2, #9
	adds	r3, r3, r2
	ldr	r2, [sp, #4]
	asrs	r1, r3, #16
	lsrs	r3, r3, #16
	mov	lr, r1
	cmp	r3, r2
	blt.n	.L_02001880
.L_02001928:
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200aaa0
	.2byte 0xc1e0
	.2byte 0x0202
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #32]
	ldr	r2, [r2, #116]
	adds	r3, #228
	ldr	r1, [r3, #0]
	ldr	r3, [r3, #4]
	mov	r8, r2
	mov	r5, r8
	movs	r2, #0
	adds	r5, #8
	mov	fp, r1
	mov	r9, r3
	mov	sl, r2
.L_0200196a:
	ldrh	r3, [r5, #28]
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r3, r3, r1
	adds	r2, r1, #0
	ands	r2, r3
	strh	r3, [r5, #28]
	cmp	r2, r1
	bne.n	.L_02001980
	b.n	.L_02001ab6
.L_02001980:
	movs	r0, #179
	lsls	r0, r0, #1
	bl 0x02009c44
	cmp	r0, #0
.L_0200198a:
	beq.n	.L_02001992
	ldrh	r3, [r5, #28]
	adds	r3, #1
	strh	r3, [r5, #28]
.L_02001992:
	ldrh	r2, [r5, #28]
	mov	r1, fp
	lsls	r3, r2, #2
	adds	r3, r3, r2
	ldr	r2, [pc, #164]
	lsls	r3, r3, #1
	adds	r4, r3, r2
	ldr	r3, [r5, #12]
	subs	r2, r3, r1
	cmp	r2, #0
	bge.n	.L_020019b0
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r2, r2, r3
.L_020019b0:
	movs	r1, #0
	ldrsh	r3, [r4, r1]
	asrs	r2, r2, #16
	adds	r7, r2, r3
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #20]
	adds	r4, #2
	subs	r3, r3, r2
	mov	r2, r9
	subs	r3, r3, r2
	cmp	r3, #0
	bge.n	.L_020019d0
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r3, r3, r1
.L_020019d0:
	movs	r1, #0
	ldrsh	r2, [r4, r1]
	asrs	r3, r3, #16
	adds	r6, r3, r2
	adds	r3, r7, #0
	adds	r3, #16
	adds	r4, #2
	cmp	r3, #255
	bhi.n	.L_02001a60
	movs	r2, #32
	negs	r2, r2
	cmp	r6, r2
	blt.n	.L_02001a60
	cmp	r6, #159
	bgt.n	.L_02001a60
	ldrb	r3, [r5, #9]
	movs	r1, #13
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r5, #9]
	ldr	r3, [pc, #48]
	ldr	r2, [pc, #48]
	ands	r7, r3
	ldrh	r3, [r5, #6]
	strb	r6, [r5, #4]
	ands	r3, r2
	orrs	r3, r7
	strh	r3, [r5, #6]
	mov	r2, r8
	ldrh	r3, [r4, #0]
	ldr	r1, [r2, #4]
	ldr	r2, [pc, #32]
	adds	r1, r1, r3
	ldr	r3, [pc, #32]
	adds	r4, #2
	ands	r1, r3
	ldrh	r3, [r5, #8]
	ldrb	r0, [r5, #5]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r5, #8]
	movs	r2, #63
	ldrb	r1, [r4, #0]
	adds	r3, r2, #0
	b.n	.L_02001a44
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0xfffffc00
	.4byte 0x000003ff
	.2byte 0x9f18
	.2byte 0x0200
.L_02001a44:
	lsls	r1, r1, #6
	ands	r3, r0
	orrs	r3, r1
	strb	r3, [r5, #5]
	ldrb	r1, [r5, #7]
	ldrb	r3, [r4, #2]
	ands	r2, r1
	lsls	r3, r3, #6
	orrs	r2, r3
	strb	r2, [r5, #7]
	adds	r0, r5, #0
	movs	r1, #240
	bl 0x02009c3c
.L_02001a60:
	ldrh	r3, [r5, #28]
	cmp	r3, #0
	bne.n	.L_02001ab6
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r3, #8
	add	r3, r8
	ldr	r6, [r3, #0]
	cmp	r6, #0
	beq.n	.L_02001aac
	bl 0x02009c04
	ldr	r3, [r6, #0]
	lsls	r2, r0, #4
	ldr	r1, [pc, #80]
	subs	r2, r2, r0
	lsls	r2, r2, #4
	adds	r3, r3, r2
	adds	r7, r3, r1
	bl 0x02009c04
	ldr	r3, [r6, #8]
	lsls	r2, r0, #2
	adds	r2, r2, r0
	lsls	r2, r2, #5
	adds	r3, r3, r2
	ldr	r2, [pc, #60]
	str	r7, [r5, #12]
	adds	r6, r3, r2
	str	r6, [r5, #20]
	movs	r0, #0
	adds	r1, r7, #0
	adds	r2, r6, #0
	bl 0x02009c94
	movs	r3, #16
	str	r0, [r5, #16]
	b.n	.L_02001ab4
.L_02001aac:
	movs	r3, #16
	str	r6, [r5, #12]
	str	r6, [r5, #20]
	str	r6, [r5, #16]
.L_02001ab4:
	strh	r3, [r5, #28]
.L_02001ab6:
	movs	r3, #1
	add	sl, r3
	mov	r1, sl
	adds	r5, #32
	cmp	r1, #63
	bhi.n	.L_02001ac4
	b.n	.L_0200196a
.L_02001ac4:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xff880000
	.2byte 0x0000
	.2byte 0xffb0
	.2byte 0xb5e0
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r1, #20
	movs	r0, #116
	sub	sp, #8
	bl 0x02009c14
	movs	r3, #128
	adds	r5, r0, #0
	movs	r0, #0
	str	r0, [sp, #0]
	adds	r7, r5, #0
	add	r0, sp, #4
	movs	r1, #0
	lsls	r3, r3, #19
	str	r1, [r0, #0]
	adds	r7, #8
	adds	r3, #212
	adds	r1, r5, #0
	ldr	r2, [pc, #136]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #3
	movs	r0, #56
	bl 0x02009c14
	adds	r6, r0, #0
	adds	r1, r6, #0
	ldr	r0, [pc, #120]
	bl 0x02009c24
	bl 0x02009c34
	movs	r1, #192
	str	r0, [r5, #0]
	lsls	r1, r1, #2
	adds	r2, r6, #0
	bl 0x02009c2c
	str	r0, [r5, #4]
	movs	r0, #56
	bl 0x02009c1c
	movs	r3, #128
	lsls	r3, r3, #4
	ldr	r0, [sp, #0]
	adds	r3, #8
	adds	r5, r5, r3
	str	r0, [r5, #0]
	movs	r5, #0
.L_02001b40:
	movs	r2, #0
	adds	r3, r7, #0
	str	r7, [sp, #0]
	stmia	r3!, {r2}
	adds	r1, r3, #0
	ldr	r3, [pc, #72]
	stmia	r1!, {r3}
	movs	r3, #180
	adds	r0, r1, #0
	lsls	r3, r3, #8
	str	r0, [sp, #0]
	str	r3, [r1, #0]
	movs	r0, #0
	str	r2, [r7, #12]
	str	r2, [r7, #20]
	movs	r1, #0
	bl 0x02009c94
	ldr	r2, [pc, #32]
	adds	r3, r5, #0
	ands	r3, r2
	lsls	r0, r0, #16
	adds	r3, #1
	adds	r5, #1
	str	r0, [r7, #16]
	strh	r3, [r7, #28]
	adds	r7, #32
	cmp	r5, #63
	bls.n	.L_02001b40
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #24]
	bl 0x02009be4
	add	sp, #8
	b.n	.L_02001b9c
	.4byte 0x0000000f
	.4byte 0x85000205
	.4byte 0x0200a890
	.4byte 0x40000400
	.2byte 0x9941
	.2byte 0x0200
.L_02001b9c:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #116]
	adds	r6, r0, #0
	bl 0x02009cdc
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r3, #8
	adds	r5, r5, r3
	movs	r3, #0
	str	r3, [r5, #0]
	cmp	r6, #0
	beq.n	.L_02001bc8
	cmp	r0, #0
	beq.n	.L_02001bc8
	adds	r3, r0, #0
	adds	r3, #8
	str	r3, [r5, #0]
.L_02001bc8:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.irp EntryTarget, 0x03000528, 0x03000508, 0x080000c1, 0x080000d1, 0x080000d9, 0x080000e1, 0x080000e9, 0x080000f9, 0x08000131, 0x08000149, 0x08000151, 0x080001a1, 0x080001c9, 0x080001d1, 0x080001e9, 0x080003c9, 0x080003d1, 0x080003d9, 0x08020031, 0x08020059, 0x08020091, 0x080200a9, 0x080200c1, 0x08020149, 0x08020151, 0x080201c1, 0x08020211, 0x08020219, 0x08020221, 0x08020279, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80e1, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8119, 0x080c8129, 0x080c8139, 0x080c8149, 0x080c8151, 0x080c8159, 0x080c8171, 0x080c8181, 0x080c8189, 0x080c81a1, 0x080c81d1, 0x080c8201, 0x080c8211, 0x080c8219, 0x080c8231, 0x080c8239, 0x080c8241, 0x080c8279, 0x080c82e1, 0x080c8379, 0x080c8391, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c84e1, 0x080c8581, 0x080c85f9, 0x080c8629, 0x080c8689, 0x080c8691, 0x080c86a9, 0x080c86e9, 0x080c8761, 0x080c8779, 0x08108009, 0x08108019, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.section .rodata,"a",%progbits
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000026
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x00000026
	.4byte 0xfff4fff8
	.4byte 0x00000014
	.4byte 0xfff80001
	.4byte 0x0014fff4
	.4byte 0x00010000
	.4byte 0xfff4fff8
	.4byte 0x00000014
	.4byte 0xfff80001
	.4byte 0x0010fff4
	.4byte 0x00010000
	.4byte 0xfff4fff8
	.4byte 0x00000010
	.4byte 0xfff80001
	.4byte 0x000cfff4
	.4byte 0x00010000
	.4byte 0xfff4fff8
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0x0000ffe0
	.4byte 0x00020002
	.4byte 0xffd00008
	.4byte 0x00020000
	.4byte 0x00100002
	.4byte 0x0000ffc0
	.4byte 0x00020002
	.4byte 0xffb00018
	.4byte 0x00020000
	.4byte 0x00200002
	.4byte 0x0000ffa0
	.4byte 0x00020002
	.4byte 0xff900028
	.4byte 0x00020000
	.4byte 0x00300002
	.4byte 0x0000ff80
	.4byte 0x00020002
	.4byte 0xff700038
	.4byte 0x00020000
	.4byte 0x00400002
	.4byte 0x0000ff60
	.4byte 0x00020002
	.4byte 0x000cfffe
	.4byte 0x000cfffc
	.4byte 0x000cfffa
	.4byte 0x0008fff8
	.4byte 0x0008fff6
	.4byte 0x0008fff4
	.4byte 0x0008fff2
	.4byte 0x0008fff0
	.4byte 0x0004ffed
	.4byte 0x0004ffeb
	.4byte 0x0004ffe8
	.4byte 0x0004ffe5
	.4byte 0x0004ffe2
	.4byte 0x0004ffdf
	.4byte 0x0004ffdc
	.4byte 0x0004ffd8
	.4byte 0x0004ffd4
	.4byte 0x0000ffd0
	.4byte 0x0000ffcc
	.4byte 0x0000ffc8
	.4byte 0x0000ffc4
	.4byte 0x0000ffc0
	.4byte 0x0000ffbc
	.4byte 0x0000ffb8
	.4byte 0x0000ffb4
	.4byte 0x0000ffb0
	.4byte 0x0000ffab
	.4byte 0x0000ffa6
	.4byte 0x0000ffa1
	.4byte 0x0000ff9c
	.4byte 0x0000ff92
	.4byte 0x0000ff88
	.4byte 0x02009e34
	.4byte 0x02009e70
	.4byte 0x02009eac
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00009999
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
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
	.4byte 0x0000009a
	.4byte 0x10124002
	.4byte 0xffffffff
	.4byte 0x1020109b
	.4byte 0xffffffff
	.4byte 0x1030209b
	.4byte 0xffffffff
	.4byte 0x1040109c
	.4byte 0xffffffff
	.4byte 0x1050209c
	.4byte 0xffffffff
	.4byte 0x1060309c
	.4byte 0xffffffff
	.4byte 0x1070409c
	.4byte 0xffffffff
	.4byte 0x1080509c
	.4byte 0xffffffff
	.4byte 0x1090503a
	.4byte 0xffffffff
	.4byte 0x04d4e002
	.4byte 0x0000009c
	.4byte 0x1010409a
	.4byte 0xffffffff
	.4byte 0x1020509a
	.4byte 0xffffffff
	.4byte 0x1030609a
	.4byte 0xffffffff
	.4byte 0x1040709a
	.4byte 0xffffffff
	.4byte 0x1050809a
	.4byte 0xffffffff
	.4byte 0x10602087
	.4byte 0xffffffff
	.4byte 0x0000009b
	.4byte 0x1010209a
	.4byte 0xffffffff
	.4byte 0x1020309a
	.4byte 0xffffffff
	.4byte 0x1030309d
	.4byte 0xffffffff
	.4byte 0x1040409d
	.4byte 0xffffffff
	.4byte 0x1050509d
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x0001a000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x0003e000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0001e000
	.4byte 0xffff0080
	.4byte 0x00000002
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0000c000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00012000
	.4byte 0xffff0071
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0001e000
	.4byte 0xffff0088
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00004000
	.4byte 0xffff008a
	.4byte 0x00000003
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00002000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0001c000
	.4byte 0xffff017f
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00010000
	.4byte 0xffff0088
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00022000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00012000
	.4byte 0xffff0080
	.4byte 0x0200a044
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00010000
	.4byte 0xffff0088
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00014000
	.4byte 0xffff008a
	.4byte 0x00000002
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0001e000
	.4byte 0xffff0070
	.4byte 0x00000003
	.4byte 0x02500000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00012000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00014000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00014000
	.4byte 0xffff0086
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00014000
	.4byte 0xffff0087
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00016000
	.4byte 0xffff0070
	.4byte 0x00000003
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x026c0000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0002c000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0002c000
	.4byte 0x007700f6
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00008000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0xffffffff
	.4byte 0xffffffff
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008c39
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x02008c39
	.4byte 0x00000002
	.4byte 0xffff0006
	.4byte 0x02008c39
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte 0x02008c39
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte 0x02008c39
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x0000c602
	.4byte 0xffff0021
	.4byte 0x020090d1
	.4byte 0x0000c602
	.4byte 0xffff0015
	.4byte 0x020090d1
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002269
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000226a
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0000226b
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0000226c
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000226d
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000226e
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0000226f
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002270
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002271
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002272
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002273
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002274
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002275
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002276
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002277
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002278
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002279
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x0000227a
	.4byte 0x00002115
	.4byte 0x095f0011
	.4byte 0x020087a5
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x020083b1
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x020083cd
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008695
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x020086a9
	.4byte 0x50008905
	.4byte 0xffff0015
	.4byte 0x02008a49
	.4byte 0x50008905
	.4byte 0xffff0016
	.4byte 0x02008a69
	.4byte 0x50008905
	.4byte 0xffff0017
	.4byte 0x02008af9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000227f
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008cb5
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002283
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002284
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002285
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002286
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0000228b
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0000228c
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x02008e99
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x02008de1
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008e41
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002281
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x02008d21
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002287
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002288
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002289
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000228a
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000228d
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000228e
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002290
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002295
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00002296
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x000021ae
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x000021b0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02009465
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02009465
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x02009465
	.4byte 0x00004602
	.4byte 0xffff0028
	.4byte 0x02008d75
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0200833d
	.4byte 0x00008515
	.4byte 0x02000008
	.4byte 0x00000000
	.4byte 0x50008905
	.4byte 0xffff0015
	.4byte 0x02008b89
	.4byte 0x50008905
	.4byte 0xffff0016
	.4byte 0x02008ba9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01000057
	.4byte 0x04061011
	.4byte 0x0e040501
	.4byte 0x0f01000f
	.4byte 0x0406205d
	.4byte 0x0e040502
	.4byte 0x3001030f
	.4byte 0x03750406
	.4byte 0x0f0e0405
	.4byte 0x400f0100
	.4byte 0x05040406
	.4byte 0x0f0ef504
	.4byte 0x031e0100
	.4byte 0x0397049a
	.4byte 0x06300702
	.4byte 0x0109f54b
	.4byte 0x1d03f30a
	.4byte 0x0a400a06
	.4byte 0x100240ad
	.4byte 0x0802445d
	.4byte 0x021f0843
	.4byte 0x2049044e
	.4byte 0x03b92205
	.4byte 0xd700341d
	.4byte 0x106b1612
	.4byte 0x02021261
	.4byte 0x30555e02
	.4byte 0x02245a02
	.4byte 0x561644bf
	.4byte 0x531f0833
	.4byte 0x0f46160f
	.4byte 0x00237919
	.4byte 0x0016f610
	.4byte 0x22621527
	.4byte 0x63010302
	.4byte 0x04302010
	.4byte 0x03ea11f8
	.4byte 0x0b1302ef
	.4byte 0x7b031205
	.4byte 0x30901411
	.4byte 0x12d303b5
	.4byte 0x01001806
	.4byte 0x8f261134
	.4byte 0xdbc31820
	.4byte 0x3c14bb04
	.4byte 0x161f0401
	.4byte 0x712701b5
	.4byte 0x01701d02
	.4byte 0x07012c05
	.2byte 0x0000
