.syntax unified
	.thumb
	.global Func_080dd820
	.thumb_func
Func_080dd820:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r5, #192
	lsls r5, r5, #18
	adds r3, r5, #0
	adds r3, #224
	ldr r7, [r3]
	movs r0, #6
	bl Func_080dd748
	movs r0, #8
	ldr r6, [r5, #108]
	bl Func_080d00f8
	ldr r2, [r7, #16]
	ldr r5, [r5, #124]
	movs r0, #160
	ldr r3, [r2, #8]
	lsls r0, r0, #3
	mov r8, r5
	adds r0, #44
	add r0, r8
	str r3, [r0]
	movs r1, #166
	ldr r3, [r2, #16]
	ldr r2, [r2, #12]
	lsls r1, r1, #3
	subs r3, r3, r2
	add r1, r8
	mov r10, r0
	movs r0, #128
	str r3, [r1]
	lsls r0, r0, #9
	mov r9, r1
	movs r1, #0
	bl BattleFx_ApplyColorToSourceBuffer
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	adds r0, #1
	bl Func_080d170c
	movs r0, #1
	bl BattleFx_StartBufferInterpolation
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #164
	adds r5, r6, r2
	movs r3, #0
	strb r3, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	bl Func_080ce31c
	adds r2, r0, #0
	movs r0, #160
	lsls r0, r0, #23
	adds r0, #5
	movs r1, #8
	bl Func_080ce458
	cmp r0, #0
	beq .L_080dd8b8
	movs r3, #24
	ldrsh r1, [r7, r3]
	movs r3, #26
	ldrsh r2, [r7, r3]
	bl Func_080ceafc
.L_080dd8b8:
	movs r0, #131
	bl Audio_PlayCue
	movs r1, #1
	strb r1, [r5]
	mov r0, r10
	ldr r2, [r0]
	cmp r2, #0
	bge .L_080dd8d2
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r2, r2, r3
.L_080dd8d2:
	movs r0, #192
	lsls r0, r0, #4
	adds r0, #168
	asrs r2, r2, #16
	adds r3, r6, r0
	strh r2, [r3]
	mov r3, r9
	ldr r2, [r3]
	cmp r2, #0
	bge .L_080dd8ee
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r2, r2, r0
.L_080dd8ee:
	movs r0, #192
	lsls r0, r0, #4
	adds r0, #170
	adds r3, r6, r0
	asrs r2, r2, #16
	strh r2, [r3]
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #166
	adds r2, r6, r3
	movs r3, #150
	lsls r3, r3, #2
	subs r0, #5
	strh r3, [r2]
	adds r3, r6, r0
	strb r1, [r3]
	bl Func_080cf554
	movs r6, #160
	lsls r6, r6, #3
	adds r6, #42
	movs r5, #0
	add r6, r8
.L_080dd91c:
	movs r0, #1
	bl WaitFrames
	strh r5, [r6]
	adds r5, #1
	cmp r5, #18
	ble .L_080dd91c
	adds r3, r7, #0
	adds r3, #33
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_080dd942
	movs r1, #144
	ldr r0, .L_080dd94c
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
.L_080dd942:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_080dd94c:
	.4byte Func_080dd754
