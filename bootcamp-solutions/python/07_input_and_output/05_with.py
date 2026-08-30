with open("test.txt", "w") as f:
        f.write("Hello, world\n")

print(f.closed)


with open("test.txt") as f:
        print(f.read())
