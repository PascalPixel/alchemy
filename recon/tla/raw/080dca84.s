.syntax unified
	.thumb
	.global Func_080dca84
	.thumb_func
Func_080dca84:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #60]
	movs r3, #1
	strb r3, [r2, #4]
	ldr r2, .L_080dcacc
	ldr r3, .L_080dcad0
	movs r1, #192
	strh r2, [r3]
	adds r3, #4
	strh r2, [r3]
	adds r3, #16
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	subs r3, #16
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	lsls r1, r1, #4
	adds r3, #2
	strh r2, [r3]
	adds r1, #108
	ldr r0, .L_080dcad4
	bl Func_080145a8
	b .L_080dcad8
.L_080dcacc:
	.4byte 0x0000739c
.L_080dcad0:
	.4byte 0x050001e2
.L_080dcad4:
	.4byte Func_080dcdc8
.L_080dcad8:
	pop {pc}
	.2byte 0x0000
