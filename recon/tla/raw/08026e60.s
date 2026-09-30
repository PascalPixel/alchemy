.syntax unified
	.thumb
	.global Func_08026e60
	.thumb_func
Func_08026e60:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #0
	mov r12, r3
	cmp r3, #0
	bne .L_08026e72
	b .L_08026f76
.L_08026e72:
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #172
	add r3, r12
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_08026eec
	ldr r1, .L_08026f78
	ldr r0, .L_08026f7c
	movs r2, #141
	lsls r2, r2, #2
	adds r3, r1, r2
	ldrh r2, [r3]
	ldr r3, [r0, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_08026ede
	movs r2, #140
	lsls r2, r2, #2
	adds r3, r1, r2
	ldrh r2, [r3]
	ldr r3, [r0, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_08026ede
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #54
	adds r3, r1, r2
	ldrh r2, [r3]
	ldr r3, [r0, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_08026ede
	movs r2, #142
	lsls r2, r2, #2
	adds r3, r1, r2
	ldrh r2, [r3]
	ldr r3, [r0, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_08026ede
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #58
	adds r3, r1, r2
	ldrh r2, [r3]
	ldr r3, [r0, #4]
	movs r0, #0
	ands r3, r2
	cmp r3, #0
	beq .L_08026f76
.L_08026ede:
	movs r2, #179
	movs r3, #128
	lsls r2, r2, #1
	lsls r3, r3, #6
	add r2, r12
	adds r3, #155
	b .L_08026f32
.L_08026eec:
	ldr r1, .L_08026f78
	ldr r4, .L_08026f7c
	movs r2, #141
	lsls r2, r2, #2
	adds r3, r1, r2
	ldrh r2, [r3]
	ldr r3, [r4, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_08026f04
	movs r2, #173
	b .L_08026f2c
.L_08026f04:
	movs r2, #140
	lsls r2, r2, #2
	adds r3, r1, r2
	ldrh r2, [r3]
	ldr r3, [r4, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_08026f18
	movs r2, #174
	b .L_08026f2c
.L_08026f18:
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #54
	adds r3, r1, r2
	ldrh r2, [r3]
	ldr r3, [r4, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_08026f38
	movs r2, #175
.L_08026f2c:
	lsls r2, r2, #1
	add r2, r12
	movs r3, #1
.L_08026f32:
	strh r3, [r2]
	movs r0, #1
	b .L_08026f76
.L_08026f38:
	movs r2, #142
	lsls r2, r2, #2
	adds r3, r1, r2
	ldrh r2, [r3]
	ldr r3, [r4, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_08026f56
	movs r2, #144
	lsls r2, r2, #2
	adds r3, r1, r2
	ldrh r0, [r3]
	bl Func_08026dd0
	b .L_08026f76
.L_08026f56:
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #58
	adds r3, r1, r2
	ldrh r2, [r3]
	ldr r3, [r4, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_08026f76
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #66
	adds r3, r1, r2
	ldrh r0, [r3]
	bl Func_08026dd0
.L_08026f76:
	pop {pc}
.L_08026f78:
	.4byte gPartyState
.L_08026f7c:
	.4byte gInput
