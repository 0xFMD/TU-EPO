tokens = ["NUMBER", "IDENTIFIER", "EOF"]

print("EOF" in tokens, "STRING" not in tokens)


a = ["R1", "R2"]
b = ["R1", "R2"]
print(a == b, a is b)
print(a == a[:], a is a[:])   # a slice is a copy, so a new object


name = ""
print(name or "anonymous")       
print("R0" and "R1")
print(not name)


def boom():
        raise AssertionError("never called")


print(False and boom())
print(True or boom())


reg = None
if reg is not None and reg.startswith("R"):
        print("register")
else:
        print("not a register")


addr = 0x40
print(0 <= addr < 0x100)  # same as 0 <= addr and addr < 0x100


# := binds inside the expression, so len() is not called twice
line = "ADD R1, R2"
if (n := len(line)) > 8:
        print(f"{line!r} is {n} chars")
