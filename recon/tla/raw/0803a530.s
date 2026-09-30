.syntax unified
	.thumb
	.global Func_0803a530
	.thumb_func
Func_0803a530:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #148
	ldr r5, [r3]
	ldr r0, [r5]
	cmp r0, #0
	beq .L_0803a54a
	movs r1, #1
	bl UiWork_Finalize
	movs r3, #0
	str r3, [r5]
.L_0803a54a:
	pop {r5, pc}
