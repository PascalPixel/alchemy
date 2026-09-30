.syntax unified
	.thumb
	.global Func_080120dc
	.thumb_func
Func_080120dc:
	push {r5, r6, r7, lr}
	movs r3, #10
	ldrsh r6, [r1, r3]
	ldr r3, .L_08012184
	movs r2, #2
	ldrsh r5, [r1, r2]
	ldr r1, [r3]
	adds r7, r0, #0
	movs r0, #0
	cmp r1, #0
	beq .L_0801217c
	adds r2, r7, #0
	adds r2, #34
	ldrb r3, [r2]
	cmp r3, #2
	bhi .L_0801210e
	adds r2, r3, #0
	lsls r3, r2, #1
	adds r3, r3, r2
	movs r2, #152
	lsls r2, r2, #1
	lsls r3, r3, #4
	adds r3, r3, r2
	ldr r2, [r1, r3]
	b .L_08012110
.L_0801210e:
	ldr r2, .L_08012188
.L_08012110:
	adds r3, r5, #0
	cmp r5, #0
	bge .L_08012118
	adds r3, #15
.L_08012118:
	asrs r1, r3, #4
	adds r3, r6, #0
	cmp r6, #0
	bge .L_08012122
	adds r3, #15
.L_08012122:
	asrs r3, r3, #4
	lsls r3, r3, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r2, r2, r3
	ldrb r3, [r2, #2]
	movs r0, #2
	cmp r3, #255
	beq .L_0801217c
	ldrb r1, [r2, #3]
	ldr r3, .L_0801218c
	lsls r1, r1, #2
	adds r0, r1, r3
	movs r2, #15
	ldrb r0, [r0]
	adds r3, r2, #0
	ands r3, r0
	lsls r3, r3, #2
	mov r12, r3
	ldr r4, .L_08012190
	ldr r3, .L_08012194
	ands r5, r2
	ands r6, r2
	mov r2, r12
	adds r0, r1, r3
	ldr r3, [r4, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	bl _call_via_r3
	ldr r3, [r7, #20]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	ble .L_0801216e
	movs r0, #1
	b .L_0801217c
.L_0801216e:
	ldr r2, .L_08012198
	cmp r0, r2
	bge .L_0801217a
	movs r0, #1
	negs r0, r0
	b .L_0801217c
.L_0801217a:
	movs r0, #0
.L_0801217c:
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
.L_08012184:
	.4byte gMapWork
.L_08012188:
	.4byte gMapCellBuffer
.L_0801218c:
	.4byte gMapCollision
.L_08012190:
	.4byte Func_080134fc
.L_08012194:
	.4byte Data_0202c001
.L_08012198:
	.4byte 0xfff40000
