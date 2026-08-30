t1 = 'This is text (string are immutable)' # t1[0] = 't' is wrong

t2 = "This is also text"
t3 = r"use '\' to escape the quotes" # raw string, which ignores escape sequences

tb = ('Multi line text '
      "here also") # or use """ your text """



print(t1[0]+t2[1:4]) # concat + slice(end is exclusive)
print(t1[0:4]*2) # slice + repeat
print(t1[-2]) # second char from last
print(t3)
print(tb)