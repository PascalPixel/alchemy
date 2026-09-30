.syntax unified
	.thumb
	.global Func_0802d958
	.thumb_func
Func_0802d958:
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
	beq .L_0802da0c
	movs r4, #197
	lsls r4, r4, #1
	adds r3, r3, r4
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	beq .L_0802da0c
	adds r2, r7, #0
	adds r2, #34
	ldrb r3, [r2]
	cmp r3, #2
	bhi .L_0802d9a6
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
	b .L_0802d9aa
.L_0802d9a6:
	ldr r0, .L_0802da10
	ldr r1, .L_0802da14
.L_0802d9aa:
	adds r3, r5, #0
	cmp r5, #0
	bge .L_0802d9b2
	adds r3, #15
.L_0802d9b2:
	asrs r2, r3, #4
	adds r3, r6, #0
	cmp r6, #0
	bge .L_0802d9bc
	adds r3, #15
.L_0802d9bc:
	asrs r3, r3, #4
	lsls r3, r3, #7
	adds r3, r2, r3
	lsls r2, r3, #2
	adds r0, r0, r2
	adds r1, r1, r3
	ldrb r3, [r0, #2]
	movs r0, #2
	cmp r3, #255
	beq .L_0802da0c
	ldrb r1, [r1]
	ldr r2, .L_0802da18
	lsls r1, r1, #2
	adds r0, r1, r2
	ldrb r0, [r0]
	movs r2, #15
	adds r3, r2, #0
	ands r3, r0
	lsls r3, r3, #2
	mov r12, r3
	ldr r4, .L_0802da1c
	ldr r3, .L_0802da20
	ands r5, r2
	ands r6, r2
	mov r2, r12
	adds r0, r1, r3
	ldr r3, [r4, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	mov lr, r3
	.2byte 0xf800
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	ble .L_0802da0a
	movs r0, #1
	b .L_0802da0c
.L_0802da0a:
	movs r0, #0
.L_0802da0c:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0802da10:
	.4byte gMapCellBuffer
.L_0802da14:
	.4byte Data_02024000
.L_0802da18:
	.4byte gMapCollision
.L_0802da1c:
	.4byte Data_0802efc4
.L_0802da20:
	.4byte Data_0202c001
