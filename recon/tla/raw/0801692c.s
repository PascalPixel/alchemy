.syntax unified
	.thumb
	.global Func_0801692c
	.thumb_func
Func_0801692c:
	push {lr}
	ldr r3, .L_08016948
	movs r0, #0
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0801693a
	movs r0, #1
.L_0801693a:
	ldr r3, .L_0801694c
	ldr r3, [r3]
	cmp r3, #0
	beq .L_08016946
	movs r3, #2
	orrs r0, r3
.L_08016946:
	pop {pc}
.L_08016948:
	.4byte Data_020038d0
.L_0801694c:
	.4byte Data_020055d0
