.syntax unified
	.thumb
	.global Func_08011f54
	.thumb_func
Func_08011f54:
	push {r5, r6, r7, lr}
	ldr r3, .L_08011fc4
	adds r5, r1, #0
	ldr r1, [r3]
	adds r6, r2, #0
	asrs r5, r5, #16
	asrs r6, r6, #16
	ldr r2, .L_08011fc8
	cmp r1, #0
	beq .L_08011f7a
	movs r2, #3
	ands r2, r0
	lsls r3, r2, #1
	adds r3, r3, r2
	movs r2, #152
	lsls r2, r2, #1
	lsls r3, r3, #4
	adds r3, r3, r2
	ldr r2, [r1, r3]
.L_08011f7a:
	adds r3, r5, #0
	cmp r5, #0
	bge .L_08011f82
	adds r3, #15
.L_08011f82:
	asrs r1, r3, #4
	adds r3, r6, #0
	cmp r6, #0
	bge .L_08011f8c
	adds r3, #15
.L_08011f8c:
	asrs r3, r3, #4
	lsls r3, r3, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r2, r2, r3
	ldrb r1, [r2, #3]
	ldr r3, .L_08011fcc
	lsls r1, r1, #2
	adds r0, r1, r3
	movs r2, #15
	ldrb r0, [r0]
	adds r3, r2, #0
	ldr r4, .L_08011fd0
	ands r3, r0
	ldr r7, .L_08011fd4
	ands r5, r2
	ands r6, r2
	lsls r3, r3, #2
	adds r0, r1, r7
	ldr r3, [r4, r3]
	adds r1, r5, #0
	adds r2, r6, #0
	bl _call_via_r3
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
.L_08011fc4:
	.4byte gMapWork
.L_08011fc8:
	.4byte gMapCellBuffer
.L_08011fcc:
	.4byte gMapCollision
.L_08011fd0:
	.4byte Func_080134fc
.L_08011fd4:
	.4byte Data_0202c001
