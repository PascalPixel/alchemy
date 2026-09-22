.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.set sub_02000118, 0x02000118
	.set sub_0200011a, 0x0200011a
	.set sub_0200012c, 0x0200012c
	.set sub_0200012e, 0x0200012e
	.set sub_02000140, 0x02000140
	.set sub_02000142, 0x02000142
	.set sub_02000154, 0x02000154
	.set sub_02000156, 0x02000156
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x020080ad, 0x02008039, 0x02008045, 0x0200804d, 0x020080a5, 0x02008041, 0x020080b1
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x80c4
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x80f4
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8108
	.2byte 0x0200
	push	{lr}
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl	sub_0200011a
	movs	r0, #48
	bl	sub_02000118
	pop	{pc}
	push	{lr}
	movs	r0, #9
	movs	r1, #1
	movs	r2, #0
	bl	sub_0200012e
	movs	r0, #68
	bl	sub_0200012c
	pop	{pc}
	push	{lr}
	movs	r0, #10
	movs	r1, #2
	movs	r2, #0
	bl	sub_02000142
	movs	r0, #88
	bl	sub_02000140
	pop	{pc}
	push	{lr}
	movs	r0, #11
	movs	r1, #3
	movs	r2, #0
	bl	sub_02000156
	movs	r0, #108
	bl	sub_02000154
	pop	{pc}
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8120
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	movs	r0, #0
	bx	lr
	.irp EntryTarget, 0x080003d9, 0x080c8581
	overlay_veneer \EntryTarget
	.endr
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
	.4byte 0x00000139
	.4byte 0x0000013a
	.4byte 0x0000013b
	.4byte 0x0000013c
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008055
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008069
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0200807d
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02008091
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02000200
	.4byte 0x03600400
	.4byte 0xffffffff
	.4byte 0xffffffff
