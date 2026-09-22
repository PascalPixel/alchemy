.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.set sub_02000640, 0x02000640
	.set sub_02000660, 0x02000660
	.set sub_02000668, 0x02000668
	.set sub_02000688, 0x02000688
	.set sub_0200068c, 0x0200068c
	.set sub_020006a4, 0x020006a4
	.set sub_020006ae, 0x020006ae
	.set sub_020006b4, 0x020006b4
	.set sub_020006bc, 0x020006bc
	.set sub_020006cc, 0x020006cc
	.set sub_020006d6, 0x020006d6
	.set sub_020006de, 0x020006de
	.set sub_020006ec, 0x020006ec
	.set sub_0200070a, 0x0200070a
	.set sub_0200070e, 0x0200070e
	.set sub_0200071a, 0x0200071a
	.set sub_0200072a, 0x0200072a
	.set sub_0200072c, 0x0200072c
	.set sub_02000738, 0x02000738
	.set sub_0200073a, 0x0200073a
	.set sub_02000746, 0x02000746
	.set sub_02000752, 0x02000752
	.set sub_02000788, 0x02000788
	.set sub_02000790, 0x02000790
	.set sub_0200079e, 0x0200079e
	.set sub_020007a8, 0x020007a8
	.set sub_020007b6, 0x020007b6
	.set sub_020007be, 0x020007be
	.set sub_020007e0, 0x020007e0
	.set sub_020007e8, 0x020007e8
	.set sub_020007ea, 0x020007ea
	.set sub_020007fa, 0x020007fa
	.set sub_02000802, 0x02000802
	.set sub_0200080c, 0x0200080c
	.set sub_02000816, 0x02000816
	.set sub_0200081c, 0x0200081c
	.set sub_0200082a, 0x0200082a
	.set sub_02000848, 0x02000848
	.set sub_0200084c, 0x0200084c
	.set sub_02000858, 0x02000858
	.set sub_02000868, 0x02000868
	.set sub_0200086a, 0x0200086a
	.set sub_0200086c, 0x0200086c
	.set sub_02000874, 0x02000874
	.set sub_02000878, 0x02000878
	.set sub_02000884, 0x02000884
	.set sub_02000886, 0x02000886
	.set sub_02000890, 0x02000890
	.set sub_020008a6, 0x020008a6
	.set sub_020008d0, 0x020008d0
	.set sub_020008d2, 0x020008d2
	.set sub_020008e2, 0x020008e2
	.set sub_020008ea, 0x020008ea
	.set sub_020008f4, 0x020008f4
	.set sub_020008f8, 0x020008f8
	.set sub_02000902, 0x02000902
	.set sub_0200092e, 0x0200092e
	.set sub_0200093c, 0x0200093c
	.set sub_02000946, 0x02000946
	.set sub_0200094a, 0x0200094a
	.set sub_02000950, 0x02000950
	.set sub_02000954, 0x02000954
	.set sub_02000958, 0x02000958
	.set sub_02000962, 0x02000962
	.set sub_0200097e, 0x0200097e
	.set sub_0200099e, 0x0200099e
	.set sub_020009b0, 0x020009b0
	.set sub_020009ca, 0x020009ca
	.set sub_020009d2, 0x020009d2
	.set sub_020009d6, 0x020009d6
	.set sub_020009f0, 0x020009f0
	.set sub_020009f8, 0x020009f8
	.set sub_02000a1e, 0x02000a1e
	.set sub_02000a20, 0x02000a20
	.set sub_02000a24, 0x02000a24
	.set sub_02000a2a, 0x02000a2a
	.set sub_02000a4a, 0x02000a4a
	.set sub_02000a5e, 0x02000a5e
	.set sub_02000a98, 0x02000a98
	.set sub_02000aa4, 0x02000aa4
	.set sub_02000ab0, 0x02000ab0
	.set sub_02000ace, 0x02000ace
	.set sub_02000ae6, 0x02000ae6
	.set sub_02000ae8, 0x02000ae8
	.set sub_02000af0, 0x02000af0
	.set sub_02000afe, 0x02000afe
	.set sub_02000b10, 0x02000b10
	.set sub_02000b12, 0x02000b12
	.set sub_02000b1c, 0x02000b1c
	.set sub_02000b24, 0x02000b24
	.set sub_02000b26, 0x02000b26
	.set sub_02000b36, 0x02000b36
	.set sub_02000b40, 0x02000b40
	.set sub_02000b42, 0x02000b42
	.set sub_02000b48, 0x02000b48
	.set sub_02000b54, 0x02000b54
	.set sub_02000b64, 0x02000b64
	.set sub_02000b70, 0x02000b70
	.set sub_02000b94, 0x02000b94
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
	bl	sub_02000640
	movs	r0, #0
	bl	sub_020006ae
	ldr	r0, [pc, #20]
	bl	sub_0200068c
	movs	r0, #8
	movs	r1, #0
	bl	sub_020006a4
	bl	sub_02000660
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x1949
	.2byte 0x0000
	push	{lr}
	bl	sub_02000668
	movs	r0, #0
	bl	sub_020006d6
	ldr	r0, [pc, #20]
	bl	sub_020006b4
	movs	r0, #9
	movs	r1, #0
	bl	sub_020006cc
	bl	sub_02000688
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
	bl	sub_020006bc
	movs	r0, #0
	bl	sub_0200072a
	ldr	r5, [pc, #68]
	adds	r0, r5, #0
	bl	sub_0200070a
	movs	r1, #0
	movs	r0, #10
	bl	sub_0200071a
	bl	sub_02000746
	movs	r1, #0
	bl	sub_020006ec
	cmp	r0, #0
	bne.n	.L_0200012c
	movs	r0, #10
	bl	sub_020006de
	adds	r0, r5, #1
	bl	sub_0200072c
	b.n	.L_02000138
.L_0200012c:
	movs	r0, #20
	bl	sub_020006ec
	adds	r0, r5, #2
	bl	sub_0200073a
.L_02000138:
	movs	r0, #10
	movs	r1, #0
	bl	sub_02000752
	bl	sub_0200070e
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
	bl	sub_02000738
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
	bl	sub_020007b6
	b.n	.L_02000196
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_02000188:
	ldr	r0, [pc, #12]
	bl	sub_02000790
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_020007a8
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
	bl	sub_02000788
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
	bl	sub_02000816
	b.n	.L_020001f4
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_020001d8:
	bl	sub_0200079e
	movs	r0, #0
	bl	sub_0200080c
	ldr	r0, [pc, #20]
	bl	sub_020007ea
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_02000802
	bl	sub_020007be
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
	bl	sub_020007e8
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
	bl	sub_0200086c
	b.n	.L_02000282
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_02000234:
	bl	sub_020007fa
	movs	r0, #0
	bl	sub_02000868
	ldr	r5, [pc, #68]
	adds	r0, r5, #0
	bl	sub_02000848
	movs	r1, #0
	adds	r0, r6, #0
	bl	sub_02000858
	bl	sub_02000884
	movs	r1, #0
	bl	sub_0200082a
	cmp	r0, #0
	bne.n	.L_0200026a
	movs	r0, #10
	bl	sub_0200081c
	adds	r0, r5, #1
	bl	sub_0200086a
	b.n	.L_02000276
.L_0200026a:
	movs	r0, #20
	bl	sub_0200082a
	adds	r0, r5, #2
	bl	sub_02000878
.L_02000276:
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_02000890
	bl	sub_0200084c
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
	bl	sub_02000874
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
	bl	sub_020008f8
	b.n	.L_020002dc
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_020002c0:
	bl	sub_02000886
	movs	r0, #0
	bl	sub_020008f4
	ldr	r0, [pc, #20]
	bl	sub_020008d2
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_020008ea
	bl	sub_020008a6
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
	bl	sub_020008d0
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
	bl	sub_02000954
	b.n	.L_02000338
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_0200031c:
	bl	sub_020008e2
	movs	r0, #0
	bl	sub_02000950
	ldr	r0, [pc, #20]
	bl	sub_0200092e
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_02000946
	bl	sub_02000902
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
	bl	sub_0200093c
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
	bl	sub_020007e0
	b.n	.L_0200043e
.L_02000390:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #33
	bl	sub_0200093c
	cmp	r0, #0
	beq.n	.L_020003c4
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #57
	bl	sub_0200094a
	cmp	r0, #0
	beq.n	.L_020003ba
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #65
	bl	sub_02000958
	cmp	r0, #0
	beq.n	.L_020003c4
.L_020003ba:
	movs	r0, #34
	bl	sub_02000962
	cmp	r0, #0
	beq.n	.L_020003d0
.L_020003c4:
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl	sub_020009b0
	b.n	.L_0200043e
.L_020003d0:
	movs	r0, #18
	bl	sub_020009b0
	movs	r1, #4
	bl	sub_020009d6
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
	bl	sub_020009d2
	adds	r0, #89
	ldrb	r3, [r0, #0]
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #2
	movs	r0, #8
	bl	sub_02000a24
	b.n	.L_0200043e
.L_0200040c:
	cmp	r3, #5
	bne.n	.L_02000432
	movs	r0, #48
	adds	r0, #255
	bl	sub_020009ca
	movs	r0, #9
	bl	sub_020009f8
	adds	r0, #89
	ldrb	r3, [r0, #0]
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #2
	movs	r0, #9
	bl	sub_02000a4a
	b.n	.L_0200043e
.L_02000432:
	cmp	r3, #4
	bne.n	.L_0200043e
	movs	r0, #48
	adds	r0, #255
	bl	sub_020009f0
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
	bl	sub_02000a20
	pop	{pc}
	push	{lr}
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #57
	bl	sub_02000a1e
	cmp	r0, #0
	beq.n	.L_020004a0
	movs	r0, #208
	lsls	r0, r0, #2
	bl	sub_02000a2a
	cmp	r0, #0
	beq.n	.L_02000490
	ldr	r0, [pc, #24]
	b.n	.L_02000492
.L_02000490:
	ldr	r0, [pc, #24]
.L_02000492:
	bl	sub_02000a98
	movs	r0, #18
	movs	r1, #0
	bl	sub_02000ab0
	b.n	.L_020004a4
.L_020004a0:
	bl	sub_0200097e
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
	bl	sub_02000a5e
	cmp	r0, #0
	bne.n	.L_020004c6
	bl	sub_0200099e
	b.n	.L_020004d4
.L_020004c6:
	ldr	r0, [pc, #16]
	bl	sub_02000ace
	movs	r0, #18
	movs	r1, #0
	bl	sub_02000ae6
.L_020004d4:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x1951
	.2byte 0x0000
	push	{r5, lr}
	bl	sub_02000aa4
	movs	r0, #0
	bl	sub_02000b12
	ldr	r0, [pc, #176]
	bl	sub_02000af0
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #60
	movs	r0, #18
	bl	sub_02000b1c
	ldr	r5, [pc, #164]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #18
	bl	sub_02000afe
	movs	r0, #18
	movs	r1, #0
	bl	sub_02000b26
	movs	r1, #128
	movs	r2, #60
	lsls	r1, r1, #1
	ldr	r0, [r5, #0]
	bl	sub_02000b42
	movs	r1, #4
	movs	r0, #18
	bl	sub_02000b12
	movs	r0, #15
	bl	sub_02000ae8
	movs	r0, #18
	movs	r1, #0
	bl	sub_02000b48
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #60
	movs	r0, #18
	bl	sub_02000b64
	movs	r1, #0
	movs	r0, #18
	bl	sub_02000b54
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl	sub_02000b24
	cmp	r0, #0
	bne.n	.L_02000568
	movs	r0, #18
	movs	r1, #0
	bl	sub_02000b70
	movs	r0, #208
	lsls	r0, r0, #2
	bl	sub_02000b10
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
	bl	sub_02000b94
.L_02000582:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #57
	bl	sub_02000b36
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #65
	bl	sub_02000b40
	bl	sub_02000b64
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
