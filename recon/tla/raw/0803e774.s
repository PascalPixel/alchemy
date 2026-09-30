.syntax unified
	.thumb
	.global Func_0803e774
	.thumb_func
Func_0803e774:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #72]
	movs r4, #192
	lsls r4, r4, #2
	adds r4, #150
	adds r2, r3, r4
	adds r4, #2
	strh r0, [r2]
	adds r2, r3, r4
	strh r1, [r2]
	movs r2, #210
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0803e7a8
.L_0803e798:
	strh r0, [r3, #16]
	strh r0, [r3, #24]
	strh r1, [r3, #18]
	strh r1, [r3, #26]
	ldr r3, [r3, #4]
	adds r0, #16
	cmp r3, #0
	bne .L_0803e798
.L_0803e7a8:
	pop {pc}
	.2byte 0x0000
