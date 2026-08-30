token = "NUMBER"

match token:
    case "NUMBER":
        print("Number")
        
    case "IDENTIFIER":
        print("Variable")
    case _:
        print("Undefined token")
        
        
vec = (10,3)
match vec:
    case (0, 0):
        print("Origin")
    case (0, y):
        print(f"Y={y}")
    case (x, 0):
        print(f"X={x}")
    case (x, y):
        print(f"X={x}, Y={y}")
    case _:
        raise ValueError("Not a point")