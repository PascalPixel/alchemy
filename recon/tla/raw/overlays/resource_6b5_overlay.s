.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x0200945d, 0x02008039, 0x0200805d, 0x02008065, 0x020093e5, 0x02008041, 0x020098c9
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9bc4
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	push	{r5, r6, lr}
	ldr	r5, [pc, #16]
	ldrh	r6, [r5, #0]
	strh	r5, [r5, #0]
	bl 0x0200991c
	bl 0x02009914
	strh	r6, [r5, #0]
	pop	{r5, r6, pc}
	.2byte 0x0208
	.2byte 0x0400
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9c6c
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9c70
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #144]
	movs	r5, #1
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	adds	r7, r0, #0
	negs	r5, r5
	cmp	r3, #3
	bne.n	.L_020000de
	ldr	r3, [pc, #128]
	movs	r0, #129
	ldr	r3, [r3, #0]
	lsls	r0, r0, #2
	lsls	r3, r3, #26
	adds	r0, #255
	lsrs	r5, r3, #30
	bl 0x0200995c
	b.n	.L_020000e8
.L_02000094:
	ldr	r3, [pc, #112]
	lsls	r2, r7, #2
	adds	r6, r2, r3
	cmp	r5, #0
	beq.n	.L_020000aa
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200995c
	b.n	.L_020000b4
.L_020000aa:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02009964
.L_020000b4:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02009954
	movs	r3, #1
	eors	r0, r3
	lsls	r2, r0, #1
	ldr	r3, [pc, #68]
	adds	r2, r2, r0
	lsls	r2, r2, #3
	adds	r2, r2, r3
	ldr	r3, [pc, #64]
	ldrb	r3, [r3, r7]
	lsls	r3, r3, #2
	ldr	r2, [r2, r3]
	ldr	r3, [r6, #0]
	cmp	r2, r3
	bne.n	.L_020000fa
	movs	r0, #1
	b.n	.L_020000fc
.L_020000de:
	movs	r0, #129
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x02009964
.L_020000e8:
	cmp	r5, #0
	blt.n	.L_020000fa
	movs	r0, #129
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x02009954
	cmp	r0, #0
	bne.n	.L_02000094
.L_020000fa:
	movs	r0, #0
.L_020000fc:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0300124c
	.4byte 0x04000128
	.4byte 0x02009ae4
	.4byte 0x02003874
	.4byte 0x02009afc
	.4byte 0x4a054b04
	.4byte 0x5c1b0081
	.4byte 0x588a4c04
	.4byte 0x511a009b
	.4byte 0x00004770
	.4byte 0x02009afc
	.4byte 0x02009ae4
	.2byte 0x3a74
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r2, #1
	movs	r0, #4
	ldr	r7, [r3, #108]
	mov	r8, r2
	bl 0x02009a2c
	ldr	r3, [r0, #16]
	movs	r2, #224
	lsls	r2, r2, #16
	cmp	r3, r2
	ble.n	.L_0200015c
	movs	r0, #193
	lsls	r0, r0, #2
	bl 0x02009964
.L_0200015c:
	movs	r2, #181
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #2
	beq.n	.L_02000242
	movs	r0, #0
	bl 0x0200806c
	movs	r0, #129
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x02009954
	ldr	r2, [pc, #336]
	cmp	r0, #0
	bne.n	.L_020001b0
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	cmp	r3, #25
	ble.n	.L_020001b4
	movs	r6, #0
	movs	r5, #3
.L_0200018e:
	ldr	r0, [pc, #320]
	ldr	r3, [pc, #320]
	adds	r0, r6, r0
	movs	r1, #20
	subs	r5, #1
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x3618
	cmp	r5, #0
	bge.n	.L_0200018e
	ldr	r2, [pc, #296]
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r0, #4
	bl 0x02008114
	b.n	.L_020001b4
.L_020001b0:
	movs	r3, #0
	str	r3, [r2, #0]
.L_020001b4:
	ldr	r3, [pc, #276]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_02000210
	movs	r0, #0
	bl 0x0200806c
	cmp	r0, #0
	beq.n	.L_02000202
	movs	r0, #1
	bl 0x0200806c
	cmp	r0, #0
	bne.n	.L_020001da
	movs	r0, #2
	bl 0x0200806c
	cmp	r0, #0
	beq.n	.L_02000202
.L_020001da:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200995c
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02009954
	cmp	r0, #0
	beq.n	.L_020001fc
	movs	r3, #181
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #1
	strh	r3, [r2, #0]
.L_020001fc:
	movs	r2, #1
	mov	r8, r2
	b.n	.L_02000210
.L_02000202:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009964
	movs	r3, #0
	mov	r8, r3
.L_02000210:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009954
	cmp	r0, #0
	beq.n	.L_02000242
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02009954
	cmp	r0, #0
	beq.n	.L_02000242
.L_0200022c:
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02009954
	cmp	r0, #0
.L_02000236:
	bne.n	.L_02000242
	movs	r3, #181
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #1
.L_02000240:
	strh	r3, [r2, #0]
.L_02000242:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009954
	cmp	r0, #0
	bne.n	.L_0200025e
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02009954
	cmp	r0, #0
	beq.n	.L_020002aa
.L_0200025e:
	movs	r0, #116
	adds	r0, #255
	bl 0x02009954
	cmp	r0, #0
	bne.n	.L_020002aa
	movs	r0, #0
	bl 0x0200806c
	cmp	r0, #0
	bne.n	.L_020002aa
	ldr	r3, [pc, #84]
	ldr	r3, [r3, #0]
	cmp	r3, #24
	ble.n	.L_020002aa
	movs	r3, #181
	lsls	r3, r3, #1
	movs	r0, #131
	adds	r2, r7, r3
	lsls	r0, r0, #1
	movs	r3, #2
	strh	r3, [r2, #0]
	adds	r0, #255
	bl 0x0200995c
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009964
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02009964
	movs	r0, #4
	bl 0x02008114
.L_020002aa:
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009954
	cmp	r0, #0
	beq.n	.L_020002c2
	movs	r3, #181
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #2
.L_020002c0:
	strh	r3, [r2, #0]
.L_020002c2:
	mov	r0, r8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200a2b8
	.4byte 0x02003874
	.2byte 0x0258
	.2byte 0x0300
	push	{lr}
.L_020002da:
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009954
	cmp	r0, #0
	bne.n	.L_02000338
	ldr	r2, [pc, #80]
	movs	r1, #150
	ldr	r3, [r2, #0]
	lsls	r1, r1, #1
	adds	r3, #1
	str	r3, [r2, #0]
	cmp	r3, r1
	bne.n	.L_02000302
	str	r0, [r2, #0]
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02009964
.L_02000302:
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02009954
	cmp	r0, #0
	bne.n	.L_02000338
	bl 0x02009a14
	movs	r0, #0
	bl 0x02009ab4
	ldr	r0, [pc, #36]
	bl 0x02009a64
	movs	r1, #0
	movs	r0, #8
	bl 0x02009a6c
	movs	r0, #5
	bl 0x020098dc
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200995c
	bl 0x02009a1c
.L_02000338:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200a2bc
	.2byte 0x13ea
	.2byte 0x0000
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r0, #4
	ldr	r5, [r3, #108]
	bl 0x02008114
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02009964
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009964
	bl 0x02009a14
	movs	r0, #0
	bl 0x02009ab4
	movs	r3, #181
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #0
	strh	r3, [r2, #0]
	ldr	r0, [pc, #28]
	bl 0x02009a64
	movs	r1, #0
	movs	r0, #8
	bl 0x02009a6c
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009964
	bl 0x02009a1c
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x13e3
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #170
	lsls	r2, r2, #1
	mov	fp, r2
	mov	r0, fp
	sub	sp, #48
	bl 0x02009904
	movs	r3, #225
	lsls	r3, r3, #2
.L_020003bc:
	movs	r2, #0
	mov	r9, r0
	movs	r7, #0
	mov	sl, r3
	mov	r8, r2
	b.n	.L_0200047a
.L_020003c8:
	ldrh	r3, [r3, #0]
	cmp	r3, fp
	bls.n	.L_020003d0
	b.n	.L_020004de
.L_020003d0:
	movs	r0, #1
	bl 0x020098dc
	movs	r3, #1
	negs	r3, r3
	add	sl, r3
	mov	r2, sl
	cmp	r2, #0
	blt.n	.L_020003ee
	ldr	r3, [pc, #328]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_020003f4
.L_020003ee:
	adds	r5, #1
	cmp	r5, #24
	bgt.n	.L_020004de
.L_020003f4:
	bl 0x0200993c
	ldr	r3, [pc, #308]
	cmp	r0, #0
	bne.n	.L_020003c8
	ldrh	r3, [r3, #0]
	mov	ip, r3
	cmp	ip, fp
	bne.n	.L_020004de
	movs	r2, #149
	lsls	r2, r2, #1
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02000414
	adds	r7, #1
.L_02000414:
	movs	r0, #2
	bl 0x020098dc
	ldr	r0, [pc, #280]
	mov	r1, sp
	bl 0x020099b4
	movs	r1, #0
	mov	r2, sp
	ldrh	r3, [r2, r1]
	cmp	r3, #0
	beq.n	.L_0200043a
.L_0200042c:
	adds	r1, #1
	cmp	r1, #4
	bgt.n	.L_0200043a
	lsls	r3, r1, #1
	ldrh	r3, [r2, r3]
	cmp	r3, #0
	bne.n	.L_0200042c
.L_0200043a:
	adds	r4, r1, #0
	movs	r1, #14
	cmp	r1, r4
	blt.n	.L_0200045a
	subs	r3, r6, r4
	adds	r0, r6, #0
	adds	r2, r3, #0
	adds	r0, #14
	adds	r2, #14
.L_0200044c:
	ldrb	r3, [r2, #0]
	subs	r1, #1
	strb	r3, [r0, #0]
	subs	r2, #1
	subs	r0, #1
	cmp	r1, r4
	bge.n	.L_0200044c
.L_0200045a:
	movs	r1, #0
	cmp	r1, r4
	bge.n	.L_02000472
	adds	r0, r6, #0
.L_02000462:
	lsls	r2, r1, #1
.L_02000464:
	mov	r3, sp
	ldrh	r3, [r3, r2]
	adds	r1, #1
	strb	r3, [r0, #0]
	adds	r0, #1
	cmp	r1, r4
	blt.n	.L_02000462
.L_02000472:
	movs	r3, #0
	strb	r3, [r6, #14]
	movs	r3, #1
	add	r8, r3
.L_0200047a:
	mov	r2, r8
	cmp	r2, #2
	bgt.n	.L_020004ec
	mov	r0, r8
	adds	r0, #128
	bl 0x0200994c
	adds	r6, r0, #0
	bl 0x02009934
	movs	r3, #1
	negs	r3, r3
	movs	r5, #0
	cmp	r0, r3
	bne.n	.L_020003f4
	b.n	.L_02000514
.L_0200049a:
	ldrh	r3, [r3, #0]
	movs	r2, #170
	lsls	r2, r2, #1
	cmp	r3, r2
	bgt.n	.L_020004de
	movs	r0, #1
	bl 0x020098dc
	movs	r3, #1
	negs	r3, r3
	add	sl, r3
	mov	r2, sl
	cmp	r2, #0
	blt.n	.L_020004c2
	ldr	r3, [pc, #116]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_020004c8
.L_020004c2:
	adds	r5, #1
	cmp	r5, #24
	bgt.n	.L_020004de
.L_020004c8:
	bl 0x0200993c
	ldr	r3, [pc, #96]
	cmp	r0, #0
	bne.n	.L_0200049a
	ldrh	r3, [r3, #0]
	mov	ip, r3
	movs	r3, #170
	lsls	r3, r3, #1
	cmp	ip, r3
	beq.n	.L_020004e4
.L_020004de:
	movs	r7, #1
	negs	r7, r7
	b.n	.L_02000516
.L_020004e4:
	movs	r0, #2
	bl 0x020098dc
	b.n	.L_02000516
.L_020004ec:
	mov	r0, r9
	bl 0x0200990c
	movs	r2, #170
	lsls	r2, r2, #1
	mov	fp, r2
	mov	r0, fp
	bl 0x02009904
	mov	r9, r0
	movs	r0, #1
	bl 0x020099f4
	bl 0x02009934
	movs	r3, #1
	negs	r3, r3
	movs	r5, #0
	cmp	r0, r3
	bne.n	.L_020004c8
.L_02000514:
	adds	r7, r0, #0
.L_02000516:
	mov	r0, r9
	bl 0x0200990c
	adds	r0, r7, #0
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0300124c
	.4byte 0x02005354
	.2byte 0x0c58
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x020099fc
	cmp	r0, #3
	ble.n	.L_02000546
	movs	r0, #3
.L_02000546:
	movs	r2, #0
	cmp	r2, r0
	bge.n	.L_02000564
	ldr	r1, [pc, #36]
.L_0200054e:
	movs	r4, #134
	lsls	r4, r4, #2
	adds	r3, r2, r4
	ldrb	r3, [r1, r3]
	cmp	r5, #0
	beq.n	.L_0200055e
	strh	r3, [r5, #0]
	adds	r5, #2
.L_0200055e:
	adds	r2, #1
	cmp	r2, r0
	blt.n	.L_0200054e
.L_02000564:
	cmp	r5, #0
	beq.n	.L_0200056c
	ldr	r3, [pc, #4]
	strh	r3, [r5, #0]
.L_0200056c:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x000000ff
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r5, #170
	lsls	r5, r5, #1
	adds	r0, r5, #0
	sub	sp, #32
	bl 0x02009904
	add	r3, sp, #16
	mov	r8, r3
	movs	r2, #150
	movs	r1, #0
	lsls	r2, r2, #2
	adds	r7, r0, #0
	mov	r0, r8
	mov	fp, r1
	mov	r9, r2
	bl 0x02008538
	str	r0, [sp, #4]
	movs	r6, #0
.L_020005ac:
	mov	r1, sp
	adds	r1, #8
	movs	r3, #0
	str	r1, [sp, #0]
	strb	r3, [r1, r6]
	adds	r6, #1
	cmp	r6, #7
	ble.n	.L_020005ac
	ldr	r2, [sp, #4]
	movs	r6, #0
	cmp	r6, r2
	bge.n	.L_02000690
.L_020005c4:
	mov	r1, r8
	lsls	r5, r6, #1
	movs	r3, #0
	ldrh	r0, [r1, r5]
	mov	sl, r3
	bl 0x0200994c
	movs	r2, #170
	adds	r1, r0, #0
	ldr	r3, [pc, #472]
	lsls	r2, r2, #1
	adds	r0, r7, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2395
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #2
	strb	r3, [r2, #0]
	mov	r1, r8
	ldrh	r2, [r1, r5]
	ldr	r1, [sp, #0]
	adds	r3, r6, #0
	subs	r3, #128
	strb	r3, [r1, r2]
	movs	r1, #170
	adds	r0, r7, #0
	lsls	r1, r1, #1
	bl 0x0200992c
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L_0200063c
	mov	fp, r0
	b.n	.L_0200079a
.L_0200060c:
	movs	r0, #1
	bl 0x020098dc
	movs	r3, #1
	negs	r3, r3
	add	r9, r3
	mov	r1, r9
	cmp	r1, #0
	blt.n	.L_0200062a
	ldr	r3, [pc, #404]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_0200063c
.L_0200062a:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #24
	ble.n	.L_0200063c
	movs	r1, #1
	negs	r1, r1
	mov	fp, r1
	b.n	.L_0200079a
.L_0200063c:
	bl 0x0200993c
	cmp	r0, #0
	bne.n	.L_0200060c
	movs	r0, #2
	bl 0x020098dc
	ldr	r2, [sp, #4]
	adds	r6, #1
	cmp	r6, r2
	blt.n	.L_020005c4
	b.n	.L_02000690
.L_02000654:
	movs	r0, #1
	bl 0x020098dc
	movs	r3, #1
	negs	r3, r3
	add	r9, r3
	mov	r1, r9
	cmp	r1, #0
	blt.n	.L_02000672
	ldr	r3, [pc, #332]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_02000680
.L_02000672:
	adds	r5, #1
	cmp	r5, #24
	ble.n	.L_02000680
	movs	r2, #1
	negs	r2, r2
	mov	fp, r2
	b.n	.L_0200079a
.L_02000680:
	bl 0x0200993c
	cmp	r0, #0
	bne.n	.L_02000654
	movs	r0, #2
	bl 0x020098dc
	adds	r6, #1
.L_02000690:
	cmp	r6, #2
	bgt.n	.L_020006b2
	movs	r1, #149
	lsls	r1, r1, #1
	adds	r3, r7, r1
	movs	r5, #0
	strb	r5, [r3, #0]
	adds	r0, r7, #0
	adds	r1, #42
	bl 0x0200992c
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L_02000680
	mov	fp, r0
	b.n	.L_0200079a
.L_020006b2:
	movs	r5, #170
	adds	r0, r7, #0
	lsls	r5, r5, #1
	bl 0x0200990c
	adds	r0, r5, #0
	bl 0x02009904
	adds	r7, r0, #0
	movs	r0, #0
	bl 0x020099f4
	ldr	r3, [pc, #228]
	adds	r1, r0, #0
	adds	r2, r5, #0
	adds	r0, r7, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c3c
	movs	r2, #148
	movs	r3, #0
	lsls	r2, r2, #1
	mov	r8, r3
	adds	r3, r7, r2
	ldr	r3, [r3, #0]
	movs	r1, #150
	lsls	r1, r1, #2
	movs	r6, #144
	adds	r4, #8
	mov	sl, r1
	movs	r5, #0
	lsls	r6, r6, #1
	cmp	r8, r3
	bge.n	.L_02000740
	adds	r0, r4, #0
.L_020006f8:
	ldrb	r3, [r0, #2]
	add	r2, sp, #8
	ldrb	r3, [r2, r3]
	strb	r3, [r0, #2]
	lsls	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_02000732
	ldr	r3, [r4, r6]
	adds	r1, r5, #0
	subs	r3, #1
	cmp	r5, r3
	bge.n	.L_02000722
	lsls	r3, r5, #2
	adds	r2, r3, r4
.L_02000714:
	ldr	r3, [r2, #4]
	adds	r1, #1
	stmia	r2!, {r3}
	ldr	r3, [r4, r6]
	subs	r3, #1
	cmp	r1, r3
	blt.n	.L_02000714
.L_02000722:
	movs	r3, #144
	lsls	r3, r3, #1
	adds	r2, r4, r3
	ldr	r3, [r2, #0]
	subs	r0, #4
	subs	r3, #1
	str	r3, [r2, #0]
	subs	r5, #1
.L_02000732:
	movs	r6, #144
	lsls	r6, r6, #1
	ldr	r3, [r4, r6]
	adds	r5, #1
	adds	r0, #4
	cmp	r5, r3
	blt.n	.L_020006f8
.L_02000740:
	movs	r1, #170
	lsls	r1, r1, #1
	adds	r0, r7, #0
	bl 0x0200992c
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	bne.n	.L_02000786
	mov	fp, r0
	b.n	.L_0200079a
.L_02000756:
	movs	r0, #1
	bl 0x020098dc
	movs	r2, #1
	negs	r2, r2
	add	sl, r2
	mov	r3, sl
	cmp	r3, #0
	blt.n	.L_02000774
	ldr	r3, [pc, #72]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_02000786
.L_02000774:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #24
	ble.n	.L_02000786
	movs	r3, #1
	negs	r3, r3
	mov	fp, r3
	b.n	.L_0200079a
.L_02000786:
	bl 0x0200993c
	cmp	r0, #0
	bne.n	.L_02000756
	movs	r0, #1
	bl 0x020098dc
	movs	r0, #2
	bl 0x020098dc
.L_0200079a:
	adds	r0, r7, #0
	bl 0x0200990c
	mov	r0, fp
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x03000730
	.2byte 0x124c
	.2byte 0x0300
	push	{r5, r6, lr}
	movs	r6, #0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02009954
	ldr	r3, [pc, #128]
	strb	r6, [r3, #0]
	cmp	r0, #0
	bne.n	.L_020007f2
	movs	r0, #5
	bl 0x020098dc
	bl 0x02008578
	adds	r6, r0, #0
	cmp	r6, #0
	blt.n	.L_0200081e
	movs	r0, #5
	bl 0x020098dc
	bl 0x0200839c
	adds	r6, r0, #0
	adds	r5, r6, #0
	cmp	r6, #0
	bge.n	.L_0200080e
	b.n	.L_0200081a
.L_020007f2:
	bl 0x0200839c
	adds	r6, r0, #0
	adds	r5, r6, #0
	cmp	r6, #0
	blt.n	.L_0200081e
	movs	r0, #10
	bl 0x020098dc
	bl 0x02008578
	adds	r6, r0, #0
	cmp	r6, #0
	blt.n	.L_0200081e
.L_0200080e:
	movs	r0, #252
	lsls	r0, r0, #2
	adds	r1, r5, #0
	bl 0x02009974
	adds	r6, r5, #0
.L_0200081a:
	cmp	r5, #0
	bge.n	.L_02000842
.L_0200081e:
	ldr	r1, [pc, #44]
	ldr	r0, [pc, #44]
	ldrh	r4, [r0, #0]
	strh	r0, [r0, #0]
	movs	r2, #0
	movs	r3, #128
	strb	r3, [r1, #1]
	ldr	r3, [pc, #36]
	strb	r2, [r1, #3]
	str	r2, [r3, #0]
	ldr	r3, [pc, #36]
	strb	r2, [r1, #2]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #32]
	str	r2, [r3, #0]
	ldr	r3, [pc, #32]
	strh	r2, [r3, #0]
	strh	r4, [r0, #0]
.L_02000842:
	adds	r0, r6, #0
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x020054c0
	.4byte 0x02003a70
	.4byte 0x04000208
	.4byte 0x020038d0
	.4byte 0x020036d4
	.4byte 0x020055d0
	.2byte 0x5354
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r0, #116
	movs	r2, #0
	adds	r0, #255
	mov	r8, r2
	movs	r7, #0
	mov	sl, r3
	movs	r6, #0
	bl 0x02009954
	cmp	r0, #0
	beq.n	.L_02000894
	bl 0x02009a14
	movs	r0, #0
	bl 0x02009ab4
	b.n	.L_0200098a
.L_02000894:
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02009954
	cmp	r0, #0
	bne.n	.L_020008a2
	b.n	.L_02000ba8
.L_020008a2:
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009954
	cmp	r0, #0
	beq.n	.L_020008ee
	b.n	.L_02000ba8
.L_020008b2:
	movs	r2, #181
	lsls	r2, r2, #1
	movs	r0, #131
	movs	r3, #2
	add	r2, sl
	lsls	r0, r0, #1
	strh	r3, [r2, #0]
	adds	r0, #255
	bl 0x0200995c
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009964
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02009964
	movs	r0, #4
	bl 0x02008114
	movs	r0, #128
	movs	r3, #1
	lsls	r0, r0, #2
	mov	r8, r3
	bl 0x02009964
	b.n	.L_02000978
.L_020008ee:
	bl 0x02009a14
	movs	r0, #0
	bl 0x02009ab4
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200995c
	movs	r0, #2
	bl 0x02008114
	movs	r0, #2
	bl 0x0200806c
	cmp	r0, #0
	bne.n	.L_0200096e
	ldr	r0, [pc, #524]
	movs	r1, #5
	movs	r2, #4
	movs	r3, #1
	bl 0x020099bc
	adds	r7, r0, #0
	b.n	.L_0200096e
.L_02000922:
	movs	r0, #1
	bl 0x020098dc
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	movs	r5, #0
	bl 0x02009954
	cmp	r0, #0
	bne.n	.L_0200093a
	movs	r5, #1
.L_0200093a:
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009954
	cmp	r0, #0
	beq.n	.L_0200094a
	movs	r5, #1
.L_0200094a:
	movs	r0, #2
	bl 0x0200806c
	cmp	r0, #0
	bne.n	.L_02000968
	movs	r0, #1
	bl 0x0200806c
	cmp	r0, #0
	bne.n	.L_02000968
	adds	r6, #1
	cmp	r6, #25
	ble.n	.L_0200096a
	movs	r5, #1
	b.n	.L_0200096a
.L_02000968:
	movs	r6, #0
.L_0200096a:
	cmp	r5, #0
	bne.n	.L_020008b2
.L_0200096e:
	movs	r0, #2
	bl 0x0200806c
	cmp	r0, #0
	beq.n	.L_02000922
.L_02000978:
	cmp	r7, #0
	beq.n	.L_02000984
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x020099ac
.L_02000984:
	movs	r0, #5
	bl 0x020098dc
.L_0200098a:
	mov	r2, r8
	cmp	r2, #0
	beq.n	.L_02000992
	b.n	.L_02000b8a
.L_02000992:
	movs	r1, #249
	lsls	r1, r1, #3
	movs	r0, #216
	bl 0x020098f4
	ldr	r5, [pc, #388]
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x020098ec
	movs	r0, #5
	bl 0x0200998c
	movs	r0, #8
	bl 0x020098dc
	movs	r0, #5
	bl 0x02009994
	movs	r0, #116
.L_020009ba:
	adds	r0, #255
	bl 0x02009954
	cmp	r0, #0
	beq.n	.L_02000a1a
.L_020009c4:
	ldr	r3, [pc, #352]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #8
	bl 0x02009a5c
	ldr	r0, [pc, #340]
	bl 0x02009a64
	movs	r1, #0
.L_020009de:
	movs	r0, #8
	bl 0x02009a6c
	movs	r0, #45
	bl 0x020098dc
	movs	r1, #128
	movs	r2, #128
	movs	r0, #4
	lsls	r1, r1, #9
.L_020009f2:
	lsls	r2, r2, #8
	bl 0x02009a34
	movs	r1, #216
	movs	r2, #184
	movs	r0, #4
	bl 0x02009a3c
	movs	r0, #4
	bl 0x02009a44
	movs	r0, #4
	movs	r1, #216
	movs	r2, #168
	bl 0x02009a3c
	movs	r0, #4
	bl 0x02009a44
	b.n	.L_02000b00
.L_02000a1a:
	movs	r1, #128
	movs	r2, #128
	movs	r0, #4
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009a34
	movs	r1, #216
	movs	r2, #200
	movs	r0, #4
	bl 0x02009a3c
	movs	r0, #4
	bl 0x02009a44
	movs	r1, #200
	movs	r2, #192
	lsls	r1, r1, #5
	lsls	r2, r2, #4
	movs	r0, #4
	adds	r1, #153
	adds	r2, #204
	bl 0x02009a34
	movs	r0, #4
	movs	r1, #216
	movs	r2, #168
	bl 0x02009a3c
	bl 0x020087b8
	cmp	r0, #0
	bge.n	.L_02000aec
	movs	r1, #128
	movs	r2, #128
	movs	r0, #4
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009a34
	movs	r2, #200
	movs	r1, #216
	movs	r0, #4
	bl 0x02009a3c
	movs	r0, #5
	bl 0x0200998c
	movs	r0, #8
	bl 0x020098dc
	movs	r0, #5
	bl 0x02009994
	movs	r0, #4
	bl 0x02009a44
	movs	r0, #216
	bl 0x020098fc
	movs	r0, #0
	bl 0x02008114
	movs	r0, #4
	bl 0x02008114
	movs	r1, #144
	lsls	r1, r1, #3
	adds	r0, r5, #0
	bl 0x020098e4
	movs	r1, #1
	adds	r0, r5, #0
	bl 0x02009944
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009964
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02009964
	movs	r0, #129
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x02009964
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009964
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02009964
	movs	r2, #181
	lsls	r2, r2, #1
	add	r2, sl
	movs	r3, #2
	strh	r3, [r2, #0]
	b.n	.L_02000b8a
.L_02000aec:
	movs	r1, #128
	movs	r2, #128
	movs	r0, #4
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x02009a34
	movs	r0, #4
	bl 0x02009a44
.L_02000b00:
	movs	r0, #116
	adds	r0, #255
	bl 0x02009954
	ldr	r5, [pc, #36]
	cmp	r0, #0
	beq.n	.L_02000b34
	adds	r0, r5, #0
	movs	r1, #8
	bl 0x02009a94
	adds	r0, r5, #0
	movs	r1, #9
	bl 0x02009a9c
	b.n	.L_02000b44
	.4byte 0x000013e4
	.4byte 0x02008135
	.4byte 0x02000240
	.4byte 0x000013f7
	.2byte 0x0138
	.2byte 0x0000
.L_02000b34:
	adds	r0, r5, #0
	movs	r1, #10
	bl 0x02009a94
	adds	r0, r5, #0
	movs	r1, #11
	bl 0x02009a9c
.L_02000b44:
	ldr	r3, [pc, #84]
	movs	r2, #166
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r3, r2
	movs	r2, #4
	strb	r2, [r3, #0]
	movs	r0, #1
	movs	r1, #1
	bl 0x02009a8c
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #48]
	ldr	r1, [pc, #52]
	strh	r3, [r2, #2]
	ldr	r3, [pc, #52]
	movs	r4, #217
	strh	r3, [r2, #6]
	lsls	r4, r4, #3
	strh	r1, [r2, #0]
	strh	r1, [r2, #4]
	adds	r4, #255
	movs	r1, #0
	adds	r0, r6, #0
.L_02000b74:
	ldr	r3, [pc, #44]
	adds	r2, r1, r3
	ldrb	r3, [r0, #0]
	adds	r1, #1
	adds	r0, #1
	strb	r3, [r2, #0]
	cmp	r1, r4
.L_02000b82:
	bls.n	.L_02000b74
	movs	r0, #216
	bl 0x020098fc
.L_02000b8a:
	bl 0x02009a1c
	b.n	.L_02000ba8
	.4byte 0x00000058
	.4byte 0x00000045
	.4byte 0x00000043
	.4byte 0x02000240
	.4byte 0x02003a74
	.2byte 0x8000
	.2byte 0x0201
.L_02000ba8:
	pop	{r3, r5}
.L_02000baa:
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
.L_02000bb0:
	push	{r5, r6, r7, lr}
	ldr	r6, [pc, #480]
	bl 0x02009a14
	movs	r0, #0
	bl 0x02009ab4
	ldr	r7, [pc, #472]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r7, r2
	movs	r0, #8
	ldr	r1, [r3, #0]
	movs	r2, #0
	bl 0x02009a5c
	movs	r0, #0
	bl 0x0200806c
	cmp	r0, #0
	bne.n	.L_02000be0
	movs	r0, #1
	bl 0x020098dc
.L_02000be0:
	movs	r0, #0
	bl 0x0200806c
	cmp	r0, #0
	bne.n	.L_02000c70
	movs	r0, #5
	bl 0x02008114
	bl 0x02008044
	movs	r0, #116
	adds	r0, #255
	bl 0x02009954
	cmp	r0, #0
	bne.n	.L_02000cb0
	adds	r0, r6, #5
	bl 0x02009a64
	movs	r1, #0
	movs	r0, #8
	bl 0x02009a6c
	movs	r0, #4
	movs	r1, #0
	bl 0x02009a24
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_02000c56
	movs	r0, #250
	movs	r1, #0
	lsls	r0, r0, #2
	bl 0x02009974
	movs	r0, #116
	adds	r0, #255
	bl 0x0200995c
	movs	r0, #185
	lsls	r0, r0, #1
	bl 0x02009964
.L_02000c36:
	movs	r0, #182
	lsls	r0, r0, #1
	bl 0x02009964
	movs	r0, #128
.L_02000c40:
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200995c
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #202
	adds	r3, r7, r2
.L_02000c50:
	adds	r0, r6, #7
	strh	r5, [r3, #0]
	b.n	.L_02000cb2
.L_02000c56:
	movs	r0, #116
	adds	r0, #255
	bl 0x02009964
	movs	r0, #182
	lsls	r0, r0, #1
	bl 0x0200995c
	movs	r0, #0
	bl 0x02008114
	adds	r0, r6, #6
	b.n	.L_02000cb2
.L_02000c70:
	movs	r0, #116
	adds	r0, #255
	bl 0x02009954
	cmp	r0, #0
	beq.n	.L_02000ca2
	movs	r0, #0
	bl 0x02008114
	ldr	r0, [pc, #280]
	bl 0x02009a64
	movs	r1, #0
	movs	r0, #8
	bl 0x02009a6c
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02009964
	movs	r0, #116
	adds	r0, #255
	bl 0x02009964
.L_02000ca2:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02009954
	cmp	r0, #0
	beq.n	.L_02000cc0
.L_02000cb0:
	adds	r0, r6, #3
.L_02000cb2:
	bl 0x02009a64
.L_02000cb6:
	movs	r0, #8
	movs	r1, #0
	bl 0x02009a6c
	b.n	.L_02000d8c
.L_02000cc0:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009954
	cmp	r0, #0
	bne.n	.L_02000cf2
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x02009954
	cmp	r0, #0
	bne.n	.L_02000cf2
	adds	r0, r6, #0
	bl 0x02009a64
	movs	r0, #8
	movs	r1, #0
	bl 0x02009a6c
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200995c
	b.n	.L_02000d8c
.L_02000cf2:
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200995c
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009954
	cmp	r0, #0
	beq.n	.L_02000d10
	adds	r0, r6, #2
	bl 0x02009a64
	b.n	.L_02000d16
.L_02000d10:
	adds	r0, r6, #1
	bl 0x02009a64
.L_02000d16:
	movs	r1, #0
	movs	r0, #8
	bl 0x02009a6c
	movs	r0, #4
	movs	r1, #0
	bl 0x02009a24
	cmp	r0, #0
	bne.n	.L_02000d7e
	movs	r0, #0
	bl 0x0200806c
	cmp	r0, #0
	beq.n	.L_02000d72
	movs	r0, #182
	lsls	r0, r0, #1
	bl 0x0200995c
	movs	r0, #185
	lsls	r0, r0, #1
	bl 0x0200995c
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009954
.L_02000d4e:
	cmp	r0, #0
	beq.n	.L_02000d5a
	adds	r0, r6, #3
	bl 0x02009a64
	b.n	.L_02000d60
.L_02000d5a:
	adds	r0, r6, #4
	bl 0x02009a64
.L_02000d60:
	movs	r0, #1
	bl 0x02008114
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200995c
	b.n	.L_02000cb6
.L_02000d72:
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200995c
	b.n	.L_02000d8c
.L_02000d7e:
	adds	r0, r6, #0
	bl 0x02009a64
	movs	r0, #8
	movs	r1, #0
	bl 0x02009a6c
.L_02000d8c:
	bl 0x02009a1c
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x000013ec
	.4byte 0x02000240
	.2byte 0x13f9
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	movs	r0, #4
	bl 0x02009a2c
	ldrh	r5, [r0, #6]
	bl 0x02009a14
	movs	r0, #0
	bl 0x02009ab4
	ldr	r6, [pc, #120]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	adds	r0, r7, #0
	bl 0x02009a5c
	ldr	r3, [pc, #104]
	movs	r2, #252
	lsls	r2, r2, #6
	adds	r5, r5, r3
	adds	r2, #254
	cmp	r5, r2
	bhi.n	.L_02000dee
	ldr	r3, [pc, #96]
	movs	r2, #179
	lsls	r2, r2, #2
	adds	r5, r6, r2
	mov	r8, r3
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_02000e00
	ldr	r0, [pc, #84]
	b.n	.L_02000e10
.L_02000dee:
	movs	r2, #128
	ldr	r3, [pc, #80]
	lsls	r2, r2, #2
	adds	r2, #210
	adds	r5, r6, r2
	mov	r8, r3
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02000e1e
.L_02000e00:
	bl 0x020099dc
	movs	r1, #5
	ldrh	r0, [r5, #0]
	bl 0x020099e4
	mov	r0, r8
	adds	r0, #1
.L_02000e10:
	bl 0x02009a64
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x02009a74
	b.n	.L_02000e2c
.L_02000e1e:
	ldr	r0, [pc, #40]
	bl 0x02009a64
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x02009a74
.L_02000e2c:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xffff5fff
	.4byte 0x0000145e
	.4byte 0x00001472
	.4byte 0x00001460
	.2byte 0x1473
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	sub	sp, #4
	adds	r5, r0, #0
	bl 0x02009a14
	movs	r0, #0
	bl 0x02009ab4
	ldr	r3, [pc, #216]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	adds	r0, r5, #0
	bl 0x02009a5c
	ldr	r0, [pc, #204]
	bl 0x02009a64
	adds	r0, r5, #0
	movs	r1, #0
.L_02000e78:
	bl 0x02009a74
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r1, #0
	movs	r2, #6
	movs	r3, #4
	bl 0x020099a4
	movs	r7, #1
	adds	r6, r0, #0
	movs	r5, #0
	negs	r7, r7
	b.n	.L_02000ea6
.L_02000e96:
	ldr	r3, [r1, #4]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000efe
	movs	r0, #1
	bl 0x020098dc
.L_02000ea6:
	cmp	r5, r7
	beq.n	.L_02000ec0
	adds	r0, r6, #0
	bl 0x020099cc
	movs	r3, #0
	adds	r0, r5, #0
	movs	r1, #3
	adds	r2, r6, #0
	str	r3, [sp, #0]
	bl 0x020099d4
	adds	r7, r5, #0
.L_02000ec0:
	ldr	r1, [pc, #124]
	movs	r2, #32
	ldr	r3, [r1, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000ece
	subs	r5, #1
.L_02000ece:
	ldr	r3, [r1, #12]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000eda
	adds	r5, #1
.L_02000eda:
	cmp	r5, #0
	bge.n	.L_02000ee0
	movs	r5, #0
.L_02000ee0:
	ldr	r3, [r1, #4]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000e96
.L_02000eea:
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x020099ac
	cmp	r5, #0
	blt.n	.L_02000f04
	adds	r0, r5, #0
	bl 0x02009acc
	b.n	.L_02000f08
.L_02000efe:
	movs	r5, #1
	negs	r5, r5
	b.n	.L_02000eea
.L_02000f04:
	ldr	r0, [pc, #60]
	b.n	.L_02000f0e
.L_02000f08:
	cmp	r0, #0
	beq.n	.L_02000f1c
	ldr	r0, [pc, #56]
.L_02000f0e:
	bl 0x02009a64
	movs	r0, #9
	movs	r1, #0
	bl 0x02009a74
	b.n	.L_02000f2a
.L_02000f1c:
	ldr	r0, [pc, #44]
	bl 0x02009a64
	movs	r0, #9
	movs	r1, #0
	bl 0x02009a74
.L_02000f2a:
	movs	r0, #10
	bl 0x020098dc
	bl 0x02009a1c
	add	sp, #4
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x00000e2f
	.4byte 0x03001150
	.4byte 0x00000e30
	.4byte 0x00000e31
	.2byte 0x0e32
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x02009a14
	movs	r0, #0
	bl 0x02009ab4
	adds	r0, r5, #0
	bl 0x02009abc
	bl 0x02009a1c
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	bl 0x020099fc
	adds	r5, r0, #0
	movs	r0, #185
	lsls	r0, r0, #1
	movs	r6, #3
	bl 0x02009954
	cmp	r0, #0
	bne.n	.L_02000f86
	movs	r6, #4
.L_02000f86:
	cmp	r5, r6
	ble.n	.L_02000f8c
	adds	r5, r6, #0
.L_02000f8c:
	movs	r1, #0
	cmp	r1, r5
	bge.n	.L_02000fae
.L_02000f92:
	ldr	r0, [pc, #32]
	movs	r3, #134
	lsls	r3, r3, #2
	adds	r2, r1, r3
	ldrb	r3, [r0, r2]
	cmp	r3, #255
	beq.n	.L_02000fae
	ldrb	r3, [r0, r2]
	movs	r0, #1
	cmp	r3, r7
	beq.n	.L_02000fb0
	adds	r1, #1
	cmp	r1, r5
	blt.n	.L_02000f92
.L_02000fae:
	movs	r0, #0
.L_02000fb0:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #4
	mov	r8, r0
	bl 0x02009a14
	movs	r0, #0
	bl 0x02009ab4
	ldr	r3, [pc, #320]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	mov	r0, r8
	bl 0x02009a5c
	ldr	r0, [pc, #308]
	bl 0x02009a64
	mov	r0, r8
	movs	r1, #0
	bl 0x02009a74
	ldr	r3, [pc, #296]
	movs	r2, #146
	ldr	r3, [r3, #0]
	lsls	r2, r2, #2
	movs	r5, #0
	ands	r3, r2
	mov	r9, r5
	cmp	r3, r2
	bne.n	.L_0200100a
	movs	r3, #1
	mov	r9, r3
.L_0200100a:
	movs	r0, #78
	bl 0x02009ad4
.L_02001010:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r2, #6
	movs	r0, #12
	movs	r1, #8
	movs	r3, #3
	bl 0x020099a4
	movs	r2, #1
	negs	r2, r2
	adds	r7, r0, #0
	mov	sl, r2
.L_02001028:
	cmp	r5, sl
	beq.n	.L_02001042
	adds	r0, r7, #0
	bl 0x020099cc
	movs	r3, #0
	adds	r0, r5, #0
	movs	r1, #3
	adds	r2, r7, #0
	str	r3, [sp, #0]
	bl 0x020099d4
	mov	sl, r5
.L_02001042:
	ldr	r1, [pc, #220]
	movs	r2, #32
	ldr	r3, [r1, #12]
	movs	r6, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001052
	subs	r6, #1
.L_02001052:
	ldr	r3, [r1, #12]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200106e
	movs	r6, #1
	b.n	.L_0200106e
.L_02001060:
	ldr	r2, [pc, #192]
	lsls	r3, r5, #1
	ldrh	r0, [r2, r3]
	bl 0x02009adc
	cmp	r0, #0
	bne.n	.L_020010a0
.L_0200106e:
	cmp	r6, #0
	beq.n	.L_020010a0
	adds	r5, r5, r6
	cmp	r5, #0
	bge.n	.L_0200107a
	movs	r5, #96
.L_0200107a:
	cmp	r5, #96
	bls.n	.L_02001080
	movs	r5, #0
.L_02001080:
	mov	r3, r9
	cmp	r3, #0
	bne.n	.L_020010a0
	ldr	r3, [pc, #144]
	movs	r2, #152
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_020010a0
	ldr	r3, [pc, #144]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02001060
.L_020010a0:
	ldr	r1, [pc, #124]
	movs	r2, #1
	ldr	r3, [r1, #4]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_020010be
	ldr	r3, [r1, #4]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_020010d6
	movs	r0, #1
	bl 0x020098dc
	b.n	.L_02001028
.L_020010be:
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x020099ac
	cmp	r5, #0
	blt.n	.L_020010dc
	ldr	r3, [pc, #88]
	lsls	r2, r5, #1
	ldrh	r0, [r3, r2]
	bl 0x02009ad4
	b.n	.L_02001010
.L_020010d6:
	movs	r5, #1
	negs	r5, r5
	b.n	.L_020010be
.L_020010dc:
	movs	r0, #78
	bl 0x02009ad4
	movs	r0, #120
	bl 0x020098dc
	movs	r0, #227
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009ad4
	ldr	r0, [pc, #56]
	bl 0x02009a64
	mov	r0, r8
	movs	r1, #0
	bl 0x02009a74
	movs	r0, #10
	bl 0x020098dc
	bl 0x02009a1c
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00001485
	.4byte 0x03001150
	.4byte 0x02009b02
	.4byte 0x02003860
	.2byte 0x1486
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	movs	r0, #4
	ldr	r5, [pc, #180]
	bl 0x02008f6c
	mov	r8, r0
	adds	r0, r6, #0
	bl 0x02008f6c
	adds	r7, r0, #0
	bl 0x02009a14
	movs	r0, #0
	bl 0x02009ab4
	ldr	r3, [pc, #156]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r0, r6, #0
	ldr	r1, [r3, #0]
	movs	r2, #0
	bl 0x02009a5c
	movs	r0, #193
	lsls	r0, r0, #2
	bl 0x02009954
	cmp	r0, #0
	beq.n	.L_020011ac
	movs	r0, #189
	lsls	r0, r0, #2
	bl 0x02009954
	movs	r3, #188
	lsls	r3, r3, #2
	adds	r0, r6, r3
	bl 0x02009954
	adds	r5, r0, #0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #5
	bl 0x02009954
	cmp	r0, #0
	beq.n	.L_020011a0
	cmp	r5, #0
	beq.n	.L_0200119c
	ldr	r5, [pc, #92]
	b.n	.L_020011bc
.L_0200119c:
	ldr	r5, [pc, #92]
	b.n	.L_020011bc
.L_020011a0:
	cmp	r5, #0
	beq.n	.L_020011a8
	ldr	r5, [pc, #88]
	b.n	.L_020011bc
.L_020011a8:
	ldr	r5, [pc, #88]
	b.n	.L_020011bc
.L_020011ac:
	mov	r2, r8
	cmp	r2, #0
	beq.n	.L_020011ba
	cmp	r7, #0
	bne.n	.L_020011bc
	ldr	r5, [pc, #80]
	b.n	.L_020011bc
.L_020011ba:
	ldr	r5, [pc, #80]
.L_020011bc:
	adds	r0, r5, r6
	bl 0x02009a64
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02009a74
	ldr	r3, [pc, #64]
	cmp	r5, r3
	bne.n	.L_020011e6
	cmp	r6, #5
	bne.n	.L_020011e6
	movs	r1, #8
	movs	r0, #5
	adds	r1, #255
	movs	r2, #0
	bl 0x02009a7c
	movs	r0, #10
	bl 0x02009a0c
.L_020011e6:
	bl 0x02009a1c
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0000140a
	.4byte 0x02000240
	.4byte 0x00001432
	.4byte 0x0000143a
	.4byte 0x00001442
	.4byte 0x0000144a
	.4byte 0x00001412
	.2byte 0x141a
	.2byte 0x0000
	push	{lr}
	bl 0x0200991c
	movs	r0, #2
	bl 0x02009924
	ldr	r0, [pc, #8]
	movs	r1, #1
	bl 0x02009a84
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0001
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r6, r0, #0
	cmp	r6, #14
	bne.n	.L_0200124a
	ldr	r3, [pc, #180]
	movs	r2, #192
	ldr	r3, [r3, #0]
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200124a
	movs	r0, #14
	bl 0x02008fbc
	b.n	.L_020012e8
.L_0200124a:
	bl 0x02009a14
	movs	r0, #0
	bl 0x02009ab4
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #6
	bl 0x02009954
	cmp	r0, #0
	beq.n	.L_02001276
	cmp	r6, #13
	beq.n	.L_0200126e
	cmp	r6, #14
	beq.n	.L_02001272
	ldr	r5, [pc, #132]
	b.n	.L_02001294
.L_0200126e:
	ldr	r5, [pc, #132]
	b.n	.L_02001294
.L_02001272:
	ldr	r5, [pc, #132]
	b.n	.L_02001294
.L_02001276:
	cmp	r6, #13
	beq.n	.L_02001280
	cmp	r6, #14
	beq.n	.L_02001284
	b.n	.L_02001288
.L_02001280:
	ldr	r5, [pc, #120]
	b.n	.L_0200128a
.L_02001284:
	ldr	r5, [pc, #112]
	b.n	.L_0200128a
.L_02001288:
	ldr	r5, [pc, #100]
.L_0200128a:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #6
	bl 0x0200995c
.L_02001294:
	adds	r0, r5, #0
	bl 0x02009a64
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x02009a6c
	movs	r0, #1
	bl 0x02009a0c
	ldr	r3, [pc, #80]
	cmp	r5, r3
	bne.n	.L_020012e4
	movs	r0, #14
	movs	r1, #1
	bl 0x02009a54
	movs	r1, #4
	movs	r0, #14
	bl 0x02009a4c
	movs	r0, #15
	bl 0x02009a0c
	ldr	r3, [pc, #56]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r0, #14
	movs	r2, #0
	bl 0x02009a5c
	movs	r0, #5
	bl 0x02009a0c
	movs	r0, #14
	movs	r1, #0
	bl 0x02009a6c
.L_020012e4:
	bl 0x02009a1c
.L_020012e8:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x03001150
	.4byte 0x00001482
	.4byte 0x0000146a
	.4byte 0x0000146b
	.4byte 0x00001468
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl 0x02009a14
	movs	r0, #0
	bl 0x02009ab4
	ldr	r3, [pc, #96]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r0, r6, #0
	ldr	r1, [r3, #0]
	movs	r2, #0
	bl 0x02009a5c
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x02009954
	cmp	r0, #0
	bne.n	.L_02001354
	bl 0x020099fc
	cmp	r0, #8
	bne.n	.L_0200133c
	ldr	r5, [pc, #60]
	b.n	.L_0200134a
.L_0200133c:
	bl 0x020099fc
	cmp	r0, #3
	bgt.n	.L_02001348
	ldr	r5, [pc, #52]
	b.n	.L_0200134a
.L_02001348:
	ldr	r5, [pc, #52]
.L_0200134a:
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x0200995c
	b.n	.L_0200135e
.L_02001354:
	movs	r0, #129
	lsls	r0, r0, #2
	ldr	r5, [pc, #40]
	bl 0x02009964
.L_0200135e:
	adds	r0, r5, #0
	bl 0x02009a64
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x02009a6c
	bl 0x02009a1c
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00001479
	.4byte 0x00001477
	.4byte 0x00001476
	.2byte 0x1478
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl 0x02009a14
	movs	r0, #0
	bl 0x02009ab4
	ldr	r5, [pc, #64]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	adds	r0, r6, #0
	bl 0x02009a5c
	movs	r3, #178
	lsls	r3, r3, #2
	adds	r2, r5, r3
	ldrh	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_020013c4
	adds	r0, r3, #0
	movs	r1, #5
	bl 0x020099e4
	ldr	r0, [pc, #28]
	bl 0x02009a64
	b.n	.L_020013ca
.L_020013c4:
	ldr	r0, [pc, #24]
	bl 0x02009a64
.L_020013ca:
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x02009a6c
	bl 0x02009a1c
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x00001474
	.2byte 0x1475
	.2byte 0x0000
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xa150
	.2byte 0x0200
	push	{lr}
	bl 0x020099ec
	ldr	r0, [pc, #8]
	movs	r1, #1
	bl 0x020099c4
	pop	{pc}
	.2byte 0x13e7
	.2byte 0x0000
	push	{lr}
	bl 0x020099ec
	ldr	r0, [pc, #8]
	movs	r1, #1
	bl 0x020099c4
	pop	{pc}
	.2byte 0x13e9
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r5, r0, #0
	adds	r3, #255
	sub	sp, #8
	cmp	r5, r3
	ble.n	.L_02001426
	adds	r5, r3, #0
.L_02001426:
	movs	r6, #0
.L_02001428:
	adds	r0, r5, #0
	movs	r1, #10
	bl 0x020098d4
	movs	r3, #1
	movs	r2, #16
	adds	r1, r0, #0
	subs	r2, r2, r6
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #27
	movs	r3, #8
	bl 0x02009984
	adds	r0, r5, #0
	movs	r1, #10
	bl 0x020098cc
	adds	r6, #1
	adds	r5, r0, #0
	cmp	r6, #2
	ble.n	.L_02001428
	bl 0x0200997c
	add	sp, #8
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	movs	r0, #191
	lsls	r0, r0, #1
	sub	sp, #8
	bl 0x0200995c
	ldr	r5, [pc, #160]
	movs	r1, #144
	lsls	r1, r1, #2
	movs	r2, #0
	adds	r3, r5, r1
	adds	r1, #2
	strh	r2, [r3, #0]
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	ldr	r3, [pc, #148]
	movs	r0, #2
	str	r2, [r3, #0]
	ldr	r3, [pc, #144]
	str	r2, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #85
	str	r2, [r3, #0]
	bl 0x02009924
	movs	r3, #180
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldrh	r0, [r5, #0]
	bl 0x02009414
	movs	r3, #13
	movs	r2, #10
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r2, #1
	movs	r1, #11
	movs	r0, #11
	bl 0x0200999c
	movs	r0, #4
	bl 0x02008114
	movs	r0, #1
	bl 0x020098dc
	movs	r0, #5
	bl 0x02009994
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #44]
	movs	r5, #0
	strh	r3, [r2, #8]
	ldr	r3, [pc, #44]
	strh	r3, [r2, #10]
	ldr	r3, [pc, #44]
	strh	r3, [r2, #12]
	ldr	r3, [pc, #44]
	strh	r3, [r2, #14]
.L_020014de:
	movs	r1, #188
	lsls	r1, r1, #2
	adds	r6, r5, r1
	adds	r0, r6, #0
	bl 0x02009964
	adds	r0, r5, #0
	bl 0x02008f6c
	cmp	r0, #0
	beq.n	.L_0200151c
	adds	r0, r6, #0
	bl 0x0200995c
	b.n	.L_0200151c
	.4byte 0x00000054
	.4byte 0x00000041
	.4byte 0x0000004c
	.4byte 0x0000004b
	.4byte 0x02000240
	.4byte 0x0200a2bc
	.4byte 0x0200a2b8
	.2byte 0x3a74
	.2byte 0x0200
.L_0200151c:
	adds	r5, #1
	cmp	r5, #7
	ble.n	.L_020014de
	ldr	r6, [pc, #720]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r6, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #8
	beq.n	.L_02001534
	b.n	.L_02001640
.L_02001534:
	bl 0x02009a14
	movs	r0, #0
	bl 0x02009ab4
	bl 0x02009aa4
	bl 0x02009aac
	movs	r0, #5
	bl 0x02008114
	movs	r3, #177
	lsls	r3, r3, #2
	adds	r2, r6, r3
	ldrh	r3, [r2, #0]
	movs	r1, #128
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r1, r1, #2
	adds	r1, #202
	adds	r2, r6, r1
	ldrh	r3, [r2, #0]
	movs	r0, #254
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r0, r0, #2
	bl 0x0200996c
	lsls	r0, r0, #24
	asrs	r6, r0, #24
	lsls	r3, r6, #1
	adds	r5, r3, #2
	cmp	r5, #14
	ble.n	.L_0200157c
	movs	r5, #14
.L_0200157c:
	movs	r0, #250
	lsls	r0, r0, #2
	bl 0x0200996c
	cmp	r0, #2
	bne.n	.L_02001598
	movs	r0, #250
	lsls	r0, r0, #2
	movs	r1, #0
	bl 0x02009974
	adds	r6, #1
	adds	r5, #1
	b.n	.L_020015a2
.L_02001598:
	adds	r1, r0, #1
	movs	r0, #250
	lsls	r0, r0, #2
	bl 0x02009974
.L_020015a2:
	ldr	r7, [pc, #592]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #8
	bl 0x02009a5c
	ldr	r0, [pc, #576]
	adds	r0, r5, r0
	bl 0x02009a64
	movs	r1, #0
	movs	r0, #8
	bl 0x02009a6c
	movs	r0, #4
	movs	r1, #0
	bl 0x02009a24
	cmp	r0, #0
	bne.n	.L_020015e2
.L_020015d0:
	cmp	r6, #90
	ble.n	.L_020015d6
	movs	r6, #90
.L_020015d6:
	movs	r0, #254
	lsls	r0, r0, #2
	adds	r1, r6, #0
	bl 0x02009974
	b.n	.L_020016e4
.L_020015e2:
	movs	r0, #116
	adds	r0, #255
	bl 0x02009964
	movs	r0, #254
	movs	r1, #1
	lsls	r0, r0, #2
	negs	r1, r1
	bl 0x02009974
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #202
	adds	r5, r7, r3
	ldrh	r0, [r5, #0]
	movs	r1, #5
	bl 0x020099e4
	movs	r1, #178
	lsls	r1, r1, #2
	adds	r2, r7, r1
	ldrh	r5, [r5, #0]
	ldrh	r3, [r2, #0]
	cmp	r3, r5
	bcs.n	.L_0200162a
	strh	r5, [r2, #0]
	ldr	r0, [pc, #484]
	bl 0x02009a64
	movs	r1, #0
	movs	r0, #8
	bl 0x02009a6c
	bl 0x02009400
	b.n	.L_02001638
.L_0200162a:
	ldr	r0, [pc, #468]
	bl 0x02009a64
	movs	r0, #8
	movs	r1, #0
	bl 0x02009a6c
.L_02001638:
	movs	r0, #0
	bl 0x02008114
	b.n	.L_020016e4
.L_02001640:
	cmp	r3, #9
	bne.n	.L_020016ea
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #198
	adds	r2, r6, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	bl 0x02009a14
	movs	r0, #0
	bl 0x02009ab4
	bl 0x02009aa4
	bl 0x02009aac
	movs	r0, #5
	bl 0x02008114
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r6, r1
	ldr	r1, [r3, #0]
	movs	r0, #8
	movs	r2, #0
	bl 0x02009a5c
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #202
	adds	r5, r6, r2
	ldrh	r0, [r5, #0]
	movs	r1, #5
	bl 0x020099e4
	movs	r3, #178
	lsls	r3, r3, #2
	adds	r2, r6, r3
	ldrh	r5, [r5, #0]
	ldrh	r3, [r2, #0]
	cmp	r3, r5
	bcs.n	.L_020016ae
	strh	r5, [r2, #0]
	ldr	r0, [pc, #352]
	bl 0x02009a64
	movs	r1, #0
	movs	r0, #8
	bl 0x02009a6c
	bl 0x02009400
	b.n	.L_020016bc
.L_020016ae:
	ldr	r0, [pc, #340]
	bl 0x02009a64
	movs	r0, #8
	movs	r1, #0
	bl 0x02009a6c
.L_020016bc:
	ldr	r3, [pc, #308]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #202
	adds	r3, r3, r1
	movs	r2, #0
	movs	r0, #116
	strh	r2, [r3, #0]
	adds	r0, #255
	bl 0x02009964
	movs	r0, #254
	movs	r1, #1
	lsls	r0, r0, #2
	negs	r1, r1
	bl 0x02009974
	movs	r0, #0
.L_020016e0:
	bl 0x02008114
.L_020016e4:
	bl 0x02009a1c
	b.n	.L_02001876
.L_020016ea:
	cmp	r3, #10
	bne.n	.L_02001796
	bl 0x02009a14
	movs	r0, #0
	bl 0x02009ab4
	bl 0x02009aa4
	bl 0x02009aac
	movs	r0, #0
	bl 0x02008114
	movs	r0, #4
	bl 0x02008114
	movs	r0, #250
	lsls	r0, r0, #2
	bl 0x02009954
	cmp	r0, #0
	beq.n	.L_0200174c
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r0, #250
	ldr	r5, [r3, #108]
	lsls	r0, r0, #2
	bl 0x02009964
	movs	r3, #181
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r0, #193
	movs	r3, #2
	strh	r3, [r2, #0]
	lsls	r0, r0, #2
	bl 0x02009964
	movs	r0, #20
	bl 0x020098dc
	bl 0x02008044
	movs	r0, #0
	bl 0x02008114
	movs	r0, #4
	b.n	.L_020016e0
.L_0200174c:
	movs	r1, #179
	lsls	r1, r1, #2
.L_02001750:
	adds	r2, r6, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #210
	adds	r2, r6, r3
	ldrh	r3, [r2, #0]
	adds	r1, r3, #1
	strh	r1, [r2, #0]
	movs	r2, #180
	lsls	r2, r2, #2
	adds	r5, r6, r2
	ldrh	r2, [r5, #0]
	lsls	r3, r1, #16
	lsrs	r3, r3, #16
	cmp	r2, r3
	bcs.n	.L_02001778
	strh	r1, [r5, #0]
.L_02001778:
	ldrh	r0, [r5, #0]
	bl 0x02009414
	bl 0x020093ec
	movs	r0, #193
.L_02001784:
	lsls	r0, r0, #2
	bl 0x0200995c
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #5
	bl 0x0200995c
	b.n	.L_020016e4
.L_02001796:
	cmp	r3, #11
	bne.n	.L_02001808
	bl 0x02009a14
	movs	r0, #0
	bl 0x02009ab4
	bl 0x02009aa4
	bl 0x02009aac
	movs	r0, #0
	bl 0x02008114
	movs	r0, #4
	bl 0x02008114
	movs	r0, #116
	adds	r0, #255
	bl 0x02009954
	cmp	r0, #0
	bne.n	.L_020017e0
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #206
	adds	r3, r6, r1
	ldrh	r2, [r3, #0]
	adds	r2, #1
	strh	r2, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #210
	adds	r3, r6, r2
	strh	r0, [r3, #0]
	bl 0x020093ec
.L_020017e0:
	movs	r0, #193
	lsls	r0, r0, #2
	bl 0x0200995c
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #5
	bl 0x02009964
	b.n	.L_020016e4
	.4byte 0x02000240
	.4byte 0x000013fa
	.4byte 0x000013f8
	.4byte 0x000013f5
	.2byte 0x13f6
	.2byte 0x0000
.L_02001808:
	bl 0x02009914
	movs	r0, #185
	lsls	r0, r0, #1
	bl 0x02009964
	movs	r0, #254
	movs	r1, #1
.L_02001818:
	lsls	r0, r0, #2
	negs	r1, r1
	bl 0x02009974
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #74
	adds	r5, r6, r3
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02001862
	bl 0x02009a14
	movs	r0, #0
	bl 0x02009ab4
	bl 0x02009aa4
	bl 0x02009aac
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r6, r1
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #8
	bl 0x02009a5c
	ldr	r0, [pc, #100]
	bl 0x02009a64
	movs	r0, #8
	movs	r1, #0
.L_0200185a:
	bl 0x02009a74
	bl 0x02009a1c
.L_02001862:
	ldr	r3, [pc, #88]
	movs	r2, #0
	strb	r2, [r5, #0]
	movs	r0, #0
	strb	r2, [r3, #0]
	bl 0x02008114
	movs	r0, #4
	bl 0x02008114
.L_02001876:
	ldr	r5, [pc, #72]
	movs	r1, #144
	lsls	r1, r1, #3
	adds	r0, r5, #0
	bl 0x020098e4
	movs	r1, #1
	adds	r0, r5, #0
	bl 0x02009944
	ldr	r3, [pc, #56]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #8
	bne.n	.L_020018a6
	movs	r0, #116
	adds	r0, #255
	bl 0x02009954
	cmp	r0, #0
	bne.n	.L_020018b0
.L_020018a6:
	movs	r0, #1
	bl 0x02009a04
	bl 0x02009ac4
.L_020018b0:
	movs	r0, #0
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x000013e5
	.4byte 0x03001200
	.4byte 0x02008135
	.2byte 0x0240
	.2byte 0x0200
	movs	r0, #0
.L_020018ca:
	bx	lr
	.irp EntryTarget, 0x03000528, 0x03000508, 0x080000c1, 0x080000d1, 0x080000d9, 0x08000149, 0x08000151, 0x08000171, 0x08000179, 0x08000301, 0x08000371, 0x08000379, 0x08000381, 0x08000389, 0x080003a9, 0x080003b9, 0x080003c1, 0x080003c9, 0x080003d1, 0x080003d9, 0x080003e9, 0x080003f1, 0x08020121, 0x08020179, 0x08020199, 0x080201a1, 0x080201e9, 0x08038011, 0x08038019, 0x08038021, 0x08038039, 0x08038041, 0x08038061, 0x080380a1, 0x08038119, 0x08038121, 0x080382e1, 0x080ad001, 0x080ad0f1, 0x080ad209, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80d1, 0x080c80f1, 0x080c8119, 0x080c8141, 0x080c8159, 0x080c8181, 0x080c8189, 0x080c81a1, 0x080c8211, 0x080c8269, 0x080c8281, 0x080c8291, 0x080c8299, 0x080c83a9, 0x080c83b9, 0x080c84e1, 0x08108011, 0x08118109, 0x08118111, 0x081c0011, 0x081c0091
	overlay_veneer \EntryTarget
	.endr
	.section .rodata,"a",%progbits
	.4byte 0x43314773
	.4byte 0x33323130
	.4byte 0x31434241
	.4byte 0x32454443
	.4byte 0x33474645
	.4byte 0x434d4753
	.4byte 0x01010100
	.4byte 0x02c50001
	.4byte 0x004a0044
	.4byte 0x000102d7
	.4byte 0x00030002
	.4byte 0x00050004
	.4byte 0x00070006
	.4byte 0x00090008
	.4byte 0x000b000a
	.4byte 0x000d000c
	.4byte 0x000f000e
	.4byte 0x00110010
	.4byte 0x00140012
	.4byte 0x00160015
	.4byte 0x00180017
	.4byte 0x001a0019
	.4byte 0x001c001b
	.4byte 0x001e001d
	.4byte 0x0022001f
	.4byte 0x00240023
	.4byte 0x00260025
	.4byte 0x00280027
	.4byte 0x002a0029
	.4byte 0x002c002b
	.4byte 0x004c0037
	.4byte 0x00320031
	.4byte 0x00340033
	.4byte 0x00390038
	.4byte 0x00360035
	.4byte 0x003b003a
	.4byte 0x003d003c
	.4byte 0x003f003e
	.4byte 0x00410040
	.4byte 0x00470046
	.4byte 0x00490048
	.4byte 0x004b0043
	.4byte 0x02bd02bc
	.4byte 0x02bf02be
	.4byte 0x02c102c0
	.4byte 0x02c302c2
	.4byte 0x02c602c4
	.4byte 0x02d102d0
	.4byte 0x02d302d2
	.4byte 0x02d502d4
	.4byte 0x02d802d6
	.4byte 0x02da02d9
	.4byte 0x02e602e5
	.4byte 0x02ee02e7
	.4byte 0x02f002ef
	.4byte 0x02f202f1
	.4byte 0xffff000a
	.4byte 0x00000150
	.4byte 0xc00000a4
	.4byte 0x00100000
	.4byte 0x01a00000
	.4byte 0x00000136
	.4byte 0xffff000b
	.4byte 0x00000150
	.4byte 0xc00000a4
	.4byte 0x00100000
	.4byte 0x01a00000
	.4byte 0x00000136
	.4byte 0xffff0008
	.4byte 0x000000d8
	.4byte 0x800000c8
	.4byte 0x00100000
	.4byte 0x01a00000
	.4byte 0x00000136
	.4byte 0xffff0009
	.4byte 0x000000d8
	.4byte 0x800000c8
	.4byte 0x00100000
	.4byte 0x01a00000
	.4byte 0x00000136
	.4byte 0xffff0001
	.4byte 0x000000d8
	.4byte 0xc0000120
	.4byte 0x00100000
	.4byte 0x01a00000
	.4byte 0x00000136
	.4byte 0xffff0000
	.4byte 0x000000d8
	.4byte 0xc00000d0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000001ff
	.4byte 0xffff00be
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x01004000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x00014000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00014000
	.4byte 0xffff00c0
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00018000
	.4byte 0xffff0158
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x017c0000
	.4byte 0x00014000
	.4byte 0xffff0056
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x00012000
	.4byte 0xffff0078
	.4byte 0x00000001
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x011c0000
	.4byte 0x00010000
	.4byte 0xffff00f2
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00014000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00018000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x00600000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00010000
	.4byte 0xffff00e8
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00010000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x00400000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00014000
	.4byte 0xffff00ea
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x0001c000
	.4byte 0xffff00c4
	.4byte 0x00000001
	.4byte 0x00400000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0001c000
	.4byte 0xffff00bc
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00014000
	.4byte 0xffff00df
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0001c000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00010000
	.4byte 0xffff00c0
	.4byte 0x00000008
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00010000
	.4byte 0x10000000
	.4byte 0x00000001
	.4byte 0x01440000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00016000
	.4byte 0x10010001
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x0001e000
	.4byte 0x10020002
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0001c000
	.4byte 0x10030003
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x006c0000
	.4byte 0x00014000
	.4byte 0x10060006
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00018000
	.4byte 0x10070007
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00014000
	.4byte 0x10050005
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00010000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00be
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x01004000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x00014000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01300000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00010000
	.4byte 0xffff00c0
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00010000
	.4byte 0x00000158
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00010000
	.4byte 0xffff0056
	.4byte 0x00000001
	.4byte 0x00400000
	.4byte 0x00000000
	.4byte 0x005c0000
	.4byte 0x00014000
	.4byte 0xffff0078
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00740000
	.4byte 0x00010000
	.4byte 0xffff00f2
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00014000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00014000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00014000
	.4byte 0x000100e8
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x006c0000
	.4byte 0x00014000
	.4byte 0x000200e7
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x009c0000
	.4byte 0x0001c000
	.4byte 0x000300ea
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00018000
	.4byte 0xffff00c4
	.4byte 0x00000001
	.4byte 0x00400000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0001c000
	.4byte 0xffff00bc
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00014000
	.4byte 0xffff00df
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00014000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00010000
	.4byte 0xffff00c0
	.4byte 0x00000008
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00010000
	.4byte 0x10000000
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00010000
	.4byte 0x10010001
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x006c0000
	.4byte 0x00014000
	.4byte 0x10020002
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x009c0000
	.4byte 0x0001c000
	.4byte 0x10030003
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00018000
	.4byte 0x10060006
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00018000
	.4byte 0x10070007
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00010000
	.4byte 0x10050005
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x007c0000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x02009131
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x02009131
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte 0x02009131
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte 0x02009131
	.4byte 0x00000000
	.4byte 0xffff0005
	.4byte 0x02009131
	.4byte 0x00000000
	.4byte 0xffff0006
	.4byte 0x02009131
	.4byte 0x00000000
	.4byte 0xffff0007
	.4byte 0x02009131
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008bb1
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008da1
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008f51
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000013eb
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000146c
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0200922d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0200922d
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008e4d
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x02009305
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x0000147a
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x0000147c
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x0000147d
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x0000147e
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x0000147b
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x02009389
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00001482
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x02008fbd
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00001453
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x02008865
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x02009211
	.4byte 0x00000006
	.4byte 0xffff0001
	.4byte 0x020082d9
	.4byte 0x00000006
	.4byte 0xffff0002
	.4byte 0x02008345
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
