combos = []

for i in range(1,4):
        for j in [3,1,4]:
                if i != j:
                        combos.append((i,j))
                        

print(combos)

print([(i,j) for i in range(1,4) for j in [3,1,4] if i!=j])



