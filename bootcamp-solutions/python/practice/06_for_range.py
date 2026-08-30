memory = [10, 20, 0, 30, -1, 40]


for i in range(len(memory)):
        if memory[i] == 0:
                continue
        if memory[i] == -1:
                break
        
        print(memory[i])
else: # will print if there isn't -1
        print("Completed")