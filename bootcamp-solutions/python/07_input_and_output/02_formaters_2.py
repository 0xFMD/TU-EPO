print("First Name: {0}, Last Name: {1}".format("F","M"))
print("First Name: {1}, Last Name: {0}".format("F","M"))
print("First Name: {fName}, Last Name: {lName}".format(fName="F",lName="M"))


file_types = {
        "ts":"TypeScript",
        "py":"Python"
};


print("file-1: {0[ts]}, file-2: {0[py]}".format(file_types))
print("file-1: {ts}, file-2: {py}".format(**file_types))

