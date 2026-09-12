@ ResourceTable_GetLongestFreeBlockRun. Unresolved C allocation; no authorship claim.
.syntax unified
.thumb
.global ResourceTable_GetLongestFreeBlockRun
.thumb_func
ResourceTable_GetLongestFreeBlockRun:
    push {lr}
    movs r2, #128
    ldr r4, .Lblock_owners
    movs r1, #0
    movs r0, #0
    lsls r2, r2, #2
.Lscan_block:
    ldrb r3, [r4]
    adds r4, #1
    cmp r3, #255
    beq .Lfree_block
    movs r1, #0
    b .Lnext_block
.Lfree_block:
    adds r1, #1
    cmp r0, r1
    bge .Lnext_block
    adds r0, r1, #0
.Lnext_block:
    subs r2, #1
    cmp r2, #0
    bne .Lscan_block
    pop {r1}
    bx r1
.balign 4, 0
.Lblock_owners:
    .word 0x03001810
