.syntax unified
	.thumb
	.global Func_0802e444
	.thumb_func
Func_0802e444:
	push {lr}
	cmp r1, #0
	beq .L_0802e4c0
	adds r3, r1, #0
	cmp r1, #0
	bge .L_0802e452
	negs r3, r1
.L_0802e452:
	cmp r3, #1
	bne .L_0802e46e
	adds r0, r0, r1
	cmp r0, #0
	bge .L_0802e462
	movs r0, #230
	lsls r0, r0, #1
	adds r0, #255
.L_0802e462:
	movs r3, #179
	lsls r3, r3, #2
	cmp r0, r3
	blt .L_0802e4c0
	movs r0, #0
	b .L_0802e4c0
.L_0802e46e:
	cmp r1, #0
	ble .L_0802e498
	ldr r1, .L_0802e4c4
	movs r2, #0
	ldrh r3, [r1, r2]
	b .L_0802e484
.L_0802e47a:
	adds r2, #1
	cmp r2, #9
	bhi .L_0802e4c0
	lsls r3, r2, #1
	ldrh r3, [r1, r3]
.L_0802e484:
	cmp r0, r3
	bge .L_0802e47a
	adds r0, r3, #0
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	cmp r0, r3
	bne .L_0802e4c0
	movs r0, #0
	b .L_0802e4c0
.L_0802e498:
	cmp r0, #0
	bne .L_0802e4a2
	ldr r3, .L_0802e4c4
	ldrh r0, [r3, #16]
	b .L_0802e4c0
.L_0802e4a2:
	ldr r1, .L_0802e4c4
	movs r2, #8
	ldrh r3, [r1, #16]
	cmp r0, r3
	ble .L_0802e4b0
	adds r0, r3, #0
	b .L_0802e4c0
.L_0802e4b0:
	subs r2, #1
	cmp r2, #0
	blt .L_0802e4c0
	lsls r3, r2, #1
	ldrh r3, [r1, r3]
	cmp r0, r3
	ble .L_0802e4b0
	adds r0, r3, #0
.L_0802e4c0:
	pop {pc}
	.2byte 0x0000
.L_0802e4c4:
	.4byte Data_0802f064
