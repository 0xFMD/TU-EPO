a, b = 0, 1
while a < 100:
    print(a, end=',')    # end replaces the default newline
    a, b = b, a + b
print()


x, y = 1, 2
x, y = y, x              # a swap needs no temp var
print(x, y)


print('a', 'b', 'c')
print('a', 'b', 'c', sep='-')
print('a', 'b', 'c', sep='', end='!\n')


i = 0
while i < 3:
    print(i, end=' ')
    i += 1
print()
