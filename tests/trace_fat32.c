#include <uapi/linux/ptrace.h>
#include <linux/fs.h>
#include <linux/sched.h>

BPF_HASH(file_ops, u64, u64);

int trace_open(struct pt_regs *ctx, struct file *file) {
    u64 pid = bpf_get_current_pid_tgid();
    u64 *count = file_ops.lookup(&pid);
    if (count) {
        (*count)++;
    } else {
        file_ops.update(&pid, &count);
    }
    return 0;
}
