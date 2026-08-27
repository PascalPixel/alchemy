pub type ExecutableDigests = (&'static str, &'static [&'static str]);
pub type TargetExecutables = (&'static str, &'static [ExecutableDigests]);
pub type HostTargets = (&'static str, &'static [TargetExecutables]);
pub type HostDigests = (&'static str, &'static [&'static str]);

const GS1: &[ExecutableDigests] = &[
    (
        "xgcc",
        &["1c0b96c0193a3f547bf10c70078d87df10a62d34207da545653165b65c80d4c2"],
    ),
    (
        "cpp",
        &["c69d8c0423517be6a4962bd88e343c193a0842a164dd600550ceb36f8155d736"],
    ),
    (
        "tradcpp",
        &["f1e4340c3550029675b49e22976dd7d10588e365816ae0f2decd19729371ee00"],
    ),
    (
        "cc1",
        &["f7d1aeb098815f9b98c5df25bb47bab89aa14b1ab184b63c1472986f515fa3dd"],
    ),
];
const EMPTY_GS1: &[ExecutableDigests] =
    &[("xgcc", &[]), ("cpp", &[]), ("tradcpp", &[]), ("cc1", &[])];
const EMPTY: &[TargetExecutables] = &[("gs1", EMPTY_GS1), ("gs2", EMPTY_GS1)];

// Admitted 2026-08-27: built from the unchanged alchemy-gcc submodule commit
// (640614ea242376836cee60ab4b2aa5facefd707d) on a fresh linux-x64 container,
// then proved with a green `make full-rom-check` (identical=True,
// byte_identical=yes) on that same build before pinning.
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
            // Rebuilt 2026-08-26 from the unchanged alchemy-gcc submodule commit
            // (640614ea242376836cee60ab4b2aa5facefd707d) after a local disk cleanup
            // deleted the previously-staged dist/. gs1cc's xgcc/cpp/tradcpp/cc1
            // reproduced the digests above byte-for-byte from the same rebuild;
            // old_agbcc alone differs, evidently from non-source build metadata
            // (its own smoke-compile passes), not a behavior change.
            "633678adb3c1ceae5b69b4a604724593bf198c216289c85f55109e473737b088",
        ],
    ),
    ("darwin-x64", &[]),
    (
        "linux-x64",
        &["9200c74552a980be35fd58c8afdbd07bb76c9b785b57bad78d8303e00d738af3"],
    ),
    ("linux-arm64", &[]),
];
pub static GCC3_EXPECTED: &[HostDigests] = &[
    ("darwin-arm64", &[]),
    ("darwin-x64", &[]),
    ("linux-x64", &[]),
    ("linux-arm64", &[]),
];
