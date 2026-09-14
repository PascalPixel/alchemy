@ Party_RemoveActiveOwner. Unresolved C allocation; no authorship claim.
.syntax unified
.thumb
.global Party_RemoveActiveOwner
.thumb_func
Party_RemoveActiveOwner:
    push {r5, r6, lr}
    adds r5, r0, #0
    bl Func_080795fc
    adds r6, r0, #0
    adds r0, r5, #0
    bl Func_08079374
    movs r1, #0
    cmp r1, r6
    bge .Lremove_owner
    ldr r0, .Lparty_state
    movs r2, #252
    lsls r2, r2, #1
    ldrb r3, [r0, r2]
    cmp r3, r5
    beq .Lremove_owner
    adds r2, r0, r2
.Lfind_owner:
    adds r1, #1
    cmp r1, r6
    bge .Lremove_owner
    adds r2, #1
    ldrb r3, [r2]
    cmp r3, r5
    bne .Lfind_owner
.Lremove_owner:
    subs r0, r6, #1
    cmp r1, r0
    bge .Lcount_owners
    ldr r3, .Lparty_state
    movs r4, #252
    adds r3, r1, r3
    lsls r4, r4, #1
    adds r2, r3, r4
    subs r1, r0, r1
.Lshift_owners:
    ldrb r3, [r2, #1]
    subs r1, #1
    strb r3, [r2]
    adds r2, #1
    cmp r1, #0
    bne .Lshift_owners
.Lcount_owners:
    bl Func_080795fc
    pop {r5, r6}
    pop {r1}
    bx r1
.balign 4, 0
.Lparty_state:
    .word 0x02000240
