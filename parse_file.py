with open("test.txt", "rt") as inpf:
    lines = [x.strip() for x in inpf.readlines()]

# Remove from "lines" empty lines and comments
lines = [x for x in lines if x != "" and x[0] != "#"]

# Split each line in two
pairs = [x.split() for x in lines]

for sensor, temp in sorted(pairs, key=lambda x: float(x[1])):
    print(f"{sensor:20} (T = {temp} K)")
