from bcc import BPF
from bcc.utils import printb


prog = """
int hello(void *ctx) {
    bpf_trace_printk("Hello!\\n");
    return 0;
}
"""


b = BPF(text=prog)
b.attach_kprobe(event=b.get_syscall_fnname("clone"), fn_name="hello")

print("%-18s %-16s %-6s %s" % ("TIME(s)", "COMM", "PID", "MESSAGE"))

while 1:
    try:
        (task, pid, cpu, flags, ts, msg) = b.trace_fields()
    except ValueError:
        continue
    except KeyboardInterrupt:
        exit()

    printb(b"%-18.9f %-16s %-6d %s" % (ts, task, pid, msg))
