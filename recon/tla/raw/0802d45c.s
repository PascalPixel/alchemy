.syntax unified
	.thumb
	.global Func_0802d45c
	.thumb_func
Func_0802d45c:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r5, r1, #0
	ldr r1, [r3, #32]
	adds r6, r2, #0
	asrs r5, r5, #16
	asrs r6, r6, #16
	cmp r1, #0
	beq .L_0802d484
	movs r2, #3
	ands r2, r0
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #158
	lsls r3, r3, #3
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r1, [r1, r3]
	b .L_0802d486
.L_0802d484:
	ldr r1, .L_0802d4c8
.L_0802d486:
	adds r3, r5, #0
	cmp r5, #0
	bge .L_0802d48e
	adds r3, #15
.L_0802d48e:
	asrs r2, r3, #4
	adds r3, r6, #0
	cmp r6, #0
	bge .L_0802d498
	adds r3, #15
.L_0802d498:
	asrs r3, r3, #4
	lsls r3, r3, #7
	adds r3, r2, r3
	ldrb r1, [r1, r3]
	ldr r3, .L_0802d4cc
	lsls r1, r1, #2
	adds r0, r1, r3
	ldrb r0, [r0]
	movs r2, #15
	ldr r4, .L_0802d4d0
	adds r3, r2, #0
	ldr r7, .L_0802d4d4
	ands r3, r0
	ands r5, r2
	ands r6, r2
	lsls r3, r3, #2
	adds r0, r1, r7
	ldr r3, [r4, r3]
	adds r1, r5, #0
	adds r2, r6, #0
	mov lr, r3
	.2byte 0xf800
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0802d4c8:
	.4byte gMapShapeGrid
.L_0802d4cc:
	.4byte gMapCollision
.L_0802d4d0:
	.4byte Map_TerrainHeightHandlers
.L_0802d4d4:
	.4byte Data_0202c001
