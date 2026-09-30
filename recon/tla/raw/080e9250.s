.syntax unified
	.thumb
	.global Func_080e9250
	.thumb_func
Func_080e9250:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r4, r1, #0
	subs r3, r2, #2
	muls r3, r4
	subs r2, #1
	adds r7, r0, r3
	adds r3, r4, #0
	muls r3, r2
	mov r12, r0
	add r3, r12
	adds r5, r2, #0
	mov r8, r3
	cmp r5, #1
	beq .L_080e929e
	movs r1, #132
	lsls r1, r1, #24
	mov lr, r1
.L_080e9276:
	adds r2, r4, #0
	cmp r4, #0
	bge .L_080e927e
	adds r2, r4, #3
.L_080e927e:
	movs r3, #128
	mov r6, lr
	asrs r2, r2, #2
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r7, #0
	mov r1, r8
	orrs r2, r6
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	subs r1, r1, r4
	subs r5, #1
	subs r7, r7, r4
	mov r8, r1
	cmp r5, #1
	bne .L_080e9276
.L_080e929e:
	ldr r3, .L_080e92b0
	mov r0, r12
	adds r1, r4, #0
	movs r2, #0
	mov lr, r3
	.2byte 0xf800
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_080e92b0:
	.4byte IwramFillWords
