import pandas as pd
import networkx as nx
import matplotlib.pyplot as plt

def generate_network_map(csv_file):
    # Read CSV file into a pandas DataFrame
    df = pd.read_csv(csv_file)

    # Create a directed graph using networkx
    G = nx.DiGraph()

    # Add edges to the graph based on the CSV data
    for index, row in df.iterrows():
        source_node = row['Source']
        target_node = row['Target']
        weight = row.get('Weight', 1)  # Default weight to 1 if not specified
        G.add_edge(source_node, target_node, weight=weight)

    # Draw the network graph
    pos = nx.spring_layout(G)  # You can choose different layout algorithms
    nx.draw(G, pos, with_labels=True, font_weight='bold', node_size=700, node_color='skyblue', font_size=8, edge_color='gray', width=1, alpha=0.7)

    # Display the graph
    plt.show()

if __name__ == "__main__":
    csv_file_path = './netstat.csv'  # Replace with your CSV file path
    generate_network_map(csv_file_path)

