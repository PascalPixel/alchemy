.syntax unified
	.thumb
	.global Func_081a1294
	.thumb_func
Func_081a1294:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_081a132c
	mov r8, r2
	ldr r3, [r3]
	ldr r2, .L_081a1330
	lsrs r3, r3, #19
	lsls r3, r3, #6
	mov r9, r3
	ldr r3, .L_081a1334
	adds r7, r1, #0
	mov r1, r8
	strh r1, [r3]
	mov r3, r8
	strh r3, [r2]
	ldr r2, .L_081a1338
	mov r1, r8
	lsls r3, r1, #16
	str r3, [r2]
	movs r3, #128
	movs r2, #0
	lsls r3, r3, #5
	mov r12, r0
	mov r10, r2
	mov r11, r3
.L_081a12d0:
	mov r3, r8
	cmp r3, #0
	bge .L_081a12d8
	adds r3, #7
.L_081a12d8:
	movs r1, #224
	lsls r1, r1, #3
	adds r1, #255
	asrs r3, r3, #3
	mov r4, r9
	lsls r5, r3, #6
	ands r4, r1
	mov lr, r1
	movs r6, #30
.L_081a12ea:
	mov r2, r12
	adds r0, r2, r5
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r1, r7, r4
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r4, #64
	mov r3, lr
	subs r6, #1
	adds r5, #64
	ands r4, r3
	cmp r6, #0
	bge .L_081a12ea
	movs r2, #1
	movs r1, #128
	add r10, r2
	lsls r1, r1, #4
	mov r3, r10
	add r12, r11
	adds r7, r7, r1
	cmp r3, #14
	ble .L_081a12d0
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_081a132c:
	.4byte Data_02007518
.L_081a1330:
	.4byte gScrollTarget
.L_081a1334:
	.4byte Data_02007520
.L_081a1338:
	.4byte Data_0200751c
