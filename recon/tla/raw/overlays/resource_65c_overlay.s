.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.set sub_020001e0, 0x020001e0
	.set sub_020001fe, 0x020001fe
	.set sub_02000214, 0x02000214
	.set sub_02000232, 0x02000232
	.set sub_02000250, 0x02000250
	.set sub_0200025a, 0x0200025a
	.set sub_02000262, 0x02000262
	.set sub_02000292, 0x02000292
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008075, 0x02008039, 0x02008045, 0x0200804d, 0x0200806d, 0x02008041, 0x0200816d
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x81b4
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x81e4
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x81fc
	.2byte 0x0200
	push	{lr}
	movs	r1, #13
	movs	r2, #14
	movs	r0, #1
	bl	sub_020001fe
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl	sub_020001e0
	pop	{pc}
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x82bc
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
	ldr	r2, [pc, #100]
	strb	r3, [r1, #23]
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r3, [r0, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #144
	subs	r2, #172
	lsls	r1, r1, #3
	str	r2, [r3, #0]
	ldr	r0, [pc, #80]
	bl	sub_02000214
	ldr	r3, [pc, #80]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	sub_02000232
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	bl	sub_02000250
	movs	r2, #11
	movs	r1, #12
	movs	r0, #0
	bl	sub_02000262
	movs	r0, #12
	movs	r1, #4
	bl	sub_0200025a
	movs	r0, #11
	movs	r1, #4
	bl	sub_02000262
	movs	r1, #130
	lsls	r1, r1, #1
	movs	r0, #1
	adds	r1, #255
	movs	r2, #13
	movs	r3, #14
	bl	sub_02000292
	movs	r0, #0
	pop	{pc}
	.4byte 0x020081b0
	.4byte 0x020080fd
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #96]
	ldr	r2, [r3, #0]
	movs	r3, #15
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_0200015c
	ldr	r1, [pc, #88]
	ldr	r3, [r1, #0]
	cmp	r3, #8
	bne.n	.L_02000114
	str	r2, [r1, #0]
.L_02000114:
	ldr	r3, [r1, #0]
	ldr	r0, [pc, #80]
	adds	r2, r3, #0
	adds	r3, #1
	str	r3, [r1, #0]
	lsls	r3, r2, #1
	adds	r3, r3, r0
	ldrh	r3, [r3, #0]
	movs	r1, #160
	lsls	r1, r1, #19
	adds	r1, #194
	strh	r3, [r1, #0]
	movs	r1, #7
	adds	r3, r2, #1
	ands	r3, r1
	lsls	r3, r3, #1
	adds	r3, r3, r0
	ldrh	r3, [r3, #0]
	movs	r4, #160
	lsls	r4, r4, #19
	adds	r4, #196
	strh	r3, [r4, #0]
	adds	r3, r2, #2
	ands	r3, r1
	lsls	r3, r3, #1
	adds	r3, r3, r0
	ldrh	r3, [r3, #0]
	adds	r4, #2
	adds	r2, #3
	strh	r3, [r4, #0]
	ands	r2, r1
	lsls	r2, r2, #1
	adds	r2, r2, r0
	ldrh	r3, [r2, #0]
	adds	r4, #2
	strh	r3, [r4, #0]
.L_0200015c:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0300122c
	.4byte 0x020081b0
	.2byte 0x0180
	.2byte 0x0500
	movs	r0, #0
	bx	lr
	.irp EntryTarget, 0x080000d1, 0x080003d1, 0x080c8089, 0x080c8119, 0x080c86a9, 0x080c86b1, 0x080c86c1, 0x080c86e9
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x00000000
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
	.4byte 0x00000036
	.4byte 0x1010c002
	.4byte 0xffffffff
	.4byte 0x1020d002
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0x08ff00c0
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00e20000
	.4byte 0x00010000
	.4byte 0x08ff00bb
	.4byte 0x00000001
	.4byte 0x01030000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0001c000
	.4byte 0x08ff00be
	.4byte 0x00000001
	.4byte 0x00f20000
	.4byte 0x00000000
	.4byte 0x01390000
	.4byte 0x0001c000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0002c000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0002c000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x0002c000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x0002c000
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
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001916
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001917
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001918
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001919
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000191a
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000191b
	.4byte 0x00008515
	.4byte 0x0203000d
	.4byte 0x02008055
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
