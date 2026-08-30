with open("test.txt", "w") as f:
        f.writelines(["Hello1\n", "Hello2\n", "Hello3\n"])


with open("test.txt") as f:
        print(repr(f.read()))
        print(repr(f.read()))


with open("test.txt") as f:
        print(repr(f.readline()))
        print(repr(f.readline()))
        print(repr(f.readlines()))



with open("test.txt") as f:
        for line in f:
                print(line.strip())
