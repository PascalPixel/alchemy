/* Copy the low two mode bits into the object's owner record. */
#include "types.h"
#include "scene.h"
#include "abi/overlays/shared/copy_mode_to_owner.h"

struct Owner {
    u8 unk0[9];
    u8 unk9_0 : 2;
    u8 mode : 2;
    u8 unk9_4 : 4;
};

    owner->mode = mode;
}
