.syntax unified
	.thumb
	.global Func_080cda84
	.thumb_func
Func_080cda84:
	push {r5, r6, r7, lr}
	adds r6, r1, #0
	adds r5, r2, #0
	bl Func_080cda58
	adds r3, r6, #0
	adds r3, #34
	ldrb r2, [r3]
	adds r7, r0, #0
	ldr r1, [r5, #8]
	ldr r0, [r5]
	ldr r3, [r6, #20]
	bl Func_08020310
	adds r2, r0, #0
	cmp r0, #0
	bge .L_080cdaa8
	negs r2, r0
.L_080cdaa8:
	movs r1, #4
	ldrsh r3, [r7, r1]
	lsls r3, r3, #16
	cmp r2, r3
	bgt .L_080cdaba
	cmp r0, #0
	bge .L_080cdabe
	negs r0, r0
	b .L_080cdabe
.L_080cdaba:
	movs r0, #1
	negs r0, r0
.L_080cdabe:
	pop {r5, r6, r7, pc}
