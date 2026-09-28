.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008155, 0x02008039, 0x02008045, 0x0200804d, 0x0200806d, 0x02008041, 0x02008185
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8230
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8260
	.2byte 0x0200
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x0200819c
	cmp	r0, #0
	beq.n	.L_02000060
	ldr	r0, [pc, #4]
	b.n	.L_02000062
.L_02000060:
	ldr	r0, [pc, #4]
.L_02000062:
	pop	{pc}
	.4byte 0x02008318
	.2byte 0x8270
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8360
	.2byte 0x0200
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x0200819c
	cmp	r0, #0
	bne.n	.L_02000142
	bl 0x020081b4
	movs	r0, #0
	bl 0x02008214
	ldr	r0, [pc, #192]
	bl 0x020081e4
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x02008204
	movs	r0, #8
	movs	r1, #1
	bl 0x020081fc
	bl 0x0200820c
	movs	r2, #2
	movs	r0, #10
	movs	r1, #4
	bl 0x020081dc
	movs	r1, #129
	movs	r0, #10
	lsls	r1, r1, #1
	bl 0x020081f4
	movs	r2, #15
	movs	r0, #10
	movs	r1, #4
	bl 0x020081d4
	movs	r0, #10
	movs	r1, #0
	bl 0x020081ec
	movs	r0, #11
	movs	r1, #4
	movs	r2, #0
	bl 0x020081dc
	movs	r0, #12
	movs	r1, #4
	movs	r2, #0
	bl 0x020081dc
	movs	r0, #13
	movs	r1, #4
	movs	r2, #0
	bl 0x020081dc
	movs	r2, #15
	movs	r0, #11
	movs	r1, #2
	bl 0x020081d4
	movs	r0, #11
	movs	r1, #0
	bl 0x020081ec
	movs	r2, #0
	movs	r0, #12
	movs	r1, #11
	bl 0x020081dc
	movs	r0, #12
	movs	r1, #4
	bl 0x020081cc
	movs	r0, #12
	movs	r1, #0
	bl 0x020081ec
	movs	r2, #0
	movs	r0, #12
	movs	r1, #4
	bl 0x020081dc
	movs	r0, #12
	movs	r1, #0
	bl 0x020081ec
	movs	r0, #13
	movs	r1, #3
	bl 0x020081cc
	movs	r0, #13
	movs	r1, #0
	bl 0x020081ec
	bl 0x020081bc
.L_02000142:
	movs	r0, #137
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x020081a4
	pop	{pc}
	.2byte 0x0000
	.2byte 0x1850
	.2byte 0x0000
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #85
	str	r2, [r3, #0]
	movs	r0, #8
	bl 0x020081c4
	movs	r1, #0
	bl 0x020081ac
	movs	r0, #9
	bl 0x020081c4
	movs	r1, #0
	bl 0x020081ac
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	bl 0x0200821c
	bl 0x020081c4
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #0
	pop	{pc}
	.irp EntryTarget, 0x080003c9, 0x080003d1, 0x08020219, 0x080c8019, 0x080c8021, 0x080c8089, 0x080c8129, 0x080c8139, 0x080c8159, 0x080c8181, 0x080c81a1, 0x080c8219, 0x080c8229, 0x080c8231, 0x080c8241, 0x080c84e1, 0x080c8779
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0xffff0000
	.4byte 0x000000c2
	.4byte 0x400000d7
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000068
	.4byte 0x10103066
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff019a
	.4byte 0x02008224
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00004000
	.4byte 0xffff0132
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00004000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02008000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0200c000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02004000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff019a
	.4byte 0x02008224
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00004000
	.4byte 0xffff0132
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x098f000a
	.4byte 0x02008075
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001855
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001856
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001857
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001858
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001859
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000185a
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000185b
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000185c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
