.syntax unified
	.thumb
	.global Func_080add5c
	.thumb_func
Func_080add5c:
	push {r5, lr}
	cmp r0, #0
	ble .L_080add6e
	adds r5, r0, #0
.L_080add64:
	subs r5, #1
	bl Func_080adcec
	cmp r5, #0
	bne .L_080add64
.L_080add6e:
	bl Func_080adc90
	pop {r5, pc}
