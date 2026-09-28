.syntax unified
	.thumb
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xa174
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
	.2byte 0xa1a4
	.2byte 0x0200
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push	{lr}
	ldr	r1, [pc, #60]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_02000064
	ldr	r0, [pc, #48]
	b.n	.L_0200008a
.L_02000064:
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #20
	bne.n	.L_02000076
	ldr	r0, [pc, #36]
	b.n	.L_0200008a
.L_02000076:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_02000088
	ldr	r0, [pc, #20]
	b.n	.L_0200008a
.L_02000088:
	ldr	r0, [pc, #20]
.L_0200008a:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000002e
	.4byte 0x0200a828
	.4byte 0x0200a750
	.4byte 0x0200a480
	.2byte 0xa240
	.2byte 0x0200
	push	{r5, lr}
	bl 0x02009d50
	movs	r0, #0
	bl 0x02009eb8
	ldr	r5, [pc, #68]
	adds	r0, r5, #0
	bl 0x02009e18
	movs	r1, #0
	movs	r0, #9
	bl 0x02009e20
	bl 0x02009ee8
	movs	r1, #0
	bl 0x02009d60
	cmp	r0, #0
	bne.n	.L_020000dc
	movs	r0, #10
	bl 0x02009d48
	adds	r0, r5, #1
	bl 0x02009e18
	b.n	.L_020000e8
.L_020000dc:
	movs	r0, #20
	bl 0x02009d48
	adds	r0, r5, #2
	bl 0x02009e18
.L_020000e8:
	movs	r0, #9
	movs	r1, #0
	bl 0x02009e30
	bl 0x02009d58
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x189c
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x02009d68
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
	bne.n	.L_02000134
	movs	r0, #1
	movs	r1, #22
	bl 0x02009f08
	b.n	.L_020001a8
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_02000134:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_02000184
	ldr	r5, [pc, #104]
	adds	r0, r5, #0
	bl 0x02009e18
	movs	r1, #0
	movs	r0, #22
	bl 0x02009e20
	bl 0x02009ee8
	movs	r1, #0
	bl 0x02009d60
	cmp	r0, #0
	bne.n	.L_0200016e
	movs	r0, #10
	bl 0x02009d48
	adds	r0, r5, #1
	bl 0x02009e18
	b.n	.L_0200017a
.L_0200016e:
	movs	r0, #20
	bl 0x02009d48
	adds	r0, r5, #2
	bl 0x02009e18
.L_0200017a:
	movs	r0, #22
	movs	r1, #0
	bl 0x02009e30
	b.n	.L_020001a8
.L_02000184:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_0200019a
	ldr	r0, [pc, #28]
	bl 0x02009e18
	b.n	.L_020001a0
.L_0200019a:
	ldr	r0, [pc, #24]
	bl 0x02009e18
.L_020001a0:
	movs	r0, #22
	movs	r1, #0
	bl 0x02009e30
.L_020001a8:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x00002126
	.4byte 0x00001d5a
	.2byte 0x18fe
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x02009d68
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #44]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_020001e8
	adds	r0, r5, #0
	bl 0x02009f00
	b.n	.L_02000230
.L_020001e8:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_0200020c
	ldr	r0, [pc, #16]
	bl 0x02009e18
	b.n	.L_02000228
	.2byte 0x0000
	.4byte 0xffffc000
	.4byte 0x02000240
	.2byte 0x2120
	.2byte 0x0000
.L_0200020c:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_02000222
	ldr	r0, [pc, #24]
	bl 0x02009e18
	b.n	.L_02000228
.L_02000222:
	ldr	r0, [pc, #20]
	bl 0x02009e18
.L_02000228:
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02009e30
.L_02000230:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x00001d54
	.2byte 0x18e2
	.2byte 0x0000
	push	{lr}
	bl 0x02009d50
	movs	r0, #0
	bl 0x02009eb8
	movs	r0, #14
	movs	r1, #4
	movs	r2, #0
	bl 0x02009df8
	movs	r0, #14
	movs	r1, #6
	movs	r2, #20
	bl 0x02009de0
	movs	r2, #0
	movs	r1, #14
	movs	r0, #4
	bl 0x02009df8
	ldr	r0, [pc, #32]
	bl 0x02009e18
	movs	r0, #14
	movs	r1, #0
	bl 0x02009e30
	movs	r1, #128
	movs	r0, #14
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009e38
	bl 0x02009d58
	pop	{pc}
	.2byte 0x0000
	.2byte 0x18aa
	.2byte 0x0000
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #138
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_020002bc
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_020002ac
	ldr	r0, [pc, #76]
	b.n	.L_020002ae
.L_020002ac:
	ldr	r0, [pc, #76]
.L_020002ae:
	bl 0x02009e18
	movs	r0, #10
	movs	r1, #0
	bl 0x02009e30
	b.n	.L_020002f6
.L_020002bc:
	movs	r0, #132
	lsls	r0, r0, #2
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_020002d8
	ldr	r0, [pc, #52]
	bl 0x02009e18
	movs	r0, #10
	movs	r1, #0
	bl 0x02009e30
	b.n	.L_020002e6
.L_020002d8:
	ldr	r0, [pc, #40]
	bl 0x02009e18
	movs	r0, #10
	movs	r1, #0
	bl 0x02009e30
.L_020002e6:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #173
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r3, #1
	strh	r3, [r2, #0]
.L_020002f6:
	pop	{pc}
	.4byte 0x0000207d
	.4byte 0x00002084
	.4byte 0x00002082
	.2byte 0x2071
	.2byte 0x0000
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #138
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_0200032c
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_02000328
	ldr	r0, [pc, #48]
	b.n	.L_0200033a
.L_02000328:
	ldr	r0, [pc, #48]
	b.n	.L_0200033a
.L_0200032c:
	movs	r0, #132
	lsls	r0, r0, #2
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_02000348
	ldr	r0, [pc, #36]
.L_0200033a:
	bl 0x02009e18
	movs	r0, #11
	movs	r1, #0
	bl 0x02009e30
	b.n	.L_02000356
.L_02000348:
	ldr	r0, [pc, #24]
	bl 0x02009e18
	movs	r0, #11
	movs	r1, #0
	bl 0x02009e30
.L_02000356:
	pop	{pc}
	.4byte 0x0000207e
	.4byte 0x00002085
	.4byte 0x00002083
	.2byte 0x2072
	.2byte 0x0000
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #138
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_0200038c
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_02000388
	ldr	r0, [pc, #52]
	b.n	.L_0200039c
.L_02000388:
	ldr	r0, [pc, #52]
	b.n	.L_0200039c
.L_0200038c:
	movs	r0, #132
	lsls	r0, r0, #2
	bl 0x02009d10
	ldr	r3, [pc, #44]
	cmp	r0, #0
	beq.n	.L_020003aa
	adds	r0, r3, #0
.L_0200039c:
	bl 0x02009e18
	movs	r0, #10
	movs	r1, #0
	bl 0x02009e30
	b.n	.L_020003b8
.L_020003aa:
	adds	r0, r3, #0
	bl 0x02009e18
	movs	r0, #10
	movs	r1, #0
	bl 0x02009e30
.L_020003b8:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0000207f
	.4byte 0x00002086
	.2byte 0x2073
	.2byte 0x0000
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #138
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_020003ec
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_020003e8
	ldr	r0, [pc, #52]
	b.n	.L_020003fc
.L_020003e8:
	ldr	r0, [pc, #52]
	b.n	.L_020003fc
.L_020003ec:
	movs	r0, #132
	lsls	r0, r0, #2
	bl 0x02009d10
	ldr	r3, [pc, #44]
	cmp	r0, #0
	beq.n	.L_0200040a
	adds	r0, r3, #0
.L_020003fc:
	bl 0x02009e18
	movs	r0, #11
	movs	r1, #0
	bl 0x02009e30
	b.n	.L_02000418
.L_0200040a:
	adds	r0, r3, #0
	bl 0x02009e18
	movs	r0, #11
	movs	r1, #0
	bl 0x02009e30
.L_02000418:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x00002080
	.4byte 0x00002087
	.2byte 0x2074
	.2byte 0x0000
	push	{lr}
	movs	r0, #137
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009d10
	cmp	r0, #0
	bne.n	.L_020004a0
	movs	r0, #137
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009d18
	ldr	r0, [pc, #120]
	bl 0x02009e18
	movs	r1, #12
	movs	r2, #0
	negs	r1, r1
	movs	r0, #14
	bl 0x02009ed8
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #2
	movs	r0, #14
	bl 0x02009df0
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #14
	movs	r1, #0
	bl 0x02009e30
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #14
	bl 0x02009e48
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #14
	movs	r1, #12
	movs	r2, #0
	bl 0x02009ed8
	movs	r0, #14
	movs	r1, #4
	movs	r2, #0
	bl 0x02009df8
	movs	r0, #14
	movs	r1, #0
	bl 0x02009e30
	b.n	.L_020004b8
.L_020004a0:
	ldr	r0, [pc, #28]
	bl 0x02009e18
	movs	r0, #14
	movs	r1, #4
	movs	r2, #0
	bl 0x02009df8
	movs	r0, #14
	movs	r1, #0
	bl 0x02009e30
.L_020004b8:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x00002052
	.2byte 0x2054
	.2byte 0x0000
	.global Func_020004c4
	.thumb_func
Func_020004c4:
	push	{lr}
	ldr	r1, [pc, #80]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_020004dc
	ldr	r0, [pc, #68]
	b.n	.L_02000514
.L_020004dc:
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #20
	bne.n	.L_020004ee
	ldr	r0, [pc, #56]
	b.n	.L_02000514
.L_020004ee:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_02000500
	ldr	r0, [pc, #40]
	b.n	.L_02000514
.L_02000500:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_02000512
	ldr	r0, [pc, #28]
	b.n	.L_02000514
.L_02000512:
	ldr	r0, [pc, #28]
.L_02000514:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000002e
	.4byte 0x0200ac9c
	.4byte 0x0200abb8
	.4byte 0x0200b08c
	.4byte 0x0200ada4
	.2byte 0xa8a0
	.2byte 0x0200
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_02000554
	movs	r1, #222
	movs	r2, #128
	movs	r0, #26
	lsls	r1, r1, #18
	lsls	r2, r2, #12
	bl 0x02009db8
	b.n	.L_0200056c
.L_02000554:
	movs	r0, #26
	bl 0x02009d68
	movs	r1, #242
	bl 0x02009d38
	movs	r0, #26
	bl 0x02009d68
	movs	r1, #0
	bl 0x02009d30
.L_0200056c:
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x02009d10
	cmp	r0, #0
	bne.n	.L_020005b4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #208
	lsls	r2, r2, #4
	adds	r2, #76
	movs	r0, #144
	adds	r3, r3, r2
	lsls	r0, r0, #4
	movs	r2, #3
	strh	r2, [r3, #0]
	adds	r0, #17
	bl 0x02009d10
	cmp	r0, #0
	bne.n	.L_020005a6
	bl 0x020087dc
	b.n	.L_02000614
.L_020005a6:
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x02009db8
	movs	r0, #10
	b.n	.L_020005e2
.L_020005b4:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_020005ec
	movs	r3, #224
	lsls	r3, r3, #8
	movs	r0, #9
	ldr	r1, [pc, #76]
	ldr	r2, [pc, #80]
	bl 0x02009dc0
	movs	r3, #192
	movs	r1, #236
	movs	r0, #10
	lsls	r1, r1, #17
	ldr	r2, [pc, #68]
	lsls	r3, r3, #8
	bl 0x02009dc0
	movs	r0, #8
.L_020005e2:
	movs	r1, #0
	movs	r2, #0
	bl 0x02009db8
	b.n	.L_02000614
.L_020005ec:
	movs	r3, #160
	lsls	r3, r3, #8
	movs	r0, #9
	ldr	r1, [pc, #36]
	ldr	r2, [pc, #36]
	bl 0x02009dc0
	movs	r3, #192
	movs	r1, #236
	movs	r0, #10
	lsls	r1, r1, #17
	ldr	r2, [pc, #28]
	lsls	r3, r3, #8
	bl 0x02009dc0
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x02009db8
.L_02000614:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02030000
	.4byte 0x03490000
	.2byte 0x0000
	.2byte 0x036d
	.global Func_02000624
	.thumb_func
Func_02000624:
	push	{r5, r6, lr}
	movs	r6, #192
	lsls	r6, r6, #18
	ldr	r3, [r6, #108]
	movs	r1, #214
	movs	r2, #133
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	adds	r3, r3, r1
	ldr	r5, [pc, #356]
	adds	r2, #255
	str	r2, [r3, #0]
	adds	r2, #11
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	sub	sp, #8
	bl 0x02009d68
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #0
	bl 0x02009e80
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r5, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #316]
	cmp	r2, r3
	bne.n	.L_020006e6
	ldr	r3, [r6, #108]
	movs	r2, #208
	lsls	r2, r2, #4
	adds	r2, #76
	movs	r0, #128
	adds	r3, r3, r2
	lsls	r0, r0, #4
	movs	r2, #3
	strh	r2, [r3, #0]
	adds	r0, #33
	bl 0x02009d10
	cmp	r0, #0
	bne.n	.L_0200069a
	movs	r0, #29
	movs	r1, #0
	movs	r2, #0
	bl 0x02009db8
	movs	r0, #30
	movs	r1, #0
	movs	r2, #0
	bl 0x02009db8
	b.n	.L_020006b2
.L_0200069a:
	movs	r0, #29
	bl 0x02009d68
	movs	r1, #3
	bl 0x02009e10
	movs	r0, #30
	bl 0x02009d68
	movs	r1, #5
	bl 0x02009e10
.L_020006b2:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_020006e6
	movs	r3, #64
	movs	r5, #16
	str	r3, [sp, #4]
	movs	r0, #2
	movs	r1, #110
	movs	r2, #16
	movs	r3, #10
	str	r5, [sp, #0]
	bl 0x02009d28
	movs	r3, #0
	str	r3, [sp, #4]
	movs	r0, #2
	movs	r1, #46
	movs	r2, #16
	movs	r3, #10
	str	r5, [sp, #0]
	bl 0x02009d20
.L_020006e6:
	ldr	r5, [pc, #180]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r5, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #20
	bne.n	.L_020006fa
	bl 0x02008570
.L_020006fa:
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r5, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #156]
	cmp	r2, r3
	bne.n	.L_0200076a
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_02000724
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x02009db8
	b.n	.L_0200076a
.L_02000724:
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #10
	bl 0x02009ef0
	movs	r0, #10
	bl 0x02009d68
	movs	r1, #0
	bl 0x02009d30
	movs	r0, #10
	bl 0x02009d68
	ldr	r1, [r0, #80]
	movs	r3, #8
	ldrb	r2, [r1, #26]
	movs	r0, #10
	orrs	r3, r2
	strb	r3, [r1, #26]
	bl 0x02009d68
	ldr	r1, [r0, #80]
	movs	r3, #3
	ldrb	r2, [r1, #17]
	movs	r0, #10
	ands	r3, r2
	movs	r2, #32
	orrs	r3, r2
	strb	r3, [r1, #17]
	bl 0x02009d68
	ldr	r2, [r0, #80]
	movs	r3, #1
	strb	r3, [r2, #25]
.L_0200076a:
	bl 0x02008534
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_0200078a
	movs	r0, #33
	bl 0x02009d68
	movs	r1, #2
	bl 0x02009e10
	b.n	.L_02000796
.L_0200078a:
	movs	r0, #28
	bl 0x02009d68
	movs	r1, #2
	bl 0x02009e10
.L_02000796:
	movs	r0, #0
	add	sp, #8
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x0000002d
	.2byte 0x002e
	.2byte 0x0000
	.global Func_020007a8
	.thumb_func
Func_020007a8:
	movs	r0, #0
	bx	lr
	push	{lr}
	ldr	r2, [r0, #80]
	cmp	r0, #0
	beq.n	.L_020007c0
	cmp	r2, #0
	beq.n	.L_020007c0
	ldrh	r3, [r2, #18]
	ldr	r1, [pc, #8]
	adds	r3, r3, r1
	strh	r3, [r2, #18]
.L_020007c0:
	movs	r0, #0
	pop	{pc}
	.2byte 0xf800
	.2byte 0xffff
	push	{lr}
	ldr	r2, [r0, #80]
	cmp	r0, #0
	beq.n	.L_020007d8
	cmp	r2, #0
	beq.n	.L_020007d8
	movs	r3, #0
	strh	r3, [r2, #18]
.L_020007d8:
	movs	r0, #0
	pop	{pc}
	push	{r5, r6, lr}
	bl 0x02009d50
	movs	r0, #0
	bl 0x02009eb8
	movs	r0, #8
	movs	r1, #1
	bl 0x02009dc8
	movs	r1, #200
	movs	r2, #222
	movs	r0, #4
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x02009db8
	movs	r0, #204
	movs	r1, #1
	movs	r2, #212
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #18
	bl 0x02009e60
	bl 0x02009e70
	bl 0x02009e78
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r1, #204
	adds	r2, #102
	movs	r0, #7
	bl 0x02009d70
	ldr	r0, [pc, #1016]
	bl 0x02009e18
	movs	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r2, #192
	movs	r1, #198
	lsls	r2, r2, #2
	movs	r0, #4
	lsls	r1, r1, #1
	adds	r2, #101
	bl 0x02009da0
	movs	r3, #192
	lsls	r3, r3, #8
	movs	r1, #25
	movs	r2, #19
	movs	r0, #11
	bl 0x02009ec8
	movs	r0, #11
	bl 0x02009d80
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x02009e38
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #11
	bl 0x02009e38
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #224
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009e38
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #11
	bl 0x02009e38
	movs	r0, #10
	bl 0x02009d48
	movs	r0, #244
	movs	r1, #1
	movs	r2, #205
	movs	r3, #1
	lsls	r2, r2, #18
	negs	r1, r1
	lsls	r0, r0, #17
	bl 0x02009e60
	bl 0x02009e68
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #2
	movs	r0, #7
	bl 0x02009df0
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #9
	bl 0x02009e48
	movs	r2, #10
	movs	r1, #0
	movs	r0, #9
	bl 0x02009e28
	movs	r0, #10
	bl 0x02009d48
	movs	r0, #7
	movs	r1, #4
	bl 0x02009dd0
	movs	r2, #10
	movs	r0, #7
	movs	r1, #0
	bl 0x02009e28
	movs	r0, #9
	movs	r1, #2
	bl 0x02009df0
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x02009e40
	movs	r2, #16
	negs	r2, r2
	movs	r1, #0
	movs	r0, #7
	bl 0x02009ed8
	movs	r0, #10
	bl 0x02009d48
	movs	r0, #7
	movs	r1, #4
	bl 0x02009dd0
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #4
	movs	r2, #0
	adds	r1, #255
	movs	r0, #9
	bl 0x02009e48
	movs	r0, #9
	bl 0x02009d68
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r5, #2
.L_0200094e:
	movs	r2, #8
	movs	r1, #0
	negs	r2, r2
	movs	r0, #9
	bl 0x02009ed8
	movs	r0, #134
	bl 0x02009f10
	subs	r5, #1
	movs	r0, #9
	movs	r1, #0
	movs	r2, #8
	bl 0x02009ed8
	cmp	r5, #0
	bge.n	.L_0200094e
	movs	r0, #9
	bl 0x02009d68
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r6, #1
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #30
	bl 0x02009d48
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #7
	bl 0x02009e48
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #7
	bl 0x02009e48
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #7
	bl 0x02009e38
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #9
	bl 0x02009e48
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r2, #0
	movs	r1, #9
	movs	r0, #10
	bl 0x02009df8
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #10
	movs	r1, #2
	bl 0x02009df0
	movs	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r2, #0
	movs	r1, #10
	movs	r0, #9
	bl 0x02009df8
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #9
	movs	r1, #2
	bl 0x02009df0
	movs	r2, #10
	movs	r1, #0
	movs	r0, #9
	bl 0x02009e28
	movs	r0, #10
	bl 0x02009d48
	movs	r0, #7
	movs	r1, #4
	bl 0x02009dd0
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #8
	movs	r2, #20
	adds	r1, #255
	movs	r0, #9
	bl 0x02009e48
	movs	r1, #192
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x02009e40
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #2
	movs	r0, #7
	bl 0x02009df0
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009e38
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #9
	bl 0x02009e38
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #10
	bl 0x02009e48
	movs	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #0
	movs	r2, #10
	movs	r0, #7
	bl 0x02009e28
	movs	r0, #20
	bl 0x02009d48
	movs	r2, #16
	movs	r0, #7
	movs	r1, #0
	bl 0x02009ed8
	movs	r0, #24
	movs	r1, #1
	bl 0x02009e90
	movs	r0, #7
	movs	r1, #8
	bl 0x02009e98
	bl 0x02009eb0
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #9
	bl 0x02009e48
	movs	r0, #9
	movs	r1, #7
	movs	r2, #0
	bl 0x02009df8
	movs	r2, #0
	movs	r1, #7
	movs	r0, #10
	bl 0x02009df8
	movs	r0, #9
	bl 0x02009d68
	movs	r1, #0
	bl 0x02009d30
	ldr	r1, [pc, #288]
	movs	r0, #9
	bl 0x02009d78
	movs	r0, #1
	bl 0x02009e88
	bl 0x02009ea0
	bl 0x02009ea8
	movs	r1, #0
	movs	r2, #10
	movs	r0, #11
	bl 0x02009e28
	movs	r0, #9
	bl 0x02009d88
	movs	r0, #9
	bl 0x02009d68
	movs	r5, #128
	lsls	r5, r5, #9
	str	r5, [r0, #24]
	movs	r0, #9
	bl 0x02009d68
	str	r5, [r0, #28]
	movs	r0, #4
	bl 0x02009d48
	movs	r0, #9
	bl 0x02009d68
	movs	r3, #2
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #9
	bl 0x02009d68
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	movs	r1, #8
	strb	r3, [r0, #0]
	movs	r2, #0
	movs	r0, #9
	negs	r1, r1
	bl 0x02009da8
	ldr	r1, [pc, #184]
	movs	r0, #9
	bl 0x02009d78
	movs	r0, #9
	bl 0x02009d80
	movs	r0, #127
	bl 0x02009f10
	movs	r0, #60
	bl 0x02009d48
	movs	r0, #9
	bl 0x02009d68
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r2, #0
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r1, #6
	movs	r0, #9
	bl 0x02009de0
	movs	r0, #3
	bl 0x02009d48
	movs	r1, #0
	movs	r0, #9
	bl 0x02009ef0
	movs	r0, #9
	bl 0x02009d68
	movs	r1, #1
	bl 0x02009d30
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #9
	bl 0x02009e38
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #0
	movs	r2, #0
	movs	r0, #10
	bl 0x02009e38
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #10
	bl 0x02009e38
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #128
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x02009e48
	movs	r1, #2
	movs	r0, #9
	bl 0x02009df0
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #10
	bl 0x02009d48
	movs	r1, #6
	movs	r2, #20
	movs	r0, #9
	bl 0x02009de0
	movs	r0, #20
	bl 0x02009d48
	b.n	.L_02000c30
	.2byte 0x0000
	.4byte 0x000018b4
	.4byte 0x0200b49c
	.2byte 0xb418
	.2byte 0x0200
.L_02000c30:
	movs	r1, #7
	movs	r2, #0
	movs	r0, #9
	bl 0x02009df8
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #10
	bl 0x02009d68
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #9
	bl 0x02009d68
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #16
	ands	r5, r3
	movs	r2, #16
	strb	r5, [r0, #0]
	negs	r1, r1
	movs	r0, #9
	bl 0x02009ed8
	movs	r1, #2
	movs	r0, #7
	bl 0x02009df0
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #9
	bl 0x02009e38
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #10
	bl 0x02009d48
	movs	r2, #20
	movs	r1, #6
	movs	r0, #9
	bl 0x02009de0
	movs	r0, #20
	bl 0x02009d48
.L_02000ca0:
	movs	r0, #9
	bl 0x02009d68
	adds	r0, #90
	ldrb	r3, [r0, #0]
	ldr	r5, [pc, #552]
	orrs	r3, r6
	strb	r3, [r0, #0]
	adds	r1, r5, #0
	movs	r0, #9
	bl 0x02009d78
	movs	r0, #4
	movs	r1, #9
	bl 0x02009ee0
	movs	r0, #11
	movs	r1, #9
	bl 0x02009ee0
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #2
	movs	r0, #10
	bl 0x02009df0
	movs	r0, #20
	bl 0x02009d48
	movs	r2, #0
	movs	r0, #10
	movs	r1, #7
	bl 0x02009df8
	movs	r0, #4
	movs	r1, #10
	bl 0x02009ee0
	movs	r0, #11
	movs	r1, #10
	bl 0x02009ee0
	movs	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #6
	adds	r1, #255
	movs	r2, #30
	movs	r0, #7
	bl 0x02009e48
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #10
	bl 0x02009e48
	movs	r1, #16
	movs	r0, #10
	negs	r1, r1
	movs	r2, #16
	bl 0x02009ed8
	movs	r2, #1
	movs	r0, #10
	movs	r1, #0
	bl 0x02009e28
	movs	r0, #7
	movs	r1, #4
	bl 0x02009dd0
	movs	r1, #0
	movs	r2, #10
	movs	r0, #7
	bl 0x02009e28
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #16
	movs	r2, #8
	negs	r1, r1
	movs	r0, #10
	bl 0x02009ed8
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #2
	movs	r0, #10
	bl 0x02009df0
	movs	r0, #20
	bl 0x02009d48
	movs	r2, #10
	movs	r1, #0
	movs	r0, #10
	bl 0x02009e28
.L_02000d7e:
	movs	r0, #10
	bl 0x02009d68
	adds	r0, #90
	ldrb	r3, [r0, #0]
	adds	r1, r5, #0
	orrs	r6, r3
	strb	r6, [r0, #0]
	movs	r0, #10
	bl 0x02009d78
	movs	r0, #40
	bl 0x02009d48
	movs	r0, #4
	bl 0x02009d88
	movs	r0, #11
	bl 0x02009d88
	movs	r0, #10
	bl 0x02009d80
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x02009db8
	movs	r2, #0
	movs	r0, #10
	movs	r1, #0
	bl 0x02009db8
	movs	r0, #8
	movs	r1, #6
	bl 0x02009dc8
	movs	r0, #8
	movs	r1, #0
	bl 0x02009e08
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #7
	bl 0x02009e38
	movs	r0, #8
	bl 0x02009dd8
	movs	r0, #4
	movs	r1, #1
	bl 0x02009e58
	bl 0x02009e68
	movs	r2, #0
	movs	r0, #11
	movs	r1, #4
	bl 0x02009e00
	movs	r1, #2
	movs	r0, #11
	bl 0x02009df0
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #0
	movs	r0, #11
	bl 0x02009e20
	movs	r0, #4
	movs	r1, #0
	bl 0x02009d60
	cmp	r0, #0
	bne.n	.L_02000e40
	movs	r0, #11
	movs	r1, #2
	bl 0x02009df0
	movs	r2, #10
	movs	r0, #11
	movs	r1, #0
	bl 0x02009e28
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02000e64
.L_02000e40:
	movs	r0, #11
	movs	r1, #4
	bl 0x02009dd0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #11
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
.L_02000e64:
	movs	r1, #2
	movs	r0, #11
	bl 0x02009df0
	movs	r0, #10
	bl 0x02009d48
	movs	r2, #10
	movs	r0, #11
	movs	r1, #0
	bl 0x02009e28
	movs	r0, #4
	movs	r1, #3
	bl 0x02009dc8
	movs	r1, #3
	movs	r0, #11
	bl 0x02009dd0
	movs	r0, #10
	bl 0x02009d48
	movs	r0, #11
	movs	r1, #2
	bl 0x02009dc8
	movs	r0, #4
	bl 0x02009d68
	cmp	r0, #0
	beq.n	.L_02000eb2
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #11
	bl 0x02009d90
.L_02000eb2:
	movs	r0, #11
	bl 0x02009db0
	movs	r1, #0
	movs	r2, #0
	movs	r0, #11
	bl 0x02009db8
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #17
	bl 0x02009d18
	bl 0x02009d58
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xb3c8
	.2byte 0x0200
	push	{lr}
	bl 0x02009d50
	movs	r0, #0
	bl 0x02009eb8
	ldr	r0, [pc, #32]
	bl 0x02009e18
	movs	r1, #128
	movs	r0, #11
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009e38
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	bl 0x02009d58
	pop	{pc}
	.2byte 0x0000
	.2byte 0x18cf
	.2byte 0x0000
	push	{lr}
	bl 0x02009d50
	movs	r0, #0
	bl 0x02009eb8
	ldr	r0, [pc, #192]
	bl 0x02009e18
	movs	r2, #10
	movs	r0, #7
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #2
	movs	r0, #7
	bl 0x02009df0
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #7
	bl 0x02009e38
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #0
	movs	r2, #10
	movs	r0, #7
	bl 0x02009e28
	movs	r0, #10
	bl 0x02009d48
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #7
	bl 0x02009e38
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #0
	movs	r2, #0
	movs	r0, #7
	bl 0x02009e38
	movs	r0, #10
	bl 0x02009d48
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #7
	bl 0x02009e38
	movs	r0, #10
	bl 0x02009d48
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #7
	bl 0x02009e38
	movs	r0, #10
	bl 0x02009d48
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #7
	bl 0x02009e38
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #7
	bl 0x02009e38
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	bl 0x02009d58
	pop	{pc}
	.2byte 0x18d0
	.2byte 0x0000
	push	{lr}
	movs	r0, #24
	movs	r1, #1
	bl 0x02009e90
	movs	r1, #8
	movs	r0, #7
	bl 0x02009e98
	bl 0x02009eb0
	movs	r0, #1
	bl 0x02009e88
	bl 0x02009ea0
	bl 0x02009ea8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #138
	bl 0x02009d18
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x02009d18
	movs	r0, #224
	lsls	r0, r0, #1
	bl 0x02009d40
	bl 0x02009d50
	movs	r0, #0
	bl 0x02009eb8
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #11
	adds	r1, #102
	adds	r2, #51
	bl 0x02009d70
	movs	r1, #205
	movs	r2, #109
	movs	r0, #11
	lsls	r1, r1, #1
	bl 0x02009d98
	movs	r0, #4
	bl 0x02009d68
	ldr	r3, [r0, #16]
	movs	r2, #190
	lsls	r2, r2, #15
	cmp	r3, r2
	ble.n	.L_02001074
	ldr	r0, [r0, #8]
	cmp	r0, #0
	bge.n	.L_0200106a
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r0, r0, r3
.L_0200106a:
	asrs	r1, r0, #16
	movs	r2, #95
	movs	r0, #4
	bl 0x02009da0
.L_02001074:
	movs	r1, #181
	movs	r2, #95
	movs	r0, #4
	lsls	r1, r1, #1
	bl 0x02009da0
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #4
	bl 0x02009e40
	movs	r0, #11
	bl 0x02009db0
	ldr	r0, [pc, #608]
	bl 0x02009e18
	movs	r1, #4
	movs	r2, #0
	movs	r0, #10
	bl 0x02009df8
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #10
	bl 0x02009e48
	movs	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #3
	movs	r0, #11
	bl 0x02009e48
	movs	r1, #189
	movs	r0, #11
	lsls	r1, r1, #1
	movs	r2, #109
	bl 0x02009da0
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #11
	bl 0x02009e38
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #11
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #11
	movs	r2, #0
	movs	r0, #10
	bl 0x02009e00
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #4
	movs	r2, #0
	movs	r0, #10
	bl 0x02009df8
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #10
	bl 0x02009e48
	movs	r2, #10
	movs	r0, #10
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #3
	movs	r0, #4
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #11
	bl 0x02009e48
	movs	r0, #11
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r2, #0
	movs	r1, #11
	movs	r0, #10
	bl 0x02009df8
	movs	r0, #30
	bl 0x02009d48
	movs	r1, #3
	movs	r0, #10
	bl 0x02009dd0
	movs	r0, #30
	bl 0x02009d48
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #10
	bl 0x02009e48
	movs	r2, #10
	movs	r0, #10
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #2
	movs	r0, #11
	bl 0x02009df0
	movs	r0, #20
	bl 0x02009d48
	movs	r2, #10
	movs	r0, #11
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #11
	bl 0x02009e40
	movs	r0, #10
	bl 0x02009d48
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #11
	bl 0x02009e40
	movs	r0, #10
	bl 0x02009d48
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #11
	bl 0x02009e40
	movs	r0, #10
	bl 0x02009d48
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #11
	bl 0x02009e48
	movs	r0, #25
	bl 0x02009d88
	movs	r1, #128
	movs	r2, #128
	movs	r0, #10
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x02009d70
	movs	r1, #128
	movs	r2, #128
	movs	r0, #11
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x02009d70
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #7
	movs	r0, #25
	lsls	r1, r1, #8
	bl 0x02009d70
	ldr	r1, [pc, #244]
	movs	r0, #11
	bl 0x02009d78
	ldr	r1, [pc, #240]
	movs	r0, #4
	bl 0x02009d78
	movs	r0, #60
	bl 0x02009d48
	ldr	r1, [pc, #228]
	movs	r0, #10
	bl 0x02009d78
	movs	r0, #10
	bl 0x02009d80
	movs	r0, #120
	bl 0x02009d48
	ldr	r1, [pc, #212]
	movs	r0, #11
	bl 0x02009d78
	movs	r1, #11
	movs	r0, #4
	bl 0x02009ee0
	movs	r0, #60
	bl 0x02009d48
	ldr	r1, [pc, #196]
	movs	r0, #10
	bl 0x02009d78
	ldr	r1, [pc, #192]
	movs	r0, #25
	bl 0x02009d78
	movs	r0, #10
	bl 0x02009d80
	movs	r0, #11
	bl 0x02009d80
	movs	r0, #25
	bl 0x02009d80
	movs	r1, #2
	movs	r0, #11
	bl 0x02009df0
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #11
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r0, #25
	movs	r1, #2
	movs	r2, #10
	bl 0x02009ec0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #4
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #1
	bl 0x02009dc8
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #10
	bl 0x02009e38
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #3
	movs	r0, #10
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x02009e38
	movs	r2, #10
	movs	r0, #10
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #3
	movs	r0, #11
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #11
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	bl 0x02009d58
	pop	{pc}
	.2byte 0x0000
	.4byte 0x00002075
	.4byte 0x02009f18
	.4byte 0x02009f4c
	.4byte 0x02009f90
	.4byte 0x02009fc4
	.4byte 0x0200a054
	.2byte 0xa0e4
	.2byte 0x0200
	push	{lr}
	movs	r0, #132
	lsls	r0, r0, #2
	bl 0x02009d18
	bl 0x02009d50
	movs	r0, #0
	bl 0x02009eb8
	movs	r0, #4
	bl 0x02009d68
	ldr	r3, [r0, #16]
	movs	r2, #190
	lsls	r2, r2, #15
	cmp	r3, r2
	ble.n	.L_0200134c
	ldr	r0, [r0, #8]
	cmp	r0, #0
	bge.n	.L_02001342
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r0, r0, r3
.L_02001342:
	asrs	r1, r0, #16
	movs	r2, #95
	movs	r0, #4
	bl 0x02009da0
.L_0200134c:
	movs	r1, #181
	movs	r2, #95
	movs	r0, #4
	lsls	r1, r1, #1
	bl 0x02009da0
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #4
	bl 0x02009e40
	ldr	r0, [pc, #228]
	bl 0x02009e18
	movs	r1, #4
	movs	r2, #0
	movs	r0, #10
	bl 0x02009df8
	movs	r0, #30
	bl 0x02009d48
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #10
	bl 0x02009e48
	movs	r0, #10
	movs	r1, #0
	movs	r2, #5
	bl 0x02009e28
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #3
	movs	r0, #11
	bl 0x02009e48
	movs	r1, #4
	movs	r2, #0
	movs	r0, #11
	bl 0x02009df8
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #0
	movs	r2, #5
	movs	r0, #11
	bl 0x02009e28
	ldr	r0, [pc, #148]
	bl 0x02009e18
	movs	r1, #128
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x02009e48
	movs	r1, #0
	movs	r0, #10
	bl 0x02009e40
	movs	r0, #10
	bl 0x02009d48
	movs	r0, #10
	movs	r1, #0
	movs	r2, #5
	bl 0x02009e28
	movs	r1, #11
	movs	r2, #0
	movs	r0, #10
	bl 0x02009e00
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #4
	movs	r2, #0
	movs	r0, #10
	bl 0x02009df8
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #10
	bl 0x02009e48
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #10
	movs	r1, #0
	movs	r2, #5
	bl 0x02009e28
	movs	r2, #0
	movs	r1, #4
	movs	r0, #11
	bl 0x02009df8
	movs	r0, #10
	bl 0x02009d48
	movs	r1, #2
	movs	r0, #11
	bl 0x02009df0
	movs	r0, #5
	bl 0x02009d48
	movs	r0, #11
	movs	r1, #0
	movs	r2, #5
	bl 0x02009e28
	bl 0x02009d58
	pop	{pc}
	.4byte 0x00002075
	.2byte 0x2081
	.2byte 0x0000
	push	{lr}
	bl 0x02009d50
	movs	r0, #0
	bl 0x02009eb8
	ldr	r0, [pc, #764]
	bl 0x02009e18
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #28
	bl 0x02009e48
	movs	r0, #28
	movs	r1, #4
	movs	r2, #0
	bl 0x02009df8
	movs	r0, #28
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r0, #15
	movs	r1, #4
	movs	r2, #0
	bl 0x02009df8
	movs	r1, #176
	movs	r0, #17
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009e38
	movs	r0, #18
	movs	r1, #4
	movs	r2, #0
	bl 0x02009df8
	movs	r0, #14
	movs	r1, #4
	movs	r2, #0
	bl 0x02009df8
	movs	r0, #15
	movs	r1, #4
	movs	r2, #0
	bl 0x02009df8
	movs	r1, #4
	movs	r2, #0
	movs	r0, #16
	bl 0x02009df8
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #223
	lsls	r1, r1, #1
	movs	r0, #4
	adds	r1, #255
	movs	r2, #93
	bl 0x02009da0
	movs	r1, #32
	movs	r0, #4
	negs	r1, r1
	movs	r2, #0
	bl 0x02009ed0
	movs	r1, #8
	movs	r3, #128
	movs	r0, #5
	negs	r1, r1
	movs	r2, #32
	lsls	r3, r3, #8
	bl 0x02009ec8
	movs	r1, #16
	movs	r3, #160
	movs	r0, #31
	negs	r1, r1
	movs	r2, #0
	lsls	r3, r3, #7
	bl 0x02009ec8
	movs	r1, #8
	movs	r3, #192
	movs	r0, #6
	negs	r1, r1
	movs	r2, #16
	lsls	r3, r3, #7
	bl 0x02009ec8
	movs	r1, #24
	movs	r3, #192
	lsls	r3, r3, #7
	negs	r1, r1
	movs	r2, #16
	movs	r0, #7
	bl 0x02009ec8
	movs	r0, #30
	bl 0x02009d48
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #15
	bl 0x02009e48
	movs	r0, #15
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #27
	bl 0x02009e48
	movs	r0, #20
	bl 0x02009d48
	movs	r2, #0
	movs	r1, #0
	movs	r0, #27
	bl 0x02009e38
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #2
	movs	r0, #27
	bl 0x02009df0
	movs	r0, #20
	bl 0x02009d48
	movs	r2, #10
	movs	r0, #27
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #3
	movs	r0, #7
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #27
	bl 0x02009e48
	movs	r0, #27
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #4
	movs	r2, #0
	movs	r0, #7
	bl 0x02009e00
	movs	r0, #30
	bl 0x02009d48
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009e38
	movs	r1, #192
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #4
	bl 0x02009e38
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #6
	movs	r2, #20
	movs	r0, #18
	bl 0x02009de0
	movs	r0, #20
	bl 0x02009d48
	movs	r2, #10
	movs	r0, #18
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #6
	bl 0x02009e40
	movs	r1, #3
	movs	r0, #7
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #28
	bl 0x02009e48
	movs	r1, #0
	movs	r0, #28
	bl 0x02009e20
	movs	r0, #4
	movs	r1, #0
	bl 0x02009d60
	cmp	r0, #0
	bne.n	.L_02001690
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #27
	bl 0x02009e48
	movs	r2, #10
	movs	r0, #27
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #7
	bl 0x02009e40
	movs	r1, #3
	movs	r0, #7
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #15
	bl 0x02009e48
	movs	r2, #10
	movs	r0, #15
	movs	r1, #0
	bl 0x02009e28
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #3
	strh	r3, [r2, #0]
	b.n	.L_02001700
.L_02001690:
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #15
	bl 0x02009e48
	bl 0x02009ef8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #27
	adds	r3, #3
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #7
	bl 0x02009e40
	movs	r1, #4
	movs	r0, #7
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r2, #10
	movs	r0, #7
	movs	r1, #0
	bl 0x02009e28
	movs	r0, #15
	movs	r1, #3
	bl 0x02009de8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #15
	bl 0x02009e50
	movs	r0, #50
	bl 0x02009d48
	movs	r0, #15
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
.L_02001700:
	movs	r0, #7
	movs	r1, #3
	bl 0x02009dc8
	movs	r0, #6
	movs	r1, #3
	bl 0x02009dc8
	movs	r0, #31
	movs	r1, #3
	bl 0x02009dc8
	movs	r0, #5
	movs	r1, #3
	bl 0x02009dc8
	movs	r1, #3
	movs	r0, #4
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #3
	movs	r0, #31
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #31
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #129
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_02001760
	bl 0x02009910
	b.n	.L_02001764
	.2byte 0x0000
	.2byte 0x2093
	.2byte 0x0000
.L_02001760:
	bl 0x02009a20
.L_02001764:
	movs	r1, #128
	movs	r2, #0
	movs	r0, #7
	lsls	r1, r1, #7
	bl 0x02009e38
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #4
	bl 0x02009e40
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #7
	movs	r1, #3
	bl 0x02009dc8
	movs	r1, #3
	movs	r0, #4
	bl 0x02009dd0
	movs	r0, #40
	bl 0x02009d48
	movs	r1, #160
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009e38
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009e38
	movs	r1, #192
	movs	r0, #31
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009e38
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009e38
	movs	r2, #0
	movs	r1, #0
	movs	r0, #7
	bl 0x02009e38
	movs	r0, #40
	bl 0x02009d48
	movs	r0, #5
	movs	r1, #3
	bl 0x02009dc8
	movs	r0, #6
	movs	r1, #3
	bl 0x02009dc8
	movs	r0, #31
	movs	r1, #3
	bl 0x02009dc8
	movs	r0, #7
	movs	r1, #3
	bl 0x02009dc8
	movs	r1, #3
	movs	r0, #4
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #31
	ldr	r1, [pc, #256]
	adds	r2, #153
	bl 0x02009d70
	movs	r0, #31
	movs	r1, #2
	bl 0x02009dc8
	movs	r0, #4
	bl 0x02009d68
	cmp	r0, #0
	beq.n	.L_02001832
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #31
	bl 0x02009d90
.L_02001832:
	movs	r0, #31
	bl 0x02009db0
	movs	r0, #31
	movs	r1, #0
	movs	r2, #0
	bl 0x02009db8
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #192]
	adds	r2, #153
	bl 0x02009d70
	movs	r0, #6
	movs	r1, #2
	bl 0x02009dc8
	movs	r0, #4
	bl 0x02009d68
	cmp	r0, #0
	beq.n	.L_02001870
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #6
	bl 0x02009d90
.L_02001870:
	movs	r0, #6
	bl 0x02009db0
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x02009db8
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #7
	ldr	r1, [pc, #132]
	adds	r2, #153
	bl 0x02009d70
	movs	r0, #7
	movs	r1, #2
	bl 0x02009dc8
	movs	r0, #4
	bl 0x02009d68
	cmp	r0, #0
	beq.n	.L_020018ae
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #7
	bl 0x02009d90
.L_020018ae:
	movs	r0, #7
	bl 0x02009db0
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x02009db8
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #5
	ldr	r1, [pc, #68]
	adds	r2, #153
	bl 0x02009d70
	movs	r0, #5
	movs	r1, #2
	bl 0x02009dc8
	movs	r0, #4
	bl 0x02009d68
	cmp	r0, #0
	beq.n	.L_020018ec
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x02009d90
.L_020018ec:
	movs	r0, #5
	bl 0x02009db0
	movs	r1, #0
	movs	r2, #0
	movs	r0, #5
	bl 0x02009db8
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #137
	bl 0x02009d18
	bl 0x02009d58
	pop	{pc}
	.2byte 0x3333
	.2byte 0x0001
	push	{lr}
	ldr	r0, [pc, #264]
	bl 0x02009e18
	movs	r0, #28
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #31
	movs	r2, #0
	movs	r0, #4
	bl 0x02009e00
	movs	r0, #40
	bl 0x02009d48
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009e38
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #31
	bl 0x02009e38
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #3
	movs	r0, #4
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #15
	bl 0x02009e48
	movs	r2, #10
	movs	r0, #15
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #3
	movs	r0, #7
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #27
	bl 0x02009e48
	movs	r2, #10
	movs	r0, #27
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #3
	movs	r0, #31
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #31
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #5
	bl 0x02009e48
	movs	r2, #10
	movs	r0, #5
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #3
	movs	r0, #17
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r2, #10
	movs	r0, #17
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	bl 0x02009e40
	movs	r1, #3
	movs	r0, #5
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #3
	movs	r0, #18
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	pop	{pc}
	.2byte 0x0000
	.2byte 0x20a2
	.2byte 0x0000
	push	{lr}
	ldr	r0, [pc, #744]
	bl 0x02009e18
	movs	r1, #3
	movs	r0, #17
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r2, #10
	movs	r0, #17
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	bl 0x02009e40
	movs	r1, #4
	movs	r0, #5
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #5
	movs	r2, #0
	movs	r0, #6
	bl 0x02009df8
	movs	r0, #20
	bl 0x02009d48
	movs	r2, #10
	movs	r0, #6
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x02009e40
	movs	r1, #3
	movs	r0, #5
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #17
	bl 0x02009e48
	movs	r0, #7
	movs	r1, #17
	movs	r2, #0
	bl 0x02009df8
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #7
	bl 0x02009e48
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #31
	bl 0x02009e48
	movs	r0, #31
	movs	r1, #7
	movs	r2, #0
	bl 0x02009df8
	movs	r0, #31
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r2, #0
	movs	r1, #31
	movs	r0, #7
	bl 0x02009df8
	movs	r0, #60
	bl 0x02009d48
	movs	r1, #3
	movs	r0, #7
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r0, #4
	movs	r1, #7
	movs	r2, #0
	bl 0x02009df8
	movs	r0, #6
	movs	r1, #7
	movs	r2, #0
	bl 0x02009df8
	movs	r1, #7
	movs	r2, #0
	movs	r0, #5
	bl 0x02009df8
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #6
	bl 0x02009e48
	movs	r0, #6
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #6
	movs	r2, #0
	movs	r0, #7
	bl 0x02009df8
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #4
	bl 0x02009e48
	movs	r1, #4
	movs	r2, #0
	movs	r0, #7
	bl 0x02009df8
	movs	r0, #20
	bl 0x02009d48
	movs	r2, #10
	movs	r0, #7
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #3
	movs	r0, #7
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #5
	bl 0x02009e48
	movs	r2, #10
	movs	r0, #5
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #4
	movs	r0, #7
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #6
	bl 0x02009e40
	movs	r2, #10
	movs	r0, #7
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #3
	movs	r0, #5
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #31
	movs	r2, #0
	movs	r0, #7
	bl 0x02009df8
	movs	r0, #20
	bl 0x02009d48
	movs	r2, #10
	movs	r0, #7
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #3
	movs	r0, #31
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #4
	movs	r2, #0
	movs	r0, #7
	bl 0x02009df8
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #0
	movs	r2, #10
	movs	r0, #7
	bl 0x02009e28
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #129
	bl 0x02009d18
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #15
	bl 0x02009e48
	movs	r2, #10
	movs	r0, #15
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #7
	bl 0x02009e40
	movs	r1, #3
	movs	r0, #7
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009e38
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #27
	bl 0x02009e48
	movs	r2, #10
	movs	r0, #27
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #3
	movs	r0, #31
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #31
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #5
	bl 0x02009e48
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009e38
	movs	r2, #10
	movs	r0, #5
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #3
	movs	r0, #17
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r2, #10
	movs	r0, #17
	movs	r1, #0
	bl 0x02009e28
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	bl 0x02009e40
	movs	r1, #3
	movs	r0, #5
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r1, #3
	movs	r0, #18
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d48
	movs	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x02009e28
	pop	{pc}
	.2byte 0x0000
	.4byte 0x000020aa
	.section .rodata,"a",%progbits
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000030
	.4byte 0x01640000
	.4byte 0x00480000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01440000
	.4byte 0x00000000
	.4byte 0x00490000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000030
	.4byte 0x01530000
	.4byte 0x006f0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000030
	.4byte 0x01640000
	.4byte 0x00480000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01440000
	.4byte 0x00000000
	.4byte 0x00490000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x01470000
	.4byte 0x00000000
	.4byte 0x00470000
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x01750000
	.4byte 0x006d0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x01470000
	.4byte 0x00000000
	.4byte 0x00470000
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x01750000
	.4byte 0x00610000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x01470000
	.4byte 0x00180000
	.4byte 0x00470000
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x01750000
	.4byte 0x00610000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
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
	.4byte 0x0000002d
	.4byte 0x1010202c
	.4byte 0xffffffff
	.4byte 0x1020302c
	.4byte 0xffffffff
	.4byte 0x1030402d
	.4byte 0xffffffff
	.4byte 0x1040302d
	.4byte 0xffffffff
	.4byte 0x1050702c
	.4byte 0xffffffff
	.4byte 0x1060702d
	.4byte 0xffffffff
	.4byte 0x1070602d
	.4byte 0xffffffff
	.4byte 0x1080402c
	.4byte 0xffffffff
	.4byte 0x1090802c
	.4byte 0xffffffff
	.4byte 0x10a0502c
	.4byte 0xffffffff
	.4byte 0x10b0c02d
	.4byte 0xffffffff
	.4byte 0x10c0b02d
	.4byte 0xffffffff
	.4byte 0x10d0e02d
	.4byte 0xffffffff
	.4byte 0x10e0d02d
	.4byte 0xffffffff
	.4byte 0x10f0602c
	.4byte 0xffffffff
	.4byte 0x1140902c
	.4byte 0xffffffff
	.4byte 0x0000002e
	.4byte 0x1010102c
	.4byte 0xffffffff
	.4byte 0x1020a02c
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff0049
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00012000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x006d0000
	.4byte 0x00014000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00016000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x0001a000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00012000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x02800000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0001a000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x02670000
	.4byte 0x00000000
	.4byte 0x01370000
	.4byte 0x00015000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x016c0000
	.4byte 0x0001c000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x015c0000
	.4byte 0x00014000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x022c0000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x0001e000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x0001a000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x025c0000
	.4byte 0x00000000
	.4byte 0x02240000
	.4byte 0x00012000
	.4byte 0xffff00d1
	.4byte 0x00000003
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0001e000
	.4byte 0xffff0048
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x02060000
	.4byte 0x0001e000
	.4byte 0x006200f5
	.4byte 0x00000002
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00004000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00380000
	.4byte 0x00024000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x02a40000
	.4byte 0x00000000
	.4byte 0x02fa0000
	.4byte 0x00018000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x02da0000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00014000
	.4byte 0x08ab00ef
	.4byte 0x00000001
	.4byte 0x034f0000
	.4byte 0x00000000
	.4byte 0x02c70000
	.4byte 0x00012000
	.4byte 0x08ab00ef
	.4byte 0x00000001
	.4byte 0x038a0000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00010000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0049
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00012000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x006d0000
	.4byte 0x00014000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00016000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x0001a000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x0000e000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x02800000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0000a000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x026c0000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0000e000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0000c000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x0001e000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x0001a000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x025c0000
	.4byte 0x00000000
	.4byte 0x02240000
	.4byte 0x00012000
	.4byte 0xffff00d1
	.4byte 0x00000003
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0001e000
	.4byte 0xffff0048
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x02060000
	.4byte 0x0001e000
	.4byte 0x006200f5
	.4byte 0x00000002
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00004000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00380000
	.4byte 0x00024000
	.4byte 0xffff0013
	.4byte 0x00000001
	.4byte 0x02660000
	.4byte 0x00000000
	.4byte 0x006f0000
	.4byte 0x00002000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x027c0000
	.4byte 0x00000000
	.4byte 0x006f0000
	.4byte 0x00005000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x05080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00008000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x05080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00008000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x05080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00008000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x05080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0000c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x02a40000
	.4byte 0x00000000
	.4byte 0x02fa0000
	.4byte 0x00018000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x02da0000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x02100000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x0001c000
	.4byte 0xffff00c0
	.4byte 0x00000001
	.4byte 0x020a0000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x0001c000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x0001c000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0x08ab0007
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x0002c000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x016a0000
	.4byte 0x00000000
	.4byte 0x034f0000
	.4byte 0x0002c000
	.4byte 0x18ff00bb
	.4byte 0x00000001
	.4byte 0x018a0000
	.4byte 0x00000000
	.4byte 0x035f0000
	.4byte 0x0001a000
	.4byte 0x18ff00bb
	.4byte 0x00000001
	.4byte 0x018c0000
	.4byte 0x00000000
	.4byte 0x03500000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00010000
	.4byte 0xffff0048
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00014000
	.4byte 0xffff00c6
	.4byte 0x00000001
	.4byte 0x01640000
	.4byte 0x00000000
	.4byte 0x006d0000
	.4byte 0x00024000
	.4byte 0x18ab00ba
	.4byte 0x00000001
	.4byte 0x01650000
	.4byte 0x00000000
	.4byte 0x00830000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
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
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000189b
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020080a5
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000018a1
	.4byte 0x0000c400
	.4byte 0xffff000b
	.4byte 0x000018a2
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000018a5
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000018a6
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000018a9
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000018ab
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x000018ac
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000018ad
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x000018ae
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x020081b9
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x000018e3
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x000018e4
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x020080fd
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x000018ff
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00001900
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x0200823d
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x00001901
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x00001902
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte 0x00002f59
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x00002f5a
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000189f
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000018a0
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000018a3
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000018a4
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000018a7
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000018a8
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000018af
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000018b0
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000018b1
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000018b2
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000018b3
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x000018e5
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x000018e6
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x000018e7
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001903
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001904
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001905
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x00001906
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x00001907
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte 0x00002f5b
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte 0x00002f5c
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x00403043
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x00403044
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x00403045
	.4byte 0x00000173
	.4byte 0xffff00cb
	.4byte 0x00403046
	.4byte 0x00000173
	.4byte 0xffff00cc
	.4byte 0x00403047
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000000
	.4byte 0x18ff0009
	.4byte 0x00002114
	.4byte 0x00000000
	.4byte 0x18ff000a
	.4byte 0x00002115
	.4byte 0x0000c400
	.4byte 0x18ff000d
	.4byte 0x00002051
	.4byte 0x0000a400
	.4byte 0x18ff000d
	.4byte 0x00002051
	.4byte 0x00008400
	.4byte 0x18ff000d
	.4byte 0x00002051
	.4byte 0x00000000
	.4byte 0x18ff000e
	.4byte 0x02008429
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001d48
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001d49
	.4byte 0x00000000
	.4byte 0xffff0007
	.4byte 0x02008ed9
	.4byte 0x00008d15
	.4byte 0x18ff0009
	.4byte 0x00002116
	.4byte 0x00008d15
	.4byte 0x18ff000a
	.4byte 0x00002117
	.4byte 0x00008d15
	.4byte 0x18ff000d
	.4byte 0x00002067
	.4byte 0x00008d15
	.4byte 0x1211000e
	.4byte 0x00002068
	.4byte 0x00008d15
	.4byte 0x18ff040e
	.4byte 0x02008429
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001d4a
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001d4b
	.4byte 0x00008d15
	.4byte 0xffff0007
	.4byte 0x02008f0d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x08ab0008
	.4byte 0x00001897
	.4byte 0x00000000
	.4byte 0x08ff0008
	.4byte 0x00001d2e
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002069
	.4byte 0x00000000
	.4byte 0x08ab0009
	.4byte 0x00001898
	.4byte 0x00000000
	.4byte 0x08ff0009
	.4byte 0x00001d2f
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000206a
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000018df
	.4byte 0x00000000
	.4byte 0x08ff000b
	.4byte 0x00001d51
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0000211d
	.4byte 0x00008d15
	.4byte 0x08ab0008
	.4byte 0x00001899
	.4byte 0x00008d15
	.4byte 0x08ff0008
	.4byte 0x00001d30
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000206b
	.4byte 0x00008d15
	.4byte 0x08ab0009
	.4byte 0x0000189a
	.4byte 0x00008d15
	.4byte 0x08ff0009
	.4byte 0x00001d31
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000206c
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000018e1
	.4byte 0x00008d15
	.4byte 0x08ff000b
	.4byte 0x00001d53
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000211f
	.4byte 0x00000173
	.4byte 0xffff00cd
	.4byte 0x00403048
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
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001d32
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001d33
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001d36
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001d37
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001d3a
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001d3b
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001d3e
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001d3f
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001d40
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001d41
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001d42
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x020081b9
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001d55
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001d56
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x020080fd
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00001d5b
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00001d5c
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x0200823d
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x00001d5d
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x00001d5e
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001d34
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001d35
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001d38
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001d39
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001d3c
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001d3d
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001d43
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001d44
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001d45
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001d46
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001d47
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001d57
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001d58
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001d59
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001d5f
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001d60
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001d61
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x00001d62
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x00001d63
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x00403043
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x00403044
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x00403045
	.4byte 0x00000173
	.4byte 0xffff00cb
	.4byte 0x00403046
	.4byte 0x00000173
	.4byte 0xffff00cc
	.4byte 0x00403047
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
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000002
	.4byte 0x0989001e
	.4byte 0x02009451
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000206d
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000206e
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0200828d
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02008309
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002088
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002089
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x000020bf
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x000020c0
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000020c1
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000020c2
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x000020c3
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000020c4
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x000020c5
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x020081b9
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00002121
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00002122
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x020080fd
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00002129
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x0000212a
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x0200823d
	.4byte 0x00000000
	.4byte 0xffff0020
	.4byte 0x0000212b
	.4byte 0x00000000
	.4byte 0xffff0021
	.4byte 0x0000212c
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000206f
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002070
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x02008369
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x020083c9
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000208a
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000208b
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x000020c6
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x000020c7
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000020c8
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000020c9
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000020ca
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000020cb
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000020cc
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00002123
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002124
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002125
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x0000212d
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x0000212e
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x0000212f
	.4byte 0x00008d15
	.4byte 0xffff0020
	.4byte 0x00002130
	.4byte 0x00008d15
	.4byte 0xffff0021
	.4byte 0x00002131
	.4byte 0x0001c014
	.4byte 0x098a000a
	.4byte 0x02009009
	.4byte 0x0001c114
	.4byte 0x098a000a
	.4byte 0x02009311
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x00403043
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x00403044
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x00403045
	.4byte 0x00000173
	.4byte 0xffff00cb
	.4byte 0x00403046
	.4byte 0x00000173
	.4byte 0xffff00cc
	.4byte 0x00403047
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x03980000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x0000002e
	.4byte 0x020087ad
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte 0x020087ad
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte 0x020087ad
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte 0x020087ad
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte 0x020087ad
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte 0x020087ad
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte 0x020087ad
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte 0x020087ad
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000064
	.4byte 0x00000000
	.4byte 0x00000024
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00030000
	.4byte 0x00000000
	.4byte 0x00000009
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00050000
	.4byte 0x00000017
	.4byte 0x00000005
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000009
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x00000009
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000017
	.4byte 0x00000005
	.4byte 0x00010000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00050000
	.4byte 0x00000000
	.4byte 0x00000009
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x00000009
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000017
	.4byte 0x00000005
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000009
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000017
	.4byte 0x00000005
	.4byte 0x00010000
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xffff0000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00010000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xffff0000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00010000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
