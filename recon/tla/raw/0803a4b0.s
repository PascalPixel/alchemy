.syntax unified
	.thumb
	.global Func_0803a4b0
	.thumb_func
Func_0803a4b0:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r6, r0, #0
	ldr r5, [r3, #60]
	cmp r6, #0
	beq .L_0803a4d4
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #95
	bl VramBlock_LoadCached
	movs r2, #152
	lsls r2, r2, #5
	adds r2, #72
	adds r3, r5, r2
	strh r0, [r3]
.L_0803a4d4:
	movs r3, #154
	lsls r3, r3, #5
	adds r2, r5, r3
	movs r3, #9
	strh r3, [r2]
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #56
	adds r2, r5, r3
	movs r3, #10
	strh r3, [r2]
	movs r2, #240
	lsls r2, r2, #4
	adds r2, #60
	adds r3, r5, r2
	movs r1, #0
	strh r1, [r3]
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #62
	adds r2, r5, r3
	movs r3, #15
	strh r3, [r2]
	movs r2, #152
	lsls r2, r2, #5
	adds r2, #66
	adds r3, r5, r2
	strh r1, [r3]
	cmp r6, #0
	beq .L_0803a51c
	movs r1, #144
	ldr r0, .L_0803a528
	lsls r1, r1, #3
	bl Func_080145a8
	b .L_0803a526
.L_0803a51c:
	movs r1, #144
	ldr r0, .L_0803a52c
	lsls r1, r1, #3
	bl Func_080145a8
.L_0803a526:
	pop {r5, r6, pc}
.L_0803a528:
	.4byte Func_0803a8c8
.L_0803a52c:
	.4byte Func_0803a8d8
