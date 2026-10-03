.syntax unified
	.thumb
	.global Func_080de550
	.thumb_func
Func_080de550:
	push {lr}
	ldr r3, .L_080de574
	ldr r2, [r3]
	movs r3, #7
	ands r2, r3
	cmp r2, #0
	bne .L_080de566
	movs r1, #2
	bl Animation_ApplyChildValuesFar
	b .L_080de570
.L_080de566:
	cmp r2, #2
	bne .L_080de570
	movs r1, #0
	bl Animation_ApplyChildValuesFar
.L_080de570:
	movs r0, #0
	pop {pc}
.L_080de574:
	.4byte gFrameCount
