.syntax unified
	.thumb
	.global Func_080d85b8
	.thumb_func
Func_080d85b8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_080d8700
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	adds r7, r0, #0
	ldr r0, [r3]
	sub sp, #12
	bl Object_GetById
	movs r2, #64
	adds r2, r2, r7
	movs r1, #0
	ldrsb r1, [r2, r1]
	adds r5, r0, #0
	mov r8, r2
	ldrb r3, [r2]
	mov r10, r1
	cmp r1, #0
	bne .L_080d866c
	ldr r3, [r5, #8]
	mov r6, sp
	str r3, [r6]
	bl Random16
	ldr r3, [r5, #12]
	lsls r2, r0, #2
	adds r2, r2, r0
	adds r3, r3, r2
	movs r2, #240
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r6, #4]
	adds r0, r6, #0
	ldr r3, [r5, #16]
	str r3, [r6, #8]
	bl Func_080dc390
	bl Random16
	lsls r5, r0, #1
	adds r5, r5, r0
	movs r3, #128
	lsls r3, r3, #10
	lsls r5, r5, #1
	adds r5, r5, r3
	bl Random16
	adds r2, r6, #0
	adds r1, r0, #0
	adds r0, r5, #0
	bl Func_0801489c
	ldr r2, [r6]
	ldr r1, .L_080d8704
	str r2, [r7, #12]
	movs r5, #192
	ldr r3, [r6, #8]
	lsls r5, r5, #10
	str r3, [r7, #16]
	adds r3, r3, r1
	str r2, [r7, #4]
	str r3, [r7, #8]
	str r5, [r7, #36]
	bl Random16
	lsls r3, r0, #1
	adds r3, r3, r0
	adds r3, r3, r5
	str r3, [r7, #32]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #40]
	str r3, [r7, #44]
	adds r3, r7, #0
	adds r3, #66
	mov r2, r10
	strb r2, [r3]
	adds r2, r7, #0
	movs r3, #1
	adds r2, #65
	strb r3, [r2]
	mov r1, r8
	ldrb r3, [r1]
	adds r3, #1
	strb r3, [r1]
	b .L_080d86f6
.L_080d866c:
	subs r3, #1
	movs r2, #128
	lsls r3, r3, #24
	lsls r2, r2, #17
	cmp r3, r2
	bhi .L_080d86ea
	adds r0, r7, #0
	bl Func_080ebe70
	adds r6, r0, #0
	cmp r6, #0
	bne .L_080d86f6
	ldr r3, [r7, #4]
	mov r5, sp
	str r3, [r5]
	ldr r3, [r7, #8]
	str r3, [r5, #8]
	bl Random16
	adds r1, r0, #0
	movs r0, #192
	adds r2, r5, #0
	lsls r0, r0, #12
	bl Func_0801489c
	ldr r3, [r5]
	str r3, [r7, #12]
	ldr r3, [r5, #8]
	str r3, [r7, #16]
	adds r3, r7, #0
	adds r3, #65
	strb r6, [r3]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #28]
	str r6, [r7, #36]
	bl Random16
	ldr r3, .L_080d8708
	adds r0, r0, r3
	movs r3, #128
	lsls r3, r3, #8
	str r0, [r7, #32]
	str r3, [r7, #40]
	str r3, [r7, #44]
	movs r0, #143
	bl Audio_PlayCue
	mov r1, r8
	movs r3, #0
	ldrsb r3, [r1, r3]
	ldrb r2, [r1]
	cmp r3, #1
	bne .L_080d86de
	subs r3, r2, #1
	strb r3, [r1]
	b .L_080d86e4
.L_080d86de:
	adds r3, r2, #1
	mov r2, r8
	strb r3, [r2]
.L_080d86e4:
	movs r3, #6
	strh r3, [r7, #58]
	b .L_080d86f6
.L_080d86ea:
	mov r3, r10
	cmp r3, #3
	bne .L_080d86f6
	adds r0, r7, #0
	bl Func_080ebf68
.L_080d86f6:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_080d8700:
	.4byte gPartyState
.L_080d8704:
	.4byte 0xff9c0000
.L_080d8708:
	.4byte 0x00023333
