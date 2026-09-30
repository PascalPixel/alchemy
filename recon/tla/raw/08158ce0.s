.syntax unified
	.thumb
	.global Func_08158ce0
	.thumb_func
Func_08158ce0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r6, r0, #0
	adds r7, r1, #0
	movs r0, #238
	ldr r1, [r3, #92]
	lsls r0, r0, #7
	adds r0, #168
	adds r0, r0, r1
	ldr r3, [r0]
	mov r8, r0
	cmp r3, #0
	ble .L_08158d3a
	bl Random16
	subs r6, #1
	ands r6, r0
	bl Random16
	lsrs r5, r7, #31
	adds r5, r7, r5
	subs r2, r7, #1
	asrs r5, r5, #1
	ands r2, r0
	subs r2, r2, r5
	ldr r1, .L_08158d60
	adds r3, r2, #0
	subs r6, r6, r5
	adds r3, #32
	strh r6, [r1, #4]
	strh r3, [r1, #6]
	ldr r1, .L_08158d64
	movs r3, #120
	subs r6, r3, r6
	subs r3, r3, r2
	str r6, [r1, #12]
	str r3, [r1, #16]
	mov r2, r8
	ldr r3, [r2]
	subs r3, #1
	str r3, [r2]
	b .L_08158d58
.L_08158d3a:
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #160
	adds r3, r1, r0
	ldr r3, [r3]
	ldr r2, .L_08158d60
	adds r0, #4
	strh r3, [r2, #4]
	adds r3, r1, r0
	ldr r3, [r3]
	strh r3, [r2, #6]
	ldr r2, .L_08158d64
	movs r3, #120
	str r3, [r2, #12]
	str r3, [r2, #16]
.L_08158d58:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08158d60:
	.4byte Data_03001120
.L_08158d64:
	.4byte gCameraSceneParameters
