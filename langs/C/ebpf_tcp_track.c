#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/pkt_cls.h>
#include <linux/skbuff.h>

#define MAX_TCP_STREAMS 1000

// Define BPF map to store timestamp of SYN packets
BPF_MAP_DEF(syn_timestamps, ARRAY, uint32_t, u64, MAX_TCP_STREAMS);

// Define BPF map to store latency of completed TCP connections
BPF_MAP_DEF(latency_map, HASH, u64, u64, MAX_TCP_STREAMS);

SEC("socket")
int stream_tracer(struct __sk_buff *skb) {
    u8 *cursor = 0;

    // Parse Ethernet header
    struct ethhdr *eth = (struct ethhdr *)(cursor);
    cursor += sizeof(struct ethhdr);

    // Parse IP header
    struct iphdr *ip = (struct iphdr *)(cursor);
    if (ip->protocol != IPPROTO_TCP)
        return TC_ACT_OK;  // Not TCP packet

    cursor += sizeof(struct iphdr);

    // Parse TCP header
    struct tcphdr *tcp = (struct tcphdr *)(cursor);

    // Retrieve source and destination port
    uint16_t src_port = ntohs(tcp->source);
    uint16_t dst_port = ntohs(tcp->dest);

    // Check if packet is SYN (SYN flag is set and ACK flag is not set)
    if ((tcp->syn == 1) && (tcp->ack == 0)) {
        // Store current timestamp for SYN packet
        u64 timestamp = bpf_ktime_get_ns();
        bpf_map_update_elem(&syn_timestamps, &src_port, &timestamp, BPF_ANY);
    }

    // Check if packet is ACK (ACK flag is set)
    if (tcp->ack == 1) {
        // Retrieve timestamp of corresponding SYN packet
        u64 *syn_timestamp = bpf_map_lookup_elem(&syn_timestamps, &dst_port);
        if (syn_timestamp) {
            // Calculate latency for completed TCP connection
            u64 latency = bpf_ktime_get_ns() - *syn_timestamp;
            // Store latency in latency map
            bpf_map_update_elem(&latency_map, &dst_port, &latency, BPF_ANY);
            // Remove SYN timestamp from map
            bpf_map_delete_elem(&syn_timestamps, &dst_port);
        }
    }

    return TC_ACT_OK;
}

char _license[] SEC("license") = "GPL";

