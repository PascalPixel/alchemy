.syntax unified
	.thumb
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x80c0
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
	.2byte 0x80f0
	.2byte 0x0200
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8100
	.2byte 0x0200
	push	{lr}
	bl 0x020080b8
	pop	{pc}
	.global Func_0200005c
	.thumb_func
Func_0200005c:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8130
	.2byte 0x0200
	.global Func_02000064
	.thumb_func
Func_02000064:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #85
	str	r2, [r3, #0]
	movs	r1, #2
	movs	r0, #8
	bl 0x020080b0
	movs	r0, #8
	bl 0x020080a8
	adds	r0, #89
	ldrb	r2, [r0, #0]
	movs	r3, #4
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #8
	bl 0x020080a8
	adds	r0, #89
	ldrb	r2, [r0, #0]
	movs	r3, #16
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.global Func_020000a4
	.thumb_func
Func_020000a4:
	movs	r0, #0
	bx	lr
	.section .rodata,"a",%progbits
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
	.4byte 0x000000b8
	.4byte 0x101070b6
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x02d00000
	.4byte 0x00000000
	.4byte 0x00930000
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
	.4byte 0x0000c400
	.4byte 0xffff0008
	.4byte 0x02008055
	.4byte 0x00000000
	.4byte 0x197f0008
	.4byte 0x000026b0
	.4byte 0x00008d15
	.4byte 0x197f0008
	.4byte 0x000026b1
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002439
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000243a
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
