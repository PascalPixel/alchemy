.syntax unified
	.thumb
	.global Func_0804db74
	.thumb_func
Func_0804db74:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #24
	add r1, sp, #8
	mov r9, r1
	movs r7, #0
	mov r2, r9
	strh r7, [r2]
	ldr r2, .L_0804dc64
	movs r1, #244
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r6, [r3, r1]
	movs r1, #245
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #10
	movs r2, #0
	ldrsh r3, [r3, r2]
	add r1, sp
	mov r10, r1
	mov r2, r10
	strh r3, [r2]
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	movs r3, #5
	movs r2, #30
	movs r0, #0
	bl UiWindow_Create
	mov r2, r10
	adds r1, r6, #0
	adds r7, r0, #0
	bl Func_0804dad0
	add r3, sp, #12
	mov r8, r3
	add r1, sp, #4
	mov r0, r8
	bl UiTextResource_Initialize
	ldr r2, .L_0804dc68
	ldr r3, [r2]
	cmp r3, #0
	beq .L_0804dbe6
	adds r5, r2, #0
.L_0804dbda:
	movs r0, #1
	bl WaitFrames
	ldr r3, [r5]
	cmp r3, #0
	bne .L_0804dbda
.L_0804dbe6:
	adds r1, r6, #0
	adds r0, r7, #0
	mov r2, r10
	mov r3, r9
	bl Func_0804dc6c
	movs r1, #1
	lsls r0, r0, #16
	asrs r5, r0, #16
	negs r1, r1
	cmp r5, r1
	bne .L_0804dc1c
	ldr r0, [sp, #4]
	bl UiTextResource_Release
	adds r0, r7, #0
	movs r1, #2
	bl UiWork_Finalize
	mov r3, r10
	adds r0, r6, #0
	movs r2, #0
	ldrsh r1, [r3, r2]
	bl Func_080c8268
	adds r0, r5, #0
	b .L_0804dc56
.L_0804dc1c:
	movs r1, #2
	negs r1, r1
	cmp r5, r1
	bne .L_0804dc36
	ldr r0, [sp, #4]
	bl UiTextResource_Release
	adds r0, r7, #0
	movs r1, #2
	bl UiWork_Finalize
	adds r0, r5, #0
	b .L_0804dc56
.L_0804dc36:
	mov r1, r9
	movs r2, #0
	ldrsh r3, [r1, r2]
	mov r0, r8
	lsls r2, r3, #3
	subs r2, r2, r3
	lsls r2, r2, #1
	adds r2, #60
	movs r1, #74
	bl UiTextResource_SetPosition
	movs r0, #1
	adds r6, r5, #0
	bl WaitFrames
	b .L_0804dbe6
.L_0804dc56:
	add sp, #24
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0804dc64:
	.4byte gPartyState
.L_0804dc68:
	.4byte gInput
