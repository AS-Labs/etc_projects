from bcc import BPF

BPF(text='int kprobe__sys_clone(void *ctx) { bpf_trace_printk("sys_clone system call was called!\\n"); return 0; } ').trace_print()
