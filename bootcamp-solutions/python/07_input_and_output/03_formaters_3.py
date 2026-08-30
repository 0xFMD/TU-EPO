vars_t = { k:str(v) for k,v in vars().items() }

msg = " ".join(f'{k}: ' + '{' + k + '};' for k in vars_t.keys())


print(msg.format(**vars_t))


# for i in range(1,5):
#         print(f"{i:2d} {i*i:3d}")



# for i in range(1,5):
#         print(repr(i).rjust(2),repr(i*i).rjust(3))
        

# print("5".zfill(6))