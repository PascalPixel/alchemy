.syntax unified
	.thumb
	.global Func_081281ec
	.thumb_func
Func_081281ec:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #36]
	movs r2, #170
	lsls r2, r2, #3
	ldr r3, .L_08128224
	adds r1, r0, r2
	movs r2, #151
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r2, #0
	strh r2, [r3]
	str r2, [r1]
	str r2, [r1, #4]
	str r2, [r1, #8]
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #98
	movs r3, #3
	adds r0, r0, r1
.L_08128216:
	subs r3, #1
	strh r2, [r0]
	subs r0, #2
	cmp r3, #0
	bge .L_08128216
	pop {pc}
	.2byte 0x0000
.L_08128224:
	.4byte gPartyState
