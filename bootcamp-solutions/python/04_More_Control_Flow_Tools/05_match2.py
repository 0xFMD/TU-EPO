class Vector:
        __match_args__ = ('x', 'y')
        def __init__(self,x,y):
                self.x=x
                self.y=y
                

v1= Vector(1,2)


match v1:
        case Vector(1,2):
                print("First case")
        case Vector(2,4):
                        print("second case")