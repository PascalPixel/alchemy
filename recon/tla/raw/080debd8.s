.syntax unified
	.thumb
	.global Func_080debd8
	.thumb_func
Func_080debd8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	movs r0, #154
	ldr r5, [r3, #16]
	movs r7, #1
	ldr r6, [r5, #80]
	ldr r1, [r6, #40]
	mov r10, r1
	bl Audio_PlayCue
	ldr r0, .L_080dec84
	bl Func_08014644
	adds r0, r5, #0
	movs r1, #0
	bl Object_SetMode
	movs r3, #0
	str r3, [r5, #108]
	movs r2, #7
	movs r5, #0
	mov r9, r5
	mov r8, r2
.L_080dec14:
	mov r1, r10
	mov r3, r8
	strb r3, [r1, #5]
	movs r3, #2
	strb r3, [r6, #26]
	movs r0, #2
	strb r7, [r6, #25]
	bl WaitFrames
	mov r2, r9
	strb r7, [r6, #25]
	strb r2, [r6, #26]
	movs r0, #2
	adds r5, #1
	bl WaitFrames
	cmp r5, #4
	bls .L_080dec14
	movs r5, #0
	movs r3, #7
	mov r8, r5
	movs r7, #1
	mov r9, r3
.L_080dec42:
	mov r1, r9
	mov r2, r10
	mov r3, r8
	strb r1, [r2, #5]
	strb r3, [r6, #26]
	movs r0, #2
	strb r7, [r6, #25]
	bl WaitFrames
	mov r1, r8
	mov r2, r10
	strb r1, [r2, #5]
	strb r7, [r6, #25]
	movs r0, #2
	adds r5, #1
	bl WaitFrames
	cmp r5, #4
	bls .L_080dec42
	movs r3, #1
	strb r3, [r6, #26]
	movs r1, #183
	ldr r3, .L_080dec88
	lsls r1, r1, #1
	adds r1, #255
	movs r2, #0
	adds r3, r3, r1
	strb r2, [r3]
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_080dec84:
	.4byte Func_080deba4
.L_080dec88:
	.4byte gPartyState
