files = ["main.c", "app.py", "index.js", "Makefile"]

print(sorted(files))
print(sorted(files, key=str.lower))
print(sorted(files, key=len))
print(sorted(files, reverse=True))


# sort by extension, then by name
print(sorted(files, key=lambda f: (f.split(".")[-1], f)))


processes = [("python", 34.5), ("node", 120.1), ("vim", 8.2)]

print(sorted(processes, key=lambda p: p[1], reverse=True))
print(max(processes, key=lambda p: p[1]))


print(files.sort())
print(files)


print(sorted(["bb", "aa", "cc", "a"], key=len))
