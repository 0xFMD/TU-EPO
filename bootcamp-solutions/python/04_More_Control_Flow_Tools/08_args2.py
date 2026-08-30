# Variadic functions

def foo(*args,x=1):
        for arg in args:
                print(arg)
                

def bar(**keywords):
        for k in keywords:
                print(k)
                
                

def baz(*args, **keywords):
        for arg in args:
                print(arg)
        print("-----Keyword------")
        for k in keywords:
                print(k)



foo("arg1","arg2","arg3")

bar(k1="keyword1",k2="keyword2",k3="keyword3")


print("\n\n\n")

baz("arg1","arg2","arg3",k1="keyword1",k2="keyword2",k3="keyword3")