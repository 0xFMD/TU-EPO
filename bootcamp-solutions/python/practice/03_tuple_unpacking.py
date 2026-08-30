packet = ("192.168.1.5", 8080, "TCP")

ip, port, protocol = packet

print(f"{protocol}://{ip}:{port}")


process = (3214, "python", "running", 42)

_, pName, *_, pUsage = process

print(pName,pUsage)