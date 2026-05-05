class Field:
    def __init__(self, type, start, end):
        self.type = type
        self.start = start
        self.end = end
        self.mask = (1<<self)


class BitField:
    def __init__(self, blockSize, fields):
        self.blockSize = blockSize
        self.fields = fields
        

