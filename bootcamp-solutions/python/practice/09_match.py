instruction = ("ADD", "R1", "R2")

match instruction:
        case ("ADD",dest,src):
                print(f"ADD, {dest}, {src}")
        case ("LOAD", register, value):
                print(f"LOAD, {register}, {value}")
        case ("JMP", address):
                print(f"JMP, {address}")
        case ("HALT",):
                print("HALT")
        case _:
                print("Error!")
                


point = (10, 5)

match point:
    case (x, y) if x > y:
        print("x > y")
    case (x, y) if x < y:
        print("x < y")
    case (x, y) if x == y:
        print("x == y")