import subprocess
import re
import networkx as nx
import matplotlib.pyplot as plt

def parse_netstat(output):
    connections = re.findall(r'(\S+:\d+)\s+\S+\s+(\S+:\d+)', output)
    return connections

def generate_network_map(connections):
    # Create a directed graph using networkx
    G = nx.DiGraph()

    # Add edges to the graph based on the netstat output
    for source, target in connections:
        G.add_edge(source, target)

    # Draw the network graph
    pos = nx.spring_layout(G)
    nx.draw(G, pos, with_labels=True, font_weight='bold', node_size=700, node_color='skyblue', font_size=8, edge_color='gray', width=1, alpha=0.7)

    # Display the graph
    plt.show()

def main():
    # Run netstat command to get network connections
    netstat_output = subprocess.check_output(['netstat', '-an'], universal_newlines=True)

    # Parse netstat output
    connections = parse_netstat(netstat_output)

    # Generate and display the network map
    generate_network_map(connections)

if __name__ == "__main__":
    main()

