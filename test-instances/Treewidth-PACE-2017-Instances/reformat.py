import os

directory_in_str : str = "."
directory = os.fsencode(directory_in_str)

def reformatGraphFile(filename : str):
    vertices : set[str] = set()

    input = open(filename, "r")
    output = open((filename + ".csv"), "w")
    lines = input.readlines()

    for line in lines[1:]:
        while (line.endswith('\n')):
            line = line[:-1]
        parts = line.split(" ")

        vertices.add(parts[0])
        vertices.add(parts[1])

        output.write(parts[0])
        output.write(",")
        output.write(parts[1])
        output.write("\n")
    
    for vertex in vertices:
        output.write(vertex + ",,1\n")
    
    input.close()
    output.close()

def reformatTdFile(filename : str):
    input = open(filename, "r")
    output = open((filename + ".csv"), "w")

    bags : map[str,list[int]] = {}
    edges : list[tuple[str,str]] = list()
    lines = input.readlines()
    
    for line in lines[1:]:
        while (line.endswith('\n')):
            line = line[:-1]
        parts = line.split(" ")
        if (line.startswith("b ")):
            # line describes contents of a bag
            node_id : str = parts[1]
            bags[node_id] = list(parts[2:])
        else:
            # line describes edge between two nodes
            edges.append((parts[0], parts[1]))

    for edge in edges:
        output.write(edge[0] + "," + edge[1] + "\n")
    
    for bag_name in bags:
        bag_content = bags[bag_name]
        line : str = bag_name + ",,"
        for node_id in bag_content:
            line += node_id +  ";"
        line = line[:-1]
        line += "\n"
        
        output.write(line)

    input.close()
    output.close()
    
for file in os.listdir():
    filename = os.fsdecode(file)
    if filename.endswith(".gr"):
        reformatGraphFile(filename)
    elif filename.endswith(".td"):
        reformatTdFile(filename)
    else:
        continue