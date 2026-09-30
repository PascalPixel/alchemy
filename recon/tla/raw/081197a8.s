.syntax unified
	.thumb
	.global Func_081197a8
	.thumb_func
Func_081197a8:
	push {lr}
	sub sp, #88
	movs r2, #0
	movs r4, #128
	adds r1, r0, #0
	movs r3, #1
	mov r0, sp
	str r2, [r0, #16]
	str r4, [r0, #8]
	str r2, [r0, #12]
	str r3, [r0, #20]
	strh r4, [r0, #36]
	str r2, [r0, #4]
	str r2, [r0]
	str r2, [r0, #24]
	bl Resource_FarCall00C + 0x50
	add sp, #88
	pop {pc}
	.2byte 0x0000
