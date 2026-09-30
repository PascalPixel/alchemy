.syntax unified
	.thumb
	.global Func_080ceba8
	.thumb_func
Func_080ceba8:
	push {r5, r6, r7, lr}
	ldr r3, .L_080cec1c
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r6, [r3, r2]
	ldr r5, .L_080cec20
	movs r7, #0
	ldrh r0, [r5]
	b .L_080cebea
.L_080cebbe:
	movs r3, #0
	ldrsh r2, [r5, r3]
	ldrh r0, [r5]
	cmp r2, #0
	beq .L_080cebea
	movs r1, #128
	lsls r1, r1, #7
	adds r3, r1, #0
	ands r3, r2
	cmp r3, #0
	bne .L_080cebea
	mov r12, r1
.L_080cebd6:
	adds r5, #4
	movs r3, #0
	ldrsh r2, [r5, r3]
	ldrh r0, [r5]
	cmp r2, #0
	beq .L_080cebea
	mov r3, r12
	ands r3, r2
	cmp r3, #0
	beq .L_080cebd6
.L_080cebea:
	lsls r3, r0, #16
	adds r5, #2
	asrs r2, r3, #16
	movs r3, #0
	ldrsh r0, [r5, r3]
	adds r5, #2
	cmp r2, #0
	beq .L_080cec18
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #255
	ands r3, r2
	cmp r3, r6
	bne .L_080cebbe
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	beq .L_080cec16
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080cebbe
.L_080cec16:
	adds r7, r5, #0
.L_080cec18:
	adds r0, r7, #0
	pop {r5, r6, r7, pc}
.L_080cec1c:
	.4byte gPartyState
.L_080cec20:
	.4byte Data_080f2204
