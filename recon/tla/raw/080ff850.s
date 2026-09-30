.syntax unified
	.thumb
	.global Func_080ff850
	.thumb_func
Func_080ff850:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r2, .L_080ff8d0
	ldr r3, .L_080ff8d4
	movs r7, #0
	mov r8, r2
	mov r10, r3
.L_080ff862:
	lsls r3, r7, #1
	ldr r2, .L_080ff8d8
	adds r3, r3, r7
	lsls r3, r3, #7
	movs r6, #0
	adds r5, r3, r2
.L_080ff86e:
	adds r0, r5, #0
	movs r1, #64
	ldr r2, .L_080ff8dc
	mov lr, r10
	.2byte 0xf800
	movs r4, #1
	adds r0, r5, #4
.L_080ff87c:
	adds r1, r6, #0
	cmp r7, #1
	bne .L_080ff886
	cmp r4, #1
	ble .L_080ff8b0
.L_080ff886:
	cmp r7, #0
	bne .L_080ff898
	subs r3, r4, #2
	cmp r6, r3
	ble .L_080ff898
	adds r1, r3, #0
	cmp r1, #0
	bge .L_080ff898
	movs r1, #0
.L_080ff898:
	lsls r1, r1, #3
	mov r3, r8
	ldr r3, [r3, r1]
	ldr r2, [r0]
	adds r1, #4
	eors r2, r3
	str r2, [r0]
	mov r3, r8
	ldr r2, [r0, #32]
	ldr r1, [r3, r1]
	eors r2, r1
	str r2, [r0, #32]
.L_080ff8b0:
	adds r4, #1
	adds r0, #4
	cmp r4, #7
	ble .L_080ff87c
	adds r6, #1
	adds r5, #64
	cmp r6, #5
	ble .L_080ff86e
	adds r7, #1
	cmp r7, #1
	ble .L_080ff862
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080ff8d0:
	.4byte Data_08105984
.L_080ff8d4:
	.4byte IwramFillWords
.L_080ff8d8:
	.4byte 0x06006000
.L_080ff8dc:
	.4byte 0x44444444
