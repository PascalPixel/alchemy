.syntax unified
	.thumb
	.global Func_080d0bec
	.thumb_func
Func_080d0bec:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r1, #168
	mov r8, r0
	lsls r1, r1, #3
	movs r0, #124
	sub sp, #4
	bl Runtime_AllocateBlock
	movs r3, #128
	adds r5, r0, #0
	movs r6, #0
	mov r0, sp
	lsls r3, r3, #19
	str r6, [r0]
	adds r3, #212
	adds r1, r5, #0
	ldr r2, .L_080d0c48
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #0
	bl DisplayTransition_FillTilemapAndSolidTile
	movs r2, #165
	lsls r2, r2, #3
	adds r3, r5, r2
	mov r2, r8
	strh r2, [r3]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #42
	adds r5, r5, r3
	movs r1, #144
	lsls r1, r1, #3
	strh r6, [r5]
	ldr r0, .L_080d0c4c
	bl Scheduler_AddOrUpdateCallback
	movs r0, #120
	bl WaitFrames
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_080d0c48:
	.4byte 0x85000150
.L_080d0c4c:
	.4byte DisplayTransition_UpdateFrame
