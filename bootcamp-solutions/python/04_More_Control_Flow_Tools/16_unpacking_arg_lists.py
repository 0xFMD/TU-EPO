bounds = [0, 16]
print(list(range(*bounds)))        


def instruction(op, dst, src):
        print(f"{op} {dst}, {src}")


parts = ["ADD", "R1", "R2"]
instruction(*parts)


def connect(host, port, debug=False):
        print(f"host={host} port={port} debug={debug}")


config = {"host": "localhost", "port": 8080}
connect(**config)



def trace(*args, **keywords):
        print(args, keywords)


trace(*parts, **config)


op, *operands = parts
print(op, operands)

*head, last = parts
print(head, last)


print([*parts, "EOF"])
print({**config, "port": 9000})
