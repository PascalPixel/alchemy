.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.set sub_020000ea, 0x020000ea
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008071, 0x02008039, 0x02008045, 0x0200804d, 0x02008069, 0x02008041, 0x02008085
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
	.2byte 0x8124
	.2byte 0x0200
	push	{lr}
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r3, #225
	adds	r1, r1, r3
	adds	r0, r1, #0
	bl	sub_020000ea
	pop	{pc}
	.2byte 0x0000
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x819c
	.2byte 0x0200
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #85
	str	r2, [r3, #0]
	movs	r0, #0
	bx	lr
	movs	r0, #0
	bx	lr
	.irp EntryTarget, 0x080003d1
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000016
	.4byte 0x00040000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00030000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00018000
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
	.4byte 0x0000009d
	.4byte 0x1010209d
	.4byte 0xffffffff
	.4byte 0x1020109d
	.4byte 0xffffffff
	.4byte 0x1030309b
	.4byte 0xffffffff
	.4byte 0x1040409b
	.4byte 0xffffffff
	.4byte 0x1050509b
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0x08e90133
	.4byte 0x02008090
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x0002c000
	.4byte 0x08ea0133
	.4byte 0x02008090
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02500000
	.4byte 0x0002c000
	.4byte 0x08eb0133
	.4byte 0x02008090
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02500000
	.4byte 0x0002c000
	.4byte 0x08ec0133
	.4byte 0x02008090
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x02800000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x18eb0001
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
	.4byte 0x00004e15
	.4byte 0x08e90008
	.4byte 0x02008055
	.4byte 0x00004e15
	.4byte 0x08ea0009
	.4byte 0x02008055
	.4byte 0x00004e15
	.4byte 0x08eb000a
	.4byte 0x02008055
	.4byte 0x00004e15
	.4byte 0x08ec000b
	.4byte 0x02008055
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
