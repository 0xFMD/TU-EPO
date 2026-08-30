file_types = {
        ".ts":"TypeScript",
        ".py":"Python"
};

print(file_types[".py"])
print(file_types[".ts"])

print(list(file_types))
print(sorted(list(file_types)))
print(list(file_types.values()))


http_status = [(200,"OK"),(403,"Forbidden"),(404,"Not Found"),(500,"Server Error")] 

print(http_status)
print(dict(http_status))


print( { i : i * 2 for i in range(1,6) } )


config = dict(host="localhost", port=8080, debug=True)

print(config)