tokens = ["NUMBER", "IDENTIFIER", "COMMENT"]

tokens.insert(1, "PLUS")
print(tokens)

tokens.insert(len(tokens), "EOF")
print(tokens)

tokens.remove("COMMENT")
print(tokens)

print(tokens.pop(0))
print(tokens)



tokens.remove("STRING")


copy = tokens.copy() # same as tokens[:], shallow copy
copy.append("EXTRA")
print(tokens, copy)



matrix = [[1, 2], [3, 4]]
m2 = matrix.copy()
m2[0].append(9)
print(matrix, m2)


tokens.clear()    # same as del tokens[:]
print(tokens)
