def foo(*args, sep=" "): # parameters after *args are keyword only
        print(*args, sep=sep) # unpack args as separate arguments
                

foo("num1", "num2", "num3", sep=" ")