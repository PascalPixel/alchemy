.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_020001b4
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
	.4byte Data_020001e4
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	ldr r0, .L_02008050
	bx lr
.L_02008050:
	.4byte Data_020001fc
	.section .text.x02008054,"ax",%progbits
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {lr}
	movs r1, #13
	movs r2, #14
	movs r0, #1
	bl Func_020001a0
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	pop {pc}
	.section .text.x0200806c,"ax",%progbits
	.global Func_0200006c
	.thumb_func
Func_0200006c:
	ldr r0, .L_02008070
	bx lr
.L_02008070:
	.4byte Data_020002bc
	.section .text.x02008074,"ax",%progbits
	.global Func_02000074
	.thumb_func
Func_02000074:
	push {lr}
	movs r0, #192
	lsls r0, r0, #18
	ldr r1, [r0, #32]
	movs r3, #13
	ldrb r2, [r1, #23]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	ldr r2, .L_020080f0
	strb r3, [r1, #23]
	movs r3, #0
	str r3, [r2]
	ldr r3, [r0, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #144
	subs r2, #172
	lsls r1, r1, #3
	str r2, [r3]
	ldr r0, .L_020080f4
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_020080f8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	bl Func_02000190
	movs r2, #11
	movs r1, #12
	movs r0, #0
	bl Func_02000198
	movs r0, #12
	movs r1, #4
	bl Object_SetModeById
	movs r0, #11
	movs r1, #4
	bl Object_SetModeById
	movs r1, #130
	lsls r1, r1, #1
	movs r0, #1
	adds r1, #255
	movs r2, #13
	movs r3, #14
	bl Func_020001a8
	movs r0, #0
	pop {pc}
.L_020080f0:
	.4byte Data_020001b0
.L_020080f4:
	.4byte Func_020000fc
.L_020080f8:
	.4byte gPartyState
	.section .text.x020080fc,"ax",%progbits
	.global Func_020000fc
	.thumb_func
Func_020000fc:
	push {lr}
	ldr r3, .L_02008160
	ldr r2, [r3]
	movs r3, #15
	ands r2, r3
	cmp r2, #0
	bne .L_0200815c
	ldr r1, .L_02008164
	ldr r3, [r1]
	cmp r3, #8
	bne .L_02008114
	str r2, [r1]
.L_02008114:
	ldr r3, [r1]
	ldr r0, .L_02008168
	adds r2, r3, #0
	adds r3, #1
	str r3, [r1]
	lsls r3, r2, #1
	adds r3, r3, r0
	ldrh r3, [r3]
	movs r1, #160
	lsls r1, r1, #19
	adds r1, #194
	strh r3, [r1]
	movs r1, #7
	adds r3, r2, #1
	ands r3, r1
	lsls r3, r3, #1
	adds r3, r3, r0
	ldrh r3, [r3]
	movs r4, #160
	lsls r4, r4, #19
	adds r4, #196
	strh r3, [r4]
	adds r3, r2, #2
	ands r3, r1
	lsls r3, r3, #1
	adds r3, r3, r0
	ldrh r3, [r3]
	adds r4, #2
	adds r2, #3
	strh r3, [r4]
	ands r2, r1
	lsls r2, r2, #1
	adds r2, r2, r0
	ldrh r3, [r2]
	adds r4, #2
	strh r3, [r4]
.L_0200815c:
	pop {pc}
	.2byte 0x0000
.L_02008160:
	.4byte Data_0300122c
.L_02008164:
	.4byte Data_020001b0
.L_02008168:
	.4byte 0x05000180
	.section .text.x0200816c,"ax",%progbits
	.global Func_0200016c
	.thumb_func
Func_0200016c:
	movs r0, #0
	bx lr
	.section .rodata.x020081b0,"a",%progbits
	.global Data_020001b0
Data_020001b0:
	.4byte 0x00000000
	.global Data_020001b4
Data_020001b4:
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
	.global Data_020001e4
Data_020001e4:
	.4byte 0x00000036
	.4byte 0x1010c002
	.4byte 0xffffffff
	.4byte 0x1020d002
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_020001fc
Data_020001fc:
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
	.global Data_020002bc
Data_020002bc:
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
	.4byte Data_02030000 + 0xd
	.4byte Func_02000054
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
