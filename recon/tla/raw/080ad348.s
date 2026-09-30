.syntax unified
	.thumb
	.global Trade_GetOfferState
	.thumb_func
Trade_GetOfferState:
	push {lr}
	cmp r0, #0
	beq .L_080ad356
	movs r0, #131
	bl Owner_GetState
	b .L_080ad358
.L_080ad356:
	ldr r0, .L_080ad35c
.L_080ad358:
	pop {pc}
	.2byte 0x0000
.L_080ad35c:
	.4byte Data_0200024c
