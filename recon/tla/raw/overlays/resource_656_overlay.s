.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02009abd, 0x02008039, 0x020080b1, 0x020080b9, 0x020084f5, 0x02008079, 0x02009c39
	overlay_veneer \EntryTarget
	.endr
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_02000050
	ldr	r0, [pc, #24]
	b.n	.L_0200005c
.L_02000050:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_0200005a
	ldr	r0, [pc, #24]
	b.n	.L_0200005c
.L_0200005a:
	ldr	r0, [pc, #24]
.L_0200005c:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000028
	.4byte 0x0200b084
	.4byte 0x00000029
	.4byte 0x0200b12c
	.2byte 0xb054
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_02000090
	ldr	r0, [pc, #20]
	b.n	.L_0200009a
.L_02000090:
	ldr	r3, [pc, #20]
	movs	r0, #0
	cmp	r2, r3
	bne.n	.L_0200009a
	ldr	r0, [pc, #16]
.L_0200009a:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000028
	.4byte 0x0200b234
	.4byte 0x00000029
	.2byte 0xb254
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb274
	.2byte 0x0200
	push	{lr}
	ldr	r2, [pc, #300]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r2, r0
	movs	r0, #0
	ldrsh	r1, [r3, r0]
	ldr	r3, [pc, #292]
	cmp	r1, r3
	bne.n	.L_02000168
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	subs	r3, #3
	cmp	r3, #27
	bls.n	.L_020000de
	b.n	.L_020001e4
.L_020000de:
	ldr	r2, [pc, #272]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0x0000
	.4byte 0x02008158
	.4byte 0x02008158
	.4byte 0x02008158
	.4byte 0x020081e4
	.4byte 0x020081e4
	.4byte 0x020081e4
	.4byte 0x020081e4
	.4byte 0x020081e4
	.4byte 0x020081e4
	.4byte 0x020081e4
	.4byte 0x020081e4
	.4byte 0x020081e4
	.4byte 0x02008160
	.4byte 0x02008160
	.4byte 0x02008160
	.4byte 0x02008160
	.4byte 0x020081e4
	.4byte 0x020081e4
	.4byte 0x020081e4
	.4byte 0x020081e4
	.4byte 0x020081e4
	.4byte 0x020081e4
	.4byte 0x0200815c
	.4byte 0x0200815c
	.4byte 0x0200815c
	.4byte 0x0200815c
	.4byte 0x02008164
	.4byte 0x02008164
	.4byte 0xe0444826
	.4byte 0xe0424826
	.4byte 0xe0404826
	.2byte 0x4826
	.2byte 0xe03e
.L_02000168:
	ldr	r3, [pc, #152]
	cmp	r1, r3
	bne.n	.L_020001e4
	movs	r0, #241
	lsls	r0, r0, #1
	adds	r3, r2, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	subs	r3, #1
	cmp	r3, #19
	bhi.n	.L_020001e0
	ldr	r2, [pc, #136]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0x0000
	.4byte 0x020081d8
	.4byte 0x020081d8
	.4byte 0x020081d8
	.4byte 0x020081d8
	.4byte 0x020081e0
	.4byte 0x020081dc
	.4byte 0x020081e0
	.4byte 0x020081e0
	.4byte 0x020081e0
	.4byte 0x020081dc
	.4byte 0x020081d8
	.4byte 0x020081d8
	.4byte 0x020081d8
	.4byte 0x020081d8
	.4byte 0x020081d8
	.4byte 0x020081d8
	.4byte 0x020081d8
	.4byte 0x020081d8
	.4byte 0x020081e0
	.4byte 0x020081d8
	.4byte 0xe004480c
	.2byte 0x480c
	.2byte 0xe002
.L_020001e0:
	ldr	r0, [pc, #48]
	b.n	.L_020001e6
.L_020001e4:
	ldr	r0, [pc, #48]
.L_020001e6:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000028
	.4byte 0x020080e8
	.4byte 0x0200b398
	.2byte 0xb410
	.2byte 0x0200
	push	{r5, r6, lr}
	lsls	r0, r0, #8
	push	{r4, r7, lr}
	lsls	r0, r0, #8
	movs	r1, r5
	movs	r0, r0
	strh	r0, [r1, #12]
	lsls	r0, r0, #8
	.2byte 0xb710
	lsls	r0, r0, #8
	.2byte 0xb7d0
	lsls	r0, r0, #8
	.2byte 0xb848
	lsls	r0, r0, #8
	.2byte 0xb380
	lsls	r0, r0, #8
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	sub	sp, #8
	mov	r9, r3
	bl 0x0200ac68
	movs	r0, #0
	bl 0x0200ada8
	movs	r7, #0
	movs	r5, #8
.L_0200023e:
	adds	r0, r5, #0
	bl 0x0200ac78
	cmp	r0, #0
	beq.n	.L_02000250
	adds	r2, r0, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
.L_02000250:
	adds	r5, #1
	cmp	r5, #63
	bls.n	.L_0200023e
	ldr	r0, [pc, #196]
	movs	r5, #0
	movs	r1, #0
.L_0200025c:
	movs	r3, #170
	lsls	r3, r3, #1
	add	r3, r9
	movs	r4, #0
	ldrsh	r2, [r3, r4]
	ldrh	r3, [r0, r1]
	cmp	r2, r3
	bne.n	.L_0200026e
	adds	r7, r5, #0
.L_0200026e:
	adds	r5, #1
	adds	r1, #12
	cmp	r5, #9
	bls.n	.L_0200025c
	movs	r0, #158
	bl 0x0200ade0
	ldr	r2, [pc, #160]
	movs	r3, #133
	mov	sl, r2
	lsls	r3, r3, #2
	add	sl, r3
	mov	r4, sl
	ldr	r0, [r4, #0]
	movs	r1, #1
	bl 0x0200ad38
	lsls	r5, r7, #1
	ldr	r2, [pc, #136]
	adds	r5, r5, r7
	lsls	r5, r5, #2
	adds	r6, r5, #0
	mov	r8, r2
	adds	r6, #8
	ldrh	r2, [r2, r6]
	movs	r1, #1
	add	r6, r8
	movs	r0, #2
	ldrh	r3, [r6, #2]
	str	r1, [sp, #0]
	str	r0, [sp, #4]
	movs	r1, #0
	movs	r0, #0
	bl 0x0200ac30
	ldrh	r2, [r6, #2]
	mov	r3, r8
	adds	r5, #4
	ldr	r0, [r3, r5]
	adds	r2, #60
	ldrh	r1, [r6, #0]
	bl 0x0200ac28
	mov	r4, sl
	ldr	r0, [r4, #0]
	movs	r1, #2
	bl 0x0200ace0
	mov	r2, sl
	ldr	r0, [r2, #0]
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x0200ac80
	mov	r3, sl
	ldr	r0, [r3, #0]
	cmp	r7, #12
	bne.n	.L_020002f2
	movs	r2, #4
	movs	r1, #0
	negs	r2, r2
	bl 0x0200acc0
	b.n	.L_020002fc
.L_020002f2:
	movs	r2, #4
	movs	r1, #2
	negs	r2, r2
	bl 0x0200acb8
.L_020002fc:
	movs	r0, #4
	bl 0x0200ac60
	movs	r3, #170
	lsls	r3, r3, #1
	add	r3, r9
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	bl 0x0200ad70
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x0200b904
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	sub	sp, #8
	ldr	r7, [r3, #108]
	bl 0x0200ac68
	movs	r0, #0
	bl 0x0200ada8
	movs	r6, #0
	movs	r5, #8
.L_02000342:
	adds	r0, r5, #0
	bl 0x0200ac78
	cmp	r0, #0
	beq.n	.L_02000354
	adds	r2, r0, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
.L_02000354:
	adds	r5, #1
	cmp	r5, #63
	bls.n	.L_02000342
	ldr	r0, [pc, #212]
	movs	r5, #0
	movs	r1, #0
.L_02000360:
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r4, #0
	ldrsh	r2, [r3, r4]
	ldrh	r3, [r0, r1]
	cmp	r2, r3
	bne.n	.L_02000372
	adds	r6, r5, #0
.L_02000372:
	adds	r5, #1
	adds	r1, #12
	cmp	r5, #3
	bls.n	.L_02000360
	cmp	r6, #1
	bhi.n	.L_0200039c
	movs	r0, #132
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_0200039c
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #228
	bl 0x0200abd8
	cmp	r0, #0
	bne.n	.L_0200039c
	adds	r6, #4
.L_0200039c:
	movs	r0, #158
	bl 0x0200ade0
	ldr	r2, [pc, #144]
	movs	r3, #133
	mov	sl, r2
	lsls	r3, r3, #2
	add	sl, r3
	mov	r4, sl
	ldr	r0, [r4, #0]
	movs	r1, #1
	bl 0x0200ad38
	lsls	r5, r6, #1
	ldr	r2, [pc, #116]
	adds	r5, r5, r6
	lsls	r5, r5, #2
	adds	r6, r5, #0
	mov	r8, r2
	adds	r6, #8
	ldrh	r2, [r2, r6]
	movs	r1, #1
	add	r6, r8
	movs	r0, #2
	ldrh	r3, [r6, #2]
	str	r1, [sp, #0]
	str	r0, [sp, #4]
	movs	r1, #0
	movs	r0, #0
	bl 0x0200ac30
	ldrh	r2, [r6, #2]
	mov	r3, r8
	adds	r5, #4
	ldr	r0, [r3, r5]
	ldrh	r1, [r6, #0]
	adds	r2, #60
	bl 0x0200ac28
	mov	r4, sl
	ldr	r0, [r4, #0]
	movs	r1, #2
	bl 0x0200ace0
	mov	r2, sl
	ldr	r0, [r2, #0]
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x0200ac80
	mov	r3, sl
	movs	r2, #4
	negs	r2, r2
	ldr	r0, [r3, #0]
	movs	r1, #2
	bl 0x0200acb8
	movs	r0, #4
	bl 0x0200ac60
	movs	r4, #170
	lsls	r4, r4, #1
	adds	r3, r7, r4
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200ad70
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x0200b97c
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #130
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_020004ea
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #108]
	ldr	r3, [pc, #156]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	bl 0x0200ac78
	adds	r5, r0, #0
	bl 0x0200ac68
	movs	r0, #0
	bl 0x0200ada8
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	movs	r3, #0
	negs	r2, r2
	negs	r0, r0
	negs	r1, r1
	bl 0x0200ad60
	ldr	r0, [r6, #0]
	movs	r1, #2
	bl 0x0200ace0
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r6, #0]
	lsls	r2, r2, #7
	lsls	r1, r1, #8
	bl 0x0200ac80
	ldr	r0, [r5, #8]
	asrs	r2, r0, #16
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_0200049c
	adds	r3, #15
.L_0200049c:
	asrs	r3, r3, #4
	lsls	r3, r3, #4
	subs	r3, r2, r3
	movs	r1, #8
	subs	r1, r1, r3
	lsls	r1, r1, #16
	ldr	r2, [r5, #12]
	adds	r1, r1, r0
	ldr	r3, [r5, #16]
	adds	r0, r5, #0
	bl 0x0200ac18
	adds	r0, r5, #0
	bl 0x0200ac20
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r5, #6]
	ldr	r1, [pc, #44]
	ldr	r0, [r6, #0]
	bl 0x0200ac88
	movs	r0, #12
	bl 0x0200abb0
	movs	r0, #123
	bl 0x0200ade0
	bl 0x0200ad98
	bl 0x0200ada0
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200ad70
.L_020004ea:
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0xade8
	.2byte 0x0200
	push	{lr}
	ldr	r2, [pc, #300]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r2, r0
	movs	r0, #0
	ldrsh	r1, [r3, r0]
	ldr	r3, [pc, #292]
	cmp	r1, r3
	bne.n	.L_020005a4
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	subs	r3, #3
	cmp	r3, #27
	bhi.n	.L_020005a0
	ldr	r2, [pc, #272]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x02008590
	.4byte 0x02008590
	.4byte 0x02008590
	.4byte 0x020085a0
	.4byte 0x020085a0
	.4byte 0x020085a0
	.4byte 0x020085a0
	.4byte 0x020085a0
	.4byte 0x020085a0
	.4byte 0x020085a0
	.4byte 0x020085a0
	.4byte 0x020085a0
	.4byte 0x02008598
	.4byte 0x02008598
	.4byte 0x02008598
	.4byte 0x02008598
	.4byte 0x020085a0
	.4byte 0x020085a0
	.4byte 0x020085a0
	.4byte 0x020085a0
	.4byte 0x020085a0
	.4byte 0x020085a0
	.4byte 0x02008594
	.4byte 0x02008594
	.4byte 0x02008594
	.4byte 0x02008594
	.4byte 0x0200859c
	.4byte 0x0200859c
	.4byte 0xe0464827
	.4byte 0xe0444827
	.4byte 0xe0424827
	.2byte 0x4827
	.2byte 0xe040
.L_020005a0:
	ldr	r0, [pc, #156]
	b.n	.L_02000622
.L_020005a4:
	ldr	r3, [pc, #156]
	cmp	r1, r3
	bne.n	.L_02000620
	movs	r0, #241
	lsls	r0, r0, #1
	adds	r3, r2, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	subs	r3, #1
	cmp	r3, #19
	bhi.n	.L_0200061c
	ldr	r2, [pc, #140]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0x0000
	.4byte 0x02008614
	.4byte 0x02008614
	.4byte 0x02008614
	.4byte 0x02008614
	.4byte 0x0200861c
	.4byte 0x02008618
	.4byte 0x0200861c
	.4byte 0x0200861c
	.4byte 0x0200861c
	.4byte 0x02008618
	.4byte 0x02008614
	.4byte 0x02008614
	.4byte 0x02008614
	.4byte 0x02008614
	.4byte 0x02008614
	.4byte 0x02008614
	.4byte 0x02008614
	.4byte 0x02008614
	.4byte 0x0200861c
	.4byte 0x02008614
	.4byte 0xe004480d
	.2byte 0x480d
	.2byte 0xe002
.L_0200061c:
	ldr	r0, [pc, #52]
	b.n	.L_02000622
.L_02000620:
	ldr	r0, [pc, #52]
.L_02000622:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000028
	.4byte 0x02008520
	.4byte 0x0200ba84
	.4byte 0x0200bae4
	.4byte 0x0200bc10
	.4byte 0x0200bc7c
	.4byte 0x0200b9d0
	.4byte 0x00000029
	.4byte 0x020085c4
	.4byte 0x0200bd9c
	.4byte 0x0200bdfc
	.4byte 0x0200bebc
	.2byte 0xb9c4
	.2byte 0x0200
	push	{r5, lr}
	sub	sp, #8
	bl 0x0200ac68
	movs	r0, #0
	bl 0x0200ada8
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #131
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02000758
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_020006ee
	movs	r0, #232
	bl 0x0200ade0
	movs	r0, #15
	bl 0x0200ac78
	movs	r1, #206
	movs	r3, #144
	lsls	r1, r1, #18
	ldr	r2, [pc, #204]
	lsls	r3, r3, #15
	bl 0x0200ac08
	movs	r0, #15
	bl 0x0200ac78
	ldr	r3, [pc, #192]
	movs	r1, #68
	str	r3, [r0, #20]
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #72
	movs	r2, #50
	movs	r3, #64
	bl 0x0200ac30
	movs	r3, #110
	str	r3, [sp, #0]
	movs	r5, #4
	movs	r0, #106
	movs	r1, #16
	movs	r2, #3
	movs	r3, #2
	str	r5, [sp, #4]
	bl 0x0200ac38
	movs	r3, #50
	str	r3, [sp, #0]
	movs	r0, #50
	movs	r1, #1
	movs	r2, #3
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200ac38
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abe8
	b.n	.L_02000760
.L_020006ee:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02000704
	movs	r0, #117
	bl 0x0200ade0
	b.n	.L_02000760
.L_02000704:
	movs	r0, #232
	bl 0x0200ade0
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #68
	movs	r1, #68
	movs	r2, #50
	movs	r3, #64
	bl 0x0200ac30
	movs	r3, #110
	str	r3, [sp, #0]
	movs	r5, #4
	movs	r0, #102
	movs	r1, #16
	movs	r2, #3
	movs	r3, #2
	str	r5, [sp, #4]
	bl 0x0200ac38
	movs	r3, #50
	str	r3, [sp, #0]
	movs	r0, #50
	movs	r1, #0
	movs	r2, #3
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200ac38
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abe0
	b.n	.L_02000760
.L_02000758:
	ldr	r0, [pc, #16]
	movs	r1, #1
	bl 0x0200ac50
.L_02000760:
	bl 0x0200ac70
	add	sp, #8
	pop	{r5, pc}
	.4byte 0xffe00000
	.2byte 0x2185
	.2byte 0x0000
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x0200add0
	pop	{pc}
	.2byte 0x0000
	.2byte 0xb010
	.2byte 0x0200
	push	{lr}
	bl 0x0200add8
	pop	{pc}
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x0200add0
	pop	{pc}
	.2byte 0x0000
	.2byte 0xb010
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	sub	sp, #8
	bl 0x0200add8
	movs	r0, #8
	bl 0x0200ac78
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	asrs	r5, r3, #20
	cmp	r5, #16
	bne.n	.L_02000814
	bl 0x0200ac68
	movs	r0, #0
	bl 0x0200ada8
	movs	r3, #46
	str	r3, [sp, #4]
	movs	r0, #16
	movs	r3, #1
	movs	r1, #47
	movs	r2, #1
	str	r5, [sp, #0]
	adds	r7, r6, #0
	bl 0x0200ac38
	movs	r0, #1
	bl 0x0200abb0
	adds	r7, #85
	movs	r3, #3
	strb	r3, [r7, #0]
.L_020007da:
	movs	r0, #1
	bl 0x0200abb0
	ldr	r5, [r6, #40]
	cmp	r5, #0
	bne.n	.L_020007da
	movs	r0, #188
	bl 0x0200ade0
	movs	r0, #10
	bl 0x0200abb0
	movs	r0, #128
	lsls	r0, r0, #2
	strb	r5, [r7, #0]
	bl 0x0200abe0
	movs	r3, #16
	movs	r2, #46
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #14
	movs	r1, #46
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ac38
	bl 0x0200ac70
.L_02000814:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x0200adc0
	pop	{pc}
	.2byte 0x0000
	.2byte 0xb014
	.2byte 0x0200
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x0200adc0
	pop	{pc}
	.2byte 0x0000
	.2byte 0xb01e
	.2byte 0x0200
	push	{lr}
	sub	sp, #8
	movs	r3, #110
	movs	r2, #4
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #1
	movs	r2, #3
	movs	r3, #1
	movs	r0, #110
	bl 0x0200ac38
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x0200abe0
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x0200adc0
	pop	{pc}
	.2byte 0x0000
	.2byte 0xb034
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #131
	sub	sp, #8
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_0200088c
	b.n	.L_02000ad8
.L_0200088c:
	ldr	r3, [pc, #596]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r7, r3, r2
	ldr	r0, [r7, #0]
	bl 0x0200ac78
	adds	r5, r0, #0
	bl 0x0200ac68
	movs	r0, #0
	bl 0x0200ada8
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r7, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200ac80
	movs	r1, #210
	ldr	r0, [r7, #0]
	lsls	r1, r1, #2
	movs	r2, #72
	bl 0x0200acb0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #50
	bl 0x0200abd8
	adds	r5, #34
	mov	r9, r0
	mov	sl, r5
	cmp	r0, #0
	beq.n	.L_02000972
	movs	r0, #243
	bl 0x0200ade0
	movs	r1, #129
	ldr	r0, [r7, #0]
	lsls	r1, r1, #1
	bl 0x0200ad48
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	movs	r0, #0
	bl 0x0200ac48
	movs	r0, #20
	bl 0x0200ac60
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ac48
	movs	r5, #3
	movs	r1, #68
	movs	r0, #68
	movs	r2, #50
	movs	r3, #64
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200ac30
	movs	r3, #2
	mov	r2, sl
	strb	r3, [r2, #0]
	movs	r0, #20
	bl 0x0200ac60
	ldr	r0, [r7, #0]
	movs	r1, #4
	movs	r2, #0
	bl 0x0200acf0
	movs	r2, #204
	lsls	r2, r2, #8
	ldr	r0, [r7, #0]
	ldr	r1, [pc, #432]
	adds	r2, #204
	bl 0x0200ac80
	movs	r1, #210
	lsls	r1, r1, #2
	movs	r2, #88
	ldr	r0, [r7, #0]
	bl 0x0200aca8
	movs	r0, #20
	bl 0x0200ac60
	movs	r0, #232
	bl 0x0200ade0
	movs	r2, #50
	movs	r3, #64
	movs	r0, #72
	movs	r1, #68
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200ac30
	movs	r0, #20
	bl 0x0200ac60
	movs	r3, #1
	mov	r2, sl
	strb	r3, [r2, #0]
	b.n	.L_02000ad4
.L_02000972:
	movs	r0, #9
	bl 0x0200ac78
	movs	r1, #2
	adds	r6, r0, #0
	movs	r0, #9
	bl 0x0200ad38
	ldr	r1, [pc, #360]
	ldr	r2, [pc, #360]
	movs	r0, #9
	bl 0x0200ac80
	movs	r0, #9
	bl 0x0200ac78
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r1, #206
	strb	r3, [r0, #0]
	lsls	r1, r1, #2
	movs	r2, #88
	movs	r0, #9
	bl 0x0200acb0
	movs	r0, #1
	bl 0x0200ac60
	movs	r0, #9
	bl 0x0200ac78
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #243
	bl 0x0200ade0
	movs	r1, #129
	ldr	r0, [r7, #0]
	lsls	r1, r1, #1
	bl 0x0200ad48
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	movs	r0, #0
	bl 0x0200ac48
	movs	r0, #20
	bl 0x0200ac60
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ac48
	movs	r5, #3
	movs	r1, #68
	movs	r0, #68
	movs	r2, #50
	movs	r3, #64
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200ac30
	movs	r3, #2
	mov	r8, r3
	mov	r2, r8
	mov	r3, sl
	strb	r2, [r3, #0]
	movs	r0, #20
	bl 0x0200ac60
	movs	r1, #192
	ldr	r0, [r7, #0]
	lsls	r1, r1, #7
	movs	r2, #20
	bl 0x0200ad28
	ldr	r0, [r7, #0]
	movs	r1, #4
	movs	r2, #0
	bl 0x0200acf0
	movs	r2, #204
	lsls	r2, r2, #8
	ldr	r0, [r7, #0]
	ldr	r1, [pc, #188]
	adds	r2, #204
	bl 0x0200ac80
	movs	r1, #210
	movs	r2, #88
	ldr	r0, [r7, #0]
	lsls	r1, r1, #2
	bl 0x0200aca8
	movs	r1, #128
	ldr	r0, [r7, #0]
	lsls	r1, r1, #8
	bl 0x0200ad30
	ldr	r0, [r7, #0]
	movs	r1, #9
	bl 0x0200adb0
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #72]
	mov	r2, r8
	adds	r6, #34
	strb	r2, [r6, #0]
	movs	r0, #9
	movs	r1, #4
	movs	r2, #0
	bl 0x0200acf0
	movs	r1, #208
	movs	r0, #9
	lsls	r1, r1, #2
	movs	r2, #74
	bl 0x0200aca8
	movs	r0, #9
	movs	r1, #4
	movs	r2, #0
	bl 0x0200acf0
	movs	r1, #214
	movs	r2, #88
	movs	r0, #9
	lsls	r1, r1, #2
	bl 0x0200aca8
	movs	r1, #129
	lsls	r1, r1, #1
	ldr	r0, [r7, #0]
	bl 0x0200ad48
	movs	r0, #20
	bl 0x0200ac60
	movs	r0, #232
	bl 0x0200ade0
	movs	r1, #68
	movs	r2, #50
	movs	r3, #64
	movs	r0, #72
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200ac30
	movs	r0, #20
	bl 0x0200ac60
	ldr	r0, [r7, #0]
	bl 0x0200ac90
	movs	r3, #1
	mov	r2, sl
	strb	r3, [r2, #0]
	mov	r3, r9
	movs	r0, #1
	strb	r3, [r6, #0]
	bl 0x0200abb0
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abe8
.L_02000ad4:
	bl 0x0200ac70
.L_02000ad8:
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x00019999
	.4byte 0x00026666
	.2byte 0x3333
	.2byte 0x0001
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #131
	sub	sp, #8
	bl 0x0200abd8
	cmp	r0, #0
	bne.n	.L_02000bda
	ldr	r5, [pc, #216]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	bl 0x0200ac78
	mov	r8, r0
	bl 0x0200ac68
	movs	r0, #0
	bl 0x0200ada8
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200ac80
	movs	r1, #203
	movs	r2, #72
	lsls	r1, r1, #2
	ldr	r0, [r5, #0]
	bl 0x0200acb0
	movs	r0, #243
	bl 0x0200ade0
	movs	r1, #129
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200ad48
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	movs	r0, #0
	bl 0x0200ac48
	movs	r0, #20
	bl 0x0200ac60
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ac48
	movs	r6, #3
	movs	r1, #68
	movs	r0, #68
	movs	r2, #50
	movs	r3, #64
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ac30
	movs	r3, #34
	add	r8, r3
	mov	r2, r8
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r0, #20
	bl 0x0200ac60
	ldr	r0, [r5, #0]
	movs	r1, #4
	movs	r2, #0
	bl 0x0200acf0
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #72]
	ldr	r2, [pc, #72]
	bl 0x0200ac80
	movs	r1, #203
	lsls	r1, r1, #2
	movs	r2, #88
	ldr	r0, [r5, #0]
	bl 0x0200aca8
	movs	r0, #20
	bl 0x0200ac60
	movs	r0, #232
	bl 0x0200ade0
	movs	r2, #50
	movs	r3, #64
	movs	r0, #72
	movs	r1, #68
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ac30
	movs	r0, #20
	bl 0x0200ac60
	movs	r3, #1
	mov	r2, r8
	strb	r3, [r2, #0]
.L_02000bda:
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00033333
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl 0x0200ac68
	movs	r0, #0
	bl 0x0200ada8
	ldr	r5, [pc, #76]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	adds	r0, r6, #0
	bl 0x0200ad08
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x0200acf8
	movs	r1, #129
	adds	r0, r6, #0
	lsls	r1, r1, #1
	bl 0x0200ad48
	movs	r0, #20
	bl 0x0200ac60
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r5, r5, r3
	movs	r3, #2
	strb	r3, [r5, #0]
	movs	r0, #11
	movs	r1, #0
	bl 0x0200ad78
	movs	r2, #190
	lsls	r2, r2, #2
	adds	r6, r6, r2
	adds	r0, r6, #0
	bl 0x0200abe0
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl 0x0200ac68
	movs	r0, #0
	bl 0x0200ada8
	ldr	r5, [pc, #76]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	adds	r0, r6, #0
	bl 0x0200ad08
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x0200acf8
	movs	r1, #129
	adds	r0, r6, #0
	lsls	r1, r1, #1
	bl 0x0200ad48
	movs	r0, #20
	bl 0x0200ac60
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r5, r5, r3
	movs	r3, #2
	strb	r3, [r5, #0]
	movs	r0, #11
	movs	r1, #0
	bl 0x0200ad78
	movs	r2, #253
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r6, r6, r2
	adds	r0, r6, #0
	bl 0x0200abe0
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl 0x0200ac68
	movs	r0, #0
	bl 0x0200ada8
	ldr	r5, [pc, #76]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	adds	r0, r6, #0
	bl 0x0200ad08
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x0200acf8
	movs	r1, #129
	adds	r0, r6, #0
	lsls	r1, r1, #1
	bl 0x0200ad48
	movs	r0, #20
	bl 0x0200ac60
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r5, r5, r3
	movs	r3, #2
	strb	r3, [r5, #0]
	movs	r0, #11
	movs	r1, #0
	bl 0x0200ad78
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #255
	adds	r6, r6, r2
	adds	r0, r6, #0
	bl 0x0200abe0
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	bl 0x0200ac78
	ldr	r1, [pc, #32]
	adds	r6, r0, #0
	ldr	r2, [pc, #32]
	adds	r0, r5, #0
	bl 0x0200ac80
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r6, #72]
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r6, #40]
	adds	r0, r5, #0
	movs	r1, #120
	movs	r2, #120
	bl 0x0200aca0
	pop	{r5, r6, pc}
	.4byte 0x00033333
	.2byte 0x9999
	.2byte 0x0001
	push	{lr}
	bl 0x0200ac68
	movs	r0, #0
	bl 0x0200ada8
	movs	r0, #0
	bl 0x0200ade0
	movs	r0, #9
	movs	r1, #2
	bl 0x0200acf8
	movs	r0, #10
	movs	r1, #2
	bl 0x0200acf8
	movs	r0, #11
	movs	r1, #2
	bl 0x0200acf8
	movs	r0, #12
	movs	r1, #2
	bl 0x0200acf8
	movs	r1, #2
	movs	r0, #13
	bl 0x0200ad00
	movs	r0, #20
	bl 0x0200ac60
	movs	r0, #147
	bl 0x0200ade0
	movs	r0, #9
	bl 0x02008d10
	movs	r0, #10
	bl 0x02008d10
	movs	r0, #11
	bl 0x02008d10
	movs	r0, #12
	bl 0x02008d10
	movs	r0, #13
	bl 0x02008d10
	movs	r0, #13
	bl 0x0200acc8
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
	movs	r2, #0
	movs	r1, #0
	movs	r0, #13
	bl 0x0200acd0
	movs	r0, #20
	bl 0x0200ac60
	movs	r0, #52
	bl 0x0200ade0
	ldr	r1, [pc, #48]
	movs	r0, #8
	bl 0x0200ac98
	ldr	r3, [pc, #44]
	movs	r2, #166
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r3, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	ldr	r0, [pc, #36]
	movs	r1, #15
	bl 0x0200ad80
	movs	r0, #11
	movs	r1, #1
	bl 0x0200ad78
	movs	r0, #132
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x0200abe0
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200ae9c
	.4byte 0x02000240
	.2byte 0x0029
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	sub	sp, #8
	bl 0x0200abd8
	mov	r8, r0
	cmp	r0, #0
	bne.n	.L_02000f44
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02000f44
	movs	r0, #8
	bl 0x0200ac78
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	asrs	r7, r3, #20
	cmp	r7, #35
	bne.n	.L_02000f44
	ldr	r3, [r6, #16]
	asrs	r5, r3, #20
	cmp	r5, #9
	bne.n	.L_02000f44
	ldr	r3, [pc, #580]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r1, r1, r3
	ldr	r0, [r1, #0]
	mov	r9, r1
	bl 0x0200ac78
	mov	sl, r0
	bl 0x0200ac68
	movs	r0, #0
	bl 0x0200ada8
	movs	r0, #20
	bl 0x0200ac60
	movs	r0, #183
	bl 0x0200ade0
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #35
	movs	r1, #68
	movs	r2, #35
	movs	r3, #69
	bl 0x0200ac30
	movs	r0, #35
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200ac38
	adds	r3, r6, #0
	adds	r3, #85
	mov	r2, r8
	strb	r2, [r3, #0]
	movs	r1, #3
	movs	r0, #8
	bl 0x0200ad38
	mov	r1, sl
	ldr	r2, [r6, #16]
	ldr	r3, [r1, #16]
	cmp	r2, r3
	ble.n	.L_02000eda
	mov	r2, r9
	ldr	r0, [r2, #0]
	movs	r1, #3
	bl 0x0200ad38
.L_02000eda:
	movs	r5, #0
.L_02000edc:
	ldr	r3, [r6, #12]
	ldr	r1, [pc, #472]
	movs	r0, #1
	adds	r3, r3, r1
	str	r3, [r6, #12]
	adds	r5, #1
	bl 0x0200abb0
	cmp	r5, #127
	bls.n	.L_02000edc
	movs	r0, #8
	movs	r1, #3
	bl 0x0200ace0
	movs	r1, #142
	movs	r2, #152
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	movs	r0, #10
	bl 0x0200acd0
	movs	r0, #20
	bl 0x0200ac60
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200abe0
	mov	r1, sl
	ldr	r2, [r6, #16]
	ldr	r3, [r1, #16]
	cmp	r2, r3
	ble.n	.L_02000f40
	ldr	r5, [pc, #400]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x0200ad38
	ldr	r0, [r5, #0]
	bl 0x0200ac78
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
.L_02000f40:
	bl 0x0200ac70
.L_02000f44:
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abd8
	mov	r8, r0
	cmp	r0, #0
	bne.n	.L_02001052
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02001052
	movs	r0, #9
	bl 0x0200ac78
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	asrs	r7, r3, #20
	cmp	r7, #29
	bne.n	.L_02001052
	ldr	r3, [r6, #16]
	asrs	r5, r3, #20
	cmp	r5, #9
	bne.n	.L_02001052
	ldr	r3, [pc, #312]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r1, r1, r3
	ldr	r0, [r1, #0]
	mov	r9, r1
	bl 0x0200ac78
	mov	sl, r0
	bl 0x0200ac68
	movs	r0, #0
	bl 0x0200ada8
	movs	r0, #20
	bl 0x0200ac60
	movs	r0, #183
	bl 0x0200ade0
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #29
	movs	r1, #68
	movs	r2, #29
	movs	r3, #69
	bl 0x0200ac30
	movs	r0, #35
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200ac38
	adds	r3, r6, #0
	adds	r3, #85
	mov	r2, r8
	strb	r2, [r3, #0]
	movs	r1, #3
	movs	r0, #9
	bl 0x0200ad38
	mov	r1, sl
	ldr	r2, [r6, #16]
	ldr	r3, [r1, #16]
	cmp	r2, r3
	ble.n	.L_02000fe8
	mov	r2, r9
	ldr	r0, [r2, #0]
	movs	r1, #3
	bl 0x0200ad38
.L_02000fe8:
	movs	r5, #0
.L_02000fea:
	ldr	r3, [r6, #12]
	ldr	r1, [pc, #200]
	movs	r0, #1
	adds	r3, r3, r1
	str	r3, [r6, #12]
	adds	r5, #1
	bl 0x0200abb0
	cmp	r5, #127
	bls.n	.L_02000fea
	movs	r0, #9
	movs	r1, #3
	bl 0x0200ace0
	movs	r1, #236
	movs	r2, #152
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	movs	r0, #11
	bl 0x0200acd0
	movs	r0, #20
	bl 0x0200ac60
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abe0
	mov	r1, sl
	ldr	r2, [r6, #16]
	ldr	r3, [r1, #16]
	cmp	r2, r3
	ble.n	.L_0200104e
	ldr	r5, [pc, #132]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x0200ad38
	ldr	r0, [r5, #0]
	bl 0x0200ac78
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
.L_0200104e:
	bl 0x0200ac70
.L_02001052:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_020010a6
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_020010a6
	bl 0x0200ac68
	movs	r0, #0
	bl 0x0200ada8
	movs	r0, #188
	bl 0x0200ade0
	movs	r3, #3
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #124
	movs	r2, #31
	movs	r3, #64
	movs	r0, #4
	bl 0x0200ac30
	movs	r0, #20
	bl 0x0200ac60
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #130
	bl 0x0200abe0
	bl 0x0200ac70
.L_020010a6:
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xe000
	.2byte 0xffff
	.2byte 0xb500
	ldr	r0, [pc, #8]
	bl 0x0200add0
	pop	{pc}
	.2byte 0x0000
	.2byte 0xb04e
	.2byte 0x0200
	push	{lr}
	bl 0x0200add8
	bl 0x02008e2c
	pop	{pc}
	push	{lr}
	movs	r0, #106
	bl 0x0200ade0
	movs	r1, #2
	movs	r0, #8
	bl 0x0200ace0
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x0200abe0
	bl 0x02008e2c
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #106
	bl 0x0200ade0
	movs	r1, #2
	movs	r0, #9
	bl 0x0200ace0
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abe0
	bl 0x02008e2c
	pop	{pc}
	push	{lr}
	movs	r0, #10
	bl 0x0200ac78
	movs	r1, #0
	bl 0x0200ad10
	pop	{pc}
	push	{lr}
	movs	r0, #11
	bl 0x0200ac78
	movs	r1, #0
	bl 0x0200ad10
	pop	{pc}
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r0, #8
	sub	sp, #8
	ldr	r5, [r3, #108]
	bl 0x0200ac78
	adds	r6, r0, #0
	bl 0x0200ac68
	movs	r0, #0
	bl 0x0200ada8
	movs	r3, #181
	lsls	r3, r3, #1
	movs	r0, #128
	adds	r2, r5, r3
	lsls	r0, r0, #2
	movs	r3, #0
	strh	r3, [r2, #0]
	adds	r0, #2
	bl 0x0200abe8
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x0200abe8
	movs	r0, #20
	bl 0x0200ac60
	movs	r0, #186
	bl 0x0200ade0
	movs	r5, #0
.L_0200117e:
	ldr	r3, [r6, #12]
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r3, r3, r2
	str	r3, [r6, #12]
	movs	r0, #1
	adds	r5, #1
	bl 0x0200abb0
	cmp	r5, #63
	bls.n	.L_0200117e
	movs	r0, #8
	movs	r1, #2
	bl 0x0200ad38
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #4
	movs	r1, #123
	movs	r2, #35
	movs	r3, #69
	bl 0x0200ac30
	movs	r3, #35
	movs	r2, #9
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	movs	r0, #36
	bl 0x0200ac38
	movs	r0, #188
	bl 0x0200ade0
	movs	r3, #3
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #124
	movs	r2, #31
	movs	r3, #64
	movs	r0, #7
	bl 0x0200ac30
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #130
	bl 0x0200abe8
	movs	r0, #20
	bl 0x0200ac60
	bl 0x0200ac70
	add	sp, #8
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r0, #9
	sub	sp, #8
	ldr	r5, [r3, #108]
	bl 0x0200ac78
	adds	r6, r0, #0
	bl 0x0200ac68
	movs	r0, #0
	bl 0x0200ada8
	movs	r3, #181
	lsls	r3, r3, #1
	movs	r0, #130
	adds	r2, r5, r3
	lsls	r0, r0, #1
	movs	r3, #0
	strh	r3, [r2, #0]
	adds	r0, #255
	bl 0x0200abe8
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abe8
	movs	r0, #20
	bl 0x0200ac60
	movs	r0, #186
	bl 0x0200ade0
	movs	r5, #0
.L_0200123c:
	ldr	r3, [r6, #12]
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r3, r3, r2
	str	r3, [r6, #12]
	movs	r0, #1
	adds	r5, #1
	bl 0x0200abb0
	cmp	r5, #63
	bls.n	.L_0200123c
	movs	r0, #9
	movs	r1, #2
	bl 0x0200ad38
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #4
	movs	r1, #123
	movs	r2, #29
	movs	r3, #69
	bl 0x0200ac30
	movs	r3, #29
	movs	r2, #9
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	movs	r0, #36
	bl 0x0200ac38
	movs	r0, #188
	bl 0x0200ade0
	movs	r3, #3
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #124
	movs	r2, #31
	movs	r3, #64
	movs	r0, #7
	bl 0x0200ac30
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #130
	bl 0x0200abe8
	movs	r0, #20
	bl 0x0200ac60
	bl 0x0200ac70
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r5, [pc, #168]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	sub	sp, #8
	bl 0x0200ac78
	adds	r6, r0, #0
	bl 0x0200ac68
	movs	r0, #0
	bl 0x0200ada8
	movs	r0, #128
	movs	r1, #128
	movs	r2, #248
	movs	r3, #128
	lsls	r3, r3, #17
	lsls	r0, r0, #12
	lsls	r1, r1, #13
	lsls	r2, r2, #16
	bl 0x0200ad68
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	ldr	r6, [pc, #120]
	ldr	r0, [r5, #0]
	adds	r2, r2, r6
	bl 0x0200acd0
	movs	r0, #64
	bl 0x0200ac78
	ldr	r2, [r0, #16]
	ldr	r1, [r0, #8]
	adds	r2, r2, r6
	movs	r0, #64
	bl 0x0200acd0
	movs	r0, #64
	bl 0x0200ac78
	movs	r1, #0
	bl 0x0200ad10
	movs	r0, #10
	bl 0x0200abb0
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200ad50
	movs	r0, #1
	bl 0x0200abb0
	movs	r3, #6
	movs	r2, #4
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #2
	movs	r2, #1
	movs	r1, #0
	movs	r0, #6
	bl 0x0200ac38
	bl 0x0200ac10
	movs	r0, #1
	bl 0x0200abb0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #228
	bl 0x0200abe0
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200ad88
	bl 0x0200ac70
	add	sp, #8
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfed0
	.2byte 0xb500
	bl 0x0200ac68
	movs	r0, #0
	bl 0x0200ada8
	movs	r0, #0
	bl 0x0200ada8
	bl 0x0200ac70
	pop	{pc}
	push	{r5, r6, lr}
	adds	r5, r0, #0
	bl 0x0200ac78
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200ac78
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #1
	adds	r0, r5, #0
	bl 0x0200ad38
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #253
	ands	r3, r2
	strb	r3, [r1, #0]
	adds	r6, #98
	movs	r3, #1
	strb	r3, [r6, #0]
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r5, r0, #0
	bl 0x0200ac78
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200ac78
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #2
	adds	r0, r5, #0
	bl 0x0200ad38
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #253
	ands	r3, r2
	strb	r3, [r1, #0]
	adds	r6, #98
	movs	r3, #1
	strb	r3, [r6, #0]
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #15
	bl 0x0200ac78
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	movs	r1, #3
	strb	r3, [r0, #0]
	movs	r0, #15
	bl 0x0200ad38
	movs	r0, #15
	bl 0x0200ac78
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #253
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #16
	bl 0x02009380
	movs	r0, #17
	bl 0x02009380
	movs	r0, #18
	bl 0x02009380
	movs	r0, #19
	bl 0x02009380
	movs	r0, #20
	bl 0x02009380
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #16
	bl 0x02009380
	movs	r0, #17
	bl 0x02009380
	movs	r0, #18
	bl 0x02009380
	movs	r0, #19
	bl 0x02009380
	movs	r0, #20
	bl 0x02009380
	movs	r0, #22
	bl 0x02009380
	pop	{pc}
	push	{lr}
	movs	r0, #10
	bl 0x020093b8
	movs	r0, #11
	bl 0x020093b8
	pop	{pc}
	push	{r5, r6, lr}
	ldr	r3, [pc, #224]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #8
	bl 0x0200ac78
	ldr	r3, [r0, #8]
	movs	r2, #6
	asrs	r5, r3, #20
	ldr	r3, [r0, #16]
	movs	r1, #105
	asrs	r6, r3, #20
	movs	r3, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #74
	movs	r2, #4
	movs	r3, #85
	bl 0x0200ac30
	cmp	r6, #25
	bne.n	.L_020014c0
	cmp	r5, #4
	bne.n	.L_02001552
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #18
	movs	r1, #126
	movs	r2, #4
	movs	r3, #85
	bl 0x0200ac30
	b.n	.L_02001552
.L_020014c0:
	cmp	r6, #26
	bne.n	.L_020014dc
	cmp	r5, #7
	bne.n	.L_02001552
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #18
	movs	r1, #126
	movs	r2, #7
	movs	r3, #86
	bl 0x0200ac30
	b.n	.L_02001552
.L_020014dc:
	cmp	r6, #27
	bne.n	.L_02001536
	cmp	r5, #4
	bne.n	.L_020014f4
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #18
	movs	r1, #126
	movs	r2, #4
	b.n	.L_0200152e
.L_020014f4:
	cmp	r5, #6
	bne.n	.L_02001508
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #18
	movs	r1, #126
	movs	r2, #6
	b.n	.L_0200152e
.L_02001508:
	cmp	r5, #7
	bne.n	.L_0200151c
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #18
	movs	r1, #126
	movs	r2, #7
	b.n	.L_0200152e
.L_0200151c:
	cmp	r5, #9
	bne.n	.L_02001552
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #18
	movs	r1, #126
	movs	r2, #9
.L_0200152e:
	movs	r3, #87
	bl 0x0200ac30
	b.n	.L_02001552
.L_02001536:
	cmp	r6, #29
	bne.n	.L_02001552
	cmp	r5, #6
	bne.n	.L_02001552
	movs	r3, #1
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #18
	movs	r1, #126
	movs	r2, #6
	movs	r3, #89
	bl 0x0200ac30
.L_02001552:
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #128
	movs	r3, #192
	lsls	r0, r0, #2
	lsls	r3, r3, #18
	adds	r0, #2
	ldr	r5, [r3, #108]
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02001594
	movs	r0, #10
	bl 0x0200ac78
	ldr	r3, [r0, #8]
	ldr	r2, [r0, #16]
	orrs	r3, r2
	cmp	r3, #0
	bne.n	.L_02001594
	movs	r3, #181
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #200
	strh	r3, [r2, #0]
	movs	r0, #8
	movs	r1, #1
	bl 0x0200ace0
.L_02001594:
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_020015c4
	movs	r0, #11
	bl 0x0200ac78
	ldr	r3, [r0, #8]
	ldr	r2, [r0, #16]
	orrs	r3, r2
	cmp	r3, #0
	bne.n	.L_020015c4
	movs	r3, #181
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #201
	strh	r3, [r2, #0]
	movs	r0, #9
	movs	r1, #1
	bl 0x0200ace0
.L_020015c4:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	movs	r3, #202
	movs	r7, #128
	movs	r6, #140
	mov	r5, sp
	lsls	r3, r3, #18
	lsls	r7, r7, #12
	lsls	r6, r6, #16
	str	r3, [r5, #0]
	str	r7, [r5, #4]
	str	r6, [r5, #8]
	mov	r8, r3
	bl 0x0200abc8
	adds	r1, r0, #0
	movs	r0, #128
	adds	r2, r5, #0
	lsls	r0, r0, #14
	bl 0x0200abd0
	movs	r0, #128
	lsls	r0, r0, #2
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	adds	r0, #162
	bl 0x0200ac00
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001642
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #52]
	movs	r1, #0
	bl 0x0200ac40
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200abf0
	adds	r0, r5, #0
	mov	r1, r8
	adds	r2, r7, #0
	adds	r3, r6, #0
	bl 0x0200ac18
	ldr	r1, [pc, #16]
	adds	r0, r5, #0
	bl 0x0200abf8
.L_02001642:
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xbef8
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	bl 0x0200ac68
	movs	r0, #0
	bl 0x0200ada8
	ldr	r0, [pc, #708]
	bl 0x0200ad18
	movs	r0, #78
	bl 0x0200ade0
	bl 0x0200ad90
	bl 0x0200ada0
	ldr	r2, [pc, #692]
	movs	r6, #133
	mov	r8, r2
	lsls	r6, r6, #2
	movs	r1, #204
	movs	r2, #204
	add	r6, r8
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r6, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200ac80
	movs	r1, #202
	ldr	r0, [r6, #0]
	movs	r2, #208
	lsls	r1, r1, #2
	bl 0x0200acb0
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200ad58
	movs	r0, #202
	movs	r1, #1
	movs	r2, #176
	movs	r3, #1
	lsls	r2, r2, #16
	lsls	r0, r0, #18
	negs	r1, r1
	bl 0x0200ad60
	ldr	r1, [r6, #0]
	movs	r0, #7
	bl 0x0200acd8
	ldr	r1, [r6, #0]
	movs	r0, #6
	bl 0x0200acd8
	ldr	r1, [r6, #0]
	movs	r0, #5
	bl 0x0200acd8
	ldr	r1, [r6, #0]
	movs	r0, #8
	bl 0x0200acd8
	movs	r0, #1
	bl 0x0200abb0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #7
	adds	r1, #204
	adds	r2, #102
	bl 0x0200ac80
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #6
	adds	r1, #204
	adds	r2, #102
	bl 0x0200ac80
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #5
	adds	r1, #204
	adds	r2, #102
	bl 0x0200ac80
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #102
	movs	r0, #8
	adds	r1, #204
	bl 0x0200ac80
	ldr	r1, [pc, #512]
	movs	r0, #6
	bl 0x0200ac88
	ldr	r1, [pc, #508]
	movs	r0, #5
	bl 0x0200ac88
	ldr	r1, [pc, #504]
	movs	r0, #8
	bl 0x0200ac88
	ldr	r1, [pc, #500]
	movs	r0, #7
	bl 0x0200ac98
	movs	r0, #20
	bl 0x0200ac60
	movs	r0, #7
	movs	r1, #0
	bl 0x0200ad20
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #7
	bl 0x0200ad30
	movs	r0, #7
	movs	r1, #0
	bl 0x0200ad20
	movs	r0, #7
	movs	r1, #3
	bl 0x0200ace0
	movs	r0, #7
	movs	r1, #0
	bl 0x0200ad20
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x0200ace8
	movs	r1, #202
	movs	r2, #148
	movs	r0, #7
	lsls	r1, r1, #2
	bl 0x0200acb0
	movs	r0, #7
	movs	r1, #2
	bl 0x0200ad00
	movs	r1, #1
	movs	r0, #7
	bl 0x0200ad38
	movs	r0, #220
	bl 0x0200ade0
	ldr	r5, [pc, #408]
	movs	r1, #144
	lsls	r1, r1, #3
	adds	r0, r5, #0
	bl 0x0200abb8
	movs	r0, #60
	bl 0x0200ac60
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200ade0
	movs	r0, #20
	bl 0x0200ac60
	adds	r0, r5, #0
	bl 0x0200abc0
	movs	r0, #251
	bl 0x0200ade0
	movs	r0, #172
	movs	r2, #232
	movs	r3, #136
	lsls	r0, r0, #18
	movs	r1, #0
	lsls	r2, r2, #18
	lsls	r3, r3, #18
	bl 0x0200ad68
	movs	r0, #202
	movs	r1, #1
	movs	r2, #224
	movs	r3, #0
	lsls	r0, r0, #18
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200ad60
	movs	r1, #202
	movs	r2, #240
	ldr	r0, [r6, #0]
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200acd0
	movs	r1, #202
	movs	r2, #210
	movs	r0, #7
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200acd0
	movs	r1, #196
	movs	r2, #234
	movs	r0, #6
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200acd0
	movs	r1, #208
	movs	r2, #234
	movs	r0, #5
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200acd0
	movs	r1, #198
	movs	r2, #244
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	movs	r0, #8
	bl 0x0200acd0
	bl 0x0200ac10
	movs	r0, #20
	bl 0x0200ac60
	movs	r0, #246
	bl 0x0200ade0
	movs	r0, #80
	bl 0x0200ac60
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #85
	str	r2, [r3, #0]
	bl 0x0200ad98
	bl 0x0200ada0
	movs	r0, #242
	bl 0x0200ac58
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #131
	bl 0x0200abe0
	movs	r0, #147
	lsls	r0, r0, #4
	bl 0x0200abe0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #49
	bl 0x0200abe0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #51
	bl 0x0200abe0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #52
	bl 0x0200abe0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #53
	bl 0x0200abe0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #54
	bl 0x0200abe0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #55
	bl 0x0200abe0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #56
	bl 0x0200abe0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #57
	bl 0x0200abe0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #58
	bl 0x0200abe0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #59
	bl 0x0200abe0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #60
	bl 0x0200abe0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #62
	bl 0x0200abe0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #50
	bl 0x0200abe0
	movs	r0, #132
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200abe0
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #118
	add	r8, r3
	mov	r2, r8
	movs	r3, #1
	strh	r3, [r2, #0]
	movs	r0, #90
	bl 0x0200ad70
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.4byte 0x00002168
	.4byte 0x02000240
	.4byte 0x0200af44
	.4byte 0x0200af88
	.4byte 0x0200afcc
	.4byte 0x0200af1c
	.2byte 0x95c9
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	adds	r6, r1, #0
	mov	r8, r0
	adds	r0, r6, #0
	bl 0x0200ac78
	adds	r5, r0, #0
	bl 0x0200ac68
	movs	r0, #0
	bl 0x0200ada8
	movs	r3, #128
	lsls	r3, r3, #5
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	movs	r0, #1
	bl 0x0200abb0
	bl 0x0200ad90
	bl 0x0200ada0
	movs	r0, #20
	bl 0x0200ac60
	movs	r0, #103
	bl 0x0200ade0
	mov	r0, r8
	ldr	r1, [pc, #28]
	bl 0x0200ac88
	ldr	r1, [pc, #28]
	adds	r0, r6, #0
	bl 0x0200ac98
	movs	r0, #20
	bl 0x0200ac60
	bl 0x0200ac70
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0200ae30
	.2byte 0xae6c
	.2byte 0x0200
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #50
	bl 0x0200abd8
	cmp	r0, #0
	bne.n	.L_020019dc
	bl 0x0200ac68
	movs	r0, #0
	bl 0x0200ada8
	bl 0x0200ad90
	bl 0x0200ada0
	movs	r0, #20
	bl 0x0200ac60
	bl 0x02009a04
	bl 0x0200ac70
.L_020019dc:
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #50
	bl 0x0200abd8
	cmp	r0, #0
	bne.n	.L_02001a02
	bl 0x0200ac68
	movs	r0, #0
	bl 0x0200ada8
	bl 0x02009a04
	bl 0x0200ac70
.L_02001a02:
	pop	{pc}
	push	{r5, lr}
	movs	r0, #9
	bl 0x0200ac78
	movs	r1, #128
	movs	r2, #0
	adds	r5, r0, #0
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x0200ad40
	movs	r0, #9
	movs	r1, #2
	bl 0x0200ad00
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #72]
	movs	r0, #9
	movs	r1, #1
	bl 0x0200ad38
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #9
	adds	r1, #204
	adds	r2, #102
	bl 0x0200ac80
	movs	r0, #9
	movs	r1, #6
	movs	r2, #0
	bl 0x0200acf0
	movs	r1, #208
	lsls	r1, r1, #2
	movs	r2, #88
	movs	r0, #9
	bl 0x0200aca8
	movs	r0, #20
	bl 0x0200ac60
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	movs	r0, #9
	ldr	r1, [pc, #48]
	bl 0x0200ac80
	movs	r0, #9
	movs	r1, #3
	bl 0x0200ad38
	movs	r1, #206
	movs	r0, #9
	lsls	r1, r1, #2
	movs	r2, #72
	bl 0x0200aca8
	movs	r1, #192
	movs	r0, #9
	lsls	r1, r1, #6
	movs	r2, #40
	bl 0x0200ad28
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abe0
	pop	{r5, pc}
	.2byte 0x9999
	.2byte 0x0001
	push	{lr}
.L_02001a9e:
	bl 0x0200ac68
	movs	r0, #0
.L_02001aa4:
	bl 0x0200ada8
	movs	r0, #164
.L_02001aaa:
	bl 0x0200ade0
	movs	r0, #120
.L_02001ab0:
	bl 0x0200ac60
	bl 0x0200ac70
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r0, #214
	lsls	r0, r0, #1
	movs	r2, #133
	adds	r3, r3, r0
	lsls	r2, r2, #1
	movs	r0, #144
	adds	r2, #255
	lsls	r0, r0, #4
	str	r2, [r3, #0]
	adds	r0, #180
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02001ae8
	movs	r0, #162
	lsls	r0, r0, #1
	bl 0x0200abe0
.L_02001ae8:
	ldr	r2, [pc, #312]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r0, #0
	ldrsh	r1, [r3, r0]
	ldr	r3, [pc, #304]
	cmp	r1, r3
	bne.n	.L_02001ba2
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	subs	r3, #3
	cmp	r3, #27
	bls.n	.L_02001b0c
	b.n	.L_02001c20
.L_02001b0c:
	ldr	r2, [pc, #284]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x02009b84
	.4byte 0x02009b84
	.4byte 0x02009b84
	.4byte 0x02009b8a
	.4byte 0x02009b8a
	.4byte 0x02009b8a
	.4byte 0x02009c20
	.4byte 0x02009c20
	.4byte 0x02009c20
	.4byte 0x02009c20
	.4byte 0x02009c20
	.4byte 0x02009c20
	.4byte 0x02009b96
	.4byte 0x02009b96
	.4byte 0x02009b96
	.4byte 0x02009b96
	.4byte 0x02009c20
	.4byte 0x02009c20
	.4byte 0x02009c20
	.4byte 0x02009c20
	.4byte 0x02009c20
	.4byte 0x02009c20
	.4byte 0x02009b90
	.4byte 0x02009b90
	.4byte 0x02009b90
	.4byte 0x02009b90
	.4byte 0x02009b9c
	.4byte 0x02009b9c
	.4byte 0xf880f000
	.4byte 0xf000e04a
	.4byte 0xe047fe2b
	.4byte 0xf93cf000
	.4byte 0xf000e044
	.4byte 0xe041fc41
	.4byte 0xfc7cf000
	.2byte 0xe03e
.L_02001ba2:
	ldr	r3, [pc, #140]
	cmp	r1, r3
	bne.n	.L_02001c20
	movs	r0, #241
	lsls	r0, r0, #1
	adds	r3, r2, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	subs	r3, #1
	cmp	r3, #19
	bhi.n	.L_02001c20
	ldr	r2, [pc, #120]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x02009c10
	.4byte 0x02009c10
	.4byte 0x02009c10
	.4byte 0x02009c10
	.4byte 0x02009c20
	.4byte 0x02009c16
	.4byte 0x02009c20
	.4byte 0x02009c20
	.4byte 0x02009c1c
	.4byte 0x02009c16
	.4byte 0x02009c10
	.4byte 0x02009c10
	.4byte 0x02009c10
	.4byte 0x02009c10
	.4byte 0x02009c10
	.4byte 0x02009c10
	.4byte 0x02009c10
	.4byte 0x02009c10
	.4byte 0x02009c20
	.4byte 0x02009c10
	.4byte 0xfe06f000
	.4byte 0xf000e004
	.4byte 0xe001feab
	.2byte 0xf000
	.2byte 0xffbc
.L_02001c20:
	movs	r0, #0
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000028
	.4byte 0x02009b14
	.4byte 0x00000029
	.2byte 0x9bc0
	.2byte 0x0200
	push	{r5, lr}
	movs	r5, #0
.L_02001c3c:
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r0, r5, r2
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02001c70
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #86
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #1
	bne.n	.L_02001c70
	movs	r3, #147
	lsls	r3, r3, #4
	adds	r0, r5, r3
	bl 0x0200abe0
	movs	r2, #136
	lsls	r2, r2, #2
	adds	r0, r5, r2
	bl 0x0200abe0
.L_02001c70:
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r0, r5, r3
	adds	r5, #1
	bl 0x0200abe8
	cmp	r5, #15
	bls.n	.L_02001c3c
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #147
	lsls	r0, r0, #4
	sub	sp, #8
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02001d1a
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x0200abd8
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02001cda
	movs	r3, #6
	movs	r2, #25
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #25
	movs	r2, #1
	movs	r3, #3
	movs	r0, #0
	bl 0x0200ac38
	movs	r0, #10
	bl 0x0200ac78
	movs	r1, #208
	movs	r3, #212
	lsls	r3, r3, #17
	lsls	r1, r1, #15
	movs	r2, #0
	bl 0x0200ac08
	movs	r0, #10
	bl 0x0200ac78
	movs	r3, #0
	str	r3, [r0, #20]
	b.n	.L_02001d14
.L_02001cda:
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
	movs	r3, #6
	movs	r2, #25
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #25
	movs	r2, #1
	movs	r3, #3
	movs	r0, #0
	bl 0x0200ac38
	movs	r0, #10
	bl 0x0200ac78
	movs	r1, #208
	movs	r3, #212
	lsls	r1, r1, #15
	movs	r2, #0
	lsls	r3, r3, #17
	bl 0x0200ac08
	movs	r0, #10
	bl 0x0200ac78
	str	r5, [r0, #20]
.L_02001d14:
	movs	r0, #1
	bl 0x0200abb0
.L_02001d1a:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #49
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02001d76
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02001d40
	movs	r0, #9
	movs	r1, #1
	bl 0x0200ad38
	b.n	.L_02001d4a
.L_02001d40:
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
.L_02001d4a:
	movs	r0, #11
	bl 0x0200ac78
	movs	r1, #208
	movs	r3, #134
	lsls	r1, r1, #15
	ldr	r2, [pc, #104]
	lsls	r3, r3, #18
	bl 0x0200ac08
	movs	r0, #11
	bl 0x0200ac78
	ldr	r3, [pc, #88]
	movs	r1, #1
	str	r3, [r0, #20]
	movs	r0, #11
	bl 0x0200ad38
	movs	r0, #1
	bl 0x0200abb0
.L_02001d76:
	ldr	r0, [pc, #76]
	bl 0x0200adb8
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02001d9a
	movs	r0, #8
	movs	r1, #10
	bl 0x02009944
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x0200abe8
	b.n	.L_02001dba
.L_02001d9a:
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02001dba
	movs	r0, #9
	movs	r1, #11
	bl 0x02009944
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abe8
.L_02001dba:
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0xffe00000
	.2byte 0xb014
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	adds	r5, r0, #0
	bl 0x0200ac78
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200ac78
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	movs	r1, #0
	mov	r8, r1
	strb	r3, [r0, #0]
	movs	r1, #1
	adds	r0, r5, #0
	bl 0x0200ad38
	adds	r3, r6, #0
	adds	r3, #85
	mov	r2, r8
	strb	r2, [r3, #0]
	ldr	r3, [pc, #12]
	str	r3, [r6, #12]
	str	r3, [r6, #20]
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xffe0
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #748]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #8
	bl 0x0200ac78
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #32
	orrs	r3, r5
	strb	r3, [r0, #0]
	ldr	r0, [pc, #724]
	bl 0x0200adc8
	movs	r0, #8
	bl 0x0200ac78
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r1, #1
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #8
	bl 0x0200ad38
	movs	r0, #9
	bl 0x0200ac78
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r5, r3
	strb	r5, [r0, #0]
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #50
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02001edc
	movs	r0, #15
	bl 0x0200ac78
	adds	r5, r0, #0
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r0, #15
	bl 0x0200ac78
	movs	r1, #206
	movs	r3, #144
	lsls	r3, r3, #15
	lsls	r1, r1, #18
	ldr	r2, [pc, #644]
	bl 0x0200ac08
	ldr	r3, [pc, #636]
	movs	r0, #128
	lsls	r0, r0, #2
	str	r3, [r5, #20]
	adds	r0, #18
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02001eb0
	movs	r3, #110
	movs	r2, #4
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #110
	movs	r1, #1
	movs	r2, #3
	movs	r3, #1
	bl 0x0200ac38
	b.n	.L_02001ec4
.L_02001eb0:
	movs	r3, #110
	movs	r2, #4
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #110
	movs	r1, #0
	movs	r2, #3
	movs	r3, #1
	bl 0x0200ac38
.L_02001ec4:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #34
	bl 0x0200abd8
	cmp	r0, #0
	bne.n	.L_02001edc
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
.L_02001edc:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #131
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02001fbc
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02001f36
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #68
	movs	r1, #68
	movs	r2, #50
	movs	r3, #64
	bl 0x0200ac30
	movs	r3, #110
	movs	r5, #4
	str	r3, [sp, #0]
	movs	r0, #102
	movs	r1, #16
.L_02001f1e:
	movs	r2, #3
	movs	r3, #2
	str	r5, [sp, #4]
	bl 0x0200ac38
	movs	r3, #50
	str	r3, [sp, #0]
	movs	r0, #50
	movs	r1, #0
	movs	r2, #3
	movs	r3, #1
	b.n	.L_02001fa4
.L_02001f36:
	movs	r0, #15
	bl 0x0200ac78
	movs	r1, #206
	movs	r3, #144
.L_02001f40:
	lsls	r1, r1, #18
	ldr	r2, [pc, #452]
	lsls	r3, r3, #15
	bl 0x0200ac08
	movs	r0, #15
	bl 0x0200ac78
	ldr	r3, [pc, #436]
	movs	r1, #68
	str	r3, [r0, #20]
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #72
	movs	r2, #50
	movs	r3, #64
	bl 0x0200ac30
	movs	r5, #4
	movs	r0, #106
	movs	r1, #16
	movs	r2, #3
	movs	r3, #2
	movs	r6, #110
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200ac38
	movs	r3, #50
	str	r3, [sp, #0]
	movs	r0, #50
	movs	r1, #1
	movs	r2, #3
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200ac38
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02001fac
	movs	r0, #110
	movs	r1, #1
	movs	r2, #3
	movs	r3, #1
	str	r6, [sp, #0]
.L_02001fa4:
	str	r5, [sp, #4]
	bl 0x0200ac38
	b.n	.L_02001fbc
.L_02001fac:
	movs	r0, #110
	movs	r1, #0
	movs	r2, #3
	movs	r3, #1
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200ac38
.L_02001fbc:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #51
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_0200203a
	movs	r3, #46
	str	r3, [sp, #0]
	movs	r5, #5
	movs	r0, #42
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200ac38
	movs	r3, #45
	str	r3, [sp, #0]
	movs	r0, #43
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200ac38
	movs	r3, #47
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	movs	r0, #43
	str	r5, [sp, #4]
	bl 0x0200ac38
	movs	r0, #16
	bl 0x0200ac78
	movs	r1, #186
	movs	r3, #176
	lsls	r3, r3, #15
	lsls	r1, r1, #18
	ldr	r2, [pc, #244]
	bl 0x0200ac08
	movs	r0, #16
	bl 0x0200ac78
	ldr	r3, [pc, #232]
	str	r3, [r0, #20]
	movs	r0, #146
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	bne.n	.L_02002040
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
	b.n	.L_02002040
.L_0200203a:
	movs	r0, #10
	bl 0x02009dc8
.L_02002040:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #52
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_0200210c
	movs	r0, #8
	bl 0x0200ac78
	movs	r6, #47
	movs	r5, #9
	adds	r7, r0, #0
	movs	r1, #0
	movs	r0, #42
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200ac38
	movs	r3, #46
	str	r3, [sp, #0]
	movs	r0, #43
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200ac38
	movs	r3, #48
	str	r3, [sp, #0]
	movs	r0, #43
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200ac38
	movs	r3, #10
	str	r3, [sp, #4]
	movs	r0, #43
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #0]
	bl 0x0200ac38
	movs	r3, #8
	str	r3, [sp, #4]
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	movs	r0, #43
	str	r6, [sp, #0]
	bl 0x0200ac38
	movs	r0, #17
	bl 0x0200ac78
	movs	r1, #190
	movs	r3, #152
	lsls	r3, r3, #16
	lsls	r1, r1, #18
	ldr	r2, [pc, #68]
	bl 0x0200ac08
	movs	r0, #17
	bl 0x0200ac78
	ldr	r3, [pc, #56]
	str	r3, [r0, #20]
	ldr	r3, [r7, #8]
	asrs	r3, r3, #20
	cmp	r3, #47
	bne.n	.L_020020e6
	ldr	r3, [r7, #16]
	asrs	r3, r3, #20
	cmp	r3, #8
	bne.n	.L_020020e6
	ldr	r0, [pc, #32]
	bl 0x0200adc8
.L_020020e6:
	movs	r0, #137
	lsls	r0, r0, #2
	bl 0x0200abd8
	cmp	r0, #0
	bne.n	.L_02002112
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
	b.n	.L_02002112
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200b010
	.2byte 0x0000
	.2byte 0xffe0
.L_0200210c:
	.2byte 0x200b
	bl 0x02009dc8
.L_02002112:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #53
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_020021de
	movs	r0, #8
	bl 0x0200ac78
	ldr	r3, [r0, #8]
	movs	r6, #49
	asrs	r3, r3, #20
	mov	r8, r3
	ldr	r3, [r0, #16]
	movs	r5, #7
	movs	r0, #42
	movs	r1, #0
	movs	r2, #1
	asrs	r7, r3, #20
	movs	r3, #1
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200ac38
	movs	r3, #6
	str	r3, [sp, #4]
	movs	r0, #43
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #0]
	bl 0x0200ac38
	movs	r3, #50
	str	r3, [sp, #0]
	movs	r0, #43
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200ac38
	movs	r3, #48
	str	r3, [sp, #0]
	movs	r0, #43
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200ac38
	movs	r3, #8
	str	r3, [sp, #4]
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	movs	r0, #43
	str	r6, [sp, #0]
	bl 0x0200ac38
	movs	r0, #18
	bl 0x0200ac78
	movs	r1, #198
	movs	r3, #240
	lsls	r3, r3, #15
	lsls	r1, r1, #18
	ldr	r2, [pc, #584]
	bl 0x0200ac08
	movs	r0, #18
	bl 0x0200ac78
	ldr	r3, [pc, #572]
	str	r3, [r0, #20]
	mov	r3, r8
	cmp	r3, #48
	bne.n	.L_020021b4
	cmp	r7, #7
	beq.n	.L_020021be
.L_020021b4:
	mov	r2, r8
	cmp	r2, #49
	bne.n	.L_020021c4
	cmp	r7, #8
	bne.n	.L_020021c4
.L_020021be:
	ldr	r0, [pc, #552]
	bl 0x0200adc8
.L_020021c4:
	movs	r0, #147
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	bne.n	.L_020021e4
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
	b.n	.L_020021e4
.L_020021de:
	movs	r0, #12
	bl 0x02009dc8
.L_020021e4:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #54
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_0200223e
	movs	r3, #51
	movs	r2, #7
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	movs	r0, #42
	bl 0x0200ac38
	movs	r0, #19
	bl 0x0200ac78
	movs	r1, #206
	movs	r3, #240
	lsls	r3, r3, #15
	lsls	r1, r1, #18
	ldr	r2, [pc, #460]
	bl 0x0200ac08
	movs	r0, #19
	bl 0x0200ac78
	ldr	r3, [pc, #448]
	str	r3, [r0, #20]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #38
	bl 0x0200abd8
	cmp	r0, #0
	bne.n	.L_02002244
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
	b.n	.L_02002244
.L_0200223e:
	movs	r0, #13
	bl 0x02009dc8
.L_02002244:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #55
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_020022e6
	movs	r6, #51
	movs	r5, #9
	movs	r0, #42
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200ac38
	movs	r3, #8
	str	r3, [sp, #4]
	movs	r0, #43
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #0]
	bl 0x0200ac38
	movs	r3, #50
	str	r3, [sp, #0]
	movs	r0, #43
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200ac38
	movs	r3, #52
	str	r3, [sp, #0]
	movs	r0, #43
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200ac38
	movs	r3, #10
	str	r3, [sp, #4]
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	movs	r0, #43
	str	r6, [sp, #0]
	bl 0x0200ac38
	movs	r0, #20
	bl 0x0200ac78
	movs	r1, #206
	movs	r3, #152
	lsls	r3, r3, #16
	lsls	r1, r1, #18
	ldr	r2, [pc, #292]
	bl 0x0200ac08
	movs	r0, #20
	bl 0x0200ac78
	ldr	r3, [pc, #280]
	str	r3, [r0, #20]
	movs	r0, #148
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	bne.n	.L_020022ec
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
	b.n	.L_020022ec
.L_020022e6:
	movs	r0, #14
	bl 0x02009dc8
.L_020022ec:
	ldr	r0, [pc, #252]
	bl 0x0200adb8
	movs	r1, #144
	ldr	r0, [pc, #248]
	lsls	r1, r1, #3
	bl 0x0200abb8
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #34
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_0200231e
	movs	r0, #9
	movs	r1, #15
	bl 0x02009944
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #34
	bl 0x0200abe8
	b.n	.L_020023ba
.L_0200231e:
	movs	r0, #146
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02002338
	movs	r0, #10
	movs	r1, #16
	bl 0x02009944
	movs	r0, #146
	b.n	.L_0200236e
.L_02002338:
	movs	r0, #137
	lsls	r0, r0, #2
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02002356
	movs	r0, #11
	movs	r1, #17
	bl 0x02009944
	movs	r0, #137
	lsls	r0, r0, #2
	bl 0x0200abe8
	b.n	.L_020023ba
.L_02002356:
	movs	r0, #147
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02002378
	movs	r0, #12
	movs	r1, #18
	bl 0x02009944
	movs	r0, #147
.L_0200236e:
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abe8
	b.n	.L_020023ba
.L_02002378:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #38
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_0200239a
	movs	r0, #13
	movs	r1, #19
	bl 0x02009944
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #38
	bl 0x0200abe8
	b.n	.L_020023ba
.L_0200239a:
	movs	r0, #148
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_020023ba
	movs	r0, #14
	movs	r1, #20
	bl 0x02009944
	movs	r0, #148
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abe8
.L_020023ba:
	ldr	r3, [pc, #56]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #26
	bne.n	.L_020023da
	movs	r0, #10
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	bne.n	.L_020023da
	bl 0x020099ac
.L_020023da:
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0xffe00000
	.4byte 0x0200b010
	.4byte 0x0200b01e
	.4byte 0x020093f1
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #28]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ac78
	ldr	r3, [r0, #80]
	movs	r0, #8
	ldrb	r1, [r3, #9]
	lsls	r1, r1, #28
	lsrs	r1, r1, #30
	bl 0x0200ad38
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #8
	sub	sp, #8
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_0200243e
	movs	r3, #16
	movs	r2, #46
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #14
	movs	r1, #46
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ac38
.L_0200243e:
	ldr	r0, [pc, #20]
	bl 0x0200adc8
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #12]
	bl 0x0200abb8
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200b010
	.2byte 0xa3f9
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	adds	r6, r0, #0
	bl 0x0200ac78
	mov	r8, r0
	adds	r0, r6, #0
	bl 0x0200ac78
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #1
	adds	r0, r6, #0
	bl 0x0200ad38
	mov	r3, r8
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	mov	r3, r8
	str	r5, [r3, #12]
	str	r5, [r3, #20]
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #824]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #8
	bl 0x0200ac78
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #32
	orrs	r3, r5
	strb	r3, [r0, #0]
	ldr	r0, [pc, #800]
	bl 0x0200adc8
	movs	r0, #8
	bl 0x0200ac78
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r1, #1
	orrs	r5, r3
	strb	r5, [r0, #0]
	movs	r0, #8
	bl 0x0200ad38
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #56
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02002528
	movs	r3, #48
	movs	r2, #32
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	movs	r0, #44
	bl 0x0200ac38
	movs	r0, #16
	bl 0x0200ac78
	movs	r1, #194
	movs	r3, #130
	lsls	r3, r3, #18
	lsls	r1, r1, #18
	movs	r2, #0
	bl 0x0200ac08
	movs	r0, #16
	bl 0x0200ac78
	movs	r3, #0
	str	r3, [r0, #20]
	movs	r0, #138
	lsls	r0, r0, #2
	bl 0x0200abd8
	cmp	r0, #0
	bne.n	.L_0200252e
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
	b.n	.L_0200252e
.L_02002528:
	movs	r0, #9
	bl 0x0200a45c
.L_0200252e:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #57
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02002588
	movs	r3, #53
	movs	r2, #32
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	movs	r0, #44
	bl 0x0200ac38
	movs	r0, #17
	bl 0x0200ac78
	movs	r1, #214
	movs	r3, #130
	lsls	r3, r3, #18
	lsls	r1, r1, #18
	movs	r2, #0
	bl 0x0200ac08
	movs	r0, #17
	bl 0x0200ac78
	movs	r3, #0
	str	r3, [r0, #20]
	movs	r0, #149
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	bne.n	.L_0200258e
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
	b.n	.L_0200258e
.L_02002588:
	movs	r0, #10
	bl 0x0200a45c
.L_0200258e:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #58
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_020025e8
	movs	r3, #43
	movs	r2, #37
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	movs	r0, #44
	bl 0x0200ac38
	movs	r0, #18
	bl 0x0200ac78
	movs	r1, #174
	movs	r3, #150
	lsls	r3, r3, #18
	lsls	r1, r1, #18
	movs	r2, #0
	bl 0x0200ac08
	movs	r0, #18
	bl 0x0200ac78
	movs	r3, #0
	str	r3, [r0, #20]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #42
	bl 0x0200abd8
	cmp	r0, #0
	bne.n	.L_020025ee
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
	b.n	.L_020025ee
.L_020025e8:
	movs	r0, #11
	bl 0x0200a45c
.L_020025ee:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #59
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02002648
	movs	r3, #48
	movs	r2, #37
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	movs	r0, #44
	bl 0x0200ac38
	movs	r0, #19
	bl 0x0200ac78
	movs	r1, #194
	movs	r3, #150
	lsls	r3, r3, #18
	lsls	r1, r1, #18
	movs	r2, #0
	bl 0x0200ac08
	movs	r0, #19
	bl 0x0200ac78
	movs	r3, #0
	str	r3, [r0, #20]
	movs	r0, #150
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	bne.n	.L_0200264e
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
	b.n	.L_0200264e
.L_02002648:
	movs	r0, #12
	bl 0x0200a45c
.L_0200264e:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #60
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_020026a6
	movs	r3, #50
	movs	r2, #37
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	movs	r0, #44
	bl 0x0200ac38
	movs	r0, #20
	bl 0x0200ac78
	movs	r1, #202
	movs	r3, #150
	lsls	r3, r3, #18
	lsls	r1, r1, #18
	movs	r2, #0
	bl 0x0200ac08
	movs	r0, #20
	bl 0x0200ac78
	movs	r3, #0
	str	r3, [r0, #20]
	movs	r0, #139
	lsls	r0, r0, #2
	bl 0x0200abd8
	cmp	r0, #0
	bne.n	.L_020026ac
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
	b.n	.L_020026ac
.L_020026a6:
	movs	r0, #13
	bl 0x0200a45c
.L_020026ac:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #62
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02002706
	movs	r3, #48
	movs	r2, #43
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	movs	r0, #44
	bl 0x0200ac38
	movs	r0, #22
	bl 0x0200ac78
	movs	r1, #194
	movs	r3, #174
	lsls	r3, r3, #18
	lsls	r1, r1, #18
	movs	r2, #0
	bl 0x0200ac08
	movs	r0, #22
	bl 0x0200ac78
	movs	r3, #0
	str	r3, [r0, #20]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #46
	bl 0x0200abd8
	cmp	r0, #0
	bne.n	.L_0200270c
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
	b.n	.L_0200270c
.L_02002706:
	movs	r0, #15
	bl 0x0200a45c
.L_0200270c:
	ldr	r0, [pc, #204]
	bl 0x0200adb8
	movs	r1, #144
	ldr	r0, [pc, #200]
	lsls	r1, r1, #3
	bl 0x0200abb8
	movs	r0, #138
	lsls	r0, r0, #2
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02002734
	movs	r0, #9
	movs	r1, #16
	bl 0x02009944
	movs	r0, #138
	b.n	.L_020027a8
.L_02002734:
	movs	r0, #149
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_0200274e
	movs	r0, #10
	movs	r1, #17
	bl 0x02009944
	movs	r0, #149
	b.n	.L_02002788
.L_0200274e:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #42
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02002770
	movs	r0, #11
	movs	r1, #18
	bl 0x02009944
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #42
	bl 0x0200abe8
	b.n	.L_020027d0
.L_02002770:
	movs	r0, #150
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02002792
	movs	r0, #12
	movs	r1, #19
	bl 0x02009944
	movs	r0, #150
.L_02002788:
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abe8
	b.n	.L_020027d0
.L_02002792:
	movs	r0, #139
	lsls	r0, r0, #2
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_020027b0
.L_0200279e:
	movs	r0, #13
	movs	r1, #20
	bl 0x02009944
	movs	r0, #139
.L_020027a8:
	lsls	r0, r0, #2
.L_020027aa:
	bl 0x0200abe8
	b.n	.L_020027d0
.L_020027b0:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #46
.L_020027b6:
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_020027d0
	movs	r0, #15
	movs	r1, #22
.L_020027c2:
	bl 0x02009944
	movs	r0, #128
.L_020027c8:
	lsls	r0, r0, #2
	adds	r0, #46
	bl 0x0200abe8
.L_020027d0:
	add	sp, #8
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x0200b010
	.4byte 0x0200b034
	.2byte 0x943d
	.2byte 0x0200
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #131
	sub	sp, #8
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_0200281c
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #17
	movs	r1, #125
	movs	r2, #27
	movs	r3, #63
	bl 0x0200ac30
	movs	r3, #25
	movs	r2, #4
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #25
	movs	r1, #0
	movs	r2, #5
	movs	r3, #3
	bl 0x0200ac38
.L_0200281c:
	add	sp, #8
	pop	{pc}
	push	{r5, r6, lr}
.L_02002822:
	ldr	r3, [pc, #312]
	movs	r2, #133
	lsls	r2, r2, #2
.L_02002828:
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	sub	sp, #8
	bl 0x0200ac78
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	movs	r1, #144
	strb	r3, [r0, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #284]
	bl 0x0200abb8
	movs	r0, #132
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_020028d2
	movs	r0, #64
	bl 0x0200ac78
	adds	r5, r0, #0
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r0, #64
	bl 0x0200ac78
	movs	r1, #136
	movs	r3, #196
	ldr	r2, [pc, #244]
	lsls	r1, r1, #16
	lsls	r3, r3, #17
	bl 0x0200ac08
	ldr	r3, [pc, #236]
	movs	r1, #3
	str	r3, [r5, #20]
	movs	r0, #64
	bl 0x0200ad38
	movs	r0, #64
	bl 0x0200ac78
	movs	r1, #1
	bl 0x0200ad10
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200acd0
	movs	r0, #1
	bl 0x0200abb0
.L_020028d2:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #228
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_0200293a
	movs	r0, #64
	bl 0x0200ac78
	ldr	r3, [pc, #132]
	ldr	r2, [r0, #16]
	ldr	r1, [r0, #8]
	adds	r2, r2, r3
	movs	r0, #64
	bl 0x0200acd0
	movs	r0, #64
	bl 0x0200ac78
	movs	r1, #0
	bl 0x0200ad10
	movs	r3, #6
	movs	r2, #4
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #6
	movs	r1, #0
	movs	r2, #1
	movs	r3, #2
	bl 0x0200ac38
	movs	r0, #128
	movs	r1, #128
	movs	r2, #248
	movs	r3, #128
	lsls	r2, r2, #16
	lsls	r3, r3, #17
	lsls	r0, r0, #12
	lsls	r1, r1, #13
	bl 0x0200ad68
	ldr	r0, [r6, #0]
	movs	r1, #0
	bl 0x0200ad50
	bl 0x0200ac10
	movs	r0, #1
	bl 0x0200abb0
.L_0200293a:
	movs	r0, #152
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02002956
	bl 0x02009a9c
	movs	r0, #152
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abe8
.L_02002956:
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x02009475
	.4byte 0xffe00000
	.4byte 0xfff00000
	.2byte 0x0000
	.2byte 0xfed0
	.2byte 0xb520
	ldr	r3, [pc, #524]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #8
	bl 0x0200ac78
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	ldr	r0, [pc, #500]
	bl 0x0200adc8
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #130
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_020029c8
	movs	r3, #3
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #4
	movs	r1, #124
	movs	r2, #31
	movs	r3, #64
	bl 0x0200ac30
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200abe0
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abe0
.L_020029c8:
	movs	r0, #8
	bl 0x0200ac78
	movs	r1, #0
	bl 0x0200ac40
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02002a46
	movs	r0, #8
	bl 0x0200ac78
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	adds	r5, r0, #0
	movs	r1, #68
	movs	r0, #35
	movs	r2, #35
	movs	r3, #69
	bl 0x0200ac30
	movs	r3, #35
	movs	r2, #9
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #35
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ac38
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #142
	movs	r3, #152
	ldr	r2, [pc, #360]
	lsls	r3, r3, #16
	adds	r0, r5, #0
	lsls	r1, r1, #18
	bl 0x0200ac08
	movs	r1, #3
	movs	r0, #8
	bl 0x0200ad38
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x0200abe0
	movs	r1, #142
	movs	r2, #152
	movs	r0, #10
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200acd0
.L_02002a46:
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02002a72
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02002a6a
	movs	r0, #8
	movs	r1, #3
	bl 0x0200ace0
	b.n	.L_02002a72
.L_02002a6a:
	movs	r0, #8
	movs	r1, #2
	bl 0x0200ace0
.L_02002a72:
	movs	r0, #9
	bl 0x0200ac78
	movs	r1, #0
	bl 0x0200ac40
	movs	r0, #10
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	bne.n	.L_02002a94
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abe0
.L_02002a94:
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02002afc
	movs	r0, #9
	bl 0x0200ac78
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	adds	r5, r0, #0
	movs	r1, #68
	movs	r0, #29
	movs	r2, #29
	movs	r3, #69
	bl 0x0200ac30
	movs	r3, #29
	movs	r2, #9
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #35
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ac38
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r3, [pc, #172]
	movs	r1, #3
	str	r3, [r5, #12]
	movs	r0, #9
	bl 0x0200ad38
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abe0
	movs	r1, #236
	movs	r2, #152
	movs	r0, #11
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200acd0
.L_02002afc:
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02002b2a
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	beq.n	.L_02002b22
	movs	r0, #9
	movs	r1, #3
	bl 0x0200ace0
	b.n	.L_02002b2a
.L_02002b22:
	movs	r0, #9
	movs	r1, #2
	bl 0x0200ace0
.L_02002b2a:
	movs	r0, #1
	bl 0x0200abb0
	ldr	r0, [pc, #88]
	bl 0x0200adb8
	movs	r1, #144
	ldr	r0, [pc, #84]
	lsls	r1, r1, #3
	bl 0x0200abb8
	movs	r0, #132
	lsls	r0, r0, #2
	bl 0x0200abd8
	cmp	r0, #0
	bne.n	.L_02002b58
	movs	r0, #10
	bl 0x0200ac78
	movs	r1, #15
	bl 0x0200ad10
.L_02002b58:
	movs	r0, #137
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200abd8
	cmp	r0, #0
	bne.n	.L_02002b72
	movs	r0, #11
	bl 0x0200ac78
	movs	r1, #15
	bl 0x0200ad10
.L_02002b72:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #28]
	bl 0x0200abb8
	add	sp, #8
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x0200b04e
	.4byte 0xfff00000
	.4byte 0x0200b014
	.4byte 0x02009465
	.2byte 0x955d
	.2byte 0x0200
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #131
	bl 0x0200abd8
	cmp	r0, #0
	bne.n	.L_02002bac
	bl 0x02009650
.L_02002bac:
	pop	{pc}
	.2byte 0x0000
	.irp EntryTarget, 0x080000c1, 0x080000d1, 0x080000d9, 0x080000f9, 0x08000129, 0x080003c9, 0x080003d1, 0x080003d9, 0x08020091, 0x080200a9, 0x080200c1, 0x080200e9, 0x08020121, 0x08020149, 0x08020151, 0x08020171, 0x08020179, 0x080201e9, 0x08020219, 0x08020229, 0x08038041, 0x080ad041, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80b1, 0x080c80b9, 0x080c80c1, 0x080c80c9, 0x080c80d9, 0x080c80e1, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8111, 0x080c8119, 0x080c8129, 0x080c8139, 0x080c8141, 0x080c8149, 0x080c8159, 0x080c8171, 0x080c8181, 0x080c81a1, 0x080c81d1, 0x080c81d9, 0x080c8201, 0x080c8211, 0x080c8219, 0x080c8229, 0x080c8231, 0x080c8239, 0x080c8261, 0x080c8279, 0x080c8281, 0x080c8291, 0x080c8379, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c84e1, 0x080c8601, 0x080c86f9, 0x080c8709, 0x080c8719, 0x080c8721, 0x080c8729, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000007
	.4byte 0xffff8000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffc00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000018
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffe00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00000400
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00000400
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00003000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe800
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000000d
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffe00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffe00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x000000a0
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03100000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03400000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0xffff0008
	.4byte 0x0210000a
	.4byte 0x0211000b
	.4byte 0x0010ffff
	.4byte 0x00110213
	.4byte 0x00120214
	.4byte 0x00130215
	.4byte 0x00140216
	.4byte 0xffff0217
	.4byte 0x02180010
	.4byte 0x02190011
	.4byte 0x021a0012
	.4byte 0x021b0013
	.4byte 0x021c0014
	.4byte 0x021e0016
	.4byte 0x0008ffff
	.4byte 0xffff0009
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
	.4byte 0xffff0009
	.4byte 0x00000196
	.4byte 0xc0000178
	.4byte 0x01500000
	.4byte 0x02680108
	.4byte 0x000001a8
	.4byte 0xffff000a
	.4byte 0x00000226
	.4byte 0xc0000179
	.4byte 0x01500000
	.4byte 0x02680108
	.4byte 0x000001a8
	.4byte 0xffff000b
	.4byte 0x00000236
	.4byte 0xc0000379
	.4byte 0x01e80000
	.4byte 0x02d80308
	.4byte 0x000003a8
	.4byte 0xffff000c
	.4byte 0x00000288
	.4byte 0xc000037e
	.4byte 0x01e80000
	.4byte 0x02d80308
	.4byte 0x000003a8
	.4byte 0xffff000d
	.4byte 0x000001b8
	.4byte 0x000000d8
	.4byte 0x01800000
	.4byte 0x02700080
	.4byte 0x00000120
	.4byte 0xffff000e
	.4byte 0x00000237
	.4byte 0x400000d7
	.4byte 0x01800000
	.4byte 0x02700080
	.4byte 0x00000120
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x00000048
	.4byte 0x40000048
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000048
	.4byte 0x40000048
	.4byte 0x00080000
	.4byte 0x00f80010
	.4byte 0x00000100
	.4byte 0xffff0002
	.4byte 0x000000a8
	.4byte 0x40000048
	.4byte 0x00080000
	.4byte 0x00f80010
	.4byte 0x00000100
	.4byte 0xffff0003
	.4byte 0x00000048
	.4byte 0xc00000d8
	.4byte 0x00080000
	.4byte 0x00f80010
	.4byte 0x00000100
	.4byte 0xffff0004
	.4byte 0x000000b8
	.4byte 0xc00000d8
	.4byte 0x00080000
	.4byte 0x00f80010
	.4byte 0x00000100
	.4byte 0xffff000b
	.4byte 0x00000048
	.4byte 0x40000178
	.4byte 0x00080000
	.4byte 0x00f80140
	.4byte 0x00000230
	.4byte 0xffff000c
	.4byte 0x000000a8
	.4byte 0x40000178
	.4byte 0x00080000
	.4byte 0x00f80140
	.4byte 0x00000230
	.4byte 0xffff000d
	.4byte 0x00000048
	.4byte 0xc0000200
	.4byte 0x00080000
	.4byte 0x00f80140
	.4byte 0x00000230
	.4byte 0xffff000e
	.4byte 0x000000b8
	.4byte 0xc0000200
	.4byte 0x00080000
	.4byte 0x00f80140
	.4byte 0x00000230
	.4byte 0xffff000f
	.4byte 0x00000078
	.4byte 0xc00001b8
	.4byte 0x00080000
	.4byte 0x00f80140
	.4byte 0x00000230
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffec01d2
	.4byte 0x01da0244
	.4byte 0x024cfff4
	.4byte 0x0013ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00100200
	.4byte 0x021000be
	.4byte 0x00ce0020
	.4byte 0x000affff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x1010102b
	.4byte 0x00000907
	.4byte 0x101030c8
	.4byte 0x000009b4
	.4byte 0x101040ea
	.4byte 0xffffffff
	.4byte 0x00203028
	.4byte 0x00302028
	.4byte 0x00406028
	.4byte 0x0050d028
	.4byte 0x00604028
	.4byte 0x00713028
	.4byte 0x10801029
	.4byte 0x0000093f
	.4byte 0x1080b029
	.4byte 0x000009e4
	.4byte 0x10810029
	.4byte 0xffffffff
	.4byte 0x10902029
	.4byte 0x0000093f
	.4byte 0x1090c029
	.4byte 0x000009e4
	.4byte 0x10911029
	.4byte 0xffffffff
	.4byte 0x00a19028
	.4byte 0x00b1a028
	.4byte 0x00c1d028
	.4byte 0x00d05028
	.4byte 0x10e03029
	.4byte 0x0000093f
	.4byte 0x10e0d029
	.4byte 0x000009e4
	.4byte 0x10e12029
	.4byte 0xffffffff
	.4byte 0x10f04029
	.4byte 0x0000093f
	.4byte 0x10f0e029
	.4byte 0x000009e4
	.4byte 0x10f14029
	.4byte 0xffffffff
	.4byte 0x0101b028
	.4byte 0x0111c028
	.4byte 0x0121e028
	.4byte 0x0130a029
	.4byte 0x01407028
	.4byte 0x0190a028
	.4byte 0x01a0b028
	.4byte 0x01b10028
	.4byte 0x01c11028
	.4byte 0x01d0c028
	.4byte 0x01e12028
	.4byte 0x00000029
	.4byte 0x00108028
	.4byte 0x00209028
	.4byte 0x0030e028
	.4byte 0x0040f028
	.4byte 0x00607029
	.4byte 0x00706029
	.4byte 0x10809029
	.4byte 0x00000983
	.4byte 0x10813029
	.4byte 0xffffffff
	.4byte 0x00908029
	.4byte 0x00a14028
	.4byte 0x05a45002
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x01004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x01004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x01004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x01004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x01004000
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff013a
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff013a
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0122
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
	.4byte 0xffff0007
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
	.4byte 0x00024000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
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
	.4byte 0x007e0000
	.4byte 0x00020001
	.4byte 0x00010002
	.4byte 0x0001007e
	.4byte 0x00040002
	.4byte 0x007e0002
	.4byte 0x00020001
	.4byte 0xffff0004
	.4byte 0x007e000a
	.4byte 0x00020001
	.4byte 0x000b0004
	.4byte 0x0001007e
	.4byte 0x00040002
	.4byte 0x0004ffff
	.4byte 0x0002007e
	.4byte 0x00020002
	.4byte 0x0000ffff
	.4byte 0x00000002
	.4byte 0x0200b8e0
	.4byte 0x00030004
	.4byte 0x0000000e
	.4byte 0x0200b8c0
	.4byte 0x000b0023
	.4byte 0x0000000f
	.4byte 0x0200b8c0
	.4byte 0x002d0006
	.4byte 0x00000010
	.4byte 0x0200b8c0
	.4byte 0x002d000b
	.4byte 0x00000011
	.4byte 0x0200b8c0
	.4byte 0x002d0015
	.4byte 0x00000012
	.4byte 0x0200b8c0
	.4byte 0x002e001a
	.4byte 0x00000014
	.4byte 0x0200b8c0
	.4byte 0x001c001d
	.4byte 0x00000019
	.4byte 0x0200b8c0
	.4byte 0x00020030
	.4byte 0x0000001a
	.4byte 0x0200b8c0
	.4byte 0x00020036
	.4byte 0x0000001d
	.4byte 0x0200b8c0
	.4byte 0x001d0029
	.4byte 0x00000001
	.4byte 0x0200b8c0
	.4byte 0x00020004
	.4byte 0x00000002
	.4byte 0x0200b8c0
	.4byte 0x0002000a
	.4byte 0x00000006
	.4byte 0x0200b8c0
	.4byte 0x0003001f
	.4byte 0x00000008
	.4byte 0x0200b8c0
	.4byte 0x00330029
	.4byte 0x0000000b
	.4byte 0x0200b8c0
	.4byte 0x00150004
	.4byte 0x0000000c
	.4byte 0x0200b8c0
	.4byte 0x0015000a
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x0200821d
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
	.4byte 0x0000c602
	.4byte 0xffff000e
	.4byte 0x0200821d
	.4byte 0x0000c602
	.4byte 0xffff0014
	.4byte 0x0200821d
	.4byte 0x000000f3
	.4byte 0xffff00c8
	.4byte 0x00403063
	.4byte 0x000001c3
	.4byte 0xffff00c9
	.4byte 0x0040306c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008bf1
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008bf1
	.4byte 0x00001815
	.4byte 0x0210000a
	.4byte 0x02008819
	.4byte 0x00001815
	.4byte 0x0211000b
	.4byte 0x02008819
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0019
	.4byte 0x0200821d
	.4byte 0x0000c602
	.4byte 0xffff001a
	.4byte 0x0200821d
	.4byte 0x00000001
	.4byte 0xffff001b
	.4byte 0x0000001b
	.4byte 0x00000001
	.4byte 0xffff001c
	.4byte 0x0000001c
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008c51
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008c51
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02008c51
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x02008c51
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008c51
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008c51
	.4byte 0x00001815
	.4byte 0x0212000f
	.4byte 0x02008839
	.4byte 0x00001815
	.4byte 0x02130010
	.4byte 0x02008829
	.4byte 0x00001815
	.4byte 0x02140011
	.4byte 0x02008829
	.4byte 0x00001815
	.4byte 0x02150012
	.4byte 0x02008829
	.4byte 0x00001815
	.4byte 0x02160013
	.4byte 0x02008829
	.4byte 0x00001815
	.4byte 0x02170014
	.4byte 0x02008829
	.4byte 0x10008c15
	.4byte 0xffff0008
	.4byte 0x02008771
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte 0x02008781
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02008771
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008781
	.4byte 0x00000003
	.4byte 0xffff0028
	.4byte 0x0200865d
	.4byte 0x00000002
	.4byte 0x02120029
	.4byte 0x02008871
	.4byte 0x00000002
	.4byte 0x0212002b
	.4byte 0x02008af5
	.4byte 0x00000002
	.4byte 0x0201002a
	.4byte 0x020099e1
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff000f
	.4byte 0x0200821d
	.4byte 0x0000c602
	.4byte 0xffff0010
	.4byte 0x0200821d
	.4byte 0x0000c602
	.4byte 0xffff0011
	.4byte 0x0200821d
	.4byte 0x0000c602
	.4byte 0xffff0012
	.4byte 0x0200821d
	.4byte 0x10008c15
	.4byte 0xffff0008
	.4byte 0x02008789
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte 0x02008799
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02008789
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008799
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff001d
	.4byte 0x0200821d
	.4byte 0x00000001
	.4byte 0xffff001e
	.4byte 0x0000001e
	.4byte 0x00000400
	.4byte 0xffff0009
	.4byte 0x02008cb1
	.4byte 0x00004400
	.4byte 0xffff0009
	.4byte 0x02008cb1
	.4byte 0x00008400
	.4byte 0xffff000a
	.4byte 0x02008cb1
	.4byte 0x00004400
	.4byte 0xffff000a
	.4byte 0x02008cb1
	.4byte 0x0000c400
	.4byte 0xffff000a
	.4byte 0x02008cb1
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02008cb1
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x02008cb1
	.4byte 0x0000c400
	.4byte 0xffff000d
	.4byte 0x02008cb1
	.4byte 0x00000400
	.4byte 0xffff000f
	.4byte 0x02008cb1
	.4byte 0x00008400
	.4byte 0xffff000f
	.4byte 0x02008cb1
	.4byte 0x0000c400
	.4byte 0xffff000f
	.4byte 0x02008cb1
	.4byte 0x00001815
	.4byte 0x02180010
	.4byte 0x02008861
	.4byte 0x00001815
	.4byte 0x02190011
	.4byte 0x02008861
	.4byte 0x00001815
	.4byte 0x021a0012
	.4byte 0x02008861
	.4byte 0x00001815
	.4byte 0x021b0013
	.4byte 0x02008861
	.4byte 0x00001815
	.4byte 0x021c0014
	.4byte 0x02008861
	.4byte 0x00001815
	.4byte 0x021e0016
	.4byte 0x02008861
	.4byte 0x10008c15
	.4byte 0xffff0008
	.4byte 0x02008771
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte 0x02008781
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02008771
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008781
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008325
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008325
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008d45
	.4byte 0x80008a05
	.4byte 0xffff00ff
	.4byte 0x02009369
	.4byte 0x50008a05
	.4byte 0xffff00ff
	.4byte 0x020092b5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008439
	.4byte 0x10008c15
	.4byte 0x02020008
	.4byte 0x020090bd
	.4byte 0x00008c15
	.4byte 0x02020008
	.4byte 0x020090cd
	.4byte 0x10008c15
	.4byte 0x02030009
	.4byte 0x020090bd
	.4byte 0x00008c15
	.4byte 0x02030009
	.4byte 0x020090cd
	.4byte 0x00000008
	.4byte 0x09820000
	.4byte 0x020090bd
	.4byte 0x00000009
	.4byte 0x09820000
	.4byte 0x020090cd
	.4byte 0x00002115
	.4byte 0x02040008
	.4byte 0x020090d9
	.4byte 0x00002115
	.4byte 0x02050009
	.4byte 0x020090f9
	.4byte 0x50001815
	.4byte 0x0210000a
	.4byte 0x02009119
	.4byte 0x50001815
	.4byte 0x0211000b
	.4byte 0x02009129
	.4byte 0x00001815
	.4byte 0x0210000a
	.4byte 0x02008819
	.4byte 0x00001815
	.4byte 0x0211000b
	.4byte 0x02008819
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte 0x02009139
	.4byte 0x00000006
	.4byte 0xffff00c9
	.4byte 0x020091f5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008439
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
