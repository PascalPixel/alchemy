.syntax unified
	.thumb
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x80c4
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
	.2byte 0x80f4
	.2byte 0x0200
	.global Func_0200004c
	.thumb_func
Func_0200004c:
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
	bl 0x02008088
	pop	{pc}
	.2byte 0x0000
	.global Func_02000068
	.thumb_func
Func_02000068:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x819c
	.2byte 0x0200
	.global Func_02000070
	.thumb_func
Func_02000070:
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
	.global Func_02000084
	.thumb_func
Func_02000084:
	movs	r0, #0
	bx	lr
	.section .rodata,"a",%progbits
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
