.syntax unified
	.thumb
	.global Unnamed_080bb7c0
	.thumb_func
Unnamed_080bb7c0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #0
	sub sp, #20
	adds r6, r1, #0
	bl Ui_GetTableWordZeroFar
	mov r11, r0
	str r5, [sp, #0]
	str r6, [sp, #4]
	b .L_080bb7e8
.L_080bb7e2:
	movs r0, #1
	bl WaitFrames
.L_080bb7e8:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_080bb7e2
	movs r0, #128
	add r5, sp, #8
	bl Resource_LoadIntoFreeSlot
	mov r2, sp
	ldrh r2, [r2]
	ldr r3, .L_080bb878
	mov r9, r2
	ldr r7, .L_080bb87c
	movs r2, #4
	adds r6, r0, #0
	mov r10, r3
	mov r8, r2
.L_080bb80a:
	adds r0, r7, #0
	movs r1, #4
	bl QueueIoWriteDelay10
	adds r0, r7, #0
	movs r1, #16
	bl QueueIoWriteDelay6
	movs r2, #16
	ldr r3, .L_080bb880
	strh r2, [r3]
	movs r3, #128
	lsls r3, r3, #23
	str r3, [r5, #4]
	movs r3, #0
	str r3, [r5, #8]
	mov r1, r11
	adds r0, r6, #0
	bl Resource_GetBuffer
	ldr r3, .L_080bb868
	ldr r2, .L_080bb86c
	ands r0, r3
	ldrh r3, [r5, #8]
	ands r3, r2
	orrs r3, r0
	mov r2, r10
	strh r3, [r5, #8]
	ldr r3, [r2]
	mov r2, r8
	ands r3, r2
	lsrs r3, r3, #1
	ldr r2, .L_080bb884
	add r3, r9
	adds r3, r3, r2
	ldr r2, .L_080bb870
	ldr r1, .L_080bb874
	ands r3, r2
	ldrh r2, [r5, #6]
	ands r2, r1
	orrs r2, r3
	strh r2, [r5, #6]
	mov r2, r10
	ldr r3, [r2]
	mov r2, r8
	b .L_080bb888
	.2byte 0x0000
.L_080bb868:
	.4byte 0x000003ff
.L_080bb86c:
	.4byte 0xfffffc00
.L_080bb870:
	.4byte 0x000001ff
.L_080bb874:
	.4byte 0xfffffe00
.L_080bb878:
	.4byte gFrameCount
.L_080bb87c:
	.4byte 0x0400004a
.L_080bb880:
	.4byte 0x04000052
.L_080bb884:
	.4byte 0x0000fffc
.L_080bb888:
	ands r3, r2
	ldr r2, [sp, #4]
	lsrs r3, r3, #2
	subs r3, r2, r3
	adds r3, #248
	strb r3, [r5, #4]
	adds r0, r5, #0
	movs r1, #240
	bl Runtime_PushSlotEntry
	ldr r3, .L_080bb8d0
	ldr r2, .L_080bb8d4
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_080bb8b0
	movs r0, #1
	bl WaitFrames
	b .L_080bb80a
.L_080bb8b0:
	adds r0, r6, #0
	bl Resource_ResetEntry
	movs r0, #1
	bl WaitFrames
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
.L_080bb8d0:
	.4byte gKeyState
.L_080bb8d4:
	.4byte 0x00000303
