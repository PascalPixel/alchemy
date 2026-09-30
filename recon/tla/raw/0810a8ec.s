.syntax unified
	.thumb
	.global Func_0810a8ec
	.thumb_func
Func_0810a8ec:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r5, [r3]
	movs r1, #129
	lsls r1, r1, #3
	adds r1, #255
	adds r3, r5, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	subs r1, #3
	mov r10, r3
	adds r3, r5, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r2, #0
	sub sp, #4
	movs r7, #0
	cmp r2, r3
	bge .L_0810a952
	adds r3, r5, #2
	movs r6, #153
	mov r8, r3
	lsls r6, r6, #3
.L_0810a928:
	mov r1, r8
	ldrsh r0, [r1, r6]
	mov r1, r10
	str r2, [sp, #0]
	bl Shop_CanServe
	ldr r2, [sp, #0]
	cmp r0, #0
	beq .L_0810a93c
	adds r2, #1
.L_0810a93c:
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #4
	adds r3, r5, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	adds r7, #1
	adds r6, #2
	cmp r7, r3
	blt .L_0810a928
.L_0810a952:
	adds r0, r2, #0
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
