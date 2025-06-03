# vortex

A small virtual machine (VM), aimed to be used via human-readable code.

## Architecture

The VM is register based, meaning that each operation performs on a set of registers. Compared to the stack based virtual machines, whose instructions operate over a stack, the register based VMs more closely mimic the actual implementation of computers. They are easier to optimize and generally are able to perform better.

The current VM implementation although aimed to be primarily used in the form of code scripts, can be integrated into other users' programs via programatically generating instruction objects and passing them to a `Vm` object instance.
