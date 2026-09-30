.syntax unified
	.thumb
	.global Func_080162dc
	.thumb_func
Func_080162dc:
	push {lr}
	ldr r2, .L_080162ec
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_080162ea
	movs r3, #1
	strb r3, [r2, #8]
.L_080162ea:
	pop {pc}
.L_080162ec:
	.4byte Data_02005360
