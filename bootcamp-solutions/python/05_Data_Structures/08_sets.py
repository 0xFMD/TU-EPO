nums = [1,2,3,4,5,5,3,2]

odd_nums = {i for i in nums if i % 2 != 0}


print(set(nums))
print(odd_nums)

print(3 in nums)


print(set(nums)-odd_nums)