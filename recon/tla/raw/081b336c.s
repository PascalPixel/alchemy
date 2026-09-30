.syntax unified
	.thumb
	.global Func_081b336c
	.thumb_func
Func_081b336c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #180
	ldr r3, [r3]
	ldr r2, [r2, #92]
	mov r8, r3
	ldr r3, .L_081b33ec
	mov r10, r2
	movs r2, #128
	sub sp, #4
	movs r5, #0
	movs r6, #0
	movs r1, #0
	lsls r2, r2, #4
.L_081b3392:
	adds r6, #1
	str r1, [r3]
	adds r3, #28
	cmp r6, r2
	bne .L_081b3392
	ldr r0, .L_081b33f0
	ldr r1, .L_081b33f4
	ldr r7, .L_081b33f8
	movs r6, #0
	movs r4, #0
	mov r2, r10
.L_081b33a8:
	adds r3, r5, #0
	adds r3, #24
	lsls r3, r3, #16
	str r3, [r2]
	ldrb r3, [r1]
	adds r6, #1
	str r0, [r2, #4]
	str r4, [r2, #16]
	str r4, [r2, #24]
	adds r1, #1
	adds r5, r5, r3
	adds r0, r0, r7
	adds r2, #28
	cmp r6, #8
	bne .L_081b33a8
	movs r3, #155
	ldr r2, .L_081b33e8
	lsls r3, r3, #3
	movs r6, #0
	add r3, r8
.L_081b33d0:
	adds r6, #1
	strh r2, [r3]
	adds r3, #2
	cmp r6, #160
	bne .L_081b33d0
	movs r7, #160
	lsls r7, r7, #3
	adds r7, #6
	movs r6, #0
	add r7, r8
	b .L_081b33fc
	.2byte 0x0000
.L_081b33e8:
	.4byte 0x00000000
.L_081b33ec:
	.4byte Data_02010018
.L_081b33f0:
	.4byte 0xffe00000
.L_081b33f4:
	.4byte Data_081b48c6
.L_081b33f8:
	.4byte 0xfff80000
.L_081b33fc:
	movs r3, #154
	adds r3, #255
	adds r5, r6, #0
	muls r5, r3
	adds r0, r5, #0
	bl Trig_Cos
	lsls r3, r0, #1
	adds r3, r3, r0
	lsrs r3, r3, #15
	strh r3, [r7]
	adds r0, r5, #0
	bl Trig_Cos
	movs r2, #110
	subs r2, r2, r6
	movs r1, #155
	lsls r3, r0, #1
	lsls r1, r1, #3
	lsls r2, r2, #1
	adds r3, r3, r0
	adds r2, r2, r1
	lsrs r3, r3, #15
	mov r1, r8
	adds r6, #1
	adds r7, #2
	strh r3, [r1, r2]
	cmp r6, #40
	bne .L_081b33fc
	mov r3, r8
	movs r2, #0
	adds r3, #148
	str r2, [r3]
	movs r1, #239
	subs r3, #8
	str r2, [r3]
	lsls r1, r1, #7
	adds r3, #4
	str r2, [r3]
	add r1, r10
	movs r3, #1
	str r3, [r1]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	add r3, r10
	str r2, [r3]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #80
	strh r2, [r3]
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #12
	movs r3, #4
	movs r0, #18
	bl UiWindow_CreateFar
	movs r6, #128
	ldr r5, .L_081b34a4
	lsls r6, r6, #3
	adds r6, #204
	adds r1, r0, #0
	add r6, r8
	str r1, [r6]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #8
	subs r5, #1
	bl UiText_DrawCharacterAtOffsetFar
	ldr r1, [r6]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081b34a4:
	.4byte 0x00000d68
