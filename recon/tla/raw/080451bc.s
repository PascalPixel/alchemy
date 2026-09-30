.syntax unified
	.thumb
	.global Func_080451bc
	.thumb_func
Func_080451bc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r2, .L_0804523c
	ldr r3, .L_08045240
	movs r7, #0
	mov r8, r2
	mov r10, r3
.L_080451ce:
	lsls r3, r7, #1
	ldr r2, .L_08045244
	adds r3, r3, r7
	lsls r3, r3, #7
	movs r6, #0
	adds r5, r3, r2
.L_080451da:
	adds r0, r5, #0
	movs r1, #64
	ldr r2, .L_08045248
	mov lr, r10
	.2byte 0xf800
	movs r4, #1
	adds r0, r5, #4
.L_080451e8:
	adds r1, r6, #0
	cmp r7, #1
	bne .L_080451f2
	cmp r4, #1
	ble .L_0804521c
.L_080451f2:
	cmp r7, #0
	bne .L_08045204
	subs r3, r4, #2
	cmp r6, r3
	ble .L_08045204
	adds r1, r3, #0
	cmp r1, #0
	bge .L_08045204
	movs r1, #0
.L_08045204:
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
.L_0804521c:
	adds r4, #1
	adds r0, #4
	cmp r4, #7
	ble .L_080451e8
	adds r6, #1
	adds r5, #64
	cmp r6, #5
	ble .L_080451da
	adds r7, #1
	cmp r7, #1
	ble .L_080451ce
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0804523c:
	.4byte Data_0805f700
.L_08045240:
	.4byte IwramFillWords
.L_08045244:
	.4byte 0x06006280
.L_08045248:
	.4byte 0x44444444
