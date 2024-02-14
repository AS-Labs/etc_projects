#include <linux/sched.h>
#include <uapi/linux/ptrace.h>

struct tcp_event_t {
    u64 timestamp;
    u32 saddr;
    u32 daddr;
    u16 sport;
    u16 dport;
    u32 pid;
    u64 start_time;
};

BPF_HASH(connections, struct tcp_event_t, u64);

int trace_tcp_connect(struct pt_regs *ctx, struct sock *sk)
{
    struct tcp_event_t event = {};
    u64 pid_tgid = bpf_get_current_pid_tgid();

    event.pid = pid_tgid >> 32;
    event.saddr = sk->__sk_common.skc_rcv_saddr;
    event.daddr = sk->__sk_common.skc_daddr;
    event.sport = sk->__sk_common.skc_num;
    event.dport = sk->__sk_common.skc_dport;
    event.timestamp = bpf_ktime_get_ns();

    connections.update(&event, &event.timestamp);

    return 0;
}

int trace_tcp_disconnect(struct pt_regs *ctx, struct sock *sk)
{
    struct tcp_event_t event = {};
    u64 pid_tgid = bpf_get_current_pid_tgid();

    event.pid = pid_tgid >> 32;
    event.saddr = sk->__sk_common.skc_rcv_saddr;
    event.daddr = sk->__sk_common.skc_daddr;
    event.sport = sk->__sk_common.skc_num;
    event.dport = sk->__sk_common.skc_dport;

    u64 *start_time_ptr = connections.lookup(&event);
    if (start_time_ptr != 0) {
        event.start_time = *start_time_ptr;
        u64 end_time = bpf_ktime_get_ns();
        u64 duration = end_time - event.start_time;

        // Output the information in CSV format
        bpf_trace_printk("%lld,%u,%u,%u,%u,%llu,%llu\n",
                         event.pid, event.saddr, event.daddr,
                         event.sport, event.dport,
                         event.start_time, duration);
    }

    return 0;
}
