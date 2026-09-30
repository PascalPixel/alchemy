.syntax unified
	.thumb
	.global Func_080dba5c
	.thumb_func
Func_080dba5c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r7, [r3]
	sub sp, #16
	ldr r2, [r7, #16]
	ldrh r3, [r7]
	mov r9, r2
	movs r2, #192
	lsls r2, r2, #8
	cmp r3, r2
	bne .L_080dbb2c
	movs r3, #224
	lsls r3, r3, #3
	adds r3, #242
	movs r5, #128
	adds r2, r7, r3
	lsls r5, r5, #2
	movs r3, #1
	strh r3, [r2]
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	adds r6, r0, #0
	ldr r3, .L_080dbb38
	movs r2, #0
	adds r1, r5, #0
	mov lr, r3
	.2byte 0xf800
	bl Resource_FindFreeEntry
	adds r1, r5, #0
	adds r2, r6, #0
	mov r8, r0
	bl VramBlock_LoadCached
	movs r2, #254
	lsls r2, r2, #3
	adds r3, r7, r2
	movs r2, #0
	mov r10, r2
	mov r2, r8
	adds r5, r0, #0
	strh r2, [r3]
	adds r0, r6, #0
	bl Sys_Free
	movs r3, #249
	lsls r3, r3, #3
	adds r6, r7, r3
	movs r3, #128
	lsls r3, r3, #24
	adds r0, r6, #0
	movs r1, #16
	movs r2, #31
	str r5, [sp, #0]
	bl Func_080eaf98
	ldrb r3, [r6, #9]
	mov r2, r10
	strh r2, [r6, #30]
	movs r2, #13
	ldrb r1, [r6, #5]
	negs r2, r2
	ands r2, r3
	movs r3, #33
	negs r3, r3
	ands r3, r1
	strb r3, [r6, #5]
	movs r3, #15
	ands r2, r3
	movs r3, #128
	lsls r3, r3, #10
	strb r2, [r6, #9]
	str r3, [r6, #24]
	ldr r3, [r7, #4]
	add r5, sp, #4
	str r3, [r5]
	mov r2, r9
	ldr r3, [r2, #12]
	movs r2, #224
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r5, #4]
	mov r2, r9
	ldr r3, [r2, #16]
	adds r0, r5, #0
	str r3, [r5, #8]
	bl Camera_WorldToScreen
	ldr r3, [r5]
	movs r1, #144
	str r3, [r6, #12]
	ldr r0, .L_080dbb3c
	ldr r3, [r5, #8]
	lsls r1, r1, #3
	str r3, [r6, #16]
	bl Scheduler_AddOrUpdateCallback
.L_080dbb2c:
	add sp, #16
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_080dbb38:
	.4byte IwramFillWords
.L_080dbb3c:
	.4byte Func_080dba44
