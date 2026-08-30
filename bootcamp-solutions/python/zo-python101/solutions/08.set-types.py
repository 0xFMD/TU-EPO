nums = {1, 2, 3, 4}
fixed_nums = frozenset({3, 4, 5, 6})


nums.add(5)
nums.remove(1)


print(nums | fixed_nums)

print(nums & fixed_nums)