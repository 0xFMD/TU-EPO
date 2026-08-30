sources = {"main.c", "parser.c", "lexer.c", "util.c"}
tracked = {"main.c", "parser.c", "main.h", "README.md"}


print(sources - tracked)
print(sources | tracked)
print(sources & tracked)
print(sources ^ tracked)


print(sources.union(["build.sh"]))
print(sources.intersection(["main.c", "app.py"]))

sources & ["main.c"]


print({"main.c"} <= sources)    # subset
print(sources > {"main.c"})


empty = set() # {} builds an empty dict, not a set
print(empty, type(empty), type({}))


# items must be hashable, so no lists inside

{["main.c"]}
