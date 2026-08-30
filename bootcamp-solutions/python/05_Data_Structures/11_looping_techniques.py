

files = ["main.c", "app.py", "index.js","main.cpp"]
sizes = [500,1500,1000]

for f,s in zip(files,sizes): # stops when the shortest list ends
        print(f"file: {f}\nsize: {s}")
        
        


# reverse

for i in reversed(range(0,6)):
        print(i)
        
        
        
for file in sorted(files):
        print(file)
        


nums = [2,10,4,33,1,50,33,66,33,50,40,33]



for num in sorted(set(nums)):
        print(num)