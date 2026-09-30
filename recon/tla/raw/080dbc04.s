.syntax unified
	.thumb
	.global Func_080dbc04
	.thumb_func
Func_080dbc04:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r5, r0, #0
	mov r8, r3
	movs r3, #197
	lsls r3, r3, #1
	add r3, r8
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	adds r6, r1, #0
	adds r7, r2, #0
	cmp r3, #3
	bne .L_080dbc2e
	bl Func_080dbcd8
	b .L_080dbca2
.L_080dbc2e:
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r7, #0
	bl Func_080dbda8
	cmp r0, #0
	bne .L_080dbc58
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r7, #0
	bl Func_080dbd5c
	cmp r0, #0
	bne .L_080dbc58
	adds r0, r7, #0
	adds r1, r5, #0
	adds r2, r6, #0
	bl Func_080201c8 + 0x8
	cmp r0, #0
	beq .L_080dbc5c
.L_080dbc58:
	movs r0, #0
	b .L_080dbca2
.L_080dbc5c:
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r7, #0
	bl Func_080dbcd8
	cmp r0, #0
	beq .L_080dbc6e
	movs r0, #1
	b .L_080dbca2
.L_080dbc6e:
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #54
	add r3, r8
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_080dbc92
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r7, #0
	bl Func_080dbb78
	ldrb r3, [r0, #3]
	movs r0, #1
	ands r0, r3
	b .L_080dbca2
.L_080dbc92:
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r7, #0
	bl Func_080dbb78
	ldrb r3, [r0, #3]
	movs r0, #1
	bics r0, r3
.L_080dbca2:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
