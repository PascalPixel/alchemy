.syntax unified
	.thumb
	.global Func_08191cc4
	.thumb_func
Func_08191cc4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r2, r3, #0
	adds r2, #176
	ldr r2, [r2]
	ldr r1, [r3, #92]
	mov r10, r2
	movs r2, #240
	ldr r3, [r3, #36]
	lsls r2, r2, #7
	adds r2, #240
	adds r1, r1, r2
	mov r9, r3
	ldr r3, [r1]
	movs r7, #0
	ldr r3, [r3, #20]
	cmp r3, #0
	beq .L_08191d30
	movs r3, #0
	adds r6, r1, #0
	mov r8, r3
	movs r5, #36
.L_08191cfa:
	ldr r3, [r6]
	adds r7, #1
	ldrsh r0, [r3, r5]
	bl Func_08118088 + 0x10
	ldr r2, [r0]
	mov r3, r8
	str r3, [r2, #8]
	movs r3, #240
	lsls r3, r3, #15
	str r3, [r2, #12]
	mov r3, r8
	str r3, [r2, #16]
	movs r3, #171
	lsls r3, r3, #8
	adds r3, #133
	str r3, [r2, #72]
	ldr r3, [r6]
	movs r1, #1
	ldrsh r0, [r3, r5]
	bl Func_08118088
	ldr r3, [r6]
	adds r5, #2
	ldr r3, [r3, #20]
	cmp r7, r3
	bne .L_08191cfa
.L_08191d30:
	mov r2, r10
	movs r3, #0
	str r3, [r2, #16]
	ldr r2, .L_08191d5c
	movs r3, #120
	str r3, [r2, #12]
	str r3, [r2, #16]
	bl Func_08014c4c
	movs r3, #206
	lsls r3, r3, #3
	add r3, r9
	ldrh r1, [r3]
	movs r0, #1
	movs r2, #24
	bl Func_08118028 + 0x18
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_08191d5c:
	.4byte gCameraSceneParameters
