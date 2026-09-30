.syntax unified
	.thumb
	.global Func_080da660
	.thumb_func
Func_080da660:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #20
	str r2, [sp, #16]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #160
	mov r10, r0
	adds r0, r1, #0
	ldr r5, [r3]
	bl ObjectTable_Get
	adds r7, r0, #0
	ldr r0, [sp, #16]
	bl ObjectTable_Get
	adds r6, r0, #0
	cmp r7, #0
	beq .L_080da6d0
	cmp r6, #0
	beq .L_080da6d0
	ldr r2, .L_080da6dc
	str r7, [r5]
	ldr r0, [r2]
	str r6, [r5, #4]
	mov r8, r2
	bl Trig_Sin
	ldr r4, [r6, #8]
	movs r3, #128
	lsls r3, r3, #12
	ldr r2, [r7, #12]
	ldr r1, [r7, #8]
	mov r12, r3
	ldr r3, [r7, #16]
	str r4, [sp, #0]
	lsls r0, r0, #2
	ldr r4, [r6, #12]
	add r2, r12
	add r4, r12
	str r4, [sp, #4]
	ldr r4, [r6, #16]
	str r0, [sp, #12]
	mov r0, r10
	str r4, [sp, #8]
	bl Func_080da6e0
	mov r2, r8
	ldr r3, [r2]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	mov r2, r8
	str r3, [r2]
.L_080da6d0:
	add sp, #20
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080da6dc:
	.4byte Data_080f394c
