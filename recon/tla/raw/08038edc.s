.syntax unified
	.thumb
	.global RenderOutput_ReleaseFree
	.thumb_func
RenderOutput_ReleaseFree:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #60]
	movs r1, #230
	lsls r1, r1, #3
	adds r3, r2, r1
	cmp r0, r3
	bcc .L_08038f06
	movs r1, #227
	lsls r1, r1, #4
	adds r3, r2, r1
	cmp r0, r3
	bcs .L_08038f06
	adds r1, #4
	adds r3, r2, r1
	ldr r2, [r3]
	str r0, [r3]
	movs r3, #0
	str r0, [r2]
	str r3, [r0]
.L_08038f06:
	pop {pc}
