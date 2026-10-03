.syntax unified
	.thumb
	.global BattlePresentation_WaitForAdvance
	.thumb_func
BattlePresentation_WaitForAdvance:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	b .L_0811ff20
.L_0811ff1a:
	movs r0, #1
	bl WaitFrames
.L_0811ff20:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_0811ff1a
	movs r0, #128
	add r7, sp, #4
	bl Resource_LoadIntoFreeSlot
	mov r11, r0
	movs r0, #0
.L_0811ff34:
	str r0, [sp, #0]
	ldr r1, .L_0811ffcc
	movs r3, #7
	ldr r5, [r1]
	movs r6, #128
	lsrs r5, r5, #2
	ands r5, r3
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #148
	ldr r3, [r3]
	ldr r2, .L_0811ffd0
	ldr r0, [r3]
	lsls r6, r6, #19
	ldr r3, [r3, #4]
	adds r6, #74
	mov r8, r1
	mov r10, r0
	movs r1, #4
	adds r0, r6, #0
	lsls r5, r5, #7
	adds r5, r5, r2
	mov r9, r3
	bl Func_08013d0c
	adds r0, r6, #0
	movs r1, #16
	bl Func_08013c58
	movs r2, #16
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r2, [r3]
	movs r3, #164
	lsls r3, r3, #8
	str r3, [r7, #4]
	movs r3, #0
	str r3, [r7, #8]
	adds r1, r5, #0
	mov r0, r11
	bl Resource_GetBuffer
	ldr r3, .L_0811ffbc
	ldr r2, .L_0811ffc0
	ands r0, r3
	ldrh r3, [r7, #8]
	ands r3, r2
	orrs r3, r0
	strh r3, [r7, #8]
	mov r0, r9
	mov r3, r10
	movs r2, #12
	ldrsh r1, [r3, r2]
	ldrh r3, [r0, #4]
	lsls r1, r1, #3
	lsrs r3, r3, #8
	adds r1, r1, r3
	ldr r3, .L_0811ffc4
	adds r1, #4
	ands r1, r3
	ldr r2, .L_0811ffc8
	ldrh r3, [r7, #6]
	ands r3, r2
	orrs r3, r1
	mov r1, r8
	ldr r0, [r1]
	b .L_0811ffd4
.L_0811ffbc:
	.4byte 0x000003ff
.L_0811ffc0:
	.4byte 0xfffffc00
.L_0811ffc4:
	.4byte 0x000001ff
.L_0811ffc8:
	.4byte 0xfffffe00
.L_0811ffcc:
	.4byte gFrameCount
.L_0811ffd0:
	.4byte Data_0812996c
.L_0811ffd4:
	strh r3, [r7, #6]
	lsls r0, r0, #12
	bl Trig_Sin
	cmp r0, #0
	bge .L_0811ffe8
	movs r2, #254
	lsls r2, r2, #7
	adds r2, #255
	adds r0, r0, r2
.L_0811ffe8:
	mov r1, r10
	asrs r2, r0, #15
	movs r0, #14
	ldrsh r3, [r1, r0]
	mov r0, r9
	lsls r3, r3, #3
	adds r2, r2, r3
	ldrh r3, [r0, #6]
	movs r1, #240
	lsrs r3, r3, #8
	adds r3, r3, r2
	adds r3, #6
	strb r3, [r7, #4]
	adds r0, r7, #0
	bl Runtime_PushSlotEntry
	ldr r1, .L_0812005c
	movs r2, #2
	ldr r3, [r1]
	ands r3, r2
	cmp r3, #0
	bne .L_0812003c
	ldr r3, [r1, #4]
	movs r2, #129
	lsls r2, r2, #2
	adds r2, #255
	ands r3, r2
	cmp r3, #0
	bne .L_0812003c
	ldr r3, [sp, #0]
	cmp r3, #15
	ble .L_08120030
	ldr r3, [r1]
	ands r3, r2
	cmp r3, #0
	bne .L_0812003c
.L_08120030:
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #0]
	adds r0, #1
	b .L_0811ff34
.L_0812003c:
	movs r0, #111
	bl Audio_PlayCue
	mov r0, r11
	bl Resource_ResetEntry
	movs r0, #1
	bl WaitFrames
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0812005c:
	.4byte gInput
