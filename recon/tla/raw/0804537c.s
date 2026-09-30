.syntax unified
	.thumb
	.global Func_0804537c
	.thumb_func
Func_0804537c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_080453c4
	adds r6, r0, #0
	ldr r3, [r3]
	lsrs r7, r3, #2
	movs r3, #3
	ands r7, r3
	cmp r7, #2
	ble .L_08045394
	movs r7, #2
.L_08045394:
	cmp r7, #0
	bgt .L_0804539a
	movs r7, #1
.L_0804539a:
	ldr r3, .L_080453c8
	ldr r5, .L_080453cc
	mov r8, r3
	adds r7, #1
	negs r3, r7
	mov r0, r8
	adds r1, r5, #0
	adds r2, r6, #0
	bl Func_080452bc
	adds r5, #32
	adds r2, r6, #0
	adds r2, #32
	mov r0, r8
	adds r1, r5, #0
	adds r3, r7, #0
	bl Func_080452bc
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_080453c4:
	.4byte Data_0300122c
.L_080453c8:
	.4byte 0x06000400
.L_080453cc:
	.4byte Data_0805f730
