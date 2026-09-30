.syntax unified
	.thumb
	.global Func_0810857c
	.thumb_func
Func_0810857c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r6, [r3]
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #220
	adds r7, r6, r2
	ldr r3, [r7]
	adds r2, #30
	ldrb r3, [r3, #5]
	adds r5, r0, #0
	mov r10, r3
	adds r3, r6, r2
	ldrh r0, [r3]
	bl Func_080c85c8
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #6
	adds r3, r6, r2
	movs r1, #0
	ldrsb r1, [r3, r1]
	mov r8, r0
	cmp r1, #3
	bne .L_081085c0
	ldr r3, .L_08108620
	ldr r2, .L_08108624
	subs r3, r3, r2
	adds r5, r5, r3
.L_081085c0:
	cmp r1, #2
	bne .L_081085cc
	ldr r3, .L_08108628
	ldr r2, .L_08108624
	subs r3, r3, r2
	adds r5, r5, r3
.L_081085cc:
	cmp r1, #0
	bne .L_081085d8
	ldr r3, .L_0810862c
	ldr r2, .L_08108624
	subs r3, r3, r2
	adds r5, r5, r3
.L_081085d8:
	ldr r2, [r7]
	movs r3, #13
	strb r3, [r2, #5]
	bl UiWork_FinalizePendingCoreFar
	mov r2, r8
	lsls r3, r2, #16
	movs r2, #34
	orrs r3, r2
	adds r0, r5, #0
	movs r1, #5
	movs r2, #0
	bl UiText_OpenMessageWindowFar
	b .L_081085fc
.L_081085f6:
	movs r0, #1
	bl WaitFrames
.L_081085fc:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_081085f6
	movs r0, #1
	bl WaitFrames
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #220
	adds r3, r6, r2
	ldr r3, [r3]
	mov r2, r10
	strb r2, [r3, #5]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_08108620:
	.4byte 0x00001317
.L_08108624:
	.4byte 0x0000124c
.L_08108628:
	.4byte 0x00001277
.L_0810862c:
	.4byte 0x000012a2
