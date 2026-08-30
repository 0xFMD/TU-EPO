letters = ['a', 'b', 'c', 'd', 'e', 'f']

letters[2:5] = ['C', 'D', 'E']  
print(letters)

letters[2:5] = ['X']
print(letters)

letters[2:3] = []                # remove a slice
print(letters)

letters[:] = []
print(letters, len(letters))


a = ['a', 'b', 'c']
n = [1, 2, 3]
x = [a, n]

print(x)
print(x[0])
print(x[0][1])
print(len(x))
