.syntax unified
	.thumb
	.global Func_080405dc
	.thumb_func
Func_080405dc:
	push {lr}
	movs r1, #197
	lsls r1, r1, #3
	movs r0, #208
	sub sp, #4
	bl Runtime_AllocateBlock
	movs r3, #0
	adds r1, r0, #0
	mov r0, sp
	str r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r2, .L_0804060c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_08040610
	bl Func_080145a8
	add sp, #4
	pop {pc}
.L_0804060c:
	.4byte 0x8500018a
.L_08040610:
	.4byte Func_080405ac
