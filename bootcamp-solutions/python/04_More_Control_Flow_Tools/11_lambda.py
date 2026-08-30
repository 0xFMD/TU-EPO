# anonymous function(lambdas)

print((lambda x,y:x+y)(2,3))


nums = [1,2,3,4,5,6]

print(*map(lambda num:num * 2,nums)) # map each item, then unpack