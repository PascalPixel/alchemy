#include "types.h"

/*
 * resource_385 owner at 0x02000400, 152 bytes: choose two nearby targets for
 * a subject using the distance-and-facing test at 0x02000350.
 *
 * Complete owner: the extended-register prologue at 0x02000400 through the
 * interworking return at 0x02000480-0x0200048f, followed by two referenced
 * pool words through 0x02000497.  The next owner is the leaf at 0x02000498.
 *
 * Subject flag bit zero chooses actor 15 rather than actor 14 for the first
 * distance/facing test, always with distance 32 and no override.  If that test
 * accepts, this owner returns immediately.  Otherwise actor zero is tested.
 * The second distance is normally 18; a signed scene halfword at workspace
 * +376 or byte +0xea4 in the engine block raises it to 26, and subject flag
 * bit one supplies the facing override in that case.
 *
 * All four static calls resolve independently and remain in machine order.
 */

struct Subject_02000400 {
    u8 reserved00[100];
    u16 flags;
};

struct EngineBlock_02000400 {
    u8 reserved0000[0xea4];
    u8 extended_range;
};

struct Engine_02000400 {
    struct EngineBlock_02000400 *block;
    u8 reserved04[44];
    u8 *scene;
};

extern struct Engine_02000400 *Data_03001e8c;
extern struct Subject_02000400 *Func_0808a080(s32 actor);
extern s32 Func_02000350(struct Subject_02000400 *subject,
    struct Subject_02000400 *candidate, s32 distance_limit, s32 override);

s32 Func_02000400(struct Subject_02000400 *subject)
{
    struct Subject_02000400 *candidate;
    s32 distance_limit;
    s32 override;

    candidate = Func_0808a080((subject->flags & 1) != 0 ? 15 : 14);
    if (Func_02000350(subject, candidate, 32, 0) != 0) {
        return 0;
    }

    candidate = Func_0808a080(0);
    distance_limit = 18;
    override = 0;
    if (*(s16 *)(Data_03001e8c->scene + 376) != 0 ||
        Data_03001e8c->block->extended_range != 0) {
        distance_limit = 26;
        override = (subject->flags & 2) != 0;
    }
    Func_02000350(subject, candidate, distance_limit, override);
    return 0;
}
