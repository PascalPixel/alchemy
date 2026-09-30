.syntax unified
	.thumb
	.global Func_0810532c
	.thumb_func
Func_0810532c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r2, [r3]
	movs r4, #136
	lsls r3, r0, #2
	lsls r4, r4, #2
	adds r3, r3, r4
	ldr r3, [r2, r3]
	cmp r3, #0
	beq .L_0810534e
	lsls r3, r0, #1
	movs r0, #148
	lsls r0, r0, #2
	adds r3, r3, r0
	strh r1, [r2, r3]
.L_0810534e:
	pop {pc}
