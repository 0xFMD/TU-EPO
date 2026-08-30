def std_arg(arg):
        print(arg)
        

def pos_only_arg(arg, /):
        print(arg)

def kw_only_arg(*, arg):
        print(arg)

def combined(pos_only, /,std, *, kw_only):
        print(pos_only,std,kw_only)
        
        
# pos_only_arg(arg=2)
# kw_only_arg(2)

combined(1,2,kw_only=3)