# libvirt-automount
This is a [libvirt hook](https://www.libvirt.org/hooks.html) that will automatically mount your file systems in /mnt/$name This code makes a lot of assumptions, but can be quickly
changed to meet your needs.

# Problem
Running hooks to mount a remote file system after a libvirtd daemon sends in a `started` hook doesn't work right away, since `sshd` on the VM must 
be started, and that doesn't happen as soon as libvirtd sends this hook.

# Solution
Use the [`started`](https://www.libvirt.org/hooks.html#qemu) hook to launch a script that calls into a C program that immediately forks, and exists the parent. The C program
then calls `execlp('qemu-start-wrapped')`;

# Assumptions
- The name of your VM, which is passed to your hook from libvirtd has a resolved name. For example, virtual machine with name (domname) `dns`, can be resolved on your machine. This can be accomplished by simply editing your `/etc/hosts` file.

# Quick start
To get started, edit `mount-sshfs.c`

Ensure up SSH key validation on the virtual machine, password login is not supported
    
  1. `git clone https://github.com/rmccullagh/libvirt-automount`
  2. `cd libvirt-automount`
  3.  Edit [qemu-start-wrapped](https://github.com/rmccullagh/libvirt-automount/blob/master/qemu-start-wrapped#L34) and replace `ryan` and `IdentityFile` with correct values
  4. `gcc -Wall -Wextra mount-sshfs.c -o mount-sshfs`
  5. `sudo cp mount-ssfs /etc/libvirt/hooks/` 
  6. `sudo cp qemu /etc/libvirt/hooks`
  7. `sudo cp qemu-start-wrapped	 /etc/libvirt/hooks`
  8. `sudo systemctl restart libvirtd.service`
  
  
 You can tail -f `/var/log/syslog` to see the messages written. 
