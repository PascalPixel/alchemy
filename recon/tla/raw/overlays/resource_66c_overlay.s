.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_02000230
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
	.4byte Data_02000260
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl Func_0200019c
	cmp r0, #0
	beq .L_02008060
	ldr r0, .L_02008064
	b .L_02008062
.L_02008060:
	ldr r0, .L_02008068
.L_02008062:
	pop {pc}
.L_02008064:
	.4byte Data_02000318
.L_02008068:
	.4byte Data_02000270
	.section .text.x0200806c,"ax",%progbits
	.global Func_0200006c
	.thumb_func
Func_0200006c:
	ldr r0, .L_02008070
	bx lr
.L_02008070:
	.4byte Data_02000360
	.section .text.x02008074,"ax",%progbits
	.global Func_02000074
	.thumb_func
Func_02000074:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl Func_0200019c
	cmp r0, #0
	bne .L_02008142
	bl Func_020001b4
	movs r0, #0
	bl Func_02000214
	ldr r0, .L_02008150
	bl Func_020001e4
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_02000204
	movs r0, #8
	movs r1, #1
	bl Func_020001fc
	bl Func_0200020c
	movs r2, #2
	movs r0, #10
	movs r1, #4
	bl Func_020001dc
	movs r1, #129
	movs r0, #10
	lsls r1, r1, #1
	bl Func_020001f4
	movs r2, #15
	movs r0, #10
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #10
	movs r1, #0
	bl Func_020001ec
	movs r0, #11
	movs r1, #4
	movs r2, #0
	bl Func_020001dc
	movs r0, #12
	movs r1, #4
	movs r2, #0
	bl Func_020001dc
	movs r0, #13
	movs r1, #4
	movs r2, #0
	bl Func_020001dc
	movs r2, #15
	movs r0, #11
	movs r1, #2
	bl ObjectMotion_Launch
	movs r0, #11
	movs r1, #0
	bl Func_020001ec
	movs r2, #0
	movs r0, #12
	movs r1, #11
	bl Func_020001dc
	movs r0, #12
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #12
	movs r1, #0
	bl Func_020001ec
	movs r2, #0
	movs r0, #12
	movs r1, #4
	bl Func_020001dc
	movs r0, #12
	movs r1, #0
	bl Func_020001ec
	movs r0, #13
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #13
	movs r1, #0
	bl Func_020001ec
	bl Func_020001bc
.L_02008142:
	movs r0, #137
	lsls r0, r0, #4
	adds r0, #255
	bl Func_020001a4
	pop {pc}
	.2byte 0x0000
.L_02008150:
	.4byte 0x00001850
	.section .text.x02008154,"ax",%progbits
	.global Func_02000154
	.thumb_func
Func_02000154:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
	str r2, [r3]
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #0
	pop {pc}
	.2byte 0x0000
	.section .text.x02008184,"ax",%progbits
	.global Func_02000184
	.thumb_func
Func_02000184:
	push {lr}
	bl Func_0200021c
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r0, #0
	pop {pc}
	.section .rodata.x02008224,"a",%progbits
.L_02008224:
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_02000230
Data_02000230:
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
	.global Data_02000260
Data_02000260:
	.4byte 0x00000068
	.4byte 0x10103066
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02000270
Data_02000270:
	.4byte 0xffff019a
	.4byte .L_02008224
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
	.4byte gOverlayArea + 0x4000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02003a90 + 0x570
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000318
Data_02000318:
	.4byte 0xffff019a
	.4byte .L_02008224
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
	.global Data_02000360
Data_02000360:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x098f000a
	.4byte Func_02000074
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
