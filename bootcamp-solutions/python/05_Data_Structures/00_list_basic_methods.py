lst = []


lst.append(1) # lst[len(lst):] = [2]


lst.extend([2,3,4,4])

print(lst)
print(lst.count(4))