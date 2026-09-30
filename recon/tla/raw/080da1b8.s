.syntax unified
	.thumb
	.global Func_080da1b8
	.thumb_func
Func_080da1b8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #160
	ldr r3, [r3]
	mov r8, r1
	adds r7, r0, #0
	mov r0, r8
	mov r10, r2
	sub sp, #12
	mov r9, r3
	bl ObjectTable_Get
	adds r5, r0, #0
	mov r0, r10
	bl ObjectTable_Get
	adds r6, r0, #0
	cmp r5, #0
	beq .L_080da21e
	cmp r6, #0
	beq .L_080da21e
	lsls r3, r7, #3
	subs r3, r3, r7
	lsls r3, r3, #2
	add r3, r9
	adds r3, #12
	mov r2, r8
	strh r2, [r3]
	mov r2, r10
	strh r2, [r3, #2]
	movs r4, #128
	ldr r0, [r6, #8]
	ldr r2, [r5, #12]
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	str r0, [sp, #0]
	lsls r4, r4, #12
	ldr r0, [r6, #12]
	adds r2, r2, r4
	adds r0, r0, r4
	str r0, [sp, #4]
	ldr r0, [r6, #16]
	str r0, [sp, #8]
	adds r0, r7, #0
	bl Func_080da22c
.L_080da21e:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
