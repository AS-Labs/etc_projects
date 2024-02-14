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
        pid = row['PID']
        comm = row['COMM']
        laddr = row['LADDR']
        raddr = row['RADDR']
        lport = row['LPORT']
        rport = row['RPORT']
        tx_kb = row['TX_KB']
        rx_kb = row['RX_KB']

        source_node = f"{laddr}:{lport}"
        target_node = f"{raddr}:{rport}"

        # Increase the count for source and target nodes
        G.add_node(source_node, pid=pid, comm=comm)
        G.add_node(target_node)

        # Add edge with attributes
        G.add_edge(source_node, target_node, tx_kb=tx_kb, rx_kb=rx_kb)

    # Draw the network graph with labels and edge attributes
    pos = nx.spring_layout(G)
    labels = {node: f"{node}\nPID: {G.nodes[node].get('pid', '')}\nCOMM: {G.nodes[node].get('comm', '')}" for node in G.nodes}
    edge_labels = {(source, target): f"TX: {G.edges[source, target]['tx_kb']} KB\nRX: {G.edges[source, target]['rx_kb']} KB" for source, target in G.edges}

    nx.draw(G, pos, with_labels=True, labels=labels, font_weight='bold', node_size=700, node_color='skyblue', font_size=8, edge_color='gray', width=1, alpha=0.7)
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels)

    # Display the graph
    plt.show()

if __name__ == "__main__":
    csv_file_path = './netstat.csv'  # Replace with your CSV file path
    generate_network_map(csv_file_path)

