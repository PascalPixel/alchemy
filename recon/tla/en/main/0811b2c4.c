#include "types.h"
#include "objdisp.h"
#include "ram_buffer.h"
#include "motobj.h"
#include "btlwork.h"
#include "btlsys.h"
#include "inventory.h"

s32 EnmUpd(void)
{
    struct BtlWork *work = BtlWk;
    u16 unit_ids[14];
    s32 x[6];
    s32 z[6];
    s32 count;
    s32 i;

    for (i = 0; i < 6 && work->enm_no[i] != 0xff; i++)
        unit_ids[i] = work->enm_no[i];
    count = i;
    Summon_LayoutPositions(unit_ids, count, x, z);
    for (i = 0; i < count; i++) {
        s32 id = work->enm_no[i];

        if (id != 0xfe) {
            struct BtlObj *object = BtlObjGet(id);

            object->anchor_x = x[i] << 16;
            object->anchor_z = z[i] << 16;
        }
    }
}
