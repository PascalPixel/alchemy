.syntax unified
	.thumb
	.global Func_081088d8
	.thumb_func
Func_081088d8:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	adds r3, #128
	ldr r5, [r3]
	movs r2, #224
	lsls r2, r2, #4
	adds r4, r5, r2
	movs r2, #128
	movs r3, #128
	lsls r2, r2, #2
	adds r2, #34
	lsls r3, r3, #19
	adds r6, r0, #0
	adds r1, r1, r2
	adds r3, #212
	adds r0, r4, #0
	ldr r2, .L_08108920
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #224
	lsls r2, r2, #2
	adds r1, r5, r2
	adds r0, r4, #0
	ldr r2, .L_08108924
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r6, #0
	movs r1, #1
	bl Func_080c8378
	movs r0, #16
	bl Func_080c8390
	pop {r5, r6, pc}
.L_08108920:
	.4byte 0x84000150
.L_08108924:
	.4byte 0x840002a0
