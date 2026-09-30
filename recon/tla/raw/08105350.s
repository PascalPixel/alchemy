.syntax unified
	.thumb
	.global Func_08105350
	.thumb_func
Func_08105350:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r4, [r3]
	lsls r2, r0, #2
	movs r0, #136
	lsls r0, r0, #2
	adds r3, r2, r0
	ldr r3, [r4, r3]
	cmp r3, #0
	beq .L_0810536e
	adds r0, #32
	adds r3, r2, r0
	str r1, [r4, r3]
.L_0810536e:
	pop {pc}
