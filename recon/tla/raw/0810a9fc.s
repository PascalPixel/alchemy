.syntax unified
	.thumb
	.global Func_0810a9fc
	.thumb_func
Func_0810a9fc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r6, #128
	lsls r6, r6, #3
	mov r8, r3
	adds r6, #220
	add r6, r8
	ldr r3, [r6]
	adds r7, r0, #0
	ldrb r3, [r3, #5]
	mov r10, r3
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #250
	add r3, r8
	ldrh r0, [r3]
	bl Func_080c85c8
	adds r5, r0, #0
	adds r0, r7, #0
	bl Func_0810a960
	ldr r2, [r6]
	movs r3, #13
	strb r3, [r2, #5]
	adds r7, r0, #0
	lsls r5, r5, #16
	bl UiWork_FinalizePendingCoreFar
	movs r3, #34
	orrs r5, r3
	adds r0, r7, #0
	movs r1, #5
	movs r2, #0
	adds r3, r5, #0
	bl UiText_OpenMessageWindowFar
	b .L_0810aa5a
.L_0810aa54:
	movs r0, #1
	bl WaitFrames
.L_0810aa5a:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_0810aa54
	movs r0, #1
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #220
	add r3, r8
	ldr r3, [r3]
	mov r2, r10
	strb r2, [r3, #5]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
