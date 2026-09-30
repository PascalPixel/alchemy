.syntax unified
	.thumb
	.global Func_080d1684
	.thumb_func
Func_080d1684:
	push {lr}
	movs r1, #168
	lsls r1, r1, #6
	adds r1, #4
	movs r0, #128
	sub sp, #4
	bl Runtime_AllocateBlock
	movs r3, #0
	adds r4, r0, #0
	mov r0, sp
	str r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r1, r4, #0
	ldr r2, .L_080d16ec
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #132
	movs r0, #160
	lsls r2, r2, #24
	lsls r0, r0, #19
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #224
	lsls r2, r2, #1
	adds r1, r4, r2
	movs r2, #132
	lsls r2, r2, #24
	ldr r0, .L_080d16f0
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #224
	lsls r3, r3, #4
	movs r0, #128
	adds r2, r4, r3
	adds r1, r4, #0
	movs r3, #0
	lsls r0, r0, #9
	bl Func_080d0e1c
	movs r1, #228
	lsls r1, r1, #2
	adds r1, #255
	ldr r0, .L_080d16f4
	bl Func_080145a8
	add sp, #4
	pop {pc}
.L_080d16ec:
	.4byte 0x85000a81
.L_080d16f0:
	.4byte 0x05000200
.L_080d16f4:
	.4byte Func_080d0ca0
