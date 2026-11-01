.syntax unified
	.thumb
	.section .text.x020081a0,"ax",%progbits
	.balign 4
	.global Func_020001a0
	.thumb_func
Func_020001a0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #136
	mov r8, r1
	cmp r1, #14
	bne .L_0200824c
	movs r0, #14
	bl Actor_GetById
	adds r7, r0, #0
	movs r0, #146
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008244
	ldr r3, [r7, #8]
	asrs r6, r3, #20
	cmp r6, #47
	bne .L_02008244
	ldr r3, [r7, #16]
	asrs r5, r3, #20
	cmp r5, #28
	bne .L_02008244
	movs r0, #14
	bl Func_0200015c
	movs r3, #1
	movs r2, #1
	movs r1, #28
	movs r0, #49
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl Func_0200106c
	movs r0, #146
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #14
	movs r1, #2
	bl Actor_SetSpritePriority
	movs r0, #10
	bl Actor_GetById
	adds r5, r0, #0
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #27
	bne .L_02008218
	movs r0, #2
	adds r0, #255
	bl Func_0200119c
	movs r3, #220
	b .L_02008228
.L_02008218:
	subs r3, #28
	cmp r3, #1
	bhi .L_0200822c
	movs r0, #129
	lsls r0, r0, #1
	bl Func_0200119c
	movs r3, #236
.L_02008228:
	lsls r3, r3, #17
	str r3, [r5, #16]
.L_0200822c:
	ldr r2, [r7, #16]
	ldr r3, [r5, #16]
	cmp r2, r3
	bge .L_02008238
	movs r0, #129
	b .L_0200823a
.L_02008238:
	movs r0, #130
.L_0200823a:
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	b .L_0200824c
.L_02008244:
	mov r0, r8
	movs r1, #1
	bl Actor_SetSpritePriority
.L_0200824c:
	mov r2, r8
	cmp r2, #15
	beq .L_02008254
	b .L_0200835a
.L_02008254:
	movs r0, #15
	bl Actor_GetById
	adds r6, r0, #0
	movs r0, #137
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200835a
	ldr r3, [r6, #8]
	asrs r3, r3, #20
	cmp r3, #53
	bne .L_0200835a
	ldr r3, [r6, #16]
	asrs r3, r3, #20
	cmp r3, #36
	bne .L_0200835a
	movs r0, #15
	bl Func_0200015c
	movs r1, #2
	movs r0, #15
	bl Actor_SetSpritePriority
	movs r0, #9
	bl Actor_GetById
	adds r7, r0, #0
	ldr r1, [r6, #8]
	ldr r3, [r7, #8]
	subs r2, r1, r3
	cmp r2, #0
	blt .L_020082a2
	movs r3, #128
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_020082ac
	b .L_020082e8
.L_020082a2:
	movs r2, #128
	subs r3, r3, r1
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_020082e8
.L_020082ac:
	adds r3, r7, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	adds r0, r7, #0
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	ldr r1, [r6, #8]
	bl Func_02001054
	adds r0, r7, #0
	bl Func_0200105c
	ldr r1, .L_02008364
	adds r0, r6, #0
	bl Func_02001044
	adds r0, r7, #0
	bl Func_0200104c
	movs r0, #128
	str r5, [r7, #8]
	str r5, [r7, #16]
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_0200835a
.L_020082e8:
	add r0, sp, #8
	adds r1, r7, #0
	movs r2, #128
	ldr r5, .L_02008368
	mov lr, r5
	.2byte 0xf800
	movs r0, #9
	bl Actor_ResetTargetsAndVelocity
	movs r1, #4
	movs r2, #0
	movs r0, #9
	bl Actor_Jump
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #9
	bl Func_0200111c
	movs r2, #0
	movs r1, #15
	movs r0, #9
	bl Actor_SetAngleToward
	movs r0, #60
	bl FieldEvent_Wait
	movs r1, #4
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl FieldEvent_Wait
	movs r2, #128
	add r1, sp, #8
	adds r0, r7, #0
	mov lr, r5
	.2byte 0xf800
	movs r3, #53
	movs r2, #36
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #53
	movs r1, #35
	movs r2, #1
	movs r3, #1
	bl Func_0200106c
	movs r0, #137
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_0200835a:
	add sp, #136
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008364:
	.4byte gData_02001404
.L_02008368:
	.4byte IwramCopyWords
