.syntax unified
	.thumb
	.global Func_08120158
	.thumb_func
Func_08120158:
	push {lr}
	b .L_08120162
.L_0812015c:
	movs r0, #1
	bl WaitFrames
.L_08120162:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_0812015c
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #148
	ldr r2, [r3]
	movs r3, #1
	str r3, [r2, #8]
	pop {pc}
