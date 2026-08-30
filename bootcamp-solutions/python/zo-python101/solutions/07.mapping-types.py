car = {
    "brand": "Koenigsegg",
    "model": "Jesko",
    "year": 2025
}


print(car["brand"])
print(car["model"])
print(car["year"])


car["color"] = "black"

car.pop("brand")

print(car)