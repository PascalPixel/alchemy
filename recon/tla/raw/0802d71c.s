.syntax unified
	.thumb
	.global Func_0802d71c
	.thumb_func
Func_0802d71c:
	push {r5, r6, r7, lr}
	adds r7, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	asrs r6, r1, #16
	ldr r1, [r3, #32]
	asrs r5, r0, #16
	ldr r3, [r3, #108]
	movs r0, #0
	cmp r1, #0
	beq .L_0802d79e
	movs r0, #197
	lsls r0, r0, #1
	adds r3, r3, r0
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r0, #0
	cmp r3, #3
	beq .L_0802d79e
	cmp r2, #2
	bgt .L_0802d758
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #158
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r2, [r1, r3]
	b .L_0802d75a
.L_0802d758:
	ldr r2, .L_0802d7a0
.L_0802d75a:
	adds r3, r5, #0
	cmp r5, #0
	bge .L_0802d762
	adds r3, #15
.L_0802d762:
	asrs r1, r3, #4
	adds r3, r6, #0
	cmp r6, #0
	bge .L_0802d76c
	adds r3, #15
.L_0802d76c:
	asrs r3, r3, #4
	lsls r3, r3, #7
	adds r3, r1, r3
	ldrb r1, [r2, r3]
	ldr r3, .L_0802d7a4
	lsls r1, r1, #2
	adds r0, r1, r3
	ldrb r0, [r0]
	movs r2, #15
	adds r3, r2, #0
	ands r3, r0
	lsls r3, r3, #2
	mov r12, r3
	ldr r4, .L_0802d7a8
	ldr r3, .L_0802d7ac
	ands r5, r2
	ands r6, r2
	mov r2, r12
	adds r0, r1, r3
	ldr r3, [r4, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	mov lr, r3
	.2byte 0xf800
	subs r0, r0, r7
.L_0802d79e:
	pop {r5, r6, r7, pc}
.L_0802d7a0:
	.4byte gMapShapeGrid
.L_0802d7a4:
	.4byte gMapCollision
.L_0802d7a8:
	.4byte Map_TerrainHeightHandlers
.L_0802d7ac:
	.4byte Data_0202c001
