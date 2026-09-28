.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008481, 0x02008039, 0x02008045, 0x0200804d, 0x020080b9, 0x02008041, 0x02008521
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x865c
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x868c
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #40]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #10
	bne.n	.L_02000062
	ldr	r0, [pc, #28]
	b.n	.L_02000076
.L_02000062:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02008524
	cmp	r0, #0
	beq.n	.L_02000074
	ldr	r0, [pc, #12]
	b.n	.L_02000076
.L_02000074:
	ldr	r0, [pc, #12]
.L_02000076:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x020088c4
	.4byte 0x02008954
	.2byte 0x86cc
	.2byte 0x0200
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #208
	lsls	r1, r1, #4
	adds	r1, #56
	adds	r2, r3, r1
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r3, [pc, #20]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #10
	beq.n	.L_020000b0
	movs	r3, #0
	strb	r3, [r2, #0]
.L_020000b0:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02008524
	cmp	r0, #0
	beq.n	.L_020000cc
	ldr	r0, [pc, #4]
	b.n	.L_020000ce
.L_020000cc:
	ldr	r0, [pc, #4]
.L_020000ce:
	pop	{pc}
	.4byte 0x02008cfc
	.2byte 0x8b4c
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #93
	bl 0x02008524
	cmp	r0, #0
	bne.n	.L_02000128
	ldr	r5, [pc, #76]
	adds	r0, r5, #0
	bl 0x02008584
	movs	r1, #0
	movs	r0, #14
	bl 0x0200858c
	bl 0x020085dc
	movs	r1, #0
	bl 0x0200855c
	cmp	r0, #0
	bne.n	.L_02000118
	adds	r0, r5, #1
	bl 0x02008584
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #93
	bl 0x0200852c
	b.n	.L_0200011e
.L_02000118:
	adds	r0, r5, #2
	bl 0x02008584
.L_0200011e:
	movs	r0, #14
	movs	r1, #0
	bl 0x02008594
	b.n	.L_02000136
.L_02000128:
	ldr	r0, [pc, #16]
	bl 0x02008584
	movs	r0, #14
	movs	r1, #0
	bl 0x02008594
.L_02000136:
	pop	{r5, pc}
	.4byte 0x00001d8b
	.2byte 0x1d8e
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #94
	bl 0x02008524
	cmp	r0, #0
	bne.n	.L_02000190
	ldr	r5, [pc, #76]
	adds	r0, r5, #0
	bl 0x02008584
	movs	r1, #0
	movs	r0, #18
	bl 0x0200858c
	bl 0x020085dc
	movs	r1, #0
	bl 0x0200855c
	cmp	r0, #0
	bne.n	.L_02000180
	adds	r0, r5, #1
	bl 0x02008584
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #93
	bl 0x0200852c
	b.n	.L_02000186
.L_02000180:
	adds	r0, r5, #2
	bl 0x02008584
.L_02000186:
	movs	r0, #18
	movs	r1, #0
	bl 0x02008594
	b.n	.L_0200019e
.L_02000190:
	ldr	r0, [pc, #16]
	bl 0x02008584
	movs	r0, #14
	movs	r1, #0
	bl 0x02008594
.L_0200019e:
	pop	{r5, pc}
	.4byte 0x00001d92
	.2byte 0x1d95
	.2byte 0x0000
	push	{r5, lr}
	ldr	r5, [pc, #64]
	adds	r0, r5, #0
	bl 0x02008584
	movs	r1, #0
	movs	r0, #19
	bl 0x0200858c
	bl 0x020085dc
	movs	r1, #0
	bl 0x0200855c
	cmp	r0, #0
	bne.n	.L_020001d6
	movs	r0, #10
	bl 0x02008544
	adds	r0, r5, #1
	bl 0x02008584
	b.n	.L_020001e2
.L_020001d6:
	movs	r0, #20
	bl 0x02008544
	adds	r0, r5, #2
	bl 0x02008584
.L_020001e2:
	movs	r0, #19
	movs	r1, #0
	bl 0x02008594
	pop	{r5, pc}
	.2byte 0x1d96
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x02008564
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #24]
	ands	r3, r2
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_0200022c
	movs	r0, #16
	adds	r1, r5, #0
	bl 0x020085e4
	b.n	.L_02000248
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_0200022c:
	bl 0x0200854c
	movs	r0, #0
	bl 0x020085c4
	ldr	r0, [pc, #20]
	bl 0x02008584
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008594
	bl 0x02008554
.L_02000248:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1e04
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x02008564
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #24]
	ands	r3, r2
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_0200028c
	movs	r0, #16
	adds	r1, r5, #0
	bl 0x020085e4
	b.n	.L_020002a8
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_0200028c:
	bl 0x0200854c
	movs	r0, #0
	bl 0x020085c4
	ldr	r0, [pc, #20]
	bl 0x02008584
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008594
	bl 0x02008554
.L_020002a8:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1e2c
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x02008564
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
	bne.n	.L_020002e8
	adds	r0, r5, #0
	bl 0x020085ec
	b.n	.L_02000304
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_020002e8:
	bl 0x0200854c
	movs	r0, #0
	bl 0x020085c4
	ldr	r0, [pc, #20]
	bl 0x02008584
	movs	r0, #26
	movs	r1, #0
	bl 0x02008594
	bl 0x02008554
.L_02000304:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1e0b
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x02008564
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
	bne.n	.L_02000344
	adds	r0, r5, #0
	bl 0x020085ec
	b.n	.L_02000360
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_02000344:
	bl 0x0200854c
	movs	r0, #0
	bl 0x020085c4
	ldr	r0, [pc, #20]
	bl 0x02008584
	movs	r0, #26
	movs	r1, #0
	bl 0x02008594
	bl 0x02008554
.L_02000360:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1e33
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #108]
	bl 0x0200854c
	movs	r0, #0
	bl 0x020085c4
	movs	r5, #8
.L_0200037c:
	adds	r0, r5, #0
	bl 0x02008564
	cmp	r0, #0
	beq.n	.L_0200038e
	adds	r2, r0, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
.L_0200038e:
	adds	r5, #1
	cmp	r5, #63
	bls.n	.L_0200037c
	movs	r3, #170
	lsls	r3, r3, #1
	adds	r6, r6, r3
	movs	r3, #0
	ldrsh	r5, [r6, r3]
	movs	r0, #158
	bl 0x020085f4
	subs	r5, #4
	ldr	r0, [pc, #80]
	lsls	r5, r5, #3
	adds	r3, r5, #4
	ldrh	r1, [r0, r3]
	adds	r3, r3, r0
	ldrh	r2, [r3, #2]
	ldr	r0, [r0, r5]
	bl 0x02008534
	ldr	r5, [pc, #64]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200856c
	ldr	r0, [r5, #0]
	bl 0x02008564
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #2
	ldr	r0, [r5, #0]
	bl 0x0200857c
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	bl 0x020085a4
	bl 0x020085ac
	bl 0x020085b4
	bl 0x02008554
	pop	{r5, r6, pc}
	.4byte 0x0200864c
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x0200854c
	movs	r0, #0
	bl 0x020085c4
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200852c
	bl 0x02008554
	pop	{pc}
	.4byte 0x049b23c0
	.4byte 0x23d06eda
	.4byte 0x3331011b
	.4byte 0x230018d1
	.4byte 0x23d0700b
	.4byte 0x3332011b
	.4byte 0x231818d1
	.4byte 0x23d0700b
	.4byte 0x3333011b
	.4byte 0x231918d2
	.2byte 0x7013
	.2byte 0x4770
	push	{lr}
	movs	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x02008574
	movs	r1, #208
	movs	r2, #208
	movs	r0, #65
	lsls	r1, r1, #15
	lsls	r2, r2, #15
	bl 0x020085bc
	pop	{pc}
	push	{lr}
	movs	r0, #65
	movs	r1, #0
	movs	r2, #0
	bl 0x020085bc
	movs	r1, #208
	movs	r2, #208
	movs	r0, #27
	lsls	r1, r1, #15
	lsls	r2, r2, #15
	bl 0x02008574
	pop	{pc}
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	subs	r2, #172
	str	r2, [r3, #0]
	movs	r0, #9
	bl 0x02008564
	movs	r1, #0
	bl 0x0200853c
	movs	r0, #10
	bl 0x02008564
	movs	r1, #0
	bl 0x0200853c
	movs	r0, #23
	bl 0x02008564
	movs	r1, #0
	bl 0x0200853c
	movs	r0, #23
	bl 0x02008564
	ldr	r3, [pc, #92]
	movs	r5, #128
	str	r3, [r0, #28]
	movs	r0, #27
	bl 0x02008564
	lsls	r5, r5, #8
	str	r5, [r0, #24]
	movs	r0, #27
	bl 0x02008564
	str	r5, [r0, #28]
	bl 0x02008464
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02008524
	cmp	r0, #0
	beq.n	.L_020004f0
	movs	r0, #23
	movs	r1, #0
	movs	r2, #0
	bl 0x02008574
.L_020004f0:
	bl 0x020085cc
	movs	r1, #128
	lsls	r1, r1, #2
	movs	r2, #24
	movs	r3, #25
	adds	r1, #2
	movs	r0, #0
	bl 0x020085d4
	movs	r0, #24
	bl 0x02008564
	movs	r1, #0
	bl 0x0200853c
	movs	r0, #25
	bl 0x0200859c
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0001
	movs	r0, #0
	bx	lr
	.irp EntryTarget, 0x080003c9, 0x080003d1, 0x08020171, 0x08020219, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80f9, 0x080c8119, 0x080c8181, 0x080c8189, 0x080c81a1, 0x080c8209, 0x080c8279, 0x080c83b1, 0x080c83b9, 0x080c8409, 0x080c84e1, 0x080c86a9, 0x080c86e9, 0x080c8779, 0x08108009, 0x08108011, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.section .rodata,"a",%progbits
	.4byte 0x0200001a
	.4byte 0x0000ffff
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00180000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000003
	.4byte 0x00000011
	.4byte 0x002a0008
	.4byte 0x00020001
	.4byte 0xffff0006
	.4byte 0x02008640
	.4byte 0x00330012
	.4byte 0x02008640
	.4byte 0x00220015
	.4byte 0xffff0000
	.4byte 0x000000e0
	.4byte 0x400000be
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000069
	.4byte 0x1010106a
	.4byte 0xffffffff
	.4byte 0x1020206a
	.4byte 0xffffffff
	.4byte 0x1030306a
	.4byte 0xffffffff
	.4byte 0x1040406a
	.4byte 0xffffffff
	.4byte 0x1050506a
	.4byte 0xffffffff
	.4byte 0x10617002
	.4byte 0xffffffff
	.4byte 0x10718002
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00f60000
	.4byte 0x00004000
	.4byte 0xffff0130
	.4byte 0x02008604
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff0130
	.4byte 0x02008604
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff019a
	.4byte 0x02008610
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff019a
	.4byte 0x02008610
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00014000
	.4byte 0xffff0074
	.4byte 0x00000003
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00014000
	.4byte 0xffff0080
	.4byte 0x00000003
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0000e000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00012000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00012000
	.4byte 0xffff0088
	.4byte 0x00000002
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0000e000
	.4byte 0xffff0093
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00016000
	.4byte 0xffff0070
	.4byte 0x00000002
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00010000
	.4byte 0xffff0133
	.4byte 0x02008634
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00014000
	.4byte 0xffff0148
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff016c
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00f60000
	.4byte 0x00004000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff019a
	.4byte 0x00000001
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff019a
	.4byte 0x00000001
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00f60000
	.4byte 0x00004000
	.4byte 0xffff0130
	.4byte 0x02008604
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff0130
	.4byte 0x02008604
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff019a
	.4byte 0x02008610
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff019a
	.4byte 0x02008610
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00014000
	.4byte 0xffff0074
	.4byte 0x00000003
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00014000
	.4byte 0xffff0080
	.4byte 0x00000003
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0000e000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00012000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00012000
	.4byte 0xffff0088
	.4byte 0x00000002
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0000e000
	.4byte 0xffff0093
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00016000
	.4byte 0xffff0070
	.4byte 0x00000002
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00010000
	.4byte 0xffff0133
	.4byte 0x02008634
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00014000
	.4byte 0xffff0148
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00680000
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
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008369
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008369
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001d8a
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x020080d9
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001d8f
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001d90
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001d91
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008141
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x020081a9
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001d99
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001da2
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x020081f1
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x020082b1
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001d9a
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001d9b
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001d9c
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001d9d
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001d9e
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001d9f
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001da0
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001da1
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001da3
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001e05
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00001e0c
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.4byte 0x00403041
	.4byte 0x10009a15
	.4byte 0xffff0008
	.4byte 0x02008089
	.4byte 0x00004e15
	.4byte 0x02010017
	.4byte 0x02008401
	.4byte 0x00008515
	.4byte 0x02020018
	.4byte 0x00000000
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008449
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008465
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
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008369
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008369
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001e0d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001e0e
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001e0f
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001e10
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001e11
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001e12
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001e13
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001e14
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001e1d
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x02008251
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x0200830d
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001e15
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001e16
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001e17
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001e18
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001e19
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001e1a
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001e1b
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001e1c
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001e1e
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001e2d
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00001e34
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.4byte 0x00403041
	.4byte 0x10009a15
	.4byte 0xffff0008
	.4byte 0x02008089
	.4byte 0x00004e15
	.4byte 0x02010017
	.4byte 0x02008401
	.4byte 0x00008515
	.4byte 0x02020018
	.4byte 0x00000000
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008449
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008465
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
