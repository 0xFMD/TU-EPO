def connect(host = "localhost", port = 8080, protocol="TCP"):
        print(f"{protocol}://{host}:{port}")
        
        
        
connect()
connect("192.168.1.5")
connect(port=3000)
connect("192.168.1.5", protocol="UDP")



def send(msg="",*,retries=3):
        print(f"{msg}")
        
        
send("hello", retries=3)


def sum_all(*args):
       return sum(args)


print(sum_all(1, 2, 3, 4))


def show_config(**args):
        for k,v in args.items():
                print(f"{k} = {v}")
                
                
show_config(host="localhost", port=8080, debug=True)