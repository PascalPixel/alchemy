.syntax unified
	.thumb
	.global Func_080fac58
	.thumb_func
Func_080fac58:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r6, [r3]
	movs r1, #139
	lsls r1, r1, #1
	adds r1, #255
	adds r3, r6, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080fac94
	movs r5, #0
.L_080fac72:
	asrs r5, r5, #24
	lsls r3, r5, #2
	adds r3, #248
	ldr r0, [r6, r3]
	movs r1, #1
	bl Animation_ApplyChildArgumentFar
	movs r1, #139
	lsls r1, r1, #1
	adds r1, #255
	adds r3, r6, r1
	adds r5, #1
	ldrb r3, [r3]
	lsls r5, r5, #24
	asrs r2, r5, #24
	cmp r2, r3
	blt .L_080fac72
.L_080fac94:
	ldr r0, .L_080fac9c
	bl Scheduler_RemoveCallback
	pop {r5, r6, pc}
.L_080fac9c:
	.4byte Func_080fabe0
