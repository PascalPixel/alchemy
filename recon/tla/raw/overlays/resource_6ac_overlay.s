.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008b65, 0x02008039, 0x02008045, 0x02008081, 0x02008af5, 0x02008041, 0x02008e85
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb43c
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb46c
	.2byte 0x0200
	push	{lr}
	movs	r1, #0
	bl 0x0200b188
	movs	r0, #0
	pop	{pc}
	push	{r5, r6, lr}
	adds	r6, r1, #0
	bl 0x0200b1f0
	adds	r5, r0, #0
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r3, #224
	lsls	r3, r3, #12
	str	r3, [r5, #12]
	movs	r1, #0
	bl 0x0200b188
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl 0x0200b118
	pop	{r5, r6, pc}
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_02000098
	ldr	r0, [pc, #24]
	b.n	.L_020000a4
.L_02000098:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_020000a2
	ldr	r0, [pc, #24]
	b.n	.L_020000a4
.L_020000a2:
	ldr	r0, [pc, #24]
.L_020000a4:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0128
	.2byte 0x0000
	push	{r3, lr}
	lsls	r0, r0, #8
	lsls	r7, r3, #4
	movs	r0, r0
	push	{r3, r4, r7, lr}
	lsls	r0, r0, #8
	push	{r4, r5, r6, r7}
	lsls	r0, r0, #8
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #464]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	movs	r3, #192
	lsls	r3, r3, #18
	mov	fp, r2
	ldr	r2, [r3, #108]
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_020000f2
	b.n	.L_02000292
.L_020000f2:
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #164
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	sl, r3
	cmp	r3, #0
	beq.n	.L_02000108
	b.n	.L_02000292
.L_02000108:
	ldr	r7, [pc, #408]
	movs	r2, #1
	ldr	r3, [r7, #0]
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_02000140
	ldr	r1, [pc, #400]
	movs	r2, #32
	ldr	r3, [pc, #400]
	ldr	r0, [pc, #404]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4964
	movs	r2, #64
	mov	r8, r1
	ldr	r3, [pc, #388]
	ldr	r1, [pc, #396]
	mov	r0, r8
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4641
	ldr	r0, [pc, #392]
	movs	r2, #64
	ldr	r3, [pc, #372]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4651
	str	r1, [r7, #0]
.L_02000140:
	ldr	r0, [r7, #0]
	cmp	r0, #0
	bge.n	.L_02000148
	adds	r0, #7
.L_02000148:
	movs	r1, #6
	asrs	r0, r0, #3
	bl 0x0200b0a8
	movs	r2, #32
	mov	r9, r0
	ldr	r1, [pc, #344]
	ldr	r0, [pc, #356]
	ldr	r3, [pc, #336]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x465a
	cmp	r2, #1
	bne.n	.L_0200019e
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #50
	bl 0x0200b0f8
	cmp	r0, #0
	beq.n	.L_0200019e
	movs	r3, #20
	movs	r7, #0
	mov	r8, r3
.L_02000178:
	ldr	r1, [pc, #312]
	mov	r2, r9
	adds	r0, r2, r7
	mov	sl, r1
	movs	r1, #6
	bl 0x0200b0a8
	lsls	r0, r0, #1
	mov	r1, sl
	adds	r0, #20
	ldrh	r3, [r1, r0]
	ldr	r2, [pc, #300]
	mov	r1, r8
	strh	r3, [r2, r1]
	adds	r7, #1
	movs	r2, #2
	add	r8, r2
	cmp	r7, #5
	ble.n	.L_02000178
.L_0200019e:
	mov	r3, fp
	cmp	r3, #2
	bne.n	.L_020001de
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #51
	bl 0x0200b0f8
	cmp	r0, #0
	beq.n	.L_020001de
	movs	r1, #20
	movs	r7, #0
	mov	r8, r1
.L_020001b8:
	ldr	r2, [pc, #248]
	mov	r3, r9
	adds	r0, r3, r7
	movs	r1, #6
	mov	sl, r2
	bl 0x0200b0a8
	lsls	r0, r0, #1
	mov	r1, sl
	adds	r0, #40
	ldrh	r3, [r1, r0]
	ldr	r2, [pc, #236]
	mov	r1, r8
	strh	r3, [r2, r1]
	adds	r7, #1
	movs	r2, #2
	add	r8, r2
	cmp	r7, #5
	ble.n	.L_020001b8
.L_020001de:
	mov	r3, fp
	cmp	r3, #3
	bne.n	.L_0200021c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #52
	bl 0x0200b0f8
	cmp	r0, #0
	beq.n	.L_0200021c
	movs	r7, #0
	movs	r5, #20
.L_020001f6:
	ldr	r1, [pc, #196]
	ldr	r2, [pc, #184]
	mov	r3, r9
	adds	r0, r3, r7
	mov	sl, r1
	movs	r1, #6
	mov	r8, r2
	bl 0x0200b0a8
	lsls	r0, r0, #1
	adds	r0, #52
	mov	r1, r8
	ldrh	r3, [r1, r0]
	mov	r2, sl
	adds	r7, #1
	strh	r3, [r2, r5]
	adds	r5, #2
	cmp	r7, #5
	ble.n	.L_020001f6
.L_0200021c:
	mov	r3, fp
	cmp	r3, #4
	bne.n	.L_0200025a
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #53
	bl 0x0200b0f8
	cmp	r0, #0
	beq.n	.L_0200025a
	movs	r7, #0
	movs	r6, #20
.L_02000234:
	ldr	r1, [pc, #132]
	ldr	r2, [pc, #124]
	mov	r3, r9
	adds	r0, r3, r7
	mov	sl, r1
	movs	r1, #6
	mov	r8, r2
	bl 0x0200b0a8
	lsls	r0, r0, #1
	adds	r0, #8
	mov	r1, r8
	ldrh	r3, [r1, r0]
	mov	r2, sl
	adds	r7, #1
	strh	r3, [r2, r6]
	adds	r6, #2
	cmp	r7, #5
	ble.n	.L_02000234
.L_0200025a:
	ldr	r1, [pc, #100]
	ldr	r0, [pc, #100]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_02000288
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	ldr	r2, [pc, #68]
	adds	r3, r3, r1
	adds	r3, #4
	stmia	r3!, {r2}
	ldr	r2, [pc, #40]
	stmia	r3!, {r2}
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #8
	str	r2, [r3, #0]
.L_02000288:
	strh	r4, [r0, #0]
	ldr	r2, [pc, #24]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
.L_02000292:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200b6c4
	.4byte 0x050001a0
	.4byte 0x03000730
	.4byte 0x0200b9bc
	.4byte 0x0200b93c
	.4byte 0x05000100
	.4byte 0x0200b97c
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	push	{lr}
	movs	r0, #123
	bl 0x0200b2b8
	ldr	r3, [pc, #16]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200b238
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x049b23c0
	.4byte 0x22d06edb
	.4byte 0x32380112
	.4byte 0x2201189b
	.2byte 0x701a
	.2byte 0x4770
	push	{lr}
	movs	r0, #10
	movs	r1, #1
	bl 0x0200b230
	bl 0x0200b228
	pop	{pc}
	push	{lr}
	bl 0x0200b1e0
	movs	r0, #0
	bl 0x0200b280
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200b248
	movs	r0, #30
	bl 0x0200b258
	movs	r0, #30
	bl 0x0200b1d8
	movs	r0, #190
	bl 0x0200b2b8
	ldr	r3, [pc, #136]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #2
	beq.n	.L_02000366
	cmp	r3, #2
	bgt.n	.L_02000350
	cmp	r3, #1
	beq.n	.L_0200035a
	b.n	.L_02000388
.L_02000350:
	cmp	r3, #3
	beq.n	.L_02000372
	cmp	r3, #4
	beq.n	.L_0200037e
	b.n	.L_02000388
.L_0200035a:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #50
	bl 0x0200b100
	b.n	.L_02000388
.L_02000366:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #51
	bl 0x0200b100
	b.n	.L_02000388
.L_02000372:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #52
	bl 0x0200b100
	b.n	.L_02000388
.L_0200037e:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #53
	bl 0x0200b100
.L_02000388:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r3, #0
	strh	r3, [r2, #0]
	movs	r0, #30
	bl 0x0200b1d8
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200b248
	movs	r0, #30
	bl 0x0200b258
	movs	r0, #30
	bl 0x0200b1d8
	movs	r0, #10
	bl 0x0200b1d8
	bl 0x0200b1e8
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r0, #179
	lsls	r0, r0, #1
	adds	r3, r2, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	sub	sp, #4
	cmp	r3, #0
	beq.n	.L_020003e2
	b.n	.L_0200052e
.L_020003e2:
	movs	r0, #192
	lsls	r0, r0, #4
	adds	r0, #164
	adds	r3, r2, r0
	movs	r5, #0
	ldrsb	r5, [r3, r5]
	cmp	r5, #0
	beq.n	.L_020003f4
	b.n	.L_0200052e
.L_020003f4:
	ldr	r7, [pc, #320]
	movs	r1, #1
	ldr	r3, [r7, #0]
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L_0200042c
	movs	r1, #160
	lsls	r1, r1, #19
	adds	r1, #64
	movs	r2, #32
	ldr	r3, [pc, #304]
	ldr	r0, [pc, #308]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4e4c
	ldr	r1, [pc, #308]
	movs	r2, #64
	ldr	r3, [pc, #292]
	adds	r0, r6, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x484b
	adds	r1, r6, #0
	movs	r2, #64
	ldr	r3, [pc, #276]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x603d
.L_0200042c:
	ldr	r0, [r7, #0]
	cmp	r0, #0
	bge.n	.L_02000434
	adds	r0, #7
.L_02000434:
	asrs	r0, r0, #3
	movs	r1, #6
	bl 0x0200b0a8
	movs	r7, #8
	mov	r8, r0
	movs	r4, #0
.L_02000442:
	mov	r1, r8
	adds	r0, r1, r4
	movs	r1, #6
	str	r4, [sp, #0]
	bl 0x0200b0a8
	ldr	r2, [pc, #244]
	lsls	r1, r0, #1
	adds	r3, r1, #0
	adds	r3, #8
	ldrh	r3, [r2, r3]
	ldr	r6, [pc, #240]
	ldr	r0, [pc, #232]
	strh	r3, [r6, r7]
	adds	r3, r1, #0
	adds	r3, #20
	ldrh	r3, [r0, r3]
	adds	r2, r7, #0
	adds	r2, #12
	strh	r3, [r6, r2]
	adds	r3, r1, #0
	adds	r3, #40
	ldrh	r3, [r0, r3]
	adds	r2, #20
	strh	r3, [r6, r2]
	adds	r3, r1, #0
	adds	r3, #52
	ldr	r4, [sp, #0]
	ldrh	r3, [r0, r3]
	adds	r2, #12
	adds	r4, #1
	strh	r3, [r6, r2]
	adds	r7, #2
	cmp	r4, #5
	ble.n	.L_02000442
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #50
	bl 0x0200b0f8
	cmp	r0, #0
	bne.n	0x020084a4
	adds	r0, r6, #0
	adds	r0, #20
	ldr	r1, [pc, #180]
	ldr	r3, [pc, #156]
	movs	r2, #12
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x20a0
	lsls	r0, r0, #4
	adds	r0, #51
	bl 0x0200b0f8
	cmp	r0, #0
	bne.n	0x020084c0
	adds	r0, r6, #0
	adds	r0, #40
	ldr	r1, [pc, #152]
	ldr	r3, [pc, #128]
	movs	r2, #12
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x20a0
	lsls	r0, r0, #4
	adds	r0, #52
	bl 0x0200b0f8
	cmp	r0, #0
	bne.n	0x020084dc
	adds	r0, r6, #0
	adds	r0, #52
	ldr	r1, [pc, #124]
	ldr	r3, [pc, #100]
	movs	r2, #12
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x20a0
	lsls	r0, r0, #4
	adds	r0, #53
	bl 0x0200b0f8
	cmp	r0, #0
	bne.n	0x020084f8
	adds	r0, r6, #0
	adds	r0, #8
	ldr	r1, [pc, #96]
	ldr	r3, [pc, #72]
	movs	r2, #12
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4916
	ldr	r0, [pc, #92]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_02000524
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r1
	adds	r3, #4
	adds	r2, #1
	stmia	r3!, {r6}
	strh	r2, [r1, #0]
	ldr	r2, [pc, #44]
	stmia	r3!, {r2}
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #16
	str	r2, [r3, #0]
.L_02000524:
	strh	r4, [r0, #0]
	ldr	r2, [pc, #16]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
.L_0200052e:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200b704
	.4byte 0x03000730
	.4byte 0x0200ba5c
	.4byte 0x0200b9dc
	.4byte 0x05000100
	.4byte 0x0200ba1c
	.4byte 0x0200ba70
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, lr}
	ldr	r5, [pc, #340]
	ldr	r3, [pc, #340]
	movs	r1, #0
	ldrsh	r2, [r5, r1]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	sub	sp, #8
	cmp	r2, r3
	bne.n	.L_02000572
	b.n	.L_020006b0
.L_02000572:
	subs	r1, r3, r2
	cmp	r1, #0
	bge.n	.L_0200057e
	movs	r1, #1
	negs	r1, r1
	b.n	.L_02000580
.L_0200057e:
	movs	r1, #1
.L_02000580:
	ldrh	r3, [r5, #0]
	ldr	r0, [pc, #312]
	adds	r1, r3, r1
	strh	r1, [r5, #0]
	ldr	r4, [pc, #308]
	ldrh	r3, [r4, #0]
	adds	r6, r3, #0
	strh	r4, [r4, #0]
	ldrh	r3, [r0, #0]
	cmp	r3, #31
	bgt.n	.L_020005c0
	lsls	r2, r3, #1
	adds	r2, r2, r3
	lsls	r1, r1, #16
	adds	r3, #1
	asrs	r1, r1, #16
	strh	r3, [r0, #0]
	movs	r3, #16
	lsls	r2, r2, #2
	subs	r3, r3, r1
	adds	r2, r2, r0
	lsls	r3, r3, #8
	adds	r2, #4
	orrs	r3, r1
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L_020005c0:
	strh	r6, [r4, #0]
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	bne.n	.L_02000638
	ldr	r5, [pc, #248]
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, #0
	bne.n	.L_020005e8
	movs	r3, #17
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #34
	movs	r1, #8
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b170
.L_020005e8:
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #1
	bne.n	.L_02000602
	movs	r3, #8
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #30
	movs	r1, #8
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b170
.L_02000602:
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, #2
	bne.n	.L_0200061e
	movs	r3, #17
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #34
	movs	r1, #8
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b170
.L_0200061e:
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #3
	bne.n	.L_02000638
	movs	r3, #8
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #30
	movs	r1, #8
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b170
.L_02000638:
	ldr	r3, [pc, #120]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #16
	bne.n	.L_020006b0
	ldr	r5, [pc, #128]
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	bne.n	.L_02000660
	movs	r3, #17
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #34
	movs	r1, #0
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b170
.L_02000660:
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, #1
	bne.n	.L_0200067a
	movs	r3, #8
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #30
	movs	r1, #4
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b170
.L_0200067a:
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #2
	bne.n	.L_02000696
	movs	r3, #17
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #34
	movs	r1, #4
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b170
.L_02000696:
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, #3
	bne.n	.L_020006b0
	movs	r3, #8
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #30
	movs	r1, #0
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b170
.L_020006b0:
	add	sp, #8
	pop	{r5, r6, pc}
	.4byte 0x0200ba7c
	.4byte 0x0200ba7e
	.4byte 0x020038e0
	.4byte 0x04000208
	.2byte 0xba80
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r7, [pc, #60]
	adds	r5, r0, #0
	movs	r3, #0
	ldrsh	r2, [r7, r3]
	sub	sp, #8
	cmp	r5, r2
	bne.n	.L_020006da
	b.n	.L_02000806
.L_020006da:
	adds	r3, r5, #0
	eors	r3, r2
	movs	r6, #1
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_020006ee
	movs	r0, #140
	lsls	r0, r0, #2
	bl 0x0200b2b8
.L_020006ee:
	adds	r3, r5, #0
	ands	r3, r6
	adds	r6, r5, #0
	strh	r5, [r7, #0]
	ldr	r2, [pc, #20]
	subs	r6, #20
	cmp	r3, #0
	beq.n	.L_02000710
	ldr	r3, [pc, #4]
	b.n	.L_02000712
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200ba82
	.2byte 0xba7e
	.2byte 0x0200
.L_02000710:
	ldr	r3, [pc, #32]
.L_02000712:
	strh	r3, [r2, #0]
	ldr	r5, [pc, #32]
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	bne.n	.L_0200073c
	movs	r3, #17
	movs	r2, #67
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #122
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b178
	b.n	.L_0200073c
	.4byte 0x00000010
	.2byte 0xba80
	.2byte 0x0200
.L_0200073c:
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #1
	bne.n	.L_02000758
	movs	r3, #8
	movs	r2, #72
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #122
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b178
.L_02000758:
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #2
	bne.n	.L_02000774
	movs	r3, #17
	movs	r2, #72
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #122
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b178
.L_02000774:
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #3
	bne.n	.L_02000790
	movs	r3, #8
	movs	r2, #67
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #122
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b178
.L_02000790:
	lsrs	r3, r6, #31
	adds	r3, r6, r3
	asrs	r3, r3, #1
	strh	r3, [r5, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_020007b2
	movs	r3, #17
	movs	r2, #67
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #125
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b178
.L_020007b2:
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #1
	bne.n	.L_020007ce
	movs	r3, #8
	movs	r2, #72
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #125
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b178
.L_020007ce:
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #2
	bne.n	.L_020007ea
	movs	r3, #17
	movs	r2, #72
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #125
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b178
.L_020007ea:
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #3
	bne.n	.L_02000806
	movs	r3, #8
	movs	r2, #67
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #125
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b178
.L_02000806:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, fp
	mov	r5, sl
	push	{r5, r6}
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6}
	movs	r3, #17
	mov	r9, r3
	movs	r3, #67
	sub	sp, #8
	mov	fp, r3
	mov	r3, r9
	str	r3, [sp, #0]
	mov	r3, fp
	str	r3, [sp, #4]
	movs	r0, #0
	movs	r1, #125
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b178
	movs	r3, #8
	mov	r8, r3
	movs	r3, #72
	mov	sl, r3
	mov	r3, r8
	str	r3, [sp, #0]
	mov	r3, sl
	str	r3, [sp, #4]
	movs	r0, #0
	movs	r1, #125
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b178
	mov	r3, r9
	str	r3, [sp, #0]
	mov	r3, sl
	str	r3, [sp, #4]
	movs	r0, #0
	movs	r1, #125
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b178
	mov	r3, r8
	str	r3, [sp, #0]
	mov	r3, fp
	str	r3, [sp, #4]
	movs	r1, #125
	movs	r2, #3
	movs	r3, #3
	movs	r0, #0
	bl 0x0200b178
	movs	r0, #1
	bl 0x0200b0b0
	ldr	r6, [pc, #60]
	ldr	r5, [pc, #64]
	movs	r0, #12
	strh	r6, [r5, #0]
	bl 0x0200b0b0
	mov	r3, r8
	strh	r3, [r5, #0]
	movs	r0, #4
	bl 0x0200b0b0
	strh	r6, [r5, #0]
	movs	r0, #4
	bl 0x0200b0b0
	mov	r3, r8
	strh	r3, [r5, #0]
	movs	r0, #4
	bl 0x0200b0b0
	strh	r6, [r5, #0]
	movs	r0, #4
	bl 0x0200b0b0
	ldr	r3, [pc, #16]
	movs	r0, #12
	strh	r3, [r5, #0]
	bl 0x0200b0b0
	b.n	.L_020008cc
	.2byte 0x0000
	.4byte 0x0000000c
	.4byte 0x00000000
	.2byte 0xba7e
	.2byte 0x0200
.L_020008cc:
	mov	r3, r9
	str	r3, [sp, #0]
	mov	r3, fp
	str	r3, [sp, #4]
	movs	r0, #0
	movs	r1, #122
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b178
	mov	r3, r8
	str	r3, [sp, #0]
	mov	r3, sl
	str	r3, [sp, #4]
	movs	r0, #0
	movs	r1, #122
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b178
	mov	r3, r9
	str	r3, [sp, #0]
	mov	r3, sl
	str	r3, [sp, #4]
	movs	r0, #0
	movs	r1, #122
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b178
	mov	r3, r8
	str	r3, [sp, #0]
	mov	r3, fp
	str	r3, [sp, #4]
	movs	r1, #122
	movs	r2, #3
	movs	r3, #3
	movs	r0, #0
	bl 0x0200b178
	movs	r0, #1
	bl 0x0200b0b0
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r3}
	mov	fp, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r5, r0, #0
	movs	r6, #1
	sub	sp, #8
	negs	r6, r6
	bl 0x0200b158
	ldr	r1, [pc, #56]
	cmp	r5, #8
	bne.n	.L_0200094a
	movs	r6, #20
.L_0200094a:
	cmp	r5, #10
	bne.n	.L_02000950
	movs	r6, #22
.L_02000950:
	cmp	r5, #12
	bne.n	.L_02000956
	movs	r6, #24
.L_02000956:
	cmp	r5, #14
	bne.n	.L_0200095c
	movs	r6, #26
.L_0200095c:
	movs	r2, #1
	negs	r2, r2
	cmp	r6, r2
	bne.n	.L_0200098c
	ldr	r2, [pc, #16]
	ldr	r3, [pc, #24]
	strh	r2, [r1, #0]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #20]
	strh	r6, [r3, #0]
	ldr	r3, [pc, #20]
	strh	r6, [r3, #0]
	b.n	.L_02000a2c
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200ba7c
	.4byte 0x0200ba7e
	.4byte 0x0200ba80
	.2byte 0xba82
	.2byte 0x0200
.L_0200098c:
	ldr	r3, [pc, #52]
	ldr	r2, [pc, #60]
	strh	r3, [r1, #0]
	ldr	r3, [pc, #52]
	ldr	r5, [pc, #56]
	strh	r3, [r2, #0]
	adds	r3, r6, #0
	subs	r3, #20
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	ldr	r2, [pc, #48]
	asrs	r3, r3, #1
	strh	r3, [r5, #0]
	strh	r6, [r2, #0]
	cmp	r3, #0
	bne.n	.L_020009d8
	movs	r3, #17
	movs	r2, #67
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #125
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b178
	b.n	.L_020009d8
	.2byte 0x0000
	.4byte 0x0000000f
	.4byte 0x00000010
	.4byte 0x0200ba7e
	.4byte 0x0200ba80
	.2byte 0xba82
	.2byte 0x0200
.L_020009d8:
	movs	r6, #0
	ldrsh	r3, [r5, r6]
	cmp	r3, #1
	bne.n	.L_020009f4
	movs	r3, #8
	movs	r2, #72
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #125
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b178
.L_020009f4:
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #2
	bne.n	.L_02000a10
	movs	r3, #17
	movs	r2, #72
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #125
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b178
.L_02000a10:
	movs	r6, #0
	ldrsh	r3, [r5, r6]
	cmp	r3, #3
	bne.n	.L_02000a2c
	movs	r3, #8
	movs	r2, #67
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #125
	movs	r2, #3
	movs	r3, #3
	bl 0x0200b178
.L_02000a2c:
	ldr	r4, [pc, #124]
	ldr	r0, [pc, #128]
	ldrh	r3, [r0, #0]
	adds	r5, r3, #0
	strh	r0, [r0, #0]
	ldrh	r3, [r4, #0]
	cmp	r3, #31
	bgt.n	.L_02000a68
	lsls	r2, r3, #1
	adds	r2, r2, r3
	adds	r3, #1
	strh	r3, [r4, #0]
	ldr	r3, [pc, #108]
	lsls	r2, r2, #2
	movs	r6, #0
	ldrsh	r1, [r3, r6]
	movs	r3, #16
	subs	r3, r3, r1
	lsls	r3, r3, #8
	adds	r2, r2, r4
	adds	r2, #4
	orrs	r1, r3
	stmia	r2!, {r1}
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L_02000a68:
	strh	r5, [r0, #0]
	ldrh	r3, [r0, #0]
	adds	r1, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r4, #0]
	cmp	r2, #31
	bgt.n	.L_02000a9a
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r4, #0]
	movs	r2, #252
	adds	r3, r3, r4
	lsls	r2, r2, #6
	adds	r3, #4
	adds	r2, #66
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_02000a9a:
	strh	r1, [r0, #0]
	ldr	r0, [pc, #24]
	movs	r1, #144
	lsls	r1, r1, #3
	bl 0x0200b0b8
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x020038e0
	.4byte 0x04000208
	.4byte 0x0200ba7c
	.2byte 0x855d
	.2byte 0x0200
	push	{lr}
	bl 0x02009608
	bl 0x0200b2a8
	ldr	r3, [pc, #36]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #194
	adds	r3, r3, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	movs	r0, #1
	bl 0x0200b238
	movs	r0, #40
	bl 0x0200b1d8
	bl 0x0200966c
	ldr	r0, [pc, #8]
	bl 0x0200b0c0
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x855d
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #64]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_02000b0c
	ldr	r0, [pc, #52]
	b.n	.L_02000b34
.L_02000b0c:
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_02000b16
	ldr	r0, [pc, #52]
	b.n	.L_02000b34
.L_02000b16:
	ldr	r3, [pc, #52]
	cmp	r2, r3
	beq.n	.L_02000b2e
	ldr	r3, [pc, #48]
	cmp	r2, r3
	beq.n	.L_02000b2e
	ldr	r3, [pc, #48]
	cmp	r2, r3
	beq.n	.L_02000b2e
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_02000b32
.L_02000b2e:
	ldr	r0, [pc, #44]
	b.n	.L_02000b34
.L_02000b32:
	ldr	r0, [pc, #44]
.L_02000b34:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000128
	.4byte 0x0200b6c8
	.4byte 0x0000011f
	.4byte 0x0200b708
	.4byte 0x00000120
	.4byte 0x00000121
	.4byte 0x00000122
	.4byte 0x00000123
	.4byte 0x0200b858
	.2byte 0xb6b8
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r2, [pc, #364]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r6, [r3, r1]
	movs	r1, #241
	lsls	r1, r1, #1
	movs	r7, #192
	adds	r3, r2, r1
	lsls	r7, r7, #18
	movs	r2, #0
	ldrsh	r5, [r3, r2]
	ldr	r3, [r7, #108]
	subs	r1, #54
	movs	r2, #128
	adds	r3, r3, r1
	lsls	r2, r2, #1
	movs	r0, #137
	str	r2, [r3, #0]
	lsls	r0, r0, #1
	sub	sp, #8
	bl 0x0200b100
	ldr	r3, [pc, #320]
	ldr	r0, [pc, #324]
	ldr	r1, [pc, #324]
	ldr	r2, [pc, #328]
	bl 0x0200954c
	ldr	r3, [pc, #324]
	cmp	r6, r3
	bne.n	.L_02000bb0
	b.n	.L_02000e76
.L_02000bb0:
	ldr	r3, [pc, #320]
	cmp	r6, r3
	beq.n	.L_02000bb8
	b.n	.L_02000e76
.L_02000bb8:
	movs	r0, #0
	bl 0x0200b278
	ldr	r3, [r7, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #88
	str	r2, [r3, #0]
	subs	r3, r5, #1
	cmp	r3, #1
	bls.n	.L_02000be2
	cmp	r5, #8
	beq.n	.L_02000be2
	cmp	r5, #10
	beq.n	.L_02000be2
	cmp	r5, #12
	beq.n	.L_02000be2
	cmp	r5, #14
	beq.n	.L_02000be2
	b.n	.L_02000e76
.L_02000be2:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #49
	bl 0x0200b0f8
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02000bf4
	b.n	.L_02000e76
.L_02000bf4:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #50
	bl 0x0200b0f8
	cmp	r0, #0
	bne.n	.L_02000c04
	b.n	.L_02000e76
.L_02000c04:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #51
	bl 0x0200b0f8
	cmp	r0, #0
	bne.n	.L_02000c14
	b.n	.L_02000e76
.L_02000c14:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #52
	bl 0x0200b0f8
	cmp	r0, #0
	bne.n	.L_02000c24
	b.n	.L_02000e76
.L_02000c24:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #53
	bl 0x0200b0f8
	cmp	r0, #0
	bne.n	.L_02000c34
	b.n	.L_02000e76
.L_02000c34:
	bl 0x0200b1e0
	movs	r0, #0
	bl 0x0200b280
	bl 0x0200b260
	bl 0x0200b268
	movs	r0, #78
	bl 0x0200b2b8
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #7
	bl 0x0200b218
	movs	r0, #224
	movs	r1, #128
	movs	r2, #216
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r1, r1, #15
	lsls	r0, r0, #16
	bl 0x0200b220
	bl 0x0200b228
	movs	r0, #20
	bl 0x0200b1d8
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200b250
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	adds	r0, #3
	bl 0x0200b248
	movs	r0, #20
	bl 0x0200b258
	movs	r0, #20
	bl 0x0200b0b0
	ldr	r5, [pc, #96]
	ldr	r3, [pc, #60]
	movs	r0, #1
	strh	r3, [r5, #0]
	bl 0x0200b0b0
	movs	r2, #4
	ldr	r0, [pc, #84]
	movs	r1, #0
	bl 0x0200b1c0
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200b248
	movs	r0, #20
	bl 0x0200b258
	movs	r0, #20
	bl 0x0200b0b0
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x0200b218
	movs	r0, #224
	movs	r1, #128
	movs	r2, #216
	b.n	.L_02000d00
	.2byte 0x0000
	.4byte 0x00007fff
	.4byte 0x02000240
	.4byte 0x0200b3f4
	.4byte 0x0200b38c
	.4byte 0x0200b39c
	.4byte 0x0200b3c8
	.4byte 0x00000128
	.4byte 0x0000011f
	.4byte 0x0500021e
	.2byte 0x2d8b
	.2byte 0x0000
.L_02000d00:
	lsls	r1, r1, #13
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r0, r0, #16
	bl 0x0200b220
	bl 0x0200b228
	movs	r0, #40
	bl 0x0200b1d8
	movs	r0, #16
	bl 0x0200b1f0
	adds	r2, r0, #0
	adds	r3, r2, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	movs	r3, #128
	adds	r1, r2, #0
	lsls	r3, r3, #13
	str	r3, [r2, #12]
	str	r3, [r2, #20]
	adds	r1, #98
	movs	r3, #18
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	adds	r1, #4
	strh	r3, [r1, #0]
	str	r6, [r2, #104]
	bl 0x0200ae6c
	movs	r0, #40
	bl 0x0200b0b0
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200b198
	movs	r3, #13
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	mov	r8, r3
	movs	r0, #216
	movs	r1, #216
	movs	r2, #65
	movs	r3, #65
	bl 0x02009324
	mov	r1, r8
	str	r1, [sp, #4]
	movs	r6, #14
	movs	r0, #232
	movs	r1, #216
	movs	r2, #66
	movs	r3, #65
	str	r6, [sp, #0]
	bl 0x02009324
	movs	r0, #232
	movs	r1, #232
	movs	r2, #66
	movs	r3, #66
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	bl 0x02009324
	mov	r2, r8
	str	r2, [sp, #0]
	movs	r5, #12
	movs	r0, #216
	movs	r1, #200
	movs	r2, #65
	movs	r3, #64
	str	r5, [sp, #4]
	bl 0x02009324
	movs	r3, #15
	str	r3, [sp, #0]
	mov	sl, r3
	movs	r0, #248
	movs	r1, #200
	movs	r2, #67
	movs	r3, #64
	str	r5, [sp, #4]
	bl 0x02009324
	mov	r1, sl
	str	r1, [sp, #0]
	movs	r0, #248
	movs	r1, #232
	movs	r2, #67
	movs	r3, #66
	str	r6, [sp, #4]
	bl 0x02009324
	mov	r2, r8
	str	r2, [sp, #0]
	movs	r0, #216
	movs	r1, #232
	movs	r2, #65
	movs	r3, #66
	str	r6, [sp, #4]
	bl 0x02009324
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r0, #200
	movs	r1, #216
	movs	r2, #64
	movs	r3, #65
	str	r5, [sp, #0]
	bl 0x02009324
	movs	r0, #232
	movs	r1, #200
	movs	r2, #66
	movs	r3, #64
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02009324
	mov	r1, sl
	mov	r2, r8
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #248
	movs	r1, #216
	movs	r2, #67
	movs	r3, #65
	bl 0x02009324
	movs	r0, #200
	movs	r1, #232
	movs	r2, #64
	movs	r3, #66
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x02009324
	movs	r3, #64
	movs	r0, #200
	movs	r1, #200
	movs	r2, #64
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02009324
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200b198
	movs	r1, #12
	movs	r2, #4
	movs	r3, #3
	movs	r0, #30
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200b170
	movs	r0, #40
	bl 0x0200b1d8
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #49
	bl 0x0200b100
	movs	r0, #80
	bl 0x0200b2b8
	bl 0x0200b2c0
	bl 0x0200b290
	bl 0x0200b1e8
.L_02000e76:
	movs	r0, #0
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r2, [pc, #560]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r5, [r3, r1]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r2, #0
	ldrsh	r6, [r3, r2]
	ldr	r3, [pc, #540]
	sub	sp, #12
	cmp	r5, r3
	beq.n	.L_02000ebc
	ldr	r3, [pc, #536]
	cmp	r5, r3
	beq.n	.L_02000ebc
	ldr	r3, [pc, #536]
	cmp	r5, r3
	beq.n	.L_02000ebc
	ldr	r3, [pc, #532]
	cmp	r5, r3
	beq.n	.L_02000ebc
	ldr	r3, [pc, #532]
	cmp	r5, r3
	bne.n	.L_02000ee2
.L_02000ebc:
	movs	r0, #1
	bl 0x0200b2b0
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #8
	lsls	r2, r2, #7
	movs	r1, #128
	str	r3, [sp, #4]
	str	r2, [sp, #8]
	movs	r3, #128
	movs	r2, #128
	lsls	r1, r1, #9
	movs	r0, #1
	lsls	r2, r2, #11
	lsls	r3, r3, #10
	str	r1, [sp, #0]
	bl 0x0200b270
.L_02000ee2:
	ldr	r3, [pc, #472]
	cmp	r5, r3
	bne.n	.L_02000fa6
	cmp	r6, #2
	beq.n	.L_02000f22
	cmp	r6, #2
	bgt.n	.L_02000ef6
	cmp	r6, #1
	beq.n	.L_02000f00
	b.n	.L_02000f86
.L_02000ef6:
	cmp	r6, #3
	beq.n	.L_02000f44
	cmp	r6, #4
	beq.n	.L_02000f66
	b.n	.L_02000f86
.L_02000f00:
	movs	r0, #8
	bl 0x0200b1f0
	movs	r1, #1
	bl 0x0200b208
	movs	r1, #4
	movs	r0, #9
	bl 0x02008058
	movs	r0, #4
	bl 0x0200b150
	movs	r0, #5
	bl 0x0200b150
	b.n	.L_02000f86
.L_02000f22:
	movs	r0, #8
	bl 0x0200b1f0
	movs	r1, #3
	bl 0x0200b208
	movs	r1, #5
	movs	r0, #9
	bl 0x02008058
	movs	r0, #6
	bl 0x0200b150
	movs	r0, #7
	bl 0x0200b150
	b.n	.L_02000f86
.L_02000f44:
	movs	r0, #8
	bl 0x0200b1f0
	movs	r1, #0
	bl 0x0200b208
	movs	r1, #2
	movs	r0, #9
	bl 0x02008058
	movs	r0, #0
	bl 0x0200b150
	movs	r0, #1
	bl 0x0200b150
	b.n	.L_02000f86
.L_02000f66:
	movs	r0, #8
	bl 0x0200b1f0
	movs	r1, #4
	bl 0x0200b208
	movs	r1, #3
	movs	r0, #9
	bl 0x02008058
	movs	r0, #2
	bl 0x0200b150
	movs	r0, #3
	bl 0x0200b150
.L_02000f86:
	movs	r0, #10
	bl 0x0200b1f0
.L_02000f8c:
	adds	r2, r0, #0
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	movs	r3, #208
	lsls	r3, r3, #15
	movs	r1, #144
	str	r3, [r0, #16]
	lsls	r1, r1, #3
	ldr	r0, [pc, #304]
	bl 0x0200b0b8
	b.n	.L_02001088
.L_02000fa6:
	ldr	r3, [pc, #300]
	cmp	r5, r3
	bne.n	.L_02001088
	subs	r3, r6, #1
.L_02000fae:
	cmp	r3, #1
	bls.n	.L_02000fc2
	cmp	r6, #8
	beq.n	.L_02000fc2
	cmp	r6, #10
	beq.n	.L_02000fc2
	cmp	r6, #12
	beq.n	.L_02000fc2
	cmp	r6, #14
	bne.n	.L_02001088
.L_02000fc2:
	bl 0x0200a118
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #49
	bl 0x0200b0f8
.L_02000fd0:
	cmp	r0, #0
	beq.n	.L_02000ff8
	movs	r5, #12
	movs	r0, #30
	movs	r1, #12
	movs	r2, #4
	movs	r3, #3
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200b170
	movs	r3, #76
	str	r3, [sp, #0]
	movs	r0, #64
	movs	r1, #64
	movs	r2, #4
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x0200b178
.L_02000ff8:
	adds	r0, r6, #0
	bl 0x02008934
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #212]
	bl 0x0200b0b8
	movs	r1, #4
	movs	r0, #12
	bl 0x02008058
	movs	r1, #5
	movs	r0, #13
	bl 0x02008058
	movs	r1, #2
	movs	r0, #14
	bl 0x02008058
	movs	r0, #15
	movs	r1, #3
	bl 0x02008058
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #50
	bl 0x0200b0f8
	cmp	r0, #0
	bne.n	.L_02001040
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b1f8
.L_02001040:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #51
	bl 0x0200b0f8
	cmp	r0, #0
	bne.n	.L_02001058
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b1f8
.L_02001058:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #52
	bl 0x0200b0f8
	cmp	r0, #0
	bne.n	.L_02001070
.L_02001066:
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b1f8
.L_02001070:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #53
.L_02001076:
	bl 0x0200b0f8
	cmp	r0, #0
	bne.n	.L_02001088
.L_0200107e:
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b1f8
.L_02001088:
	movs	r0, #163
	lsls	r0, r0, #4
	bl 0x0200b0f8
	cmp	r0, #0
	beq.n	.L_020010b0
	ldr	r3, [pc, #32]
	movs	r1, #253
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	ldr	r3, [pc, #56]
	ldr	r2, [pc, #60]
	movs	r1, #160
	subs	r3, r3, r2
	adds	r0, r0, r3
	lsls	r1, r1, #19
	bl 0x0200b1b8
.L_020010b0:
	movs	r0, #0
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000128
	.4byte 0x00000120
	.4byte 0x00000121
	.4byte 0x00000122
	.4byte 0x00000123
	.4byte 0x020080c1
	.4byte 0x0000011f
	.4byte 0x020083c5
	.4byte 0x00000121
	.2byte 0x010e
	.2byte 0x0000
	push	{r5, lr}
	bl 0x0200b1e0
	movs	r0, #0
	bl 0x0200b280
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200b250
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	adds	r0, #3
	bl 0x0200b248
	movs	r0, #20
	bl 0x0200b258
	movs	r0, #20
	bl 0x0200b0b0
	ldr	r5, [pc, #52]
	ldr	r3, [pc, #44]
	movs	r0, #1
	strh	r3, [r5, #0]
	bl 0x0200b0b0
	movs	r2, #0
	ldr	r0, [pc, #40]
	movs	r1, #0
	bl 0x0200b1c0
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200b248
	movs	r0, #20
	bl 0x0200b258
	movs	r0, #20
	bl 0x0200b0b0
	bl 0x0200b1e8
	b.n	.L_02001150
	.4byte 0x00007fff
	.4byte 0x0500021e
	.2byte 0x2d87
	.2byte 0x0000
.L_02001150:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	bl 0x0200b1e0
	movs	r0, #0
	bl 0x0200b280
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200b250
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	adds	r0, #3
	bl 0x0200b248
	movs	r0, #20
	bl 0x0200b258
	movs	r0, #20
	bl 0x0200b0b0
	ldr	r5, [pc, #52]
	ldr	r3, [pc, #44]
	movs	r0, #1
	strh	r3, [r5, #0]
	bl 0x0200b0b0
	movs	r2, #0
	ldr	r0, [pc, #40]
	movs	r1, #0
	bl 0x0200b1c0
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200b248
	movs	r0, #20
	bl 0x0200b258
	movs	r0, #20
	bl 0x0200b0b0
	bl 0x0200b1e8
	b.n	.L_020011c0
	.4byte 0x00007fff
	.4byte 0x0500021e
	.2byte 0x2d88
	.2byte 0x0000
.L_020011c0:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	bl 0x0200b1e0
	movs	r0, #0
	bl 0x0200b280
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200b250
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	adds	r0, #3
	bl 0x0200b248
	movs	r0, #20
	bl 0x0200b258
	movs	r0, #20
	bl 0x0200b0b0
	ldr	r5, [pc, #52]
	ldr	r3, [pc, #44]
	movs	r0, #1
	strh	r3, [r5, #0]
	bl 0x0200b0b0
	movs	r2, #0
	ldr	r0, [pc, #40]
	movs	r1, #0
	bl 0x0200b1c0
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200b248
	movs	r0, #20
	bl 0x0200b258
	movs	r0, #20
	bl 0x0200b0b0
	bl 0x0200b1e8
	b.n	.L_02001230
	.4byte 0x00007fff
	.4byte 0x0500021e
	.2byte 0x2d89
	.2byte 0x0000
.L_02001230:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	bl 0x0200b1e0
	movs	r0, #0
	bl 0x0200b280
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200b250
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	adds	r0, #3
	bl 0x0200b248
	movs	r0, #20
	bl 0x0200b258
	movs	r0, #20
	bl 0x0200b0b0
	ldr	r5, [pc, #52]
	ldr	r3, [pc, #44]
	movs	r0, #1
	strh	r3, [r5, #0]
	bl 0x0200b0b0
	movs	r2, #0
	ldr	r0, [pc, #40]
	movs	r1, #0
	bl 0x0200b1c0
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200b248
	movs	r0, #20
	bl 0x0200b258
	movs	r0, #20
	bl 0x0200b0b0
	bl 0x0200b1e8
	b.n	.L_020012a0
	.4byte 0x00007fff
	.4byte 0x0500021e
	.2byte 0x2d8a
	.2byte 0x0000
.L_020012a0:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r4, r0, #0
	adds	r3, r1, #0
	lsls	r4, r4, #16
	movs	r0, #148
	adds	r6, r2, #0
	lsls	r3, r3, #16
	adds	r0, #255
	adds	r1, r4, #0
	movs	r2, #0
	bl 0x0200b128
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001318
	movs	r1, #0
	bl 0x0200b188
	movs	r1, #3
	adds	r0, r5, #0
	bl 0x0200b118
	ldr	r1, [r5, #80]
	cmp	r6, #0
	beq.n	.L_020012ee
	ldrb	r2, [r1, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	strb	r3, [r1, #9]
	adds	r0, r5, #0
	ldr	r1, [pc, #52]
	bl 0x0200b120
	b.n	.L_02001318
.L_020012ee:
	ldrb	r2, [r1, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
	bl 0x0200b0c8
	lsls	r0, r0, #3
	lsrs	r0, r0, #16
	subs	r0, #4
	movs	r3, #128
	lsls	r0, r0, #16
	lsls	r3, r3, #11
	str	r0, [r5, #36]
	str	r3, [r5, #40]
	ldr	r1, [pc, #12]
	adds	r0, r5, #0
	bl 0x0200b120
.L_02001318:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0200b87c
	.2byte 0xb900
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r8, r0
	adds	r7, r1, #0
	movs	r0, #144
	sub	sp, #8
	mov	sl, r2
	mov	r9, r3
	bl 0x0200b2b8
	adds	r1, r7, #0
	adds	r1, #12
	mov	r0, r8
	movs	r2, #1
	bl 0x020092a4
	movs	r6, #0
.L_0200134c:
	bl 0x0200b0c8
	lsls	r5, r0, #1
	adds	r5, r5, r0
	bl 0x0200b0c8
	lsls	r1, r0, #1
	adds	r1, r1, r0
	lsls	r5, r5, #2
	lsrs	r5, r5, #16
	lsls	r1, r1, #2
	add	r5, r8
	lsrs	r1, r1, #16
	subs	r5, #6
	adds	r1, r7, r1
	subs	r1, #6
	adds	r0, r5, #0
	movs	r2, #0
	adds	r6, #1
	bl 0x020092a4
	cmp	r6, #2
	bls.n	.L_0200134c
	movs	r0, #4
	bl 0x0200b0b0
	ldr	r2, [sp, #36]
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	adds	r2, #64
	ldr	r3, [sp, #40]
	mov	r0, sl
	mov	r1, r9
	bl 0x0200b148
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	movs	r0, #222
	bl 0x0200b1c8
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_0200145c
	movs	r0, #147
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200b0f8
	cmp	r0, #0
	beq.n	.L_020013c0
	b.n	.L_02001548
.L_020013c0:
	bl 0x0200b1e0
	movs	r0, #0
	bl 0x0200b280
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #7
	bl 0x0200b218
	movs	r0, #224
	movs	r1, #128
	movs	r2, #216
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r1, r1, #15
	lsls	r0, r0, #16
	bl 0x0200b220
	bl 0x0200b228
	movs	r0, #20
	bl 0x0200b1d8
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200b250
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	adds	r0, #3
	bl 0x0200b248
	movs	r0, #20
	bl 0x0200b258
	movs	r0, #20
	bl 0x0200b0b0
	ldr	r5, [pc, #60]
	ldr	r3, [pc, #56]
	movs	r0, #1
	strh	r3, [r5, #0]
	bl 0x0200b0b0
	movs	r2, #4
	ldr	r0, [pc, #52]
	movs	r1, #0
	bl 0x0200b1c0
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200b248
	movs	r0, #20
	bl 0x0200b258
	movs	r0, #20
	bl 0x0200b0b0
	movs	r0, #147
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200b100
	bl 0x0200b1e8
	b.n	.L_02001548
	.4byte 0x00007fff
	.4byte 0x0500021e
	.2byte 0x2d84
	.2byte 0x0000
.L_0200145c:
	movs	r0, #163
	lsls	r0, r0, #4
	bl 0x0200b0f8
	cmp	r0, #0
	beq.n	.L_02001548
	bl 0x0200b1e0
	movs	r0, #0
	bl 0x0200b280
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #7
	bl 0x0200b218
	movs	r0, #224
	movs	r1, #128
	movs	r2, #216
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r1, r1, #15
	lsls	r0, r0, #16
	bl 0x0200b220
	bl 0x0200b228
	movs	r0, #20
	bl 0x0200b1d8
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200b250
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	adds	r0, #3
	bl 0x0200b248
	movs	r0, #20
	bl 0x0200b258
	movs	r0, #20
	bl 0x0200b0b0
	ldr	r5, [pc, #64]
	ldr	r3, [pc, #60]
	movs	r0, #1
	strh	r3, [r5, #0]
	bl 0x0200b0b0
	ldr	r5, [pc, #56]
	movs	r1, #0
	adds	r0, r5, #0
	movs	r2, #4
	bl 0x0200b1c0
	adds	r5, #1
	movs	r0, #10
	bl 0x0200b0b0
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #3
	bl 0x0200b1c0
	movs	r0, #224
	movs	r1, #176
	movs	r2, #216
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r1, r1, #15
	lsls	r0, r0, #16
	bl 0x0200b220
	b.n	.L_02001508
	.2byte 0x0000
	.4byte 0x00007fff
	.4byte 0x0500021e
	.2byte 0x2d85
	.2byte 0x0000
.L_02001508:
	bl 0x0200b228
	movs	r0, #20
	bl 0x0200b1d8
	movs	r0, #140
	lsls	r0, r0, #2
	bl 0x0200b2b8
	bl 0x0200880c
	movs	r0, #20
	bl 0x0200b1d8
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200b248
	movs	r0, #20
	bl 0x0200b258
	movs	r0, #40
	bl 0x0200b0b0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #46
	bl 0x0200b100
	bl 0x0200b1e8
.L_02001548:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r4, r1, #0
	movs	r1, #192
	lsls	r1, r1, #18
	adds	r1, #128
	ldr	r6, [r1, #0]
	ldr	r1, [pc, #148]
	adds	r5, r0, #0
	str	r5, [r1, #0]
	ldr	r1, [pc, #148]
	str	r4, [r1, #0]
	ldr	r1, [pc, #148]
	str	r2, [r1, #0]
	ldr	r2, [pc, #148]
	str	r3, [r2, #0]
	movs	r2, #255
	ldrh	r3, [r5, #0]
	b.n	.L_02001596
.L_02001570:
	ldrh	r0, [r4, #0]
	adds	r4, #2
	ldrh	r2, [r4, #0]
	adds	r4, #2
	ldrh	r1, [r5, #0]
	ldrh	r3, [r4, #0]
	adds	r5, #2
	adds	r4, #2
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	movs	r2, #160
	lsls	r2, r2, #19
	lsls	r1, r1, #1
	orrs	r3, r0
	adds	r1, r1, r2
	strh	r3, [r1, #0]
	ldrh	r3, [r5, #0]
	movs	r2, #255
.L_02001596:
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_020015a4
	ldrh	r3, [r4, #0]
	cmp	r3, r2
	bne.n	.L_02001570
.L_020015a4:
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	movs	r0, #160
	lsls	r2, r2, #24
	adds	r3, #212
	lsls	r0, r0, #19
	adds	r1, r6, #0
	adds	r2, #112
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r1, r6, r2
	movs	r2, #132
	lsls	r2, r2, #24
	ldr	r0, [pc, #56]
	adds	r2, #112
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200b248
	ldr	r3, [pc, #44]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #194
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #2
	bne.n	.L_020015ee
	bl 0x0200983c
.L_020015ee:
	pop	{r5, r6, pc}
	.4byte 0x0200b928
	.4byte 0x0200b92c
	.4byte 0x0200b930
	.4byte 0x0200b91c
	.4byte 0x05000200
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	ldr	r2, [pc, #64]
	ldr	r3, [pc, #44]
	ldr	r5, [pc, #64]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #64]
	ldr	r0, [r3, #0]
	bl 0x020096e8
	ldr	r2, [pc, #60]
	ldr	r3, [pc, #32]
	strh	r0, [r5, #0]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #56]
	ldr	r3, [pc, #28]
	movs	r1, #144
	strh	r3, [r2, #0]
	ldr	r2, [pc, #52]
	ldr	r3, [pc, #24]
	lsls	r1, r1, #3
	strh	r3, [r2, #0]
	ldr	r0, [pc, #48]
	bl 0x0200b0b8
	b.n	.L_02001668
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0000000f
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0x0200b938
	.4byte 0x0200b934
	.4byte 0x0200b928
	.4byte 0x0200b924
	.4byte 0x0200b920
	.4byte 0x0200b918
	.2byte 0x970d
	.2byte 0x0200
.L_02001668:
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	subs	r2, #172
	str	r2, [r3, #0]
	ldr	r0, [pc, #8]
	bl 0x0200b0c0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x970d
	.2byte 0x0200
	push	{r5, lr}
	ldr	r2, [pc, #56]
	ldr	r3, [pc, #40]
	ldr	r5, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	ldr	r0, [r3, #0]
	bl 0x020096e8
	ldr	r2, [pc, #32]
	ldr	r3, [pc, #48]
	strh	r0, [r5, #0]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #48]
	movs	r1, #144
	strh	r2, [r3, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #20]
	lsls	r1, r1, #3
	strh	r3, [r2, #0]
	ldr	r0, [pc, #40]
	bl 0x0200b0b8
	b.n	.L_020016e4
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x0200b938
	.4byte 0x0200b934
	.4byte 0x0200b928
	.4byte 0x0200b924
	.4byte 0x0200b920
	.4byte 0x0200b918
	.2byte 0x970d
	.2byte 0x0200
.L_020016e4:
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	ldr	r1, [pc, #20]
	ldrh	r3, [r0, #0]
	movs	r2, #0
	cmp	r3, r1
	beq.n	.L_02001704
.L_020016f4:
	adds	r0, #2
	ldrh	r3, [r0, #0]
	adds	r2, #1
	cmp	r3, r1
	bne.n	.L_020016f4
	b.n	.L_02001704
	.2byte 0xffff
	.2byte 0x0000
.L_02001704:
	subs	r2, #1
	adds	r0, r2, #0
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	ldr	r1, [pc, #172]
	movs	r4, #0
	ldrh	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_02001746
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #255
	adds	r3, r2, r0
	strh	r3, [r1, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_02001746
	ldr	r0, [pc, #148]
	movs	r4, #1
	ldrh	r2, [r0, #0]
	strh	r2, [r1, #0]
	movs	r1, #128
	lsls	r3, r2, #16
	lsls	r1, r1, #10
	cmp	r3, r1
	bls.n	.L_02001746
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r3, r2, r1
	strh	r3, [r0, #0]
.L_02001746:
	cmp	r4, #0
	bne.n	.L_0200174c
	b.n	.L_02001838
.L_0200174c:
	ldr	r3, [pc, #116]
	ldr	r6, [pc, #120]
	ldr	r1, [r3, #0]
	ldrh	r3, [r6, #0]
	movs	r5, #0
	cmp	r5, r3
	bcs.n	.L_0200179a
	ldr	r3, [pc, #112]
	ldr	r2, [pc, #112]
	ldr	r7, [r3, #0]
	mov	lr, r2
	mov	ip, r6
.L_02001764:
	mov	r3, lr
	ldrh	r2, [r3, #0]
	ldrh	r3, [r6, #0]
	movs	r0, #160
	muls	r3, r2
	adds	r3, r3, r5
	lsls	r3, r3, #1
	ldrh	r3, [r3, r7]
	lsls	r0, r0, #19
	lsls	r3, r3, #1
	adds	r4, r3, r0
	ldrh	r0, [r1, #0]
	adds	r1, #2
	ldrh	r2, [r1, #0]
	adds	r1, #2
	ldrh	r3, [r1, #0]
	adds	r1, #2
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r0
	strh	r3, [r4, #0]
	adds	r5, #1
	mov	r2, ip
	ldrh	r3, [r2, #0]
	cmp	r5, r3
	bcc.n	.L_02001764
.L_0200179a:
	ldr	r3, [pc, #44]
	movs	r0, #160
	ldrh	r1, [r3, #0]
	ldr	r3, [pc, #48]
	lsls	r2, r1, #1
	ldr	r3, [r3, #0]
	lsls	r0, r0, #19
	ldrh	r3, [r2, r3]
	adds	r2, r2, r1
	lsls	r3, r3, #1
	adds	r4, r3, r0
	ldr	r3, [pc, #36]
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_020017e0
	ldr	r3, [pc, #32]
	b.n	.L_020017e2
	.4byte 0x0200b920
	.4byte 0x0200b924
	.4byte 0x0200b930
	.4byte 0x0200b934
	.4byte 0x0200b91c
	.4byte 0x0200b938
	.4byte 0x0200b928
	.4byte 0x0200b918
	.2byte 0xb92c
	.2byte 0x0200
.L_020017e0:
	ldr	r3, [pc, #68]
.L_020017e2:
	lsls	r2, r2, #1
	ldr	r3, [r3, #0]
	adds	r1, r3, r2
	ldrh	r0, [r1, #0]
	adds	r1, #2
	ldrh	r2, [r1, #0]
	ldrh	r3, [r1, #2]
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r0
	strh	r3, [r4, #0]
	ldr	r1, [pc, #48]
	ldr	r2, [pc, #32]
	ldrh	r3, [r1, #0]
	eors	r3, r2
	strh	r3, [r1, #0]
	ldr	r1, [pc, #40]
	ldr	r2, [pc, #44]
	ldrh	r3, [r1, #0]
	adds	r3, #1
	strh	r3, [r1, #0]
	lsls	r3, r3, #16
	ldrh	r2, [r2, #0]
	lsrs	r3, r3, #16
	cmp	r3, r2
	bne.n	.L_02001838
	ldr	r3, [pc, #8]
	strh	r3, [r1, #0]
	b.n	.L_02001838
	.2byte 0x0000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0200b930
	.4byte 0x0200b918
	.4byte 0x0200b938
	.2byte 0xb934
	.2byte 0x0200
.L_02001838:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	movs	r6, #192
	lsls	r6, r6, #18
	ldr	r5, [r6, #108]
	movs	r1, #214
	lsls	r1, r1, #1
	mov	r8, r1
	add	r5, r8
	ldr	r2, [r5, #0]
	ldr	r0, [pc, #100]
	movs	r1, #1
	mov	sl, r2
	bl 0x0200b248
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #2
	bl 0x0200b248
	ldr	r2, [r6, #108]
	movs	r3, #128
	lsls	r3, r3, #1
	mov	r1, r8
	str	r3, [r2, r1]
	bl 0x0200b260
	bl 0x0200b268
	bl 0x0200968c
	bl 0x0200b2a0
	movs	r0, #40
	bl 0x0200b0b0
	bl 0x0200966c
	movs	r0, #128
	movs	r1, #1
	lsls	r0, r0, #9
	bl 0x0200b248
	movs	r0, #16
	bl 0x0200b258
	movs	r0, #16
	bl 0x0200b0b0
	ldr	r3, [pc, #28]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #194
	adds	r3, r3, r2
	movs	r2, #0
	strb	r2, [r3, #0]
	mov	r3, sl
	str	r3, [r5, #0]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.4byte 0x00202108
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r4, [r0, #8]
	ldr	r3, [r1, #8]
	subs	r2, r4, r3
	cmp	r2, #0
	blt.n	.L_020018da
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_020018e4
	b.n	.L_02001924
.L_020018da:
	movs	r2, #128
	subs	r3, r3, r4
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_02001924
.L_020018e4:
	ldr	r4, [r0, #12]
	ldr	r3, [r1, #12]
	subs	r2, r4, r3
	cmp	r2, #0
	blt.n	.L_020018f8
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02001902
	b.n	.L_02001924
.L_020018f8:
	movs	r2, #128
	subs	r3, r3, r4
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_02001924
.L_02001902:
	ldr	r0, [r0, #16]
	ldr	r1, [r1, #16]
	subs	r2, r0, r1
	cmp	r2, #0
	blt.n	.L_02001916
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02001920
	b.n	.L_02001924
.L_02001916:
	movs	r2, #128
	subs	r3, r1, r0
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_02001924
.L_02001920:
	movs	r0, #1
	b.n	.L_02001926
.L_02001924:
	movs	r0, #0
.L_02001926:
	pop	{pc}
	push	{lr}
	ldr	r4, [r0, #8]
	ldr	r3, [r1, #8]
	subs	r2, r4, r3
.L_02001930:
	cmp	r2, #0
	blt.n	.L_0200193e
	movs	r3, #192
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02001948
	b.n	.L_0200197a
.L_0200193e:
	movs	r2, #192
	subs	r3, r3, r4
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_0200197a
.L_02001948:
	ldr	r2, [r1, #12]
	ldr	r3, [r0, #12]
	subs	r3, r3, r2
	ldr	r2, [pc, #48]
	adds	r3, r3, r2
	ldr	r2, [pc, #48]
	cmp	r3, r2
	bhi.n	.L_0200197a
	ldr	r0, [r0, #16]
	ldr	r1, [r1, #16]
	subs	r2, r0, r1
	cmp	r2, #0
	blt.n	.L_0200196c
	movs	r3, #192
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02001976
	b.n	.L_0200197a
.L_0200196c:
	movs	r2, #192
	subs	r3, r1, r0
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_0200197a
.L_02001976:
	movs	r0, #1
	b.n	.L_0200197c
.L_0200197a:
	movs	r0, #0
.L_0200197c:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0007ffff
	.2byte 0xfffe
	.2byte 0x001f
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #336]
	ldr	r2, [pc, #336]
	mov	sl, r3
	movs	r3, #133
	lsls	r3, r3, #2
	add	r3, sl
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	sub	sp, #4
	mov	r8, r2
	bl 0x0200b1f0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	ldr	r5, [r6, #68]
	mov	r9, r3
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #72]
	adds	r3, r3, r5
	str	r3, [r6, #8]
	ldr	r3, [r6, #12]
	ldr	r7, [r6, #76]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r4, r0, #0
	adds	r3, r3, r7
	adds	r0, r5, #0
	movs	r1, #18
	str	r3, [r6, #16]
	str	r4, [sp, #0]
	bl 0x0200b0a0
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	ldr	r4, [sp, #0]
	cmp	r7, #0
	bge.n	.L_020019e4
	adds	r3, #15
.L_020019e4:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #76]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #24]
	ldr	r1, [r6, #80]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [r6, #52]
	ldr	r3, [r6, #28]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r1, #18]
	ldrh	r2, [r2, #0]
	adds	r3, r3, r2
	strh	r3, [r1, #18]
	ldr	r2, [r4, #12]
	ldr	r3, [r4, #20]
	cmp	r2, r3
	bne.n	.L_02001a76
	adds	r0, r6, #0
	adds	r1, r4, #0
	bl 0x020098c4
	cmp	r0, #0
	beq.n	.L_02001a76
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	add	r3, sl
	ldrb	r2, [r3, #0]
	cmp	r2, #0
	bne.n	.L_02001a76
	ldr	r1, [r6, #76]
	cmp	r1, #0
	beq.n	.L_02001a4a
	mov	r3, r8
	adds	r3, #104
	strh	r2, [r3, #0]
	mov	r2, r8
	adds	r2, #106
	cmp	r1, #0
	ble.n	.L_02001a42
	movs	r3, #1
	b.n	.L_02001a48
.L_02001a42:
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
.L_02001a48:
	strh	r3, [r2, #0]
.L_02001a4a:
	adds	r2, r6, #0
	adds	r2, #35
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r1, #0
	ldr	r2, [r6, #76]
	ldr	r3, [r6, #16]
	str	r1, [r6, #76]
	subs	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #72]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r6, #72]
	mov	r2, r8
	movs	r3, #1
	strh	r3, [r2, #4]
	ldrh	r2, [r2, #10]
	movs	r3, #170
	lsls	r3, r3, #1
	add	r3, r9
	strh	r2, [r3, #0]
.L_02001a76:
	movs	r3, #84
	mov	r2, r8
	ldrh	r0, [r2, r3]
	movs	r7, #0
	cmp	r0, #0
	beq.n	.L_02001ad6
	mov	r5, r8
	adds	r5, #84
.L_02001a86:
	bl 0x0200b1f0
	adds	r4, r0, #0
	ldr	r2, [r4, #12]
	ldr	r3, [r4, #20]
	cmp	r2, r3
	bne.n	.L_02001ac8
	adds	r0, r6, #0
	adds	r1, r4, #0
	bl 0x02009928
	cmp	r0, #0
	beq.n	.L_02001ac8
	ldrh	r1, [r5, #2]
	cmp	r1, #0
	bne.n	.L_02001ac2
	adds	r2, r6, #0
	adds	r2, #35
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r2, [r6, #76]
	ldr	r3, [r6, #16]
	str	r1, [r6, #76]
	subs	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #72]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r6, #72]
	b.n	.L_02001ac8
.L_02001ac2:
	movs	r3, #1
	mov	r2, r8
	strh	r3, [r2, #6]
.L_02001ac8:
	adds	r7, #1
	adds	r5, #4
	cmp	r7, #3
	bgt.n	.L_02001ad6
	ldrh	r0, [r5, #0]
	cmp	r0, #0
	bne.n	.L_02001a86
.L_02001ad6:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r3, r5, #0
	movs	r6, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	adds	r3, #4
	strb	r6, [r3, #0]
	movs	r1, #0
	bl 0x0200b188
	ldr	r1, [pc, #20]
	adds	r0, r5, #0
	bl 0x0200b120
	adds	r3, r5, #0
	adds	r3, #100
	strh	r6, [r3, #0]
	str	r6, [r5, #48]
	str	r6, [r5, #52]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xb2d4
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [r0, #8]
	adds	r6, r1, #0
	adds	r7, r2, #0
	ldr	r1, [r0, #0]
	ldr	r2, [r0, #4]
	movs	r0, #183
	lsls	r0, r0, #1
	bl 0x0200b128
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001b5c
	movs	r3, #0
	str	r3, [r5, #68]
	str	r3, [r5, #72]
	str	r6, [r5, #76]
	movs	r1, #2
	bl 0x02009aec
	ldr	r3, [pc, #24]
	str	r3, [r5, #108]
	cmp	r7, #0
	beq.n	.L_02001b54
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #2
	strb	r3, [r2, #0]
.L_02001b54:
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200b118
.L_02001b5c:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x9989
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #332]
	ldr	r2, [pc, #332]
	mov	sl, r3
	movs	r3, #133
	lsls	r3, r3, #2
	add	r3, sl
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	sub	sp, #4
	mov	r8, r2
	bl 0x0200b1f0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	ldr	r5, [r6, #68]
	mov	r9, r3
	ldr	r3, [r6, #16]
	ldr	r2, [r6, #72]
	adds	r3, r3, r5
	str	r3, [r6, #16]
	ldr	r3, [r6, #12]
	ldr	r7, [r6, #76]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #8]
	adds	r4, r0, #0
	adds	r3, r3, r7
	adds	r0, r5, #0
	movs	r1, #18
	str	r3, [r6, #8]
	str	r4, [sp, #0]
	bl 0x0200b0a0
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	ldr	r4, [sp, #0]
	cmp	r7, #0
	bge.n	.L_02001bc0
	adds	r3, #15
.L_02001bc0:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #76]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #24]
	ldr	r1, [r6, #80]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [r6, #52]
	ldr	r3, [r6, #28]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r1, #18]
	ldrh	r2, [r2, #0]
	adds	r3, r3, r2
	strh	r3, [r1, #18]
	ldr	r2, [r4, #12]
	ldr	r3, [r4, #20]
	cmp	r2, r3
	bne.n	.L_02001c50
	adds	r0, r6, #0
	adds	r1, r4, #0
	bl 0x020098c4
	cmp	r0, #0
	beq.n	.L_02001c50
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	add	r3, sl
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_02001c50
	ldr	r2, [r6, #76]
	cmp	r2, #0
	beq.n	.L_02001c24
	mov	r1, r8
	adds	r1, #106
	strh	r3, [r1, #0]
	subs	r1, #2
	cmp	r2, #0
	ble.n	.L_02001c1c
	movs	r3, #1
	b.n	.L_02001c22
.L_02001c1c:
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
.L_02001c22:
	strh	r3, [r1, #0]
.L_02001c24:
	adds	r2, r6, #0
	adds	r2, #35
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r1, #0
	ldr	r3, [r6, #16]
	ldr	r2, [r6, #68]
	str	r1, [r6, #76]
	subs	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #72]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r6, #72]
	mov	r2, r8
	movs	r3, #1
	strh	r3, [r2, #4]
	ldrh	r2, [r2, #10]
	movs	r3, #170
	lsls	r3, r3, #1
	add	r3, r9
	strh	r2, [r3, #0]
.L_02001c50:
	movs	r3, #84
	mov	r2, r8
	ldrh	r0, [r2, r3]
	movs	r7, #0
	cmp	r0, #0
	beq.n	.L_02001cb0
	mov	r5, r8
	adds	r5, #84
.L_02001c60:
	bl 0x0200b1f0
	adds	r4, r0, #0
	ldr	r2, [r4, #12]
	ldr	r3, [r4, #20]
	cmp	r2, r3
	bne.n	.L_02001ca2
	adds	r0, r6, #0
	adds	r1, r4, #0
	bl 0x02009928
	cmp	r0, #0
	beq.n	.L_02001ca2
	ldrh	r1, [r5, #2]
	cmp	r1, #0
	bne.n	.L_02001c9c
	adds	r2, r6, #0
	adds	r2, #35
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r2, [r6, #76]
	ldr	r3, [r6, #16]
	str	r1, [r6, #76]
	subs	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #72]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r6, #72]
	b.n	.L_02001ca2
.L_02001c9c:
	movs	r3, #1
	mov	r2, r8
	strh	r3, [r2, #6]
.L_02001ca2:
	adds	r7, #1
	adds	r5, #4
	cmp	r7, #3
	bgt.n	.L_02001cb0
	ldrh	r0, [r5, #0]
	cmp	r0, #0
	bne.n	.L_02001c60
.L_02001cb0:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r3, r5, #0
	movs	r6, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	adds	r3, #4
	strb	r6, [r3, #0]
	movs	r3, #3
	ldr	r0, [r5, #80]
	ands	r1, r3
	ldrb	r2, [r0, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	lsls	r1, r1, #2
	orrs	r3, r1
	strb	r3, [r0, #9]
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x0200b188
	ldr	r1, [pc, #16]
	adds	r0, r5, #0
	bl 0x0200b120
	adds	r3, r5, #0
	adds	r3, #100
	strh	r6, [r3, #0]
	str	r6, [r5, #48]
	str	r6, [r5, #52]
	pop	{r5, r6, pc}
	.2byte 0xb2d4
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [r0, #8]
	adds	r6, r1, #0
	adds	r7, r2, #0
	ldr	r1, [r0, #0]
	ldr	r2, [r0, #4]
	movs	r0, #183
	lsls	r0, r0, #1
	bl 0x0200b128
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001d4a
	movs	r3, #0
	str	r3, [r5, #68]
	str	r3, [r5, #72]
.L_02001d28:
	negs	r3, r6
	str	r3, [r5, #76]
	movs	r1, #3
	bl 0x02009cc4
	ldr	r3, [pc, #24]
	str	r3, [r5, #108]
	cmp	r7, #0
	beq.n	.L_02001d42
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #2
	strb	r3, [r2, #0]
.L_02001d42:
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200b118
.L_02001d4a:
	pop	{r5, r6, r7, pc}
	.2byte 0x9989
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [r0, #8]
	adds	r6, r1, #0
	adds	r7, r2, #0
	ldr	r1, [r0, #0]
	ldr	r2, [r0, #4]
	movs	r0, #183
	lsls	r0, r0, #1
	bl 0x0200b128
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001d90
	movs	r3, #0
	str	r3, [r5, #68]
	str	r3, [r5, #72]
	str	r6, [r5, #76]
	movs	r1, #3
	bl 0x02009cc4
	ldr	r3, [pc, #24]
	str	r3, [r5, #108]
	cmp	r7, #0
	beq.n	.L_02001d88
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #2
	strb	r3, [r2, #0]
.L_02001d88:
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200b118
.L_02001d90:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x9b65
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [r0, #8]
	adds	r6, r1, #0
	adds	r7, r2, #0
	ldr	r1, [r0, #0]
	ldr	r2, [r0, #4]
	movs	r0, #183
	lsls	r0, r0, #1
	bl 0x0200b128
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001dda
	movs	r3, #0
	str	r3, [r5, #68]
	str	r3, [r5, #72]
	negs	r3, r6
	str	r3, [r5, #76]
	movs	r1, #3
	bl 0x02009cc4
	ldr	r3, [pc, #24]
	str	r3, [r5, #108]
	cmp	r7, #0
	beq.n	.L_02001dd2
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #2
	strb	r3, [r2, #0]
.L_02001dd2:
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200b118
.L_02001dda:
	pop	{r5, r6, r7, pc}
	.2byte 0x9b65
	.2byte 0x0200
	push	{lr}
	ldr	r4, [r0, #8]
	ldr	r3, [r1, #8]
	subs	r2, r4, r3
	cmp	r2, #0
	blt.n	.L_02001df6
	movs	r3, #192
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02001e00
	b.n	.L_02001e30
.L_02001df6:
	movs	r2, #192
	subs	r3, r3, r4
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_02001e30
.L_02001e00:
	ldr	r2, [r1, #12]
	ldr	r3, [r0, #12]
	subs	r3, r3, r2
	ldr	r2, [pc, #44]
	subs	r3, #1
	cmp	r3, r2
	bhi.n	.L_02001e30
	ldr	r0, [r0, #16]
	ldr	r1, [r1, #16]
	subs	r2, r0, r1
	cmp	r2, #0
	blt.n	.L_02001e22
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r2, r3
	blt.n	.L_02001e2c
	b.n	.L_02001e30
.L_02001e22:
	movs	r2, #128
	subs	r3, r1, r0
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_02001e30
.L_02001e2c:
	movs	r0, #1
	b.n	.L_02001e32
.L_02001e30:
	movs	r0, #0
.L_02001e32:
	pop	{pc}
	.2byte 0xfffe
	.2byte 0x000f
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #144]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200b1f0
	ldr	r5, [r6, #68]
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #72]
	adds	r3, r3, r5
	str	r3, [r6, #8]
	ldr	r3, [r6, #12]
	ldr	r7, [r6, #76]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	mov	r8, r0
	adds	r3, r3, r7
	adds	r0, r5, #0
	movs	r1, #18
	str	r3, [r6, #16]
	bl 0x0200b0a0
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02001e7c
	adds	r3, #15
.L_02001e7c:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #76]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #24]
	ldr	r1, [r6, #80]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [r6, #52]
	ldr	r3, [r6, #28]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r1, #18]
	ldrh	r2, [r2, #0]
	adds	r3, r3, r2
	strh	r3, [r1, #18]
	mov	r3, r8
	ldr	r2, [r3, #12]
	ldr	r3, [r3, #20]
	cmp	r2, r3
	bne.n	.L_02001eca
	adds	r0, r6, #0
	mov	r1, r8
	bl 0x02009de0
	cmp	r0, #0
	beq.n	.L_02001eca
	ldr	r2, [r6, #76]
	ldr	r3, [r6, #16]
	subs	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #72]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r6, #72]
	movs	r3, #0
	str	r3, [r6, #76]
.L_02001eca:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r3, r5, #0
	movs	r6, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	adds	r3, #4
	strb	r6, [r3, #0]
	movs	r1, #0
	bl 0x0200b188
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200b118
	adds	r0, r5, #0
	ldr	r1, [pc, #24]
	bl 0x0200b120
	adds	r0, r5, #0
	movs	r1, #10
	bl 0x0200b208
	adds	r3, r5, #0
	adds	r3, #100
	strh	r6, [r3, #0]
	str	r6, [r5, #48]
	str	r6, [r5, #52]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xb2d4
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [r0, #8]
	ldr	r2, [r0, #4]
	adds	r6, r1, #0
	ldr	r1, [r0, #0]
	ldr	r0, [pc, #104]
	adds	r3, r3, r0
	movs	r0, #30
	adds	r0, #255
	bl 0x0200b128
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001f86
	bl 0x0200b0c8
	adds	r3, r0, #0
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #12
	movs	r3, #192
	lsls	r3, r3, #6
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	bl 0x0200b0d8
	str	r0, [r5, #68]
	bl 0x0200b0c8
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	str	r0, [r5, #72]
	bl 0x0200b0c8
	lsls	r0, r0, #17
	lsrs	r0, r0, #16
	adds	r0, r0, r6
	str	r0, [r5, #76]
	bl 0x0200b0c8
	ldr	r3, [pc, #36]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r3, r5, #0
	adds	r3, #100
	strh	r0, [r3, #0]
	movs	r1, #2
	adds	r0, r5, #0
	bl 0x02009ed4
	ldr	r3, [pc, #20]
	adds	r0, r5, #0
	str	r3, [r5, #108]
	movs	r1, #1
	bl 0x0200b190
.L_02001f86:
	pop	{r5, r6, pc}
	.4byte 0xfffe0000
	.4byte 0xffff8000
	.2byte 0x9e39
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r0, [pc, #364]
	sub	sp, #8
	mov	r8, r0
	movs	r0, #192
	lsls	r0, r0, #18
	ldr	r1, [r0, #32]
	mov	r7, r8
	adds	r2, r1, #0
	adds	r2, #228
	ldr	r3, [r2, #0]
	ldr	r2, [r2, #4]
	mov	r9, r3
	ldr	r3, [pc, #344]
	mov	r4, r9
	ands	r4, r3
	ands	r2, r3
	ldr	r3, [r1, #0]
	mov	r9, r4
	ldr	r3, [r3, #4]
	adds	r7, #20
	str	r3, [sp, #4]
	mov	sl, r2
	ldr	r0, [r0, #108]
	str	r0, [sp, #0]
	mov	r0, r8
	movs	r4, #6
	ldrsh	r3, [r0, r4]
	cmp	r3, #0
	beq.n	0x02009fee
	movs	r1, #8
	ldrsh	r3, [r0, r1]
	cmp	r3, #0
	bne.n	0x02009fee
	ldr	r3, [r0, #16]
	cmp	r3, #0
	beq.n	0x02009fee
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4642
	ldrh	r3, [r2, #6]
	mov	r4, r8
	movs	r2, #0
	mov	r0, r8
	strh	r3, [r4, #8]
	strh	r2, [r0, #6]
	movs	r1, #3
	mov	fp, r1
.L_02002000:
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	cmp	r3, #0
	beq.n	.L_020020f0
	ldr	r5, [r7, #8]
	cmp	r5, #0
	beq.n	.L_020020f0
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200b0f8
	ldr	r4, [sp, #0]
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r3, r4, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_0200202a
	movs	r3, #1
	orrs	r0, r3
.L_0200202a:
	adds	r6, r5, #0
	adds	r6, #91
	strb	r0, [r6, #0]
	mov	r0, r8
	movs	r4, #14
	ldrsh	r3, [r0, r4]
	cmp	r3, #0
	beq.n	.L_02002044
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200b0f8
	strb	r0, [r6, #0]
.L_02002044:
	ldr	r2, [r5, #8]
	mov	r1, r9
	ldr	r3, [r5, #16]
	subs	r2, r2, r1
	ldr	r1, [r5, #12]
	mov	r4, sl
	subs	r3, r3, r4
	subs	r1, r3, r1
	movs	r0, #6
	ldrsh	r4, [r7, r0]
	asrs	r3, r1, #16
	adds	r1, r3, #0
	asrs	r2, r2, #16
	subs	r1, #8
	cmp	r4, #0
	bne.n	.L_0200207a
	adds	r3, r2, #7
	movs	r2, #167
	lsls	r2, r2, #1
	cmp	r3, r2
	bhi.n	.L_020020f0
	movs	r3, #48
	negs	r3, r3
	cmp	r1, r3
	ble.n	.L_020020f0
	cmp	r1, #239
	bgt.n	.L_020020f0
.L_0200207a:
	movs	r0, #2
	ldrsh	r3, [r7, r0]
	ldrh	r1, [r7, #2]
	cmp	r3, #0
	bgt.n	.L_020020ec
	ldrh	r3, [r7, #4]
	movs	r1, #240
	ands	r1, r3
	cmp	r1, #32
	beq.n	.L_020020c0
.L_0200208e:
	cmp	r1, #32
	bgt.n	.L_0200209c
	cmp	r1, #0
	beq.n	.L_020020dc
	cmp	r1, #16
	beq.n	.L_020020ce
	b.n	.L_020020e8
.L_0200209c:
	cmp	r1, #48
	beq.n	.L_020020b2
	cmp	r1, #128
.L_020020a2:
	bne.n	.L_020020e8
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	adds	r0, #8
	adds	r2, r4, #0
	bl 0x02009f14
	b.n	.L_020020e8
.L_020020b2:
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	adds	r0, #8
	adds	r2, r4, #0
	bl 0x02009d08
	b.n	.L_020020e8
.L_020020c0:
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	adds	r0, #8
.L_020020c6:
	adds	r2, r4, #0
.L_020020c8:
	bl 0x02009d98
	b.n	.L_020020e8
.L_020020ce:
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	adds	r0, #8
	adds	r2, r4, #0
	bl 0x02009d50
.L_020020da:
	b.n	.L_020020e8
.L_020020dc:
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	adds	r0, #8
	adds	r2, r4, #0
	bl 0x02009b1c
.L_020020e8:
	movs	r3, #8
	b.n	.L_020020ee
.L_020020ec:
	subs	r3, r1, #1
.L_020020ee:
	strh	r3, [r7, #2]
.L_020020f0:
	movs	r1, #1
.L_020020f2:
	negs	r1, r1
	add	fp, r1
	mov	r2, fp
	adds	r7, #16
	cmp	r2, #0
	blt.n	.L_02002100
.L_020020fe:
	b.n	.L_02002000
.L_02002100:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200254c
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb560
	movs	r0, #10
	adds	r0, #255
	ldr	r6, [pc, #68]
.L_02002120:
	bl 0x0200b0f8
	cmp	r0, #0
	bne.n	0x0200a132
	ldr	r3, [pc, #60]
	adds	r0, r6, #0
	movs	r1, #116
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x206e
.L_02002134:
	movs	r1, #1
	movs	r2, #0
	movs	r3, #0
	adds	r0, #255
	bl 0x0200b128
	adds	r5, r0, #0
	adds	r2, r5, #0
.L_02002144:
	adds	r2, #92
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r1, #1
	bl 0x0200b118
	ldr	r1, [r5, #80]
	movs	r2, #1
.L_02002154:
	ldrb	r3, [r1, #16]
	str	r5, [r6, #112]
	strh	r3, [r6, #12]
	ldrb	r3, [r1, #17]
	orrs	r3, r2
	strb	r3, [r1, #17]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0200254c
	.2byte 0x0258
	.2byte 0x0300
	push	{r5, lr}
	ldr	r5, [pc, #12]
	ldr	r0, [r5, #112]
	bl 0x0200b130
	movs	r3, #0
	str	r3, [r5, #112]
	pop	{r5, pc}
	.4byte 0x0200254c
	.4byte 0x81d84b01
	.4byte 0x00004770
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r8, r1
	ldr	r1, [pc, #280]
	sub	sp, #16
	adds	r6, r0, #0
	movs	r0, #10
	str	r1, [sp, #4]
	adds	r0, #255
	adds	r1, #20
	str	r2, [sp, #12]
	str	r3, [sp, #8]
	mov	r9, r1
	bl 0x0200b0f8
	cmp	r0, #0
	bne.n	.L_02002270
	ldrh	r3, [r6, #0]
	movs	r2, #0
	mov	fp, r2
	mov	sl, r3
	adds	r6, #2
	cmp	r3, #0
	ble.n	.L_0200222e
.L_020021c6:
	ldrh	r7, [r6, #0]
	movs	r1, #15
	ands	r1, r7
	movs	r3, #240
	mov	r0, sl
	str	r1, [sp, #0]
	ands	r7, r3
	bl 0x0200b1f0
	adds	r5, r0, #0
	adds	r6, #2
	cmp	r5, #0
	beq.n	.L_0200221a
	movs	r1, #0
	bl 0x0200b188
	adds	r3, r5, #0
	movs	r2, #128
	adds	r3, #98
	movs	r1, #1
	ands	r2, r7
	strb	r1, [r3, #0]
	cmp	r2, #0
	bne.n	.L_020021fa
	subs	r3, #9
	strb	r2, [r3, #0]
.L_020021fa:
	mov	r2, r9
	mov	r3, r9
	strh	r1, [r2, #0]
	mov	r0, sl
	strh	r7, [r3, #4]
	bl 0x0200b1f0
	mov	r1, r9
	str	r0, [r1, #8]
	ldr	r2, [sp, #0]
	lsls	r3, r2, #16
	str	r3, [r1, #12]
	mov	r3, fp
	strh	r3, [r1, #2]
	movs	r2, #16
	add	r9, r2
.L_0200221a:
	movs	r3, #1
	add	fp, r3
	mov	r1, fp
	cmp	r1, #3
	bgt.n	.L_0200222e
	ldrh	r2, [r6, #0]
	adds	r6, #2
	mov	sl, r2
	cmp	r2, #0
	bgt.n	.L_020021c6
.L_0200222e:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_02002270
	movs	r1, #0
	ldrh	r2, [r3, #0]
	mov	fp, r1
	ldr	r1, [sp, #4]
	movs	r3, #2
	add	r8, r3
	movs	r3, #84
	strh	r2, [r1, r3]
	cmp	r2, #0
	ble.n	.L_02002270
	adds	r2, r1, #0
	adds	r2, #84
.L_0200224c:
	mov	r1, r8
	ldrh	r3, [r1, #0]
	movs	r1, #1
	strh	r3, [r2, #2]
	add	fp, r1
	movs	r3, #2
	add	r8, r3
	mov	r3, fp
	adds	r2, #4
	cmp	r3, #3
	bgt.n	.L_02002270
	mov	r1, r8
	ldrh	r3, [r1, #0]
	movs	r1, #2
	add	r8, r1
	strh	r3, [r2, #0]
	cmp	r3, #0
	bgt.n	.L_0200224c
.L_02002270:
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #4]
	add	r1, sp, #8
	str	r2, [r3, #16]
	ldrh	r1, [r1, #0]
	ldr	r2, [sp, #4]
	strh	r1, [r2, #10]
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #80
	ldrh	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L_020022a0
	movs	r3, #192
	movs	r2, #128
	lsls	r3, r3, #4
	lsls	r2, r2, #19
	adds	r3, #8
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #16
	strh	r3, [r1, #0]
.L_020022a0:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #20]
	bl 0x0200b0b8
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200254c
	.4byte 0x02009f95
	.4byte 0x01004b02
	.4byte 0x231418c0
	.4byte 0x47705ec0
	.4byte 0x0200254c
	.4byte 0x01004b02
	.4byte 0x828118c0
	.4byte 0x00004770
	.4byte 0x0200254c
	.4byte 0x68184b01
	.4byte 0x00004770
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r0, #130
	lsls	r0, r0, #1
	ldr	r5, [pc, #24]
	bl 0x0200b0f8
	cmp	r0, #0
	bne.n	.L_0200230a
	ldr	r3, [r5, #0]
	adds	r3, #1
	str	r3, [r5, #0]
	cmp	r3, r6
	blt.n	.L_0200230a
	str	r0, [r5, #0]
.L_0200230a:
	ldr	r0, [r5, #0]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0200254c
	.4byte 0x01004b06
	.4byte 0x230f18c0
	.4byte 0x3014400b
	.4byte 0x60c3041b
	.4byte 0x40194b01
	.4byte 0x47708081
	.4byte 0x000000f0
	.4byte 0x0200254c
	.4byte 0x01004b02
	.4byte 0x834118c0
	.4byte 0x00004770
	.4byte 0x0200254c
	.4byte 0x01004b02
	.4byte 0x231818c0
	.4byte 0x47705ec0
	.2byte 0x254c
	.2byte 0x0200
	push	{lr}
	ldr	r2, [pc, #12]
	cmp	r0, #3
	bhi.n	.L_02002362
	lsls	r3, r0, #2
	adds	r3, #84
	strh	r1, [r2, r3]
.L_02002362:
	pop	{pc}
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	sub	sp, #16
	ldr	r6, [r3, #108]
	bl 0x0200b298
	mov	r8, r0
	bl 0x0200b1f0
	bl 0x0200b1d0
	movs	r5, #0
	adds	r7, r0, #0
	cmp	r5, r7
	bge.n	.L_020023aa
.L_0200238e:
	ldr	r2, [pc, #192]
	movs	r1, #134
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldrb	r0, [r2, r3]
	bl 0x0200b0f0
	ldrh	r3, [r0, #56]
	lsls	r2, r5, #1
	mov	r1, sp
	adds	r5, #1
	strh	r3, [r1, r2]
	cmp	r5, r7
	blt.n	.L_0200238e
.L_020023aa:
	movs	r0, #10
	negs	r0, r0
	movs	r1, #0
	bl 0x0200b288
	movs	r2, #182
	lsls	r2, r2, #1
	movs	r4, #183
	adds	r3, r6, r2
	lsls	r4, r4, #1
	movs	r2, #0
	strh	r2, [r3, #0]
	movs	r1, #129
	adds	r3, r6, r4
	strh	r2, [r3, #0]
	mov	r0, r8
	lsls	r1, r1, #1
	movs	r5, #0
	bl 0x0200b210
	cmp	r5, r7
	bge.n	.L_02002444
.L_020023d6:
	ldr	r1, [pc, #120]
	movs	r2, #134
	lsls	r2, r2, #2
	adds	r2, r2, r5
	ldrb	r0, [r1, r2]
	mov	sl, r1
	mov	r8, r2
	bl 0x0200b0f0
	movs	r4, #56
	ldrsh	r3, [r0, r4]
	cmp	r3, #0
	ble.n	.L_020023fe
	movs	r1, #183
	lsls	r1, r1, #1
	adds	r2, r6, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_0200243e
.L_020023fe:
	mov	r3, sp
	lsls	r2, r5, #1
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_0200243e
	movs	r2, #182
	lsls	r2, r2, #1
	adds	r1, r6, r2
	ldrh	r3, [r1, #0]
	movs	r4, #184
	adds	r2, r3, #1
	lsls	r3, r3, #16
	lsls	r4, r4, #1
	asrs	r3, r3, #15
	strh	r2, [r1, #0]
	adds	r3, r3, r4
	mov	r1, sl
	mov	r4, r8
	ldrb	r2, [r1, r4]
	movs	r1, #181
	strh	r2, [r6, r3]
	movs	r3, #255
	lsls	r1, r1, #1
	lsls	r3, r3, #8
	adds	r2, r6, r1
	adds	r3, #255
	strh	r3, [r2, #0]
	movs	r3, #50
	adds	r3, #255
	adds	r2, r0, r3
	movs	r3, #0
	strb	r3, [r2, #0]
.L_0200243e:
	adds	r5, #1
	cmp	r5, r7
	blt.n	.L_020023d6
.L_02002444:
	add	sp, #16
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #96]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200b1f0
	ldr	r5, [r6, #68]
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #72]
	adds	r3, r3, r5
	str	r3, [r6, #8]
	ldr	r3, [r6, #12]
	ldr	r7, [r6, #76]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r0, r5, #0
	adds	r3, r3, r7
	movs	r1, #18
	str	r3, [r6, #16]
	bl 0x0200b0a0
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02002492
	adds	r3, #15
.L_02002492:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #76]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #24]
	movs	r1, #128
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [r6, #52]
	ldr	r3, [r6, #28]
	lsls	r1, r1, #5
	adds	r3, r3, r2
	str	r3, [r6, #28]
	ldr	r2, [r6, #80]
	ldrh	r3, [r2, #18]
	adds	r3, r3, r1
	strh	r3, [r2, #18]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #32]
	ldr	r2, [r3, #0]
	movs	r3, #7
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_020024d0
	cmp	r2, #4
	beq.n	.L_020024d8
	b.n	.L_020024de
.L_020024d0:
	movs	r1, #10
	bl 0x0200b1a0
	b.n	.L_020024de
.L_020024d8:
	movs	r1, #0
	bl 0x0200b1a0
.L_020024de:
	pop	{pc}
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #484]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	adds	r3, r5, r0
	ldrb	r3, [r3, #0]
	sub	sp, #16
	cmp	r3, #0
	beq.n	.L_02002506
	b.n	.L_020026c4
.L_02002506:
	movs	r0, #10
	movs	r1, #0
	negs	r0, r0
	bl 0x0200a368
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldr	r0, [r3, #0]
	bl 0x0200b1f0
	ldr	r2, [pc, #444]
	adds	r6, r0, #0
	str	r2, [sp, #0]
	bl 0x0200b1e0
	movs	r0, #0
	bl 0x0200b280
	ldr	r3, [pc, #432]
	adds	r0, r6, #0
	str	r3, [r6, #108]
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	movs	r1, #49
	bl 0x0200b118
.L_0200253e:
	adds	r3, r6, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	bl 0x0200b168
	ldr	r3, [sp, #0]
	ldr	r1, [sp, #0]
	adds	r3, #104
	adds	r1, #106
	mov	r9, r1
	mov	sl, r3
	add	r1, sp, #4
	cmp	r0, #7
	bne.n	.L_020025ba
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	mov	r1, r9
	lsls	r3, r3, #17
	str	r3, [r6, #36]
	movs	r5, #0
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	lsls	r3, r3, #17
	str	r3, [r6, #44]
	movs	r3, #128
	lsls	r3, r3, #5
	str	r3, [r6, #52]
.L_02002578:
	mov	r0, sl
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	ldr	r3, [r6, #8]
	lsls	r2, r2, #19
	add	r1, sp, #4
	adds	r3, r3, r2
	str	r3, [r1, #0]
	mov	r0, r9
	ldr	r3, [r6, #12]
	str	r3, [r1, #4]
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	ldr	r3, [r6, #16]
	lsls	r2, r2, #19
	adds	r3, r3, r2
	str	r3, [r1, #8]
	adds	r0, r6, #0
	bl 0x0200b180
	cmp	r0, #0
	beq.n	.L_020025ac
	movs	r3, #0
	str	r3, [r6, #36]
	str	r3, [r6, #44]
	b.n	.L_020026b8
.L_020025ac:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200b0b0
	cmp	r5, #9
	ble.n	.L_02002578
	b.n	.L_020026b8
.L_020025ba:
	mov	r0, sl
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	ldr	r3, [r6, #8]
	lsls	r2, r2, #19
	adds	r3, r3, r2
	str	r3, [r1, #0]
	mov	r0, r9
	ldr	r3, [r6, #12]
	str	r3, [r1, #4]
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	ldr	r3, [r6, #16]
	lsls	r2, r2, #19
	adds	r3, r3, r2
	str	r3, [r1, #8]
	adds	r0, r6, #0
	bl 0x0200b180
	cmp	r0, #0
	bgt.n	.L_020026b8
	cmp	r0, #0
	bge.n	.L_02002604
	movs	r3, #128
	lsls	r3, r3, #7
.L_020025ec:
	strh	r3, [r6, #6]
	ldr	r3, [pc, #232]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	movs	r2, #1
	ldr	r0, [r3, #0]
	movs	r1, #6
	negs	r2, r2
	bl 0x0200b200
	b.n	.L_020026b8
.L_02002604:
	ldrh	r3, [r6, #32]
	movs	r2, #0
	subs	r3, #2
	mov	fp, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #20]
	mov	r8, r2
	adds	r7, r5, #0
	adds	r7, #89
.L_02002618:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02002648
	ldrb	r2, [r7, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02002648
	cmp	r5, r6
	beq.n	.L_02002648
	ldrh	r3, [r5, #32]
	adds	r0, r5, #0
	adds	r0, #8
	subs	r3, #2
	mov	r1, fp
	add	r2, sp, #4
	bl 0x0200b1b0
	cmp	r0, #0
	blt.n	.L_02002648
	movs	r0, #1
	bl 0x0200b0b0
	b.n	.L_020026b8
.L_02002648:
	movs	r3, #1
	add	r8, r3
	mov	r0, r8
	adds	r7, #128
	adds	r5, #128
	cmp	r0, #63
	ble.n	.L_02002618
	mov	r2, sl
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	ldr	r2, [r6, #8]
	lsls	r3, r3, #17
	adds	r1, r2, r3
	str	r1, [r6, #8]
	mov	r2, r9
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	ldr	r2, [r6, #16]
	ldr	r7, [pc, #116]
	lsls	r3, r3, #17
	ldr	r0, [pc, #116]
	adds	r5, r2, r3
	adds	r3, r1, #0
	ands	r3, r7
	movs	r4, #128
	adds	r2, r3, r0
	lsls	r4, r4, #9
	str	r5, [r6, #16]
	cmp	r2, r4
	ble.n	.L_02002686
	adds	r2, r4, #0
.L_02002686:
	ldr	r0, [pc, #100]
	cmp	r2, r0
	bge.n	.L_0200268e
	adds	r2, r0, #0
.L_0200268e:
	subs	r3, r1, r2
	ldr	r1, [pc, #84]
	str	r3, [r6, #8]
	adds	r3, r5, #0
	ands	r3, r7
	adds	r2, r3, r1
	cmp	r2, r4
	ble.n	.L_020026a0
	adds	r2, r4, #0
.L_020026a0:
	cmp	r2, r0
	bge.n	.L_020026a6
	adds	r2, r0, #0
.L_020026a6:
	subs	r3, r5, r2
	str	r3, [r6, #16]
	ldr	r2, [sp, #0]
	movs	r3, #0
	strh	r3, [r2, #4]
	movs	r0, #1
	bl 0x0200b0b0
	b.n	.L_0200253e
.L_020026b8:
	movs	r3, #0
	str	r3, [r6, #108]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200b1a0
.L_020026c4:
	ldr	r0, [sp, #0]
	movs	r3, #0
	strh	r3, [r0, #4]
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200254c
	.4byte 0x0200a4bd
	.4byte 0x000fffff
	.4byte 0xfff80000
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb500
	ldr	r3, [pc, #16]
	movs	r2, #4
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02002700
	bl 0x0200a4e4
.L_02002700:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	movs	r1, #217
	lsls	r1, r1, #1
	adds	r6, r5, r1
	ldrh	r3, [r6, #0]
	sub	sp, #12
	cmp	r3, #0
	bne.n	.L_02002734
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r5, r2
	adds	r1, #2
	ldr	r0, [r3, #0]
	adds	r3, r5, r1
	ldr	r1, [r3, #0]
	bl 0x0200b240
	movs	r3, #1
	strh	r3, [r6, #0]
.L_02002734:
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200b108
	movs	r2, #179
	lsls	r2, r2, #1
	movs	r1, #173
	adds	r3, r5, r2
	lsls	r1, r1, #1
	movs	r2, #0
	strh	r2, [r3, #0]
	adds	r3, r5, r1
	adds	r1, #4
	strh	r2, [r3, #0]
	adds	r3, r5, r1
	subs	r1, #2
	strh	r2, [r3, #0]
	adds	r3, r5, r1
	adds	r1, #8
	strh	r2, [r3, #0]
	movs	r0, #10
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	movs	r1, #0
	negs	r0, r0
	bl 0x0200a368
	movs	r0, #224
	movs	r1, #224
	lsls	r1, r1, #8
	lsls	r0, r0, #11
	bl 0x0200b218
	ldr	r3, [pc, #192]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b1f0
	adds	r6, r0, #0
	movs	r0, #131
	lsls	r0, r0, #1
	ldr	r7, [pc, #176]
	bl 0x0200b100
	bl 0x0200b1e0
	movs	r0, #0
	bl 0x0200b280
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200b108
	ldr	r3, [pc, #156]
	movs	r1, #49
	str	r3, [r6, #108]
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	adds	r0, r6, #0
	bl 0x0200b118
	ldr	r3, [r7, #108]
	adds	r3, #100
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_02002820
.L_020027c0:
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	movs	r0, #0
	bl 0x0200b168
	ldr	r1, [r7, #108]
	cmp	r0, #7
	beq.n	.L_020027e0
	ldr	r2, [r1, #112]
	ldr	r3, [r6, #8]
	adds	r3, r3, r2
	str	r3, [r6, #8]
	ldr	r3, [r6, #16]
	ldr	r2, [r1, #120]
	adds	r3, r3, r2
	b.n	.L_02002808
.L_020027e0:
	ldr	r3, [r1, #112]
	cmp	r3, #0
	beq.n	.L_020027f4
	ldr	r3, [r6, #8]
	ldr	r2, [pc, #88]
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r6, #8]
.L_020027f4:
	ldr	r3, [r7, #108]
	ldr	r3, [r3, #120]
	cmp	r3, #0
	beq.n	.L_0200280a
	ldr	r3, [r6, #16]
	ldr	r2, [pc, #68]
	movs	r1, #128
	ands	r3, r2
	lsls	r1, r1, #12
	adds	r3, r3, r1
.L_02002808:
	str	r3, [r6, #16]
.L_0200280a:
	movs	r3, #0
	strh	r3, [r7, #4]
	movs	r0, #1
	bl 0x0200b0b0
	ldr	r3, [r7, #108]
	adds	r3, #100
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_020027c0
.L_02002820:
	movs	r5, #0
	movs	r0, #30
	bl 0x0200b1d8
	adds	r0, r6, #0
	str	r5, [r6, #108]
	movs	r1, #0
	bl 0x0200b1a0
	strh	r5, [r7, #4]
	add	sp, #12
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200254c
	.4byte 0x0200a4bd
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb500
	ldr	r3, [pc, #20]
	movs	r2, #4
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_0200285c
	bl 0x0200a708
	movs	r0, #1
	b.n	.L_0200285e
.L_0200285c:
	movs	r0, #0
.L_0200285e:
	pop	{pc}
	.4byte 0x0200254c
	.4byte 0x22044b01
	.4byte 0x47705e98
	.2byte 0x254c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r1, [pc, #344]
	sub	sp, #4
	ldr	r3, [r1, #112]
	mov	fp, r0
	cmp	r3, #0
	bne.n	.L_0200288c
	b.n	.L_020029e4
.L_0200288c:
	movs	r2, #0
	str	r2, [sp, #0]
.L_02002890:
	bl 0x0200b0c8
	adds	r5, r0, #0
	bl 0x0200b0c8
	mov	r3, fp
	ldr	r3, [r3, #8]
	lsls	r5, r5, #4
	mov	r8, r3
	add	r8, r5
	lsls	r0, r0, #4
	mov	r1, r8
	subs	r1, r1, r0
	mov	r8, r1
	bl 0x0200b0c8
	adds	r6, r0, #0
	bl 0x0200b0c8
	adds	r5, r0, #0
	bl 0x0200b0c8
	mov	r2, fp
	ldr	r3, [r2, #16]
	ldr	r2, [r2, #12]
	lsls	r5, r5, #4
	lsls	r6, r6, #3
	movs	r1, #128
	lsls	r0, r0, #4
	adds	r6, r6, r2
	lsls	r1, r1, #11
	adds	r3, r3, r5
	subs	r3, r3, r0
	adds	r6, r6, r1
	movs	r0, #234
	adds	r0, #255
	mov	r1, r8
	adds	r2, r6, #0
	bl 0x0200b128
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_020029c4
	bl 0x0200b0c8
	mov	sl, r0
	bl 0x0200b0c8
	adds	r6, r0, #0
	bl 0x0200b0c8
	adds	r5, r0, #0
	bl 0x0200b0c8
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r5, r5, r0
	ldr	r1, [pc, #216]
	adds	r0, r7, #0
	mov	r9, r2
	bl 0x0200b120
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200b188
	mov	r1, sl
	movs	r2, #128
	lsls	r2, r2, #10
	lsls	r3, r1, #2
	adds	r3, r3, r2
	str	r3, [r7, #40]
	mov	r0, sl
	bl 0x0200b0d8
	ldr	r3, [pc, #184]
	lsls	r6, r6, #3
	mov	r8, r3
	adds	r1, r0, #0
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x62f8
	mov	r0, sl
	bl 0x0200b0d0
	adds	r1, r0, #0
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x4a24
	movs	r3, #168
	lsls	r3, r3, #7
	adds	r3, #122
	str	r3, [r7, #72]
	lsrs	r5, r5, #2
	ldr	r3, [r2, #112]
	add	r5, r9
	movs	r6, #0
	mov	r1, r9
	str	r5, [r7, #24]
	str	r5, [r7, #28]
	str	r1, [r7, #68]
	ldr	r5, [r7, #80]
	str	r0, [r7, #36]
	str	r6, [r7, #52]
	ldr	r3, [r3, #80]
	ldrb	r0, [r5, #16]
	mov	r8, r3
	bl 0x0200b0e8
	ldrb	r3, [r5, #17]
	ldr	r1, [pc, #100]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r5, #17]
	ldrh	r3, [r1, #12]
	ldr	r0, [r5, #40]
	strb	r3, [r5, #16]
	bl 0x0200b110
	str	r6, [r5, #40]
	strb	r6, [r5, #27]
	mov	r2, r8
	ldrb	r3, [r2, #20]
	ldrb	r0, [r5, #5]
	strb	r3, [r5, #20]
	ldrb	r3, [r2, #21]
	strb	r3, [r5, #21]
	ldrb	r1, [r2, #5]
	movs	r2, #63
	adds	r3, r2, #0
	lsrs	r1, r1, #6
	lsls	r1, r1, #6
	ands	r3, r0
	orrs	r3, r1
	strb	r3, [r5, #5]
	mov	r1, r8
	ldrb	r3, [r1, #7]
	ldrb	r1, [r5, #7]
	lsrs	r3, r3, #6
	lsls	r3, r3, #6
	ands	r2, r1
	orrs	r2, r3
	strb	r2, [r5, #7]
	mov	r3, r8
	ldrh	r2, [r3, #8]
	ldr	r1, [pc, #28]
	ldrh	r3, [r5, #8]
	lsls	r2, r2, #22
	lsrs	r2, r2, #22
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #8]
.L_020029c4:
	ldr	r1, [sp, #0]
	subs	r1, #1
	str	r1, [sp, #0]
	cmp	r1, #0
	blt.n	.L_020029d0
	b.n	.L_02002890
.L_020029d0:
	b.n	.L_020029e4
	.2byte 0x0000
	.4byte 0xfffffc00
	.4byte 0x0200254c
	.4byte 0x0200b304
	.2byte 0x021c
	.2byte 0x0300
.L_020029e4:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
	str	r3, [sp, #0]
	movs	r3, #1
	mov	fp, r3
.L_02002a10:
	movs	r0, #70
	adds	r0, #255
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #4]
	bl 0x0200b128
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_02002ab2
	bl 0x0200b0c8
	adds	r5, r0, #0
	bl 0x0200b0c8
	ldr	r3, [sp, #0]
	lsrs	r5, r5, #4
	adds	r5, r3, r5
	lsrs	r0, r0, #4
	movs	r3, #128
	subs	r5, r5, r0
	lsls	r3, r3, #7
	mov	sl, r3
	adds	r3, r5, #0
	add	r3, sl
	mov	r9, r3
	bl 0x0200b0c8
	mov	r8, r0
	mov	r3, r8
	lsls	r3, r3, #3
	mov	r8, r3
	bl 0x0200b0c8
	ldr	r1, [pc, #116]
	adds	r6, r0, #0
	adds	r0, r7, #0
	bl 0x0200b120
	movs	r1, #0
	adds	r0, r7, #0
	bl 0x0200b188
	movs	r3, #160
	lsls	r3, r3, #9
	adds	r5, r5, r3
	str	r5, [r7, #40]
	mov	r0, r9
	bl 0x0200b0d8
	ldr	r5, [pc, #88]
	adds	r1, r0, #0
	mov	r0, r8
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x62f8
	mov	r0, r9
	bl 0x0200b0d0
	adds	r1, r0, #0
	mov	r0, r8
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2300
	str	r3, [r7, #52]
	movs	r3, #168
	lsls	r3, r3, #7
	adds	r3, #122
	lsrs	r6, r6, #1
	str	r3, [r7, #72]
	movs	r3, #128
	add	r6, sl
	lsls	r3, r3, #8
	str	r0, [r7, #36]
	str	r6, [r7, #24]
	str	r6, [r7, #28]
	str	r3, [r7, #68]
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200b1a0
.L_02002ab2:
	movs	r3, #1
	negs	r3, r3
	add	fp, r3
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L_02002a10
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200b348
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200b0f8
	adds	r5, #91
	strb	r0, [r5, #0]
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r3, #192
	movs	r0, #100
	lsls	r3, r3, #18
	adds	r0, r0, r5
	ldr	r6, [r3, #108]
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	sub	sp, #56
	mov	r8, r0
	cmp	r3, #0
	beq.n	.L_02002b10
	b.n	.L_02002d9c
.L_02002b10:
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200b0f8
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r3, r6, r2
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #0
	beq.n	.L_02002b2a
	movs	r3, #1
	orrs	r0, r3
.L_02002b2a:
	adds	r3, r5, #0
	adds	r3, #91
	strb	r0, [r3, #0]
	add	r7, sp, #44
	ldr	r3, [r5, #8]
	movs	r0, #128
	str	r3, [r7, #0]
	lsls	r0, r0, #12
	ldr	r3, [r5, #12]
	adds	r2, r7, #0
	str	r3, [r7, #4]
	ldr	r3, [r5, #16]
	str	r3, [r7, #8]
	ldrh	r1, [r5, #6]
	bl 0x0200b0e0
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #8]
	movs	r0, #0
	bl 0x0200b168
	cmp	r0, #7
	bne.n	0x0200aba0
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r5, #64]
	str	r3, [r5, #60]
	str	r3, [r5, #56]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r5, #52]
	mov	r0, r8
	movs	r3, #1
	strh	r3, [r0, #0]
	movs	r0, #145
	bl 0x0200b2b8
	movs	r0, #160
	lsls	r0, r0, #11
	movs	r2, #128
	adds	r1, r0, #0
	lsls	r2, r2, #9
	bl 0x0200b198
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200b198
	ldr	r3, [r5, #104]
	cmp	r3, #0
	beq.n	0x0200aba0
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x23c0
	lsls	r3, r3, #18
	ldr	r0, [r3, #32]
	ldr	r3, [r5, #8]
	ldr	r1, [r5, #16]
	asrs	r3, r3, #20
	str	r3, [sp, #32]
	movs	r3, #184
	lsls	r3, r3, #1
	ldr	r4, [sp, #32]
	asrs	r1, r1, #20
	adds	r2, r0, r3
	ldr	r2, [r2, #0]
	lsls	r3, r1, #7
	adds	r3, r4, r3
	lsls	r3, r3, #2
	adds	r2, r2, r3
	str	r2, [sp, #28]
	movs	r4, #212
	lsls	r4, r4, #1
	adds	r2, r0, r4
	ldr	r2, [r2, #0]
	subs	r4, #92
	adds	r2, r2, r3
	str	r2, [sp, #24]
	movs	r2, #164
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldr	r3, [r3, #0]
	asrs	r3, r3, #20
	str	r3, [sp, #20]
	adds	r3, r0, r4
	adds	r4, #52
	ldr	r2, [r3, #0]
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	adds	r4, #4
	asrs	r3, r3, #20
	str	r3, [sp, #16]
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r0, #1
	asrs	r3, r3, #20
	adds	r3, r1, r3
	subs	r3, #2
	asrs	r2, r2, #20
	negs	r0, r0
	str	r3, [sp, #8]
	str	r0, [sp, #40]
	subs	r3, r1, #1
	adds	r1, r1, r2
	subs	r1, #1
	mov	r8, r3
	mov	fp, r1
.L_02002c0c:
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #20]
	movs	r4, #1
	adds	r3, r0, r1
	subs	r3, #1
	negs	r4, r4
	mov	r9, r3
	str	r4, [sp, #36]
	adds	r3, r0, r2
	adds	r6, r0, #0
	subs	r3, #1
	subs	r6, #1
	mov	sl, r3
.L_02002c28:
	ldr	r4, [sp, #40]
	ldr	r0, [sp, #36]
	lsls	r3, r4, #7
	adds	r3, r3, r0
	lsls	r3, r3, #2
	ldr	r1, [sp, #28]
	str	r3, [sp, #12]
	adds	r2, r3, r1
	ldrb	r3, [r2, #2]
	cmp	r3, #77
	bne.n	.L_02002c88
	movs	r3, #0
	strb	r3, [r2, #2]
	mov	r2, sl
	mov	r3, fp
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #64
	movs	r1, #64
	movs	r2, #1
	movs	r3, #1
	bl 0x0200b178
	mov	r4, r8
	movs	r1, #64
	movs	r2, #1
	movs	r3, #1
	movs	r0, #64
	str	r4, [sp, #4]
	str	r6, [sp, #0]
	bl 0x0200b170
	movs	r0, #159
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200b2b8
	mov	r3, r8
	movs	r4, #128
	lsls	r4, r4, #12
	lsls	r2, r3, #20
	lsls	r0, r6, #20
	adds	r0, r0, r4
	ldr	r1, [r5, #12]
	ldrh	r3, [r5, #6]
	adds	r2, r2, r4
	bl 0x0200a9f4
.L_02002c88:
	ldr	r4, [sp, #12]
	ldr	r0, [sp, #24]
	adds	r2, r4, r0
	ldrb	r3, [r2, #2]
	cmp	r3, #77
	bne.n	.L_02002cdc
	movs	r3, #0
	strb	r3, [r2, #2]
	ldr	r2, [sp, #8]
	mov	r1, r9
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #64
	movs	r1, #64
	movs	r2, #1
	movs	r3, #2
	bl 0x0200b178
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #64
	movs	r2, #1
	movs	r3, #1
	movs	r0, #64
	str	r6, [sp, #0]
	bl 0x0200b170
	movs	r0, #143
	lsls	r0, r0, #2
	bl 0x0200b2b8
	mov	r3, r8
	movs	r4, #128
	lsls	r4, r4, #12
	lsls	r2, r3, #20
	lsls	r0, r6, #20
	adds	r0, r0, r4
	ldr	r1, [r5, #12]
	ldrh	r3, [r5, #6]
	adds	r2, r2, r4
	bl 0x0200a9f4
.L_02002cdc:
	ldr	r0, [sp, #36]
	movs	r4, #1
	adds	r0, #1
	add	r9, r4
	adds	r6, #1
	add	sl, r4
	str	r0, [sp, #36]
	cmp	r0, #1
	ble.n	.L_02002c28
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #40]
	adds	r1, #1
	adds	r2, #1
	str	r1, [sp, #8]
	add	r8, r4
	add	fp, r4
	str	r2, [sp, #40]
	cmp	r2, #1
.L_02002d00:
	ble.n	.L_02002c0c
	ldr	r3, [r5, #24]
	movs	r4, #128
	lsls	r4, r4, #9
	cmp	r3, r4
	bge.n	.L_02002d1a
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r2
	str	r3, [r5, #24]
	ldr	r3, [r5, #28]
	adds	r3, r3, r2
	str	r3, [r5, #28]
.L_02002d1a:
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #8]
	adds	r0, r5, #0
	bl 0x0200b140
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #108]
	ldr	r3, [pc, #176]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	ldr	r6, [pc, #172]
	bl 0x0200b1f0
	ldr	r1, [r5, #8]
	ldr	r3, [r0, #8]
	subs	r2, r1, r3
	cmp	r2, #0
	blt.n	.L_02002d50
	movs	r1, #160
	lsls	r1, r1, #13
	cmp	r2, r1
	blt.n	.L_02002d5a
	b.n	.L_02002dd0
.L_02002d50:
	movs	r2, #160
	subs	r3, r3, r1
	lsls	r2, r2, #13
	cmp	r3, r2
	bge.n	.L_02002dd0
.L_02002d5a:
	ldr	r3, [r5, #12]
	ldr	r2, [r0, #12]
	ldr	r4, [pc, #136]
	ldr	r1, [pc, #136]
	subs	r3, r3, r2
	adds	r3, r3, r4
	cmp	r3, r1
	bhi.n	.L_02002dd0
	ldr	r3, [r5, #16]
	ldr	r0, [r0, #16]
	subs	r2, r3, r0
	cmp	r2, #0
	blt.n	.L_02002d7e
	movs	r3, #160
	lsls	r3, r3, #13
	cmp	r2, r3
	blt.n	.L_02002d88
	b.n	.L_02002dd0
.L_02002d7e:
	movs	r4, #160
	subs	r3, r0, r3
	lsls	r4, r4, #13
.L_02002d84:
	cmp	r3, r4
	bge.n	.L_02002dd0
.L_02002d88:
	movs	r3, #2
	strh	r3, [r6, #4]
	ldrh	r3, [r6, #10]
	movs	r0, #170
	lsls	r0, r0, #1
	adds	r3, #2
	adds	r2, r7, r0
	str	r5, [r6, #108]
	strh	r3, [r2, #0]
	b.n	.L_02002dd0
.L_02002d9c:
	cmp	r3, #1
	bne.n	.L_02002dd0
	adds	r3, r5, #0
	adds	r3, #91
	movs	r2, #0
	strb	r2, [r3, #0]
	ldr	r3, [r5, #24]
	cmp	r3, #0
	ble.n	.L_02002dc2
	ldr	r2, [pc, #64]
	adds	r0, r5, #0
	adds	r3, r3, r2
	str	r3, [r5, #24]
	ldr	r3, [r5, #28]
	adds	r3, r3, r2
	str	r3, [r5, #28]
	bl 0x0200a870
	b.n	.L_02002dd0
.L_02002dc2:
	str	r2, [r5, #16]
	str	r2, [r5, #12]
	str	r2, [r5, #8]
	str	r2, [r5, #44]
	str	r2, [r5, #40]
	str	r2, [r5, #36]
	str	r2, [r5, #108]
.L_02002dd0:
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200254c
	.4byte 0x0007ffff
	.4byte 0x001ffffe
	.2byte 0xf000
	.2byte 0xffff
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	ldr	r6, [sp, #24]
	adds	r5, r1, #0
	mov	r9, r2
	mov	sl, r3
	bl 0x0200b1f0
	mov	r8, r0
	adds	r0, r5, #0
	bl 0x0200b1f0
	adds	r5, r0, #0
	mov	r0, r8
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #16]
	ldr	r1, [r0, #8]
	adds	r0, r5, #0
	bl 0x0200b138
	adds	r0, r5, #0
	adds	r1, r6, #0
.L_02002e28:
	bl 0x0200b120
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200b188
	adds	r3, r5, #0
	adds	r3, #85
	movs	r6, #0
	strb	r6, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200b118
	adds	r3, r5, #0
	mov	r2, r8
	adds	r3, #100
	str	r2, [r5, #104]
	mov	r0, sl
	strh	r6, [r3, #0]
	adds	r3, #2
	strh	r0, [r3, #0]
	mov	r2, r9
	subs	r3, #4
	strb	r2, [r3, #0]
	ldr	r3, [pc, #12]
	str	r3, [r5, #108]
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, pc}
	.2byte 0xaad5
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	fp, r0
	mov	r3, fp
	adds	r3, #98
	ldrb	r0, [r3, #0]
	sub	sp, #12
	bl 0x0200b1f0
	mov	r3, fp
	adds	r7, r0, #0
	adds	r3, #102
	movs	r2, #0
	ldrsh	r6, [r3, r2]
	ldr	r3, [r7, #80]
	mov	r2, fp
	mov	r9, r3
	ldr	r3, [r2, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	movs	r0, #128
	ldr	r3, [r2, #12]
	ldr	r2, [pc, #88]
	adds	r1, r6, #0
	adds	r3, r3, r2
	str	r3, [r5, #4]
	mov	r2, fp
	ldr	r3, [r2, #16]
	lsls	r0, r0, #14
	adds	r2, r5, #0
	str	r3, [r5, #8]
	bl 0x0200b0e0
	ldr	r3, [pc, #60]
	movs	r2, #0
	mov	sl, r3
	adds	r3, r7, #0
	mov	r8, r2
	adds	r3, #85
	mov	r2, sl
	strh	r6, [r7, #6]
	strb	r2, [r3, #0]
	adds	r0, r7, #0
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	bl 0x0200b138
	ldr	r3, [pc, #40]
	mov	r2, sl
	str	r3, [r7, #108]
	adds	r3, r7, #0
	adds	r3, #90
	strb	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #6
	str	r3, [r7, #52]
	ldr	r3, [pc, #20]
	mov	r2, r9
	adds	r6, r6, r3
	b.n	.L_02002f08
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0xfff40000
	.4byte 0x0200aae9
	.2byte 0xc000
	.2byte 0xffff
.L_02002f08:
	.2byte 0x4643
	strh	r6, [r2, #18]
	str	r3, [r7, #24]
	str	r3, [r7, #28]
	adds	r3, r7, #0
	mov	r2, r8
	adds	r3, #100
	strh	r2, [r3, #0]
	mov	r2, fp
	ldr	r3, [r2, #104]
	movs	r0, #104
	str	r3, [r7, #104]
	bl 0x0200b2b8
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r7, r5, #0
	adds	r7, #100
	movs	r3, #0
	ldrsh	r6, [r7, r3]
	cmp	r6, #0
	bne.n	.L_02002f6c
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	movs	r0, #0
	bl 0x0200b160
	movs	r3, #128
	lsls	r3, r3, #13
	adds	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #9
	str	r0, [r5, #12]
	movs	r1, #1
	adds	r0, r5, #0
	str	r6, [r5, #24]
	str	r6, [r5, #28]
	str	r3, [r5, #48]
	str	r3, [r5, #52]
	bl 0x0200b1a8
	b.n	.L_02002fd4
.L_02002f6c:
	cmp	r6, #30
	bgt.n	.L_02002f84
	cmp	r6, #30
	bne.n	.L_02002fd4
	movs	r0, #136
	bl 0x0200b2b8
	ldr	r0, [r5, #104]
	movs	r1, #2
	bl 0x0200b118
	b.n	.L_02002fd4
.L_02002f84:
	cmp	r6, #60
	bgt.n	.L_02002fac
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	ldrh	r3, [r7, #0]
	ldr	r0, [r5, #104]
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	lsrs	r3, r3, #31
	adds	r2, r2, r3
	ldr	r3, [pc, #48]
	asrs	r2, r2, #1
	ands	r2, r3
	lsls	r1, r2, #3
	subs	r1, r1, r2
	bl 0x0200b1a0
	b.n	.L_02002fd4
.L_02002fac:
	movs	r1, #1
	ldr	r0, [r5, #104]
	bl 0x0200b118
	movs	r0, #184
	bl 0x0200b2b8
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	movs	r0, #0
	bl 0x0200b160
	movs	r3, #0
	str	r0, [r5, #12]
	str	r3, [r5, #104]
	movs	r0, #0
	b.n	.L_02002fdc
	.2byte 0x0000
	.2byte 0x0001
	.2byte 0x0000
.L_02002fd4:
	ldrh	r3, [r7, #0]
	movs	r0, #1
	adds	r3, #1
	strh	r3, [r7, #0]
.L_02002fdc:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r7, r5, #0
	adds	r7, #100
	movs	r3, #0
	ldrsh	r6, [r7, r3]
	cmp	r6, #0
	bne.n	.L_0200302a
	movs	r0, #136
	bl 0x0200b2b8
	ldr	r0, [r5, #104]
	movs	r1, #2
	bl 0x0200b118
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	movs	r0, #0
	bl 0x0200b160
	movs	r3, #128
	lsls	r3, r3, #13
	adds	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #48]
	movs	r3, #128
	str	r0, [r5, #12]
	lsls	r3, r3, #8
	adds	r0, r5, #0
	movs	r1, #1
	str	r6, [r5, #24]
	str	r6, [r5, #28]
	str	r3, [r5, #52]
	bl 0x0200b1a8
	b.n	.L_02003078
.L_0200302a:
	cmp	r6, #32
	bgt.n	.L_02003052
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	ldrh	r3, [r7, #0]
	ldr	r0, [r5, #104]
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	lsrs	r3, r3, #31
	adds	r2, r2, r3
	ldr	r3, [pc, #48]
	asrs	r2, r2, #1
	ands	r2, r3
	lsls	r1, r2, #3
	subs	r1, r1, r2
	bl 0x0200b1a0
	b.n	.L_02003078
.L_02003052:
	movs	r1, #1
	ldr	r0, [r5, #104]
	bl 0x0200b118
	movs	r0, #184
	bl 0x0200b2b8
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	movs	r0, #0
	bl 0x0200b160
	movs	r3, #0
	str	r0, [r5, #12]
	str	r3, [r5, #104]
	movs	r0, #0
	b.n	.L_02003080
	.2byte 0x0001
	.2byte 0x0000
.L_02003078:
	ldrh	r3, [r7, #0]
	movs	r0, #1
	adds	r3, #1
	strh	r3, [r7, #0]
.L_02003080:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200ae6c
	movs	r3, #0
	str	r3, [r5, #8]
	str	r3, [r5, #12]
	str	r3, [r5, #16]
	str	r3, [r5, #36]
	str	r3, [r5, #40]
	str	r3, [r5, #44]
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0000
	.irp EntryTarget, 0x03000528, 0x03000508, 0x080000c1, 0x080000d1, 0x080000d9, 0x080000f9, 0x08000119, 0x08000121, 0x08000129, 0x080001b9, 0x080003c1, 0x080003c9, 0x080003d1, 0x080003d9, 0x08020071, 0x08020091, 0x080200a9, 0x080200c1, 0x080200c9, 0x080200e9, 0x08020149, 0x08020179, 0x08020199, 0x080201b1, 0x080201c1, 0x080201d9, 0x080201e9, 0x080201f1, 0x08020211, 0x08020219, 0x08020221, 0x08020229, 0x08020279, 0x08020291, 0x08020349, 0x08020391, 0x08038211, 0x080ad039, 0x080ad0f1, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8089, 0x080c80f9, 0x080c8151, 0x080c8171, 0x080c8219, 0x080c8231, 0x080c8239, 0x080c8241, 0x080c8249, 0x080c8279, 0x080c8351, 0x080c8379, 0x080c8381, 0x080c8391, 0x080c83a9, 0x080c83b9, 0x080c83c1, 0x080c8481, 0x080c84e1, 0x080c8549, 0x080c8571, 0x080c8779, 0x080c88d9, 0x080c88e9, 0x080c8919, 0x081c0011, 0x081c0079
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x0000002e
	.4byte 0x0200804d
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
	.4byte 0x00ca00c9
	.4byte 0x00cc00cb
	.4byte 0x00ce00cd
	.4byte 0xffff00cf
	.4byte 0x00190017
	.4byte 0x00170016
	.4byte 0x00160019
	.4byte 0x00190017
	.4byte 0x00170016
	.4byte 0x00160019
	.4byte 0x00190017
	.4byte 0x00170016
	.4byte 0x00160019
	.4byte 0x00190017
	.4byte 0xffff0016
	.4byte 0x001b001e
	.4byte 0x0016001f
	.4byte 0x001f0015
	.4byte 0x00120014
	.4byte 0x000d001f
	.4byte 0x001b000b
	.4byte 0x0008000b
	.4byte 0x00080018
	.4byte 0x00150005
	.4byte 0x001f001f
	.4byte 0xffff001f
	.4byte 0x00c900ce
	.4byte 0x00cb00ca
	.4byte 0x00cd00cc
	.4byte 0x00ce00cd
	.4byte 0x00ca00c9
	.4byte 0x00cc00cb
	.4byte 0x00cd00cc
	.4byte 0x00c900ce
	.4byte 0x00cb00ca
	.4byte 0x00cc00cb
	.4byte 0x00ce00cd
	.4byte 0x00ca00c9
	.4byte 0x00cb00ca
	.4byte 0x00cd00cc
	.4byte 0x00c900ce
	.4byte 0x00ca00c9
	.4byte 0x00cc00cb
	.4byte 0x00ce00cd
	.4byte 0xffff0000
	.4byte 0x000000c0
	.4byte 0x800000c0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000128
	.4byte 0x00105127
	.4byte 0x00208125
	.4byte 0x00304124
	.4byte 0x0040f126
	.4byte 0x0000011f
	.4byte 0x00101129
	.4byte 0x00202118
	.4byte 0x00301123
	.4byte 0x00401120
	.4byte 0x00501122
	.4byte 0x00601121
	.4byte 0x0070811f
	.4byte 0x0080711f
	.4byte 0x0090a11f
	.4byte 0x00a0911f
	.4byte 0x00b0c11f
	.4byte 0x00c0b11f
	.4byte 0x00d0e11f
	.4byte 0x00e0d11f
	.4byte 0x00000120
	.4byte 0x0010411f
	.4byte 0x00201125
	.4byte 0x00000121
	.4byte 0x0010611f
	.4byte 0x00201126
	.4byte 0x00000122
	.4byte 0x0010511f
	.4byte 0x00201124
	.4byte 0x00000123
	.4byte 0x0010311f
	.4byte 0x00201127
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff01a2
	.4byte 0x0200b2c8
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00020000
	.4byte 0xffff016d
	.4byte 0x00000007
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x0200b2c8
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00020000
	.4byte 0xffff019d
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00020000
	.4byte 0xffff019d
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff01a0
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00020000
	.4byte 0xffff01a0
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01028000
	.4byte 0xffff01a0
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01020000
	.4byte 0xffff01a0
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01028000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00024000
	.4byte 0xffff02a1
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019c
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
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x020082c9
	.4byte 0x10009a15
	.4byte 0xffff0008
	.4byte 0x020082e9
	.4byte 0x60009a15
	.4byte 0xffff000a
	.4byte 0x020082fd
	.4byte 0x20009a15
	.4byte 0xffff000a
	.4byte 0x0200830d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000031
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
	.4byte 0x00000002
	.4byte 0x1a300014
	.4byte 0x020086c9
	.4byte 0x00000002
	.4byte 0x1a300015
	.4byte 0x020086c9
	.4byte 0x00000002
	.4byte 0x1a300016
	.4byte 0x020086c9
	.4byte 0x00000002
	.4byte 0x1a300017
	.4byte 0x020086c9
	.4byte 0x00000002
	.4byte 0x1a300018
	.4byte 0x020086c9
	.4byte 0x00000002
	.4byte 0x1a300019
	.4byte 0x020086c9
	.4byte 0x00000002
	.4byte 0x1a30001a
	.4byte 0x020086c9
	.4byte 0x00000002
	.4byte 0x1a30001b
	.4byte 0x020086c9
	.4byte 0x00009c05
	.4byte 0xffff0001
	.4byte 0x02008abd
	.4byte 0x00000003
	.4byte 0xffff001e
	.4byte 0x020090e5
	.4byte 0x00000003
	.4byte 0xffff001f
	.4byte 0x02009155
	.4byte 0x00000003
	.4byte 0xffff0020
	.4byte 0x020091c5
	.4byte 0x00000003
	.4byte 0xffff0021
	.4byte 0x02009235
	.4byte 0x00000002
	.4byte 0x0a2e0022
	.4byte 0x020093a1
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00003000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00003000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xffffe800
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffd000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffd000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00001800
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000c
	.4byte 0xc0010000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
