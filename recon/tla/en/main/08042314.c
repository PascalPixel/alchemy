/* Draft: RenderOutput_Create, complete native EN extent 136 bytes.
   2026-10-03: the inherited TBS-shaped draft did not compile because
   Ram_VramBlockCache is not a TLA declaration. Native TLA uses physical
   ResourceTableEntries, signed input coordinates and sentinel 254.
   OUTPUT.H and VRAM_TAB.H now own the callable and cache-record contracts;
   explicit s16 coordinate views preserve the shared RenderInput storage.

   Three bounded ordinary forms: maintained owners and direct void append
   scored 340/5; grouping x/y stores before next=NULL scored 120/2; a named
   list view also scored 120/2. The simpler second form is retained.
   After those trials, the existing TBS ignored-scalar call-site device
   scored 60/1, fixing the list-argument move but leaving the zero move.
   It is not retained. A separately authorized fourth ordinary form put
   x before next=NULL and y after: 255/17 (14 register-only, 3 reordered).
   That changed coordinate register allocation and was worse; the second
   ordinary form remains. This draft has no new matching device. Remaining
   differences are the positions of movs r0,#0 and mov r0,r8. No complete
   exact extent, other-edition match or adoption is claimed. */
#include "RENDER_INPUT.H"
#include "OUTPUT.H"
#include "VRAM_TAB.H"
#include "RESOURCE.H"

struct RenderOutput *RenderOutput_Create(
    s32 slot,
    s32 flags,
    struct RenderInput *input,
    s32 offset_x,
    s32 offset_y)
{
    s32 x;
    struct RenderOutput *output;
    s32 y;

    output = RenderOutput_AcquireFree();
    if (output == NULL) {
        Resource_ResetEntry((u32)slot);
        return NULL;
    }
    x = offset_x + (s16)input->x * 8 + 8;
    y = offset_y + (s16)input->y * 8 + 8;
    x &= 0x1ff;
    y &= 0xff;
    output->packed = (x << 16) | y | flags;
    output->table.value = ResourceTableEntries[slot].offset >> 5;
    output->sentinel = 254;
    output->x = x;
    output->y = (s16)y;
    output->next = NULL;
    output->index = (s8)slot;
    output->kind = 1;
    output->active = 1;
    RenderOutput_AppendToList((struct RenderOutputList *)input, output);
    return output;
}
