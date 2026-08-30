
import sys
from pathlib import Path

sys.path.append(str(Path(__file__).parent / "00_fibo"))

import fibo    # this import is what writes the .pyc

print(fibo.__file__)        # the source python read
print(fibo.__cached__)      # the byte compiled copy it will reuse next time


cache = Path(fibo.__cached__).parent
print(cache.name, "->", [p.name for p in cache.iterdir()])


# the version tag in the filename lets several interpreters share one source tree
print(sys.implementation.cache_tag)

# Python compares the source's timestamp and size against the header of the .pyc,
# so an edited source is always recompiled - a stale .pyc is never used silently.
print(Path(fibo.__file__).stat().st_mtime)

# True when writing .pyc files is disabled (python -B, or PYTHONDONTWRITEBYTECODE)
print(sys.dont_write_bytecode)

# note: the module named on the command line is never cached, only imports are
print(__name__, __file__)
