# lists are mutable
odd_nums = [1,3,5]
even_nums = [0,2,4]

even_nums.append(6) # insert 6 at the tail
sliced = even_nums[:2] 

print(even_nums)
print(sliced)
print(even_nums + sliced) # concatenation


# assignment to a list is a reference not copy

even_nums2 = even_nums   # if you want a shallow copy not a reference use slice

print(id(even_nums2) == id(even_nums)) # same object

even_nums2.append(8) # side effect, also changes even_nums

print(even_nums)
print(even_nums2)


print("\n\n\nFibonacci series")
a, b = 0, 1
while a < 10:
    print(a)
    a, b = b, a+b