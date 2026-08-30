num = 10
text = "hello"
nums = [1, 2, 3]
user = {"name": "Ali"}

print(num + 5)
print(text + " world") 
print(nums + [4, 5])   


print(num * 3)
print(text * 3)
print(nums * 2)
print(len(text))
print(len(nums))
print(len(user))


print("e" in text)
print(2 in nums)
print("name" in user)



for char in text:
    print(char)



for num in nums:
        print(num)

for k,v in user.items():
        print(f"{k}: {v}")