# list is mutable
nums = [1, 2, 3]
nums2 = [1, 2, 3]

copy = nums
copy2 = nums2[:]

copy[0] = 4
copy2[0] = 4

print(nums)
print(copy)
print(f"nums id: {id(nums)}")
print(f"copy id: {id(copy)}")
print(f"is same id?: {id(copy) == id(nums)}")
print("\n=======nums2 & copy2=======\n")
print(nums2)
print(copy2)
print(f"nums2 id: {id(nums2)}")
print(f"copy2 id: {id(copy2)}")
print(f"is same id?: {id(copy2) == id(nums2)}")
