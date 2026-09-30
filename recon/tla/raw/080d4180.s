.syntax unified
	.thumb
	.global Func_080d4180
	.thumb_func
Func_080d4180:
	push {r5, r6, lr}
	adds r0, r2, #0
	movs r2, #192
	lsls r2, r2, #18
	ldr r6, [r2, #108]
	adds r2, r3, #0
	adds r1, r0, #0
	cmp r2, #119
	ble .L_080d4196
	adds r2, #32
	b .L_080d4198
.L_080d4196:
	subs r2, #32
.L_080d4198:
	cmp r0, #8
	bge .L_080d419e
	movs r1, #8
.L_080d419e:
	movs r3, #156
	lsls r3, r3, #1
	cmp r1, r3
	ble .L_080d41a8
	adds r1, r3, #0
.L_080d41a8:
	cmp r2, #20
	bge .L_080d41ae
	movs r2, #20
.L_080d41ae:
	cmp r2, #220
	ble .L_080d41b4
	movs r2, #220
.L_080d41b4:
	movs r0, #226
	lsls r0, r0, #1
	adds r3, r6, r0
	movs r4, #0
	ldrsh r0, [r3, r4]
	movs r3, #1
	bl UiText_OpenMessageWindowFar
	adds r5, r0, #0
	b .L_080d41ce
.L_080d41c8:
	movs r0, #1
	bl WaitFrames
.L_080d41ce:
	adds r0, r5, #0
	bl UiWork_IsIdleFar
	cmp r0, #0
	beq .L_080d41c8
	movs r0, #226
	lsls r0, r0, #1
	adds r2, r6, r0
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	pop {r5, r6, pc}
	.2byte 0x0000
