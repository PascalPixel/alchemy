.syntax unified
	.thumb
	.global Func_080cda58
	.thumb_func
Func_080cda58:
	push {lr}
	adds r2, r0, #0
	ldr r0, .L_080cda80
	movs r1, #0
	ldrsh r3, [r0, r1]
	movs r1, #1
	negs r1, r1
	cmp r3, r1
	beq .L_080cda7e
	cmp r2, r3
	beq .L_080cda7e
	mov r12, r1
.L_080cda70:
	adds r0, #8
	movs r1, #0
	ldrsh r3, [r0, r1]
	cmp r3, r12
	beq .L_080cda7e
	cmp r2, r3
	bne .L_080cda70
.L_080cda7e:
	pop {pc}
.L_080cda80:
	.4byte Data_080eff28
