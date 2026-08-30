# default args


def foo(a,x=1,y=2):
        print(f"a={a}, x={x}, y={y}")
        


foo(10)



# mutable
def bar(num,numbers = []):
        numbers.append(num)
        return numbers


print(bar(1))
print(bar(2))
print(bar(3))


# Keywords args

foo(a=5,y=4)


  # invalid
  
# foo(2,a=2)
# foo(x=2,2)