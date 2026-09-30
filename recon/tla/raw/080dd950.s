.syntax unified
	.thumb
	.global Func_080dd950
	.thumb_func
Func_080dd950:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r2, r3, #0
	adds r2, #224
	ldr r6, [r3, #108]
	ldr r7, [r2]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #164
	adds r5, r6, r2
	ldr r1, [r3, #124]
	movs r3, #0
	ldrsb r3, [r5, r3]
	mov r8, r1
	cmp r3, #0
	beq .L_080dda28
	movs r0, #167
	bl Audio_PlayCue
	ldr r0, .L_080dda24
	bl Scheduler_RemoveCallback
	movs r1, #192
	lsls r1, r1, #4
	movs r2, #0
	adds r1, #166
	strb r2, [r5]
	adds r3, r6, r1
	movs r5, #128
	strh r2, [r3]
	movs r0, #0
	lsls r5, r5, #9
	bl Func_080dd748
	movs r1, #1
	adds r0, r5, #0
	bl Func_080d170c
	movs r0, #1
	bl Func_080d17ac
	movs r0, #0
	movs r1, #0
	bl Func_080d172c
	movs r1, #0
	adds r0, r5, #0
	bl Func_080d170c
	movs r0, #30
	bl Func_080d17ac
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	bl Func_080ce31c
	adds r2, r0, #0
	movs r0, #128
	lsls r0, r0, #23
	adds r0, #5
	movs r1, #8
	bl Func_080ce458
	cmp r0, #0
	beq .L_080dd9e8
	movs r2, #24
	ldrsh r1, [r7, r2]
	movs r3, #26
	ldrsh r2, [r7, r3]
	bl Func_080ceafc
.L_080dd9e8:
	adds r3, r7, #0
	adds r3, #52
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_080dda28
	movs r2, #160
	ldr r3, .L_080dda20
	lsls r2, r2, #3
	adds r2, #62
	add r2, r8
	strb r3, [r2]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #60
	add r3, r8
	movs r2, #1
	strb r2, [r3]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #61
	add r3, r8
	strb r2, [r3]
	movs r0, #10
	bl WaitFrames
	b .L_080dda28
.L_080dda20:
	.4byte 0x00000000
.L_080dda24:
	.4byte Func_080dd754
.L_080dda28:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
