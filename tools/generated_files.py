#!/usr/bin/env python3
"""Small, bounded helpers for replacing sets of generated files."""
import subprocess
from pathlib import Path


IMAGE_SUFFIXES = (".png", ".jpg", ".jpeg", ".gif", ".bmp", ".webp")


def prune_files(directory, pattern, keep):
    """Remove stale generated files matching pattern and return their paths.

    Callers supply a narrowly scoped directory and filename pattern.  Pruning
    happens only after they have generated and verified the complete new set.
    """
    directory = Path(directory)
    expected = {Path(name).name for name in keep}
    removed = []
    if not directory.exists():
        return removed
    for path in sorted(directory.glob(pattern)):
        if path.is_file() and path.name not in expected:
            path.unlink()
            removed.append(path)
    return removed


def unused_tracked_images(root, regions):
    """Return existing tracked images absent from a built asset manifest."""
    root = Path(root).resolve()
    if not (root / ".git").exists():
        return []
    result = subprocess.run(
        ["git", "ls-files", "-z", "--", *[f"*{suffix}"
                                             for suffix in IMAGE_SUFFIXES]],
        cwd=root, check=True, stdout=subprocess.PIPE)
    tracked = {
        name.decode().replace("\\", "/")
        for name in result.stdout.split(b"\0") if name
    }
    sources = set()
    for region in regions:
        for name in region.get("sources", []):
            path = Path(name)
            if path.suffix.lower() not in IMAGE_SUFFIXES:
                continue
            if path.is_absolute():
                path = path.resolve().relative_to(root)
            sources.add(path.as_posix())
    return sorted(
        name for name in tracked - sources if (root / name).is_file())
