.syntax unified
	.thumb
	.global Func_0811bd50
	.thumb_func
Func_0811bd50:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #36]
	adds r7, r1, #0
	adds r1, r2, #0
	sub sp, #12
	adds r1, #132
	cmp r0, #7
	ble .L_0811bd66
	subs r0, #120
.L_0811bd66:
	adds r0, #116
	ldrb r3, [r2, r0]
	movs r5, #0
	cmp r3, #255
	beq .L_0811bd78
	ldrb r3, [r2, r0]
	movs r2, #44
	muls r3, r2
	adds r5, r1, r3
.L_0811bd78:
	ldr r5, [r5]
	movs r1, #0
	adds r0, r5, #0
	bl GetMotionRecord
	adds r6, r0, #0
	bl Func_0811bd10
	ldr r3, [r5, #8]
	mov r0, sp
	str r3, [r0]
	adds r1, r7, #0
	ldr r3, [r5, #12]
	str r3, [r0, #4]
	ldr r3, [r5, #16]
	str r3, [r0, #8]
	bl Render_ProjectPoint
	ldr r1, [r6, #12]
	ldr r3, .L_0811bdac
	mov lr, r3
	.2byte 0xf800
	movs r0, #0
	add sp, #12
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0811bdac:
	.4byte IwramMulQ16
