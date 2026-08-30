instructions = ["LOAD", "ADD", "NOP", "ADD", "HALT"]

print(f"Number of Add: {instructions.count("ADD")}")
instructions.append("STORE")
instructions.insert(1,"MOV")
instructions.remove("NOP")

print(instructions)

instructions.reverse()

print(instructions)