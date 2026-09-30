.syntax unified
	.thumb
	.global Func_08045330
	.thumb_func
Func_08045330:
	push {r5, r6, r7, lr}
	ldr r3, .L_0804536c
	adds r7, r0, #0
	ldr r3, [r3]
	lsrs r6, r3, #2
	movs r3, #3
	ands r6, r3
	cmp r6, #2
	ble .L_08045344
	movs r6, #2
.L_08045344:
	cmp r6, #0
	bgt .L_0804534a
	movs r6, #1
.L_0804534a:
	ldr r5, .L_08045370
	adds r6, #1
	negs r3, r6
	adds r1, r5, #0
	adds r2, r7, #0
	ldr r0, .L_08045374
	bl Func_080452bc
	adds r5, #32
	adds r2, r7, #0
	ldr r0, .L_08045378
	adds r2, #32
	adds r1, r5, #0
	adds r3, r6, #0
	bl Func_080452bc
	pop {r5, r6, r7, pc}
.L_0804536c:
	.4byte Data_0300122c
.L_08045370:
	.4byte Data_0805f730
.L_08045374:
	.4byte 0x06000220
.L_08045378:
	.4byte 0x06000240
