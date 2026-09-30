.syntax unified
	.thumb
	.global Menu_EndResourceSelection
	.thumb_func
Menu_EndResourceSelection:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #232
	ldr r5, [r3]
	ldr r0, .L_0804d168
	bl Scheduler_RemoveCallback
	ldr r0, [r5, #120]
	cmp r0, #0
	beq .L_0804d134
	movs r1, #2
	bl UiWork_Finalize
.L_0804d134:
	adds r2, r5, #0
	adds r2, #142
	movs r1, #0
	ldrsh r3, [r2, r1]
	movs r6, #0
	cmp r6, r3
	bge .L_0804d158
	adds r7, r2, #0
	adds r5, #18
.L_0804d146:
	ldrh r0, [r5]
	bl Resource_ResetEntry
	movs r2, #0
	ldrsh r3, [r7, r2]
	adds r6, #1
	adds r5, #20
	cmp r6, r3
	blt .L_0804d146
.L_0804d158:
	movs r0, #232
	bl Runtime_ReleaseHeapBlock
	movs r0, #1
	bl WaitFrames
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0804d168:
	.4byte Func_0804cda8
