.syntax unified
	.thumb
	.global Func_0803dd68
	.thumb_func
Func_0803dd68:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #72]
	movs r3, #229
	lsls r3, r3, #2
	adds r4, r2, r3
	ldrh r3, [r4]
	cmp r3, #16
	beq .L_0803dd96
	lsls r3, r3, #1
	mov r12, r3
	movs r3, #213
	lsls r3, r3, #2
	add r3, r12
	strh r0, [r2, r3]
	movs r3, #221
	lsls r3, r3, #2
	add r3, r12
	strh r1, [r2, r3]
	ldrh r3, [r4]
	adds r3, #1
	strh r3, [r4]
.L_0803dd96:
	pop {pc}
