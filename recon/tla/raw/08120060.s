.syntax unified
	.thumb
	.global Func_08120060
	.thumb_func
Func_08120060:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	movs r0, #0
	sub sp, #20
	adds r6, r1, #0
	bl Ui_GetTableWordZeroFar
	mov r10, r0
	str r5, [sp, #0]
	str r6, [sp, #4]
	b .L_08120082
.L_0812007c:
	movs r0, #1
	bl WaitFrames
.L_08120082:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_0812007c
	movs r0, #128
	add r5, sp, #8
	bl Resource_LoadIntoFreeSlot
	mov r3, sp
	ldrh r3, [r3]
	movs r7, #128
	lsls r7, r7, #19
	adds r6, r0, #0
	adds r7, #74
	mov r8, r3
.L_081200a0:
	adds r0, r7, #0
	movs r1, #4
	bl Func_08013d0c
	adds r0, r7, #0
	movs r1, #16
	bl Func_08013c58
	movs r2, #16
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r2, [r3]
	movs r3, #128
	lsls r3, r3, #23
	str r3, [r5, #4]
	movs r3, #0
	str r3, [r5, #8]
	mov r1, r10
	adds r0, r6, #0
	bl Resource_GetBuffer
	ldr r3, .L_08120100
	ldr r2, .L_08120104
	ands r0, r3
	ldrh r3, [r5, #8]
	ldr r1, .L_08120108
	ands r3, r2
	orrs r3, r0
	strh r3, [r5, #8]
	ldr r3, .L_08120110
	ldr r0, [r3]
	movs r3, #4
	ands r0, r3
	movs r3, #255
	lsrs r2, r0, #1
	lsls r3, r3, #8
	adds r3, #252
	add r2, r8
	adds r2, r2, r3
	ldr r3, .L_0812010c
	lsrs r0, r0, #2
	ands r2, r3
	ldrh r3, [r5, #6]
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #6]
	b .L_08120114
.L_08120100:
	.4byte 0x000003ff
.L_08120104:
	.4byte 0xfffffc00
.L_08120108:
	.4byte 0xfffffe00
.L_0812010c:
	.4byte 0x000001ff
.L_08120110:
	.4byte gFrameCount
.L_08120114:
	ldr r3, [sp, #4]
	movs r1, #240
	subs r0, r3, r0
	adds r0, #248
	strb r0, [r5, #4]
	adds r0, r5, #0
	bl Runtime_PushSlotEntry
	ldr r3, .L_08120154
	movs r2, #129
	ldr r3, [r3, #4]
	lsls r2, r2, #2
	adds r2, #255
	ands r3, r2
	cmp r3, #0
	bne .L_0812013c
	movs r0, #1
	bl WaitFrames
	b .L_081200a0
.L_0812013c:
	adds r0, r6, #0
	bl Resource_ResetEntry
	movs r0, #1
	bl WaitFrames
	movs r0, #1
	add sp, #20
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_08120154:
	.4byte gInput
