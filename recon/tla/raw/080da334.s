.syntax unified
	.thumb
	.global Func_080da334
	.thumb_func
Func_080da334:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	mov r10, r0
	adds r0, r1, #0
	adds r5, r2, #0
	sub sp, #16
	bl ObjectTable_Get
	adds r7, r0, #0
	adds r0, r5, #0
	bl ObjectTable_Get
	adds r6, r0, #0
	cmp r7, #0
	beq .L_080da3ac
	cmp r6, #0
	beq .L_080da3ac
	ldr r2, .L_080da3b8
	movs r5, #128
	ldr r0, [r2]
	mov r8, r2
	bl Trig_Sin
	ldr r4, [r6, #8]
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	str r4, [sp, #0]
	lsls r5, r5, #12
	ldr r4, [r6, #12]
	lsls r0, r0, #2
	adds r4, r4, r5
	str r4, [sp, #4]
	adds r2, r2, r5
	ldr r4, [r6, #16]
	str r0, [sp, #12]
	mov r0, r10
	str r4, [sp, #8]
	bl Func_080da3bc
	mov r2, r8
	ldr r3, [r2]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	mov r2, r8
	str r3, [r2]
	movs r0, #6
	bl WaitFrames
	adds r0, r7, #0
	movs r1, #2
	bl Object_SetMode
	adds r0, r6, #0
	movs r1, #2
	bl Object_SetMode
.L_080da3ac:
	add sp, #16
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080da3b8:
	.4byte Data_080f3948
