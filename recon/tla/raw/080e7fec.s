.syntax unified
	.thumb
	.global Func_080e7fec
	.thumb_func
Func_080e7fec:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r2, .L_080e8070
	adds r7, r0, #0
	adds r5, r1, #0
	mov r8, r2
	adds r3, r7, #0
	cmp r7, #0
	bge .L_080e8002
	adds r3, r7, #7
.L_080e8002:
	asrs r7, r3, #3
	adds r3, r5, #0
	cmp r5, #0
	bge .L_080e800c
	adds r3, r5, #7
.L_080e800c:
	asrs r5, r3, #3
	ldr r3, .L_080e8074
	movs r1, #128
	lsls r1, r1, #3
	movs r2, #0
	ldr r0, .L_080e8078
	mov lr, r3
	.2byte 0xf800
	movs r6, #0
	adds r1, r5, #0
	cmp r1, #0
	blt .L_080e805a
	cmp r1, #19
	bgt .L_080e806a
.L_080e8028:
	adds r0, r7, #0
	movs r4, #0
	cmp r0, #0
	blt .L_080e804a
	cmp r0, #29
	bgt .L_080e805a
.L_080e8034:
	ldr r5, .L_080e8078
	lsls r3, r6, #5
	adds r3, r3, r4
	lsls r2, r6, #4
	lsls r3, r3, #1
	adds r2, r2, r4
	adds r3, r3, r5
	lsls r2, r2, #1
	mov r5, r8
	ldrh r2, [r2, r5]
	strh r2, [r3]
.L_080e804a:
	adds r4, #1
	adds r0, #1
	cmp r4, #15
	bgt .L_080e805a
	cmp r0, #0
	blt .L_080e804a
	cmp r0, #29
	ble .L_080e8034
.L_080e805a:
	adds r6, #1
	adds r1, #1
	cmp r6, #15
	bgt .L_080e806a
	cmp r1, #0
	blt .L_080e805a
	cmp r1, #19
	ble .L_080e8028
.L_080e806a:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_080e8070:
	.4byte Data_080f39c4
.L_080e8074:
	.4byte IwramFillWords
.L_080e8078:
	.4byte 0x06002000
