pub type ExecutableDigests = (&'static str, &'static [&'static str]);
pub type TargetExecutables = (&'static str, &'static [ExecutableDigests]);
pub type HostTargets = (&'static str, &'static [TargetExecutables]);
pub type HostDigests = (&'static str, &'static [&'static str]);

// Appended after a source-unchanged rebuild of bcfb458 on 2026-08-27; all four
// binaries came from one staging event and passed `make full-rom-check`.
const GS1: &[ExecutableDigests] = &[
    (
        "xgcc",
        &[
            "1c0b96c0193a3f547bf10c70078d87df10a62d34207da545653165b65c80d4c2",
            "bf6161b89ca5832dff2784d92d2a39c857123e26fa4654ecc8ee46d3937b0b3a",
        ],
    ),
    (
        "cpp",
        &[
            "c69d8c0423517be6a4962bd88e343c193a0842a164dd600550ceb36f8155d736",
            "05509d9a475f1cc8d7937ad245eaf00f4e00b4f8b659016a4c5e86bad71d5ef7",
        ],
    ),
    (
        "tradcpp",
        &[
            "f1e4340c3550029675b49e22976dd7d10588e365816ae0f2decd19729371ee00",
            "2bed3f31593d16ab3c762be9725090fb7fb4d090ec9581b022070ee66541b759",
        ],
    ),
    (
        "cc1",
        &[
            "f7d1aeb098815f9b98c5df25bb47bab89aa14b1ab184b63c1472986f515fa3dd",
            "4525e2477f4d903580155e8d79cbb965b5af89e0d9ac903287713736238d37eb",
            // Clean local rebuild of pinned alchemy-gcc bcfb458.
            "3f8a272981eba0b383b1b8d573feedbf6ba1715deb67213523c46b3ba35256b2",
        ],
    ),
];
const EMPTY_GS1: &[ExecutableDigests] =
    &[("xgcc", &[]), ("cpp", &[]), ("tradcpp", &[]), ("cc1", &[])];
const EMPTY: &[TargetExecutables] = &[("gs1", EMPTY_GS1), ("gs2", EMPTY_GS1)];

// Source-unchanged 640614e linux-x64 rebuild, admitted 2026-08-27 only after a
// byte-identical `make full-rom-check` on that build.
const LINUX_X64_GS1: &[ExecutableDigests] = &[
    (
        "xgcc",
        &["cc35e03215be6ec15aea1463c79e9598b754430c0927c2062ea337c46b725abc"],
    ),
    (
        "cpp",
        &["c01747023ae601e84c7739317d96bfae5ceab88de7271a83f7073fddeb9893aa"],
    ),
    (
        "tradcpp",
        &["cbf6602147a0b6cec2884d47ee56d44d1278a1fee31d2c413968c488c278787f"],
    ),
    (
        "cc1",
        &["d0c3f7857e568272f6d2a0d25f2138daeac2ee0941c4828db3abd10d9204b096"],
    ),
];

pub static EXPECTED: &[HostTargets] = &[
    ("darwin-arm64", &[("gs1", GS1), ("gs2", GS1)]),
    ("darwin-x64", EMPTY),
    (
        "linux-x64",
        &[("gs1", LINUX_X64_GS1), ("gs2", LINUX_X64_GS1)],
    ),
    ("linux-arm64", EMPTY),
];
pub static AGBCC_EXPECTED: &[HostDigests] = &[
    (
        "darwin-arm64",
        &[
            "dfbbb5a8a0a9e14e7ed84a0a723eb6e68974836159a2c3e925b904058382e15a",
            // Source-unchanged 640614e rebuild after cleanup; agscc reproduced,
            // while old_agbcc's metadata-only variant passed its smoke compile.
            "633678adb3c1ceae5b69b4a604724593bf198c216289c85f55109e473737b088",
            // Source-unchanged bcfb458 rebuild; two stage runs reproduced it,
            // then `make full-rom-check` proved it before pinning.
            "0c4cd3bbbb475245ec9ab5c9bada68f99ff4f2f6dec751c5c287731e9dbeaea8",
            // Source-unchanged bcfb458 rebuild after an accidental make clean
            // wiped dist; agscc reproduced its approved digest exactly, this
            // one reproduced across two stage runs, then `make full-rom-check`
            // proved it before pinning.
            "52f71341c3a221baef0d8777cd833da9fcc24dfd1203b9fc861ec5b37eadcf9c",
        ],
    ),
    ("darwin-x64", &[]),
    (
        "linux-x64",
        &["9200c74552a980be35fd58c8afdbd07bb76c9b785b57bad78d8303e00d738af3"],
    ),
    ("linux-arm64", &[]),
];
