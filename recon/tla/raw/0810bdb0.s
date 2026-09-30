.syntax unified
	.thumb
	.global Func_0810bdb0
	.thumb_func
Func_0810bdb0:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r2, [r3]
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #230
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_0810bdf2
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #20
	subs r1, #30
	adds r6, r2, r3
	adds r5, r2, r1
	movs r7, #14
.L_0810bdda:
	ldrh r3, [r5]
	adds r5, #2
	cmp r3, #96
	beq .L_0810bdea
	adds r0, r6, #0
	movs r1, #240
	bl Func_08014128
.L_0810bdea:
	subs r7, #1
	adds r6, #12
	cmp r7, #0
	bge .L_0810bdda
.L_0810bdf2:
	pop {r5, r6, r7, pc}
