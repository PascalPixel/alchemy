.syntax unified
	.thumb
	.global Func_0802d7b0
	.thumb_func
Func_0802d7b0:
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
	beq .L_0802d862
	movs r0, #197
	lsls r0, r0, #1
	adds r3, r3, r0
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r0, #0
	cmp r3, #3
	beq .L_0802d862
	cmp r2, #2
	bgt .L_0802d7f2
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r4, #156
	lsls r2, r3, #3
	lsls r4, r4, #1
	adds r3, r2, r4
	adds r4, #4
	ldr r0, [r1, r3]
	adds r3, r2, r4
	ldr r1, [r1, r3]
	b .L_0802d7f6
.L_0802d7f2:
	ldr r0, .L_0802d864
	ldr r1, .L_0802d868
.L_0802d7f6:
	adds r3, r5, #0
	cmp r5, #0
	bge .L_0802d7fe
	adds r3, #15
.L_0802d7fe:
	asrs r2, r3, #4
	adds r3, r6, #0
	cmp r6, #0
	bge .L_0802d808
	adds r3, #15
.L_0802d808:
	asrs r3, r3, #4
	lsls r3, r3, #7
	adds r3, r2, r3
	lsls r2, r3, #2
	adds r0, r0, r2
	adds r1, r1, r3
	ldrb r3, [r0, #2]
	movs r0, #2
	cmp r3, #255
	beq .L_0802d862
	ldrb r1, [r1]
	ldr r2, .L_0802d86c
	lsls r1, r1, #2
	adds r0, r1, r2
	ldrb r0, [r0]
	movs r2, #15
	adds r3, r2, #0
	ands r3, r0
	lsls r3, r3, #2
	mov r12, r3
	ldr r4, .L_0802d870
	ldr r3, .L_0802d874
	ands r5, r2
	ands r6, r2
	mov r2, r12
	adds r0, r1, r3
	ldr r3, [r4, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	mov lr, r3
	.2byte 0xf800
	movs r3, #128
	subs r0, r0, r7
	lsls r3, r3, #12
	cmp r0, r3
	ble .L_0802d854
	movs r0, #1
	b .L_0802d862
.L_0802d854:
	ldr r4, .L_0802d878
	cmp r0, r4
	bge .L_0802d860
	movs r0, #1
	negs r0, r0
	b .L_0802d862
.L_0802d860:
	movs r0, #0
.L_0802d862:
	pop {r5, r6, r7, pc}
.L_0802d864:
	.4byte gMapCellBuffer
.L_0802d868:
	.4byte Data_02024000
.L_0802d86c:
	.4byte Data_0202c000
.L_0802d870:
	.4byte Data_0802efc4
.L_0802d874:
	.4byte Data_0202c001
.L_0802d878:
	.4byte 0xfff40000
