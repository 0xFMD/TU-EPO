tokens = ["NUMBER", "PLUS", "COMMENT", "IDENTIFIER", "EOF", "NUMBER"]


for t in tokens:
        if t == "EOF":
                break
        print(t)


print("-----continue-----")

for t in tokens:
        if t == "COMMENT":
                continue
        print(t)


print("-----else-----")

for i, t in enumerate(tokens):
        if t == "STRING":
                print("found at", i)
                break
else:
        print("no STRING token")


for i, t in enumerate(tokens):
        if t == "IDENTIFIER":
                print("found at", i)
                break
else:
        print("no IDENTIFIER token")


print("-----nested-----")

matrix = [[1, 2], [3, 4]]

for row in matrix:
        for cell in row:
                if cell == 3:
                        break
                print(cell)
