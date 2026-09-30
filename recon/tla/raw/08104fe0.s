.syntax unified
	.thumb
	.global Func_08104fe0
	.thumb_func
Func_08104fe0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r5, [r3]
	mov r8, r0
	movs r0, #138
	lsls r0, r0, #1
	mov r9, r1
	mov r10, r2
	movs r1, #7
	movs r2, #0
	adds r3, r5, r0
.L_08105002:
	subs r1, #1
	str r2, [r3]
	subs r3, #4
	cmp r1, #0
	bge .L_08105002
	movs r0, #0
	bl Func_08104ef8
	movs r2, #140
	movs r3, #148
	lsls r2, r2, #1
	adds r6, r5, #0
	lsls r3, r3, #1
	movs r1, #0
	adds r4, r5, r2
	movs r7, #0
	adds r6, #248
	adds r0, r5, r3
.L_08105026:
	ldmia r6!, {r3}
	lsls r2, r1, #2
	mov r12, r2
	cmp r3, #0
	beq .L_0810505c
	mov r3, r8
	movs r2, #12
	ldrsh r3, [r3, r2]
	mov r2, r8
	add r3, r9
	adds r3, r7, r3
	lsls r3, r3, #3
	strh r3, [r4]
	movs r3, #14
	ldrsh r2, [r2, r3]
	mov lr, r2
	mov r3, lr
	add r3, r10
	lsls r3, r3, #3
	adds r3, #16
	movs r2, #156
	strh r3, [r0]
	lsls r2, r2, #1
	movs r3, #128
	add r2, r12
	lsls r3, r3, #9
	str r3, [r5, r2]
.L_0810505c:
	adds r1, #1
	adds r4, #2
	adds r7, #3
	adds r0, #2
	cmp r1, #3
	ble .L_08105026
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #9
	adds r2, r5, r3
	movs r1, #144
	movs r3, #1
	strb r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_08105088
	bl Func_080145a8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_08105088:
	.4byte Func_08104da8
