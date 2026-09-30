.syntax unified
	.thumb
	.global Func_0810be3c
	.thumb_func
Func_0810be3c:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r0, .L_0810be6c
	ldr r6, [r3]
	bl Func_08014644
	movs r5, #192
	lsls r5, r5, #4
	adds r5, #200
	movs r7, #14
.L_0810be54:
	ldrh r3, [r5, r6]
	cmp r3, #96
	beq .L_0810be60
	adds r0, r3, #0
	bl Func_08014274
.L_0810be60:
	subs r7, #1
	adds r5, #2
	cmp r7, #0
	bge .L_0810be54
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0810be6c:
	.4byte Func_0810bdb0
