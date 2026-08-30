from collections import deque

stack = [1,2,3]
queue = deque(stack[:])

stack.append(4)

stack.append(5)

print(stack.pop())
print(stack.pop())



print("====dequeue======")

print(queue.popleft())
print(queue.popleft())