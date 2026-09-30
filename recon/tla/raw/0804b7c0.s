.syntax unified
	.thumb
	.global Func_0804b7c0
	.thumb_func
Func_0804b7c0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #36]
	movs r1, #166
	ldr r3, .L_0804b870
	lsls r1, r1, #1
	adds r1, #255
	adds r3, r3, r1
	ldrb r3, [r3]
	sub sp, #132
	cmp r3, #2
	beq .L_0804b7ec
	cmp r3, #2
	ble .L_0804b7ec
	movs r1, #1
	mov r8, r1
	cmp r3, #4
	ble .L_0804b7f0
.L_0804b7ec:
	movs r3, #0
	mov r8, r3
.L_0804b7f0:
	mov r1, r8
	cmp r1, #0
	bne .L_0804b80e
	adds r3, r2, #0
	adds r3, #67
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_0804b808
	movs r3, #1
	mov r8, r3
.L_0804b808:
	mov r1, r8
	cmp r1, #0
	beq .L_0804b864
.L_0804b80e:
	movs r3, #42
	str r3, [sp, #0]
	movs r3, #4
	movs r1, #7
	movs r2, #30
	movs r0, #0
	bl UiWindow_Create
	mov r10, r0
	bl Func_080396bc
	add r5, sp, #4
	adds r1, r5, #0
	movs r2, #52
	ldr r0, .L_0804b874
	bl UiText_CopyMessageString
	adds r0, r5, #0
	mov r1, r10
	movs r2, #0
	movs r3, #4
	bl Func_0803aae4
	movs r5, #192
	ldr r7, .L_0804b878
	lsls r5, r5, #18
	movs r6, #3
	adds r5, #228
.L_0804b846:
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7, #4]
	ands r3, r6
	cmp r3, #0
	bne .L_0804b85c
	ldr r3, [r5]
	ldr r3, [r3, #76]
	cmp r3, #0
	bne .L_0804b846
.L_0804b85c:
	mov r0, r10
	movs r1, #1
	bl UiWork_Finalize
.L_0804b864:
	mov r0, r8
	add sp, #132
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0804b870:
	.4byte gPartyState
.L_0804b874:
	.4byte 0x00000c9a
.L_0804b878:
	.4byte gInput
