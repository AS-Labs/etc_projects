#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

// Define BPF maps
struct bpf_map_def SEC("maps") tcp_connections = {
    .type = BPF_MAP_TYPE_LRU_HASH,
    .key_size = sizeof(__u32), // Use IP addresses as keys
    .value_size = sizeof(struct tcp_connection_info),
    .max_entries = 10240,
};

struct tcp_connection_info {
    __u64 start_time;
    __u64 end_time;
    __u32 src_ip;
    __u32 dst_ip;
    __u16 src_port;
    __u16 dst_port;
};

// Probe functions for connection events
SEC("kprobe/tcp_v4_connect")
int kprobe__tcp_v4_connect(struct pt_regs *ctx) {
    __u32 src_ip = (__u32) PT_REGS_PARM1(ctx);
    __u32 dst_ip = (__u32) PT_REGS_PARM2(ctx);
    __u16 src_port = (__u16) PT_REGS_PARM3(ctx);
    __u16 dst_port = bpf_ntohs(PT_REGS_PARM4(ctx));

    struct tcp_connection_info info = {
        .start_time = bpf_ktime_get_ns(),
        .src_ip = src_ip,
        .dst_ip = dst_ip,
        .src_port = src_port,
        .dst_port = dst_port,
    };

    bpf_map_update_elem(&tcp_connections, &src_ip, &info, BPF_ANY);
    return 0;
}

SEC("kprobe/tcp_v4_close")
int kprobe__tcp_v4_close(struct pt_regs *ctx) {
    __u32 src_ip = (__u32) PT_REGS_PARM1(ctx);
    __u32 dst_ip = (__u32) PT_REGS_PARM2(ctx);

    struct tcp_connection_info *info;
    info = bpf_map_lookup_elem(&tcp_connections, &src_ip);
    if (info) {
        info->end_time = bpf_ktime_get_ns();
        // Calculate latency
        __u64 latency = info->end_time - info->start_time;
        // Trigger perf event with latency data
        bpf_perf_event_output(ctx, &tcp_connections, BPF_F_CURRENT_CPU, &latency, sizeof(latency));
    }
    return 0;
}
