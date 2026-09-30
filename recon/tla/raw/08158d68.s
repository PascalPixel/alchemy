.syntax unified
	.thumb
	.global Func_08158d68
	.thumb_func
Func_08158d68:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #92]
	adds r5, r0, #0
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #168
	adds r6, r1, r0
	ldr r3, [r6]
	cmp r3, #0
	ble .L_08158da8
	bl Random16
	lsrs r3, r5, #31
	subs r2, r5, #1
	adds r3, r5, r3
	asrs r3, r3, #1
	ands r2, r0
	subs r2, r2, r3
	ldr r1, .L_08158dc0
	adds r3, r2, #0
	adds r3, #32
	strh r3, [r1, #6]
	ldr r1, .L_08158dc4
	movs r3, #120
	subs r3, r3, r2
	str r3, [r1, #16]
	ldr r3, [r6]
	subs r3, #1
	str r3, [r6]
	b .L_08158dbc
.L_08158da8:
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #164
	adds r3, r1, r0
	ldr r3, [r3]
	ldr r2, .L_08158dc0
	strh r3, [r2, #6]
	ldr r2, .L_08158dc4
	movs r3, #120
	str r3, [r2, #16]
.L_08158dbc:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_08158dc0:
	.4byte Data_03001120
.L_08158dc4:
	.4byte gCameraSceneParameters
