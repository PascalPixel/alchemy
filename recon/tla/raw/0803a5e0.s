.syntax unified
	.thumb
	.global Func_0803a5e0
	.thumb_func
Func_0803a5e0:
	push {r5, lr}
	adds r5, r0, #0
	b .L_0803a5ec
.L_0803a5e6:
	movs r0, #1
	bl WaitFrames
.L_0803a5ec:
	bl UiWork_IsComplete
	cmp r0, #0
	beq .L_0803a5e6
	adds r0, r5, #0
	bl Func_0803a54c
	b .L_0803a602
.L_0803a5fc:
	movs r0, #1
	bl WaitFrames
.L_0803a602:
	bl UiWork_IsComplete
	cmp r0, #0
	beq .L_0803a5fc
	pop {r5, pc}
