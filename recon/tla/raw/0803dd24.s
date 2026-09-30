.syntax unified
	.thumb
	.global Func_0803dd24
	.thumb_func
Func_0803dd24:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #72]
	movs r2, #210
	lsls r2, r2, #2
	adds r3, r1, r2
	movs r0, #0
	adds r2, #82
	str r0, [r3]
	adds r3, r1, r2
	strh r0, [r3]
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #158
	adds r4, r1, r3
	ldrh r2, [r4]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	beq .L_0803dd58
	movs r2, #231
	lsls r2, r2, #2
	adds r3, r1, r2
	strh r0, [r3]
	strh r0, [r4]
.L_0803dd58:
	movs r2, #232
	lsls r2, r2, #2
	adds r3, r1, r2
	subs r2, #12
	strh r0, [r3]
	adds r3, r1, r2
	strh r0, [r3]
	pop {pc}
