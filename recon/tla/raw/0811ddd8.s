.syntax unified
	.thumb
	.global Func_0811ddd8
	.thumb_func
Func_0811ddd8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r7, r1, #0
	movs r1, #0
	str r1, [r7, #28]
	adds r6, r0, #0
	ldr r2, [r6, #88]
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #255
	ands r3, r2
	str r3, [r7]
	movs r3, #192
	lsls r3, r3, #6
	ands r2, r3
	lsrs r2, r2, #12
	str r2, [r7, #24]
	sub sp, #4
	ldrb r3, [r6]
	mov r10, r1
	str r3, [r7, #8]
	movs r3, #1
	ldrsb r3, [r6, r3]
	cmp r1, r3
	bge .L_0811de5a
	movs r2, #36
	adds r2, r2, r7
	adds r5, r6, #3
	mov r8, r2
.L_0811de16:
	ldrb r0, [r5]
	str r1, [sp, #0]
	bl Func_08123534
	ldr r1, [sp, #0]
	cmp r0, #0
	beq .L_0811de4c
	ldrb r0, [r5]
	bl Owner_GetState
	movs r2, #56
	ldrsh r3, [r0, r2]
	ldr r1, [sp, #0]
	cmp r3, #0
	bne .L_0811de40
	ldr r3, [r6, #88]
	movs r2, #128
	lsls r2, r2, #9
	ands r3, r2
	cmp r3, #0
	beq .L_0811de4c
.L_0811de40:
	ldrb r3, [r5]
	mov r2, r8
	strh r3, [r2]
	movs r3, #2
	add r8, r3
	adds r1, #1
.L_0811de4c:
	movs r3, #1
	ldrsb r3, [r6, r3]
	movs r2, #1
	add r10, r2
	adds r5, #1
	cmp r10, r3
	blt .L_0811de16
.L_0811de5a:
	cmp r1, #0
	bne .L_0811de64
	ldrb r3, [r6, #3]
	movs r1, #1
	strh r3, [r7, #36]
.L_0811de64:
	ldrb r3, [r6, #3]
	add sp, #4
	str r3, [r7, #12]
	movs r3, #1
	str r1, [r7, #20]
	str r3, [r7, #16]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
