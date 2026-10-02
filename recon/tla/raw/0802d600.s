.syntax unified
	.thumb
	.global Func_0802d600
	.thumb_func
Func_0802d600:
	push {r5, r6, lr}
	movs r4, #192
	movs r5, #3
	ands r5, r2
	lsls r4, r4, #18
	ldr r6, [r4, #32]
	lsls r4, r5, #3
	subs r4, r4, r5
	movs r5, #156
	lsls r5, r5, #1
	lsls r4, r4, #3
	adds r4, r4, r5
	ldr r5, [r6, r4]
	ldr r4, .L_0802d63c
	asrs r1, r1, #20
	adds r5, r5, r4
	ldr r4, .L_0802d640
	lsls r1, r1, #7
	asrs r5, r5, #2
	asrs r0, r0, #20
	adds r5, r5, r4
	adds r0, r0, r1
	adds r5, r5, r0
	movs r1, #0
	adds r0, r2, #0
	adds r2, r3, #0
	bl Func_080c8730
	strb r0, [r5]
	pop {r5, r6, pc}
.L_0802d63c:
	.4byte 0xfdff0000
.L_0802d640:
	.4byte gMapShapeGrid
