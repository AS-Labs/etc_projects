#include <linux/ptrace.h>
#include <gnu/stubs-32.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <stdint.h>

// define the sys_enter section, where we want to attach for the tracepoint.
SEC("tracepoint/syscalls/sys_enter")
int BPF_PROG(sys_trace, struct pt_regs *ctx) {
    // Get syscall id from context (register). first arg in a syscall is in RDI.
    // RDI: part of the 64bit EISA and is used to store function arguments, function return values and other data during execution.
    uint64_t id = PT_REGS_PARM1(ctx);

    // Create the buffer to store the current process name
    char comm[16];

    // Get the process name and store it in the comm buffer.
    bpf_get_current_comm(comm, sizeof(comm));

    // Print using BPF printk function
    bpf_printk("Syscall %d from process %s\n", (uint32_t)id, comm);

    return 0;
}

// GPL license
char _license[] SEC("license") = "GPL";
