.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008341, 0x02008039, 0x02008045, 0x0200804d, 0x020080c9, 0x02008041, 0x02008451
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x87c4
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x87f4
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	cmp	r2, r3
	bne.n	.L_02000064
	ldr	r0, [pc, #12]
	b.n	.L_02000066
.L_02000064:
	ldr	r0, [pc, #12]
.L_02000066:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000003a
	.4byte 0x02008960
	.2byte 0x8840
	.2byte 0x0200
	push	{lr}
	bl 0x020085c4
	movs	r0, #0
	bl 0x0200862c
	ldr	r0, [pc, #20]
	bl 0x02008604
	movs	r0, #8
	movs	r1, #0
	bl 0x02008614
	bl 0x020085cc
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x1949
	.2byte 0x0000
	push	{lr}
	bl 0x020085c4
	movs	r0, #0
	bl 0x0200862c
	ldr	r0, [pc, #20]
	bl 0x02008604
	movs	r0, #9
	movs	r1, #0
	bl 0x02008614
	bl 0x020085cc
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x229a
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	cmp	r2, r3
	bne.n	.L_020000e0
	ldr	r0, [pc, #12]
	b.n	.L_020000e2
.L_020000e0:
	ldr	r0, [pc, #12]
.L_020000e2:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000003a
	.4byte 0x02008bac
	.2byte 0x8a20
	.2byte 0x0200
	push	{r5, lr}
	bl 0x020085c4
	movs	r0, #0
	bl 0x0200862c
	ldr	r5, [pc, #68]
	adds	r0, r5, #0
	bl 0x02008604
	movs	r1, #0
	movs	r0, #10
	bl 0x0200860c
	bl 0x02008634
	movs	r1, #0
	bl 0x020085d4
	cmp	r0, #0
	bne.n	.L_0200012c
	movs	r0, #10
	bl 0x020085bc
	adds	r0, r5, #1
	bl 0x02008604
	b.n	.L_02000138
.L_0200012c:
	movs	r0, #20
	bl 0x020085bc
	adds	r0, r5, #2
	bl 0x02008604
.L_02000138:
	movs	r0, #10
	movs	r1, #0
	bl 0x02008614
	bl 0x020085cc
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1932
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x020085dc
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
	bne.n	.L_02000188
	movs	r0, #13
	adds	r1, r5, #0
	bl 0x0200863c
	b.n	.L_02000196
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_02000188:
	ldr	r0, [pc, #12]
	bl 0x02008604
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008614
.L_02000196:
	pop	{r5, pc}
	.2byte 0x1940
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x020085dc
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
	bne.n	.L_020001d8
	movs	r0, #4
	adds	r1, r5, #0
	bl 0x0200864c
	b.n	.L_020001f4
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_020001d8:
	bl 0x020085c4
	movs	r0, #0
	bl 0x0200862c
	ldr	r0, [pc, #20]
	bl 0x02008604
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008614
	bl 0x020085cc
.L_020001f4:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1943
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	bl 0x020085dc
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
	bne.n	.L_02000234
	adds	r0, r6, #0
	bl 0x02008644
	b.n	.L_02000282
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_02000234:
	bl 0x020085c4
	movs	r0, #0
	bl 0x0200862c
	ldr	r5, [pc, #68]
	adds	r0, r5, #0
	bl 0x02008604
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x0200860c
	bl 0x02008634
	movs	r1, #0
	bl 0x020085d4
	cmp	r0, #0
	bne.n	.L_0200026a
	movs	r0, #10
	bl 0x020085bc
	adds	r0, r5, #1
	bl 0x02008604
	b.n	.L_02000276
.L_0200026a:
	movs	r0, #20
	bl 0x020085bc
	adds	r0, r5, #2
	bl 0x02008604
.L_02000276:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02008614
	bl 0x020085cc
.L_02000282:
	pop	{r5, r6, pc}
	.2byte 0x1946
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x020085dc
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
	bne.n	.L_020002c0
	adds	r0, r5, #0
	bl 0x02008644
	b.n	.L_020002dc
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_020002c0:
	bl 0x020085c4
	movs	r0, #0
	bl 0x0200862c
	ldr	r0, [pc, #20]
	bl 0x02008604
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008614
	bl 0x020085cc
.L_020002dc:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x2297
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x020085dc
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
	bne.n	.L_0200031c
	adds	r0, r5, #0
	bl 0x02008644
	b.n	.L_02000338
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_0200031c:
	bl 0x020085c4
	movs	r0, #0
	bl 0x0200862c
	ldr	r0, [pc, #20]
	bl 0x02008604
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008614
	bl 0x020085cc
.L_02000338:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x2221
	.2byte 0x0000
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	movs	r2, #133
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	adds	r3, r3, r1
	ldr	r5, [pc, #240]
	adds	r2, #255
	str	r2, [r3, #0]
	adds	r2, #11
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x020085dc
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	movs	r1, #240
	orrs	r3, r2
	lsls	r1, r1, #1
	strb	r3, [r0, #0]
	adds	r3, r5, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #208]
	cmp	r2, r3
	bne.n	.L_020003de
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #5
	bne.n	.L_02000390
	bl 0x02008454
	b.n	.L_0200043e
.L_02000390:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #33
	bl 0x020085a4
	cmp	r0, #0
	beq.n	.L_020003c4
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #57
	bl 0x020085a4
	cmp	r0, #0
	beq.n	.L_020003ba
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #65
	bl 0x020085a4
	cmp	r0, #0
	beq.n	.L_020003c4
.L_020003ba:
	movs	r0, #34
	bl 0x020085a4
	cmp	r0, #0
	beq.n	.L_020003d0
.L_020003c4:
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x020085e4
	b.n	.L_0200043e
.L_020003d0:
	movs	r0, #18
	bl 0x020085dc
	movs	r1, #4
	bl 0x020085fc
	b.n	.L_0200043e
.L_020003de:
	ldr	r3, [pc, #108]
	cmp	r2, r3
	bne.n	.L_0200043e
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #1
	bne.n	.L_0200040c
	movs	r0, #8
	bl 0x020085dc
	adds	r0, #89
	ldrb	r3, [r0, #0]
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #2
	movs	r0, #8
	bl 0x0200861c
	b.n	.L_0200043e
.L_0200040c:
	cmp	r3, #5
	bne.n	.L_02000432
	movs	r0, #48
	adds	r0, #255
	bl 0x020085b4
	movs	r0, #9
	bl 0x020085dc
	adds	r0, #89
	ldrb	r3, [r0, #0]
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #2
	movs	r0, #9
	bl 0x0200861c
	b.n	.L_0200043e
.L_02000432:
	cmp	r3, #4
	bne.n	.L_0200043e
	movs	r0, #48
	adds	r0, #255
	bl 0x020085b4
.L_0200043e:
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000039
	.2byte 0x003a
	.2byte 0x0000
	movs	r0, #0
	bx	lr
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
	bl 0x020085b4
	pop	{pc}
	push	{lr}
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #57
	bl 0x020085a4
	cmp	r0, #0
	beq.n	.L_020004a0
	movs	r0, #208
	lsls	r0, r0, #2
	bl 0x020085a4
	cmp	r0, #0
	beq.n	.L_02000490
	ldr	r0, [pc, #24]
	b.n	.L_02000492
.L_02000490:
	ldr	r0, [pc, #24]
.L_02000492:
	bl 0x02008604
	movs	r0, #18
	movs	r1, #0
	bl 0x02008614
	b.n	.L_020004a4
.L_020004a0:
	bl 0x020084dc
.L_020004a4:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0000194f
	.2byte 0x1950
	.2byte 0x0000
	push	{lr}
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #57
	bl 0x020085a4
	cmp	r0, #0
	bne.n	.L_020004c6
	bl 0x020084dc
	b.n	.L_020004d4
.L_020004c6:
	ldr	r0, [pc, #16]
	bl 0x02008604
	movs	r0, #18
	movs	r1, #0
	bl 0x02008614
.L_020004d4:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x1951
	.2byte 0x0000
	push	{r5, lr}
	bl 0x020085c4
	movs	r0, #0
	bl 0x0200862c
	ldr	r0, [pc, #176]
	bl 0x02008604
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #60
	movs	r0, #18
	bl 0x02008624
	ldr	r5, [pc, #164]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #18
	bl 0x020085f4
	movs	r0, #18
	movs	r1, #0
	bl 0x02008614
	movs	r1, #128
	movs	r2, #60
	lsls	r1, r1, #1
	ldr	r0, [r5, #0]
	bl 0x02008624
	movs	r1, #4
	movs	r0, #18
	bl 0x020085ec
	movs	r0, #15
	bl 0x020085bc
	movs	r0, #18
	movs	r1, #0
	bl 0x02008614
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #60
	movs	r0, #18
	bl 0x02008624
	movs	r1, #0
	movs	r0, #18
	bl 0x0200860c
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x020085d4
	cmp	r0, #0
	bne.n	.L_02000568
	movs	r0, #18
	movs	r1, #0
	bl 0x02008614
	movs	r0, #208
	lsls	r0, r0, #2
	bl 0x020085ac
	b.n	.L_02000582
.L_02000568:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #18
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	bl 0x02008614
.L_02000582:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #57
	bl 0x020085ac
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #65
	bl 0x020085ac
	bl 0x020085cc
	pop	{r5, pc}
	.4byte 0x0000194a
	.4byte 0x02000240
	.irp EntryTarget, 0x080003c9, 0x080003d1, 0x080003d9, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c80f9, 0x080c8119, 0x080c8159, 0x080c8171, 0x080c8181, 0x080c8189, 0x080c81a1, 0x080c8201, 0x080c8211, 0x080c84e1, 0x080c8779, 0x08108009, 0x08108011, 0x08108019
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00004ccc
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00004ccc
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000014
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
	.4byte 0x00000039
	.4byte 0x10101037
	.4byte 0xffffffff
	.4byte 0x10202037
	.4byte 0xffffffff
	.4byte 0x10303037
	.4byte 0xffffffff
	.4byte 0x10404037
	.4byte 0xffffffff
	.4byte 0x10502084
	.4byte 0xffffffff
	.4byte 0x0000003a
	.4byte 0x10105037
	.4byte 0xffffffff
	.4byte 0x1050909a
	.4byte 0xffffffff
	.4byte 0x1040808b
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00010000
	.4byte 0xffff0048
	.4byte 0x02008654
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00010000
	.4byte 0xffff004d
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0003c000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00010000
	.4byte 0xffff004c
	.4byte 0x0200870c
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x0001c000
	.4byte 0xffff0082
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00014000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00010000
	.4byte 0xffff00d1
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00014000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00010000
	.4byte 0xffff004c
	.4byte 0x00000002
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0001c000
	.4byte 0xffff00ef
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0001a000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00930000
	.4byte 0x00014000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x01d20000
	.4byte 0x00014000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x0001c000
	.4byte 0xffff0048
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x0001c000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00014000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00014000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00014000
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
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001930
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001931
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020080f5
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0000193c
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000193d
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0200814d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001942
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0200819d
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002194
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002195
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008471
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001935
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001936
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001937
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000193e
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000193f
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001941
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001944
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001945
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002197
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002198
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x020084b1
	.4byte 0x00000173
	.4byte 0xffff00ce
	.4byte 0x00403049
	.4byte 0x00000173
	.4byte 0xffff00cf
	.4byte 0x0040304a
	.4byte 0x00000173
	.4byte 0xffff00d0
	.4byte 0x0040304b
	.4byte 0x00000173
	.4byte 0xffff00d1
	.4byte 0x00403057
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
	.4byte 0x020081fd
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008289
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002298
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002299
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020082e5
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x020081fd
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001949
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000229a
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000229b
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000229c
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002222
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x02008079
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x020080a1
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
