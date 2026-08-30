files = {
    "main.py": 120,
    "lexer.py": 340,
    "parser.py": 280
}

print(files["lexer.py"])

files["vm.py"] = 720

files["main.py"] = files["main.py"] * 2


for f,s in files.items():
        print(f"File name: {f}, Size: {s}")