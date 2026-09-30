.syntax unified
	.thumb
	.global Func_080168f8
	.thumb_func
Func_080168f8:
	push {r5, lr}
	ldr r3, .L_08016920
	movs r5, #0
	b .L_08016910
.L_08016900:
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_08016924
	adds r5, #1
	cmp r5, r3
	bhi .L_0801691e
	ldr r3, .L_08016920
.L_08016910:
	ldr r3, [r3]
	cmp r3, #0
	bne .L_08016900
	ldr r3, .L_08016928
	ldr r3, [r3]
	cmp r3, #0
	bne .L_08016900
.L_0801691e:
	pop {r5, pc}
.L_08016920:
	.4byte Data_020038d0
.L_08016924:
	.4byte 0x000927bf
.L_08016928:
	.4byte Data_020055d0
