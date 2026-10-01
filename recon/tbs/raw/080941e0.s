.syntax unified
	.thumb
	.global Func_080941e0
	.thumb_func
Func_080941e0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_080942c4
	movs r1, #247
	ldr r6, [r3]
	ldr r3, .L_080942c8
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r0, [r3, r2]
	sub sp, #8
	bl AudioCommand_PlayFar
	movs r0, #144
	lsls r0, r0, #1
	bl AudioCommand_PlayFar
	movs r0, #147
	bl AudioCommand_PlayFar
	movs r1, #207
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bne .L_08094266
	ldr r2, .L_080942cc
	ldr r3, .L_080942d0
	strh r2, [r3]
	ldr r0, .L_080942d4
	movs r1, #16
	bl DisplayTransition_Finish
	movs r3, #227
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r3, #0
	strh r3, [r2]
	movs r0, #16
	bl WaitFrames
	movs r7, #240
	movs r1, #240
	movs r5, #0
	lsls r1, r1, #2
	lsls r7, r7, #7
	movs r6, #30
.L_08094242:
	adds r3, r7, #0
	orrs r3, r1
	ldr r2, .L_080942d0
	orrs r3, r6
	strh r3, [r2]
	movs r0, #1
	str r1, [sp, #4]
	bl WaitFrames
	ldr r2, .L_080942d8
	ldr r1, [sp, #4]
	adds r5, #1
	subs r1, #64
	adds r7, r7, r2
	subs r6, #2
	cmp r5, #15
	ble .L_08094242
	b .L_080942b6
.L_08094266:
	ldr r3, .L_080942cc
	movs r5, #160
	lsls r5, r5, #19
	strh r3, [r5]
	ldr r0, .L_080942dc
	movs r1, #16
	bl DisplayTransition_Finish
	movs r3, #227
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r3, #0
	strh r3, [r2]
	movs r0, #16
	bl WaitFrames
	movs r7, #240
	movs r2, #240
	mov r8, r5
	lsls r2, r2, #2
	lsls r7, r7, #7
	movs r6, #30
	movs r5, #15
.L_08094294:
	adds r3, r7, #0
	orrs r3, r2
	orrs r3, r6
	mov r1, r8
	strh r3, [r1]
	movs r0, #1
	str r2, [sp, #0]
	bl WaitFrames
	ldr r3, .L_080942d8
	ldr r2, [sp, #0]
	subs r5, #1
	subs r2, #64
	adds r7, r7, r3
	subs r6, #2
	cmp r5, #0
	bge .L_08094294
.L_080942b6:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
.L_080942c4:
	.4byte gEventWork
.L_080942c8:
	.4byte gCell
.L_080942cc:
	.4byte 0x00007fff
.L_080942d0:
	.4byte 0x050001e6
.L_080942d4:
	.4byte 0x00000401
.L_080942d8:
	.4byte 0xfffff800
.L_080942dc:
	.4byte 0x00000207
