def foo(num1: int, num2: int) -> int:
    """Add two integers.

    Args:
        num1 (int): First number.
        num2 (int): Second number.

    Returns:
        int: The sum of num1 and num2.
    """
    return num1 + num2

print(foo(1,2))

print(foo.__doc__)