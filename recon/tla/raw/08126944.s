.syntax unified
	.thumb
	.global Func_08126944
	.thumb_func
Func_08126944:
	push {r5, r6, r7, lr}
	movs r2, #128
	ldr r3, .L_0812695c
	lsls r2, r2, #19
	adds r2, #80
	movs r7, #128
	strh r3, [r2]
	ldr r6, .L_08126960
	lsls r7, r7, #19
	movs r5, #1
	adds r7, #82
	b .L_08126964
.L_0812695c:
	.4byte 0x00002044
.L_08126960:
	.4byte 0x00001010
.L_08126964:
	subs r3, r6, r5
	strh r3, [r7]
	movs r0, #1
	adds r5, #2
	bl WaitFrames
	cmp r5, #16
	ble .L_08126964
	pop {r5, r6, r7, pc}
	.2byte 0x0000
