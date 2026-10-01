.syntax unified
	.thumb
	.global Func_0802d87c
	.thumb_func
Func_0802d87c:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r7, r0, #0
	movs r2, #10
	ldrsh r6, [r1, r2]
	movs r0, #2
	ldrsh r5, [r1, r0]
	ldr r1, [r3, #32]
	movs r0, #0
	ldr r3, [r3, #108]
	cmp r1, #0
	beq .L_0802d93c
	movs r4, #197
	lsls r4, r4, #1
	adds r3, r3, r4
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	beq .L_0802d93c
	adds r2, r7, #0
	adds r2, #34
	ldrb r3, [r2]
	cmp r3, #2
	bhi .L_0802d8ca
	adds r2, r3, #0
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r0, #156
	lsls r2, r3, #3
	lsls r0, r0, #1
	movs r4, #158
	adds r3, r2, r0
	lsls r4, r4, #1
	ldr r0, [r1, r3]
	adds r3, r2, r4
	ldr r1, [r1, r3]
	b .L_0802d8ce
.L_0802d8ca:
	ldr r0, .L_0802d940
	ldr r1, .L_0802d944
.L_0802d8ce:
	adds r3, r5, #0
	cmp r5, #0
	bge .L_0802d8d6
	adds r3, #15
.L_0802d8d6:
	asrs r2, r3, #4
	adds r3, r6, #0
	cmp r6, #0
	bge .L_0802d8e0
	adds r3, #15
.L_0802d8e0:
	asrs r3, r3, #4
	lsls r3, r3, #7
	adds r3, r2, r3
	lsls r2, r3, #2
	adds r0, r0, r2
	adds r1, r1, r3
	ldrb r3, [r0, #2]
	movs r0, #2
	cmp r3, #255
	beq .L_0802d93c
	ldrb r1, [r1]
	ldr r2, .L_0802d948
	lsls r1, r1, #2
	adds r0, r1, r2
	ldrb r0, [r0]
	movs r2, #15
	adds r3, r2, #0
	ands r3, r0
	lsls r3, r3, #2
	mov r12, r3
	ldr r4, .L_0802d94c
	ldr r3, .L_0802d950
	ands r5, r2
	ands r6, r2
	mov r2, r12
	adds r0, r1, r3
	ldr r3, [r4, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	mov lr, r3
	.2byte 0xf800
	ldr r3, [r7, #20]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	ble .L_0802d92e
	movs r0, #1
	b .L_0802d93c
.L_0802d92e:
	ldr r4, .L_0802d954
	cmp r0, r4
	bge .L_0802d93a
	movs r0, #1
	negs r0, r0
	b .L_0802d93c
.L_0802d93a:
	movs r0, #0
.L_0802d93c:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0802d940:
	.4byte gMapCellBuffer
.L_0802d944:
	.4byte Data_02024000
.L_0802d948:
	.4byte gMapCollision
.L_0802d94c:
	.4byte Map_TerrainHeightHandlers
.L_0802d950:
	.4byte Data_0202c001
.L_0802d954:
	.4byte 0xfff40000
