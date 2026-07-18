#!/usr/bin/env python3
"""Single entry point for the native, byte-verified compiler bundle."""
import hashlib
import platform
import subprocess
import threading
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
BUNDLE = ROOT / "alchemy-gcc"
DRIVER = BUNDLE / "xgcc"
M2C = BUNDLE / "m2c-venv/bin/m2c"
DECOMP_PERMUTER = BUNDLE / "decomp-permuter"
CFLAGS = (
    "-O2", "-mthumb", "-mthumb-interwork", "-mcpu=arm7tdmi",
    "-fno-builtin", "-nostdinc", "-ffreestanding", "-fcall-used-r4",
)

EXPECTED = {
    "xgcc": "8087fb1911b00aafe8ba9dc1530ca84a98774206f24d95b3ac8a8f01bf8a6eb6",
    "cpp": "28621e18b2a6b663e1ea6e47750ca0133483f4287bc271265cc7e2fcfa69a2eb",
    "tradcpp": "88dae1204f5e928c7de003fd25263e91a18802f8ffde48b6f076e2ee1ea3e59a",
    "cc1": "14d4ad0a4fba9054250af66efd489d577d6f0397b54aeb75f9bcead65256fc5c",
}
_validated = False
_validation_lock = threading.Lock()


def validate_bundle():
    """錬金炉確認。Reject incomplete, foreign, or changed compiler bundles."""
    global _validated
    if _validated:
        return
    with _validation_lock:
        if _validated:
            return
        if platform.system() != "Darwin" or platform.machine() != "arm64":
            raise RuntimeError("alchemy-gcc requires native arm64 macOS")
        for name, expected in EXPECTED.items():
            path = BUNDLE / name
            if not path.is_file() or not path.stat().st_mode & 0o111:
                raise RuntimeError(f"alchemy-gcc is missing executable {name}")
            actual = hashlib.sha256(path.read_bytes()).hexdigest()
            if actual != expected:
                raise RuntimeError(f"alchemy-gcc/{name} has an unapproved digest")
        # 起動確認。Warm the relocated helpers serially before parallel builds;
        # otherwise macOS may gate several first launches at the same instant.
        smoke = subprocess.run(
            [str(DRIVER), f"-B{BUNDLE}/", "-S", "-x", "c",
             "-o", "/dev/null", "/dev/null"],
            capture_output=True, text=True)
        if smoke.returncode:
            detail = (smoke.stderr or smoke.stdout).strip()
            raise RuntimeError(f"alchemy-gcc smoke compile failed: {detail}")
        _validated = True


def compiler_command(*arguments):
    """Return the approved driver invocation with its local helper prefix."""
    validate_bundle()
    return [str(DRIVER), f"-B{BUNDLE}/", *map(str, arguments)]


def main():
    validate_bundle()
    size = sum((BUNDLE / name).stat().st_size for name in EXPECTED)
    print(f"alchemy-gcc=ok host=arm64 files={len(EXPECTED)} bytes={size}")


if __name__ == "__main__":
    main()
