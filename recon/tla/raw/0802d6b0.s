.syntax unified
	.thumb
	.global Func_0802d6b0
	.thumb_func
Func_0802d6b0:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r5, r0, #0
	ldr r0, [r3, #32]
	adds r4, r2, #0
	asrs r1, r1, #20
	asrs r4, r4, #20
	ldr r2, .L_0802d6e4
	cmp r0, #0
	beq .L_0802d6d8
	movs r2, #3
	ands r2, r5
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r2, [r0, r3]
.L_0802d6d8:
	lsls r3, r4, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r2, r2, r3
	ldrb r0, [r2, #2]
	pop {r5, pc}
.L_0802d6e4:
	.4byte gMapCellBuffer
