with open("test.bin", "wb+") as f:
        f.write(b"0123456789abcdef")
        
        print(f.tell())
        f.seek(5)
        print(f.read(1))

        f.seek(-3, 2)
        print(f.read(1))

        f.seek(0)
        print(f.read())
