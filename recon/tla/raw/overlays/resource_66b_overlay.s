.syntax unified
	.thumb
	.section .text.x0200810e,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200815e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020081ea,"ax",%progbits
	.2byte 0x0000
	.section .text.x020083f8,"ax",%progbits
	.global Func_020003f8
	.thumb_func
Func_020003f8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r6, r1, #0
	adds r0, r6, #0
	sub sp, #68
	bl Actor_GetById
	adds r7, r0, #0
	ldr r3, [r7, #8]
	asrs r3, r3, #20
	cmp r3, #76
	beq .L_02008416
	b .L_0200856c
.L_02008416:
	bl Func_0200120c
	movs r0, #0
	bl Func_020012e4
	movs r3, #74
	movs r2, #33
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #74
	movs r1, #31
	movs r2, #1
	bl Func_020011d4
	movs r0, #15
	bl Actor_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #76
	beq .L_02008444
	b .L_02008568
.L_02008444:
	movs r1, #85
	adds r2, r7, #0
	adds r1, r1, r7
	movs r3, #0
	adds r2, #89
	strb r3, [r1]
	strb r3, [r2]
	mov r8, r1
	movs r5, #0
.L_02008456:
	movs r0, #1
	bl wait
	ldr r2, [r7, #80]
	ldr r1, .L_02008578
	ldrh r3, [r2, #18]
	adds r5, #1
	adds r3, r3, r1
	strh r3, [r2, #18]
	ldr r3, [r7, #80]
	ldrh r0, [r3, #18]
	bl Math_Cosine
	lsrs r3, r0, #31
	adds r0, r0, r3
	ldr r3, [r7, #8]
	asrs r0, r0, #1
	subs r3, r3, r0
	str r3, [r7, #8]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	cmp r5, #17
	bls .L_02008456
	ldr r3, .L_0200857c
	movs r1, #192
	movs r2, #192
	str r3, [r7, #108]
	adds r0, r6, #0
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Actor_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #3
	lsls r2, r2, #2
	adds r0, r6, #0
	adds r1, #169
	adds r2, #18
	bl Actor_ResetAndSetPosition
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	mov r1, r8
	movs r3, #3
	str r2, [r7, #72]
	strb r3, [r1]
	adds r3, r7, #0
	adds r3, #34
	movs r5, #0
	strb r5, [r3]
	adds r0, r6, #0
	mov r10, r2
	bl Actor_WaitForMoveById
	movs r1, #0
	adds r0, r7, #0
	bl Func_020003dc
	movs r0, #188
	bl Func_0200131c
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl Func_020011f4
	movs r0, #141
	bl Func_0200131c
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_020011f4
	ldr r2, [r7, #80]
	movs r3, #128
	lsls r3, r3, #5
	strh r3, [r2, #18]
	movs r3, #138
	add r4, sp, #16
	lsls r3, r3, #1
	str r5, [r7, #108]
	strh r3, [r4, #24]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r4, #8]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r4, #16]
	ldr r3, .L_02008580
	mov r2, r10
	str r3, [r4, #20]
	movs r3, #224
	str r2, [r4, #12]
	lsls r3, r3, #13
	ldr r2, [r7, #16]
	ldr r1, [r7, #12]
	ldr r0, [r7, #8]
	str r3, [sp, #8]
	movs r3, #0
	str r5, [sp, #0]
	str r5, [sp, #4]
	str r4, [sp, #12]
	bl Func_020001ec
	movs r0, #154
	bl Func_0200131c
	adds r0, r6, #0
	movs r1, #3
	bl Actor_SetModeById
	bl Func_020011fc
	movs r0, #15
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #132
	bl GameFlag_SetBit
.L_02008568:
	bl Func_02001214
.L_0200856c:
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008578:
	.4byte 0xffffff00
.L_0200857c:
	.4byte Func_020003cc
.L_02008580:
	.4byte 0x00013333
	.section .bss,"aw",%nobits
	.global gData_02001ae0
gData_02001ae0:
