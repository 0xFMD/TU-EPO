fileName = "test.txt"
f = open(fileName, "w")
f.write("Hello, World - 1\n")
f.close()

f = open(fileName, "a")
f.write("Hello, World - 2\n")
f.close()


f = open(fileName, "r")
print(f.read())
print(f.closed)

f.close()
print(f.closed)


raw = open(fileName, "rb")
print(raw.read())
raw.close()
