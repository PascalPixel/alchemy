.syntax unified
	.thumb
	.global Func_080cded4
	.thumb_func
Func_080cded4:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #162
	adds r2, r5, r3
	movs r3, #0
	strh r3, [r2]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #164
	adds r3, r5, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	adds r6, r0, #0
	cmp r3, #0
	beq .L_080cdf0e
	movs r3, #1
	ands r3, r6
	cmp r3, #0
	bne .L_080cdf0e
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #144
	bl Func_080ce574
.L_080cdf0e:
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #172
	adds r3, r5, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080cdf32
	movs r3, #2
	ands r3, r6
	cmp r3, #0
	bne .L_080cdf32
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #155
	bl Func_080ce574
.L_080cdf32:
	ldr r3, .L_080cdf58
	movs r2, #155
	lsls r2, r2, #2
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080cdf56
	movs r3, #4
	ands r3, r6
	cmp r3, #0
	bne .L_080cdf56
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #139
	bl Func_080ce574
.L_080cdf56:
	pop {r5, r6, pc}
.L_080cdf58:
	.4byte gPartyState
