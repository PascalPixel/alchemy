.syntax unified
	.thumb
	.global Func_0802d4d8
	.thumb_func
Func_0802d4d8:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r5, r0, #0
	ldr r0, [r3, #32]
	adds r4, r2, #0
	asrs r1, r1, #16
	asrs r4, r4, #16
	cmp r0, #0
	beq .L_0802d500
	movs r2, #3
	ands r2, r5
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #158
	lsls r3, r3, #3
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r0, [r0, r3]
	b .L_0802d502
.L_0802d500:
	ldr r0, .L_0802d528
.L_0802d502:
	cmp r1, #0
	bge .L_0802d508
	adds r1, #15
.L_0802d508:
	adds r2, r4, #0
	asrs r1, r1, #4
	cmp r2, #0
	bge .L_0802d512
	adds r2, #15
.L_0802d512:
	asrs r3, r2, #4
	lsls r3, r3, #7
	adds r3, r1, r3
	ldrb r3, [r0, r3]
	ldr r2, .L_0802d52c
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrb r3, [r3]
	movs r0, #15
	ands r0, r3
	pop {r5, pc}
.L_0802d528:
	.4byte Data_02024000
.L_0802d52c:
	.4byte gMapCollision
