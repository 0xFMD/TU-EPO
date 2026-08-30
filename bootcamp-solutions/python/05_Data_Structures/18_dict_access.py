status = {200: "OK", 403: "Forbidden", 404: "Not Found"}

print(status[404])



print(status.get(500))
print(status.get(500, "Unknown"))


print(500 in status, 404 in status)
print("OK" in status.values())


status[500] = "Server Error"
print(status)

del status[403]
print(status)

print(status.pop(404))
print(status)


keys = status.keys()
print(keys)

status[301] = "Moved"
print(keys)

print(list(keys))


counts = {}
for t in ["NUMBER", "PLUS", "NUMBER"]:
        counts.setdefault(t, 0)
        counts[t] += 1
print(counts)
