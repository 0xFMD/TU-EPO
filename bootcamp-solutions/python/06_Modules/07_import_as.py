
import sys
from pathlib import Path

sys.path.append(str(Path(__file__).parent / "00_fibo"))

import fibo as fibonacci
from fibo import fib2 as fib_list

fibonacci.fib(20)
print()
print(fib_list(20))


# alias
import fibo
print(fibo is fibonacci)

# the module keeps its real name
print(fibonacci.__name__)


# a module is only executed once, imports reuse the cached object
print("fibo" in sys.modules)
print(sys.modules["fibo"] is fibonacci)
