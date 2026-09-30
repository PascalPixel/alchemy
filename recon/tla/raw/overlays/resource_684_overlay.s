.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_020000c0
	.section .text.x02008040,"ax",%progbits
	.global Func_02000040
	.thumb_func
Func_02000040:
	movs r0, #0
	bx lr
	.section .text.x02008044,"ax",%progbits
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr r0, .L_02008048
	bx lr
.L_02008048:
	.4byte Data_020000f0
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	ldr r0, .L_02008050
	bx lr
.L_02008050:
	.4byte Data_02000100
	.section .text.x02008054,"ax",%progbits
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {lr}
	bl Func_020000b8
	pop {pc}
	.section .text.x0200805c,"ax",%progbits
	.global Func_0200005c
	.thumb_func
Func_0200005c:
	ldr r0, .L_02008060
	bx lr
.L_02008060:
	.4byte Data_02000130
	.section .text.x02008064,"ax",%progbits
	.global Func_02000064
	.thumb_func
Func_02000064:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
	str r2, [r3]
	movs r1, #2
	movs r0, #8
	bl ObjectMotion_SetActionVariant
	movs r0, #8
	bl Object_GetById
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #4
	orrs r3, r2
	strb r3, [r0]
	movs r0, #8
	bl Object_GetById
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #16
	orrs r3, r2
	strb r3, [r0]
	movs r0, #0
	pop {pc}
	.2byte 0x0000
	.section .text.x020080a4,"ax",%progbits
	.global Func_020000a4
	.thumb_func
Func_020000a4:
	movs r0, #0
	bx lr
	.section .rodata.x020080c0,"a",%progbits
	.global Data_020000c0
Data_020000c0:
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
	.global Data_020000f0
Data_020000f0:
	.4byte 0x000000b8
	.4byte 0x101070b6
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02000100
Data_02000100:
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
	.global Data_02000130
Data_02000130:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c400
	.4byte 0xffff0008
	.4byte Func_02000054
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
