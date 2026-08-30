matrix = [
    [1, 2, 3, 4],
    [5, 6, 7, 8],
    [9, 10, 11, 12],
]



transpose = [[matrix[row][col] for row in range(len(matrix))] for col in range(len(matrix[0]))]

print(transpose)


evens = [matrix[i][j] for i in range(len(matrix)) for j in range(len(matrix[0])) if matrix[i][j] % 2 == 0]


print(evens)

# for i in range(0,4):
#         for j in range(0,3):
#                 print(matrix[j][i])