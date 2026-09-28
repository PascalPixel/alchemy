.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
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
	bl 0x020080bc
	movs	r0, #48
	bl 0x020080b4
	pop	{pc}
	push	{lr}
	movs	r0, #9
	movs	r1, #1
	movs	r2, #0
	bl 0x020080bc
	movs	r0, #68
	bl 0x020080b4
	pop	{pc}
	push	{lr}
	movs	r0, #10
	movs	r1, #2
	movs	r2, #0
	bl 0x020080bc
	movs	r0, #88
	bl 0x020080b4
	pop	{pc}
	push	{lr}
	movs	r0, #11
	movs	r1, #3
	movs	r2, #0
	bl 0x020080bc
	movs	r0, #108
	bl 0x020080b4
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
	.section .rodata,"a",%progbits
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
