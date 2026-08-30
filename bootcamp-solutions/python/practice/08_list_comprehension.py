nums = [1, 2, 3, 4, 5, 6, 7, 8]

print([i * 2 for i in nums if i % 2 == 0])


matrix = [
    [1, 2, 3],
    [4, 5, 6,10, 11],
    [7, 8, 9, 12]
]

even_matrix = [ matrix[i][j] for i in range(len(matrix)) for j in range(len(matrix[i])) if matrix[i][j] % 2 == 0 ]

print(even_matrix)


matrix = [
    [1, 2, 3, 4],
    [5, 6, 7, 8],
    [9, 10, 11, 12]
]

transpose = [[matrix[j][i] for j in range(len(matrix))]
             for i in range(len(matrix[0]))]

print(transpose)