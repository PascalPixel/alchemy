.syntax unified
	.thumb
	.global BattleFx_DispatchRequestKind
	.thumb_func
BattleFx_DispatchRequestKind:
	push {r5, r6, lr}
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #224
	ldr r6, [r3]
	ldr r0, [r2, #108]
	movs r2, #30
	ldrsh r1, [r6, r2]
	adds r2, r6, #0
	movs r3, #26
	ldrsh r5, [r6, r3]
	adds r2, #32
	movs r3, #0
	strb r3, [r2]
	cmp r1, #1
	bne .L_080db69a
	adds r0, r5, #0
	bl Func_080dd054
	b .L_080db840
.L_080db69a:
	cmp r1, #7
	bne .L_080db6a6
	adds r0, r5, #0
	bl Func_080ddda0
	b .L_080db840
.L_080db6a6:
	cmp r1, #11
	bne .L_080db6b2
	adds r0, r5, #0
	bl Func_080df1fc
	b .L_080db840
.L_080db6b2:
	cmp r1, #4
	bne .L_080db6be
	adds r0, r5, #0
	bl Func_080de214
	b .L_080db840
.L_080db6be:
	cmp r1, #5
	bne .L_080db6ca
	adds r0, r5, #0
	bl Func_080dede0
	b .L_080db840
.L_080db6ca:
	cmp r1, #6
	bne .L_080db6d6
	adds r0, r5, #0
	bl Func_080de654
	b .L_080db840
.L_080db6d6:
	cmp r1, #12
	bne .L_080db6e2
	adds r0, r5, #0
	bl Func_080dda30
	b .L_080db840
.L_080db6e2:
	cmp r1, #3
	bne .L_080db6ee
	adds r0, r5, #0
	bl Func_080de9ec
	b .L_080db840
.L_080db6ee:
	cmp r1, #14
	bne .L_080db6fa
	adds r0, r5, #0
	bl Func_080dfcf8
	b .L_080db840
.L_080db6fa:
	cmp r1, #13
	bne .L_080db706
	adds r0, r5, #0
	bl Func_080e03c4
	b .L_080db840
.L_080db706:
	cmp r1, #16
	bne .L_080db710
	bl Func_080e1214
	b .L_080db840
.L_080db710:
	cmp r1, #17
	bne .L_080db71c
	adds r0, r5, #0
	bl Func_080e0978
	b .L_080db840
.L_080db71c:
	cmp r1, #10
	bne .L_080db726
	bl Func_080decb8
	b .L_080db840
.L_080db726:
	cmp r1, #15
	bne .L_080db730
	bl Func_080e0dd4
	b .L_080db840
.L_080db730:
	cmp r1, #8
	bne .L_080db73a
	bl Func_080dd820
	b .L_080db840
.L_080db73a:
	cmp r1, #23
	bne .L_080db746
	adds r0, r5, #0
	bl Func_080e4a2c
	b .L_080db840
.L_080db746:
	cmp r1, #26
	bne .L_080db752
	adds r0, r5, #0
	bl Func_080e3cfc
	b .L_080db840
.L_080db752:
	cmp r1, #25
	bne .L_080db75e
	adds r0, r5, #0
	bl Func_080e306c
	b .L_080db840
.L_080db75e:
	cmp r1, #20
	bne .L_080db76a
	adds r0, r5, #0
	bl Func_080e68c8
	b .L_080db840
.L_080db76a:
	cmp r1, #19
	bne .L_080db776
	adds r0, r5, #0
	bl Func_080e8db0
	b .L_080db840
.L_080db776:
	cmp r1, #27
	bne .L_080db782
	adds r0, r5, #0
	bl Func_080e92b4
	b .L_080db840
.L_080db782:
	cmp r1, #24
	bne .L_080db78e
	adds r0, r5, #0
	bl Func_080e9aec
	b .L_080db840
.L_080db78e:
	cmp r1, #30
	bne .L_080db79a
	adds r0, r5, #0
	bl Func_080e7818
	b .L_080db840
.L_080db79a:
	cmp r1, #22
	bne .L_080db7a6
	adds r0, r5, #0
	bl Func_080e783c
	b .L_080db840
.L_080db7a6:
	cmp r1, #21
	bne .L_080db7b2
	adds r0, r5, #0
	bl Func_080e5724
	b .L_080db840
.L_080db7b2:
	cmp r1, #18
	bne .L_080db7be
	adds r0, r5, #0
	bl Func_080e5d54
	b .L_080db840
.L_080db7be:
	cmp r1, #28
	bne .L_080db7c8
	bl Func_080e82cc
	b .L_080db840
.L_080db7c8:
	cmp r1, #2
	bne .L_080db80c
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r0, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080db7e2
	bl Func_080dd950
.L_080db7e2:
	ldr r3, .L_080db844
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #106
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	movs r1, #26
	ldrsh r3, [r6, r1]
	cmp r2, r3
	beq .L_080db800
	ldr r3, [r6, #20]
	movs r2, #1
	adds r3, #91
	strb r2, [r3]
.L_080db800:
	movs r2, #24
	ldrsh r0, [r6, r2]
	adds r1, r5, #0
	bl Func_080dc978
	b .L_080db840
.L_080db80c:
	cmp r1, #9
	bne .L_080db840
	ldr r3, .L_080db844
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #106
	adds r6, r3, r1
	movs r2, #0
	ldrsh r0, [r6, r2]
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_080db832
	bl Func_080e035c
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r6]
.L_080db832:
	adds r0, r5, #0
	bl BattleEffect_PauseObject
	strh r5, [r6]
	adds r0, r5, #0
	bl Func_080e011c
.L_080db840:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080db844:
	.4byte gPartyState
